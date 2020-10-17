#include "IIC.h"

DEV_IIC * iic_master_0;



/*****************
 * I2C device initialization
 * I2C_0
 * Conenction:
 * J2.6: 3V3
 * J2.5: GND
 * J2.4: SDA
 * J2.3: SCL
 ****************/


void Master0_slvdev_init(uint32_t slv_addr)
{
	iic_master_0 = iic_get_dev(IIC_MASTER_0);/* get IIC Master_0 device */

	iic_master_0->iic_open(DEV_MASTER_MODE, IIC_SPEED_FAST);/* open IIC device in master mode */

	iic_master_0->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));/* Set target slave device address */
}

/*write data to slvdev*/
int32_t Master0_slvdev_write_reg(uint8_t regaddr, uint8_t regvalue)
{
	uint8_t buf[2]; /* reg buffer */

	iic_master_0->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));/* Set next condition for following transmit operation */
	buf[0] = regaddr;
	buf[1] = regvalue;
	return iic_master_0->iic_write(buf, 2);
}

/*read data from slvdev*/
int32_t Master0_slvdev_read_reg(uint8_t regaddr, uint8_t *data)
{
	uint8_t buf[1];
	int32_t ercd;

	iic_master_0->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	buf[0] = regaddr;
	iic_master_0->iic_write(buf, 1); /* Master write IIC_slv addr to IIC wire */
	iic_master_0->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = iic_master_0->iic_read(buf, 1); /* Read data from slvdev */
	*data = buf[0];

	return ercd;
}


