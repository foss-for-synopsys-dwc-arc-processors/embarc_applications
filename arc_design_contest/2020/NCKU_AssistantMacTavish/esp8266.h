#ifndef _ESP8266_H
#define _ESP8266_H

#include "embARC_debug.h"
#include "embARC.h"
#include "board.h"
#include "ez_sio.h"

/* ESP8266 Definitions */
#define CHECK_CONN          "getCON\n"
#define CHECK_CONN_NUM      (7)
#define SET_SSID            "ssid:"
#define SET_PSWD            "pswd:"
#define SET_WIFI            "setCON\n"
#define SET_WIFI_NUM        (7)
#define UPDATE_HTTP_REQ     "init:http://192.168.137.1:5000/all\n"
#define UPDATE_HTTP_REQ_NUM (35)
#define REACH_MQTT_REQ      "reachMQTT:192.168.137.1\n"
#define REACH_MQTT_REQ_NUM  (24)
#define SUB_REQ             "sub:arc\n"
#define SUB_REQ_NUM         (8)
#define ESP8266_MAX_CONN_TRIAL  3
/* End ESP8266 Definitions */

typedef enum{
  NO_REPLY = -1,
  NO_CONN,
  NO_MQTT,
  CONN,
} CONN_STATE_DEF;

typedef struct{
  EZ_SIO* uart;
  CONN_STATE_DEF connState;
  char* buff;
} esp8266_def;

esp8266_def* esp8266_init(uint32_t uartId, uint32_t uartBaudRate, uint32_t txBuffSize, uint32_t rxBuffSize);
void esp8266_getConnState(esp8266_def* esp8266);
void esp8266_setSSID(esp8266_def* esp8266, char* ssid);
void esp8266_setPSWD(esp8266_def* esp8266, char* pswd);
void esp8266_setConnection(esp8266_def* esp8266);
void esp8266_httpRequest(esp8266_def* esp8266);
void esp8266_reachMQTT(esp8266_def* esp8266);
void esp8266_subArc(esp8266_def* esp8266);
uint32_t esp8266_write(esp8266_def* esp8266, char* buff, uint32_t count);

void df_init(esp8266_def* esp8266);
void df_setVolume(esp8266_def* esp8266);
void df_playTrack(esp8266_def* esp8266, uint32_t trackNum);

#endif