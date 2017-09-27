/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * \version 2017.09
 * \date 2017-09-27
 * \author WangShuai(mrwangs@huast.edu.cn)
--------------------------------------------- */
/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief	Header file of action recognition function.
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */

/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* standard HAL */
#include <stdlib.h>

/* custom HAL */
#include "imu.h"
#include "mpu6050.h"
#include "scope.h"

/* Some templates of waves definition */
#define WAVE_P			1
#define WAVE_N			2
#define WAVE_PN			6
#define WAVE_NP 		9
#define WAVE_NN 		10
#define WAVE_PNP 		25
#define WAVE_PNN 		26
#define WAVE_NPN 		38
#define WAVE_PNPN 		102
#define WAVE_NPNP 		153
#define WAVE_NPNN 		154
#define WAVE_NNPN 		166
#define WAVE_PNPNP 		409
#define WAVE_PNNPN 		422
#define WAVE_NPNPN 		614
#define WAVE_NPNPP		618
#define WAVE_PNPNPN		1638

/* Some flags definition */
#define GET_PEAK_PO			0x11 /* The flag of getting the first positive peak order */
#define GET_PEAK_NE			0x10 /* The flag of getting the first negative peak order */

/* Some threshold which is used in data dealing definition */
#define JUDGE_WAVE_PO_THRESHOLD 	0.3  /* The threshold of determing the acceleration waveform peak is positive */
#define JUDGE_WAVE_NE_THRESHOLD 	0.5  /* The threshold of determing the acceleration waveform peak is negative */
#define JUDGE_SYMBOL_THRESHOLD 		800  /* The threshold of determing the symbol of acc */
#define JUDGE_START_THRESHOLD 		200
#define JUDGE_STOP_THRESHOLD 		40
#define JUDGE_YACC_SWING_THRESHOLD 	400
#define JUDGE_YACC_CIRLE_THRESHOLD 	300

#define PEAK_WAVE_NEGATIVE 		0x02 /* The record of negative peak */
#define PEAK_WAVE_POSITIVE 		0x01 /* The record of negative peak */
#define ACC_FILTER_NUM 			10   /* The number of filtering windows */
#define ACC_BUF_WINDOW_NUM 		15   /* The number of dynamically updated caches */

/* Acceleration filter cache */
typedef struct acc_upbuf {
	int acc_x_buf[ACC_FILTER_NUM]; /* The buffer of x_axis's acceleration that is used to filter */
	int acc_y_buf[ACC_FILTER_NUM];
	int acc_z_buf[ACC_FILTER_NUM];
	int32_t acc_dif_buf[ACC_FILTER_NUM];
} ACC_UPBUF;

/* The structure is used to record some specials of the wave of an action */
typedef struct acc_special {
	int8_t acc_x_num_crest;
	int8_t acc_y_num_valley;
	int8_t acc_x_sign_first_peak;
	int8_t acc_y_sign_first_peak;
	int32_t length_reaction;
} ACC_SPECIAL;

static MPU050_UPDATA n_data;
static ACC_UPBUF acc_upbuf;
static ACC_SPECIAL acc_special_data;

MPU050_UPDATA *imu_mpu6050_update_ptr = &n_data;

/**
 * \brief	Action captures state variables.
 * 7 bit		Capture success flag,1 represents one capture is complete
 * 6:1 bit	The type of action which is captured.
 * 0 bit 	Set 1 if capture the start signal of action.
*/
static uint8_t partern_rec_startflag;
static uint8_t shaking_rec_startflag;

/**
 * \brief	Init the device which is used to get data of acceleration.
 * \parameter gyro_rng	Set the range of gyro,0(250 degree/s),1(500 degree/2),2(1000 degree/s),3(2000 degree/s).
 * \parameter accel_rng	Set the range of accelerometer,0(2g),1(4g),2(8g),3(16g).
 */
void imu_init(uint8_t gyro_rng, uint8_t acc_rng)
{
	mpu6050_init(gyro_rng, acc_rng);
}

/**
 * \brief	Get the start and stop point and set the first position of partern_rec_startflag.
 */
