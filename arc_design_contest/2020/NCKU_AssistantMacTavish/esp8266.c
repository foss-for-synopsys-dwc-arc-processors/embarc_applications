#include "esp8266.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "ez_sio.h"
#include "stdlib.h"

/* DFPlayer Command Define */
const uint8_t DFCommandTable[12][11] = {
  {':', 0x7e, 0xff, 0x6, 0xc, 0x1, 0x0, 0x0, 0xfe, 0xee, 0xef},  // Initializing
  {':', 0x7e, 0xff, 0x6, 0x6, 0x1, 0x0, 0x14, 0xfe, 0xe0, 0xef}, // Setting Volume
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0x1, 0xfe, 0xf6, 0xef},  // Play First Track
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0x2, 0xfe, 0xf5, 0xef},  // Play Second Track
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0x3, 0xfe, 0xf4, 0xef},  // Play Second Track
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0x4, 0xfe, 0xf3, 0xef},  // Play Second Track
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0x5, 0xfe, 0xf2, 0xef},  // Play Second Track
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0x6, 0xfe, 0xf1, 0xef},  // Play Second Track
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0x7, 0xfe, 0xf0, 0xef},  // Play Second Track
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0x8, 0xfe, 0xef, 0xef},  // Play Second Track 
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0x9, 0xfe, 0xee, 0xef},  // Play Second Track   
  {':', 0x7e, 0xff, 0x6, 0x3, 0x1, 0x0, 0xa, 0xfe, 0xed, 0xef},  // Play Second Track  
};
/* End DFPlayer Command Define */

esp8266_def* esp8266_init(uint32_t uartId, uint32_t uartBaudRate, uint32_t txBuffSize, uint32_t rxBuffSize){
  esp8266_def* esp8266 = malloc(sizeof(esp8266_def));
  esp8266->uart = ez_sio_open(uartId, uartBaudRate, txBuffSize, rxBuffSize);
  esp8266->buff = malloc(100*sizeof(char));
  esp8266->connState = NO_REPLY;
  while(esp8266->connState == NO_REPLY){
    esp8266_getConnState(esp8266);
    board_delay_ms(1000, 1);
  }
  return esp8266;
}

void esp8266_getConnState(esp8266_def* esp8266){
  ez_sio_read(esp8266->uart, esp8266->buff, 200);
  ez_sio_write(esp8266->uart, CHECK_CONN, CHECK_CONN_NUM);
  board_delay_ms(100, 1);
  memset(esp8266->buff, 0, 100);
  ez_sio_read(esp8266->uart, esp8266->buff, 2);
  if(esp8266->buff[0] == 0){
    esp8266->connState = NO_REPLY;
  }
  else if(esp8266->buff[0] == '0'){
    esp8266->connState = NO_CONN;
  }
  else if(esp8266->buff[0] == '1'){
    esp8266->connState = NO_MQTT;
  }
  else if(esp8266->buff[0] == '2'){
    esp8266->connState = CONN;
  }
  EMBARC_PRINTF("%d", esp8266->buff[0]);
}

void esp8266_setSSID(esp8266_def* esp8266, char* ssid){
  memset(esp8266->buff, 0, 100);
  sprintf(esp8266->buff, "%s\n", ssid);
  ez_sio_write(esp8266->uart, esp8266->buff, sizeof(ssid) + 1);
  EMBARC_PRINTF("%s %d", esp8266->buff, sizeof(ssid) + 1);
  board_delay_ms(1000, 1);
}

void esp8266_setPSWD(esp8266_def* esp8266, char* pswd){
  memset(esp8266->buff, 0, 100);
  sprintf(esp8266->buff, "%s\n", pswd);
  ez_sio_write(esp8266->uart, esp8266->buff, sizeof(pswd) + 1);
  EMBARC_PRINTF("%s %d", esp8266->buff, sizeof(pswd) + 1);
  board_delay_ms(1000, 1);
}

void esp8266_setConnection(esp8266_def* esp8266){
  ez_sio_write(esp8266->uart, SET_WIFI, SET_WIFI_NUM);
  board_delay_ms(1000, 1);
}

void esp8266_httpRequest(esp8266_def* esp8266){
  ez_sio_read(esp8266->uart, esp8266->buff, 200);
  memset(esp8266->buff, 0, 200);
  ez_sio_write(esp8266->uart, UPDATE_HTTP_REQ, UPDATE_HTTP_REQ_NUM);
  while(rb_isempty(&esp8266->uart->rcv_rb));
  board_delay_ms(1000, 1);
  ez_sio_read(esp8266->uart, esp8266->buff, 200);
}

void esp8266_reachMQTT(esp8266_def* esp8266){
  ez_sio_write(esp8266->uart, REACH_MQTT_REQ, REACH_MQTT_REQ_NUM);
  board_delay_ms(500, 1);
}

void esp8266_subArc(esp8266_def* esp8266){
  ez_sio_write(esp8266->uart, SUB_REQ, SUB_REQ_NUM);
  board_delay_ms(100, 1);
}

void df_init(esp8266_def* esp8266){
  ez_sio_write(esp8266->uart, DFCommandTable[0], 11);
  board_delay_ms(200, 1);
  ez_sio_read(esp8266->uart, esp8266->buff, 100);
}

void df_setVolume(esp8266_def* esp8266){
  ez_sio_write(esp8266->uart, DFCommandTable[1], 11);
  board_delay_ms(1000, 1);
  ez_sio_read(esp8266->uart, esp8266->buff, 100);
}

void df_playTrack(esp8266_def* esp8266, uint32_t trackNum){
  ez_sio_read(esp8266->uart, esp8266->buff, 100);
  ez_sio_write(esp8266->uart, DFCommandTable[trackNum + 1], 11);
}