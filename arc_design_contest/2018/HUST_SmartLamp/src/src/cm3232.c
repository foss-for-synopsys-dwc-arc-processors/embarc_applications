#include "cm3232.h"
#include "i2c.h"

#include "timer.h"
#include "stdlib.h"

const uint8_t Slave_Address=0x10;
const uint8_t Slave_Address_Write=0x20;
const uint8_t Slave_Address_Read=0x21;

uint16_t cm3232_time[2][3] = {0x00,0x04,0x08,100,200,400};

void CM3232_Init(void)
{
	I2CC_Init();
//	CM3232_Write(0x00,0x08);//0x08-400ms  0x00-100ms 0x04-200ms
	CM3232_Write(0x00,cm3232_time[0][CM3232_TIME]);
}

uint16_t CM3232_Read(uint8_t Command)
{
	uint8_t templ=0;
	uint8_t temph=0;
	uint16_t temp=0;
	I2C_Start();
	I2C_Send_Byte(Slave_Address_Write);
	I2C_Wait_Ack();
	I2C_Send_Byte(Command);
	I2C_Wait_Ack();
	I2C_Start();
	I2C_Send_Byte(Slave_Address_Read);
	I2C_Wait_Ack();
	templ=I2C_Read_Byte(0);
	temph=I2C_Read_Byte(1);
	I2C_Stop();
	temp=temph;
	temp=(temp<<8)+templ;
	return temp;
}

void CM3232_Write(uint8_t Command,uint8_t Data)
{
	I2C_Start();
	I2C_Send_Byte(Slave_Address_Write);
	I2C_Wait_Ack();
	I2C_Send_Byte(Command);
	I2C_Wait_Ack();
	I2C_Send_Byte(Data);
	I2C_Wait_Ack();
	I2C_Stop();
}

uint16_t CM3232_Bright(uint8_t data)
{
	uint8_t t;
	uint16_t temp=0;
	for(t=0;t<data;t++)
	{
		temp+=CM3232_Read(0x50)/data;
		delay_us(25000);
	}
	return temp;
}


