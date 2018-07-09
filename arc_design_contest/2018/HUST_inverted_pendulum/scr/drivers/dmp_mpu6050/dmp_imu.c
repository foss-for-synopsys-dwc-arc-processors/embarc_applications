/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "imu.h"

static DEV_IIC  *emsk_imu_sensor; /* IMU6050 sensor object */

int board_get_ms(unsigned long *count)
{
	count = (unsigned long)OSP_GET_CUR_MS();
	return 0;
}

/**
 * \brief	write mpu6050 register
 * \param   slv_addr 	slave address
 * \param 	reg_addr	register address
 * \param   length		the length of 
 * \param 	*data 		data to be written
 * \retval	>=0			write success, return bytes written
 * \retval	!E_OK		write failed
 */
int mpu6050_reg_write(unsigned char slave_addr, unsigned char reg_addr,
 						unsigned char length, unsigned char const *data)
{
	int32_t ercd = E_OK;

	emsk_imu_sensor = iic_get_dev(DW_IIC_0_ID);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor != NULL);

	/* make sure set the imu sensor's slave address */
	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slave_addr));
	/** write register address then read register value */
	ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	ercd = emsk_imu_sensor->iic_write(&reg_addr, 1);
	ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_imu_sensor->iic_write(data, length);

	if(ercd == length){
		ercd = E_OK;
	}
error_exit:
	return ercd;
}

/**
 * \brief	read mpu6050 register
 * \param   slv_addr 	slave address
 * \param 	reg_addr 	register address
 * \param   length   	the length of seq
 * \param 	*data	 	data
 * \retval	>=0			read success, return bytes read
 * \retval	!E_OK		read failed
 */
 int mpu6050_reg_read(unsigned char slave_addr, unsigned char reg_addr,
 						unsigned char length, unsigned char *data)
{
	int32_t ercd = E_OK;

	emsk_imu_sensor = iic_get_dev(DW_IIC_0_ID);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor != NULL);

	/** make sure set the temp sensor's slave address */
	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slave_addr));
	/* write register address then read register value */
	ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));

	/* send data reading request to mpu6050 */
	ercd = emsk_imu_sensor->iic_write(&reg_addr, 1);

	emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	/* read len data from mpu6050 */
	ercd = emsk_imu_sensor->iic_read(data, length);

	if(ercd == length){
		ercd = E_OK;
	}
	
error_exit:
	return ercd;
}


/**
 * \brief	imu sensor initialize
 * \retval	E_OK	initialize success
 * \retval	!E_OK	initialize failed
 */
int mpu6050_enable(void)
{
	int ercd = E_OK;

	emsk_imu_sensor = iic_get_dev(DW_IIC_0_ID);

	EMSK_IMU_SENSOR_CHECK_EXP_NORTN(emsk_imu_sensor != NULL);

	/* the device working as master */
	ercd = emsk_imu_sensor->iic_open(DEV_MASTER_MODE, IIC_SPEED_FAST);
	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = emsk_imu_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(IMU_SENSOR_ADDR));
	}

error_exit:
	return ercd;
}



