
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

/* custom HAL */
#include "mpu6050.h"


/**
 * MPU6050 internal register address macro definition
 * Gyroscope sampling rate, typical: 0x07 (125Hz)
 */

#define	SMPLRT_DIV			0x19

/* Low pass filter frequency, typical value: 0x06 (5Hz) */
#define	LPF_CONFIG			0x1A	
#define	MPU_GYRO_CFG_REG		0x1B	
#define	MPU_ACCEL_CFG_REG	    	0x1C	
#define MPU_INT_EN_REG			0X38	
#define MPU_USER_CTRL_REG		0X6A 	
#define MPU_FIFO_EN_REG			0X23	
#define MPU_INTBP_CFG_REG		0X37  	
#define MPU_PWR_MGMT1_REG		0x6B  	
#define MPU_PWR_MGMT2_REG		0X6C

#define	TEMP_OUT_H			0x41
#define	TEMP_OUT_L			0x42
#define	GYRO_XOUT_H			0x43
#define	GYRO_XOUT_L			0x44	
#define GYRO_YOUT_H			0x45
#define GYRO_YOUT_L			0x46
#define GYRO_ZOUT_H			0x47
#define	GYRO_ZOUT_L			0x48
#define	WHO_AM_I			0x75



static DEV_IIC *mpu_iic_port;
static uint32_t point = (MUPADDRESS>>1);

/** Initialize the IIC device and set the slave address to MPU6050. */
static void iic_mpu6050_int(void)
{
	mpu_iic_port = iic_get_dev(MPU6050_IIC_NUM);
	
	/* Set the IIC speed as host standard speed */
	mpu_iic_port->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	mpu_iic_port->iic_control(IIC_CMD_ENA_DEV, (void *)point);
	mpu_iic_port->iic_control(IIC_CMD_MST_SET_TAR_ADDR,(void *)point);

}

/**
 * \brief 		MPU6050 write register function.
 * \parameter regaddr	The address of the register that you want to write.
 * \parameter val	The valuation that you want to write.
 */
static void mpu6050_write_reg(uint8_t regaddr,uint8_t val)
{
	uint8_t send_data[2];
	uint8_t point;
	
	/* Place the register address value and the data you want to send into the array */
	send_data[0] = regaddr;
	send_data[1] = val;
	
	/* Set the IIC send end operation to stop */
	mpu_iic_port->iic_control( IIC_CMD_MST_SET_NEXT_COND,(void*)(IIC_MODE_STOP));
	mpu_iic_port->iic_write((void*)send_data,2);
}

/**
 * \brief 		MPU6050 read register function.
 * \parameter regaddr	The address of the register that you want to write.
 * \retval		The valuation of the register.
*/
static uint8_t mpu6050_read_reg(uint8_t regaddr)
{
	uint8_t REG_data;
	uint8_t send_data = regaddr;

	mpu_iic_port->iic_control( IIC_CMD_MST_SET_NEXT_COND,(void*)(IIC_MODE_RESTART));
	mpu_iic_port->iic_write((void*)(&send_data),1);
	mpu_iic_port->iic_control( IIC_CMD_MST_SET_NEXT_COND,(void*)(IIC_MODE_STOP));
	mpu_iic_port->iic_read((void*)(&REG_data), 1);

	return REG_data;
}

/**
 * \brief	MPU6050 init.
 * \parameter gyro_rng	Set the range of gyro,0(250 degree/s),1(500 degree/2),2(1000 degree/s),3(2000 degree/s).
 * \parameter accel_rng	Set the range of accelerometer,0(2g),1(4g),2(8g),3(16g).
 */
void mpu6050_init(uint8_t gyro_rng,uint8_t accel_rng)
{  
	iic_mpu6050_int();
	/* Reset MPU6050 */
	mpu6050_write_reg(MPU_PWR_MGMT1_REG,0X80); 
	vTaskDelay(100);   
	/* Wake up MPU6050 */           
	mpu6050_write_reg( MPU_PWR_MGMT1_REG, 0x07 );	
	/* Set sampling frequency divider */
	mpu6050_write_reg( SMPLRT_DIV, 0x06);  
	/* Set the low-pass filter */   
	mpu6050_write_reg( LPF_CONFIG, 0x06 );
	/* Turn off all interrupts */    
	mpu6050_write_reg(MPU_INT_EN_REG,0x00);     
	/* Turn off IIC master mode */
	mpu6050_write_reg(MPU_USER_CTRL_REG,0X00); 
	/* Close fifo */ 
	mpu6050_write_reg(MPU_FIFO_EN_REG,0X00);
	/* Set interrupt pin active low */
	mpu6050_write_reg(MPU_INTBP_CFG_REG,0X80); 
	/* Set CLKSEL, PLL, X axis for reference */   
	mpu6050_write_reg(MPU_PWR_MGMT1_REG,0X01); 
	/* Enable accelerometer and gyroscope */ 
	mpu6050_write_reg(MPU_PWR_MGMT2_REG,0X00);
	/* Configure the gyroscope full scale */
	mpu6050_write_reg( MPU_GYRO_CFG_REG,(gyro_rng << 3));
	/* Configure the accelerometer full scale */
	mpu6050_write_reg( MPU_ACCEL_CFG_REG,(accel_rng << 3)); 
}

/** 
 * \brief	Set the IIC slave address to MPU6050 
 */
void iic_point_mpu(void)
{
	mpu_iic_port->iic_control(IIC_CMD_MST_SET_TAR_ADDR,(void *)point);
	if (mpu_iic_port->iic_control(IIC_CMD_MST_SET_TAR_ADDR,(void *)point) == E_OK) {
		//EMBARC_PRINTF("Target reset mpu6050 OK!\n");
	}
}

/**
 *\brief	Get the MPU6050 data.
 * \parameter	regaddr	the address of the register that you want to get.
 * \retval	data 	the data you get after processing
 */
int mpu6050_get_data(uint8_t regaddr)
{
	int data;
	uint16_t data_temp;
	uint8_t H = 0, L = 0;
	
	/* Read high 8 bits data */
	H = mpu6050_read_reg(regaddr);
	
	/* Read low 8 bits data */
	L = mpu6050_read_reg(regaddr + 1);
	data_temp = (H << 8) | L; 
	
	/* Change the data into a signed integer */
	if(data_temp < 32768) {
		data = data_temp;
	} else {
		data = -(65535 - data_temp);
	}
	return data;
}

/** @} */