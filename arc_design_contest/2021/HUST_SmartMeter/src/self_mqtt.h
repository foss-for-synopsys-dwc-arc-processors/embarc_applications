#ifndef __SELF_MQTT_H_
#define __SELF_MQTT_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "esp8266_network.h"



typedef struct
{
	uint8_t *rxbuf;uint16_t rxlen;
	uint8_t *txbuf;uint16_t txlen;
	void (*Init)(uint8_t *prx,uint16_t rxlen,uint8_t *ptx,uint16_t txlen);
	uint8_t (*Connect)(char *ClientID,char *Username,char *Password);
	uint8_t (*SubscribeTopic)(char *topic,uint8_t qos,uint8_t whether);
	uint8_t (*PublishData)(char *topic, char *message, uint8_t qos);
	void (*SendHeart)(void);
	void (*Disconnect)(void);
}_typdef_mqtt;

extern _typdef_mqtt _mqtt;

#endif
