/*
 * package.h
 *
 *  Created on: 2018年5月3日
 *      Author: Administrator
 */

#ifndef PACKAGE_PACKAGE_H_
#define PACKAGE_PACKAGE_H_

#ifndef __PACKAGE_H
#define __PACKAGE_H
#include "embARC.h"
#include "embARC_debug.h"

#include "app_config.h"

#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

extern uint16_t pkt_serial;	//发送包，数量计数
extern char say_did[30];

void usart3_app_task(void *pvParameters);
void sendpack2server(uint16_t package_num);
void pack2server(void *pvParameters);
void wifi2server(uint16_t data_type);
uint8_t serverdata_process(usart_task_data usart3_app_temp);
void period40or5_0002_Callback(TimerHandle_t xTimer);
void disconnect_process(void);
void say2server(char*toid,char*msg);
#endif



#endif /* PACKAGE_PACKAGE_H_ */
