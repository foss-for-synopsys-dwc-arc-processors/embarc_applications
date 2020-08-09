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

#include "cc2541.h"

#include "esp8266.h"

#include "Spotter.h"
#include "recorder.h"
#include "CYBase.mod.h"
#include "Group_1.mod.h"
#include "Group_2.mod.h"

#include "dev_uart.h"
#include "ez_sio.h"

#include <stdio.h>
#include <string.h>

/* ESP8266 Buffer Define */
static esp8266_def* esp8266;
/* End ESP8266 Buffer Define */

/* HM10 Buffer Define */
static cc2541_def* cc2541;
/* End HM10 Buffer Define */

/* DSpotter Memory Define */
//use fixed size memory to avoid heap too small
#define pMem ((void*)0x30000000)
#define nMemSize (0x00020000)
#define RECORD_TIMEOUT  8000
int16_t* pData;
uint32_t spottedMS;

typedef enum{
  LIVING_ROOM = 0,
  STUDY = 1<<2,
  DOOR = 2<<2,
  GATE = 3<<2,
  KITCHEN = 4<<2,
  DINING_ROOM = 5<<2,
} PLACE_DEF;

typedef enum{
  LIGHT = 0,
  AC = 1<<1,
} ELECTRIC_TYPE_DEF;

typedef enum{
  ON = 0,
  OFF = 1,
} SWITCH_TYPE_DEF;

typedef enum {
  NO_DETECT = -2,
  FIRST_STAGE = -1,
  LIV_L_ON = LIVING_ROOM|LIGHT|ON,
  LIV_L_OFF = LIVING_ROOM|LIGHT|OFF,
  LIV_AC_ON = LIVING_ROOM|AC|ON,
  LIV_AC_OFF = LIVING_ROOM|AC|OFF,
  STU_L_ON = STUDY|LIGHT|ON,
  STU_L_OFF = STUDY|LIGHT|OFF,
  STU_AC_ON = STUDY|AC|ON,
  STU_AC_OFF = STUDY|AC|OFF,
  DOOR_L_ON = DOOR|LIGHT|ON,
  DOOR_L_OFF = DOOR|LIGHT|OFF,
  DOOR_AC_ON = DOOR|AC|ON,
  DOOR_AC_OFF = DOOR|AC|OFF,
  GATE_L_ON = GATE|LIGHT|ON,
  GATE_L_OFF = GATE|LIGHT|OFF,
  GATE_AC_ON = GATE|AC|ON,
  GATE_AC_OFF = GATE|AC|OFF,
  KIT_L_ON = KITCHEN|LIGHT|ON,
  KIT_L_OFF = KITCHEN|LIGHT|OFF,
  KIT_AC_ON = KITCHEN|AC|ON,
  KIT_AC_OFF = KITCHEN|AC|OFF,
  DIN_L_ON = DINING_ROOM|LIGHT|ON,
  DIN_L_OFF = DINING_ROOM|LIGHT|OFF,
  DIN_AC_ON = DINING_ROOM|AC|ON,
  DIN_AC_OFF = DINING_ROOM|AC|OFF,
} COMMAND_STATE_DEF;
COMMAND_STATE_DEF recNum = NO_DETECT;
/* End DSPotter Memory Define */

/* Device Table Define */
typedef enum{
  NONE,
  REG,
  SEARCHED,
} DEVICE_STATE;

struct{
  char mac[12];
  char inqNum;
  DEVICE_STATE state;
} deviceTable[12];
/* End Device Table Define */

struct{
  enum{
    NO_CONTROL,
    REACH,
    SEND,
    ACK,
    RECONN,
  } state;
  int index;
  uint32_t delay_ms;
  char command[10];
  uint32_t command_length;
} controlBuffer;

typedef enum{
  TRIGGER,
  REGISTER,
  DELETE,
  SET_ON,
  SET_OFF,
} MQTT_COMMAND_TYPE;

struct{
  enum{
    IDLE,
    WAIT,
    SEARCH,
  } state;
  uint32_t delay_ms;
  MQTT_COMMAND_TYPE command;
} mqttHandler;

static int initRecognize(){
	int nRet;
	nRet = Recorder_Init();
	if(nRet!= 0){
		printf("record init fail: %d\r\n", nRet);
		return 1;
	}
	Spotter_Reset();
	Recorder_Start();	
	return nRet;
}

