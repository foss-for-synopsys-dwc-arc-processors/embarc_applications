#include "TskMotor.h"
#include "dbgUart.h"
#include "timer.h"
#include "imu.h"
#include "spi.h"
#include "FastMath_table.h"
#include "Queue/Queue.h"

#include "embARC.h"
#include "embARC_debug.h"

#include <math.h>
#include <stdlib.h>

/* 
 * motorTask Priority & stackSize 
 */
const int motorTskPrio = 6;
const int motorTskStkSize = 512;

/*
 * multi-task communication
 */
QueueHandle_t motMbCmd;

/** function control  */
bool AngEnabled = false;
bool AcqZerosEnabled = false;
bool MotorEnabled = false;
bool SeqSetEnabled = false;
bool PutStrEnabled = false;

/** for debug */
bool DEBUG_LQR = false;
bool PRINT_IMU = false;
bool PRINT_PWM = false;
bool PRINT_TIME = false;
/* 
 * print period, T = PRINT_PERIOD * 2ms
 * dynamically adjusted according to actual demand
 */
int PRINT_PERIOD = LPF_FACTOR;

/* 
 * Accl 1-order Lowpass Filter
 * y(n) = (1-a) * y(n-1) + a * x
 *           float         q1.7
 * set a  -1~0.99219 -> -128 ~ 127 
 * 
 * factor 128
 */
int AcclLPFParam = (int)(0.02f * LPF_FACTOR);

int SpdLPFParam = (int)(0.02f * LPF_FACTOR);
int AngLPFParam = (int)(2.f * LPF_FACTOR);
int GyroLPFParam = (int)(1.f * LPF_FACTOR);

/*
 * vertical status starting and termination threshold 
 * 
 * unit:rad factor 65536
 */
int ANGLEDIFF = (int)(0.08f * 2097152);
int ANGLIMIT = (int)(0.4f * 2097152);

/* kalman params, factor 65536 */
Kalman1Var AngleKal = {
    .Q = 20,            /** 0.0006 * 32768   */
    .R = 3244,          /** 0.099 * 32768    */
    .xpos = 0,
    .Ppos = 0
};

/** lqr params, factor 1*/
lqr_q lqr = {
    .k1 = -60,
    .k2 = -40,      //-40 for normal mode, -60 for 
    .k3 = 300,
    .k4 = 16
};

/* 
 * setpoint:
 * 			Velocity --> linear speed
 * 			Timeout  --> period of Velocity
 * 			Acc      --> linear acceleration
 * 			Omega 	 --> (y axial)pitch angular velocity
 * 			ThetaZ	 --> (z axial)yaw angle
 * 			ThetaY 	 --> (y axial)pitch angle
 * Factor:  65536
 * 
 * 			|<-  timeout  ->|
 * 		    -----------------
 * 		   /|				 \
 * 		  /	|Velocity	  	  \-----------> desire.Acc
 *	0m/s / 	|				   \  0m/s
 * -----/  -|-					\------   	 
 * 
 */
VelOmega desire = {
    .Velocity = 0,
    .Timeout = 0,
    .Acc = 1258291,           /** -0.6 * (1 << 21) */
    .Omega = 0,
    .ThetaZ = 0,
    .ThetaY = -1096181        /** -0.5227 * (1 << 21) */
};

/** pid params, factor 256, avoid overflow*/
Pid angPid = {
    .p = 140, 
    .i = 0, 
    .d = 0, 
    .n = 256,
    .accI = 0,
    .accD = 0       
};

/*
 * for atan LUT, index = SAT16((AcclX / AcclZ) << 7 + 192) (-1.5~0.45-> 0~255)
 *               accl2Ang = Table[index]
 *    ATTENTION: accl2Ang = real_angle << 10  
 */
extern int16_t accl2AngleIndex(ImuValues *imuValsV, int16_t *AcclX, int16_t *AcclZ, int16_t *LPFParam);

