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
 * \defgroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER        embARC iBaby Smarthome Node Wearable Node Driver
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief	emsk acceleration sensor driver for ibaby wearable node
 * \details
 *		realize driver for acceleration sensor using designware iic driver
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief	emsk acceleration sensor driver for ibaby wearable node
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * @{
 */

/* standard C HAL */
#include <stdio.h>

/* embARC HAL */
#include "arc.h"
#include "arc_builtin.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "dev_iic.h"
#include "board.h"

#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "acceleration.h"


/**
 * \brief	write mpu6050 register
 * \param 	*seq 	register address and value to be written
 * \param       len     the length of seq
 * \retval	>=0	write success, return bytes written
 * \retval	!E_OK	write failed
 */
static int32_t mpu6050_reg_write(uint8_t *seq, uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_imu_sensor = iic_get_dev(ACC_SENSOR_IIC_ID);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor != NULL);

	/* make sure set the imu sensor's slave address */
	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(acc_sensor_addr));

	ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_imu_sensor->iic_write(seq, len);

error_exit:
	return ercd;
}

/**
 * \brief	read mpu6050 register
 * \param 	seq	register address
 * \param 	*val    returned value
 * \param 	len     the length of returned value
 * \retval	>=0	read success, return bytes read
 * \retval	!E_OK	read failed
 */
static int32_t mpu6050_reg_read(uint8_t seq, uint8_t *val, uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_imu_sensor = iic_get_dev(ACC_SENSOR_IIC_ID);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor != NULL);

	/** make sure set the temp sensor's slave address */
	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(acc_sensor_addr));
	/* write register address then read register value */
	ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));

	/* send data reading request to mpu6050 */
	ercd = emsk_imu_sensor->iic_write(&seq, 1);

	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	/* read len data from mpu6050 */
	ercd = emsk_imu_sensor->iic_read(val, len);
	
error_exit:
	return ercd;
}

/**
 * \brief	acceleration sensor initialize
 * \param[in]	slv_addr	imu sensor iic slave address
 * \retval	E_OK	init success
 * \retval	!E_OK	init failed
 */
extern int32_t acc_sensor_init(uint32_t slv_addr)
{
	int32_t ercd = E_OK;

	emsk_imu_sensor = iic_get_dev(ACC_SENSOR_IIC_ID);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor != NULL);

	/* the device working as master */
	ercd = emsk_imu_sensor->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));
		acc_sensor_addr = slv_addr;
		
		/* write value to mpu6050 to set registers */
		mpu6050_reg_write(imu_init_seq1, 2);
		mpu6050_reg_write(imu_init_seq0, 5);
	}

error_exit:
	return ercd;
}

/** val is the return acceleration data */
/**
 * \brief	read acceleration sensor value
 * \param[out]	val	return acceleration data
 * \retval	E_OK	read success
 * \retval	!E_OK	read failed
 */
extern int32_t acc_sensor_read(acc_values* acc_temp)
{
	int32_t ercd = E_OK;

	emsk_imu_sensor = iic_get_dev(ACC_SENSOR_IIC_ID);

	/* read 6 data from mpu6050 */
	ercd = mpu6050_reg_read(MPU6050_REG_ACCEL_XOUT_H, acc_data.buf, 6);

	if(ercd != 6) {
		ercd = E_OBJ;
	} else {
		acc_temp->accl_x = acc_unit * (float)(short)((acc_data.axh << 8) | acc_data.axl);
		acc_temp->accl_y = acc_unit * (float)(short)((acc_data.ayh << 8) | acc_data.ayl); 
		acc_temp->accl_z = acc_unit * (float)(short)((acc_data.azh << 8) | acc_data.azl);

		ercd = E_OK;
	}

error_exit:
	return ercd;
}

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */

