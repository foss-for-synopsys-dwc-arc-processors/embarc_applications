/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "imu.h"


/* MPU6050 registers */
#define MPU6050_REG_SAMPLE_RATE          0x19 /* Gyro sample rate */
#define MPU6050_REG_EXT_SYNC_DISABLE     0x1A /* Ext sync disable */
#define MPU6050_REG_GYRO_FULL_SCALE      0x1B /* Gyro full scale  */
#define MPU6050_REG_ACCEL_FULL_SCALE     0x1C /* Accel full scale */
#define MPU6050_REG_POWER_MANAGEMENT     0x6B /* Power management */
#define MPU6050_REG_ACCEL_XOUT_H         0x3B /* Accel_x out[15:8] */
#define MPU6050_REG_ACCEL_XOUT_L         0x3C /* Accel_x out[7:1] */
#define MPU6050_REG_ACCEL_YOUT_H         0x3D /* Accel_y out[15:8] */
#define MPU6050_REG_ACCEL_YOUT_L         0x3E /* Accel_y out[7:1] */
#define MPU6050_REG_ACCEL_ZOUT_H         0x3F /* Accel_z out[15:8] */
#define MPU6050_REG_ACCEL_ZOUT_L         0x40 /* Accel_z out[7:1] */
#define MPU6050_REG_TEMP_OUT_H           0x41 /* Temp out[15:8] */
#define MPU6050_REG_TEMP_OUT_L           0x42 /* Temp out[7:1] */
#define MPU6050_REG_GYRO_XOUT_H          0x43 /* Gyro_x out[15:8] */
#define MPU6050_REG_GYRO_XOUT_L          0x44 /* Gyro_x out[7:1] */
#define MPU6050_REG_GYRO_YOUT_H          0x45 /* Gyro_y out[15:8] */
#define MPU6050_REG_GYRO_YOUT_L          0x46 /* Gyro_y out[7:1] */
#define MPU6050_REG_GYRO_ZOUT_H	     	 0x47 /* Gyro_z out[15:8] */
#define MPU6050_REG_GYRO_ZOUT_L          0x48 /* Gyro_z out[7:1] */

/* imu data received buffer */
union _imuData
{
    char buf[14];
    //char buf[14];
    struct
    {
        char axh, axl, ayh, ayl, azh, azl;
        char th, tl;
        char gxh, gxl, gyh, gyl, gzh, gzl;
    };
} imuData;

/* configure related register, using the auto increase function */
static uint8_t imu_init_seq0[] = {
	MPU6050_REG_SAMPLE_RATE,
	0x00, /* 0x19: sr = gyro rate / 1 */
	0x01, /* 0x1A: ext sync disable, dlpf = 1(accl 1k, gyro 1k) */
	0x18, /* 0x1B: gyro fs = 2k deg/sec (34.6/s)*/
	0x00  /* 0x1C: accl fs = 2g (19.6m/sq.s) */
};

static uint8_t imu_init_seq1[] = {
	MPU6050_REG_POWER_MANAGEMENT,
	0x00 /* 0x6B: no sleep */
};

static DEV_IIC  *emsk_imu_sensor; /* IMU6050 sensor object */

/**
 * \brief	write mpu6050 register
 * \param 	*seq 	register address and value to be written
 * \param   len     the length of seq
 * \retval	>=0	write success, return bytes written
 * \retval	!E_OK	write failed
 */
static int32_t mpu6050_reg_write(uint8_t *seq, uint8_t len, uint8_t i2cId)
{
	int32_t ercd = E_PAR;

	emsk_imu_sensor = iic_get_dev(i2cId);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor != NULL);

	/* make sure set the imu sensor's slave address */
	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(IMU_SENSOR_ADDR));

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
static int32_t mpu6050_reg_read(uint8_t seq, uint8_t *val, uint8_t len, uint8_t i2cId)
{
	int32_t ercd = E_PAR;

	emsk_imu_sensor = iic_get_dev(i2cId);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor != NULL);

	/** make sure set the temp sensor's slave address */
	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(IMU_SENSOR_ADDR));
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
 * \brief	imu sensor initialize
 * \retval	E_OK	initialize success
 * \retval	!E_OK	initialize failed
 */
int imuInit(uint8_t i2cId)
{
	int ercd = E_OK;

	emsk_imu_sensor = iic_get_dev(i2cId);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor != NULL);

	/* the device working as master */
	ercd = emsk_imu_sensor->iic_open(DEV_MASTER_MODE, IIC_SPEED_FAST);
	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(IMU_SENSOR_ADDR));

		/* write value to mpu6050 to set registers */
		mpu6050_reg_write(imu_init_seq1, 2, i2cId);
		mpu6050_reg_write(imu_init_seq0, 5, i2cId);
	}

error_exit:
	return ercd;
}

/**
 * \brief	read imu sensor value
 * \param	receive imu data(raw value)
 * \retval	E_OK	read success
 * \retval	!E_OK	read failed
 */
int imuGetValues(ImuValues* imuVal, uint8_t i2cId)
{
	int ercd = E_OK;

	/* read 14 data from mpu6050 */
	ercd = mpu6050_reg_read(MPU6050_REG_ACCEL_XOUT_H, (uint8_t *)(imuData.buf), 14, i2cId);

	if (ercd != 14) {
		ercd = E_OBJ;
	} else {
	    imuVal->acclX = (short)((imuData.axh << 8) | imuData.axl);
	    imuVal->acclY = (short)((imuData.ayh << 8) | imuData.ayl);
	    imuVal->acclZ = (short)((imuData.azh << 8) | imuData.azl);

	    imuVal->angvX = (short)((imuData.gxh << 8) | imuData.gxl);
	    imuVal->angvY = (short)((imuData.gyh << 8) | imuData.gyl);
	    imuVal->angvZ = (short)((imuData.gzh << 8) | imuData.gzl);

		ercd = E_OK;
	}

error_exit:
	return ercd;
}
