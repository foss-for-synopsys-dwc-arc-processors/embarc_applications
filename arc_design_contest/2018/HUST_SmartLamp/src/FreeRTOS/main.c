/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */

/**
 * \defgroup	EMBARC_APP_FREERTOS_KERNEL	embARC FreeRTOS Kernel Example
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_OS_FREERTOS
 * \brief	embARC Example for testing FreeRTOS task switch and interrupt/exception handling
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *
 * ### Design Concept
 *     This example is designed to show the functionality of FreeRTOS.
 *
 * ### Usage Manual
 *     Test case for show how FreeRTOS is working by task switching and interrupt/exception processing.
 *     ![ScreenShot of freertos-demo under freertos](pic/images/example/emsk/emsk_freertos_demo.jpg)
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_KERNEL
 * \brief	main source file of the freertos demo
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_KERNEL
 * @{
 */

#include "embARC.h"
#include "embARC_debug.h"
#include <stdlib.h>
#include "FreeRTOS.h"
#include "stdint.h"


#include "timer.h"

#include "malloc.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

//
#include "stdlib.h"

#include "cm3232.h"
#include "usart1task.h"
#include "usart3task.h"
#include "usart4task.h"
#include "ESP8266.h"
#include "package.h"
#include "initsys.h"
#include "bright.h"
#include "irf.h"

#include "task_config.h"
#include "app_config.h"

//看门狗实现：在低等级任务中，标记事件标志组。最高等级任务检测到所有任务正常执行，就喂狗�#include "sys.h"


//看门狗实现：在低等级任务中，标记事件标志组。最高等级任务检测到所有任务正常执行，就喂狗。 看门狗时间根据实际运行情况来
 /*
 低级任务标记：
 xEventGroupSetBits(xCreatedEventGroup, TASK_BIT_3);
 高级任务检测：
 uxBits = xEventGroupWaitBits(xCreatedEventGroup,
                                          TASK_BIT_ALL,
                                          pdTRUE,
                                         pdTRUE,
                                          xTicksToWait);

         if((uxBits & TASK_BIT_ALL) == TASK_BIT_ALL)
         {
              IWDG_Feed();
              printf("五个用户任务都正常运行\r\n");
         }
         else
*/
//#include "strpos.h"//测试
/************************************************
************************************************/
/*******************任务参数*******************/
////任务优先级
//#define START_TASK_PRIO		1//宏定义在app_config.h中
////任务堆栈大小
//#define START_STK_SIZE 		256
//任务句柄
TaskHandle_t StartTask_Handler;//在main中定义
//任务函数
//void start_task(void *pvParameters);//在任务函数所在文件和头文件定义



////任务优先级
//#define TASK1_TASK_PRIO		2
////任务堆栈大小
//#define TASK1_STK_SIZE 		256
//任务句柄
TaskHandle_t Task1Task_Handler;
//任务函数
void task1_task(void *pvParameters);
//#define task1_task_delay      20//



////任务优先级
//#define KEYPROCESS_TASK_PRIO 3
////任务堆栈大小
//#define KEYPROCESS_STK_SIZE  256
//任务句柄
TaskHandle_t Keyprocess_Handler;
//任务函数
void Keyprocess_task(void *pvParameters);
//#define Keyprocess_task_delay 20//
uint8_t KEY_Scan(void);//键盘扫描

////任务优先级
//#define USART1_TASK_PRIO 30
////任务堆栈大小
//#define USART1_STK_SIZE  256
//任务句柄
//TaskHandle_t USART1_Handler;
//任务函数
//void USART1_task(void *pvParameters);
//#define USART1_task_delay     10//任务运行间隔 ms

//TaskHandle_t FREE_ERRORMEM_Handler;
//TaskHandle_t smart_config__Handler;


/**********************队列****************************/
QueueHandle_t Key_Queue;   		      //按键值消息队列句柄


uint8_t KEY_Scan(void)
{
	static uint8_t key_up=1;//按键按松开标志
	uint32_t KEY;
//	if(mode)key_up=1;  //支持连按
	KEY=button_read(EMSK_BUTTON_MASK);
	if(key_up&&KEY)
	{
		vTaskDelay(10);//去抖动
		key_up=0;
		if(KEY==0x01)return 0x01;
		else if(KEY==0x02)return 0x02;
		else if(KEY==0x04)return 0x04;
	}else if(KEY==0x00)key_up=1;
	return 0;// 无按键按下
}

char InfoBuffer[1000];				//保存测试信息的数组

void check_task_state(void)
{
#if 1
	//优先级与堆栈使用情况
	memset(InfoBuffer,0,1000);				//信息缓冲区清零
#if 1
	printf("/*************  vTaskList()         *************/\r\n");
	printf("/******R ready B block S suspend D delete*******/\r\n");
	printf("name\t\t\t\tflag\tprio\tleast\tnum\r\n");
	vTaskList(InfoBuffer);							//获取所有任务的信息
	printf("%s\r\n",InfoBuffer);					//通过串口打印所有任务的信息
#endif
	printf("/**************************end**************************/\r\n");
	//任务运行时间情况
#if 0
	memset(InfoBuffer,0,1000);				//信息缓冲区清零
  vTaskGetRunTimeStats(InfoBuffer);		//获取任务运行时间信息
	printf("任务名\t\t\t运行时间\t运行所占百分比\r\n");
	printf("%s\r\n",InfoBuffer);
#endif
#endif
}

//查询Message_Queue队列中的总队列数量和剩余队列数量
void check_msg_queue(void)
{
    uint8_t *p;
		uint8_t msgq_remain_size;	//消息队列剩余大小
    uint8_t msgq_total_size;     //消息队列总大小
    uint32_t freemem;

    taskENTER_CRITICAL();   //进入临界区

		//任务相关
		check_task_state();

    msgq_remain_size=uxQueueSpacesAvailable(USART1_Queue);//得到队列剩余大小
    msgq_total_size=uxQueueMessagesWaiting(USART1_Queue)+uxQueueSpacesAvailable(USART1_Queue);//得到队列总大小，总大小=使用+剩余的。
		p=pvPortMalloc(30);	//申请内存 sprintf的字符数一定不能超过30，不然内存炸了

		sprintf((char*)p,"USART1_Q:%d/%d",msgq_remain_size,msgq_total_size);	//显示DATA_Msg剩余大小/总大小
    printf("%s\r\n",p);
	#if 1 //
		msgq_remain_size=uxQueueSpacesAvailable(USART1_Task_Queue);//得到队列剩余大小
    msgq_total_size=uxQueueMessagesWaiting(USART1_Task_Queue)+uxQueueSpacesAvailable(USART1_Task_Queue);//得到队列总大小，总大小=使用+剩余的。
		sprintf((char*)p,"USART1_tsak_Q:%d/%d",msgq_remain_size,msgq_total_size);	//显示DATA_Msg剩余大小/总大小
    printf("%s\r\n",p);

	msgq_remain_size=uxQueueSpacesAvailable(USART3_Queue);//得到队列剩余大小
msgq_total_size=uxQueueMessagesWaiting(USART3_Queue)+uxQueueSpacesAvailable(USART3_Queue);//得到队列总大小，总大小=使用+剩余的。
	sprintf((char*)p,"USART3_Queue:%d/%d",msgq_remain_size,msgq_total_size);	//显示DATA_Msg剩余大小/总大小
printf("%s\r\n",p);

		msgq_remain_size=uxQueueSpacesAvailable(USART3_Send_Queue);//得到队列剩余大小
    msgq_total_size=uxQueueMessagesWaiting(USART3_Send_Queue)+uxQueueSpacesAvailable(USART3_Send_Queue);//得到队列总大小，总大小=使用+剩余的。
		sprintf((char*)p,"u3_Send_Q:%d/%d",msgq_remain_size,msgq_total_size);	//显示DATA_Msg剩余大小/总大小
    printf("%s\r\n",p);

		msgq_remain_size=uxQueueSpacesAvailable(USART3_Task_rn_other_Queue);//得到队列剩余大小
    msgq_total_size=uxQueueMessagesWaiting(USART3_Task_rn_other_Queue)+uxQueueSpacesAvailable(USART3_Task_rn_other_Queue);//得到队列总大小，总大小=使用+剩余的。
		sprintf((char*)p,"rn_other_Q:%d/%d",msgq_remain_size,msgq_total_size);	//显示DATA_Msg剩余大小/总大小
    printf("%s\r\n",p);

		msgq_remain_size=uxQueueSpacesAvailable(USART3_Task_server_Queue);//得到队列剩余大小
    msgq_total_size=uxQueueMessagesWaiting(USART3_Task_server_Queue)+uxQueueSpacesAvailable(USART3_Task_server_Queue);//得到队列总大小，总大小=使用+剩余的。
		sprintf((char*)p,"u3server_Q:%d/%d",msgq_remain_size,msgq_total_size);	//显示DATA_Msg剩余大小/总大小
    printf("%s\r\n",p);

	msgq_remain_size=uxQueueSpacesAvailable(USART4_Queue);//得到队列剩余大小
msgq_total_size=uxQueueMessagesWaiting(USART4_Queue)+uxQueueSpacesAvailable(USART4_Queue);//得到队列总大小，总大小=使用+剩余的。
	sprintf((char*)p,"U4_Queue:%d/%d",msgq_remain_size,msgq_total_size);	//显示DATA_Msg剩余大小/总大小
dprintf("%s\r\n",p);

	msgq_remain_size=uxQueueSpacesAvailable(USART4_Send_Queue);//得到队列剩余大小
msgq_total_size=uxQueueMessagesWaiting(USART4_Send_Queue)+uxQueueSpacesAvailable(USART4_Send_Queue);//得到队列总大小，总大小=使用+剩余的。
	sprintf((char*)p,"U4_Send_Q:%d/%d",msgq_remain_size,msgq_total_size);	//显示DATA_Msg剩余大小/总大小
dprintf("%s\r\n",p);
	#endif

    freemem=xPortGetFreeHeapSize();		//获取剩余内存大小
    sprintf((char*)p,"freemem:%d",freemem);	//显示DATA_Msg剩余大小
    printf("%s\r\n",p);

		vPortFree(p);		//释放内存


    taskEXIT_CRITICAL();    //退出临界区
}

