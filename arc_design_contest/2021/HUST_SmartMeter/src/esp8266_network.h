#ifndef __ESP_NETWORK_H
#define __ESP_NETWORK_H

#include "my_uart.h"




/*连接服务器宏定义*/
#define TCP "TCP"
#define UDP "UDP"
#define IP  "122.114.122.174"
#define PORT 40915

// #define Net_SendString(str) Uart_SendString(str)
// #define Net_SendBuf(buf,len) Uart_SendBuf(buf,len)

typedef struct
{
	uint8_t *rxbuf;uint16_t rxlen;
	uint8_t *txbuf;uint16_t txlen;
	
	uint8_t (*Check)(void);
	uint8_t (*Init)(uint8_t *prx,uint16_t rxlen,uint8_t *ptx,uint16_t txlen);
	void (*Restore)(void);
	uint8_t (*ConnectAP)(char *ssid,char *pswd);
	uint8_t (*ConnectServer)(char* mode,char *ip,uint16_t port);
	uint8_t (*DisconnectServer)(void);
	uint8_t (*OpenTransmission)(void);
	void (*CloseTransmission)(void);		
	void (*SendString)(char *str);
	void (*SendBuf)(char *buf,uint32_t len);
}_typdef_net;

extern _typdef_net _net;

#endif
