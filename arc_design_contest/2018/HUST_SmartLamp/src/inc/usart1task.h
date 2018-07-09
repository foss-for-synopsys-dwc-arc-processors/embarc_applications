/*
 * usart1task.h
 *
 *  Created on: 2018��5��4��
 *      Author: Administrator
 */

#ifndef USART1TASK_USART1TASK_H_
#define USART1TASK_USART1TASK_H_

#include "stdio.h"


#include "app_config.h"

#if 1
#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif

#define USART1_NUM_0 0x01
#define USART1_NUM_1 0x02
#define USART1_NUM_2 0x04
#define USART1_NUM_3 0x08
#define USART1_NUM_4 0x10
#define USART1_NUM_5 0x20

extern TaskHandle_t USART1_Handler;
extern TaskHandle_t USART1_app_Handler;

extern QueueHandle_t USART1_Queue;
extern QueueHandle_t USART1_Task_Queue;
extern SemaphoreHandle_t BinarySemaphore_free;	//��ֵ�ź������
extern SemaphoreHandle_t  xMutex;//printf �̰߳�ȫ����

//extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern uint8_t usart_data_all[USART1_NUM][USART1_PER_LEN];//����1����buff
extern uint16_t USART_RX_STA;         		//����״̬���
extern uint8_t usart_buff_flag;//�����Щ���鱻ʹ���ˣ��´����ĸ������д�
extern uint8_t usart1_time_out;//����1���ճ�ʱ��ʱ
extern  uint8_t usart1_task_time_out;//����1 task���ճ�ʱ��ʱ

extern usart_dat usart1_data;
extern USART_TASK_PROCESS usart1_task_process;//���ݴ������̱��
extern usart_task_data usart1_task_temp;//���͵�usart1task������

uint32_t uart1_init(uint32_t bound);
void release_buff(uint8_t buff_num);
void occupy_buff(void);
void USART1_task(void *pvParameters);
void usart1_app_task(void *pvParameters);
void USART1_IRQHandler(void);


//����dprintf �궨��
void printf_nosense(char *format, ...);

#if ASSERT_PRINTF_ENABLE
#define dprintf printf
#else
#define dprintf printf_nosense
#endif

#endif /* USART1TASK_USART1TASK_H_ */