void start_task();//(void *pvParameters);

int main(void)
{

//	supersound_IOinit_1();
	uart1_init(115200);					//初始化串口
	uart3_init(115200);
	pwm_gpio_int();
	usart4_init();
	init_system();					//初始化系统
//	timertest_int();
	CM3232_Init();
	irf_init();
	//	vTaskSuspendAll();//super sound
//	taskENTER_CRITICAL();           //进入临界区    adc
	//创建开始任务
#if 0
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄
#else
    start_task();
//    user_idel();
#endif
//    taskEXIT_CRITICAL();            //退出临界区
//    xTaskResumeAll();
#if 1
    vTaskStartScheduler();
#else
//	while(1)
	{
//		vTaskSuspend(NULL);
	}//开启任务调度
#endif
}
#define SUSPEND 0
//开始任务任务函数
void start_task()//(void *pvParameters)
{
#if SUSPEND
	vTaskSuspendAll();//super sound
#else
    taskENTER_CRITICAL();           //进入临界区   adc
#endif

	//创建消息队列
    Key_Queue=xQueueCreate(KEYMSG_Q_NUM,sizeof(uint8_t));        //创建消息Key_Queue
		USART1_Queue=xQueueCreate(USART1_Q_NUM,sizeof(usart_dat));//底层串口1缓冲数据Queue
		USART1_Task_Queue=xQueueCreate(USART1_TASK_Q_NUM,sizeof(usart_task_data));
		USART3_Queue=xQueueCreate(USART3_Q_NUM,sizeof(usart_dat));//底层串口3缓冲数据Queue
//		printf("%d",USART3_Queue);
		USART3_Task_server_Queue=xQueueCreate(USART3_TASK_SERVER_Q_NUM ,sizeof(usart_task_data));//串口3分帧数据：服务器数据
		USART3_Task_rn_other_Queue=xQueueCreate(USART3_TASK_RN_OTHER_Q_NUM  ,sizeof(usart_task_data));//串口3分帧数据：\r\n数据和没有标记的数据
	  USART3_Send_Queue = xQueueCreate(USART3_Send_Q_NUM ,sizeof(usart_task_data));
		pack2server_queue = xQueueCreate(pack2server_Q_NUM  ,sizeof(uint16_t));
		connect_to_server_Queue = xQueueCreate(connect_to_server_Q_NUM   ,sizeof(uint8_t));
		USART4_Queue = xQueueCreate(USART4_Q_NUM,sizeof(uint8_t));
		USART4_Send_Queue = xQueueCreate(USART4SEND_Q_NUM	,sizeof(usart_task_data));
		SuperSound_Queue = xQueueCreate(SuperSound_Q_NUM   ,sizeof(uint8_t));
#if ASSERT_PRINTF_ENABLE
		Dprintf_Queue = xQueueCreate(Dprintf_Q_NUM,sizeof(usart_task_data));
#endif
//		USART5_Queue = xQueueCreate(USART5_Q_NUM,sizeof(u8));
//		USART5_Send_Queue = xQueueCreate(USART5SEND_Q_NUM	,sizeof(usart_task_data));

	//创建二值信号量
	BinarySemaphore_free=xSemaphoreCreateBinary();

	/* 创建互斥信号量 */
	SendenableMutexSemaphore=xSemaphoreCreateMutex();
	#if ASSERT_PRINTF_ENABLE
//   xMutex = xSemaphoreCreateMutex();

	#endif

	//定时器
	//创建wifi单次定时器
	OneShot_config60_Handle=xTimerCreate((const char*			)"OneShotTimer60s",
																			(TickType_t			)  WIFI_TIM60S_TICKS ,
																			(UBaseType_t			)pdFALSE,//单次定时器
																			(void*					)  WIFI_TIM60S_ID,
																			(TimerCallbackFunction_t) OneShot60_10Callback);
	OneShot_config10_Handle=xTimerCreate((const char*			)"OneShotTimer10s",
																			(TickType_t			)  WIFI_TIM10S_TICKS ,
																			(UBaseType_t			)pdFALSE,//单次定时器
																			(void*					)  WIFI_TIM10S_ID,
																			(TimerCallbackFunction_t) OneShot60_10Callback);
	//延时启动发送服务器数据功能
	OneShot_CanServer_Handle=xTimerCreate((const char*			)"OneShotCanServer",
																			(TickType_t			)  CanServer_TICKS ,
																			(UBaseType_t			)pdFALSE,//单次定时器
																			(void*					)  CanServer_ID,
																			(TimerCallbackFunction_t) CanServer_Callback);


	//定时发送0002包
	period40or5_0002_Handle=xTimerCreate((const char*			)"period40or5_0002",
																			(TickType_t			)  PKG0002_5S_TICKS ,//测试时5s一次
																			(UBaseType_t			)pdTRUE,//周期定时器
																			(void*					)  PKG0002_TIM_ID,
																			(TimerCallbackFunction_t)period40or5_0002_Callback);
//软件定时器wifi timer
#if 1
	OneShot_config1MS_Handle=xTimerCreate((const char*			)"wifi_timer",
																			(TickType_t			)  WIFI_1MS_TICKS ,//测试时5s一次
																			(UBaseType_t			)pdTRUE,//周期定时器
																			(void*					)  WIFI_TIM1MS_ID,
																			(TimerCallbackFunction_t)softtimer);
	period_waittouch_Handle=xTimerCreate((const char*			)"period_waittouch",
																			(TickType_t			)  WAIT_TOUCH_TICKS ,//测试时5s一次
																			(UBaseType_t			)pdTRUE,//周期定时器
																			(void*					)  WAIT_TOUCH_ID,
																			(TimerCallbackFunction_t)period_waittouch_Callback);
	//调节灯光亮度定时器
	period_Bright_Handle=xTimerCreate((const char*			)"period_Bright",
																			(TickType_t			)  Bright_TICKS ,//周期
																			(UBaseType_t			)pdTRUE,//周期定时器
																			(void*					)  Bright_ID,
																			(TimerCallbackFunction_t)period_Bright_Callback);


#endif
    //创建TASK1任务
    xTaskCreate((TaskFunction_t )task1_task,
                (const char*    )"task1_task",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_TASK_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    //创建TASK2任务
    xTaskCreate((TaskFunction_t )Keyprocess_task,
                (const char*    )"keyprocess_task",
                (uint16_t       )KEYPROCESS_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )KEYPROCESS_TASK_PRIO,
                (TaskHandle_t*  )&Keyprocess_Handler);

		//串口1数据，task处理任务 分帧操作
		xTaskCreate((TaskFunction_t )USART1_task,
                (const char*    )"USART1_task",
                (uint16_t       )USART1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )USART1_TASK_PRIO,
                (TaskHandle_t*  )&USART1_Handler);
		//串口1 APP层处理任务
		xTaskCreate((TaskFunction_t )usart1_app_task,
                (const char*    )"usart1_app_task",
                (uint16_t       )USART1_APP_SIZE,
                (void*          )NULL,
                (UBaseType_t    )USART1_APP_PRIO,
                (TaskHandle_t*  )&USART1_app_Handler);

		//串口3数据，task处理任务 分帧操作
		xTaskCreate((TaskFunction_t )USART3_task,
                (const char*    )"USART3_task",
                (uint16_t       )USART3_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )USART3_TASK_PRIO,
                (TaskHandle_t*  )&USART3_Handler);

		//free串口3接收错误内存
		xTaskCreate((TaskFunction_t )FREE_ERRORMEM_task,
                (const char*    )"FREE_ERRORMEM_task",
                (uint16_t       )FREE_ERRORMEM_SIZE,
                (void*          )NULL,
                (UBaseType_t    )FREE_ERRORMEM_PRIO,
                (TaskHandle_t*  )&FREE_ERRORMEM_Handler);

		//串口3发送数据，task处理任务
		xTaskCreate((TaskFunction_t )USART3_send_task,
                (const char*    )"USART3_send_task",
                (uint16_t       )USART3_SEND_SIZE,
                (void*          )NULL,
                (UBaseType_t    )USART3_SEND_PRIO,
                (TaskHandle_t*  )&USART3_send_Handler);

		//串口3 APP层处理任务
		xTaskCreate((TaskFunction_t )usart3_app_task,
                (const char*    )"usart3_app_task",
                (uint16_t       )USART3_APP_SIZE,
                (void*          )NULL,
                (UBaseType_t    )USART3_APP_PRIO,
                (TaskHandle_t*  )&USART3_app_Handler);

		//串口4 APP层处理任务
		xTaskCreate((TaskFunction_t )USART4_app_task,
                (const char*    )"usart4_app_task",
                (uint16_t       )USART4_APP_SIZE,
                (void*          )NULL,
                (UBaseType_t    )USART4_APP_PRIO,
                (TaskHandle_t*  )&USART4_app_Handler);
		//串口5 APP层处理任务
