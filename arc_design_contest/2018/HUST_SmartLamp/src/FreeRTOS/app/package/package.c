/*
 * package.c
 *
 *  Created on: 2018年5月3日
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
//与服务器通信，以及数据和命令的处理
//接收到wifi模块数据处理，
//把数据组包发送给wifi模块

//任务定义
QueueHandle_t pack2server_queue;
TaskHandle_t USART3_app_Handler;
TaskHandle_t pack2server_Handler;

TimerHandle_t	period40or5_0002_Handle;			//00 02包 定时发送
//每个发送包的长度					1  2  3  4  5  6	7	 8	9	 A	B	 C	D	 E	F 10 11 12 13 14 15 16 17 18 19 1a
//uint16_t tx_package_len[26] = {78,56,34,34,66,56,00,35,00,00,00,00,00,00,00,00,48,49,00,00,00,00,00,00,00,00};//十进制

uint16_t pkt_serial=1;	//发送包，数量计数
uint8_t time_serial;//时间序号 8008和0008的功能

char *DEVICEID = "6333";
char *APIKEY = "6599b2865";
char update_did[6]="6333";//设备ID test_arc
char update_id_rtc[6]="5644";//接口ID test02_01
char update_id_cm3232[]="5686";//数据接口 光强
char update_id_touch[]="5687";//数据接口 档位
char update_id_time[]="6098";
char update_id_flag[]="6100";
char say_did[30]="U4571";
char say_did_default[]="U4571";
char say_content[64]="hello";

//串口3 APP层处理任务
void usart3_app_task(void *pvParameters)
{
	usart_task_data usart3_app_temp;//接收USART3_task 发出的服务器分帧数据

	uint8_t*buffer;
	uint8_t len,i;
//	uint64_t irf_temp;

	EMBARC_PRINTF("usart3_app_task start!\r\n");
//    vTaskResume(USART3_Handler);
	while(1)
	{


	if(xQueueReceive(USART3_Task_server_Queue,&usart3_app_temp,0))//接收到服务器数据portMAX_DELAY
	{
		#if 1//测试接收服务器数据是否正确
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
		//1检验crc32 device_id 协议版本号 包序号 等

		//处理数据
		serverdata_process(usart3_app_temp);

		vPortFree(usart3_app_temp.pstr);
	}
//还要处理无效的\R\N和other数据，避免内存一直被占用，队列不能为空

		if(xQueueReceive(USART3_Task_rn_other_Queue,&usart3_app_temp,0))//接收到服务器数据portMAX_DELAY
	{
		#if 0//使用串口3，把板子模拟成串口使用
		buffer=pvPortMalloc(usart3_app_temp.len+1);
		memcpy(buffer,usart3_app_temp.pstr,usart3_app_temp.len);
		buffer[usart3_app_temp.len]=0;
		dprintf("U3re:%2d:%s\r\n",usart3_app_temp.len,buffer);//正常使用printf 不是调试
//		EMBARC_PRINTF("u3app RNdat\r\n");
		vPortFree(buffer);
		#endif
		if(0xff!=findpos((uint8_t*)"CLOSED",6,usart3_app_temp.pstr,usart3_app_temp.len))
		{
			disconnect_process();
		}
		//发送完成
		else if(0xff!=findpos((uint8_t*)"SEND OK",7,usart3_app_temp.pstr,usart3_app_temp.len))
		{
			//释放信号量
			//一旦有一个没有收到回执，就会再也不发送 定时器延时使能
			xTimerStop(OneShot_CanServer_Handle,10);
			xSemaphoreGive(SendenableMutexSemaphore);					//释放信号量
//			u3cansend = 1;
		}
        //每次发送完成数据后，会返回SEND OK 如果没有，就说明出问题
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
	   /*用CJSON处理接收到的信息*/
	    cJSON *jsonObj ;
	    cJSON *method;
	    char *m;
	    jsonObj = cJSON_Parse(( char*)usart3_app_temp.pstr);
	    //json字符串解析失败，直接退出
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
	        //防止设备在线状态未消除，先登出
	//        checkout();
	//        //防止连续发送指令过快
	//        delay_ms(500);
	//        checkin();
	    }

	    //有设备或用户登录，发送欢迎信息
	    else if(strncmp(m, "login", 5) == 0)
	    {
	        char *from_id = cJSON_GetObjectItem(jsonObj, "ID")->valuestring;
	        char new_content[] = "Welcome new user!";
	        say2server(from_id, new_content);
	    }
	    //0004检查后的返回
	    else if(strncmp(m, "checked", 7) == 0)//在线 这个返回会把SEND OK覆盖
	    {
	        xTimerStop(OneShot_CanServer_Handle,10);
	        xSemaphoreGive(SendenableMutexSemaphore);					//释放信号量
	    }
	    //0004检查后的返回，表示掉线
	    else if(strncmp(m, "connected", 9) == 0)//掉线了
	    {
	        disconnect_process();
	    }
	    //checkin成功
	    else if(strncmp(m, "checkinok", 9) == 0)//
	    {
	        touch_flag|=(0x0001<<14);//标记上线
	    }
	    //收到say指令，执行相应动作，并进行相应回复
	    else if(strncmp(m, "say", 3) == 0/* && millis() - lastSayTime > 10*/)
	    {

	        char *content = cJSON_GetObjectItem(jsonObj, "C")->valuestring;
	        char *from_id = cJSON_GetObjectItem(jsonObj, "ID")->valuestring;
	        char *user_name = cJSON_GetObjectItem(jsonObj, "NAME")->valuestring;//获取当前操作用户名

	        dprintf("re say:%s:%s\r\n",user_name,content);
	//        lastSayTime = millis();

	        if(strlen(content)==1)//数字
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
	        else//命令
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


//发送给服务器的包，进行组帧然后发送给发送函数
void pack2server(void *pvParameters)
{
	uint16_t packtype;
	while(1)
	{
		if(xQueueReceive(pack2server_queue,&packtype,0))//接收到服务器数据portMAX_DELAY
		{
			wifi2server(packtype);
		}

		vTaskDelay(pack2server_delay);
	}
}

//调用发送数据包 外部调用
void sendpack2server(uint16_t package_num)
{
	uint16_t temp = package_num;
	BaseType_t err;
	err=xQueueSend(pack2server_queue,&temp,0);
	if(err==errQUEUE_FULL)   	//发送消息帧
	{

	dprintf("队列pack2server_Queue已满，%4x数据发送失败!\r\n",temp);
	}
}

void say2server(char*toid,char*msg)
{
    sprintf(say_did,"%s",toid);
    sprintf(say_content,"%s",msg);
    wifi2server(0x0003);
}

//wifi发给服务器的帧，组帧函数
//根据协议进行组帧操作，然后发送给串口3的发送队列
void wifi2server(uint16_t data_type)
{
		uint8_t* p;
	    uint16_t len=0;
	    uint8_t if_send=1,i;
	    usart_task_data USART3_send_data;//USART3send数据
	    BaseType_t err;

	    len = 150;//测试
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

	        //光强 档位
	        if(!(touch_flag&(0x0001<<14)))
	            if_send =0;//
	        //上传一个数据
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
	        //say当前状态
	        if(!(touch_flag&(0x0001<<14)))
	            if_send =0;//
	        sprintf((char*)p,"{\"M\":\"say\",\"ID\":\"%s\",\"C\":\"%s\"}\n", say_did, say_content);//id不一样fromID
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
	#if 0//测试发给服务器数据是否正确
	        dprintf("dat2server %4x:",data_type);
	        for(i=0; i<len; i++)
	        {
	            dprintf(" %2x",p[i]);
	        }
	        dprintf("\r\n");
	#endif

	        err=xQueueSend(USART3_Send_Queue,&USART3_send_data,10);
	        if(err==errQUEUE_FULL)   	//发送消息帧
	        {
	            //发送失败需要free内存
	            vPortFree(USART3_send_data.pstr);
	            pkt_serial--;
	            dprintf("队列USART3_Send_Queue已满，server%4x发送失败!\r\n",data_type);
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


//定时发送包2 默认40s发送一次  睡眠和学习改变周期为5s一次
void period40or5_0002_Callback(TimerHandle_t xTimer)		//
{
    BaseType_t err;
    static uint8_t checkpkg_count = 0;
#if 1//
    if(touch_flag&(0x0001<<14))//已经上线
    {
        uint16_t temp;
        //检测wifi连接状态？

        checkpkg_count++;
        if(checkpkg_count>1)
        {
            checkpkg_count = 0;
            temp = 0x0004;//check
            err=xQueueSend(pack2server_queue,&temp,0);//
        }

        //发送包2
        temp = 0x0002;
        err=xQueueSend(pack2server_queue,&temp,0);//定时器任务中，不能进行延时操作

//                     wifi2server(0x0002);

        if(err==errQUEUE_FULL)   	//发送消息帧
        {

            dprintf("队列pack2server_Queue已满，周期发送0002数据发送失败!\r\n");
        }
        //	wifi2server(0x0002);
    }
    else//尝试上线
    {
        uint8_t temp = 0;
        err=xQueueSend(connect_to_server_Queue,&temp,0);
        if(err==errQUEUE_FULL)   	//发送消息帧
        {

            dprintf("队列connect_to_server_Queue已满，1数据发送失败!\r\n");
        }
        vTaskResume(connect_to_server_Handler);

    }
#endif
}

void disconnect_process(void)
{
//    u8 temp = 0;
    touch_flag&=(~(0x0001<<4));//标记下线
    touch_flag&=(~(0x0001<<14));
    if(touch_flag&(0x0001<<7))//开灯状态
    {
        mcu_sendcmd2touch(Wifi_Off);
    }
//     xQueueSend(connect_to_server_Queue,&temp,0);
//     vTaskResume(connect_to_server_Handler);
    xTimerChangePeriod(period40or5_0002_Handle,PKG0002_5S_TICKS,10);//改变定时器周期
//	xTimerStop(period40or5_0002_Handle,10);
    dprintf("disconnect to server\r\n");
}
//end package.c

