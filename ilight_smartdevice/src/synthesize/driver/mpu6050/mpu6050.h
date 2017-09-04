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
 * \version 2017.03
 * \date 2017-08-20
 * \author Zhiwei Zhang
--------------------------------------------- */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief	emsk mpu6050 sensor driver for iLight
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */


#ifndef _MPU6050_H_
#define _MPU6050_H_

/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/*!<IIC write when the address byte data, + 1 for the read */
#define	MUPADDRESS		0xD0	
/* X-axis acceleration measurement value register,ACCEL_XOUT_H is those high 8 bits */
#define	ACCEL_XOUT_H			0x3B
#define ACCEL_XOUT_L			0x3C
/* Y-axis acceleration measurement value register,ACCEL_YOUT_H is those high 8 bits */
#define	ACCEL_YOUT_H			0x3D
#define ACCEL_YOUT_L			0x3E
/* Z-axis acceleration measurement value register,ACCEL_ZOUT_H is those high 8 bits */
#define	ACCEL_ZOUT_H			0x3F
#define ACCEL_ZOUT_L			0x40

/*!<using IIC interface 0 */
#define MPU6050_IIC_NUM         DW_IIC_0_ID

/*!<MPU6050 init function.parameter:gyro_rng,gyro parameter setting,0(250/s),1(500/2),2(1000/s),3(2000/s) */
void mpu6050_init(uint8_t gyro_rng,uint8_t accel_rng); 
/*!<Get the specify register data */
int mpu6050_get_data(uint8_t regaddr);
/*!<Make the IIC point to mpu */
void iic_point_mpu(void);

#endif /* _MPU6050_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE */