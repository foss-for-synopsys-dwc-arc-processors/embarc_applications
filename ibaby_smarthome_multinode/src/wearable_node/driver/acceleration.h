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
 * \version 2017.06
 * \date 2017-06-26
 * \author dbHu(wwmhu@outlook.com)
--------------------------------------------- */
/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief	acceleration sensor driver related header file
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * @{
 */
#ifndef _ACCELERATION_H_
#define _ACCELERATION_H_

/* using IIC 1 interface */
#define ACC_SENSOR_IIC_ID	DW_IIC_1_ID

/** MPU6050 registers */
#define MPU6050_REG_SAMPLE_RATE          0x19 /* Gyro sample rate */ 
// #define MPU6050_REG_EXT_SYNC_DISABLE     0x1A /* Ext sync disable */
// #define MPU6050_REG_GYRO_FULL_SCALE      0x1B /* Gyro full scale  */
// #define MPU6050_REG_ACCEL_FULL_SCALE     0x1C /* Accel full scale */
#define MPU6050_REG_POWER_MANAGEMENT     0x6B /* Power management */
#define MPU6050_REG_ACCEL_XOUT_H         0x3B /* Accel_x out[15:8] */
// #define MPU6050_REG_ACCEL_XOUT_L         0x3C /* Accel_x out[7:1] */
// #define MPU6050_REG_ACCEL_YOUT_H         0x3D /* Accel_y out[15:8] */
// #define MPU6050_REG_ACCEL_YOUT_L         0x3E /* Accel_y out[7:1] */
// #define MPU6050_REG_ACCEL_ZOUT_H         0x3F /* Accel_z out[15:8] */
// #define MPU6050_REG_ACCEL_ZOUT_L         0x40 /* Accel_z out[7:1] */
// #define MPU6050_REG_TEMP_OUT_H           0x41 /* Temp out[15:8] */
// #define MPU6050_REG_TEMP_OUT_L           0x42 /* Temp out[7:1] */
// #define MPU6050_REG_GYRO_XOUT_H          0x43 /* Gyro_x out[15:8] */
// #define MPU6050_REG_GYRO_XOUT_L          0x44 /* Gyro_x out[7:1] */
// #define MPU6050_REG_GYRO_YOUT_H          0x45 /* Gyro_y out[15:8] */
// #define MPU6050_REG_GYRO_YOUT_L          0x46 /* Gyro_y out[7:1] */
// #define MPU6050_REG_GYRO_ZOUT_H          0x47 /* Gyro_z out[15:8] */
// #define MPU6050_REG_GYRO_ZOUT_L          0x48 /* Gyro_z out[7:1] */

/**
 * I2C address of PmodIMU4 can be selected via AD0: 0x68, 0x69
 * AD0 connect to GND is 0x68
 */
/* MPU6050 address */
#define MPU6050_A0_PIN   0 /*!< I2C Serial Bus Address Selection Pin */
#define MPU6050_ADDRESS  (0x68 + MPU6050_A0_PIN)
/* acceleration sensor address */
#define ACC_SENSOR_ADDR	  MPU6050_ADDRESS

#define EMSK_IMU_SENSOR_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)


typedef struct __accValues
{
	float accl_x, accl_y, accl_z;
} acc_values;

union _acc_data
{
	uint8_t buf[6];
	struct {
		uint8_t axh, axl, ayh, ayl, azh, azl;
	};
} acc_data;

/* configure related register, using the auto increase function */
static uint8_t imu_init_seq0[] = {
	MPU6050_REG_SAMPLE_RATE,
	0x00, /*!< 0x19: sr = gyro rate / 1 */
	0x01, /*!< 0x1A: ext sync disable, dlpf = 1(accl 1k, gyro 1k) */
	0x18, /*!< 0x1B: gyro fs = 2k deg/sec (34.6/s)*/
	0x00  /*!< 0x1C: accl fs = 2g (19.6m/sq.s) */
};

static uint8_t imu_init_seq1[] = {
	MPU6050_REG_POWER_MANAGEMENT,
	0x00 /*!< 0x6B: no sleep */
};

static const float acc_unit = 5.9855042e-4f; /*!< m/sq.s/LSB @ fs=2g */

static DEV_IIC  *emsk_imu_sensor; /*!< IMU6050 sensor object */
static uint32_t acc_sensor_addr;  /*!< variable of acceleration sensor address */


/* function for acceleration sensor initialize */
extern int32_t acc_sensor_init(uint32_t slv_addr);

/* function for reading acceleration sensor data */
extern int32_t acc_sensor_read(acc_values* accel_val);

#endif  /* _WEARABLE_NODE_DRIVER_ACCELERATION_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */
