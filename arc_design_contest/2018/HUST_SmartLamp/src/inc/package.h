/*
 * package.h
 *
 *  Created on: 2018��5��3��
 *      Author: Administrator
 */

#ifndef PACKAGE_PACKAGE_H_
#define PACKAGE_PACKAGE_H_

#ifndef __PACKAGE_H
#define __PACKAGE_H
#include "embARC.h"
#include "embARC_debug.h"

#include "app_config.h"

#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

extern uint16_t pkt_serial;	//���Ͱ�����������

void usart3_app_task(void *pvParameters);
void sendpack2server(uint16_t package_num);
void pack2server(void *pvParameters);
void wifi2server(uint16_t data_type);
void serverdata_process(usart_task_data usart3_app_temp);
void period40or5_0002_Callback(TimerHandle_t xTimer);
#endif



#endif /* PACKAGE_PACKAGE_H_ */
