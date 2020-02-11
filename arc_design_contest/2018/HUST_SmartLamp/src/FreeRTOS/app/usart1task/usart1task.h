/*
 * usart1task.h
 *
 *  Created on: 2018年5月4日
 *      Author: Administrator
 */

#ifndef USART1TASK_USART1TASK_H_
#define USART1TASK_USART1TASK_H_

#include "stdio.h"


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
extern SemaphoreHandle_t BinarySemaphore_free;	//二值信号量句柄
extern QueueHandle_t Dprintf_Queue;
extern SemaphoreHandle_t  xMutex;//printf 线程安全控制

//extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern uint8_t usart_data_all[USART1_NUM][USART1_PER_LEN];//串口1缓冲buff
extern uint16_t USART_RX_STA;         		//接收状态标记
extern uint8_t usart_buff_flag;//标记哪些数组被使用了，下次往哪个数组中存
extern uint8_t usart1_time_out;//串口1接收超时计时
extern  uint8_t usart1_task_time_out;//串口1 task接收超时计时



extern usart_dat usart1_data;
extern USART_TASK_PROCESS usart1_task_process;//数据处理流程标记
extern usart_task_data usart1_task_temp;//发送到usart1task队列中
uint32_t uart1_init(uint32_t bound);
void release_buff(uint8_t buff_num);
void occupy_buff(void);
void USART1_task(void *pvParameters);
void usart1_app_task(void *pvParameters);
void USART1_IRQHandler(void);
void printf_task(char *format, ...);
//USART1_task中会存在多包组帧数据出现错误的情况，需要进行free 错误内存的操作
//void FREE_ERRORMEM_task(void *pvParameters);

//调试dprintf 宏定义
void printf_nosense(char *format, ...);

#if ASSERT_PRINTF_ENABLE
#define dprintf printf//printf  printf_task
#else
#define dprintf printf_nosense
#endif



/*


                                                    MMMMMMM
                                                    .MMMMMMMMH
                                                      .MMMMMMMMMMM
                                                        MMMMMMMMMMMM'
                                                         'MMMMMMMMMMMMM
                                                           MMMMMMMMMMMMMMM
                                                            MMMMMMMMMMMMMMMM.
                                                             .MMMMMMMMMMMMMMMMH          H
                                                              MMMMMMMMMMMMMMMMMM          M
                                                               :MMMMMMMMMMMMMMMMMM
                                                               :MMMMMMMMMMMMMMMMMMMM        M
                                                                .MMMMMMMMMMMMMMMMMMMMM       M
                                                                 MMMMMMMMMMMMMMMMMMMMMM:      M
                                                                 MMMMMMMMMMMMMMMMMMMMMMMM      M
                                                                  HMMMMMMMMMMMMMMMMMMMMMMM     HM
                                                                  MMMMMMMMMMMMMMMMMMMMMMMMM     .I
                                                                   MMMMMMMMMMMMMMMMMMMMMMMMM     M
                                                                   'MMMMMMMMMMMMMMMMMMMMMMMMM'    M
                                                                    MMMMMMMMMMMMMMMMMMMMMMMMMM.    M
                                                                    MMMMMMMMMMMMMMMMMMMMMMMMMMMH   MM
                                                                    :MMMMMMMMMMMMMMMMMMMMMMMMMMMM   M
                                                                     MMMMMMMMMMMMMMMMMMMMMMMMMMMM.   M
                                                                     MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM   M
                ..MMMMMMMMMMMMMMMMMMMMMMMMMMM:M.                     :MMMMMMMMMMMMMMMMMMMMMMMMMMMMM:  H
      MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMH                MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM   M
   MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM            MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM   M
    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM         MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM  M
       MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'      MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM. '.
          MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM    'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM' M
            HMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM.''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM.M
               MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'M
                IMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:M
                  MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                    .MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'
                      MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'
                        MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                         MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                           .MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                                            H'                    :
                            HMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                                       MH                    :MM
                             :MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                                   MM.                  MMMMM
                               MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                                M:                MMMMMMMM.
                                MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                           :M             'MMMMMMMMMMMM
                                 MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                        MM           MMMMMMMMMMMMMMM
                                  .MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                     MM       MMMMMMMMMMMMMMMMMMM
                                   .MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM.                 MM    MMMMMMMMMMMMMMMMMMMMMM
                  MMMMMM             HMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM               MM  MMMMMMMMMMMMMMMMMMMMMMMM
                        MMMM.         MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM            MMMMMMMMMMMMMMMMMMMMMMMMMMMM
                            MMM'       MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'        MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                              MMMM      MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                MMMM    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:
                                  MMMM   MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                    MMM  HMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMH
               MMMMMMMMM:            .MM  MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                 MMMMMMMMMMMMMMMMMMH   MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                     MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'MMMMMMMMMMMMMMMMMM.H'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                        MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM MMMMMMMM'MMMMMMMMMMMMMMMMMMO''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'
                           MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM MMMMMMMM''MMMMMMMMMMMMMMMMM.''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'
                             MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM: MMMMMM.'' MMMMMMMMMMMMMMMMM'''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM.
                               MMMMMMMMMMMMMMMMMMMMMMMMMMMMM' MMMMMM''':MMMMMMMMMMMMMM'M'''MMMMMMMMMMMMMMMM'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                 .MMMMMMMMMMMMMMMMMMMMMMMMMMMM MMMMM''''MMMMMMMMMMMMMM'''''MMMMMMMMMMMMMMM'''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                   IMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'''''MMMMMMMMMMMMM.'''''MMMMMMMMMMMMM''''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM.
                                     MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'''''MMMMMMMMMMMMM''''''MMMMMMMMMMMMM''M'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                       MMMMMMMMMMMMMMMMMMMMMMMMMM'MMM'''''MMMMMMMMMMMM''M''''MMMMMMMMMMM'''M'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM           .MMMMMMMMMMMMMMMM'
                                        MMMMMMMMMMMMMMMMMM'MMMMMMM'MMM'''''MMMMMMMMMMMH'M''''MMMMMMMMMMM'''M'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'    MMMMMM
                                         'MMMMMMMMMMM'''MM'''MMMMM''.M.'''''MMMMMMMMMMM'M'''''MMMMMMMMM'''':'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMH''
                                           .MMMMMMMMM'MM''M''''MMMM ''M''''''MMMMMMMMMM'M'''''MMMMMMMMM'''''M.MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'.:
                                             MMMMMMMM.'''MM''''''MMMM''M'''''''MMMMMMMM'' '''''MMMMMMMMM''''M'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM'
                                              MMMMMMM.''''MM'''M'''MM''''''''''''MMMMMMM'M'''MMMMMMMMMM:''''M'MMMMMMMMMMMM'''''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:M'
                                               MMMMMM  '.M'M'''M''''MM''''''''''''MMMMMM'M'MMMMMMMMMM''''''''M'MMMMMMMMMM''''''''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                                MMMMM'M'M''M'''M'''''MMM''''''''''''MMMM'MMMMMMMMMMM'''''''''M'M MM''MMM'''MM'::''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                      .MMM'      MMMM:''M''''''M'''''MMMM''''''''''''MMMMMMMMMMMMMMMM''''''''''M'MM''MM'''M''''''''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                    'MMMMMMMMMMMMMMMMM'''M.MM''M'''''MMMMM''''''''''''MMMMMMMMMMMM''M''''''''''''MM''''''MM'''''M''MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                          MMMMMMMMMMMM''HMM'M'''M''''MMMMMMM'''''''.MMMMMMMMMMMMM'''M.'''''''''''MM'''''MM''M''''.'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                            'MMMMMMMMMM''MMMM''''M''''MMMMMMM''''MMMMMMM'''''M.''''''M'''''''''''MM''''M'M'''M'''M'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                               MMMMMMMM 'MMMM'''' M''''MM'MM .'''MMMMH'''''''''''''''''''''''''''M'''''M MM:'''''M'MMMMMMMMMMMMMMMMMMMMMMMMMMM:'
                                                 MMMMMMM''H'M''''''M.''''''':''''M:M'''''''''''''''''''''''''''''''''''MM'''''M''M'MMMMMMMMMMMMMMMMMMMMMMMM
                                                    MMMMM''MM'''''''''''''''''''''''M''''''''''''''''''''''''''''''''''MMHM'''M':'MMMMMMMMMMMMMMMMMMMMMH
                                                      MMMMM'M'''''''''''''''''''''''HMMM'''''''''''''''''''''''''''''''M''M'''M'''MMMMMMMMMMMMMMMMM:
                                                        MMMMM''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''.''M'''MMMMMMMMMMMMMM
                                                           M'''''''''':MM''''''''''''''''''''''''''''''''''''''''''''''''''M''M''MMMMMMMMMMMM
                                                           M'''''''''''M'''''''''''''''''''''''''''''''''''''''''''''''''''''M''MMMMMMMMMM
                                                          M''''''''''''''''''''''''''''''''''''''''''''''''''''''''MMM'''M'''''MMMMMMMM
                                                           M'''''''''''''''''''''''''''''''''''''''''''''''''''''''HM'.M''''''MMMMMMMMMMMMMMMMMMMMMMMM
                                                            M''''''''''''''''''''''''''''''''''''''''''''''''''H'''MM:''''''MMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                                             HM''''''''''''''''''''''''''''''''''''''''''''''OMM:'''''''''MMMMMMMMMMMMMMMMMMM'
                                                               M:'''''''''HMMMM''''''''''''''''''''''''''''''MMMM.'''''MMMMMMMMMMMMM M
                                                                .M''''''MMO::::M''''''''''''''''''''''''''''MMMMMMMMMMMMMMMMMM.
                                                                  MM''''MMMHIHMM''''''''''''''''''''''''''MMMMMMMMMMMMMMM
                                                                   MM'''''''''''''''''''''''''''''''''''MMMMMMMMMMMMMMMMMM:
                                                                     M''''''''''''''''''''''''''''''MMMMMMMMMMMMMMMMMMMMM
                                                                      M ''''''''''''''''''''''MMMMM'''MMMMM
                                                                       MM''''''''''''''.MMMMM'''''''''MMM
                                                                        MMM''''''MMMMMM''''''''''''''''MM
                                                                        M'MMMMMMM''''''''''''''''''''''MM
                                                                        H'''''''''''''''''''''''''''''''M
                                                                       :''''''''''''''''''''''''''''''''M
                                                                  '    M'''''''''''''''''''''''''''''''':M
                                                                 MM:::MM'''''''''''''''''''''''''''''''''MMM       MMMMH    H'MMMMMM:HM
                                                                MH::MMM''''''''''''''''''''''''''''''''''''''MMMM'MH::::::::::::::::::::M.
                                                         MMMMMHMH::M:MM'''''''''''''''''''''''''''''''''''''''''MM:::::::::::::::IMM':MM::M
                                                     MMM::::::M::HM':M''''''''''''''''''''''''''''''''''''''''.MH::::::::::::::MM'''''''':MM
                                                MMMH:::::::::M::MM'''''''''''''''''''''''''''''''''''''''''''MM::::::::::::::M''''''''''''''':M
                                            HMMM:::::::::::MM::::M'''''''''''''''''''''''''':MHMMMMMMMMMM''.MM:::::::::::::MI'''''''''''''''''''MM
                                           M::::::::::::::MM:::::MMM'''''''''''''''''''HMMM'''''''''''''''MM::::::::::::::M'''''''''''''''''''''''MM
                                            M:::::::::::::M:::::::MMMM:''''''''''''''MM'''''''''''''''''MM:::::::::::::::MM'''''''''''''''''''''''''MM
                                            M::::::::::::::::::::::M'''M'''''''M'''M''''''''''''''''''MM:::::::::::::::::M'''''''''''''''''''''''''''MM
                                           MH:::H::::::::::::::::::M''''''''''..'::'''''''''''''''':MM:::::::::::::::::::M''''''''''''''''''''''''''''M
                                          M:':::M:::::::::::::::::::M''''''MMM:''''''''''''''''''MMH:::::::::::::::::::::M''''''''''''''''''''''''''''M
                                         M'''M::M::::H:::::::::::::::O''''''''''''''''''''''MMMM:::::::::::::::::::::::::M''''''''''''''''''''''''''''M
                                        '.'''M:M:::::M:::::::::::::::H''''''''''''''''''''MM:::::::::::::::::::::::::::::M''''''''''''''''''''''''''''M
                                       'M''''.HM::::::M:::H:::::::::::H'''M'''''''''''MMM::::::::::::::::::::::::::::::::M'''''.''''''''''''''''''''''M
                                       M''''''M::::::::::::M:::::::::::M''M''''''''MMH:::::::::::::::::::::::::::::::::::MM'''''''''''''''''''''''''''M'
                                      M'''''''M::::::::M::::H::::::::::::'H''''''MM::::::::::::::::::::::::::::::::::::::MM'''H:''''''''''''''''''''''MHM
                                      ''''''''M:::::::::M:::::::::::::::MM ''''M:::::::::::::::::::::::::::H:::::::::::::OM'''M'''''''''''''''''''''''M'HM
                                     'M''''''H:::::::::::MM::::M:::::::::M''M::::::::::::::::::::::::::::.::::::::::::::::M''MM'''''''''''''''''''''''''''M
                                      M''''''M:::::::::::MMM:::::::::::::HM::::::::::::::::::::::::::::M::::::::::::::::::MM.MM'''''''''''''''''''''''''''MM
                                      M''''''M::::::::::::MMH::::::::::::::::::::::::::::::::::::::::MH:::::::::::::::::::M:MMM''''''''''''''''''''''''''''H'
                                      H:'''''H:::::::::::::MMM::::::::::::::::::::::::::::::::::::::M:::::::::::::::::::::::::M'''''''''''''''''''''''''''''M
                                      M'''''M:::::::::::::::HMM:::::::::::::::::::::::::::::::::::M::::::::::::::M::::::::::::MO'''''''''''''''''''''''''''''M
                                      M'''''M::::::::::::::::::M:::::::::::::::::::::::::::::::HMM:::::::::::::::::M::::::::::MM''''''''''''''''''''''''''''':M
                                      M'''''M::::::::::::::::::::::::::::::::::::::::::::HMH::::::::::::::::::::M::::M::::::::MM''''''''''''''''''''''''''''''.'
                                     M.''''.::::::::::::::::::::::::::::::::::::::::HM::::::::::::::::::::::::::::M:::MH:::::::M'''''''''''''''''''''''''''''''M
                                     M'''''M::::::::::::::::::::::::::::::::::::MM::::::::::::::::::::::::::::::::::M:::MM:::::M'''''''''''''''''''''''''''''''':
                                    .M'''''M::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::M::::MM:MM''''''''''''''''''''''''''''''''M
                                    M''''''M::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::M:::::HM'''''''''''''''''''''''''''''''''M
                                   'M'''''.H:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MM::IM''''''''''''''''''''''''''''''''M
                                   M''''''M:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::HMMMM''''''''''''''''''''''''''''''''MM
                                   M''''''M:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MMM M''''''''''''''''''''''''''''''''MM
                                   M'''''.H::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::MM   :M''''''''''''''''''''''''''''''''M
                                  M.'''''M::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::HM     M''''''''''''''''''''''''''''''''M
                                  M''''''M::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::M      'M'''''''''''''''''''''''''''''''.'
                                  M''''''M:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::M::      .M'''''''''''''''''''''''''''''''M
                                 M'''''''MMI::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::H::M         M'''''''''''''''''''''''''''''M.
                                 M''''''''''MM:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::M:::M          'M'M''''''''''''''''''''''''''M.
                                 M.'''''''''''MM::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::M::::M            M:M''''''''''''''''''''''''''MM
                                  M'''''''''''''MM:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::M             HM''''''''''''''''''''''''''''MM
                                  M.''''''''''''''M:::::::::::::::::MMMMM::MM::::::::::::::::::::::::::::::::::::::::::::MM               M.''''''''''''''''''''''''''''MM
                                  'M''''''''''''''''MM::::::::::::OMM '  M   MM:HMMMMMMMMMMI:::::::::::::::::::::::::::MM                 MM'''''''''''''''''''''''''''''OM
                                  M'''''''''''''''''MMMMMMMMM'     M MM M   M .M                   '::'H:HMMMMM:::::MM.                   ::''''''''''''''''''''''''''''''M
                                  :''''''''''''''''''MM           M  H  '  MM   M :'...H                        'MMH                      M:'''''''''''''''''''''''''''''''M
                                 M''''''''''''''''''MMM      HMM: M    M  MMMM     MMM.'                          M                       MM'''''''''''''''''''''''''''''''M:
                                'M''''''''''''''''''''MMMMO      MM  M   .M MMH   M                                M                      MM'''''''''''''''''''''''''''''''.M
                                M:'''''''''''''''''''MM:           HH    M:  MM   M'                              M                       .M''''''''''''''''''''''''''''''''M
                                M''''''''''''''''''''MM       MMM  M  M   :   M:  I:::::::::MOHMMMM'             M                         MM'''''''''''''''''''''''''''''''M'
                                M''''''''''''''''''''':MHMO:::::  ':HMM:H:M   .M   M:::::::::M::::::::::M::IMMM M                           MM''''''''''''''''''''''''''''''MM
                                M'''''''''''''''''''''M::::::::M  O  :::::M:   M   M::::::::::M::::::::::M:::::HM                            MM''''''''''''''''''''''''''''':M
                                M'''''''''''''''''''':M::::::::H  M H::::::M   M   M:::::::::::M:::::::::HM:::::MM                            MM'''''''''''''''''''''''''''''M
                                M''''''''''''''''''''M::::::::M   M M::::::M   MO  ':::::::::::MM:::::::::M:::::::M                            MM''''''''''''''''''''''''''''MM
                                M'''''''''''''''''''M:::::::::M  H  M::::::M    M   M:::::::::::M::::::::::H::::::MM                            MM'''''''''''''MMMMMMMMMMMMMMMMMM
                                HM'''''''''''''''''HM:::::::::   M  M::::::M    M   M::::::::::::M:::::::::M:::::::M                             M'''''''''' MMMMMMMMMMMMMMMMMMMM
                                 MMMMMMMMMMMMMMMMMMM:::::::::M   M  H::::::M    M   M::::::::::::H:::::::::M:::::::MM                             M''''''HMMMMMMMMMMMMMMMMMMMMMMM
                                HMMMMMMMMMMMMMMMMMMM:::::::::   M'  M:::::::         M::::::::::::M:::::::::::::::::M                              M''''MMMMMMMMMMMMMMMMMMMMMMMMM
                                 MMMMMMMMMMMMMMMMMM:::::::::M   M   ::::::::M    .   M:::::::::::::M::::::::::::::::.M                              M'MMMMMMMMMMMMMMMMMMMMMMMMMMM.
                                 MMMMMMMMMMMMMMMMMMH::::::::M   M  '::::::::M    M   M:::::::::::::H:::::::::::::::::MH                              MMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                'MMMMMMMMMMMMMMMMMM:::::::::M   :  :::::::::M    M    M:::::::::::::::::::::::::::::::M                             MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                MMMMMMMMMMMMMMMMMMM:::::::::    .  M::::::::M    '    M::::::::::::::::::::::::::::::::M                            'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                MMMMMMMMMMMMMMMMMMM::::::::M   M   M::::::::M         M:::::::::::::::::::::::::::::::::'                            MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                MMMMMMMMMMMMMMMMMMM::::::::M   M   M::::::::M         M:::::::::::::::::::::::::::::::::M                             MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                MMMMMMMMMMMMMMMMMMM::::::::M   M   M::::::::M          ::::::::::::::::::::::::::::::::::M                            MMMMMMMMMMMMMMMMMMMMMMMMMMMMMH
                                MMMMMMMMMMMMMMMMMMM::::::::    H   M::::::::M          M:::::::::::::::::::::::::::::::::M                             MMMMMMMMMMMMMMMMMMMMMMMMMMMM
                               :MMMMMMMMMMMMMMMMMMM:::::::H    '   H::::::::M          M::::::::::::::::::::::::::::::::::M                             MMMMMMMMMMMMMM:''''''''''M
                                MMMMMMMMMMMMMMMMMMM:::::::M        :::::::::M          M:::::::::::::::::::::::::::::::::::M                             MMMMMM.''''''''''''''''''.
                                 MMM:''''''''MMMMMM:::::::M   M   .:::::::::M'          O:::::::::::::::::::::M:::::::::::::M                             M'''''''''''''''''''''''M
                                 M'''''''''''''''MM:::::::M       M:::::::::M           M:::::::::::::::::::::M:::::::::::::M                            M''''''''''''''''''''''''M
                                 M''M''''''''''''MM:::::::        M:::::::::M           H:::::::::::::::::::::H:::::::::::::M.                         MM'''''''''''''''''''''''''.M
                                 M''M''''''''''''M::::::::        ::::::::::M.          M:::::::::::::::::::::H::::::::::::H:::                      MM'''''''''''''''''M''''''''''M
                                M.''M''''''''''''M:::::::M        .:::::::::M.          H::::::::::::::::::::::::::::::::::H::M                      M'''''''''''''''''''M'''''''''M
                                M'''M'''''''''''M::::::::M         :::::::::M.           ::::::::::::::::::::M:::::::::::::M::MM                     :'''''''''''''''''''M''''''''':
                                M'''M''''''''''MH::::::::M        ':::::::::M'           M:::::::::::::::::::M:::::::::::::M::M:M                     M'''.M''''''''M'''''''''''''''M
                                :''M''''''''''MM:::::::::'        '::::::::::M           M:::::::::::::::::::M:::::::::::::I:::::M                    M''''M''''''''M'''''M'''''''''M
                               M'''M'''''''''MM::::::::::          ::::::::::M           'H:::::::::::::::::M::::::::::::::::::::HM                   'M''''M'''''''M'''''M''''':'''M
                               M''''''''''''MM''H:::::::M         :::::::::::M            M:::::::::::::::::M:::::::::::::::::::::HM                   M'MMM:'''''''M'''''M'''''''''M
                               M''O'''''''IM'' :M:::::::M         :::::::::::M            M:::::::::::::::::M::::::::::::::::::::::M:                  M''''''''''''M''''''''''''MMM
                              M'''' M''''' ''''MM:::::::M          M:::::::::M          MMH::::::::::::::::M:::::M::::::::::::::::::M                  MM'''M''''''''M'''''M''''M
                              MM''MM'M''''M  M'MM:::::::M          H:::::::::M       .M::::::::::::::::::::M::::M::::::::::::::::::::M                    M'M:'''''''M''''MMMMMM
                               MM'':''''M'.''M:'M:::::::M          ::::::::::M      ::::::::::::::::::::::H:::::H:::::::::::::::::::::M                    M'M''''''''''''M
                                .M'''''':M''H'''M:::::::M MM:::::MMM:::::::::M    M:::::::::::::::::::::::M::::M::::::::::::::::::::::OM                   :M'''''''MMMMMM
                                 HM'''MM'M''M'''M:::::::M::::::::::::::::::::M   M:::::::::::::::::::::::HM:::M::::::::::::::::::::::::MM                    :''''MM
                                   MMMMMM''M MMMM::::::::::::::::::::::::::::M MM::::::::::::::::::::::::M::::M:::::::::::::::::::::::::MM                   'MMM
                                                M::::::::::::::::::::::::::::MHH:::::::::::::::::::::::::M:::M:::::::::::::::::::::::::::M.
                                               'M::::::::::::::::::::::::::::MMMM:::::::::::::::::::::::M:::M:::::::::::::::::::::::::::::M
                                               HM::::::::::::::::::::::::::::::MM:::::::::::::::::::::::M::OH::::::::::::::::::::::::::::::M
                                               MH:::::::::::::::::::::::::::::::M::::::::::::::::::::::M:::M:::::::::::::::::::::::::::::::M
                                               M::::::::::::::::::::::::::::::::M:::::::::::::::::::::OM::M::::::::::::::::::::::::::::::::M
                                               M::::::::::::::::::::::::::::::::MM::::::::::::::::::::M::MH::::::::::::::::::::::::::::::::M
                                               M::::::::::::::::::::::::::::::::OM::::::::::::::::::::M::M:::::::::::::::::::::::::::::::::MH'
                                               M::::M::::::::::::::::::::::::::::MM:::::::::::::::::::::M::::::::::::::::::::::::::::::::::M::M
                                               M::::M::::::::::::::::::::::::::::MM::::::::::::::::::::M::::::::::::::::::::::::::::::::::MM:::M
                                               M::::MH::::::::::::::::::::::::::::MH:::::::::::::::::::M::::::::::::::::::::::::::::::::::M::::I
                                               M::::HM::::::::::::::::::::::::::::MM::::::::::::::::::::::::::::::::::::::::::::::::::::::M::::.
                                              'M:::::M::::::::::::::::::::::::::::M:::::::::::::::::::::::::::::::::::::::::::::::::::::::M:::H
                                              MM:::::M:::::::::::::::::::::::::::::M::::::::::::::::::::::::::::::::::::::::::::::::::::::M:::OM
                                              MM:::::MH::::::::::::::::::::::::::::HH::::::::::::::::::::::::::::::::::::::::::::::::::::MM:::MMM
                                              MH::::::M::::::::::::::::::::::::::::MM::::::::::::::::::::::::::::::::::::::::::::::::::::M::::M:MM
                                              MH::::::M::::::::::::::::::::::::::::IM::::::::::::::::::::::::::::::::::::::::::::::::::::M::::M::M
                                              MH:::::::M:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::M::::M:::M:
                                              MH:::::::M::::::::::::::::::::::::::::M:::::::::::::::::::::::::::::::::::::::::::::::::::MM:::MM:::IM'
                                             'M::::::::HH:::::::::::::::::::::::::::.M::::::::::::::::::::::::::::::::::::::::::::::::::MH:::MM::::HMM
                                             IM:::::::::M:::::::::::::::::::::::::::HM::::::::::::::::::::::::::::::::::::::::::::::::::M::::M::::::MM:
                                             MM:::::::::H:::::::::::::::::::::::::::HM::::::::::::::::::::::::::::::::::::::::::::::::::M::::M::::::::M'
                                             MM::::::::::M:::::::::::::::::::::::::::M:::::::::::::::::::::::::::::::::::::::::::::::::HM::::M::::::::MM
                                             MM::::::::::MM::::::::::::::::::::::::::M:::::::::::::::::::::::::::::::::::::::::::::::::MH::::M::::::::.MM
                                             M::::::::::::M::::::::::::::::::::::::::MM::::::::::::::::::::::::::::::::::::::::::::::::M::::IM:::M:::::HMM
                                             M:::::::::::::M::::::::::::::::::::::::::M::::::::::::::::::::::::::::::::::::::::::::::::M:::::M:::M:::::::M.
                                            'M:::::::::::::MM:::::::::::::::::::::::::M:::::::::::::::::::::::::::::::::::::::::::::::::::::M::::MI::::::MM
                                            MH::::::::::::::MO::::::::::::::::::::::::M:::::::::::::::::::::::::::::::::::::::::::::::::::::M:::::M:::::::M
                                            MM:::::::::::::::M::::::::::::::::::::::::H::::::::::::::::::::::::::::::::::::::::::::::::::::HM:::::M:::::::MM
                                            M::::::::::::::::MM:::::::::::::::::::::::.::::::::::::::::::::::::::::::::::::::::::::::::::::M::::::M::::::::M
                                            M:::::::::::::::::M:::::::::::::::::::::::HM::::::::::::::::::::::::::::::::::::::::::::::::::HM::::::M:::::::::M
                                            M::::::::::::::::::MH::::::::::::::::::::::M::::::::::::::::::::::::::::::::::::::::::::::::::M:::::::M:::::::::M.
                                           :M::::::::::::::::::::::::::::::::::::::::::M:::::::::::::::::::::::::::::::::::::::::::::::::MM:::::::M::::::::::M
                                           'M::::::::::::::::::::::::::::::::::::::::::M:::::::::::::::::::::::::::::::::::::::::::::::::M::::::::M::::::::::M
                                            MH:::::::::::::::::::::::::::::::::::::::::M::::::::::::::::::::::::::::::::::::::::::::::::MM:::::::::::::::::::M
                                             MM::::::::::::::::::::::::::::::::::::::::HM::::::::::::::::::::::::::::::::::::::::::::::HM:::::::::H:::::::::M
                                              MM::::::::::::::::::::::::::::::::::::::::MM:::::::::::::::::::::::::::::::::::::::::::::MM::::::::M::::::::::M
                                               HM::::::::::::::::::::::::::::::::::::::::HMH::::::::::::::::::::::::::::::::::::::::::MM:::::::::M::::::::::M
                                                .MH:::::::::::::::::::::::::::::::::::::::HMM::::::::::::::::::::::::::::::::::::::::HMM:::::::::M:::::::::M
                                                  MM::::::::::::::::::::::::::::::::::::::MMMMMM:::::::::::::::::::::::::::::::::::::MH::::::::::M:::::::::M
                                                   MM::::::::::::::::::::::::::::::::::::MM:::MMMH::::::::::::::::::::::::::::::::::::::::::::::MM::::::::::
                                                    MMH:::::::::::::::::::::::::::::::::MM:::::M: MM::::::::::::::::::::::::::::::::::::::::::::M:::::::::M
                                                    MMMM::::::::::::::::::::::::::::::MMI::::::M  MMMM:::::::::::::::::::::::::::::::::::::::::M::::::::::M
                                                   MM:MMM:::::::::::::::::::::::::::MMM::::::::M  'M::MH:::::::::::::::::::::::::::::::::::::::M::::::::::H
                                                   HM:::MMM:::::::::::::::::::::::IMM::::::::::M   M::::MM::::::::::::::::::::::::::::::::::::MM:::::::::M
                                                   IM::::MMM:::::::::::::::::::::MM::::::MM:::M     ::::::MM::::::::::::::::::::::::::::::::::MO:::::::::M
                                                   MM::::::MMM:::::::::::::::::MM:::::::M:M:::M     M:::::::MM:::::::::::::::::::::::::::::::MM:::::::::M
                                                   MM::::::MOMM::::::::::::::MM::::::::M::::::M     M:::::::::HM:::::::::::::::::::::::::::::MM:::::::::M
                                                   MM::::::M::IMM:::::::::MMM::::::::HM:::::::M     .:::::::::::MM::::M:::::::::::::::::::::HM:::::::::M
                                                   MM::::::M::::MM::::::M:::::::::::MM::::::::M     I:::::::::::::MM::::M:::::::::::::::::::M::::::::::M
                                                   MH::::::M:::::MM::::::::::::::::MM:::::::::M      O::::::::::::::M::::MM::::::::::::::::HM:::::::::M
                                                   MM::::::MH:::::M:::::::::::::::MI::::::::OMH      'M::::::::::::::MH::::MM::::::::::::::M:::::::::HM
                                                   MM:::::::M:::::H::::::::::::::M::::::::HMH           MM::::::::::::H::::::M:::::::::::::::::::::::M'
                                                    MM::::::M::::::::::::::::::HM::::::::M'               MM:::::::::::H::::::MM:::::::::::::::::::::M
                                                     MM:::::H::::::::::::::::::M::::::MM                     MI:::::::::::::::::MM::::::::::::::::::MM
                                                      MM:::::M::::::::::::::::M:::::MM                         MM::::::::::::::::MMM::::::::::::::::M
                                                        MH::::M::::::::::::::M::HMMMM                            MMM:::::::::::::::MMM:::::::::::::MM'
                                                         MMMMMMMMMMMMMMMMMMMMMMMMMMMM                              'MM::::::::::::::MMMMM:::::::::MMM
                                                          MMMMMMMMMMMMMMMMMMMMMMMMMMM                                 MM:::::::::::::MMMM:::MMMMMMMMM
                                                           MMMMMMMMMMMMMMMMMMMMMMM.M                                   :MMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                                            M''''''''''''''''''''''MM                                   MMMMMMMMMMMMMMMMMMMMMMMMMMM'M
                                                            M'''''''''''''''''''''''M                                    MMMMMMMMMMMMMMMMMMMMMMM:'''''
                                                          O''''''''''''''''''''''''''                                     MMMMMMMMMM.I.''''''''''''''M
                                                          H''M'''''''''''''''''''''''M                                     M''''''''''''''''''''''''''M
                                                         MM'''' :MM'''''''''''''''''''M                                   'M''.MH:''''''''''''''''''''M
                                                      MM'''''''''''''''''''''''''''''HM                                   .M.''''''''''''''''''''''''''M
                                                   MM:''''''''''''''''''''''''''''''MMMM                                  MM'''''''''''''''''''''''''''M
                                                MMMMMMM'''''''''''''''''''''''''' MMMMMM                                 'M'''''''IHMMMMMMMM '''''''''''M
                                             MMMMMMMMMMM:'''''''''''''''''''''HMMMMMMMMM                                 MH'':MMMMMMMMMMMMMMMMM'''''''''M
                                          'MMMMMMMMMMMMMMMH''''''''''HMMMMMMMMMMMMMMMMMMM                               OM'MMMMMMMMMMMMMMMMMMMMMH'''''''MM
                                        MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                               M'MMMMMMMMMMMMMMMMMMMMMMMM '''''MM
                                     'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                               MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:
                                     MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                              MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                               MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM.                                 MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                    MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:                                     'MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                     .MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM                                               HMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMH
                                            'MMMMMMMMMM:                                                             MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                                                                                                      MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                                                                                                      MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
                                                                                                                      MMMMMMMMMMMMMMMMMMMMMMMMMMMM.
                                                                                                                       MMMMMMMMMMMMMMMMMMMMM


*/


#endif /* USART1TASK_USART1TASK_H_ */
