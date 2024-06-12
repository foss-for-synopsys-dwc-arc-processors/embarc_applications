#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Spotter_IC.h"
#include "recorder.h"
#include "mpu9250.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "conf.h"

//全域變數
LED_MODE led = LED_DARK;
static DEV_GPIO_PTR gpio_moter,gpio_led,pwm;
static MOV_DRECT mov_dir;

static uint32_t switchway = 0;
static uint32_t timepwmcount = 0;
static uint32_t timempucount=0;
static int gpID,cmdID;
static uint32_t in1,in2,in3,in4;
static int Enc1=0,Enc2=0;
MPU9250_DATA mpu9250_data = { 0 };
/**/
static int Rtarget_Enc;
static int Ltarget_Enc;
/**/
static int Loutput=1;
static int Routput=1;
/**/
static float yal=0;
float TargetDeg=0;
/**/
int Turn_out=0;
int MAX_PWM=100;
int mapflag=0,recodering_flag=0,TurnAro_flag=0;
VOICE_CMD map1[10]={CMD_GO_FRONT,CMD_TURN_LEFT_DEG_90,CMD_TURN_LEFT_DEG_90,CMD_TURN_RIGHT_DEG_90,CMD_TURN_RIGHT_DEG_90};
VOICE_CMD map0[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
static int Enc1Count=0,Enc2Count=0,preEnc1=0,preEnc2=0;
static WorkStatus work_st = RECON_VOICE;//reconition
int Blocked=0;

static void GPIO_INIT();
static void TIMER_INIT();
static void prr(int);
static void moterPWM(void *ptr);
static MPU9250_DEFINE(mpu9250_sensor, MPU9250_IIC_ID, MPU9250_IIC_ADDRESS);
static void right_rotation(int dir,int pwm_1);
static void left_rotation(int dir,int pwm_2);
static void check_Enc();
static void Turn(int );
static void PRINT_WK_ST();

void Map_Inverter(VOICE_CMD*,int);

static int doRecognize();
void mpu9250(float*);
#include "CYBase.mod.h"
#include "Group_1.mod.h"
#include "Group_2.mod.h"

int main(int argc, char **argv){
	//float yal;
	gpio_moter=gpio_get_dev(IOTDK_MOTER_ID);
	gpio_led=gpio_get_dev(IOTDK_LED_ID);
	GPIO_INIT();
	mpu9250_sensor_init(mpu9250_sensor);
	//timer1
	TIMER_INIT();
	
	int nRet;
	
	printf("CSpotter version: %s\r\n", Spotter_GetVersion());
	
	nRet= Spotter_Init2((void*)pCYBase_mod, (const void*[]){pGroup_1_mod, pGroup_2_mod, 0}, 0); 
	if(nRet!= 0){
		goto Exit;
	}
	
	while(1){
			if(work_st==RECON_VOICE){
				Spotter_Reset();
				doRecognize();
			}
			else if( work_st == EXE_CMD){
				mpu9250(&yal);
				if(preEnc1==Enc1){
					Enc1Count++;
				}
				else{
					preEnc1=Enc1;
					Enc1Count=0;
				}
				if(preEnc2==Enc2){
					Enc2Count++;	
				}
				else{
					preEnc2=Enc2;
					Enc2Count=0;
				}
				if(Blocked){
					Enc2Count=0;
					Enc1Count=0;
				}
				if( Enc1Count > 100 && Enc2Count > 100 ){
					
					if(recodering_flag){
						mapflag++;
					}
					if(TurnAro_flag==1){
						mov_dir = MOV_TURN_LEFT_90;
						TargetDeg = yal + DEG_90;
						TurnAro_flag++;
					}else if(TurnAro_flag==2){
						mov_dir = MOV_TURN_LEFT_90;
						TargetDeg = yal + DEG_90;
						TurnAro_flag=0;
					}
					else if(TurnAro_flag==0){
						mov_dir=MOV_STOP;
						work_st=RECON_VOICE;
					}
					Enc2Count=0;
					Enc1Count=0;
				}
					
			}
			else if( work_st == BACK2HOME){
				mpu9250(&yal);
				//EMBARC_PRINTF("MP_F %d mov %d\n",mapflag,map1[mapflag]);
				if(preEnc1==Enc1){
					Enc1Count++;
				}
				else{
					preEnc1=Enc1;
					Enc1Count=0;
				}
				
				if(preEnc2==Enc2){
					Enc2Count++;	
				}
				else{
					preEnc2=Enc2;
					Enc2Count=0;
				}
				if(Blocked){
					Enc2Count=0;
					Enc1Count=0;
				}

				if( Enc1Count > 100 && Enc2Count > 100 ){
					if(TurnAro_flag==1){
						mov_dir = MOV_TURN_LEFT_90;
						TargetDeg = yal + DEG_90;
						TurnAro_flag++;
					}
					else if(TurnAro_flag==2){
						mov_dir = MOV_TURN_LEFT_90;
						TargetDeg = yal + DEG_90;
						TurnAro_flag=0;
					}
					else{
				switch(map1[mapflag]){
					case CMD_TURN_RIGHT_DEG_45:	
						break;
					case CMD_TURN_RIGHT_DEG_90:
						//mpu9250(&yal);
						mov_dir = MOV_TURN_RIGHT_90;
						TargetDeg = yal - DEG_90;
						break;
					case CMD_TURN_LEFT_DEG_45:
						break;
					case CMD_TURN_LEFT_DEG_90:
						//mpu9250(&yal);
						mov_dir = MOV_TURN_LEFT_90;
						TargetDeg = yal + DEG_90;
						break;
					case CMD_STOP:
						mov_dir = MOV_STOP;
						break;
					case CMD_GO_FRONT:
						mov_dir = MOV_GO_FRONT;
						Ltarget_Enc = Enc2+1000;
						Rtarget_Enc = Enc1+1000;
						break;
					case CMD_TURN_AROUND:
						//mpu9250(&yal);
						TurnAro_flag=1;
						break;
					case -1:
					default:
						mov_dir=MOV_STOP;
						work_st=RECON_VOICE;
						mapflag=0;
						break;
				}
				mapflag++;
				}
				Enc2Count=0;
				Enc1Count=0;				
					
				}
			}
			else if(work_st == RECORD_MAP){
				recodering_flag=1;
				mapflag=0;
				//EMBARC_PRINTF("MP_F %d\n",recodering_flag);
				mov_dir=MOV_STOP;
				work_st=RECON_VOICE;
			}

			//EMBARC_PRINTF("Routput= %d taskfin= %d targetEnc = %d NowEnc = %d \n",Routput,finish_task,Rtarget_Enc,Enc1);	
			//EMBARC_PRINTF("Lout= %d Enc2= %d\r\n",Loutput,Enc2);
			//EMBARC_PRINTF("DDD\n");
			//PRINT_WK_ST();
			//EMBARC_PRINTF_FlOAT("Tar",TargetDeg);
			//EMBARC_PRINTF_FlOAT("yal",yal);

		
	}
	printf("Done\r\n");
Exit:
 	Spotter_Release();
	exit(0);
	return 0;
}


static void moterPWM(void *ptr){
	timer_int_clear(TIMER_1);
	switch(mov_dir){
		case MOV_STOP:
		//EMBARC_PRINTF("way %d\r\n",mov_dir);
			break;
		case MOV_GO_FRONT://前;
			right_rotation(0,Routput);
			left_rotation(0,Loutput);
			break;
		case MOV_TURN_LEFT_90://左
			Turn(DIR_LEFT);			
			break;
		case MOV_TURN_RIGHT_90://右
			Turn(DIR_RIGHT);	
			break;
		case MOV_TURN_AROUND:
			//Turn(DIR_LEFT);	
			break;
		default:
			break;
	}
		
	gpio_led->gpio_read(&Blocked, 1 << IOTDK_LED_1);

	if (timepwmcount == 250||Blocked){
			gpio_moter->gpio_write(0<< IOTDK_PWM_1_0, 1 << IOTDK_PWM_1_0);
			gpio_moter->gpio_write(0<< IOTDK_PWM_1_1, 1 << IOTDK_PWM_1_1);
			gpio_moter->gpio_write(0<< IOTDK_PWM_2_0, 1 << IOTDK_PWM_2_0);
			gpio_moter->gpio_write(0<< IOTDK_PWM_2_1, 1 << IOTDK_PWM_2_1);
			timepwmcount = 0;
	}

	switch(led){
		case 0:
			gpio_led->gpio_write(0<< IOTDK_LED_0, 1 <<IOTDK_LED_0);
			break;
		case 1:
			gpio_led->gpio_write(1<< IOTDK_LED_0, 1 <<IOTDK_LED_0);
			break;
		case 2:	
			gpio_led->gpio_write(0<< IOTDK_LED_0, 1 <<IOTDK_LED_0);
			break;
	}
		
	
	gpio_moter->gpio_read(&in4, 1 << IOTDK_ENC_2_1);
	gpio_moter->gpio_read(&in2, 1 << IOTDK_ENC_1_1);
	if(!in1){
		gpio_moter->gpio_read(&in1, 1 << IOTDK_ENC_1_0);
		if(in1)
			if(in2)
				Enc1++;
			else
				Enc1--;
	}
	else
	{
		gpio_moter->gpio_read(&in1, 1 << IOTDK_ENC_1_0);
	}
	
	if(!in3){
			gpio_moter->gpio_read(&in3, 1 << IOTDK_ENC_2_0);
		if(in3)
			if(in4)
				Enc2++;
			else
				Enc2--;
			
	}
	else{
		gpio_moter->gpio_read(&in3, 1 << IOTDK_ENC_2_0);
	}
	check_Enc();
	timepwmcount++;

}
static int doRecognize(){
	int nRet;
	nRet= Recorder_Init();
	int16_t *pData;
	if(nRet!= 0){
		printf("record init fail: %d\r\n", nRet);
		return 1;
	}
	Spotter_Reset();
	Recorder_Start();
	while(work_st==RECON_VOICE){

		//EMBARC_PRINTF("[1]= %d [2]= %d \r\n",Enc1,Enc2);
		//EMBARC_PRINTF("mov_dir is %d\r\n",mov_dir);
		pData= Recorder_GetBuffer();
		if(pData== 0) continue;

		nRet= Spotter_AddSample(pData, Recorder_GetBufferSize());
		if(nRet== CSPOTTER_SUCCESS){
			gpID=Spotter_GetResultGroupId();
			cmdID=Spotter_GetResultId();
			printf("CSpotter detect result gpID=%d cmdID=%d\n", gpID, cmdID);
			if(gpID){
				switch(cmdID){
					case CMD_TURN_RIGHT_DEG_45:
						
						break;
					case CMD_TURN_RIGHT_DEG_90:
						//mpu9250(&yal);
						if(recodering_flag)
							map1[mapflag]=CMD_TURN_RIGHT_DEG_90;
						mov_dir = MOV_TURN_RIGHT_90;
						TargetDeg = yal - DEG_90;
						break;
					case CMD_TURN_LEFT_DEG_45:
						break;
					case CMD_TURN_LEFT_DEG_90:
						//mpu9250(&yal);
						if(recodering_flag)
							map1[mapflag]=CMD_TURN_LEFT_DEG_90;
						mov_dir = MOV_TURN_LEFT_90;
						TargetDeg = yal + DEG_90;
						break;
					case CMD_BACK2HOME:
						recodering_flag=0;
						work_st = BACK2HOME;
						TurnAro_flag=1;
						Map_Inverter(map1,mapflag);
						mapflag=0;
						break;
					case CMD_STOP:
						mov_dir = MOV_STOP;
						break;
					case CMD_GO_FRONT:
						if(recodering_flag)
							map1[mapflag]=CMD_GO_FRONT;
						mov_dir = MOV_GO_FRONT;
						Ltarget_Enc = Enc2+1000;
						Rtarget_Enc = Enc1+1000;
						break;
					case CMD_TURN_AROUND:
						//mpu9250(&yal);
						if(recodering_flag)
							map1[mapflag]=CMD_TURN_AROUND;
						mov_dir = MOV_TURN_AROUND;
						TurnAro_flag=1;
						break;
					case CMD_START_RECORD:
						work_st = RECORD_MAP;
						break;

				}
			}
		}else if(nRet!= CSPOTTER_ERR_NeedMoreSample){
			printf("recognize fail: %d\n" , nRet);
			break;
		}
		Recorder_NextBuffer();
	}
	Recorder_Stop();
	Recorder_Release();
	return nRet;
}
void GPIO_INIT(){
	gpio_moter->gpio_open((1 << IOTDK_PWM_1_0));
	gpio_moter->gpio_open((1 << IOTDK_PWM_1_1));
	gpio_moter->gpio_open((1 << IOTDK_PWM_2_0));
	gpio_moter->gpio_open((1 << IOTDK_PWM_2_1));
	gpio_moter->gpio_open((1 << IOTDK_ENC_1_0));
	gpio_moter->gpio_open((1 << IOTDK_ENC_1_1));
	gpio_moter->gpio_open((1 << IOTDK_ENC_2_0));
	gpio_moter->gpio_open((1 << IOTDK_ENC_2_1));
	gpio_moter->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_PWM_1_0));
	gpio_moter->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_PWM_1_1));
	gpio_moter->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_PWM_2_0));
	gpio_moter->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_PWM_2_1));
	//gpio_moter->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)(1 << IOTDK_Enc_1_0));
	gpio_moter->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << IOTDK_ENC_1_0));
	gpio_moter->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << IOTDK_ENC_1_1));
	gpio_moter->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << IOTDK_ENC_2_0));
	gpio_moter->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << IOTDK_ENC_2_1));


	//gpio_moter->gpio_control(GPIO_CMD_SET_BIT_ISR,  enen );
	//gpio_moter->gpio_control(GPIO_CMD_GET_BIT_INT_CFG,  enenen );
	//print_CPIO_CFG_INFO();
	//int_handler_install(INTNO_TIMER1,moterPWM);

	gpio_led->gpio_open((1 << IOTDK_LED_0));
	gpio_led->gpio_open((1 << IOTDK_LED_1));
	gpio_led->gpio_open((1 << IOTDK_LED_2));
	gpio_led->gpio_open((1 << IOTDK_LED_3));
	gpio_led->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_LED_0));
	gpio_led->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << IOTDK_LED_1));
	gpio_led->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_LED_2));
	gpio_led->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(1 << IOTDK_LED_3));
	//gpio_led->gpio_write(1<< IOTDK_LED_3, 1 << IOTDK_LED_3);

	//gpio_led->gpio_open((1 << 1));
	//gpio_led->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(1 << 1));
}
void TIMER_INIT()
{
	if (timer_present(TIMER_1))
	{
		EMBARC_PRINTF("timer 1 is present\n");
		timer_stop(TIMER_1); // Stop it first since it might be enabled before 
		int_handler_install(INTNO_TIMER1,moterPWM);
		int_pri_set(INTNO_TIMER1, INT_PRI_MIN);
		int_enable(INTNO_TIMER1);
		timer_start(TIMER_1, TIMER_CTRL_IE, 0.0001 * BOARD_CPU_CLOCK);
	}
	else
	{
		EMBARC_PRINTF("timer 1 is not present\r\n");
	}
}
void prr(int val){
		EMBARC_PRINTF("%d%d%d%d%d%d%d%d%d\r\n",val,val,val,val,val,val,val,val,val,val);
}
void mpu9250(float* yal){
	mpu9250_sensor_read(mpu9250_sensor, &mpu9250_data);
#ifdef MPU9250_USE_DMP
		char datay[10];
		sprintf(datay, "%06.1f", mpu9250_data.yaw);

		*yal=strtof(datay,datay+0x0A);
		
		//EMBARC_PRINTF("dmp:  pitch=%s,  roll=%s,  yaw=%s, \r\n", datap, datar, datay);
		//EMBARC_PRINTF_FlOAT("yal",*yal);
		
		//board_delay_ms(100, 1);
		//EMBARC_PRINTF("\x1b[2k\x1b\x45");
		
		
#else
		EMBARC_PRINTF("accel   x=%5d,   y=%5d,   z=%5d \r\n", mpu9250_data.accel_x, mpu9250_data.accel_y, mpu9250_data.accel_z);
		EMBARC_PRINTF("gpro    x=%5d,   y=%5d,   z=%5d \r\n", mpu9250_data.gyro_x, mpu9250_data.gyro_y, mpu9250_data.gyro_z);
		EMBARC_PRINTF("mag     x=%5d,   y=%5d,   z=%5d \r\n", mpu9250_data.mag_x, mpu9250_data.mag_y, mpu9250_data.mag_z);
		board_delay_ms(100, 1);
		EMBARC_PRINTF("\x1b[2k\x1b\x45");
		EMBARC_PRINTF("\x1b[2k\x1b\x45");
		EMBARC_PRINTF("\x1b[2k\x1b\x45");
#endif
}

