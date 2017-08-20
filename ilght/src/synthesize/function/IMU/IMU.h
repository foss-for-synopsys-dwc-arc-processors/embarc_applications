/*
	*Attitude solving function
	*Writer:Mr.WangS
	*Data:2017-3-21
*/
#ifndef _IMU_H_
#define _IMU_H_

#include <math.h>
#include "mpu6050.h"
#include "scope.h"
//#include "word.h"

/**Some waveform feature value macro definitions*/

/**Indicates that there is only one positive characteristic waveform.01b*/
#define ACTION_START 0x04
#define ACTION_LEFT 0x01
#define ACTION_RIGHT 0x02
#define ACTION_OVER 0X03
#define ACTION_SHAKING  0x05
/**Indicates that there is positive characteristic waveforms.0110b*/
#define WAVE_P 1
/**Indicates that there is only one NEGATIVE characteristic waveform.10b*/
#define WAVE_N 2
/**Indicates that there are positive and negative characteristic waveforms.0110b*/
#define WAVE_PN 6
/** negative positive.1001b*/
#define WAVE_NP 9
/** negative negative.1010b*/
#define WAVE_NN 10
/** positive negative positive.011001b*/
#define WAVE_PNP 25 
/** positive negative negative.011010b*/
#define WAVE_PNN 26
/**negative positive negative.100110b*/
#define WAVE_NPN 38
/**positive negative positive ngative.01100110b*/
#define WAVE_PNPN 102
/**negative positive ngative positive.10011001b*/
#define WAVE_NPNP 153
/**negative positive ngative positive.10011010b*/
#define WAVE_NPNN 154
/**negative ngative positive ngative.10100110b*/
#define WAVE_NNPN 166
/**positive negative positive ngative positive.0110011001b*/
#define WAVE_PNPNP 409
/**positive negative positive ngative positive.0110100110*/
#define WAVE_PNNPN 422
/**negative positive ngative positive negative.1001100110b*/
#define WAVE_NPNPN 614
/**negative positive ngative positive positive.1001101010b*/
#define WAVE_NPNPP 618
/**positive negative positive ngative positive ngative.011001100110b*/
#define WAVE_PNPNPN 1638


/**The flag of getting the first positive peak order*/
#define GET_PEAK_PO 0x11
/**The flag of getting the first negative peak order*/
#define GET_PEAK_NE 0x10
/**The threshold of determing the acceleration waveform peak is positive*/
#define JUDGE_WAVE_PO_THRESHOLD 0.3
/**The threshold of determing the acceleration waveform peak is negative*/
#define JUDGE_WAVE_NE_THRESHOLD 0.5
/*The threshold of determing the symbol of acc.*/
#define JUDGE_SYMBOL_THRESHOLD 800

/**The polarity of the first peak ,negative*/
#define NEGATIVE 0x02
/**The polarity of the first peak ,positive*/
#define POSITIVE 0x01      
/**Acceleration sliding window*/
#define FILTER_ACC_NUM 10	
/**The number of acceleration cache sliding window*/
#define ACC_BUF_WINDOW_NUM 15
/**The number of energy cache*/
#define ENERGY_BUF_NUM 120 		
/**Acceleration cache maximum*/
#define ACC_BUF_NUM 120	
/**The threshold of the start of action*/
#define JUDGE_START_THRESHOLD 200  
/**The threshold of the stop of action*/
#define JUDGE_STOP_THRESHOLD 40
/**The Y-axis acceleration threshold of judging swing*/
#define JUDGE_YACC_SWING_THRESHOLD 400
/**The Y_axis acceleration threshold of judging circle*/
#define JUDGE_YACC_CIRLE_THRESHOLD 300

/**Acceleration filter cache*/
typedef struct acc_upbuf{
	int acc_x_buf[FILTER_ACC_NUM];
	int acc_y_buf[FILTER_ACC_NUM];
	int acc_z_buf[FILTER_ACC_NUM];
	int32_t acc_dif_buf[FILTER_ACC_NUM];
}ACC_UPBUF;
/**MPU6050 data update structure*/
typedef struct mpu6050_updata{
	int32_t acc_x;						//The acceleration of x_axis
	int32_t acc_y;						//The acceleration of y_axis
	int32_t acc_z;						//The acceleration of z_axis
	int32_t acc_judge_x_buf[ACC_BUF_NUM];		//The buffer of x_axis's acceleration that is used to pattern reconition 
	int32_t acc_judge_y_buf[ACC_BUF_NUM];		//The buffer of y_axis's acceleration that is used to pattern reconition 
	int32_t acc_judge_z_buf[ACC_BUF_NUM];		//The buffer of z_axis's acceleration that is used to pattern reconition 
	int32_t acc_judge_dif_buf[ENERGY_BUF_NUM];	//The buffer of	energy.
}MPU050_UPDATA,*MPU050_UPDATA_PTR;

typedef struct acc_special{
	int8_t acc_x_num_crest;
	int8_t acc_y_num_valley;
	int8_t	acc_x_sign_first_peak;
	int8_t  acc_y_sign_first_peak;
	int32_t length_reaction;
}ACC_SPECIAL;

extern MPU050_UPDATA *mpu6050_updata_ptr;

/**Updata and calculate acceleration value and buffers*/
//void imu_updata_data(void);
/**pattern recognition*/
uint8_t imu_pattern_recongnition(void);

int8_t imu_get_shaking_start(void);

void mpu6050_updata(void);

uint8_t imu_get_action_shaking(void);

#endif