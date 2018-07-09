/*
 * i2c.c
 *
 *  Created on: 2018年5月23日
 *      Author: Administrator
 */

#include "i2c.h"
#include "timer.h"


#define TASK_CRITICAL 0//是否进入临界区保护时序

#define I2C_PORT		DEV_GPIO_PORT_C
#define GPIO_I2C_MASK		0x00030000		//portA的16-17Pin，接J3的7-8口
#define GPIO_I2C_LOW	  (0)
#define GPIO_I2CSDA_MASK	0X00010000		//portA的16Pin，接J3的7口
#define GPIO_I2CSCL_MASK	0X00020000		//portA的17Pin，接J3的8口

	#if TASK_CRITICAL
#include "task_config.h"
	#endif

DEV_GPIO * iic_cm3232;

void I2CC_Init(void)
{
	uint32_t temp = 0;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 							//PC12 11
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					//推挽输出
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
	iic_cm3232 = gpio_get_dev(I2C_PORT);

//	EMSK_GPIO_CHECK_EXP_NORTN(uart4 != NULL);
	if (iic_cm3232->gpio_open(GPIO_I2C_MASK) == E_OPNED) {
		iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_I2CSCL_MASK));
		iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(GPIO_I2CSDA_MASK));
		iic_cm3232->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_I2C_MASK));
	}
	iic_cm3232->gpio_write(GPIO_I2CSCL_MASK,GPIO_I2CSCL_MASK);
//	iic_cm3232->gpio_write(GPIO_I2CSDA_MASK,GPIO_I2CSDA_MASK);
	iic_cm3232->gpio_read(&temp,GPIO_I2CSDA_MASK);
//	printf("%d",temp);

//	I2C_SCL=1;
//	I2C_SDA=1;
}

void I2C_Start(void)
{
	#if TASK_CRITICAL
	taskENTER_CRITICAL();           //进入临界区
	#endif
	iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_I2CSDA_MASK));		//sda线输出
	iic_cm3232->gpio_write(GPIO_I2CSDA_MASK,GPIO_I2CSDA_MASK);		//	I2C_SDA=1;
	iic_cm3232->gpio_write(GPIO_I2CSCL_MASK,GPIO_I2CSCL_MASK);		//	I2C_SCL=1;
	delay_us(4);
	iic_cm3232->gpio_write((~GPIO_I2CSDA_MASK),GPIO_I2CSDA_MASK);		//	I2C_SDA=0;
	delay_us(4);
	iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
	#if TASK_CRITICAL
	 taskEXIT_CRITICAL();            //退出临界区
	#endif
}

void I2C_Stop(void)
{
	#if TASK_CRITICAL
	taskENTER_CRITICAL();           //进入临界区
	#endif
	iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_I2CSDA_MASK));
	iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
	iic_cm3232->gpio_write((~GPIO_I2CSDA_MASK),GPIO_I2CSDA_MASK);		//	I2C_SDA=0;
	delay_us(4);
	iic_cm3232->gpio_write(GPIO_I2CSCL_MASK,GPIO_I2CSCL_MASK);		//	I2C_SCL=1;
	iic_cm3232->gpio_write(GPIO_I2CSDA_MASK,GPIO_I2CSDA_MASK);		//	I2C_SDA=1;
	delay_us(4);
	#if TASK_CRITICAL
	 taskEXIT_CRITICAL();            //退出临界区
	#endif
}