void control_device(){
  if(controlBuffer.state == NO_CONTROL)
    return;
  else if(controlBuffer.state == REACH){
    if(deviceTable[controlBuffer.index].state == NONE){
      df_playTrack(esp8266, 7);
      controlBuffer.state = NO_CONTROL;
      return;
    }
    else if(deviceTable[controlBuffer.index].state == REG){
      df_playTrack(esp8266, 8);
      controlBuffer.state = NO_CONTROL;
    }
    else if(deviceTable[controlBuffer.index].state == SEARCHED){
      char command[11];
      sprintf(command, "AT+CONN%c\r\n", deviceTable[controlBuffer.index].inqNum);
      ez_sio_write(cc2541->uart, command, 10);
      controlBuffer.state = SEND;
      controlBuffer.delay_ms = 600 + OSP_GET_CUR_MS();
    }
  }
  else if(controlBuffer.state == SEND && OSP_GET_CUR_MS() > controlBuffer.delay_ms){
    ez_sio_read(cc2541->uart, cc2541->rxBuff, 200);
    ez_sio_write(cc2541->uart, controlBuffer.command, controlBuffer.command_length);
    controlBuffer.state = ACK;
    EMBARC_PRINTF("%s", controlBuffer.command);
    controlBuffer.delay_ms = 400 + OSP_GET_CUR_MS();
  }
  else if(controlBuffer.state == ACK && OSP_GET_CUR_MS() > controlBuffer.delay_ms){
    memset(cc2541->rxBuff, 0, 200);
    ez_sio_read(cc2541->uart, cc2541->rxBuff, 200);
    EMBARC_PRINTF("ACK%s", cc2541->rxBuff);
    if(strncmp(cc2541->rxBuff, "OK", 2) == 0) df_playTrack(esp8266, 8);
    else if(strncmp(controlBuffer.command, "on", 2) == 0) df_playTrack(esp8266, 5);
    else if(strncmp(controlBuffer.command, "off", 3) == 0) df_playTrack(esp8266, 6);
    controlBuffer.state = RECONN;
    memset(controlBuffer.command, 0, 10);
    memset(cc2541->rxBuff, 0, 200);
    cc2541->rstPort->gpio_write(0, cc2541->rstPin);
    board_delay_ms(100, 1);
    cc2541->rstPort->gpio_write(cc2541->rstPin, cc2541->rstPin);
    controlBuffer.delay_ms = OSP_GET_CUR_MS() + 400;
  }
  else if(controlBuffer.state == RECONN && OSP_GET_CUR_MS() > controlBuffer.delay_ms){
    controlBuffer.state = NO_CONTROL;
    ez_sio_write(cc2541->uart, "AT+INQ\r\n", 8);
  }
}

void handleMQTT(){
  if(mqttHandler.state == IDLE && rb_isempty(&esp8266->uart->rcv_rb)){
    return;
  }
  else if(mqttHandler.state == IDLE){
    mqttHandler.state = WAIT;
    mqttHandler.delay_ms = OSP_GET_CUR_MS() + 200;
  }
  else if(mqttHandler.state == WAIT && mqttHandler.delay_ms > OSP_GET_CUR_MS()){
    ez_sio_read(esp8266->uart, esp8266->buff, 200);
    EMBARC_PRINTF("%s\n", esp8266->buff);
    char* ptr = esp8266->buff;
    char mac[13];
    int index;
    while(*ptr != '}'){
      if(strncmp(ptr, "trig", 4) == 0){
        strncpy(controlBuffer.command, "trig", 4);
        controlBuffer.command_length = 4;
        mqttHandler.command = TRIGGER;
        ptr += 16;
      }
      else if(strncmp(ptr, "set_open", 8) == 0){
        strncpy(controlBuffer.command, "set_on", 6);
        controlBuffer.command_length = 6;
        mqttHandler.command = SET_ON;
        ptr += 18;
      }
      else if(strncmp(ptr, "set_close", 9) == 0){
        strncpy(controlBuffer.command, "set_off", 7);
        controlBuffer.command_length = 7;
        mqttHandler.command = SET_OFF;
        ptr += 19;
      }
      else if(strncmp(ptr, "reg", 3) == 0){
        mqttHandler.command = REGISTER;
        ptr += 15;
      }
      else if(strncmp(ptr, "del", 3) == 0){
        mqttHandler.command = DELETE;
        ptr += 15;
      }
      else if(*ptr == '/'){
        ptr ++;
        if(*(ptr++) == '0'){
          switch(*(ptr++)){
            case '0':
              index = 0;
              break;
            case '1':
              index = 1;
              break;
            case '2':
              index = 2;
              break;
            case '3':
              index = 3;
              break;
            case '4':
              index = 4;
              break;
            case '5':
              index = 5;
              break;
          }
        }
        else {
          switch(*(ptr++)){
            case '0':
              index = 6;
              break;
            case '1':
              index = 7;
              break;
            case '2':
              index = 8;
              break;
            case '3':
              index = 9;
              break;
            case '4':
              index = 10;
              break;
            case '5':
              index = 11;
              break;
          }
        }
      }
      else if(*ptr == '@'){
        ptr++;
        strncpy(mac, ptr, 12);
      }
      else{
        ptr ++;
      }
    }
    memset(esp8266->buff, 0, 200);
    if(mqttHandler.command == SET_ON || 
       mqttHandler.command == SET_OFF ||
       mqttHandler.command == TRIGGER){
      controlBuffer.index = index;
      controlBuffer.state = REACH;
      mqttHandler.state = IDLE;
    }
    else if(mqttHandler.command == REGISTER){
      strncpy(deviceTable[index].mac, mac, 12);
      ez_sio_read(cc2541->uart, cc2541->rxBuff, 200);
      memset(cc2541->rxBuff, 0, 200);
      ez_sio_write(cc2541->uart, "AT+INQ\r\n", 8);
      mqttHandler.state = SEARCH;
      mqttHandler.delay_ms = OSP_GET_CUR_MS() + 10000;
    }
    else if(mqttHandler.command == DELETE){
      deviceTable[index].state = NONE;
      memset(deviceTable[index].mac, 0, 12);
      deviceTable[index].inqNum = 0;
      mqttHandler.state = IDLE;
    }
  }
  else if(mqttHandler.state == SEARCH && OSP_GET_CUR_MS() > mqttHandler.delay_ms){
    ez_sio_read(cc2541->uart, cc2541->rxBuff, 200);
    char* ptr = cc2541->rxBuff;
    while(strncmp(ptr, "+INQE", 5) != 0){
      if(strncmp(ptr, "OK", 2) == 0){
        ptr += 4;
      }
      else if(strncmp(ptr, "+INQS", 5) == 0){
        ptr += 7;
      }
      else if(strncmp(ptr, "+INQ:", 5) == 0){
        ptr += 5;
        char inqNum = *ptr;
        ptr += 4;
        char mac[13];
        strncpy(mac, ptr, 12);
        ptr += 17;
        // EMBARC_PRINTF("get%c %12s\n",inqNum, mac);
        for(int i = 0; i < 12; i ++){
          // EMBARC_PRINTF("%s", mac);
          if(strncmp(mac, deviceTable[i].mac, 12) == 0){
            deviceTable[i].inqNum = inqNum;
            deviceTable[i].state = SEARCHED;
          }
        }
      }
      else 
        ptr++;
    }
    for(int i = 0; i < 12; i ++){
      EMBARC_PRINTF("%d %c\n", i, deviceTable[i].inqNum);
    }
    mqttHandler.state = IDLE;
  }
}

int main(void)
{
  /* Init ESP and Play Opening Sound */
  esp8266 = esp8266_init(DFSS_UART_1_ID, UART_BAUDRATE_115200, 200, 200);
  // do{
  //   ez_sio_write(esp8266, checkConnection, 7);
  //   board_delay_ms(100, 1);
  //   ez_sio_read(esp8266, esp8266Buffer, 2);
  //   EMBARC_PRINTF("%c", esp8266Buffer[0]);
  // } while(esp8266Buffer[0] == 0);
  df_init(esp8266);
  df_setVolume(esp8266);
  df_playTrack(esp8266, 1);
  board_delay_ms(6000, 1);
  /* End Init ESP and Play Opening Sound */
  
  /* HM10 Init */
  EMBARC_PRINTF("========================== Init HM10 =================================\n");
  cc2541 = cc2541_init(DFSS_UART_2_ID, UART_BAUDRATE_9600, 200, 200, DFSS_GPIO_8B1_ID, 4);
  if(cc2541==NULL)
    EMBARC_PRINTF("CC2541 Didn't Init Successfully.\n");  
  /* End HM10 Init */

	/* ESP8266 Connect Wifi */
	EMBARC_PRINTF("============================ Connect WiFi ============================\n");
  esp8266_getConnState(esp8266);
  if(esp8266->connState == NO_CONN){
    if(cc2541_get_connection_role(cc2541) == 1){
      cc2541_set_connection_role(cc2541, 0);
    }
    df_playTrack(esp8266, 2);
    board_delay_ms(4500, 1);
    memset(cc2541->rxBuff, 0, 200);
    do{
      esp8266_getConnState(esp8266);
      if(esp8266->connState == NO_CONN){
        while(cc2541->rxBuff[0] == 0){
          if(rb_isempty(&cc2541->uart->rcv_rb) == 0){
            cc2541_read(cc2541);
            // EMBARC_PRINTF("%s", cc2541->rxBuff);
            char ssid[100], pswd[100];
            char* ptr = cc2541->rxBuff;
            while(*(ptr++) != ';');
            strncpy(ssid, cc2541->rxBuff, ptr-cc2541->rxBuff - 1);
            char* ptr2 = ptr + 1;
            while(*(ptr2++) != ';');
            strncpy(pswd, ptr, ptr2-ptr-1);
            EMBARC_PRINTF("%s\n%s", ssid, pswd);
            memset(esp8266->buff, 0, 100);
            sprintf(esp8266->buff, "%s\n", ssid);
            ez_sio_write(esp8266->uart, esp8266->buff, ptr-cc2541->rxBuff);
            EMBARC_PRINTF("%s %d", esp8266->buff, ptr-cc2541->rxBuff);
            board_delay_ms(1000, 1);

            memset(esp8266->buff, 0, 100);
            sprintf(esp8266->buff, "%s\n", pswd);
            ez_sio_write(esp8266->uart, esp8266->buff, ptr2-ptr);
            EMBARC_PRINTF("%s %d", esp8266->buff, ptr2-ptr);
            board_delay_ms(1000, 1);
            esp8266_setConnection(esp8266);
            esp8266_getConnState(esp8266);
            memset(cc2541->rxBuff, 0, 200);
            EMBARC_PRINTF("CONN:%d", esp8266->connState);
          }
        }
      }
    } while(esp8266->connState == NO_CONN);
    cc2541_disconnect(cc2541);
  }
  
  // EMBARC_PRINTF("%d\n", esp8266_conn_status(esp8266));
  // while(esp8266->wifi_connected == false){
  //   for(trialNum = 0; trialNum < ESP8266_MAX_CONN_TRIAL; trialNum ++){
  //     if(esp8266_conn_status(esp8266) == ESP8266_WIFI_CONN_SUCCESS)
  //       break;
  //   }
  //   if(esp8266->wifi_connected == false){
  //     EMBARC_PRINTF("Can't connect to Wifi...\n");
  //     if(cc2541_get_connection_role(cc2541) == 1){
  //       EMBARC_PRINTF("Setting CC2541 to Client.\n");
  //       cc2541_set_connection_role(cc2541, 0);
  //     }
  //     memset(ssid, 0, 100);
  //     memset(pswd, 0, 100);
  //     memset(cc2541->rxBuff, 0, 200);
  //     // ssid[0] = '\"';
  //     // pswd[0] = '\"';
  //     while(ssid[1] == 0 || pswd[1] == 0){
  //       // if(rb_isempty(&cc2541->uart->rcv_rb) == 0){
  //         cc2541_read(cc2541);
  //         if(cc2541->rxBuff[0]){
  //           char* token = strtok(cc2541->rxBuff, ";");
  //           sprintf(ssid, "\"%s\"", token);
  //           token = strtok(NULL, "\n");
  //           sprintf(pswd, "\"%s\"", token);
  //           EMBARC_PRINTF("Connected: %s\n", cc2541->rxBuff);
  //         }
  //         // else{
  //         //   input[0] = 0;
  //         // }
  //         board_delay_ms(100, 1);
  //         // EMBARC_PRINTF("Expected\n");
  //         // if(input[0] == 's'){
  //         //   sscanf(input, "ssid:%s", ssid + 1);
  //         //   strcat(ssid, "\"");
  //         // }
  //         // else if(input[0] == 'p'){
  //         //   sscanf(input, "pswd:%s", pswd + 1);
  //         //   strcat(pswd, "\"");
  //         // }
  //       // }
  //       // EMBARC_PRINTF("Expected\n");
  //     }
  //     esp8266_wifi_connect(esp8266, ssid, pswd, true);
  //   }
  // }
  cc2541_set_connection_role(cc2541, 1);
  board_delay_ms(100, 1);
  /* End ESP8266 Connect Wifi */


	/* ESP8266 Connect Server */
  esp8266_httpRequest(esp8266);
  EMBARC_PRINTF("%s", esp8266->buff);
  char* ptr = esp8266->buff;
  while(*ptr != '\r'){
    while(*(ptr++) !='/');
    int index;
    if(*(ptr++) == '0'){
      switch(*(ptr++)){
        case '0':
          index = 0;
          break;
        case '1':
          index = 1;
          break;
        case '2':
          index = 2;
          break;
        case '3':
          index = 3;
          break;
        case '4':
          index = 4;
          break;
        case '5':
          index = 5;
          break;
      }
    }
    else {
      switch(*(ptr++)){
        case '0':
          index = 6;
          break;
        case '1':
          index = 7;
          break;
        case '2':
          index = 8;
          break;
        case '3':
          index = 9;
          break;
        case '4':
          index = 10;
          break;
        case '5':
          index = 11;
          break;
      }
    }
    EMBARC_PRINTF("HI%c\n", *(ptr));
    ptr++;
    strncpy(deviceTable[index].mac, ptr, 12);
    ptr += 12;
  }

  for(int i = 0; i < 12; i ++){
    EMBARC_PRINTF("%d %s\n", i, deviceTable[i].mac);
    deviceTable[i].state = REG;
  }
  memset(esp8266->buff, 0, 200);
  esp8266_reachMQTT(esp8266);
  esp8266_subArc(esp8266);
  /* End ESP8266 Connect Server */
  
  /* CC2541 Connect */
  cc2541_inq_dev(cc2541);
  ptr = cc2541->rxBuff;
  EMBARC_PRINTF("FIRST");
  while(strncmp(ptr, "+INQE", 5) != 0){
    if(strncmp(ptr, "OK", 2) == 0){
      ptr += 4;
    }
    else if(strncmp(ptr, "+INQS", 5) == 0){
      ptr += 7;
    }
    else if(strncmp(ptr, "+INQ:", 5) == 0){
      ptr += 5;
      char inqNum = *ptr;
      ptr += 4;
      char mac[13];
      strncpy(mac, ptr, 12);
      ptr += 17;
      // EMBARC_PRINTF("get%c %12s\n",inqNum, mac);
      for(int i = 0; i < 12; i ++){
        // EMBARC_PRINTF("%s", mac);
        if(strncmp(mac, deviceTable[i].mac, 12) == 0){
          deviceTable[i].inqNum = inqNum;
          deviceTable[i].state = SEARCHED;
        }
      }
    }
    else 
      ptr++;
  }
  for(int i = 0; i < 12; i ++){
    EMBARC_PRINTF("%d %c\n", i, deviceTable[i].inqNum);
  }
  df_playTrack(esp8266, 10);
  int nRet;
	printf("\r\nDSpotter version: %s\r\n", Spotter_GetVersion());

	nRet= Spotter_Init((void*)pCYBase_mod, (const void*[]){(void*)pGroup_1_mod, (void*)pGroup_2_mod, 0}, pMem, nMemSize);
	if(nRet!= 0){
		goto Exit;
	}
	Spotter_Reset();
	printf("Begin recognize\r\n");
  ez_sio_read(cc2541->uart, cc2541->rxBuff, 200);
  ez_sio_read(esp8266->uart, esp8266->buff, 200);
  memset(cc2541->rxBuff, 0, 200);
  memset(esp8266->buff, 0, 200);
  uint32_t startMS = OSP_GET_CUR_MS();
  initRecognize();
	while (1) {
    control_device();
		pData= Recorder_GetBuffer();
		if(pData== 0) continue;
		nRet = Spotter_AddSample(pData, Recorder_GetBufferSize());
		if(nRet== SPOTTER_SUCCESS){
			int gpID = Spotter_GetResultGroupId(),
          cmdID = Spotter_GetResultId();
      printf("Spotter detect result gpID=%d cmdID=%d\n", gpID, cmdID);
      /* First Stage Recognition */
      if(gpID == 0){
        recNum = FIRST_STAGE;
        spottedMS = OSP_GET_CUR_MS();
        df_playTrack(esp8266, (OSP_GET_CUR_US()%2+3));
        // ez_sio_write(esp8266, DFCommandTable[(OSP_GET_CUR_US()%2+4)], 11);
      }
      /* Second Stage Recognition */
      else if(gpID == 1 && recNum == FIRST_STAGE){
        switch(cmdID){
          case 0: 
            recNum = LIV_L_ON; 
            controlBuffer.index = 0;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 1: 
            recNum = LIV_L_OFF; 
            controlBuffer.index = 0;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;

          case 12: 
            recNum = LIV_AC_ON; 
            controlBuffer.index = 6;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 13: 
            recNum = LIV_AC_OFF; 
            controlBuffer.index = 6;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;
          
          case 2: 
            recNum = STU_L_ON; 
            controlBuffer.index = 1;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;          
          case 3: 
            recNum = STU_L_OFF;
            controlBuffer.index = 1;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;

          case 14: 
            recNum = STU_AC_ON;
            controlBuffer.index = 7;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 15: 
            recNum = STU_AC_OFF;
            controlBuffer.index = 7;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;

          case 4: 
            recNum = DOOR_L_ON;
            controlBuffer.index = 2;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 5: 
            recNum = DOOR_L_OFF;
            controlBuffer.index = 2;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;

          case 16: 
            recNum = DOOR_AC_ON; 
            controlBuffer.index = 8;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 17: 
            recNum = DOOR_AC_OFF;            
            controlBuffer.index = 8;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;

          case 6: 
            recNum = GATE_L_ON;
            controlBuffer.index = 3;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH; 
          break;
          case 7: 
            recNum = GATE_L_OFF; 
            controlBuffer.index = 3;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;
          
          case 18: 
            recNum = GATE_AC_ON; 
            controlBuffer.index = 9;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 19: 
            recNum = GATE_AC_OFF; 
            controlBuffer.index = 9;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;

          case 8: 
            recNum = KIT_L_ON;
            controlBuffer.index = 4;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 9: 
            recNum = KIT_L_OFF; 
            controlBuffer.index = 4;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;

          case 20: 
            recNum = KIT_AC_ON;
            controlBuffer.index = 10;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 21: 
            recNum = KIT_AC_OFF; 
            controlBuffer.index = 10;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;
          
          case 10: 
            recNum = DIN_L_ON;
            controlBuffer.index = 5;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 11: 
            recNum = DIN_L_OFF; 
            controlBuffer.index = 5;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;

          case 22: 
            recNum = DIN_AC_ON;
            controlBuffer.index = 11;
            strncpy(controlBuffer.command, "on", 2);
            controlBuffer.command_length = 2;
            controlBuffer.state = REACH;
          break;
          case 23: 
            recNum = DIN_AC_OFF;
            controlBuffer.index = 11;
            strncpy(controlBuffer.command, "off", 3);
            controlBuffer.command_length = 3;
            controlBuffer.state = REACH;
          break;
        }
        recNum=NO_DETECT;
      }
		}
    /* In First Stage and Timed Out */
    if(OSP_GET_CUR_MS() - spottedMS > RECORD_TIMEOUT && recNum == FIRST_STAGE){
      printf("Timed out\n");
      recNum = NO_DETECT;
    }
    handleMQTT();
    Recorder_NextBuffer();   
  }
  Recorder_Stop();
	Recorder_Release();
	printf("Done\r\n");
Exit:
 	Spotter_Release();
	return E_OK;
}