void imu_get_stastp_point(void)
{
	/* If the value of acceleration difference is bigger than the threshold of start,set the first bit of  \
		partern_rec_startflag one.Get the start of action */
	if ((imu_mpu6050_update_ptr->acc_judge_dif_buf[1] >= JUDGE_START_THRESHOLD) && \
	                (imu_mpu6050_update_ptr->acc_judge_dif_buf[0] >= JUDGE_START_THRESHOLD)) {
		if (imu_mpu6050_update_ptr->acc_judge_dif_buf[3] <= JUDGE_START_THRESHOLD) {
			partern_rec_startflag |= 0x01;
		}
	}

	/* If the start action has been got and the value of acceleration difference is smaller than the threshold of stop,set
		the eighth bit of partern_rec_startflag one */
	if ((imu_mpu6050_update_ptr->acc_judge_dif_buf[1] <= JUDGE_STOP_THRESHOLD) && \
	                (imu_mpu6050_update_ptr->acc_judge_dif_buf[0] <= JUDGE_STOP_THRESHOLD)) {
		if ((partern_rec_startflag & 0x01) && (imu_mpu6050_update_ptr->acc_judge_dif_buf[3] >= JUDGE_STOP_THRESHOLD)) {
			partern_rec_startflag &= 0xfe;
			partern_rec_startflag |= 0x80;
		}
	}
}

/**
 * \brief	Get the start and stop point of shaking and set the first position of partern_rec_startflag.
 */
void imu_shaing_get_stastp_point(void)
{
	/* If the value of acceleration difference is bigger than the threshold of start,set the first bit of
		partern_rec_startflag one.Get the start of action */
	if ((imu_mpu6050_update_ptr->acc_judge_dif_buf[1] >= JUDGE_START_THRESHOLD) && \
	                (imu_mpu6050_update_ptr->acc_judge_dif_buf[0] >= JUDGE_START_THRESHOLD)) {
		if (imu_mpu6050_update_ptr->acc_judge_dif_buf[3] <= JUDGE_START_THRESHOLD) {
			shaking_rec_startflag |= 0x01;
		}
	}

	/* If the start action has been got and the value of acceleration difference is smaller than
		the threshold of stop,set the eighth bit of partern_rec_startflag one */
	if ((imu_mpu6050_update_ptr->acc_judge_dif_buf[1] <= JUDGE_STOP_THRESHOLD) && \
	                (imu_mpu6050_update_ptr->acc_judge_dif_buf[0] <= JUDGE_STOP_THRESHOLD)) {
		if ((shaking_rec_startflag & 0x01) && (imu_mpu6050_update_ptr->acc_judge_dif_buf[3] >= JUDGE_STOP_THRESHOLD)) {
			shaking_rec_startflag &= 0xfe;
			shaking_rec_startflag |= 0x80;
		}
	}
}

/**
 * \brief	Get the point of start of shaking.
 * \retval	ACTION_START Get the start point of shaking.
 * \retval	1 Not get the start point.
 */
int8_t imu_get_shaking_start(void)
{
	if ((imu_mpu6050_update_ptr->acc_judge_y_buf[1] < -1000) && \
	                (imu_mpu6050_update_ptr->acc_judge_y_buf[1] <= imu_mpu6050_update_ptr->acc_judge_y_buf[2])) {
		return ACTION_START;
	} else {
		return 1;
	}
}

/**
 * \brief	Get some features of the data of action that is caputured.
 * \param	*point 		The address of the highest bit 0f the acceleration buffer that need to obtain the eigenvalue.
 * \param	multi_throd	Set the threshold for positive and negative feature judgments.z:0.3,y:0.4.
 * \param	recognition.	The value of the wave of action.Set the last 2 bits PEAK_WAVE_NEGATIVE when get a negative peak.
 * \param	peak_order. 	The order of the peak of y axies or z axies.
 * \param	peak_flag. 	Sign of get the position of first positive or negative peak.
*/
void imu_acc_feature_get(int32_t *point, float multi_throd, uint32_t *recognition, uint8_t *peak_order, uint8_t peak_flag)
{
	uint8_t i;
	int32_t max_value = 0, min_value = 0, last_value_flag = 0;
	int32_t *cal_point = point, posi_zero_value, nega_zero_value;

	*recognition = 0;
	/* Get the the maximum value and minimum value */
	for (i = ACC_DATA_CON_NUM - 1; i > ACC_BUF_WINDOW_NUM; i--) {
		if (*cal_point > max_value) {
			max_value = *cal_point;
		}
		if (*cal_point < min_value) {
			min_value = *cal_point;
		}
		cal_point--;
	}

	/* Point the pointer to the first address of the acceleration cache */
	cal_point += (ACC_DATA_CON_NUM - 1 - ACC_BUF_WINDOW_NUM);
	/* Get the first positive peak's position */
	if (peak_flag == GET_PEAK_PO) {
		for (i = ACC_DATA_CON_NUM - 1; i > ACC_BUF_WINDOW_NUM; i--) {
			/* If the value is bigger than 0.8 times of the maximum,
				calibrate the first positive peak's position */
			if (*cal_point >= (max_value * 0.8)) {
				*peak_order = i;
				break;
			}
			cal_point--;
		}
	} else {
		for (i = ACC_DATA_CON_NUM - 1; i > ACC_BUF_WINDOW_NUM; i--) {
			if ((*cal_point) <= (min_value * 0.8)) {
				/* If the value is smaller than 0.8 times of the minimal,
					calibrate the first negative peak's position */
				*peak_order = i;
				break;
			}
			cal_point--;
		}
	}

	/* Set the threshold for determining acceleration 0 */
	posi_zero_value = 0.1 * max_value;
	nega_zero_value = 0.1 * min_value;
	/* Set the positive and negative thresholds.If the max_value*multi_throd is smaller
		than JUDGE_SYMBOL_THRESHOLD,the threshold will be set as JUDGE_SYMBOL_THRESHOLD */
	max_value = (max_value * multi_throd > JUDGE_SYMBOL_THRESHOLD) ? max_value * multi_throd : JUDGE_SYMBOL_THRESHOLD;
	min_value = (min_value * multi_throd < -JUDGE_SYMBOL_THRESHOLD) ? min_value * multi_throd : -JUDGE_SYMBOL_THRESHOLD;

	/* Judge the positive and negative of the intercept waveform
	  If the judgment waveform is positive，the variable "(*recognition)" is shifted by two bits and
	  	takes 0x02.Take "last_value_flag" with 0x02 \
	  If the judgment waveform is negative，the variable "(*recognition)" is shifted by two bits and
	  	takes 0x01.Take "last_value_flag" with 0x01 \
	  Take "last_value_flag" with 0x00
	*/
	for (i = ACC_DATA_CON_NUM - 1; i >= ACC_BUF_WINDOW_NUM; i--)
	{
		/* If the last feature is not negative(Judge by the second bit of last_value_flag,if the second
			bit of last_value_flag is not one),\
		   and the value is smaller than min_value */
		if (((*point) < min_value) && ((last_value_flag & 0x02) == 0x00)) {
			(*recognition) = ((*recognition) << 2) | PEAK_WAVE_NEGATIVE;
			last_value_flag &= 0x00;
			last_value_flag |= PEAK_WAVE_NEGATIVE;
		}

		/* If the last feature is not zero(Judge by the first and second bits of last_value_flag,if these
			bits of last_value_flag are zero) */
		if ((last_value_flag & 0x03) != 0x00) {
			if (((*point) > nega_zero_value) && (*point) < posi_zero_value) {
				last_value_flag &= 0x00;
			}
		}

		/* If the last feature is not positive(Judge by the first bit of last_value_flag,if the first bit
			 of last_value_flag is not one) */
		if (((*point) > max_value) && (((*recognition) & 0x01) == 0x00)) {
			(*recognition) = ((*recognition) << 2) | PEAK_WAVE_POSITIVE;
			last_value_flag &= 0x00;
			last_value_flag |= PEAK_WAVE_POSITIVE;
		}

		point--;
	}
}

/**
 * \brief 	Update x_axis,y_axis,z_axis's date of acceleration an filter.
 * \param	none.
*/
void imu_mpu6050_update(void)
{
	int8_t i;
	static uint8_t filter_cnt = 0;	/* Filter cache flag */
	//float scope_data[4];		/* Ued in scope */
	uint32_t cal_acc_temp[3], acc_sum;
	uint32_t acc_dif_temp = 0;
	int acc_temp[3] = {0};

	acc_upbuf.acc_x_buf[filter_cnt] = mpu6050_get_data(ACCEL_XOUT_H);
	acc_upbuf.acc_y_buf[filter_cnt] = mpu6050_get_data(ACCEL_YOUT_H);
	acc_upbuf.acc_z_buf[filter_cnt] = mpu6050_get_data(ACCEL_ZOUT_H);

	for (i = 0; i < ACC_FILTER_NUM; i++) {
		acc_temp[0] += acc_upbuf.acc_x_buf[i];
		acc_temp[1] += acc_upbuf.acc_y_buf[i];
		acc_temp[2] += acc_upbuf.acc_z_buf[i];
	}

	acc_temp[0] = acc_temp[0] / ACC_FILTER_NUM;
	acc_temp[1] = acc_temp[1] / ACC_FILTER_NUM;
	acc_temp[2] = acc_temp[2] / ACC_FILTER_NUM;

	imu_mpu6050_update_ptr->acc_x = 16 * ((acc_temp[0]) * 1000) / (32768);
	imu_mpu6050_update_ptr->acc_y = 16 * ((acc_temp[1]) * 1000) / (32768);
	imu_mpu6050_update_ptr->acc_z = 16 * ((acc_temp[2]) * 1000) / (32768);

	/* Upte those buffer which is update dynamically */
	for (i = ACC_BUF_WINDOW_NUM - 1; i > 0; i--) {
		imu_mpu6050_update_ptr->acc_judge_x_buf[i] = imu_mpu6050_update_ptr->acc_judge_x_buf[i - 1];
	}
	imu_mpu6050_update_ptr->acc_judge_x_buf[0] = imu_mpu6050_update_ptr->acc_x;
	for (i = ACC_BUF_WINDOW_NUM - 1; i > 0; i--) {
		imu_mpu6050_update_ptr->acc_judge_y_buf[i] = imu_mpu6050_update_ptr->acc_judge_y_buf[i - 1];
	}
	imu_mpu6050_update_ptr->acc_judge_y_buf[0] = imu_mpu6050_update_ptr->acc_y;
	for (i = ACC_BUF_WINDOW_NUM - 1; i > 0; i--) {
		imu_mpu6050_update_ptr->acc_judge_z_buf[i] = imu_mpu6050_update_ptr->acc_judge_z_buf[i - 1];
	}
	imu_mpu6050_update_ptr->acc_judge_z_buf[0] = imu_mpu6050_update_ptr->acc_z;

	/* If the statr of action is captured ,store date in buffer */
	if (partern_rec_startflag & 0x01) {
		for (i = ACC_DATA_CON_NUM - 1; i > ACC_BUF_WINDOW_NUM; i--) {
			imu_mpu6050_update_ptr->acc_judge_y_buf[i] = imu_mpu6050_update_ptr->acc_judge_y_buf[i - 1];
			imu_mpu6050_update_ptr->acc_judge_x_buf[i] = imu_mpu6050_update_ptr->acc_judge_x_buf[i - 1];
			imu_mpu6050_update_ptr->acc_judge_z_buf[i] = imu_mpu6050_update_ptr->acc_judge_z_buf[i - 1];
		}
		imu_mpu6050_update_ptr->acc_judge_y_buf[ACC_BUF_WINDOW_NUM] = imu_mpu6050_update_ptr->acc_judge_y_buf[ACC_BUF_WINDOW_NUM - 1] * 4;
		imu_mpu6050_update_ptr->acc_judge_x_buf[ACC_BUF_WINDOW_NUM] = imu_mpu6050_update_ptr->acc_judge_x_buf[ACC_BUF_WINDOW_NUM - 1];
		imu_mpu6050_update_ptr->acc_judge_z_buf[ACC_BUF_WINDOW_NUM] = imu_mpu6050_update_ptr->acc_judge_z_buf[ACC_BUF_WINDOW_NUM - 1] + 1000;
	}

	cal_acc_temp[0] = (imu_mpu6050_update_ptr->acc_judge_x_buf[1] - imu_mpu6050_update_ptr->acc_judge_x_buf[0]);
	cal_acc_temp[1] = (imu_mpu6050_update_ptr->acc_judge_y_buf[1] - imu_mpu6050_update_ptr->acc_judge_y_buf[0]);
	cal_acc_temp[2] = (imu_mpu6050_update_ptr->acc_judge_z_buf[1] - imu_mpu6050_update_ptr->acc_judge_z_buf[0]);

	acc_upbuf.acc_dif_buf[filter_cnt] = abs(cal_acc_temp[0]) + abs(cal_acc_temp[1]) + abs(cal_acc_temp[2]);

	for (i = 0; i < ACC_FILTER_NUM; i++) {
		acc_dif_temp += acc_upbuf.acc_dif_buf[i];
	}
	acc_dif_temp = acc_dif_temp / ACC_FILTER_NUM;
	for (i = ACC_DATA_CON_NUM - 1; i > 0; i--) {
		imu_mpu6050_update_ptr->acc_judge_dif_buf[i] = imu_mpu6050_update_ptr->acc_judge_dif_buf[i - 1];
	}
	imu_mpu6050_update_ptr->acc_judge_dif_buf[0] = acc_dif_temp;

	filter_cnt++;

	if (filter_cnt == ACC_FILTER_NUM) {
		filter_cnt = 0;
	}
}

