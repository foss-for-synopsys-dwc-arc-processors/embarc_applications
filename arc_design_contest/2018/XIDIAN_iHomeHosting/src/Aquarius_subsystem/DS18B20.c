#include "embARC.h"
#include "embARC_debug.h"
#include <stdio.h>
#include "dev_gpio.h"

#include "DS18B20.h"




//延时函数，设y是实际延迟时间，x是设置值，则y=0.87911111*x
//如需延时50us，即y=50,则x=（y/0.87911111）取整
static void delay_us( uint32_t us) 
{
	uint32_t volatile x,y;
	for(x=2;x>0;x--)
		for(y=us;y>0;y--);
	
}

uint8_t DS18b20_init(DEV_GPIO_PTR ds18b20_port,uint8_t ds18b20_bit)
{
	uint32_t read_data;
	uint32_t CONT = 0;
	uint8_t FLAG = 1;	
	uint32_t DS18B20_MASK = 0x01 << ds18b20_bit;
	uint32_t DS18B20_WRITE_1 = 0x01 << ds18b20_bit;
	
	ds18b20_port->gpio_open(GPIO_DIR_OUTPUT);
	// set io output
	ds18b20_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(DS18B20_MASK));
	ds18b20_port->gpio_write(DS18B20_WRITE_1,DS18B20_MASK);
	delay_us(24);
	ds18b20_port->gpio_write(0x00,DS18B20_MASK);
	delay_us(854);
	ds18b20_port->gpio_write(DS18B20_WRITE_1,DS18B20_MASK);	
	// set io input
	ds18b20_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(DS18B20_MASK));	
	do{	
		CONT++;
		delay_us(12);
		if(CONT > 8000)FLAG = 0;
		ds18b20_port->gpio_read((&read_data),DS18B20_MASK);
		read_data = (read_data & DS18B20_MASK);				
	}while((read_data != 0) &&(FLAG == 1));	
	delay_us(547);
	// set io output	
	ds18b20_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(DS18B20_MASK));
	ds18b20_port->gpio_write(DS18B20_WRITE_1,DS18B20_MASK);	
	
	return FLAG;
}

uint8_t DS18b20_ReadOneChar(DEV_GPIO_PTR ds18b20_port,uint8_t ds18b20_bit)
{
	uint8_t i=0;
	uint32_t read_bit;
	uint8_t data=0x00;	
	uint32_t DS18B20_MASK = 0x01 << ds18b20_bit;
	uint32_t DS18B20_WRITE_1 = 0x01 << ds18b20_bit;

	ds18b20_port->gpio_open(GPIO_DIR_OUTPUT);
	// set io output
	ds18b20_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(DS18B20_MASK));
	ds18b20_port->gpio_write(DS18B20_WRITE_1,DS18B20_MASK);
	delay_us(20);

	for(i=8;i>0;i--)
	{
		data >>= 1;
		// set io output
		ds18b20_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(DS18B20_MASK));
		ds18b20_port->gpio_write(0x0000,DS18B20_MASK);

		for(uint32_t volatile y=39;y>0;y--); //delay
		
		ds18b20_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(DS18B20_MASK));		
		ds18b20_port->gpio_read((&read_bit),DS18B20_MASK);
		read_bit = read_bit & DS18B20_MASK;
		if(read_bit) data |=0x80;
		else data &= 0x7f;
		delay_us(52);		
	}		
	return data;
}

void DS18b20_WriteOneChar(DEV_GPIO_PTR ds18b20_port,uint8_t ds18b20_bit, uint8_t data)
{
	
	uint32_t DS18B20_MASK = 0x01 << ds18b20_bit;
	uint32_t DS18B20_WRITE_1 = 0x01 << ds18b20_bit;
	ds18b20_port->gpio_open(GPIO_DIR_OUTPUT);
	//set io output
	ds18b20_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(DS18B20_MASK));	
	for(uint8_t i=8;i>0;i--)
	{

		ds18b20_port->gpio_write(0x0000,DS18B20_MASK);
		delay_us(12);
		if(data & 0x01)
			ds18b20_port->gpio_write(DS18B20_WRITE_1,DS18B20_MASK);
		else
			ds18b20_port->gpio_write(0x0000,DS18B20_MASK);
		delay_us(70);
		ds18b20_port->gpio_write(DS18B20_WRITE_1,DS18B20_MASK);
		delay_us(3);
		data >>= 1;
	}
}

void DS18b20_ReadTemperature(DEV_GPIO_PTR ds18b20_port, uint8_t ds18b20_bit, DS18B20_DEF_PTR obj)
{
	uint8_t tempL=0,tempH=0;
	uint16_t temp_data;
	DS18b20_init(ds18b20_port,ds18b20_bit);	
	DS18b20_WriteOneChar(ds18b20_port,ds18b20_bit,0xcc);//跳过读序列号的操作
	DS18b20_WriteOneChar(ds18b20_port,ds18b20_bit,0x44);//启动温度转换
	delay_us(200000);//转换需要一点时间，延时 

	DS18b20_init(ds18b20_port,ds18b20_bit);
	DS18b20_WriteOneChar(ds18b20_port,ds18b20_bit,0xcc);//跳过读序列号的操作
	DS18b20_WriteOneChar(ds18b20_port,ds18b20_bit,0xbe);//读温度寄存器（头两个值分别为温度的低位和高位） 
	tempL = DS18b20_ReadOneChar(ds18b20_port,ds18b20_bit);//读出温度的低位LSB
	tempH = DS18b20_ReadOneChar(ds18b20_port,ds18b20_bit);//读出温度的高位MSB	
	printf("tempH = %2x, tempL = %2x\r\n",tempH,tempL);
	
	temp_data = tempH *256 + tempL;

	if(temp_data & 0xf800) 
	{
		obj->temperature_sign = 1; //温度是负值
		temp_data = (~temp_data) + 1; //转化成正值
	}
	else
	{
		obj->temperature_sign = 0; //温度是正值
	}

	obj->temperature_int = (temp_data & 0x7f0)/16;
	obj->temperature_decimal = temp_data & 0x0f;	
}

