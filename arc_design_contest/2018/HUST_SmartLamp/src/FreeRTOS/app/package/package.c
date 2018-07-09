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
#include "cm3232.h"
#include "irf.h"
#include "cJSON.h"

#include "task_config.h"
#define delay_ms vTaskDelay
//�������ͨ�ţ��Լ����ݺ�����Ĵ���
//���յ�wifiģ�����ݴ���
//������������͸�wifiģ��

//������
QueueHandle_t pack2server_queue;
TaskHandle_t USART3_app_Handler;
TaskHandle_t pack2server_Handler;

TimerHandle_t	period40or5_0002_Handle;			//00 02�� ��ʱ����
//ÿ�����Ͱ��ĳ���					1  2  3  4  5  6	7	 8	9	 A	B	 C	D	 E	F 10 11 12 13 14 15 16 17 18 19 1a
//uint16_t tx_package_len[26] = {78,56,34,34,66,56,00,35,00,00,00,00,00,00,00,00,48,49,00,00,00,00,00,00,00,00};//ʮ����

uint16_t pkt_serial=1;	//���Ͱ�����������
uint8_t time_serial;//ʱ����� 8008��0008�Ĺ���

char *DEVICEID = "6333";
char *APIKEY = "6599b2865";
char update_did[6]="6333";//�豸ID test_arc
char update_id_rtc[6]="5644";//�ӿ�ID test02_01
char update_id_cm3232[]="5686";//���ݽӿ� ��ǿ
char update_id_touch[]="5687";//���ݽӿ� ��λ
char update_id_time[]="6098";
char update_id_flag[]="6100";
char say_did[30]="U4571";
char say_did_default[]="U4571";
char say_content[64]="hello";

