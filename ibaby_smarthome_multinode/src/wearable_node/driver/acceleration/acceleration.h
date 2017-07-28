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

/*!< using IIC 1 interface */
#define ACC_SENSOR_IIC_ID  DW_IIC_1_ID

/**
 * I2C address of PmodIMU4 can be selected via AD0: 0x68, 0x69
 * AD0 connect to GND is 0x68
 */
/*!< MPU6050 address */
#define MPU6050_A0_PIN    0 /*!< I2C Serial Bus Address Selection Pin */
#define MPU6050_ADDRESS   (0x68 + MPU6050_A0_PIN)

/*!< acceleration sensor address */
#define ACC_SENSOR_ADDR	  MPU6050_ADDRESS

#define EMSK_IMU_SENSOR_CHECK_EXP_NORTN(EXPR)    CHECK_EXP_NOERCD(EXPR, error_exit)

/*!< struct for acceleration data storaged */
typedef struct __accValues
{
	float accl_x, accl_y, accl_z;
} acc_values;

extern int32_t acc_sensor_init(uint32_t slv_addr);
extern int32_t acc_sensor_read(acc_values* accel_val);

#endif  /* _WEARABLE_NODE_DRIVER_ACCELERATION_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */
