/**
	*MPU6050 init and red data functions.
	*Writer:Liruige
	*data:2017-3-20 Mr.WangS modify
*/
#ifndef _MPU6050_H_
#define _MPU6050_H_

#include "embARC.h"
#include "embARC_debug.h"
/*MPU6050 internal register address macro definition*/

/**Gyroscope sampling rate, typical: 0x07 (125Hz)*/
#define	SMPLRT_DIV				0x19
/**Low pass filter frequency, typical value: 0x06 (5Hz)*/
#define	LPF_CONFIG				0x1A	
#define	MPU_GYRO_CFG_REG		0x1B	
#define	MPU_ACCEL_CFG_REG	    0x1C	
#define MPU_INT_EN_REG			0X38	
#define MPU_USER_CTRL_REG		0X6A 	
#define MPU_FIFO_EN_REG			0X23	
#define MPU_INTBP_CFG_REG		0X37  	
#define MPU_PWR_MGMT1_REG		0x6B  	
#define MPU_PWR_MGMT2_REG		0X6C
/**X-axis acceleration measurement value register,ACCEL_XOUT_H is those high 8 bits*/
#define	ACCEL_XOUT_H      	    0x3B
#define ACCEL_XOUT_L	        0x3C
/**Y-axis acceleration measurement value register,ACCEL_YOUT_H is those high 8 bits*/
#define	ACCEL_YOUT_H	        0x3D
#define ACCEL_YOUT_L	        0x3E
/**Z-axis acceleration measurement value register,ACCEL_ZOUT_H is those high 8 bits*/
#define	ACCEL_ZOUT_H	        0x3F
#define ACCEL_ZOUT_L	        0x40
#define	TEMP_OUT_H				0x41
#define	TEMP_OUT_L				0x42
#define	GYRO_XOUT_H				0x43
#define	GYRO_XOUT_L				0x44	
#define GYRO_YOUT_H				0x45
#define GYRO_YOUT_L				0x46
#define GYRO_ZOUT_H				0x47
#define	GYRO_ZOUT_L				0x48
#define	WHO_AM_I				0x75
/**IIC write when the address byte data, + 1 for the read*/
#define	MUPADDRESS				0xD0	//IIC写入时的地址字节数据，+1为读取

#define MPU6050_IIC_NUM         DW_IIC_0_ID

/**MPU6050 init function.parameter:gyro_rng,gyro parameter setting,0(250/s),1(500/2),2(1000/s),3(2000/s)*/
void mpu6050_init(uint8_t gyro_rng,uint8_t accel_rng); 
/*Get the specify register data.*/
int mpu6050_get_data(uint8_t regaddr);
/*Make the IIC point to mpu.*/
void iic_point_mpu(void);

#endif