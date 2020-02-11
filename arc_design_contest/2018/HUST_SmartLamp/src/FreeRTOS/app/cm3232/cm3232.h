/*
 * cm3232.h
 *
 *  Created on: 2018Äê5ÔÂ23ÈÕ
 *      Author: Administrator
 */


#ifndef CM3232_CM3232_H_
#define CM3232_CM3232_H_
#include "stdint.h"

#define CM3232_TIME 2  //0 1 2

extern	uint16_t cm3232_time[2][3];
void CM3232_Init(void);
uint16_t CM3232_Read(uint8_t Comand);
void CM3232_Write(uint8_t Command,uint8_t Data);
uint16_t CM3232_Bright(uint8_t data);
//void Auto_Bright(uint16_t data);


#endif /* CM3232_CM3232_H_ */
