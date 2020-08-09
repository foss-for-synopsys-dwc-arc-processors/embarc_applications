#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "embARC.h"
#include "dw_i2s.h"

#define BUFFER_SIZE 160
#define NUM_OF_BUFFER 3

static int16_t gDevBuffer[BUFFER_SIZE* 2];
static DEV_BUFFER gDEV_BUFFER;

static int16_t gBuffer[NUM_OF_BUFFER][BUFFER_SIZE];

static int gWrite=0;
static int gRead= 0;
static int gIsRunnning= 0;
static int gIsOverflow= 0;

static DEV_I2S_PTR gDevice;

/*
此範例程式使用Adafruit SPH0645LM4H I2S 麥克風
CSpotter 語音音量最大介於5000 ~ 20000 之間的辨識效果最佳。
聲音過小或爆音都會造成較差的辨識效果
*/
#define BIAS	1700
#define GAIN	8
static int16_t normalize(int16_t sample){
	int32_t v= (((int32_t)sample) + BIAS)* GAIN;
	if(v> SHRT_MAX) return SHRT_MAX;
	if(v< SHRT_MIN) return SHRT_MIN;
	return (int16_t)v;
}

static void callback_on_record_data(uint32_t count){
	 int i;
	 int16_t *buffer;
	 if(!gIsRunnning) return;
	 if(!gIsOverflow&& gWrite- gRead>= NUM_OF_BUFFER){
		 gIsOverflow= 1;
		 gWrite= gRead+ 1;
		 printf("record buffer overflow\r\n");
	 }else if(gWrite> 10000&& gRead== gWrite){
		gRead= gWrite= 0;
	 }
	 buffer= gBuffer[gWrite%NUM_OF_BUFFER];
	 for(i= 0; i< BUFFER_SIZE; i++){
		 buffer[i]= normalize(gDevBuffer[i*2]);
	 }
	 gWrite++;
	 dw_i2s_control(gDevice, I2S_CMD_SET_RXINT_BUF,&gDEV_BUFFER);
 }
 
 static void callback_on_record_error(uint32_t error){
	 printf("record fail\r\n");
 }
 
int Recorder_Init(){
	int nRet;
	DW_I2S_CONFIG *i2s_config_ptr;
	gDevice= i2s_get_dev(DW_I2S_1_ID);
	if(gDevice== 0) return 1;
	i2s_config_ptr = (DW_I2S_CONFIG *)(gDevice->i2s_info.i2s_config);
//	i2s_config_ptr->data_res[0] = I2S_AUD_DATA_32B;
	i2s_rx_clk_div(1538/(32*2)); //for sample rate 16000
	DEV_BUFFER_INIT(&gDEV_BUFFER, gDevBuffer, BUFFER_SIZE*2);
	nRet= dw_i2s_open(gDevice, DEV_MASTER_MODE, I2S_DEVICE_RECEIVER);
	if(nRet!= E_OK){
		printf("open record fail: %d\r\n", nRet);
	}
	dw_i2s_control(gDevice, I2S_CMD_ENA_DEV, 0);
	dw_i2s_control(gDevice, I2S_CMD_MST_SET_CLK, (void*)1);
	dw_i2s_control(gDevice, I2S_CMD_SET_RXCHDT_BUF, (void*)0x0080000); //receive 8 sample frame each time, so BUFFER_SIZE should be multiple of 8 
	dw_i2s_control(gDevice, I2S_CMD_SET_RXCB, callback_on_record_data);
	dw_i2s_control(gDevice, I2S_CMD_SET_ERRCB, callback_on_record_error);
	dw_i2s_control(gDevice, I2S_CMD_SET_RXINT_BUF,&gDEV_BUFFER);
    return 0;
}

int16_t* Recorder_GetBuffer(){
	if(gRead>= gWrite) return 0;
	return gBuffer[gRead%NUM_OF_BUFFER];
}

int Recorder_NextBuffer(){
	if(gRead>= gWrite) return 0;
	gRead++;
	return 1;
}

int Recorder_GetBufferSize(){
	return BUFFER_SIZE;
}

int Recorder_Start(){
	gRead= gWrite= 0;
	gIsRunnning= 1;
	dw_i2s_control(gDevice, I2S_CMD_SET_RXINT, (void*)1);
	return 0;
}

void Recorder_Stop(){
	gIsRunnning= 0;
	dw_i2s_control(gDevice, I2S_CMD_SET_RXINT, (void*)0);
}



void Recorder_Release(){
	Recorder_Stop();
	dw_i2s_control(gDevice, I2S_CMD_MST_SET_CLK, (void*)0);
}
