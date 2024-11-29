/*
 * usart4task.h
 *
 *  Created on: 2018年5月16日
 *      Author: Administrator
 */

#ifndef USART4TASK_USART4TASK_H_
#define USART4TASK_USART4TASK_H_

#include "stdio.h"


#include "task_config.h"
#include "app_config.h"
//如果使用ucos,则包括下面的头文件即可.
#if 1
#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//#define USART_REC_LEN  			200  	//定义最大接收字节数 200
//#define EN_USART4_RX 0
#define USART4_FIFO_LEN 50//串口4的接收FIFO大小
#define USART4_SEND_LEN 50//发送fifo大小

extern suart_struct uart4_ctr;//模拟串口4的控制参数
extern fifo_struct  usart4_send_fifo;
extern suart_send_struct send_ctr4;
extern fifo_struct  usart4_fifo;//串口4的数据存储FIFO 接收fifo
extern QueueHandle_t USART4_Queue;
extern QueueHandle_t USART4_Send_Queue;

extern TaskHandle_t USART4_app_Handler;//
extern TimerHandle_t	period_waittouch_Handle;

extern void usart4_init(void);
void USART4_app_task(void *pvParameters);
uint8_t senddata_by_usart4(uint8_t*str,uint8_t len);//外部调用发送函数
void suart_var_init(suart_struct* re_ctr);
void period_waittouch_Callback(TimerHandle_t xTimer);
void EXTI1_IRQHandler(void *ptr);
void mcu_sendcmd2touch(uint8_t cmd);
void server_control_light(uint8_t mode);
//APP数据处理函数
void touch_process(uint8_t touch_cmd);
void touch_process_poweroff(void);//poweroff的处理
void touch_process_poweron(void);//poweron的处理 进入自动调光
void touch_process_lightswitch(uint8_t light,uint8_t wifi);//控制直接调到某个档位

#define EMSK_GPIOUART4_PORT		DEV_GPIO_PORT_A
#define GPIO_UART4_MASK		0x00030000		//portA的16-17Pin，接J3的7-8口
#define GPIO_UART4_LOW	  (0)
#define GPIO_UART4IN_MASK	0X00010000		//portA的16Pin，接J3的7口
#define GPIO_UART4OUT_MASK	0X00020000		//portA的17Pin，接J3的8口
extern DEV_GPIO * uart4;



#endif /* USART4TASK_USART4TASK_H_ */
