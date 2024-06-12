/*
 * ESP8266.c
 *
 *  Created on: 2018��5��3��
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
#define ESP8266_PRINT_RECEIVE 0//�Ƿ��ӡ���յ�������
#define WIFI_TRYTIMES					10//���Զ�ȡwifiģ���ִ��Ϣ����

TaskHandle_t smart_config_Handler;
TaskHandle_t connect_to_server_Handler;

TimerHandle_t	OneShot_config60_Handle;			//���ζ�ʱ��60s���
TimerHandle_t	OneShot_config10_Handle;			//���ζ�ʱ��10s���
TimerHandle_t	OneShot_config1MS_Handle;//1ms��ʱ�����

QueueHandle_t connect_to_server_Queue;

char IP_buf[16]="121.42.180.30";//"";
char portnum[11]="8181";							//���Զ˿�"11192";	//uint8_t* portnum="8086"; 9124 ���Ĭ�϶˿�Ӧ����9123 ֱ����9124�Ժ��ǲ��е�
//const char* IP_tbl[12]={"1","2","3","4","5","6","7","8","9",".","0"};

//8266 smart config
//�ȹ�����˴���3��������񣬴���smartconfig����
//��ⴥ�ذ�����ҲҪ�������У������˳�����ģʽ
void smartconfig_task(void *pvParameters)
{
	uint8_t time=100;
	BaseType_t err;
	uint8_t wificonfig_flag = 0;//���ã���ǽ��յ�����ʼ����wifi��  ͬʱ����Ƿ����óɹ�
//	uint8_t pos_temp;
	usart_task_data USART3_task_data;//USART3task��֡����
	#if ESP8266_PRINT_RECEIVE//���Դ���3���յ�������
	uint8_t* buffer;
	#endif
	//
	dprintf("smart_config start!\r\n");
//	vTaskSuspendAll();
//
//	vTaskResume(smart_config__Handler);
//	vTaskResume(FREE_ERRORMEM_Handler);
//	vTaskResume(USART3_Handler);
//	vTaskResume(USART3_send_Handler);
//	//���д��ذ������ ��������ģ�ⴥ�ذ�����
//	vTaskResume(Task1Task_Handler);
//	vTaskResume(Keyprocess_Handler);
//		start_config_task();




	//����Ӧ��Ҫ��λ�����Ǹ�λ��û�жԶ����е��ڴ����free�����Բ��ܼ򵥵ĸ�λ����Ҫ���������ݽ���������free�ڴ�
	//�ȰѴ���3���ݽ����꣬�ٿ�ʼ��������

	{
//		quit_trans();
//		if(send_cmd_wait_ack("AT","OK",10,1))
//		{
//			printf("8266 at ok\r\n");
//
//		}
		free_usart3_rn_other_queue();
		while((!send_cmd_wait_ack("AT","OK",WIFI_TRYTIMES,1))&&--time)//���WIFIģ���Ƿ�����
	{
		quit_trans();//�˳�͸��
		send_cmd_wait_ack("AT+CIPMODE=0","OK",WIFI_TRYTIMES,1);  //�ر�͸��ģʽ
		delay_ms(10);
	}
	if(time==0)
	{
		goto stop_smartconfig;
	}

	free_usart3_rn_other_queue();
	send_cmd_wait_ack("AT+CWMODE=1","OK",WIFI_TRYTIMES,1);//��Ϊstationģ

	time=3;
	free_usart3_rn_other_queue();
	while((!send_cmd_wait_ack("AT+CWSTOPSMART","OK",WIFI_TRYTIMES,1))&&--time);//���˳�smartconfig��

	time=10;
	free_usart3_rn_other_queue();
	while((!send_cmd_wait_ack("AT+CWSTARTSMART","OK",WIFI_TRYTIMES,1))&&--time);//��ⷢ���Ƿ�ɹ�
	if(time==0)
	{
		goto stop_smartconfig;
	}


	//����60s��ʱ����ʱ�䵽�Զ��˳�����ģʽ
	err = xTimerStart(OneShot_config60_Handle,100);
	configASSERT(err);
//	goto stop_smartconfig;//������
	while(1)
	{

		//�ȼ����û�� get wifi info ��־
		if(wificonfig_flag==0)
		{
			if(xQueueReceive(USART3_Task_rn_other_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//���յ���֡��������
			{//���ܳ���receive�˶��С�û�н��е�free �ʹ���60s�Ķ�ʱ�����ĵͶ�ʱ�����ȼ����߽�ֹ������ȣ�
				//������ʵ���Լ�⵽wifi����Ϣ��ssid��passsword
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

				#if ESP8266_PRINT_RECEIVE//���Դ���3���յ�������
				buffer=pvPortMalloc(USART3_task_data.len+1);
				memcpy(buffer,USART3_task_data.pstr,USART3_task_data.len);
				buffer[USART3_task_data.len]=0;
				dprintf("smartU3RD:%2d:%s\r\n",USART3_task_data.len,buffer);
				vPortFree(buffer);
				#endif

				vPortFree(USART3_task_data.pstr);
				if(wificonfig_flag==1)//��⵽get wifi info ����10s��ʱ�������ҹر�60s��ʱ��
				{
					err = xTimerStop(OneShot_config60_Handle,100);
					configASSERT(err);
					err = xTimerStart(OneShot_config10_Handle,100);
					configASSERT(err);
					free_usart3_rn_other_queue();
				}
			}

		}

		//���wifi sta �Ƿ���2�����2���������ã�����10s��ʱ����ر�wifi����ģʽ
		else if(wificonfig_flag==1)
		{
			if(check_consta()==2)//����wifi�ɹ�
			{
					err = xTimerStop(OneShot_config10_Handle,100);
					configASSERT(err);
					dprintf("connect to wifi\r\n");
					free_usart3_rn_other_queue();

					send_cmd_wait_ack("AT+CWAUTOCONN=1","OK",WIFI_TRYTIMES,1);//�����Զ�����
					goto stop_smartconfig;
//					stop_config_task(1);
			}
		}

			delay_ms(20);//����������ʱ��
	}


		stop_smartconfig://����wifi����ģʽ д�ɺ����ȽϺ�
		stop_config_task(wificonfig_flag);

//	vTaskDelay(SMARTCONFIG_TASK_delay);
	}
}



void connect_to_server(void *pvParameters)
{
	uint8_t temp;
	dprintf("start connect to server\r\n");
	while(1)
	{
		if(xQueueReceive(connect_to_server_Queue,&temp,portMAX_DELAY))//���յ�����������
		{
			dprintf("start connect to server\r\n");
//			delay_ms(30);
			if(temp)//�Ƿ���Ҫ�Ͽ���ǰ����
			{
				disconnect_TCP();
//				delay_ms(30);
			}
			connect_to_server_process();


		}
//			printf(" end of connetced to server\r\n");
//		vTaskDelay(pack2server_delay);
//		vTaskDelete(connect_to_server_Handler);//delete
			vTaskSuspend(connect_to_server_Handler);
	}



}
//���ӷ����� ԭatk_8266_In_trans
uint8_t connect_to_server_process(void )//������� ���ݶ������ݾ����Ƿ���Ҫ�Ͽ�ԭ���� Ȼ��������ӷ�������
{
		char *t;
		BaseType_t err;
		uint16_t temp;
		free_usart3_rn_other_queue();
		free_usart3_server_queue();
		vTaskSuspend(USART3_app_Handler);

		t=pvPortMalloc(64);
		sprintf(t,"AT+CIPSTART=\"TCP\",\"%s\",%s",(uint8_t*)IP_buf,(uint8_t*)portnum);

		send_cmd_wait_ack(t,"OK",WIFI_TRYTIMES*3,1);//���ӵ�������
		delay_ms(100);
		vPortFree(t);
		free_usart3_rn_other_queue();
	if(check_consta()==3)//û�л�ִ������?
	{
	    touch_flag|=(0x0001<<14);//�������
		free_usart3_rn_other_queue();
		delay_ms(50);
		printf("connetced to server\r\n");
		pkt_serial = 1;
		vTaskResume(USART3_app_Handler);

		temp = 0x0000;
		err=xQueueSend(pack2server_queue,&temp,10);

		temp = 0x0001;
		err=xQueueSend(pack2server_queue,&temp,10);
		if(err==errQUEUE_FULL)   	//������Ϣ֡
		{

			printf("����pack2server_Queue������0001���ݷ���ʧ��!\r\n");
		}
//		wifi2server(0x0001);
//		vTaskResume(USART3_app_Handler);
		return 1;
	}

	return 0;
}


//�Ͽ�����
void disconnect_TCP(void)
{
	free_usart3_rn_other_queue();
	send_cmd_wait_ack("AT+CIPCLOSE","OK",WIFI_TRYTIMES,1);	//�Ͽ�ԭTCP����
}

//����0��û���յ��ڴ�Ӧ��
//����1���յ��ڴ�Ӧ��
//��Ҫ\r\n����С�ڻ���buff�ĳ���
//����ģʽ�����⣺1��һ��Ӧ��Ĳ�����ִ�����һ��ָ���free_usart3_rn_other_queue��2ֻ�ܶԱ�һ��Ŀ��Ӧ�𣺶���Ա�ʱ����д������жԱ�
uint8_t check_cmd(char*str)
{

	usart_task_data USART3_task_data;//USART3task��֡����
	uint8_t ret = 0xff;
	#if ESP8266_PRINT_RECEIVE//���Դ���3���յ�������
	uint8_t* buffer;
	#endif

	//��⴮��3���Ͷ����Ƿ�Ϊ�գ��Ž��в���
//	if(!uxQueueMessagesWaiting(USART3_Send_Queue))
	{
		if(xQueueReceive(USART3_Task_rn_other_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//���յ���֡��������
		{
//			if(USART3_task_data.type==USART_RN)
			{

				ret = findpos((uint8_t*)str,strlen(str),USART3_task_data.pstr,USART3_task_data.len);
				#if ESP8266_PRINT_RECEIVE//���Դ���3���յ�������
				buffer=pvPortMalloc(USART3_task_data.len+1);
				memcpy(buffer,USART3_task_data.pstr,USART3_task_data.len);
				buffer[USART3_task_data.len]=0;
				dprintf("ret:%d\tcmd:%2d:%s\tU3RD:%2d:%s\r\n",ret,strlen(str),str,USART3_task_data.len,buffer);
				vPortFree(buffer);
				#endif
				vPortFree(USART3_task_data.pstr);
			}

		}
	}
	return !(ret==0xff);
}

//1 �յ��ڴ�Ӧ�� 0ָ��ͺ�û���ڴ�Ӧ��
//��ʱwaittime*10ms �����л�����ʱ
//rn����Ƿ��������β����\R\N

void send_cmd(char*cmd,uint8_t rn)
{
	usart_task_data USART3_send_data;//USART3send����
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
	if(err==errQUEUE_FULL)   	//������Ϣ֡
	{
		//����ʧ����Ҫfree�ڴ�
		vPortFree(USART3_send_data.pstr);
		dprintf("����USART3_Send_Queue������ָ���RNʧ��!\r\n");
	}
}

uint8_t send_cmd_wait_ack(char *cmd,char *ack,uint16_t waittime,uint8_t rn)
{
	uint8_t ret = 0;

	send_cmd(cmd,rn);

	//�ȴ����Ͷ���Ϊ��
//	while(uxQueueMessagesWaiting(USART3_Send_Queue))
//	{
//			vTaskDelay(10);//�����л�
//
//	}

	while(--waittime)
	{
		if(check_cmd(ack))
		{
			ret = 1;
			break;
		}
		vTaskDelay(10);//�����л�
	}
	if(!waittime) ret = 0;
	return ret;
}



//1 �˳��ɹ���0�˳�ʧ��
uint8_t quit_trans(void)
{
		uint8_t i;


	uart3->uart_write("+",1);
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)

	uart3->uart_write("+",1);
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)

	uart3->uart_write("+",1);
	delay_ms(100);					//�ȴ�500ms

	for(i=0;i<6;i++)
	{
		if(send_cmd_wait_ack("+","+",3,0))
			return 1;
	}

	return send_cmd_wait_ack("+","+",10,0);
}


//���wifi����״̬ ��ǰȷ���˳�͸����
//����ֵ:0,1,		2,���IP		3���������� 	4��ʧȥ����		5��δ���ӵ�WiFi 7,û�б�׼��ִ ��8��û�н��в���
uint8_t check_consta(void)
{
	uint8_t ret = 8,i;
	usart_task_data USART3_task_data;//USART3task��֡����
	#if ESP8266_PRINT_RECEIVE//���Դ���3���յ�������
	uint8_t* buffer;
	#endif

	free_usart3_rn_other_queue();
//	if(!quit_trans()) return 7;//�˳�͸��ʧ��

	send_cmd("AT+CIPSTATUS",1);

//	delay_ms(60);
	for(i=0;i<30;i++)//����10�Σ�ȷ����ȫ�����ݽ�����
	{

		delay_ms(60);//��Ҫ������������н��մ����ʱ��
		if(xQueueReceive(USART3_Task_rn_other_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//���յ���֡��������
			{
				#if ESP8266_PRINT_RECEIVE//���Դ���3���յ�������
				buffer=pvPortMalloc(USART3_task_data.len+1);
				memcpy(buffer,USART3_task_data.pstr,USART3_task_data.len);
				buffer[USART3_task_data.len]=0;
				dprintf("staU3RD:%2d:%s\r\n",USART3_task_data.len,buffer);
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
					ret = 7;//û��״̬

				}



			}
		}

			return ret;
}
//������غ���
//�Ѵ���3 �� \r\n���ݽ����꣬free���ڴ棬�Ա����¿�ʼ��������
void free_usart3_rn_other_queue(void)
{
	usart_task_data USART3_task_data;//USART3task��֡����

	#if ESP8266_PRINT_RECEIVE//���Դ���3���յ�������
	uint8_t* buffer;
	#endif

//	while(uxQueueMessagesWaiting(USART3_Send_Queue))//�ȴ����Ͷ���Ϊ��
//	{
//	delay_ms(10);
//	}

	while(uxQueueMessagesWaiting(USART3_Task_rn_other_Queue))//��ս��ն���
	{
		if(xQueueReceive(USART3_Task_rn_other_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//���յ���֡��������
		{
				#if ESP8266_PRINT_RECEIVE//���Դ���3���յ�������
				buffer=pvPortMalloc(USART3_task_data.len+1);
				memcpy(buffer,USART3_task_data.pstr,USART3_task_data.len);
				buffer[USART3_task_data.len]=0;
				dprintf("freeU3RD:%2d:%s\r\n",USART3_task_data.len,buffer);
				vPortFree(buffer);
				#endif
			vPortFree(USART3_task_data.pstr);
//			printf("freeU3RD:success\r\n");
		}
		delay_ms(10);
	}

}

//free������������
void free_usart3_server_queue(void)
{
	usart_task_data USART3_task_data;//USART3task��֡����

		while(uxQueueMessagesWaiting( USART3_Task_server_Queue))//��ս��ն���
	{
		if(xQueueReceive( USART3_Task_server_Queue,&USART3_task_data,0/*portMAX_DELAY*/))//���յ���֡��������
		{
			vPortFree(USART3_task_data.pstr);
		}
	}
}


