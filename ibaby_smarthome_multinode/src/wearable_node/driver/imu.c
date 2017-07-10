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
 * \defgroup	BOARD_EMSK_DRV_IMU	EMSK  Sensor Driver
 * \ingroup	BOARD_EMSK_DRIVER
 * \brief	EMSK PMOD acceleration Sensor Driver
 * \details
 *		Realize driver for PMOD acceleration sensor using Designware IIC driver.
 */

/**
 * \file
 * \ingroup	BOARD_EMSK_DRV_IMU
 * \brief	emsk pmod acceleration sensor driver
 */

/**
 * \addtogroup	BOARD_EMSK_DRV_IMU
 * @{
 */

#include "arc.h"
#include "arc_builtin.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"

/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#include "dev_iic.h"
#include "board.h"
#include "imu.h"
#include <stdio.h>

#define DRIVER_TEST                      0    /*!< driver test */ 
/*! MPU6050 registers */
#define MPU6050_REG_SAMPLE_RATE          0x19 /*!< Gyro sample rate */ 
#define MPU6050_REG_EXT_SYNC_DISABLE     0x1A /*!< Ext sync disable */
#define MPU6050_REG_GYRO_ FULL_SCALE     0x1B /*!< Gyro full scale  */
#define MPU6050_REG_ACCEL_FULL_SCALE     0x1C /*!< Accel full scale */
#define MPU6050_REG_POWER_MANAGEMENT     0x6B /*!< Power management */
#define MPU6050_REG_ACCEL_XOUT_H         0x3B /*!< Accel_x out[15:8] */
#define MPU6050_REG_ACCEL_XOUT_L		 0x3C /*!< Accel_x out[7:1] */
#define MPU6050_REG_ACCEL_YOUT_H 		 0x3D /*!< Accel_y out[15:8] */
#define MPU6050_REG_ACCEL_YOUT_L		 0x3E /*!< Accel_y out[7:1] */
#define MPU6050_REG_ACCEL_ZOUT_H		 0x3F /*!< Accel_z out[15:8] */
#define MPU6050_REG_ACCEL_ZOUT_L		 0x40 /*!< Accel_z out[7:1] */
#define MPU6050_REG_TEMP_OUT_H           0x41 /*!< Temp out[15:8] */
#define MPU6050_REG_TEMP_OUT_L		     0x42 /*!< Temp out[7:1] */
#define MPU6050_REG_GYRO_XOUT_H          0x43 /*!< Gyro_x out[15:8] */
#define MPU6050_REG_GYRO_XOUT_L		     0x44 /*!< Gyro_x out[7:1] */
#define MPU6050_REG_GYRO_YOUT_H 		 0x45 /*!< Gyro_y out[15:8] */
#define MPU6050_REG_GYRO_YOUT_L		     0x46 /*!< Gyro_y out[7:1] */
#define MPU6050_REG_GYRO_ZOUT_H	     	 0x47 /*!< Gyro_z out[15:8] */
#define MPU6050_REG_GYRO_ZOUT_L		     0x48 /*!< Gyro_z out[7:1] */


static DEV_IIC *emsk_imu_sensor;
static uint32_t imu_sensor_slvaddr;
#define EMSK_IMU_SENSOR_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)

union _imu_data
{
    uint8_t buf[14];
    struct
    {
    	uint8_t axh, axl, ayh, ayl, azh, azl;
    	uint8_t th, tl;
    	uint8_t gxh, gxl, gyh, gyl, gzh, gzl;
    };
} imu_data;

union _acc_data
{
    uint8_t buf[6];
    struct
    {
    	uint8_t axh, axl, ayh, ayl, azh, azl;
    };
} acc_data;

/* 	 configure related register,using the auto increase function	*/
uint8_t imu_init_seq0[] = {
        MPU6050_REG_SAMPLE_RATE,
        0x00, // 0x19: sr = gyro rate / 1
        0x01, // 0x1A: ext sync disable, dlpf = 1(accl 1k, gyro 1k)
        0x18, // 0x1B: gyro fs = 2k deg/sec (34.6/s)
        0x00  // 0x1C: accl fs = 2g (19.6m/sq.s)
};

uint8_t imu_init_seq1[] = {
        MPU6050_REG_POWER_MANAGEMENT,
        0x00    // 0x6B: no sleep
};

/**
 * \brief	write mpu6050 register
 * \param 	*seq 	register address and value to be written
 * \param   len     the length of seq
 * \retval	>=0	write success, return bytes written
 * \retval	!E_OK	write failed
 */
