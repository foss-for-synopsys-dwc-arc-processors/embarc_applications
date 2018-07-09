/*
 * usart4task.h
 *
 *  Created on: 2018��5��16��
 *      Author: Administrator
 */

#ifndef USART4TASK_USART4TASK_H_
#define USART4TASK_USART4TASK_H_

#include "stdio.h"


#include "task_config.h"
#include "app_config.h"
//���ʹ��ucos,����������ͷ�ļ�����.
#if 1
#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


#define USART4_FIFO_LEN 50//����4��FIFO��С
//#define USART4_SEND_LEN 50//����fifo��С

extern suart_struct uart4_ctr;//ģ�⴮��4�Ŀ��Ʋ���

extern QueueHandle_t USART4_Queue;
extern QueueHandle_t USART4_Send_Queue;

extern TaskHandle_t USART4_app_Handler;//
extern TimerHandle_t	period_waittouch_Handle;

extern void usart4_init(void);
void USART4_app_task(void *pvParameters);
uint8_t senddata_by_usart4(uint8_t*str,uint8_t len);//�ⲿ���÷��ͺ���
void suart_var_init(suart_struct* re_ctr);
void period_waittouch_Callback(TimerHandle_t xTimer);
void EXTI1_IRQHandler(void *ptr);
void mcu_sendcmd2touch(uint8_t cmd);
void server_control_light(uint8_t mode);
//APP���ݴ�����
void touch_process(uint8_t touch_cmd);
void touch_process_poweroff(void);//poweroff�Ĵ���
void touch_process_poweron(void);//poweron�Ĵ��� �����Զ�����
void touch_process_lightswitch(uint8_t light,uint8_t wifi);//����ֱ�ӵ���ĳ����λ






#endif /* USART4TASK_USART4TASK_H_ */