void EMBARC_PRINTF_FlOAT(char* str,float x) 
{
	int b = 10*(x);
	if(b/10 == 0)
			EMBARC_PRINTF("%s = %c0.%d\r\n",str,(b>0)? ' ':'-',( b>0 )? b%10 : -b%10);
		else 
			EMBARC_PRINTF("%s = %d.%d\r\n",str,(int)x,( b>0 )? b%10 : -b%10);
}
void PRINT_WK_ST()
{
	if (work_st == EXE_CMD )
				EMBARC_PRINTF("wk_st %d \r\n",work_st);
			else if (work_st == RECON_VOICE)
				EMBARC_PRINTF("wk_st2 %d \r\n",work_st);
}
int Straight_Controler(){


}
//int Straight_Controler(){


//}
void right_rotation(int dir,int pwm_1){
	//cEMBARC_PRINTF("IOTDK_PWM_1_0 is running\r\n");
	if (pwm_1>MAX_PWM){
		pwm_1=MAX_PWM;
	}
	else if(pwm_1<-MAX_PWM)
		pwm_1=-MAX_PWM;
	if(pwm_1>0){
	if(dir==0){
		//EMBARC_PRINTF("I\r\n");
		if(timepwmcount == 200-pwm_1){
			gpio_moter->gpio_write(0<< IOTDK_PWM_1_1, 1 << IOTDK_PWM_1_1);
			gpio_moter->gpio_write(1<< IOTDK_PWM_1_0, 1 << IOTDK_PWM_1_0);
			//EMBARC_PRINTF("pwm1= %d\r\n",pwm_1);
		}
	}
	else if(dir==1){
		if(timepwmcount == 200-pwm_1){
			gpio_moter->gpio_write(0<< IOTDK_PWM_1_0, 1 << IOTDK_PWM_1_0);
			gpio_moter->gpio_write(1<< IOTDK_PWM_1_1, 1 << IOTDK_PWM_1_1);
		}
	}
}
else if(pwm_1<0){
	pwm_1=-pwm_1;
	if(dir==0){
		if(timepwmcount == (200-pwm_1)){
			gpio_moter->gpio_write(0<< IOTDK_PWM_1_0, 1 << IOTDK_PWM_1_0);
			gpio_moter->gpio_write(1<< IOTDK_PWM_1_1, 1 << IOTDK_PWM_1_1);
			//EMBARC_PRINTF("pwm1= %d\r\n",pwm_1);
		}
	}
	else if(dir==1){
		if(timepwmcount == 200-pwm_1){
			gpio_moter->gpio_write(0<< IOTDK_PWM_1_1, 1 << IOTDK_PWM_1_1);
			gpio_moter->gpio_write(1<< IOTDK_PWM_1_0, 1 << IOTDK_PWM_1_0);
		}
	}
}
}

void left_rotation(int dir,int pwm_2){
	if (pwm_2>MAX_PWM){
		pwm_2=MAX_PWM;
	}
	else if(pwm_2<-MAX_PWM)
		pwm_2=-MAX_PWM;
	//EMBARC_PRINTF("pwm2= %d\r\n",pwm_2);
	if(pwm_2>0){
	if(dir==0){
		if(timepwmcount == 200-pwm_2){
			gpio_moter->gpio_write(0<< IOTDK_PWM_2_1, 1 << IOTDK_PWM_2_1);
			gpio_moter->gpio_write(1<< IOTDK_PWM_2_0, 1 << IOTDK_PWM_2_0);
			
		}
	}
	else if(dir==1){
		if(timepwmcount == 200-pwm_2){
			gpio_moter->gpio_write(0<< IOTDK_PWM_2_0, 1 << IOTDK_PWM_2_0);
			gpio_moter->gpio_write(1<< IOTDK_PWM_2_1, 1 << IOTDK_PWM_2_1);
		}
	}
	}
	else if(pwm_2<0){
		pwm_2=-pwm_2;
	if(dir==0){
	if(timepwmcount == 200-pwm_2){
			gpio_moter->gpio_write(0<< IOTDK_PWM_2_0, 1 << IOTDK_PWM_2_0);
			gpio_moter->gpio_write(1<< IOTDK_PWM_2_1, 1 << IOTDK_PWM_2_1);
			//EMBARC_PRINTF("pwm2= %d\r\n",pwm_2);
		}
	}
	else if(dir==1){
		if(timepwmcount == 200-pwm_2){
			gpio_moter->gpio_write(0<< IOTDK_PWM_2_1, 1 << IOTDK_PWM_2_1);
			gpio_moter->gpio_write(1<< IOTDK_PWM_2_0, 1 << IOTDK_PWM_2_0);
		}
	}	
	}
}
void check_Enc(){
	int P=3;
	int Rerr=( Enc1 - Rtarget_Enc );
	int Lerr=( Enc2 - Ltarget_Enc );
	int Rout = - Rerr * P;
	int Lout = - Lerr * P;
	if( Rout < 20 && Rout > -20 ){
		Routput=0;
	}
	else{
		Routput=Rout;
	}
	
	if(Lout<20&&Lout>-20){
		Loutput=0;
	}
	else{
		Loutput=Lout;
	}
}

void set_work_status_EXE_CMD()
{
	work_st = EXE_CMD;
}
void Turn(int dir){
	int p=3;
	//EMBARC_PRINTF("Turn_out= %d",Turn_out);
	if( dir == DIR_LEFT ){
		if(TargetDeg>180){
			if(yal<0)
				TargetDeg=TargetDeg-360 ;
			else
				Turn_out = -p * ( yal - TargetDeg );
		}
		Turn_out = -p * ( yal - TargetDeg );
		if( Turn_out < 20 && Turn_out > -20 ){
			Turn_out = 0;
		}
		//EMBARC_PRINTF("Turn_out= %d",Turn_out);
			right_rotation(0,Turn_out);
			left_rotation(1,Turn_out);
	}
	else if( dir == DIR_RIGHT ){
		if(TargetDeg<-180){
			if(yal>0)
				TargetDeg=TargetDeg+360 ;
			else
				Turn_out = -p * ( yal - TargetDeg );
		}
		Turn_out = -p * ( yal - TargetDeg );
		if( Turn_out < 20 && Turn_out > -20 ){
			Turn_out = 0;
		}
		right_rotation(0,Turn_out);
		left_rotation(1,Turn_out);
	}
	
}
void Map_Inverter(VOICE_CMD* map,int size){
	int flag=0;
	VOICE_CMD temp;
	size=size-1;
	while(flag<=size){
		if(map1[flag]==CMD_TURN_RIGHT_DEG_90)
			map1[flag]=CMD_TURN_LEFT_DEG_90;
		else if(map1[flag]==CMD_TURN_LEFT_DEG_90)
			map1[flag]=CMD_TURN_RIGHT_DEG_90;
		flag++;
	}
	flag=0;
	while(flag<=size/2){
		temp=map1[size-flag];
		map1[size-flag]=map1[flag];
		map1[flag]=temp;
		flag++;
		//EMBARC_PRINTF("map = %d %d %d",map1[0],map1[1],map1[2]);
	}
	

}