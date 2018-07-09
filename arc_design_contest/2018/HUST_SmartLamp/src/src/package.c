/*
 * package.c
 *
 *  Created on: 2018��5��3��
 *      Author: Administrator
 */


#include "package.h"
//
#include "string.h"
#include "stdlib.h"

#include "malloc.h"
#include "timer.h"
#include "crc32.h"
#include "ESP8266.h"
#include "usart3task.h"
#include "usart4task.h"
#include "strpos.h"
#include "initsys.h"

#include "task_config.h"
#define delay_ms vTaskDelay
//�������ͨ�ţ��Լ����ݺ�����Ĵ���
//���յ�wifiģ�����ݴ���
//������������͸�wifiģ��

//������
QueueHandle_t pack2server_queue;
TaskHandle_t USART3_app_Handler;
TaskHandle_t pack2server_Handler;

TimerHandle_t	period40or5_0002_Handle;
uint16_t tx_package_len[26] = {12,56,34,34,66,56,00,35,00,00,00,00,00,00,00,00,48,49,00,00,00,00,00,00,00,00};//ʮ����

uint16_t pkt_serial=1;	//���Ͱ�����������
uint8_t time_serial;

//����3 APP�㴦������
void usart3_app_task(void *pvParameters)
{
	usart_task_data usart3_app_temp;//����USART3_task �����ķ�������֡����

	uint8_t*buffer;
	uint8_t len,i;

	EMBARC_PRINTF("usart3_app_task start!\r\n");
//    vTaskResume(USART3_Handler);
	while(1)
	{

	if(xQueueReceive(USART3_Task_server_Queue,&usart3_app_temp,0))//���յ�����������portMAX_DELAY
	{
		#if 1//���Խ��շ����������Ƿ���ȷ
		EMBARC_PRINTF("re server dat %4x:",(usart3_app_temp.pstr[20]<<8)+usart3_app_temp.pstr[21]);
		len = (usart3_app_temp.pstr[4]<<8)+usart3_app_temp.pstr[5];
		for(i=0;i<len;i++)
		{
			EMBARC_PRINTF(" %2x",usart3_app_temp.pstr[i]);
		}
		EMBARC_PRINTF("\r\n");
		#endif

		//��������
		serverdata_process(usart3_app_temp);

		vPortFree(usart3_app_temp.pstr);
	}
//��Ҫ������Ч��\R\N��other���ݣ������ڴ�һֱ��ռ�ã����в���Ϊ��

		if(xQueueReceive(USART3_Task_rn_other_Queue,&usart3_app_temp,0))//���յ�����������portMAX_DELAY
	{
		#if 1//ʹ�ô���3���Ѱ���ģ��ɴ���ʹ��
		buffer=pvPortMalloc(usart3_app_temp.len+1);
    memcpy(buffer,usart3_app_temp.pstr,usart3_app_temp.len);
		buffer[usart3_app_temp.len]=0;
		printf("U3:%2d:%s\r\n",usart3_app_temp.len,buffer);//����ʹ��printf ���ǵ���
//		EMBARC_PRINTF("u3app RNdat\r\n");
		vPortFree(buffer);
		#endif
		if(0xff!=findpos((uint8_t*)"CLOSED",6,usart3_app_temp.pstr,usart3_app_temp.len))
		{
			touch_flag&=(~(0x0001<<4));//�������
			touch_flag&=(~(0x0001<<14));
			if(touch_flag&(0x0001<<7))//����״̬
			{
					mcu_sendcmd2touch(Wifi_Off);
			}
			xTimerChangePeriod(period40or5_0002_Handle,PKG0002_5S_TICKS,10);//�ı䶨ʱ������
			//
//			xTimerStop(period40or5_0002_Handle,10);
			EMBARC_PRINTF("disconnect to server\r\n");
		}
		//�������
		else if(0xff!=findpos((uint8_t*)"SEND OK",7,usart3_app_temp.pstr,usart3_app_temp.len))
		{
			//�ͷ��ź���
			//һ����һ��û���յ���ִ���ͻ���Ҳ������ ��ʱ����ʱʹ��
			xTimerStop(OneShot_CanServer_Handle,10);
			xSemaphoreGive(SendenableMutexSemaphore);					//�ͷ��ź���
//			u3cansend = 1;
		}
		//ÿ�η���������ݺ󣬻᷵��SEND OK ���û�У���˵��������

		vPortFree(usart3_app_temp.pstr);
	}

	vTaskDelay(USART3_APP_delay);
	}
}