//������ʱ���ص�����  ��ʱ���رյ�����ģʽ��������ʧ��
void OneShot60_10Callback(TimerHandle_t xTimer)		//���ζ�ʱ���ص�����
{
	uint32_t timer_ID;
	configASSERT(xTimer);

	timer_ID = (uint32_t) pvTimerGetTimerID(xTimer);

	//����ʱ������

	switch(timer_ID)
	{
		case WIFI_TIM60S_ID:
			dprintf("60s failed\r\n");
			stop_config_task(0);
			break;
		case WIFI_TIM10S_ID:
			dprintf("10s failed\r\n");
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

		touch_flag&=(~(0x0001<<4));//�������
		touch_flag&=(~(0x0001<<14));
		touch_flag&=(~(0x0001<<7));//���Ϊ�ص�״̬
		touch_flag&=0xfff0;//��¼�ƹ⵵λΪ0��

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
		dprintf("stop config\r\n");
		set_rtc_sec(0);
		pkt_serial = 0;//���ĸ�������

		touch_flag&=(~(0x0001<<5));//ֹͣwifiset
		mcu_sendcmd2touch(Wifi_Quit);
//		delay_ms(100);
//		mcu_sendcmd2touch(Power_On);
		touch_process_poweron();

		delay_ms(50);//��ģ���ִ��ɣ��������á���
		free_usart3_rn_other_queue();//����Ҫ��������жϽ��յ�������

		vTaskResume(USART3_app_Handler);
		vTaskResume(USART1_app_Handler);
		vTaskResume(USART1_Handler);



	//�������ذ�


	//���ӳɹ������߲���
	if(ifsuccess)
	{
		dprintf("stop config 1\r\n");
		vTaskResume(connect_to_server_Handler);

//		connect_to_server();
				temp = 0;
				err=xQueueSend(connect_to_server_Queue,&temp,10);
				if(err==errQUEUE_FULL)   	//������Ϣ֡
				{

					dprintf("����connect_to_server_Queue������0���ݷ���ʧ��!\r\n");
				}
	}
	xTimerChangePeriod(period40or5_0002_Handle,PKG0002_5S_TICKS,10);
	xTimerStart(period40or5_0002_Handle,10);
	vTaskDelete(smart_config_Handler);//delete
}