//����3 APP�㴦������
void usart3_app_task(void *pvParameters)
{
	usart_task_data usart3_app_temp;//����USART3_task �����ķ�������֡����

	uint8_t*buffer;
	uint8_t len,i;
//	uint64_t irf_temp;

	EMBARC_PRINTF("usart3_app_task start!\r\n");
//    vTaskResume(USART3_Handler);
	while(1)
	{


	if(xQueueReceive(USART3_Task_server_Queue,&usart3_app_temp,0))//���յ�����������portMAX_DELAY
	{
		#if 1//���Խ��շ����������Ƿ���ȷ
			#if 0
        buffer=pvPortMalloc(usart3_app_temp.len+1);
        memcpy(buffer,usart3_app_temp.pstr,usart3_app_temp.len);
        buffer[usart3_app_temp.len]=0;
        dprintf("re server dat:%3d:%s\r\n",usart3_app_temp.len,buffer);//
        vPortFree(buffer);
			#endif
#else
		EMBARC_PRINTF("re server dat %4x:",(usart3_app_temp.pstr[20]<<8)+usart3_app_temp.pstr[21]);
		len = (usart3_app_temp.pstr[4]<<8)+usart3_app_temp.pstr[5];
		for(i=0;i<len;i++)
		{
			EMBARC_PRINTF(" %2x",usart3_app_temp.pstr[i]);
		}
		EMBARC_PRINTF("\r\n");
		#endif
		//1����crc32 device_id Э��汾�� ����� ��

		//��������
		serverdata_process(usart3_app_temp);

		vPortFree(usart3_app_temp.pstr);
	}
//��Ҫ������Ч��\R\N��other���ݣ������ڴ�һֱ��ռ�ã����в���Ϊ��

		if(xQueueReceive(USART3_Task_rn_other_Queue,&usart3_app_temp,0))//���յ�����������portMAX_DELAY
	{
		#if 0//ʹ�ô���3���Ѱ���ģ��ɴ���ʹ��
		buffer=pvPortMalloc(usart3_app_temp.len+1);
		memcpy(buffer,usart3_app_temp.pstr,usart3_app_temp.len);
		buffer[usart3_app_temp.len]=0;
		dprintf("U3re:%2d:%s\r\n",usart3_app_temp.len,buffer);//����ʹ��printf ���ǵ���
//		EMBARC_PRINTF("u3app RNdat\r\n");
		vPortFree(buffer);
		#endif
		if(0xff!=findpos((uint8_t*)"CLOSED",6,usart3_app_temp.pstr,usart3_app_temp.len))
		{
			disconnect_process();
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
        else if(0xff!=findpos((uint8_t*)"link in",7,usart3_app_temp.pstr,usart3_app_temp.len))
        {
            disconnect_process();
        }
		vPortFree(usart3_app_temp.pstr);
	}

	vTaskDelay(USART3_APP_delay);
	}
}

uint8_t serverdata_process(usart_task_data usart3_app_temp)
{
	   /*��CJSON������յ�����Ϣ*/
	    cJSON *jsonObj ;
	    cJSON *method;
	    char *m;
	    jsonObj = cJSON_Parse(( char*)usart3_app_temp.pstr);
	    //json�ַ�������ʧ�ܣ�ֱ���˳�
	    if(!jsonObj)
	    {
	        dprintf("json string wrong!\r\n");
	        disconnect_process();

	        return 0;
	    }
	    method = cJSON_GetObjectItem(jsonObj, "M");
	    m = method->valuestring;

	#if 1
	    if(strncmp(m, "WELCOME", 7) == 0)
	    {
	        //��ֹ�豸����״̬δ�������ȵǳ�
	//        checkout();
	//        //��ֹ��������ָ�����
	//        delay_ms(500);
	//        checkin();
	    }

	    //���豸���û���¼�����ͻ�ӭ��Ϣ
	    else if(strncmp(m, "login", 5) == 0)
	    {
	        char *from_id = cJSON_GetObjectItem(jsonObj, "ID")->valuestring;
	        char new_content[] = "Welcome new user!";
	        say2server(from_id, new_content);
	    }
	    //0004����ķ���
	    else if(strncmp(m, "checked", 7) == 0)//���� ������ػ��SEND OK����
	    {
	        xTimerStop(OneShot_CanServer_Handle,10);
	        xSemaphoreGive(SendenableMutexSemaphore);					//�ͷ��ź���
	    }
	    //0004����ķ��أ���ʾ����
	    else if(strncmp(m, "connected", 9) == 0)//������
	    {
	        disconnect_process();
	    }
	    //checkin�ɹ�
	    else if(strncmp(m, "checkinok", 9) == 0)//
	    {
	        touch_flag|=(0x0001<<14);//�������
	    }
	    //�յ�sayָ�ִ����Ӧ��������������Ӧ�ظ�
	    else if(strncmp(m, "say", 3) == 0/* && millis() - lastSayTime > 10*/)
	    {

	        char *content = cJSON_GetObjectItem(jsonObj, "C")->valuestring;
	        char *from_id = cJSON_GetObjectItem(jsonObj, "ID")->valuestring;
	        char *user_name = cJSON_GetObjectItem(jsonObj, "NAME")->valuestring;//��ȡ��ǰ�����û���

	        dprintf("re say:%s:%s\r\n",user_name,content);
	//        lastSayTime = millis();

	        if(strlen(content)==1)//����
	        {
	            if((content[0]>('0'-1))&&(content[0]<('8'+1)))
	            {
	                uint8_t touch_light_temp = content[0]-'0';
	                char say2server_temp[20];
	                server_control_light(0x20|touch_light_temp);
	                sprintf(say2server_temp,"light mode: %1d",touch_flag&0x000f);
	                say2server(from_id, say2server_temp);
	            }
	            else
	            {
	                say2server(from_id, "invalid cmd");
	            }

	        }
	        else//����
	        {

	            if(strncmp(content, "offOn", 5) == 0)
	            {
	                uint8_t touch_light_temp = 4;
	                char say2server_temp[20];
	                server_control_light(0x20|touch_light_temp);
	                sprintf(say2server_temp,"light mode:on %1d",touch_flag&0x000f);
	                say2server(from_id, say2server_temp);

	            }
	            else if(strncmp(content, "play", 4) == 0)
	            {

	                char say2server_temp[20];
	                server_control_light(0x10);
	                sprintf(say2server_temp,"light mode: auto");
	                say2server(from_id, say2server_temp);
	            }
	            else if(strncmp(content, "stop", 4) == 0)
	            {
	                char say2server_temp[20];
	                server_control_light(0x00);
	                sprintf(say2server_temp,"light mode: off");
	                say2server(from_id, say2server_temp);

	            }
	            else if(strncmp(content, "minus", 5) == 0)
	            {
	                char say2server_temp[20];
	                uint8_t touch_light_temp = touch_flag&0x000f;
	                if(touch_light_temp)
	                {
	                    touch_flag&=0xfff0;
	                    touch_light_temp--;
	                    touch_flag|=(touch_light_temp);
	                }

	                server_control_light(0x20|touch_light_temp);
	                sprintf(say2server_temp,"light mode:minus %1d",touch_flag&0x000f);
	                say2server(from_id, say2server_temp);

	            }
	            else if(strncmp(content, "plus", 4) == 0)
	            {
	                char say2server_temp[20];
	                uint8_t touch_light_temp = touch_flag&0x000f;
	                if(touch_light_temp<8)
	                {
	                    touch_flag&=0xfff0;
	                    touch_light_temp++;
	                    touch_flag|=(touch_light_temp);
	                }

	                server_control_light(0x20|touch_light_temp);
	                sprintf(say2server_temp,"light mode:plus %1d",touch_flag&0x000f);
	                say2server(from_id, say2server_temp);

	            }
	            else
	            {
	                char new_content[] = "invalid cmd";
	                //do something here....

	                say2server(from_id, new_content);

	            }
	        }
	    }
	#endif
	    if(jsonObj)
	        cJSON_Delete(jsonObj);
	    return 1;
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

void say2server(char*toid,char*msg)
{
    sprintf(say_did,"%s",toid);
    sprintf(say_content,"%s",msg);
    wifi2server(0x0003);
}

//wifi������������֡����֡����
//����Э�������֡������Ȼ���͸�����3�ķ��Ͷ���
void wifi2server(uint16_t data_type)
{
		uint8_t* p;
	    uint16_t len=0;
	    uint8_t if_send=1,i;
	    usart_task_data USART3_send_data;//USART3send����
	    BaseType_t err;

	    len = 150;//����
	//	dprintf("wifi2server\r\n");
	    p = pvPortMalloc(len);
	    configASSERT(p);
	    memset(p,0,len);
	    switch(data_type)
	    {
	    case 0x0000://checkout
	    {
	        sprintf((char*)p,"{\"M\":\"checkout\",\"ID\":\"%s\",\"K\":\"%s\"}\n", DEVICEID, APIKEY);
	        len=strlen((char*)p);
	    }
	    break;
	    case 0x0001://checkin
	    {

	        sprintf((char*)p,"{\"M\":\"checkin\",\"ID\":\"%s\",\"K\":\"%s\"}\n", DEVICEID, APIKEY);
	        len=strlen((char*)p);
	    }
	    break;
	    case 0x0002://value
	    {
	        float update_value_rtc,update_value_cm3232,update_value_touch;

	        //��ǿ ��λ
	        if(!(touch_flag&(0x0001<<14)))
	            if_send =0;//
	        //�ϴ�һ������
	        update_value_rtc =(float) get_rtc_sec();
	        update_value_cm3232 = (float)CM3232_Read(0x50);
	        update_value_touch = (float)(touch_flag&0x000f);

	        //{"M":"update","ID":"xxxx","V":{"xxxx":"0","xxxx":"44","xxxx":"31"}}
	        if(study_ctr.startstudy)
	        {
		        sprintf((char*)p,"{\"M\":\"update\",\"ID\":\"%s\",\"V\":{\"%s\":\"%f\",\"%s\":\"%f\",\"%s\":\"%f\",\"%s\":\"%f\",\"%s\":\"%f\"}}\n",
		                update_did,
						update_id_rtc, update_value_rtc,
						update_id_cm3232,update_value_cm3232,
						update_id_touch,update_value_touch,
						update_id_time,(float)(get_rtc_sec()-study_ctr.time),
						update_id_flag,(float)study_ctr.ch1);

	        }
	        else
	        {
	        sprintf((char*)p,"{\"M\":\"update\",\"ID\":\"%s\",\"V\":{\"%s\":\"%f\",\"%s\":\"%f\",\"%s\":\"%f\"}}\n",
	                update_did, update_id_rtc, update_value_rtc,update_id_cm3232,update_value_cm3232,update_id_touch,update_value_touch);
	        }
	        len=strlen((char*)p);
	    }
	    break;
	    case 0x0003://say
	    {
	        //say��ǰ״̬
	        if(!(touch_flag&(0x0001<<14)))
	            if_send =0;//
	        sprintf((char*)p,"{\"M\":\"say\",\"ID\":\"%s\",\"C\":\"%s\"}\n", say_did, say_content);//id��һ��fromID
	        len=strlen((char*)p);
	    }
	    break;
	    case 0x0004://check state
	    {

	        sprintf((char*)p,"{\"M\":\"status\"}\n");//
	        len=strlen((char*)p);
	    }
	    break;

	    default:
	        if_send = 0;
	        break;
	    }
	    if(if_send)
	    {

	        USART3_send_data.type = USART_SERVER;
	        USART3_send_data.len  = len;
	        USART3_send_data.pstr = p;
	#if 0//���Է��������������Ƿ���ȷ
	        dprintf("dat2server %4x:",data_type);
	        for(i=0; i<len; i++)
	        {
	            dprintf(" %2x",p[i]);
	        }
	        dprintf("\r\n");
	#endif

	        err=xQueueSend(USART3_Send_Queue,&USART3_send_data,10);
	        if(err==errQUEUE_FULL)   	//������Ϣ֡
	        {
	            //����ʧ����Ҫfree�ڴ�
	            vPortFree(USART3_send_data.pstr);
	            pkt_serial--;
	            dprintf("����USART3_Send_Queue������server%4x����ʧ��!\r\n",data_type);
	        }
	        else
	        {
	            if(data_type==0x0002)
	                xTimerReset(period40or5_0002_Handle,10);
	            pkt_serial++;
	        }

	    }
	    else
	    {
	        vPortFree(p);
	    }

}


//��ʱ���Ͱ�2 Ĭ��40s����һ��  ˯�ߺ�ѧϰ�ı�����Ϊ5sһ��
void period40or5_0002_Callback(TimerHandle_t xTimer)		//
{
    BaseType_t err;
    static uint8_t checkpkg_count = 0;
#if 1//
    if(touch_flag&(0x0001<<14))//�Ѿ�����
    {
        uint16_t temp;
        //���wifi����״̬��

        checkpkg_count++;
        if(checkpkg_count>1)
        {
            checkpkg_count = 0;
            temp = 0x0004;//check
            err=xQueueSend(pack2server_queue,&temp,0);//
        }

        //���Ͱ�2
        temp = 0x0002;
        err=xQueueSend(pack2server_queue,&temp,0);//��ʱ�������У����ܽ�����ʱ����

//                     wifi2server(0x0002);

        if(err==errQUEUE_FULL)   	//������Ϣ֡
        {

            dprintf("����pack2server_Queue���������ڷ���0002���ݷ���ʧ��!\r\n");
        }
        //	wifi2server(0x0002);
    }
    else//��������
    {
        uint8_t temp = 0;
        err=xQueueSend(connect_to_server_Queue,&temp,0);
        if(err==errQUEUE_FULL)   	//������Ϣ֡
        {

            dprintf("����connect_to_server_Queue������1���ݷ���ʧ��!\r\n");
        }
        vTaskResume(connect_to_server_Handler);

    }
#endif
}

void disconnect_process(void)
{
//    u8 temp = 0;
    touch_flag&=(~(0x0001<<4));//�������
    touch_flag&=(~(0x0001<<14));
    if(touch_flag&(0x0001<<7))//����״̬
    {
        mcu_sendcmd2touch(Wifi_Off);
    }
//     xQueueSend(connect_to_server_Queue,&temp,0);
//     vTaskResume(connect_to_server_Handler);
    xTimerChangePeriod(period40or5_0002_Handle,PKG0002_5S_TICKS,10);//�ı䶨ʱ������
//	xTimerStop(period40or5_0002_Handle,10);
    dprintf("disconnect to server\r\n");
}
//end package.c