int32_t mpu6050_reg_write(uint8_t *seq,uint8_t len)
{
	int32_t ercd = E_PAR;

	/*
	*	arc\embARC\board\emsk\emsk.h:
		#define BOARD_ADC_IIC_ID		DW_IIC_0_ID
		#define BOARD_TEMP_SENSOR_IIC_ID	DW_IIC_0_ID
	*/
	emsk_imu_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);  

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor!=NULL);

	/** make sure set the imu sensor's slave address */
	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(imu_sensor_slvaddr));

	ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_imu_sensor->iic_write(seq, len);

error_exit:
	return ercd;
}

/**
 * \brief	read mpu6050 register
 * \param 	seq		register address
 * \param 	*val    returned value
 * \param 	len     the length of returned value
 * \retval	>=0	read success, return bytes read
 * \retval	!E_OK	read failed
 */
int32_t mpu6050_reg_read(uint8_t seq,uint8_t *val,uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_imu_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor!=NULL);

	/** make sure set the temp sensor's slave address */
	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(imu_sensor_slvaddr));
	/* write register address then read register value */
	ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));//ercd = 0

	ercd = emsk_imu_sensor->iic_write(&seq, 1);//ercd = 1

	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_imu_sensor->iic_read(val, len);//ercd = 14
	
error_exit:
	return ercd;
}

/**
 * \brief	imu sensor init
 * \param[in]	slv_addr	imu sensor iic slave address
 * \retval	E_OK	init success
 * \retval	!E_OK	init failed
 */
int32_t imu_sensor_init(uint32_t slv_addr)
{
	int32_t ercd = E_OK;

	emsk_imu_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor!=NULL);

	ercd = emsk_imu_sensor->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));
		imu_sensor_slvaddr = slv_addr;
		
		/******set mpu6050 reg ******/
		mpu6050_reg_write(imu_init_seq1, 2);
		
		mpu6050_reg_write(imu_init_seq0, 5);

	}

error_exit:
	return ercd;
}

/** val is the return mpu6050		 */
/**
 * \brief	read mpu6050 sensor value
 * \param[out]	val	return mpu6050 (accel,gyro,temp)
 * \retval	E_OK	read success
 * \retval	!E_OK	read failed
 */
int32_t imu_sensor_read(imu_values* imu_val)
{
	int32_t ercd = E_OK;

	emsk_imu_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor!=NULL);

	ercd = mpu6050_reg_read(MPU6050_REG_ACCEL_XOUT_H, imu_data.buf, 14);//ercd = 14

	if(ercd != 14) {
		ercd = E_OBJ;
	} else {
		ercd = E_OK;
		imu_val->accl_x = accl_unit * (float)(short)((imu_data.axh << 8) | imu_data.axl);
		imu_val->accl_y = accl_unit * (float)(short)((imu_data.ayh << 8) | imu_data.ayl); 
		imu_val->accl_z = accl_unit * (float)(short)((imu_data.azh << 8) | imu_data.azl);

		imu_val->angv_x = gyro_unit * (float)(short)((imu_data.gxh << 8) | imu_data.gxl);
		imu_val->angv_y = gyro_unit * (float)(short)((imu_data.gyh << 8) | imu_data.gyl);
		imu_val->angv_z = gyro_unit * (float)(short)((imu_data.gzh << 8) | imu_data.gzl);

		imu_val->temp = (float)(short)((imu_data.th << 8) | imu_data.tl) * 2.941176471e-3f + 36.53f;
	}

error_exit:
	return ercd;
}

/** val is the return mpu6050(accel)	 */
/**
 * \brief	read mpu6050 sensor value
 * \param[out]	val	return mpu6050 (accel)
 * \retval	E_OK	read success
 * \retval	!E_OK	read failed
 */
int32_t accel_sensor_read(acc_values* accel_val)
{
	int32_t ercd = E_OK;
	char str[20];

	emsk_imu_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	ercd = mpu6050_reg_read(MPU6050_REG_ACCEL_XOUT_H, acc_data.buf, 6);//ercd = 14

	if(ercd != 6) {
		ercd = E_OBJ;
	} else {
		ercd = E_OK;
		accel_val->accl_x = accl_unit * (float)(short)((acc_data.axh << 8) | acc_data.axl);
		accel_val->accl_y = accl_unit * (float)(short)((acc_data.ayh << 8) | acc_data.ayl); 
		accel_val->accl_z = accl_unit * (float)(short)((acc_data.azh << 8) | acc_data.azl);
	}

error_exit:
	return ercd;
}

/** @} end of group BOARD_EMSK_IMU */

