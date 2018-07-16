#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include <string.h>
#include "arc.h"
#include "GPRS.h"
#define I2C_SLAVE_ADDR2  0x01
DEV_IIC *dev_iic_gprs=NULL;
 uint8_t *phoneNum = "00310038003700300030003800310033003000360031"; 
uint8_t *msg = "4E3B4EBAFF0C4E3B4EBAFF0C621189818DDF4E224F604E86";

static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}

void call(void)
{	
	
	dev_iic_gprs = iic_get_dev(DW_IIC_0_ID);
    dev_iic_gprs->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_iic_gprs->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR2));
	uint8_t i;
	int8_t *cmd[2]={"ATD18829283316;\r\n",};
	for(i = 0; i < 4; i++)
	{
		dev_iic_gprs->iic_write(cmd[i], strlen(cmd[i]));
		delay_ms(100);
	}	
	uint8_t end[]={0x1a,0x00};
	dev_iic_gprs->iic_write(end, strlen(end));
printf("end1\r\n");

}

void send(uint8_t *phoneNum, uint8_t *msg)
{	
	dev_iic_gprs = iic_get_dev(DW_IIC_0_ID	);
    dev_iic_gprs->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_iic_gprs->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR2));
	
	uint8_t i;
	uint8_t pnum[64];
	int8_t *cmd[5]={"AT+CMGF=1\r\n", "AT+CSCS=\"UCS2\"\r\n",
				  "AT+CSCA?\r\n",  "AT+CSMP=17,167,0,25\r\n",};
	snprintf(pnum, 64, "AT+CMGS=\"%s\"\r\n", phoneNum);
	for(i = 0; i < 4; i++)
	{
	dev_iic_gprs->iic_write(cmd[i], strlen(cmd[i]));
		delay_ms(100);
	}	
	dev_iic_gprs->iic_write(pnum, strlen(pnum));
	delay_ms(100);
	dev_iic_gprs->iic_write(msg, strlen(msg));
	delay_ms(100);
	uint8_t end[]={0x1a,0x00};
	dev_iic_gprs->iic_write(end, strlen(end));
	printf("end\r\n");
}