void serverdata_process(usart_task_data usart3_app_temp)
{
	uint16_t cmd=0;
	uint8_t* p;
	BaseType_t err;
	uint16_t temp;

	p=usart3_app_temp.pstr;
	cmd |= (p[20])<<8;
	cmd |= p[21];
	EMBARC_PRINTF("re cmd:%4x\r\n",cmd);
	switch(cmd)
	{
		case 0x8003:
		{
			sendpack2server(0x0003);
			server_control_light(p[22]);
		}
			break;

		case 0x8008:
		{
			set_rtc_sec(p[26]+(p[25]<<8)+(p[24]<<16)+(p[23]<<24));
			time_serial = p[22]+1;
			wifi2server(0x0008);//����û���ö��������������Ͱ�ָ��
			if(time_serial==6)
			{
					touch_flag|=(0x0001<<14);//�������
					touch_flag|=(0x0001<<4);
					if(touch_flag&(0x0001<<7))//����״̬
					{
						mcu_sendcmd2touch(Wifi_On);
					}
					xTimerStop(period40or5_0002_Handle,10);
					delay_ms(300);//wifiģ����Ҫһ�㷴Ӧʱ��
					sendpack2server(0x0002);
					xTimerChangePeriod(period40or5_0002_Handle,PKG0002_40S_TICKS,10);
					xTimerStart(period40or5_0002_Handle,10);
			}
		}
			break;

		case 0x8009:
		{
			disconnect_TCP();
			touch_flag&=(~(0x0001<<4));//�������
			touch_flag&=(~(0x0001<<14));
			if(touch_flag&(0x0001<<7))//����״̬
			{
				mcu_sendcmd2touch(Wifi_Off);
			}
			xTimerStop(OneShot_CanServer_Handle,10);
			xTimerChangePeriod(period40or5_0002_Handle,PKG0002_5S_TICKS,10);//�ı䶨ʱ������
			xTimerStart(period40or5_0002_Handle,10);
			dprintf("8009 disconnect to server\r\n");
		}
		break;

		case 0x800A:
		{
			uint32_t IP_P=0;
			sprintf((char*)IP_buf,"%d.%d.%d.%d",p[22],p[23],p[24],p[25]);
			IP_P=p[29]+(p[28]<<8)+(p[27]<<16)+(p[26]<<24);
			sprintf((char*)portnum,"%d",IP_P);
//			EMBARC_PRINTF("IP:%s:%s\r\n",IP_buf,portnum);
				temp = 1;
				err=xQueueSend(connect_to_server_Queue,&temp,10);
				if(err==errQUEUE_FULL)   	//������Ϣ֡
				{

					EMBARC_PRINTF("����connect_to_server_Queue������1���ݷ���ʧ��!\r\n");
				}
				else
				{
					vTaskResume(connect_to_server_Handler);
				}
//			disconnect_TCP();
//			delay_ms(30);
//			connect_to_server();
		}
			break;

		default:
			break;
	}
}


//���͸��������İ���������֡Ȼ���͸����ͺ���
void pack2server(void *pvParameters)
{
	uint16_t packtype;
	while(1)
	{
		if(xQueueReceive(pack2server_queue,&packtype,0))//���յ�����������portMAX_DELAY
		{
			wifi2server(packtype);
		}

		vTaskDelay(pack2server_delay);
	}
}

//���÷������ݰ� �ⲿ����
void sendpack2server(uint16_t package_num)
{
	uint16_t temp = package_num;
	BaseType_t err;
	err=xQueueSend(pack2server_queue,&temp,0);
	if(err==errQUEUE_FULL)   	//������Ϣ֡
	{

	dprintf("����pack2server_Queue������%4x���ݷ���ʧ��!\r\n",temp);
	}
}

