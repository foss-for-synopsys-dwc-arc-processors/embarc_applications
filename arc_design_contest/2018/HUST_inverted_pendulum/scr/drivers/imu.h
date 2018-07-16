#ifndef _IMU_H_
#define _IMU_H_

/*!< using IIC 1 interface */
#define IMU_VERTICAL_IIC_ID  	DW_IIC_0_ID
#define IMU_HORIZONTAL_IIC_ID  	DW_IIC_1_ID

/**
 * I2C address of PmodIMU2 can be selected via AD0: 0x68, 0x69
 * AD0 connect to GND is 0x68
 */
/*!< MPU6050 address */
#define MPU6050_A0_PIN    0 /*!< I2C Serial Bus Address Selection Pin */
#define MPU6050_ADDRESS   (0x68 + MPU6050_A0_PIN)

/*!< imu sensor address */
#define IMU_SENSOR_ADDR	  MPU6050_ADDRESS

#define EMSK_IMU_SENSOR_CHECK_EXP_NORTN(EXPR)    CHECK_EXP_NOERCD(EXPR, error_exit)

/*!< struct for imu data storaged */
typedef struct __imuValues
{
	short acclX, acclY, acclZ;
    short angvX, angvY, angvZ;
} ImuValues;

int imuInit(uint8_t i2cId);
int imuGetValues(ImuValues* imuVal, uint8_t i2cId);

#endif  /* _IMU_H_ */

