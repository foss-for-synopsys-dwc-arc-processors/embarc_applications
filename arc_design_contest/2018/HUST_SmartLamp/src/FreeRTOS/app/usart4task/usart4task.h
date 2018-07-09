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

//#define USART_REC_LEN  			200  	//�����������ֽ��� 200
//#define EN_USART4_RX 0
#define USART4_FIFO_LEN 50//����4�Ľ���FIFO��С
#define USART4_SEND_LEN 50//����fifo��С

extern suart_struct uart4_ctr;//ģ�⴮��4�Ŀ��Ʋ���
extern fifo_struct  usart4_send_fifo;
extern suart_send_struct send_ctr4;
extern fifo_struct  usart4_fifo;//����4�����ݴ洢FIFO ����fifo
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

#define EMSK_GPIOUART4_PORT		DEV_GPIO_PORT_A
#define GPIO_UART4_MASK		0x00030000		//portA��16-17Pin����J3��7-8��
#define GPIO_UART4_LOW	  (0)
#define GPIO_UART4IN_MASK	0X00010000		//portA��16Pin����J3��7��
#define GPIO_UART4OUT_MASK	0X00020000		//portA��17Pin����J3��8��
extern DEV_GPIO * uart4;



#endif /* USART4TASK_USART4TASK_H_ */
