#ifndef USER_PHYSPARAMS_H
#define USER_PHYSPARAMS_H

#define SPDSEQLENMAX 512
#define ANGSEQLENMAX 1024

/** control period*/
#define Ts  	0.002f
#define TS_RECI 500		/** 1/Ts*/

/** EncUnit = 0.067 * pi / 15000 / 4 / 0.002 */
#define EncUnit 0.067f * 3.14159f / 0.002f / 60000.f
/*!< imu sensor data convert coefficient */
#define GyroUnit  1.0642252e-3f;  // rad/s/LSB @ fs=2000deg/s
#define AcclUnit  5.9855042e-4f;  // m/sq.s/LSB @ fs=2g

/*
 * LPF params << 10
 * y(n) = (1-a) * y(n-1) + a * x
 *            float            q1.7
 * set a  -1~0.99902  ->  -512 ~ 511 
 */
#define LPF_FACTOR 128

/** kalman params << 15 */
#define A 				32768      	/** 1 << 15 */
#define A_TRAN 			32768      	/** 1 << 15 */
#define B            	66        	/** (1 << 15) * Ts */
#define H            	32768      	/** 1 << 15 */
#define H_TRAN       	32768		/** 1 << 15 */
#define I            	32768      	/** 1 << 15 */

/** lqr params << 21 */
#define GYRO_FACTOR  	2232       	    /** (1 << 21) * GyroUnit */
#define ANG_FACTOR   	2097152       	/** (1 << 21))     */
#define ENCL_FACTOR		3679    		/** (1 << 21)) * EncUnit */
#define ENCR_FACTOR     3679            /** (1 << 21)) * EncUnit */

/*
 * 1-order kalman 
 */
typedef struct
{
    //int16_t A, B, H (constant, A = 1, B = Ts, H = 1)
    int Q, R, K;
    int xpri, Ppri;
    int xpos, Ppos;
}Kalman1Var;

typedef struct 
{
	int k1, k2, k3, k4;
}lqr_q;

typedef struct
{
	int p, i, d, n;
    int accI, accD;
}Pid;

typedef struct{
    int Velocity;
    int Timeout;
    int Acc;
    int Omega;
    int ThetaZ;
    int ThetaY;
}VelOmega;

#endif