//wifi������������֡����֡����
//����Э�������֡������Ȼ���͸�����3�ķ��Ͷ���
void wifi2server(uint16_t data_type)
{
	uint8_t* p;
	uint16_t len=0;
	uint8_t if_send=1,i;
	uint32_t crc32;

	usart_task_data USART3_send_data;//USART3send����
	BaseType_t err;

	EMBARC_PRINTF("wifi2server\r\n");
	if(data_type)
	len  = tx_package_len[(data_type&0xff)-1];

	if(len>24)
	{
	#if 1
		p = pvPortMalloc(len);
		configASSERT(p);
		memset(p,0,len);

		memcpy(p,server_start,4);
		p[4] = (len>>8)&0xff;
		p[5] = len&0xff;
		p[6] = (sys_config.pkt_version>>24)&0xff;
		p[7] = (sys_config.pkt_version>>16)&0xff;
		p[8] = (sys_config.pkt_version>>8)&0xff;
		p[9] = sys_config.pkt_version&0xff;
		memcpy(p+10,DEVICE_ID,8);
		p[18]=(pkt_serial>>8)&0xff;
		p[19]=pkt_serial&0xff;
		p[20]=(data_type>>8)&0xff;
		p[21]=data_type&0xff;

		memcpy(p+len-4,server_stop,4);
		#endif
		switch(data_type)
		{
			case 0x0001:
				break;
			case 0x0002:
			{
				uint16_t bright_temp;
				if(!(touch_flag&(0x0001<<14))) if_send =0;
				crc32 = get_rtc_sec();//�ݴ浱ǰϵͳʵ��
				for(i=0;i<4;i++)//ʱ��
				{
					p[22+i] = (crc32>>(8*(3-i)))&0xff;
				}
				//light�ƹ�ģʽ
				if(touch_flag&(0x0001<<7))//����״̬
				{
					if(touch_flag&(0x0001<<8))//�Զ�
					{
						p[26] = 0x10; p[27] = 1+(touch_flag&0x000f);
					}
					else if(touch_flag&(0x0001<<9))
					{
						p[27] = 1+(touch_flag&0x000f);
						p[26] = 0x30|(p[27]-1);
					}
					else//�ֶ�
					{
						p[27] = 1+(touch_flag&0x000f);
						p[26] = 0x20|(p[27]-1);
					}
				}
				else
				{
					p[26] = 0;
					p[27] = 0;
				}
				bright_temp = CM3232_Read(0x50);
				p[28] = bright_temp>>8;
				p[29]= bright_temp&0xff;
			}
				break;
			case 0x0003:
				break;
			case 0x0008:
			{
				p[22] = time_serial;//ʱ�����1 3 5
				crc32 = get_rtc_sec();//�ݴ浱ǰϵͳʵ��
				for(i=0;i<4;i++)//ʱ��
				{
					p[23+i] = (crc32>>(8*(3-i)))&0xff;
				}
			}
				break;
			default:if_send = 0;
				break;
		}

		if(if_send)
		{
			crc32 = Get_Crc32(p, len);
			for(i=0;i<4;i++)
			{
				p[len-8+i] = (crc32>>(8*(3-i)))&0xff;
			}

			USART3_send_data.type = USART_SERVER;
			USART3_send_data.len  = len;
			USART3_send_data.pstr = p;
			#if 0//���Է��������������Ƿ���ȷ
			EMBARC_PRINTF("dat2server %4x:",data_type);
			for(i=0;i<len;i++)
			{
				EMBARC_PRINTF(" %2x",p[i]);
			}
			EMBARC_PRINTF("\r\n");
			#endif
			err=xQueueSend(USART3_Send_Queue,&USART3_send_data,10);
			if(err==errQUEUE_FULL)   	//������Ϣ֡
			{
				//����ʧ����Ҫfree�ڴ�
				vPortFree(USART3_send_data.pstr);
				pkt_serial--;
				EMBARC_PRINTF("����USART3_Send_Queue������server0001����ʧ��!\r\n");
			}
			else
			{
				if(data_type==0x0002) xTimerReset(period40or5_0002_Handle,10);
				pkt_serial++;
			}
		}
		else
		{
			vPortFree(p);
		}
	}
}


//��ʱ���Ͱ�2 Ĭ��40s����һ��
void period40or5_0002_Callback(TimerHandle_t xTimer)		//
{
				BaseType_t err;
				if(touch_flag&(0x0001<<14))//�Ѿ�����
				{
					uint16_t temp;
				temp = 0x0002;
				err=xQueueSend(pack2server_queue,&temp,0);//��ʱ�������У����ܽ�����ʱ����
				if(err==errQUEUE_FULL)   	//������Ϣ֡
				{
					EMBARC_PRINTF("����pack2server_Queue���������ڷ���0002���ݷ���ʧ��!\r\n");
				}
				}
				else//��������
				{
					uint8_t temp = 1;
					err=xQueueSend(connect_to_server_Queue,&temp,0);
					if(err==errQUEUE_FULL)   	//������Ϣ֡
					{

						dprintf("����connect_to_server_Queue������1���ݷ���ʧ��!\r\n");
					}
					vTaskResume(connect_to_server_Handler);
				}

}
//end package.c

