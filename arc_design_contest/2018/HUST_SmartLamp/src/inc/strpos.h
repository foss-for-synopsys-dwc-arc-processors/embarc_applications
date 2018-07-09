/*
 * strpos.h
 *
 *  Created on: 2018年5月3日
 *      Author: Administrator
 */

#ifndef STRPOS_STRPOS_H_
#define STRPOS_STRPOS_H_

#include "embARC.h"
#include "embARC_debug.h"
extern uint8_t server_start[4] ;
extern uint8_t server_stop[4] ;
extern uint8_t rn_stop[2];

uint8_t findpos(uint8_t*pstr1,uint8_t len1,uint8_t *pstr2,uint8_t len2);//1是标志位，2是原始数据数组
uint8_t strsame(uint8_t*str1,uint8_t*str2,uint8_t len);

#endif /* STRPOS_STRPOS_H_ */