uint8_t I2C_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	uint32_t	temp=1;
	#if TASK_CRITICAL
	taskENTER_CRITICAL();           //进入临界区
	#endif
	iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_I2CSDA_MASK));
	iic_cm3232->gpio_write(GPIO_I2CSDA_MASK,GPIO_I2CSDA_MASK);		//	I2C_SDA=1;
	delay_us(1);
	iic_cm3232->gpio_write(GPIO_I2CSCL_MASK,GPIO_I2CSCL_MASK);		//	I2C_SCL=1;
	delay_us(1);
	#if TASK_CRITICAL
	 taskEXIT_CRITICAL();            //退出临界区
	#endif
		iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(GPIO_I2CSDA_MASK));
	 iic_cm3232->gpio_read(&temp,GPIO_I2CSDA_MASK);
		temp = temp & GPIO_I2CSDA_MASK;
		temp = temp >> 16;

	while(temp)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			I2C_Stop();
			return 1;
		}
		iic_cm3232->gpio_read(&temp,GPIO_I2CSDA_MASK);
		temp = (~temp) & GPIO_I2CSDA_MASK;
		temp = temp >> GPIO_I2CSDA_MASK;
		printf("wait_read_loop:%d\r\n",temp);
	}
	iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
	return 0;
}

void I2C_Ack(void)
{
	#if TASK_CRITICAL
	taskENTER_CRITICAL();           //进入临界区
	#endif
	iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
	iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_I2CSDA_MASK));
	iic_cm3232->gpio_write((~GPIO_I2CSDA_MASK),GPIO_I2CSDA_MASK);		//	I2C_SDA=0;
	delay_us(2);
		iic_cm3232->gpio_write(GPIO_I2CSCL_MASK,GPIO_I2CSCL_MASK);		//	I2C_SCL=1;
	delay_us(2);
	iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
	#if TASK_CRITICAL
	 taskEXIT_CRITICAL();            //退出临界区
	#endif
}

void I2C_NAck(void)
{
	#if TASK_CRITICAL
	taskENTER_CRITICAL();           //进入临界区
	#endif
	iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
	iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_I2CSDA_MASK));
	iic_cm3232->gpio_write(GPIO_I2CSDA_MASK,GPIO_I2CSDA_MASK);		//	I2C_SDA=1;
	delay_us(2);
		iic_cm3232->gpio_write(GPIO_I2CSCL_MASK,GPIO_I2CSCL_MASK);		//	I2C_SCL=1;
	delay_us(2);
	iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
	#if TASK_CRITICAL
	 taskEXIT_CRITICAL();            //退出临界区
	#endif
}

void I2C_Send_Byte(uint8_t txd)
{
	uint8_t t;
		#if TASK_CRITICAL
	taskENTER_CRITICAL();           //进入临界区
	#endif
	iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_I2CSDA_MASK));
	iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
	for(t=0;t<8;t++)
	{
		iic_cm3232->gpio_write((((txd&0x80)>>7)<<16),GPIO_I2CSDA_MASK);
		txd<<=1;
		delay_us(2);
		iic_cm3232->gpio_write(GPIO_I2CSCL_MASK,GPIO_I2CSCL_MASK);		//	I2C_SCL=1;
		delay_us(2);
		iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
		delay_us(2);
	}
	#if TASK_CRITICAL
	 taskEXIT_CRITICAL();            //退出临界区
	#endif
}

uint8_t I2C_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	uint32_t	temp=0;
	#if TASK_CRITICAL
	taskENTER_CRITICAL();           //进入临界区
	#endif
	iic_cm3232->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(GPIO_I2CSDA_MASK));
	for(i=0;i<8;i++)
	{
		iic_cm3232->gpio_write((~GPIO_I2CSCL_MASK),GPIO_I2CSCL_MASK);		//	I2C_SCL=0;
		delay_us(2);
		iic_cm3232->gpio_write(GPIO_I2CSCL_MASK,GPIO_I2CSCL_MASK);		//	I2C_SCL=1;
		receive<<=1;
		iic_cm3232->gpio_read(&temp,GPIO_I2CSDA_MASK);
		temp = temp & GPIO_I2CSDA_MASK;
		temp = temp >> 16;
//		 printf("wait_read:%d\r\n",temp);
		receive += temp;
		delay_us(1);
	}
	#if TASK_CRITICAL
	 taskEXIT_CRITICAL();            //退出临界区
	#endif
	if(!ack)
		I2C_NAck();
	else
		I2C_Ack();
	return receive;
}


