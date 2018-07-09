/*
 * usart3task.h
 *
 *  Created on: 2018年5月3日
 *      Author: Administrator
 */

#ifndef USART3TASK_USART3TASK_H_
#define USART3TASK_USART3TASK_H_

#include "stdio.h"
#include "task_config.h"
#include "app_config.h"

#if 1
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif

extern  TaskHandle_t FREE_ERRORMEM_Handler;
extern  TaskHandle_t USART3_Handler;
extern  TaskHandle_t USART3_send_Handler;


extern uint8_t usart3_data_all[USART3_NUM][USART3_PER_LEN];//串口1缓冲buff
extern uint16_t USART3_RX_STA;         		//接收状态标记
extern uint8_t usart3_buff_flag;//标记哪些数组被使用了，下次往哪个数组中存
extern uint8_t usart3_time_out;//串口1接收超时计时
extern uint8_t usart3_task_time_out;//串口1 task接收超时计时

extern QueueHandle_t USART3_Queue;
//extern QueueHandle_t USART3_Task_Queue;
extern QueueHandle_t USART3_Task_server_Queue;//存储服务器数据
extern QueueHandle_t USART3_Task_rn_other_Queue;//存储\r\n数据和没有格式的数据
extern QueueHandle_t USART3_Send_Queue;//串口3发送队列
extern SemaphoreHandle_t BinarySemaphore_free;	//二值信号量句柄

extern usart_dat usart3_data;
extern USART_TASK_PROCESS usart3_task_process;//数据处理流程标记
extern usart_task_data usart3_task_temp;//发送到usart3task队列中
extern DEV_UART *uart3;
//如果想串口中断接收，请不要注释以下宏定义
uint32_t uart3_init(uint32_t bound);
void release_buff3(uint8_t buff_num);
void occupy_buff3(void);
void USART3_task(void *pvParameters);
void USART3_IRQHandler(void);
//USART1_task中会存在多包组帧数据出现错误的情况，需要进行free 错误内存的操作
void FREE_ERRORMEM_task(void *pvParameters);

void u3_senddata(uint8_t *data,uint16_t length);
void USART3_send_task(void *pvParameters);
void CanServer_Callback(TimerHandle_t xTimer);


#endif /* USART3TASK_USART3TASK_H_ */
