/*
 * i2c.h
 *
 *  Created on: 2018年5月23日
 *      Author: Administrator
 */

#include "task_config.h"
#include "app_config.h"

#ifndef I2C_I2C_H_
#define I2C_I2C_H_

//IO读写设置


//IO操作函数
//#define I2C_SCL    PCout(7) //SCL
//#define I2C_SDA    PCout(6) //SDA
//#define READ_SDA   PCin(6)  //??SDA

//I2C操作函数
void I2CC_Init(void);                //???I2C?IO?
void I2C_Start(void);								//??I2C????
void I2C_Stop(void);	  						//??I2C????
void I2C_Send_Byte(uint8_t txd);					//I2C??????
uint8_t I2C_Read_Byte(unsigned char ack);//I2C??????
uint8_t I2C_Wait_Ack(void); 							//I2C??ACK??
void I2C_Ack(void);									//I2C??ACK??
void I2C_NAck(void);								//I2C???ACK??


#endif /* I2C_I2C_H_ */
