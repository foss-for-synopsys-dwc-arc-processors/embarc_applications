#ifndef _TSKMOTOR_H_
#define _TSKMOTOR_H_

#include "physParams.h"
#include "embARC.h"
#include "Queue/Queue.h"

typedef enum 
{
	EnableMotors    = 0x00010000,
	DisableMotors   = 0x00020000,
	EnableAng       = 0x00030000,
	DisableAng      = 0x00040000,
	EnableAcqZeros  = 0x00050000,
	DisableAcqZeros = 0x00060000,
	EnableSeqSet	= 0x00070000,
	DisableSeqSet	= 0x00080000,
	EnablePutStr	= 0x00090000,
	DisablePutStr	= 0x000A0000

}MsgType;

extern QueueHandle_t motMbCmd; 
extern Kalman1Var AngleKal;
extern lqr_q lqr;
extern VelOmega desire;
extern Pid angPid;
extern int AcclLPFParam;
extern int SpdLPFParam;
extern int AngLPFParam;
extern int GyroLPFParam;
extern int ANGLEDIFF;
extern int ANGLIMIT;
extern int PRINT_PERIOD;
extern bool PRINT_TIME;
extern bool DEBUG_LQR;
extern bool PRINT_IMU;
extern bool PRINT_PWM;

/** [saturate: data between min and max] */
Inline int saturate(int data, int max, int min)
{
	int value = (data > max)? max : ((data < min)? min : data);
	return value;
}

/** [saturate: data between min and max] */
Inline int deadzone(int data, int plimit, int nlimit)
{
    int value = (data > plimit)? data : ((data < nlimit)? data : 0);
    return value;
}

/** [calcSeq: Enqueue (data calc based on Ts) */
Inline int calcSeq(Queue *q, int32_t p0, int32_t p1, int32_t acc)
{
    int i, imax;
    if(p0 == p1)
        return 0;
    else
    {
        if(p1 < p0) acc = -acc;
        /** t = delta_p / acc, imax = t / Ts */
        imax = (int)((p1 - p0) * TS_RECI / acc);

        if(imax > SPDSEQLENMAX - 1) 
            return (SPDSEQLENMAX + 1);
        else{
            for(i = 1; i <= imax; i++)
            {
                q_en_int(q, p0 + (int)(i * acc / TS_RECI));
            }  
            q_en_int(q, p1);
        }
        return (imax + 1);
    }
}

/** [lqrCalc_q: calculate lqrOut, lqrOut -> voltage] */
Inline int32_t lqrCalc_q(lqr_q *lqr, int32_t pos, int32_t vel, int32_t ang, int32_t av)
{

    int32_t res = lqr->k1 * pos + lqr->k2 * vel + lqr->k3 * ang + lqr->k4 * av;

    return res;
}

/** [kalmanPredict_q: kalman predict xpri & Ppri] */
Inline void kalmanPredict_q(Kalman1Var *kalVal, int32_t u)
{
    /* 
     * x(k|(k-1)) = A * x((k-1)|(k-1)) + B * u(k)
     * enlarge gyro and ensure xpri factor(16)
     */
    kalVal->xpri = kalVal->xpos + ((B * u * GYRO_FACTOR) >> 15);

    /*
     * p(k|(k-1)) = A * p((k-1)|(k-1)) * A.Tran() + Q
     * A = A.Tran() = 1 << 16
     * for ensure amplification factor, ingore multiplication associated with A
     */
    kalVal->Ppri = kalVal->Ppos + kalVal->Q;
}

/** [kalmanPredict_q: kalman correct K & Ppos & xpos] */
Inline int32_t kalmanCorrect_q(Kalman1Var *kalVal, int32_t z)
{
    /*
     * K(k) = p(k|(k-1)) * H.Tran() * ((H * p(k|(k-1)) * H.Tran() + R)).Inv()     
     * H = H.Tran() = 1 << 16, ingore multiplication, so 
     * K(k) = p(k|(k-1)) / ( p(k|(k-1)) + R)
     * for ensure amplification factor, K(k) = K(k) << 16
     */
    kalVal->K = (kalVal->Ppri << 15) / (kalVal->Ppri + kalVal->R);
    /*
     * x(k|k) = x(k|(k-1)) + K(k) * (z(k) - H * x(k|(k-1)))
     * H = 1 << 16, ingore multiplication
     */
    kalVal->xpos = kalVal->xpri + (kalVal->K * (z * ANG_FACTOR - kalVal->xpri) >> 15);
    /*
     * p(k|k) = ( I – K(k) * H ) * p(k|(k-1))
     * H = 1 << 16, ingore multiplication
     */
    kalVal->Ppos = (I - kalVal->K) * kalVal->Ppri >> 16;

    return kalVal->xpos;
}

/** [pidTick_q: calculate pidOut, pidOut -> normalization (factor 2^16)] */
Inline int32_t pidTick_q(Pid *pid, int32_t diff)
{
    int32_t pout;
    int32_t dout;
  
    pout = ((diff) * pid->p) >> 8;

    pid->accI += (diff * pid->i / 500) >> 8;

    if(pid->accI > 65536)
        pid->accI = 65536;
    else if(pid->accI < -65536)
        pid->accI = -65536;

    dout = ((diff * pid->d) >> 8 - pid->accD) * pid->n >> 8;    
    pid->accD += dout / 500;

    if(pid->accD > 65536)
        pid->accD = 65536;
    else if(pid->accD < -65536)
        pid->accD = -65536;

    pout += pid->accI;

    pout += dout;

    if(pout >65536)
        pout = 65536;
    else if(pout < -65536)
        pout = -65536;
    
    return pout;
}

void motorInit(void);

#endif