/*
 * task_config.h
 *
 *  Created on: 2018��5��3��
 *      Author: Administrator
 */

#ifndef TASK_CONFIG_H_
#define TASK_CONFIG_H_


#include "../app/usart3task/usart3task.h"//����printf
#include "app_config.h"

#include "FreeRTOS.h"					//FreeRTOSʹ��
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "usart1task.h"//����printf

extern uint16_t touch_flag;//���ذ�ģʽ

extern  SemaphoreHandle_t SendenableMutexSemaphore;	//�����ź���

extern QueueHandle_t pack2server_queue;
extern QueueHandle_t connect_to_server_Queue;

//�������ļ��е��õ�������
extern TaskHandle_t Task1Task_Handler;
extern TaskHandle_t Keyprocess_Handler;

extern TaskHandle_t USART3_app_Handler;
extern TaskHandle_t pack2server_Handler;
extern TaskHandle_t smart_config_Handler;
extern TaskHandle_t connect_to_server_Handler;

extern TimerHandle_t	OneShot_config60_Handle;			//���ζ�ʱ��60s���
extern TimerHandle_t	OneShot_config10_Handle;			//���ζ�ʱ��10s���
extern TimerHandle_t	period40or5_0002_Handle;				//��ʱ����0002��
extern TimerHandle_t	OneShot_config1MS_Handle;
extern TimerHandle_t OneShot_CanServer_Handle;				//���󷵻ص��²��ٸ�����������
extern TimerHandle_t period_Bright_Handle;						//���ڵƹ�

void check_msg_queue(void);





#endif /* TASK_CONFIG_H_ */