/**
 * \brief	According to the data that buffered,recongnize the action of shaking.
 * \retval	ACTION_SHAKING.	Capture the action of shaking.
 * \retval	1		Not get the action of shaking.
*/
uint8_t imu_get_action_shaking(void)
{
	uint8_t i, y_order, z_order;
	uint32_t recognition_z = 0, recognition_y = 0;
	imu_get_stastp_point();
	imu_shaing_get_stastp_point();

	if (shaking_rec_startflag & 0x80)
	{
		imu_acc_feature_get(&(imu_mpu6050_update_ptr->acc_judge_y_buf[ACC_DATA_CON_NUM - 1]), \
		                    JUDGE_WAVE_NE_THRESHOLD, &recognition_y, &y_order, GET_PEAK_PO);
		imu_acc_feature_get(&(imu_mpu6050_update_ptr->acc_judge_z_buf[ACC_DATA_CON_NUM - 1]), \
		                    JUDGE_WAVE_PO_THRESHOLD, &recognition_z, &z_order, GET_PEAK_NE);

		if ((recognition_z >= WAVE_PN) || (recognition_y >= WAVE_PN)) {
			shaking_rec_startflag = 0;
			return ACTION_SHAKING;
		}
	}

	return 1;
}

/**
 * \brief	Recongnize the type of action and return value related.
 * \retval	ACTION_START	Captured the action of T whiched represent the sign of mode changing.
 * \retval	ACTION_LEFT	Captured the action of LEFT which represent change mode in left.
 * \retval	ACTION_RIGHT	Captured the action of RIGHT which represent change mode in right.
 * \retval	ACTION_OVER	Captured the action of ACTION_OVER which represent the sign of stop mode changing.
 * \rerval	0		Can't recongnize the type of action.
*/
uint8_t imu_pattern_recongnition(void)
{
	uint8_t i, y_order, z_order;
	static uint8_t start_action_get_flag;
	uint32_t recognition_z = 0, recognition_y = 0;
	uint8_t action_result = 0;
	imu_get_stastp_point();

	if (partern_rec_startflag & 0x80) {
		imu_acc_feature_get(&(imu_mpu6050_update_ptr->acc_judge_y_buf[ACC_DATA_CON_NUM - 1]), JUDGE_WAVE_NE_THRESHOLD, \
		                    &recognition_y, &y_order, GET_PEAK_PO);
		imu_acc_feature_get(&(imu_mpu6050_update_ptr->acc_judge_z_buf[ACC_DATA_CON_NUM - 1]), JUDGE_WAVE_PO_THRESHOLD, \
		                    &recognition_z, &z_order, GET_PEAK_NE);

		/* Only when the system does not enter the mode of switching state,we will judge whether we have got the T */
		if (start_action_get_flag == 0) {
			/* Judge T!  Z:6,2,38.  Y:9,38 */
			if ((recognition_z == WAVE_PN) || (recognition_z == WAVE_N) || (recognition_z == WAVE_NPN)) {
				if ((recognition_y == WAVE_NP) || (recognition_y == WAVE_NPN)) {
					/* Judge first action is vertical or vertical to reduce miscarriage of justice */
					if (y_order > z_order) {
						start_action_get_flag = 0x01;
						EMBARC_PRINTF("Recognition OK!!  :T!\r\n");
						action_result = ACTION_START;
					}
				}
			}
		}

		/* Only when the system enter the mode of switching state,we will judge left or right */
		if  (start_action_get_flag != 0) {
			if ((recognition_y == WAVE_NP) || (recognition_y == WAVE_NPN)) {
				if ((recognition_z == 0) || (recognition_z == WAVE_P) || (recognition_z == WAVE_N)) {
					action_result = ACTION_LEFT;
					EMBARC_PRINTF("Recognition OK!!  :LEFT\r\n");
				}
			}
			if ((recognition_y == WAVE_PN) || (recognition_y == WAVE_PNP)) {
				if ((recognition_z == 0) || (recognition_z == WAVE_P) || (recognition_z == WAVE_N)) {
					action_result = ACTION_RIGHT;
					EMBARC_PRINTF("Recognition OK!!  :RIGHT\r\n");
				}
			}
		}

		if (start_action_get_flag != 0) {
			if ((recognition_z == WAVE_PNP) || (recognition_z == WAVE_NP)) {
				if ((recognition_y == WAVE_NP)) {
					action_result = ACTION_OVER;
					start_action_get_flag = 0;
				}
			}
		}

		partern_rec_startflag &= 0x7f;
		for (i = 0; i < ACC_DATA_CON_NUM; i++)
		{
			imu_mpu6050_update_ptr->acc_judge_x_buf[i] = 0;
			imu_mpu6050_update_ptr->acc_judge_y_buf[i] = 0;
			imu_mpu6050_update_ptr->acc_judge_z_buf[i] = 0;

		}
	}
	return action_result;
}

/** @} */