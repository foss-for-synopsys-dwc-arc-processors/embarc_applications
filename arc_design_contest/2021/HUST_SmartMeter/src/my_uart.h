#ifndef __myuart_H
#define __myuart_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "dev_uart.h"


extern DEV_UART *uart_obj;


#define BYTE0(dwTemp)       (*( char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))


#define UART_MAX_RECV_LEN		2048					//最大接收缓存字节数
#define UART_MAX_SEND_LEN		2048

void UART_Init(uint8_t uart_id,uint32_t baudrate);

extern uint8_t  UART_RX_BUF[UART_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
extern uint8_t  UART_TX_BUF[UART_MAX_SEND_LEN];
extern uint16_t UART_RX_STA;   						//接收数据状态
extern uint8_t temp_rx;


void u1_printf(char* fmt, ...);
uint8_t FindStr(char* dest,char* src,uint16_t retry_nms);
void Uart_SendBuf(char* buf,uint32_t size);
void Uart_SendString(char* buf);

#ifdef __cplusplus
}
#endif
#endif 