extern void kalmanPredict(Kalman1Var *kalVal, int32_t u);
extern int32_t kalmanCorrect(Kalman1Var *kalVal, int32_t z);
extern int32_t lqrCalc(lqr_q *lqr, int32_t pos, int32_t vel, int32_t ang, int32_t av);
extern int32_t pidTick(Pid *pid, int32_t diff);

void motorTask(void *pvParameters)
{
    BaseType_t rtn;
    MsgType msg;
    int32_t ercd;
    uint64_t start_us = 0, end_us = 0;

    int data[8];
    char cmdf[2] = {0x03, 0xFC};
    char cmdr[2] = {0xFC, 0x03};

    /** velocity timeout, default value: 65535 * 2ms, about 2 minutes*/
    int timeCnt = 0, timeout = 65535, tsCtl = 0;

    /** angle status check, if less than starting threshold 1s, enter upright mode*/
    int16_t status = 0;

    /*
     * -------------------------|---------------------------
     * 			gyro 			|			accl
     * -------------------------|---------------------------
     * unit: m/s^2 factor: 115	|	unit: m/s^2 factor: 115
     * -------------------------|---------------------------
     * 	angular Velocity 		|	linear acceleration		
     * 		 |	 				|			|
     *    	 |AVG Filter		|			|a_X/a_Y ratio
     *    	 |					|			|
     *  gyro bias				|	pitch angle tan value
     *    	 |					|			|
     *     	 |					|			|kalman Filter
     *     	 |					|			|
     *  gyro relative value	----|--->pitch angle
     *  ------------------------|---------------------------
     *  H -> Horizontal, V -> vertical
     */
    ImuValues imuValsH, imuValsV;
    int16_t staticCnt = 0, staticCntCyced = 0;
    int16_t GyroX = 0, GyroY = 0, AcclX = 0, AcclZ = 0;
    int16_t XBias = 164, ZBias = 2785;
    int16_t GyroXZero = 0, GyroYZero = 0;
    int32_t accl2Ang = 0, acclIndex = 0;
    int32_t gyroXZeroAcc = 0, gyroYZeroAcc = 0;
    int32_t angle1 = 0, angle = 0, lqrGyroY = 0;
    int16_t AngParam = 0, YawParam = 0;
    /*
     * speed and pos related values
     */
    Queue spd;
    Queue ang;
    int16_t qei[2] = {0, 0}, qei1Tmp = 0, pwm[2] = {0, 0};
    int32_t pwmDeal[2] = {0, 0};
    int 	angXout = 0, speed = 0, pos = 0, posDes = 0;
    int 	angTmp = 0, spdTmp = 0;
    int32_t lqrOut = 0, pidOut = 0;
    int posL = 0, posR = 0;
    putStr("MOT TIME:%s, %s\r\n",  __DATE__, __TIME__);

    /** initialize spi */
    ercd = cpld_spi_init();    
    configASSERT(ercd == E_OK);

    /** ensure motor stationary*/
    pwm[0] = pwm[1] = 0;
    ercd = spi_write_pwm(&pwm[0], 0);
    configASSERT(ercd == E_OK);
    ercd = spi_write_pwm(&pwm[1], 1);
    configASSERT(ercd == E_OK);

    /** initialize imu */
    ercd = imuInit(IMU_VERTICAL_IIC_ID);
    configASSERT(ercd == E_OK);

    /** accelerated sequence, to avoid mutation*/
    q_init_int(&spd, SPDSEQLENMAX);   

    /** yaw sequence, 2rad/s */
    q_init_int(&ang, ANGSEQLENMAX);

    /** lauch 2Ms tick interrupt */
    base_timer_init(); 

    vTaskDelay(10);

    while(1)
    {

    	/** semphore keep 2ms-loop */
     	rtn = xSemaphoreTake(SemMotTick, 2);
        configASSERT(rtn == pdPASS);
        
        if(PRINT_TIME)
            start_us = board_get_cur_us();
        

        /* 
         * read encder  
         *  -qei[0] -- R 
         *  qei[1]  -- L
         */
        ercd = spi_read_qei(&qei[0], 0);
        configASSERT(ercd == E_OK);
        ercd = spi_read_qei(&qei[1], 1);
        configASSERT(ercd == E_OK);

        /*
         * TODO:SPI exist data error
         * For avoid fault data
         */
        if(qei[1] > 2040) qei[1] = qei1Tmp;
        qei1Tmp = qei[1];

        /** read imu type:int16_t */
        ercd = imuGetValues(&imuValsV, IMU_VERTICAL_IIC_ID);
        configASSERT(ercd == E_OK);

        /** relative value = initial value - bias */
        GyroY = -imuValsV.angvY + GyroYZero;
        GyroX = imuValsV.angvX - GyroXZero;

        /** imu adj zeros, auto adj zeros if static 1.28s */
        if(AcqZerosEnabled)
        {
            staticCnt++;
            staticCntCyced = (staticCnt & 0x0FFF);  // cyced per 8.192s(@Ts=2ms)

            if(staticCntCyced == 127)               // desire static 254ms(@Ts=2ms), adj zero start
            {
                gyroYZeroAcc = 0;
                gyroXZeroAcc = 0;
            }
            else if(staticCntCyced >= 128 && staticCntCyced < 640)
            {
                gyroXZeroAcc += imuValsV.angvX;
                gyroYZeroAcc += imuValsV.angvY;
            }
            else if(staticCntCyced == 640)   // adj zero finish
            {
                GyroXZero = gyroXZeroAcc >> 9;
                GyroYZero = gyroYZeroAcc >> 9;
                putStr("Y%d, X%d\r\n",
                        GyroYZero, GyroXZero);
            }
        }
        else 
            staticCnt = 0;

        /** timeout for line velocity & yaw angle */
        if(timeout <= 0)
        {

            timeout = 65535;

            /** decelerated sequence */
            if(desire.Velocity != 0)
            {
                SeqSetEnabled = true;
                desire.Velocity = 0.f;  
            }
        }

        if(SeqSetEnabled)
        {
            SeqSetEnabled = false; 

        	/*
        	 * based on current value, recalculate relative sequence
        	 * update timeout
        	 */
            q_clr(&spd);
            q_clr(&ang);

        	ercd = calcSeq(&spd, spdTmp, desire.Velocity, desire.Acc);
       		configASSERT(ercd < SPDSEQLENMAX);
        
			ercd = calcSeq(&ang, angTmp, desire.ThetaZ, desire.Acc);
			configASSERT(ercd < ANGSEQLENMAX);

            if(desire.Timeout > 0)  timeout = desire.Timeout * TS_RECI >> 16;
        }

        if(AngEnabled)
        {
        	/* 
             * calculate angle 
             * LPF for accl, avoid shaking
             * change atan LUT to function, to a better accuracy
             * change kalman filter to dynamic complementary filter
             */
            AcclX = ((LPF_FACTOR - AcclLPFParam) * AcclX - AcclLPFParam * (imuValsV.acclX + XBias)) / LPF_FACTOR;
            AcclZ = ((LPF_FACTOR - AcclLPFParam) * AcclZ - AcclLPFParam * (imuValsV.acclZ + ZBias)) / LPF_FACTOR;
            accl2Ang = (int)(atan2f(AcclX, AcclZ) * ANG_FACTOR);

            AngParam = saturate(51 + AngLPFParam * abs(desire.ThetaY - accl2Ang) / 936  * abs(GyroY) / ANG_FACTOR, 128, -128);
            angle1 = ((LPF_FACTOR - AngParam) * accl2Ang + AngParam * (angle1  + GyroY * GYRO_FACTOR / TS_RECI)) / LPF_FACTOR;
            angle = angle1;;
        }

        if(MotorEnabled)
        {
            if(status < 499)
            {   
                
                /** check start-up status, 0.1rad * 2^21 */
                if(((desire.ThetaY - angle) < ANGLEDIFF) && ((angle - desire.ThetaY) < ANGLEDIFF))
                {
                    status++;
                }
                else status = 0;

                /** angle delta value < ANGLEDIFF keep 1s=2ms*500, start controller*/
                if(status == 499)
                {
                    putStr("ON\r\n");
                    //AcclX = 0;
                    //AcclZ = 0;
                    //angle1 = 0;
                    angXout = 0;
                    speed = 0;
                    pos = 0;
                    posDes = 0;
                    spdTmp = 0;
                    angTmp = 0;
                    //lqrGyroY = 0;
                    SeqSetEnabled = true; 
                }
            }

            else
            {   
                timeout--;

                /** due to sensor question, through LR wheel difference value to obtain yaw angle */
                /** accumulate yaw angle, delta theta = s(L - R) / W, 6250 = 1 / T / W */
                YawParam = saturate(100 + 500 * abs(desire.ThetaY - angle) / ANG_FACTOR, 128, -128);
                angXout += ((YawParam * (-qei[1] * ENCL_FACTOR - qei[0] * ENCR_FACTOR) / 185) + (LPF_FACTOR - YawParam) * (GyroX * GYRO_FACTOR / TS_RECI)) / LPF_FACTOR;

                /** calculate current speed, Low-pass filter*/
                speed = ((LPF_FACTOR - SpdLPFParam) * speed + SpdLPFParam * (qei[1] * ENCL_FACTOR - qei[0] * ENCR_FACTOR) / 2) / LPF_FACTOR;
                /** accumulate actual position */
                pos += (qei[1] * ENCL_FACTOR - qei[0] * ENCR_FACTOR)/ TS_RECI / 2;
                /** dequeue, if queue if empty, keep final value*/
                q_de_int(&spd, &spdTmp);
                q_de_int(&ang, &angTmp);

                /** calcilate setting position */
                posDes += spdTmp / TS_RECI; 

                pidOut = pidTick(&angPid, angTmp - angXout);

                lqrGyroY = ((LPF_FACTOR - GyroLPFParam) * lqrGyroY + GyroLPFParam * GyroY * GYRO_FACTOR) / LPF_FACTOR;

                lqrOut = lqrCalc(&lqr,
                                 // saturate(posDes - pos, 65000, -65000),
                                 posDes - pos,
                                 spdTmp - speed,
                                 desire.ThetaY - angle,
                                 desire.Omega - lqrGyroY
                                );

                if(((desire.ThetaY - angle) > ANGLIMIT) || ((angle - desire.ThetaY) > ANGLIMIT))
                {
                    /** avoid reset pwm fail */
                    if(status < 600)
                    { 
                        lqrOut = lqrOut / (status - 499);
                        pidOut = pidOut / (status - 499);
                        status++;
                    }
                    else
                    {
                        lqrOut = 0;
                        pidOut = 0;
                        status++;
                        if(status == 610)
                            status = 0;
                    }
                }

                /* 
                 * (lqrOut >> (k_factor + 5)) / battery voltage   -->   normalization 
                 * pidOut range[-1, 1] 
                 * pwm = lqrOut/ Vref + pidOut
                 * saturate pwm to [-65000, 65000], equal to [-0.99, 0.99] << 16
                 * pwm >> 5 to be 12 bits
                 */
                pwmDeal[0] = saturate((lqrOut >> 10) - pidOut , 64200, -64200);
                pwmDeal[1] = saturate((lqrOut >> 10) + pidOut , 64200, -64200);
                Asm("asrsr %0, %1, %2" : "=r"(pwm[0]) : "r"(pwmDeal[0]), "r"(5));
                Asm("asrsr %0, %1, %2" : "=r"(pwm[1]) : "r"(pwmDeal[1]), "r"(5));

                ercd = spi_write_pwm(&pwm[0], 0);
                configASSERT(ercd == E_OK);
                ercd = spi_write_pwm(&pwm[1], 1);
                configASSERT(ercd == E_OK);
                //tsCtl = 0;
            }
        }

        if(PutStrEnabled){
            if(timeCnt > PRINT_PERIOD){
                /* 
                 * MPU & PC protocol 
                 * cmdf data cmdr
                 * [0x03 0xFC ... 0xFC 0x03]
                 * data can't change to ASCII code  
                 */
                putData(cmdf, 2);
                /* approximately 1024 timers large*/
                if(DEBUG_LQR){
                    data[0] = angle >> 11;
                    data[1] = GyroY;
                    data[2] = pos >> 11;
                    data[3] = speed >> 11;
                    data[4] = lqrOut >> 12; 
                    data[5] = angXout >> 11;
                    data[6] = pidOut;
                    data[7] = end_us;
                    putData(data, 8 * sizeof(int));     
                }

                else if(PRINT_PWM){                    
                    data[0] = qei[1] * ENCL_FACTOR >> 11;
                    data[1] = -qei[0] * ENCR_FACTOR >> 11;
                    data[2] = pwm[0];
                    data[3] = pwm[1];
                    data[4] = spdTmp >> 11; 
                    data[5] = angTmp >> 11;
                    data[6] = timeout;
                    data[7] = end_us;
                    putData(data, 8 * sizeof(int));     
                }

                else if(PRINT_IMU){                    
                    data[0] = GyroY;
                    data[1] = GyroX;
                    data[2] = AcclX >> 1;
                    data[3] = AcclZ >> 1;
                    data[4] = accl2Ang >> 11; 
                    data[5] = angle >> 11;
                    data[6] = AngParam;
                    data[7] = end_us;
                    putData(data, 8 * sizeof(int));     
                }

                putData(cmdr, 2);

                timeCnt = 0;
            }
            timeCnt++;

        }

        if(xQueueReceive(motMbCmd, &msg, (TickType_t)0))
        {
            switch(msg & 0xFFFF0000)
            {
            case EnableMotors:
                spdTmp = 0.f;
                angTmp = 0.f;
                MotorEnabled = true;
                break;

            case DisableMotors:
                // write pwm 0
                pwm[0] = 0;
                pwm[1] = 0;
                ercd = spi_write_pwm(&pwm[0], 0);
                configASSERT(ercd == E_OK);
                ercd = spi_write_pwm(&pwm[1], 1);
                configASSERT(ercd == E_OK);

                angPid.accI = 0;
                angPid.accD = 0;

                AngleKal.xpos = 0;
                AngleKal.Ppos = 0;

                status = 0;
                MotorEnabled = false;
                break;

            case EnableAng:
                AngEnabled = true;
                break;
            case DisableAng:
                AngEnabled = false;
                break;
            case EnableAcqZeros:
                AcqZerosEnabled = true;
                break;
            case DisableAcqZeros:
                AcqZerosEnabled = false;
                break;
            case EnableSeqSet:
                SeqSetEnabled = true;
                break;
            case DisableSeqSet:
                SeqSetEnabled = false;
                break;
            case EnablePutStr:
                PutStrEnabled = true;
                break;
            case DisablePutStr:
                PutStrEnabled = false;
                break;
            default:
                break;
            }
        }

        if(PRINT_TIME)
            end_us = board_get_cur_us() - start_us; 
    }

}

void motorInit()
{

	BaseType_t rtn;

    motMbCmd = xQueueCreate(4, sizeof(MsgType));
    configASSERT(motMbCmd);

    // Create tasks
    rtn=xTaskCreate(motorTask, "motorTask", motorTskStkSize, NULL, motorTskPrio, NULL);
    configASSERT(rtn==pdPASS);

}