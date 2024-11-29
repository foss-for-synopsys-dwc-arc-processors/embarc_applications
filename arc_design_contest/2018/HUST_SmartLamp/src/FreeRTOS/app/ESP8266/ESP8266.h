/*
 * ESP8266.h
 *
 *  Created on: 2018年5月3日
 *      Author: Administrator
 */

#ifndef ESP8266_ESP8266_H_
#define ESP8266_ESP8266_H_

#ifndef __ESP8266_H__
#define __ESP8266_H__


#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"



extern char IP_buf[16];
extern char portnum[11];

void smartconfig_task(void *pvParameters);
void connect_to_server(void *pvParameters);
uint8_t connect_to_server_process(void );
void disconnect_TCP(void);
void OneShot60_10Callback(TimerHandle_t xTimer);		//单次定时器回调函数
uint8_t send_cmd_wait_ack(char *cmd,char *ack,uint16_t waittime,uint8_t rn);
uint8_t check_cmd(char*str);
void send_cmd(char*cmd,uint8_t rn);
uint8_t quit_trans(void);
void free_usart3_rn_other_queue(void);
void free_usart3_server_queue(void);
void stop_config_task(uint8_t ifsuccess);
void start_config_task(void);
uint8_t check_consta(void);

#if 0
void atk_8266_init(void);


void atk_8266_at_response(uint8_t mode);
uint8_t* atk_8266_check_cmd(uint8_t *str);
uint8_t* atk_8266_check_cmd2(uint8_t *str);
uint8_t atk_8266_apsta_check(void);
uint8_t atk_8266_send_data(uint8_t *data,uint8_t *ack,u16 waittime);
uint8_t atk_8266_send_cmd(uint8_t *cmd,uint8_t *ack,u16 waittime);
uint8_t atk_8266_quit_trans(void);
uint8_t atk_8266_consta_check(void);
void atk_8266_get_wanip(uint8_t* ipbuf);
void atk_8266_test(void);
uint8_t ATK8266_SMART(void);
uint8_t atk_8266_In_trans(void);
uint8_t atk_8266_disconnect(void);
uint8_t atk_8266_connectTCP(void);
void atk_8266_In_trans1(void);
uint8_t atk_8266_In_trans2(void);//任务调度模式实现

#endif

#endif



#endif /* ESP8266_ESP8266_H_ */
