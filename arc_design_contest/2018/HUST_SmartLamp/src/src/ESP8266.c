/*
 * ESP8266.c
 *
 *  Created on: 2018年5月3日
 *      Author: Administrator
 */


#include "ESP8266.h"


#include "malloc.h"
#include "string.h"
#include "timer.h"

#include "usart3task.h"
#include "usart1task.h"
#include "usart4task.h"
#include "strpos.h"
#include "package.h"

#include "task_config.h"
//#include "suart.h"
//#include "touch.h"
//#include "iwdg.h"
#define delay_ms vTaskDelay
#define ESP8266_PRINT_RECEIVE 1//是否打印接收到的数据
#define WIFI_TRYTIMES					10//尝试读取wifi模块回执信息次数

TaskHandle_t smart_config_Handler;
TaskHandle_t connect_to_server_Handler;

TimerHandle_t	OneShot_config60_Handle;			//单次定时器60s句柄
TimerHandle_t	OneShot_config10_Handle;			//单次定时器10s句柄
TimerHandle_t	OneShot_config1MS_Handle;//1ms定时器句柄

QueueHandle_t connect_to_server_Queue;

char IP_buf[16]="233.119.120.110";
char portnum[11]="9854";

//8266 smart config
//先挂起除了串口3以外的任务，创建smartconfig任务
//检测触控板任务也要继续运行，方便退出配置模式
void smartconfig_task(void *pvParameters)
{
	uint8_t time=100;
	BaseType_t err;
	uint8_t wificonfig_flag = 0;//复用，标记接收到“开始配置wifi”  同时标记是否配置成功
	usart_task_data USART3_task_data;//USART3task分帧数据
	#if ESP8266_PRINT_RECEIVE//测试串口3接收到的数据
	uint8_t* buffer;
	#endif
	//
	printf("smart_config start!\r\n");
	{
		free_usart3_rn_other_queue();
		while((!send_cmd_wait_ack("AT","OK",WIFI_TRYTIMES,1))&&--time)//检查WIFI模块是否在线
	{
		quit_trans();//退出透传
		send_cmd_wait_ack("AT+CIPMODE=0","OK",WIFI_TRYTIMES,1);  //关闭透传模式
		delay_ms(10);
	}
	if(time==0)
	{
		goto stop_smartconfig;
	}
	free_usart3_rn_other_queue();
	send_cmd_wait_ack("AT+CWMODE=1","OK",WIFI_TRYTIMES,1);//设为station模

	time=3;
	free_usart3_rn_other_queue();
	while((!send_cmd_wait_ack("AT+CWSTOPSMART","OK",WIFI_TRYTIMES,1))&&--time);//先退出smartconfig，

	time=10;
	free_usart3_rn_other_queue();
	while((!send_cmd_wait_ack("AT+CWSTARTSMART","OK",WIFI_TRYTIMES,1))&&--time);//检测发送是否成功
	if(time==0)
	{
		goto stop_smartconfig;
	}


	//开启60s定时器，时间到自动退出配置模式
	err = xTimerStart(OneShot_config60_Handle,100);
	configASSERT(err);
	while(1)
	{
		//先检测有没有 get wifi info 标志
		if(wificonfig_flag==0)
		{
			if(xQueueReceive(USART3_Task_rn_other_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//接收到分帧串口数据
			{//可能出现receive了队列。没有进行到free 就触发60s的定时器：改低定时器优先级或者禁止任务调度？
				//这里其实可以检测到wifi的信息的ssid和passsword
				if(0xff!=findpos((uint8_t*)"get",3,USART3_task_data.pstr,USART3_task_data.len))
				{
					wificonfig_flag=1;
				}
				else if(0xff!=findpos((uint8_t*)"wifi",4,USART3_task_data.pstr,USART3_task_data.len))
				{
					wificonfig_flag=1;
				}
				else if(0xff!=findpos((uint8_t*)"info",4,USART3_task_data.pstr,USART3_task_data.len))
				{
					wificonfig_flag=1;
				}

				#if ESP8266_PRINT_RECEIVE//测试串口3接收到的数据
				buffer=pvPortMalloc(USART3_task_data.len+1);
				memcpy(buffer,USART3_task_data.pstr,USART3_task_data.len);
				buffer[USART3_task_data.len]=0;
				printf("smartU3RD:%2d:%s\r\n",USART3_task_data.len,buffer);
				vPortFree(buffer);
				#endif

				vPortFree(USART3_task_data.pstr);
				if(wificonfig_flag==1)//检测到get wifi info 后开启10s定时器，并且关闭60s定时器
				{
					err = xTimerStop(OneShot_config60_Handle,100);
					configASSERT(err);
					err = xTimerStart(OneShot_config10_Handle,100);
					configASSERT(err);
					free_usart3_rn_other_queue();
				}
			}

		}

		//检测wifi sta 是否变成2，变成2，结束配置，否则10s定时器会关闭wifi配置模式
		else if(wificonfig_flag==1)
		{
			if(check_consta()==2)//连接wifi成功
			{
					err = xTimerStop(OneShot_config10_Handle,100);
					configASSERT(err);
					printf("connect to wifi\r\n");
					free_usart3_rn_other_queue();

					send_cmd_wait_ack("AT+CWAUTOCONN=1","OK",WIFI_TRYTIMES,1);//设置自动连接
					goto stop_smartconfig;
			}
		}

			delay_ms(20);//给其他任务时间
	}
		stop_smartconfig://结束wifi配置模式 写成函数比较好
		stop_config_task(wificonfig_flag);
	}
}

void connect_to_server(void *pvParameters)
{
	uint8_t temp;
	printf("start connect to server\r\n");
	while(1)
	{
		if(xQueueReceive(connect_to_server_Queue,&temp,portMAX_DELAY))//接收到服务器数据
		{
			printf("start connect to server\r\n");
			if(temp)//是否需要断开当前连接
			{
				disconnect_TCP();
			}
			connect_to_server_process();
		}
			vTaskSuspend(connect_to_server_Handler);
	}
}

uint8_t connect_to_server_process(void )
{
		char *t;
		BaseType_t err;
		uint16_t temp;
		free_usart3_rn_other_queue();
		free_usart3_server_queue();
		vTaskSuspend(USART3_app_Handler);
		t=pvPortMalloc(64);
		sprintf(t,"AT+CIPSTART=\"TCP\",\"%s\",%s",(uint8_t*)IP_buf,(uint8_t*)portnum);
		send_cmd_wait_ack(t,"OK",WIFI_TRYTIMES*3,1);//连接到服务器
		delay_ms(100);
		vPortFree(t);
		free_usart3_rn_other_queue();
	if(check_consta()==3)
	{
		free_usart3_rn_other_queue();
		delay_ms(50);
		printf("connetced to server\r\n");
		pkt_serial = 1;
		vTaskResume(USART3_app_Handler);
		temp = 0x0001;
		err=xQueueSend(pack2server_queue,&temp,10);
		if(err==errQUEUE_FULL)   	//发送消息帧
		{
			printf("队列pack2server_Queue已满，0001数据发送失败!\r\n");
		}
		return 1;
	}
	return 0;
}


//断开连接
void disconnect_TCP(void)
{
	free_usart3_rn_other_queue();
	send_cmd_wait_ack("AT+CIPCLOSE","OK",WIFI_TRYTIMES,1);	//断开原TCP连接
}
//返回0，没有收到期待应答
//返回1，收到期待应答
//需要\r\n长度小于缓冲buff的长度
//这种模式的问题：1上一次应答的残留：执行完成一条指令就free_usart3_rn_other_queue，2只能对比一个目标应答：多个对比时单独写代码进行对比
uint8_t check_cmd(char*str)
{
	usart_task_data USART3_task_data;//USART3task分帧数据
	uint8_t ret = 0xff;
	#if ESP8266_PRINT_RECEIVE//测试串口3接收到的数据
	uint8_t* buffer;
	#endif
	{
		if(xQueueReceive(USART3_Task_rn_other_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//接收到分帧串口数据
		{
			{
				ret = findpos((uint8_t*)str,strlen(str),USART3_task_data.pstr,USART3_task_data.len);
				#if ESP8266_PRINT_RECEIVE//测试串口3接收到的数据
				buffer=pvPortMalloc(USART3_task_data.len+1);
				memcpy(buffer,USART3_task_data.pstr,USART3_task_data.len);
				buffer[USART3_task_data.len]=0;
				printf("ret:%d\tcmd:%2d:%s\tU3RD:%2d:%s\r\n",ret,strlen(str),str,USART3_task_data.len,buffer);
				vPortFree(buffer);
				#endif
				vPortFree(USART3_task_data.pstr);
			}
		}
	}
	return !(ret==0xff);
}

//1 收到期待应答。 0指令发送后没有期待应答
//延时waittime*10ms 任务切换的延时
//rn标记是否在命令结尾加上\R\N
void send_cmd(char*cmd,uint8_t rn)
{
	usart_task_data USART3_send_data;//USART3send数据
	uint8_t*p;
	BaseType_t err;
	if(rn)
		USART3_send_data.type = USART_RN;
	else
		USART3_send_data.type = USART_DIRECT;
	USART3_send_data.len  = strlen(cmd);
	p = pvPortMalloc(USART3_send_data.len);
	memcpy(p,cmd,USART3_send_data.len);
	USART3_send_data.pstr = p;
	err=xQueueSend(USART3_Send_Queue,&USART3_send_data,10);
	if(err==errQUEUE_FULL)   	//发送消息帧
	{
		vPortFree(USART3_send_data.pstr);
		printf("队列USART3_Send_Queue已满，指令发送RN失败!\r\n");
	}
}

uint8_t send_cmd_wait_ack(char *cmd,char *ack,uint16_t waittime,uint8_t rn)
{
	uint8_t ret = 0;
	send_cmd(cmd,rn);
	while(--waittime)
	{
		if(check_cmd(ack))
		{
			ret = 1;
			break;
		}
		vTaskDelay(10);//任务切换
	}
	if(!waittime) ret = 0;
	return ret;
}

//1 退出成功，0退出失败
uint8_t quit_trans(void)
{
	uint8_t i;
	uart3->uart_write("+",1);
	delay_ms(15);					//大于串口组帧时间(10ms)
	uart3->uart_write("+",1);
	delay_ms(15);					//大于串口组帧时间(10ms)
	uart3->uart_write("+",1);
	delay_ms(100);					//等待500ms
	for(i=0;i<6;i++)
	{
		if(send_cmd_wait_ack("+","+",3,0))
			return 1;
	}

	return send_cmd_wait_ack("+","+",10,0);
}

//检查wifi连接状态 提前确保退出透传了
//返回值:0,1,		2,获得IP		3，建立连接 	4，失去连接		5，未连接到WiFi 7,没有标准回执 ，8，没有进行操作
uint8_t check_consta(void)
{
	uint8_t ret = 8,i;
	usart_task_data USART3_task_data;//USART3task分帧数据
	#if ESP8266_PRINT_RECEIVE//测试串口3接收到的数据
	uint8_t* buffer;
	#endif
	free_usart3_rn_other_queue();
	send_cmd("AT+CIPSTATUS",1);
	for(i=0;i<30;i++)//接收10次，确保把全部数据接收完
	{
		delay_ms(60);//需要给其他任务进行接收处理的时间
		if(xQueueReceive(USART3_Task_rn_other_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//接收到分帧串口数据
			{
				#if ESP8266_PRINT_RECEIVE//测试串口3接收到的数据
				buffer=pvPortMalloc(USART3_task_data.len+1);
				memcpy(buffer,USART3_task_data.pstr,USART3_task_data.len);
				buffer[USART3_task_data.len]=0;
				printf("staU3RD:%2d:%s\r\n",USART3_task_data.len,buffer);
				vPortFree(buffer);
				#endif
				if(0xff!=findpos((uint8_t*)"STATUS:0",8,USART3_task_data.pstr,USART3_task_data.len))
				{
					vPortFree(USART3_task_data.pstr);
					return 0;
				}
				else if(0xff!=findpos((uint8_t*)"STATUS:1",8,USART3_task_data.pstr,USART3_task_data.len))
				{
					vPortFree(USART3_task_data.pstr);
					return 1;
				}
				else if(0xff!=findpos((uint8_t*)"STATUS:2",8,USART3_task_data.pstr,USART3_task_data.len))
				{
					vPortFree(USART3_task_data.pstr);
					return 2;
				}
				else if(0xff!=findpos((uint8_t*)"STATUS:3",8,USART3_task_data.pstr,USART3_task_data.len))
				{
					vPortFree(USART3_task_data.pstr);
					return 3;
				}
				else if(0xff!=findpos((uint8_t*)"STATUS:4",8,USART3_task_data.pstr,USART3_task_data.len))
				{
					vPortFree(USART3_task_data.pstr);
					return 4;
				}
				else if(0xff!=findpos((uint8_t*)"STATUS:5",8,USART3_task_data.pstr,USART3_task_data.len))
				{
					vPortFree(USART3_task_data.pstr);
					return 5;
				}
				else
				{
					vPortFree(USART3_task_data.pstr);
					ret = 7;//没有状态
				}
			}
		}
			return ret;
}
//任务相关函数
//把串口3 的 \r\n数据接收完，free掉内存，以便重新开始接收命令
void free_usart3_rn_other_queue(void)
{
	usart_task_data USART3_task_data;//USART3task分帧数据

	#if ESP8266_PRINT_RECEIVE//测试串口3接收到的数据
	uint8_t* buffer;
	#endif

	while(uxQueueMessagesWaiting(USART3_Task_rn_other_Queue))//清空接收队列
	{
		if(xQueueReceive(USART3_Task_rn_other_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//接收到分帧串口数据
		{
				#if ESP8266_PRINT_RECEIVE//测试串口3接收到的数据
				buffer=pvPortMalloc(USART3_task_data.len+1);
				memcpy(buffer,USART3_task_data.pstr,USART3_task_data.len);
				buffer[USART3_task_data.len]=0;
				printf("freeU3RD:%2d:%s\r\n",USART3_task_data.len,buffer);
				vPortFree(buffer);
				#endif
			vPortFree(USART3_task_data.pstr);
		}
		delay_ms(10);
	}
}

//free掉服务器数据
void free_usart3_server_queue(void)
{
	usart_task_data USART3_task_data;//USART3task分帧数据

		while(uxQueueMessagesWaiting( USART3_Task_server_Queue))//清空接收队列
	{
		if(xQueueReceive( USART3_Task_server_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//接收到分帧串口数据
		{
			vPortFree(USART3_task_data.pstr);
		}
	}
}

//两个定时器回调函数  定时器关闭的配置模式都是配置失败
void OneShot60_10Callback(TimerHandle_t xTimer)		//单次定时器回调函数
{
	uint32_t timer_ID;
	configASSERT(xTimer);
	timer_ID = (uint32_t) pvTimerGetTimerID(xTimer);

	//处理定时器任务
	switch(timer_ID)
	{
		case WIFI_TIM60S_ID:
			printf("60s failed\r\n");
			stop_config_task(0);
			break;
		case WIFI_TIM10S_ID:
			printf("10s failed\r\n");
			stop_config_task(0);
			break;
		default:configASSERT(0);
			break;
	}
}

void start_config_task(void)
{
		set_rtc_sec(0);
		vTaskSuspend(USART1_Handler);
		vTaskSuspend(USART1_app_Handler);
		vTaskSuspend(USART3_app_Handler);

		touch_flag&=(~(0x0001<<4));//标记下线
		touch_flag&=(~(0x0001<<14));
		touch_flag&=(~(0x0001<<7));//标记为关灯状态
		touch_flag&=0xfff0;//记录灯光档位为0档

		xTimerStop(period40or5_0002_Handle,10);
		free_usart3_server_queue();
		xTaskCreate((TaskFunction_t )smartconfig_task,
		(const char*    )"smartconfig_task",
		(uint16_t       )SMARTCONFIG_TASK_SIZE,
		(void*          )NULL,
		(UBaseType_t    )SMARTCONFIG_TASK_PRIO,
		(TaskHandle_t*  )&smart_config_Handler);

}

void stop_config_task(uint8_t ifsuccess)
{
		uint8_t temp;
		BaseType_t err;
		printf("stop config\r\n");
		set_rtc_sec(0);
		pkt_serial = 0;//包的个数计数

		touch_flag&=(~(0x0001<<5));//停止wifiset
		mcu_sendcmd2touch(Wifi_Quit);
		touch_process_poweron();
		delay_ms(50);//让模块回执完成，不起作用。。
		free_usart3_rn_other_queue();//还需要清空其他中断接收到的数据

		vTaskResume(USART3_app_Handler);
		vTaskResume(USART1_app_Handler);
		vTaskResume(USART1_Handler);
	//启动触控板
	//连接成功，上线操作
	if(ifsuccess)
	{
		printf("stop config 1\r\n");
		vTaskResume(connect_to_server_Handler);
				temp = 0;
				err=xQueueSend(connect_to_server_Queue,&temp,10);
				if(err==errQUEUE_FULL)   	//发送消息帧
				{

					printf("队列connect_to_server_Queue已满，0数据发送失败!\r\n");
				}
	}
	xTimerChangePeriod(period40or5_0002_Handle,PKG0002_5S_TICKS,10);
	xTimerStart(period40or5_0002_Handle,10);
	vTaskDelete(smart_config_Handler);//delete
}