//		xTaskCreate((TaskFunction_t )USART5_app_task,
//                (const char*    )"usart5_app_task",
//                (uint16_t       )USART5_APP_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )USART5_APP_PRIO,
//                (TaskHandle_t*  )&USART5_app_Handler);
//		irf_init();
		//irf APP层处理任务
		xTaskCreate((TaskFunction_t )IRF_app_task,
                (const char*    )"IRF_app_task",
                (uint16_t       )IRF_APP_SIZE,
                (void*          )NULL,
                (UBaseType_t    )IRF_APP_PRIO,
                (TaskHandle_t*  )&IRF_app_Handler);
		//mp3播放控制
		xTaskCreate((TaskFunction_t )MP3_task,
                (const char*    )"MP3_task",
                (uint16_t       )MP3_TASK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )MP3_TASK_PRIO,
                (TaskHandle_t*  )&MP3_TASK_Handler);
		//发送给服务器的包进行组包
		xTaskCreate((TaskFunction_t )pack2server,
                (const char*    )"pack2server",
                (uint16_t       )pack2server_SIZE,
                (void*          )NULL,
                (UBaseType_t    )pack2server_PRIO,
                (TaskHandle_t*  )&pack2server_Handler);
			xTaskCreate((TaskFunction_t )connect_to_server,
									(const char*    )"connect_to_server",
									(uint16_t       )connect_to_server_SIZE,
									(void*          )NULL,
									(UBaseType_t    )connect_to_server_PRIO,
									(TaskHandle_t*  )&connect_to_server_Handler);
		vTaskSuspend(connect_to_server_Handler);

#if 0
		//wifi配置任务
			xTaskCreate((TaskFunction_t )smartconfig_task,
									(const char*    )"smartconfig_task",
									(uint16_t       )SMARTCONFIG_TASK_SIZE,
									(void*          )NULL,
									(UBaseType_t    )SMARTCONFIG_TASK_PRIO,
									(TaskHandle_t*  )&smart_config_Handler);

		vTaskDelete(smart_config_Handler);
#endif
		//任务相关初始化 初始化系统
	mcu_sendcmd2touch(TOUCH_ON);//开启触控板

	xTimerStart(period40or5_0002_Handle,10);//定时连接服务器和发送0002包

    vTaskDelete(StartTask_Handler); //删除开始任务
#if !SUSPEND
    taskEXIT_CRITICAL();            //退出临界区
#else
    xTaskResumeAll();
#endif
}

//task1任务函数
void task1_task(void *pvParameters)
{
	uint8_t key,i=0;
    BaseType_t err;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 10;

	/* 获取当前的系统时间 */
	xTimerStart(OneShot_config1MS_Handle,1);
    xLastWakeTime = xTaskGetTickCount();

    printf("task1 success\r\n");
	while(1)
	{
		vTaskDelay(10);
		key=KEY_Scan();           	//扫描按键

        if((Key_Queue!=NULL)&&(key))   	//消息队列Key_Queue创建成功,并且按键被按下
        {
//					if(key) printf("key:%d\r\n",key);
            err=xQueueSend(Key_Queue,&key,10);
            if(err==errQUEUE_FULL)   	//发送按键值
            {
                printf("队列Key_Queue已满，数据发送失败!\r\n");
            }
        }
        i++;
        if(i==50)
        	led_write(0x00,EMSK_LED_MASK);
        if(i==100)
        {
            i=0;
            led_write(0x01,EMSK_LED_MASK);
        }
//        vTaskDelay(task1_task_delay);                           //延时10ms，也就是10个时钟节拍
						/* vTaskDelayUntil是绝对延迟，vTaskDelay是相对延迟。*/
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

#define CORRECT_COUNT 20
//Keyprocess_task函数
void Keyprocess_task(void *pvParameters)
{
	uint8_t num=0,num_up=0,key;
    uint8_t* buffer;
	uint8_t temp;
	BaseType_t err;
	unsigned long adc_temp;
//    usart_data usart1_temp;
//		usart_dat usart1_temp;
		usart_task_data usart1_task_temp;
	eTaskState state_test;
    printf("Keyprocess_task success\r\n");
	while(1)
	{
        if(Key_Queue!=NULL)
        {
            if(xQueueReceive(Key_Queue,&key,portMAX_DELAY))//请求消息Key_Queue
            {
                switch(key)
                {
                    case 0x01:		//KEY_UP控制LED1
                        num_up++;
                        led_write((num_up%8),EMSK_LED_MASK);
                        check_msg_queue();//检usart_Queue队列的容量

                    	//noperson
                    	server_control_light(0x21);
                    	study_ctr.noperson = ReadData2_7705();
                    	for(temp=0;temp<CORRECT_COUNT;temp++)
                    	{
                    		adc_temp = ReadData2_7705();
                    		dprintf("%d temp1= %d\r\n",temp,adc_temp);
                    		if(adc_temp<study_ctr.noperson) study_ctr.noperson = adc_temp;
                    		vTaskDelay(100);
                    	}
                    	dprintf("noperson %d\r\n",study_ctr.noperson);
                    	server_control_light(0x22);
//                        compare_pwm = 30;
                        break;
                    case 0x02:
                    	dprintf("start correct\r\n");



                    	//noperson up
                    	server_control_light(0x23);
                    	study_ctr.up = ReadData1_7705();
                    	for(temp=0;temp<CORRECT_COUNT;temp++)
                    	{
                    		adc_temp = ReadData1_7705();
                    		dprintf("%d temp2 = %d\r\n",temp,adc_temp);
                    		if(adc_temp<study_ctr.up) study_ctr.up = adc_temp;
                    		vTaskDelay(100);
                    	}
                    	dprintf("up %d\r\n",study_ctr.up);

                    	//noperson forward
                    	server_control_light(0x24);
                    	study_ctr.forward = ReadData2_7705();
                    	for(temp=0;temp<CORRECT_COUNT;temp++)
                    	{
                    		adc_temp = ReadData2_7705();
                    		dprintf("%d temp3= %d\r\n",temp,adc_temp);
                    		if(adc_temp<study_ctr.forward) study_ctr.forward = adc_temp;
                    		vTaskDelay(100);
                    	}
                    	dprintf("forward %d\r\n",study_ctr.forward);
                    	server_control_light(0x25);

//                    	compare_pwm = 60;
//                    	start_config_task();
//						printf("key1_press:%d\r\n",get_rtc_sec());
#if 0//测试模拟串口4
			senddata_by_usart4((uint8_t*)"hello usart5\r\n",14);
#endif
#if 0//光感测试
										printf("bright is %d\r\n",CM3232_Bright(3));
										printf("bright is %d\r\n",CM3232_Bright(5));

#endif
//											disconnect_TCP();
//											delay_ms(30);
//											connect_to_server();
#if 0
												temp = 1;
												err=xQueueSend(connect_to_server_Queue,&temp,10);
												if(err==errQUEUE_FULL)   	//发送消息帧
												{

													printf("队列connect_to_server_Queue已满，1数据发送失败!\r\n");
												}
												vTaskResume(connect_to_server_Handler);
#endif
                        #if 0
										//底层每次发送地址过来
                        if(xQueueReceive(USART1_Queue,&usart1_temp,portMAX_DELAY))
                        {
                            buffer=pvPortMalloc(usart1_temp.plen+1);
                            memcpy(buffer,usart1_temp.pstr,usart1_temp.plen);
														buffer[usart1_temp.plen]=0;
                            printf("uasrt1data:%s\r\n",buffer);
                            //free内存
                            vPortFree(buffer);
                            vPortFree(usart1_temp.pstr);
                        }
												//底层每次发送200的缓冲buff
												if(xQueueReceive(USART1_Queue,&usart1_temp,0))
                        {
													buffer=pvPortMalloc(usart1_temp.len+1);
													memcpy(buffer,usart_data_all[usart1_temp.buff_now],usart1_temp.len);
													buffer[usart1_temp.len]=0;
													printf("usart1 %4d:%s\r\n",usart1_temp.len,buffer);
                          //free内存
                          vPortFree(buffer);
													release_buff(usart1_temp.buff_now);

												}
												if(xQueueReceive(USART1_Task_Queue,&usart1_task_temp,0))//处理每一帧数据，模拟处理任务
										{
											buffer=pvPortMalloc(usart1_task_temp.len+1);
											memcpy(buffer,usart1_task_temp.pstr,usart1_task_temp.len);
											buffer[usart1_task_temp.len]=0;
											printf("usart1task %3d %1d:%s\r\n",usart1_task_temp.len,usart1_task_temp.type,buffer);
											 vPortFree(buffer);
											vPortFree(usart1_task_temp.pstr);
										}





                        #endif
                        break;
                    case 0x04:		//KEY0刷新LCD背景
                    	study_ctr.startstudy = !study_ctr.startstudy;
                    	dprintf("study flag %d\r\n",study_ctr.startstudy );
                    	if(study_ctr.startstudy)
                    	{
                    		server_control_light(0x28);
                    		mp3_play(1);
        					study_ctr.time = get_rtc_sec();
        					study_ctr.timeflag = 1;
                    	}

                    	/*
                        num++;
//                        compare_pwm = 90;
                    printf("key0_press:%d\r\n",num);

										//进入wifi配置模式   check_task_state之后再进行查询状态就会出错
										state_test = eTaskGetState(smart_config_Handler);
										printf("state = %d\r\n",state_test);
										*/
										#if 0
										if(state_test==eDeleted||state_test==eReady)//check_task_state之后再进行查询状态 虽然没有任务，但是状态变成eReady
										{
											printf("creat\r\n");
//											vTaskResume(smart_config__Handler);
													//wifi配置任务
											xTaskCreate((TaskFunction_t )smartconfig_task,
																	(const char*    )"smartconfig_task",
																	(uint16_t       )SMARTCONFIG_TASK_SIZE,
																	(void*          )NULL,
																	(UBaseType_t    )SMARTCONFIG_TASK_PRIO,
																	(TaskHandle_t*  )&smart_config_Handler);
										}
										else if(state_test==eBlocked)//正常情况下， eBlocked状态进入删除任务的模式
										{

											printf("delete\r\n");
											stop_config_task(0);
										}
										#endif
                        break;
                }
            }
        }
		vTaskDelay(Keyprocess_task_delay);      //延时10ms，也就是10个时钟节拍
	}
}







