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
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

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
 * \version 2017.08
 * \date 2017-08-20
 * \author Mr.WangS(mrwangs@hust.edu.cn)
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

#ifndef _IMU_H_
#define _IMU_H_

/* The type of action definition. */
#define ACTION_LEFT		0x01
#define ACTION_RIGHT		0x02
#define ACTION_OVER		0X03
#define ACTION_START		0x04
#define ACTION_SHAKING		0x05
/* The max number of data. */
#define ACC_DATA_CON_NUM 	120 

#define GYRO_RNG 		2
#define ACCEL_RNG 		3


/* MPU6050 data update structure */
typedef struct imu_mpu6050_update {
	int32_t acc_x; /* The acceleration of x_axis */
	int32_t acc_y;
	int32_t acc_z;
	int32_t acc_judge_x_buf[ACC_DATA_CON_NUM]; /* The buffer of x_axis's acceleration that is used to pattern reconition.*/
	int32_t acc_judge_y_buf[ACC_DATA_CON_NUM];		
	int32_t acc_judge_z_buf[ACC_DATA_CON_NUM];	
	int32_t acc_judge_dif_buf[ACC_DATA_CON_NUM]; /* The buffer of energy. */
}MPU050_UPDATA,*MPU050_UPDATA_PTR;

MPU050_UPDATA *imu_mpu6050_update_ptr;

/*function init the mpu6050*/
extern void imu_init(uint8_t gyro_rng,uint8_t acc_rng);

/* function pattern recognition */
extern uint8_t imu_pattern_recongnition(void);

/* function get the start of action of shaking,used in light_mode_word() */
extern int8_t imu_get_shaking_start(void);

/* function get the action of shaking */
extern uint8_t imu_get_action_shaking(void);

extern void imu_mpu6050_update(void);

#endif/* _IMU_H_ */
/** @} end of group EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE */