#include "my_uart.h"

uint8_t temp_rx;

uint8_t  UART_TX_BUF[UART_MAX_SEND_LEN];
uint8_t  UART_RX_BUF[UART_MAX_RECV_LEN]; 		//接收缓冲,最大USART3_MAX_RECV_LEN字节
uint16_t UART_RX_STA=0;

DEV_UART *uart_obj;


static void uart_rx_callback(void *ptr){

	DEV_UART *obj;

	if (ptr == NULL) return;
	obj = (DEV_UART *)ptr;
      uint32_t rd_avail;
	  obj->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
	  while(rd_avail){
		  obj->uart_read((void *)&temp_rx, rd_avail);
		  UART_RX_BUF[UART_RX_STA++]=temp_rx;
		  //EMBARC_PRINTF("%c",temp_rx);
		  obj->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
	  }    
}


void UART_Init(uint8_t uart_id,uint32_t baudrate){
  uart_obj = uart_get_dev(uart_id);
  int32_t ercd = uart_obj->uart_open(baudrate);
	if ((ercd != E_OK) && (ercd != E_OPNED))  return ;
    if (ercd == E_OPNED) {
		ercd = uart_obj->uart_control(UART_CMD_SET_BAUD, (void *)(baudrate));
		if (ercd != E_OK)  return ;
	}
	uart_obj->uart_control(UART_CMD_SET_TXINT, (void *)(DEV_DISABLED));
	uart_obj->uart_control(UART_CMD_SET_RXINT, (void *)(DEV_DISABLED));
	uart_obj->uart_control(UART_CMD_SET_RXCB, (void *)uart_rx_callback);
	uart_obj->uart_control(UART_CMD_SET_RXINT, (void *)(DEV_ENABLED));

}


void u1_printf(char* fmt,...){  
	uint8_t i; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)UART_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)UART_TX_BUF);		
  uart_obj->uart_write(UART_TX_BUF,i);
}

void Uart_SendString(char* buf){
	u1_printf("%s",buf);
}
void Uart_SendBuf(char* buf,uint32_t size){
	uart_obj->uart_write(buf,size);
}


uint8_t FindStr(char* dest,char* src,uint16_t retry_nms){
    retry_nms/=10;                   //超时时间

    while(strstr(dest,src)==0 && retry_nms--)//等待串口接收完毕或超时退出
    {		
		board_delay_ms(20,1);
    }
	
   if(retry_nms)     return 1;                       

   return 0; 
}
