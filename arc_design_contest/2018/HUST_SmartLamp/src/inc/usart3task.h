/*
 * usart3task.h
 *
 *  Created on: 2018��5��3��
 *      Author: Administrator
 */

#ifndef USART3TASK_USART3TASK_H_
#define USART3TASK_USART3TASK_H_

#include "stdio.h"
#include "task_config.h"
#include "app_config.h"

#if 1
#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif

extern  TaskHandle_t FREE_ERRORMEM_Handler;
extern  TaskHandle_t USART3_Handler;
extern  TaskHandle_t USART3_send_Handler;


extern uint8_t usart3_data_all[USART3_NUM][USART3_PER_LEN];//����1����buff
extern uint16_t USART3_RX_STA;         		//����״̬���
extern uint8_t usart3_buff_flag;//�����Щ���鱻ʹ���ˣ��´����ĸ������д�
extern uint8_t usart3_time_out;//����1���ճ�ʱ��ʱ
extern uint8_t usart3_task_time_out;//����1 task���ճ�ʱ��ʱ

extern QueueHandle_t USART3_Queue;
//extern QueueHandle_t USART3_Task_Queue;
extern QueueHandle_t USART3_Task_server_Queue;//�洢����������
extern QueueHandle_t USART3_Task_rn_other_Queue;//�洢\r\n���ݺ�û�и�ʽ������
extern QueueHandle_t USART3_Send_Queue;//����3���Ͷ���
extern SemaphoreHandle_t BinarySemaphore_free;	//��ֵ�ź������

extern usart_dat usart3_data;
extern USART_TASK_PROCESS usart3_task_process;//���ݴ������̱��
extern usart_task_data usart3_task_temp;//���͵�usart3task������
extern DEV_UART *uart3;
//����봮���жϽ��գ��벻Ҫע�����º궨��
uint32_t uart3_init(uint32_t bound);
void release_buff3(uint8_t buff_num);
void occupy_buff3(void);
void USART3_task(void *pvParameters);
void USART3_IRQHandler(void);
//USART1_task�л���ڶ����֡���ݳ��ִ�����������Ҫ����free �����ڴ�Ĳ���
void FREE_ERRORMEM_task(void *pvParameters);

void u3_senddata(uint8_t *data,uint16_t length);
void USART3_send_task(void *pvParameters);
void CanServer_Callback(TimerHandle_t xTimer);


#endif /* USART3TASK_USART3TASK_H_ */
