/* ------------------------------------------
 * Copyright (c) 2018, Synopsys, Inc. All rights reserved.

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
#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "stdio.h"
#include "string.h"

#include "recognize.h"
#include "display.h"
#include "spi_att7053.h"
#include "my_uart.h"
#include "esp8266_network.h"
#include "self_mqtt.h"
#include "string_KMP.h"
#include "data_provider.h"

#define ATT7053_SPI0 		0	
#define WIN_LENGTH			12
#define WIN_LENGTH_2		8
#define CONNECT_ERROR       (0)
#define CONNECT_OK			(1)

constexpr int kTensorArenaSize = 10 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
TfLiteTensor* model_input = nullptr;
TfLiteTensor* model_output = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;

sample_data_node data[WIN_LENGTH];
ATT7053_DEF(att7053,ATT7053_SPI0,DEV_SPI_CS_LINE_1);

//*****************ESP8266部分*******************

extern DEV_UART *uart_obj;

void Mqtt_Progress(char *buf,uint16_t len);
char mqtt_message[300]; 
float temperature=0;
float humidity=0;

float EnvHumidity_Low_Threshold=60;
float EnvHumidity_High_Threshold=95;
float EnvTemp_Low_Threshold=10;
float EnvTemp_High_Threshold=30;

uint8_t sta;
//**********************************************************

void PrintEvent(sample_data_node_ptr front)
{
	for(uint32_t i=0;i<WIN_LENGTH;i++){
		front = front->next_ptr;
		EMBARC_PRINTF("%d,%d,%d\n",front->IRMS,-(front->active_power),-(front->reactive_power));
	}
}

bool connect_cloud()
{
	EMBARC_PRINTF("Connnecting to the cloud platform...\r\n");
	board_delay_ms(500, 1);
	//检查ESP8266
    if(_net.Init(UART_RX_BUF,UART_MAX_RECV_LEN,UART_TX_BUF,UART_MAX_SEND_LEN)==0){
		EMBARC_PRINTF("Net Init Error!\r\n");
		return CONNECT_ERROR;
	}
	//连接wifi
	if(_net.ConnectAP("HUAWEI","zhang111")==0){
		EMBARC_PRINTF("Conncet AP Error!\r\n");
		return CONNECT_ERROR;
	}
	//连接TCP   //********ID端口：a1sezVXBekj.iot-as-mqtt.cn-shanghai.aliyuncs.com*******
	if(_net.ConnectServer("TCP","a1HJbdW0mhX.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883)==0) {  //设备号ProductKey：a1sezVXBekj
		EMBARC_PRINTF("Conncet Server Error!\r\n");	
		return CONNECT_ERROR;
	}
	//登录MQTT		
	if(_mqtt.Connect(
		"ARC01|securemode=3,signmethod=hmacsha1|",//ClientID
		"ARC01&a1HJbdW0mhX",//Username
		"668C6050F7632F3ACFA491DD1E7D9AABC215A90A"//Password
		) == 0){	
		EMBARC_PRINTF("Enter MQTT Error!\r\n");
		return CONNECT_ERROR;	
	}	
	//订阅主题
	if(_mqtt.SubscribeTopic("/sys/a1HJbdW0mhX/ARC01/thing/service/property/set",0,1) == 0) {		
		EMBARC_PRINTF("SubscribeTopic Error!\r\n");
		return CONNECT_ERROR;
	}
	board_delay_ms(1000, 1);
	return CONNECT_OK;				
}

int main(void)
{
//************************初始化*************************
	EMBARC_PRINTF(" Initialize begin!\r\n");
	if(ATT7053_init(att7053, 1200000)!=E_OK){
		EMBARC_PRINTF("ATT7053 Initial Error!!!!\r\n");
	}
	EMU_Init(att7053);  //配置采集芯片寄存器
	data_container_init(data,WIN_LENGTH);
	m_u8g_init();
	UART_Init(1,115200);
	tflm_ini(model,model_input,interpreter,tensor_arena,kTensorArenaSize,error_reporter);
	EMBARC_PRINTF(" Initialize finish!\r\n");
//******************esp8266连接云平台********************
	if(connect_cloud()==CONNECT_OK){
		EMBARC_PRINTF("Successfully connected to the cloud platform\r\n");
	};
//******************************************************   
//	msleep(1000);
	uint16_t app_state = 0;
	uint16_t d_power = 0;
	uint16_t eflag = 0;
	float current_power = 0;
	char c_power[2][10];
	sample_data_node_ptr data_buffer = data;
	uint32_t num = 0;

 	while(1) {
		get_data(att7053, data_buffer);
		if(eflag == 0 && is_event(data,WIN_LENGTH,data_buffer)){
			data_provider(model_input,data_buffer,WIN_LENGTH);
			app_state = recognize(app_state,interpreter,model_output,error_reporter);
			d_power= data_buffer->active_power;
			current_power = d_power*0.05;
			sprintf(c_power[0], "%4.1f", current_power);
			EMBARC_PRINTF("    Current Power: %s W\r\n", c_power[0]);
			eflag = WIN_LENGTH;
		}	
		if( eflag > 0){
			eflag--;
		}	
		//************OLED屏幕显示*************************
		d_power= ((data_buffer->active_power<<16)>>16);
		display(app_state,d_power);
		//************************************************
		//将传感器数据，采样数据，模型推理识别结果等上传到云平台
		float Voltage_frequency = 460500/Read_Reg(att7053,Voltage_Frequency_Register);
		float Current_value = (data_buffer->IRMS)*0.00286;
		uint8_t Active_power = d_power * 0.05;
		uint8_t Reactive_power = data_buffer->reactive_power * 0.02;
		uint8_t DDS_State = (app_state & (uint8_t)1)==1;
		uint8_t Fan_State = (app_state & (uint8_t)2)==2;
		uint8_t cnt_2s=0;
		if(++cnt_2s>=20){
			cnt_2s=0;
			sprintf(mqtt_message,
			"{\"method\":\"thing.service.property.set\",\"id\":\"123\",\"params\":{\
				\"Voltage_frequency\":%.1f,\
				\"Current_value\":%.1f,\
				\"Active_power\":%d,\
				\"Reactive_power\":%d,\
				\"Fan_State\":%d,\
				\"DDS_State\":%d,\
			},\"version\":\"1.0.0\"}",
			Voltage_frequency,
			Current_value,
			Active_power,
			Reactive_power,
			Fan_State,
			DDS_State
			);
        	EMBARC_PRINTF("massage=%s\r\n",mqtt_message);
			_mqtt.PublishData("/sys/a1HJbdW0mhX/ARC01/thing/event/property/post",mqtt_message,0);			
		}
		if(UART_RX_STA){
			Mqtt_Progress((char*)_mqtt.rxbuf,UART_RX_STA);
			EMBARC_PRINTF("%s",_mqtt.rxbuf);		
			memset(_mqtt.rxbuf,0,_mqtt.rxlen);
			UART_RX_STA = 0;
		}			
//********************************************************************************************
		data_buffer = data_buffer->next_ptr;
		num++;		
		msleep(100);
    }
}

float string2float(char *buf)
{
	float res=0;
	int postive=1;
	if((*buf)=='-'){
		postive=-1;
		buf++;
	}
	uint8_t point_flag=0;
	while((*buf)!='}'){
		if((*buf)=='.'){
			point_flag=1;
			buf++;
			break;
		}
		if((*buf)>='0'&&(*buf)<='9'){	
			res = (*buf)-'0'+res*10;
		}
		buf++;
	}
	if(point_flag==1){
		res = res+((*buf)-'0')*0.1;
	}
	return postive*res;
}


uint8_t *p;
void Mqtt_Progress(char *buf,uint16_t len)
{
	 
	char *keyStr = "\"params\":{\"EniTemp";
	char printbuf[5]={0};
	int i = KMP(buf,keyStr);

	EMBARC_PRINTF("%c\r\n",*(buf+i));
	if(i==-1)	return;
	if(buf[i]=='H'&&buf[i+1]=='i'&&buf[i+2]=='g'&&buf[i+3]=='h'){
		EnvHumidity_High_Threshold = string2float(buf+i+16);
		sprintf(printbuf,"%.1f",EnvHumidity_High_Threshold);
		EMBARC_PRINTF("EnvHumidity_High_Threshold=%s\r\n",printbuf);
	}
	if(buf[i]=='L'&&buf[i+1]=='o'&&buf[i+2]=='w'){
		EnvHumidity_Low_Threshold = string2float(buf+i+15);
		sprintf(printbuf,"%.1f",EnvHumidity_Low_Threshold);
		EMBARC_PRINTF("EnvHumidity_Low_Threshold=%s\r\n",printbuf);
	}	
}
