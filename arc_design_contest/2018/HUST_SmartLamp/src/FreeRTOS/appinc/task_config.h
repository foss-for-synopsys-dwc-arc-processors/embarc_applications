/*
 * task_config.h
 *
 *  Created on: 2018年5月3日
 *      Author: Administrator
 */

#ifndef TASK_CONFIG_H_
#define TASK_CONFIG_H_


#include "../app/usart3task/usart3task.h"//调试printf
#include "app_config.h"

#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include "usart1task.h"//调试printf

extern uint16_t touch_flag;//触控板模式

extern  SemaphoreHandle_t SendenableMutexSemaphore;	//互斥信号量

extern QueueHandle_t pack2server_queue;
extern QueueHandle_t connect_to_server_Queue;

//在其他文件中调用的任务句柄
extern TaskHandle_t Task1Task_Handler;
extern TaskHandle_t Keyprocess_Handler;

extern TaskHandle_t USART3_app_Handler;
extern TaskHandle_t pack2server_Handler;
extern TaskHandle_t smart_config_Handler;
extern TaskHandle_t connect_to_server_Handler;

extern TimerHandle_t	OneShot_config60_Handle;			//单次定时器60s句柄
extern TimerHandle_t	OneShot_config10_Handle;			//单次定时器10s句柄
extern TimerHandle_t	period40or5_0002_Handle;				//定时发送0002包
extern TimerHandle_t	OneShot_config1MS_Handle;
extern TimerHandle_t OneShot_CanServer_Handle;				//错误返回导致不再给服务器发包
extern TimerHandle_t period_Bright_Handle;						//调节灯光

void check_msg_queue(void);





#endif /* TASK_CONFIG_H_ */
