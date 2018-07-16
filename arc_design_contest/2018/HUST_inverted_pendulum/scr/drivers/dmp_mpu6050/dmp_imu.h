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

extern int mpu6050_reg_write(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char const *data);
extern int mpu6050_reg_read(unsigned char slave_addr, unsigned char reg_addr, unsigned char length, unsigned char *data);
extern int board_get_ms(unsigned long *count);
extern int mpu6050_enable(void);
#endif  /* _IMU_H_ */

