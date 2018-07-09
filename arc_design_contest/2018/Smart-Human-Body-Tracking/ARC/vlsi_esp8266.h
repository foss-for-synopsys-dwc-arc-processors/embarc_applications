#ifndef ESP8266_H
#define ESP8266_H
#include "dev_uart.h"
#include "ringbuffer.h"

#define AT_OK 			0
#define AT_ERROR 		-1
#define AT_NOREPLEY -2
#define AT_OK_STR		"OK"
#define AT_ERROR_STR	"ERROR"
#define UART_MAX_BUF_SIZE 1024
#define UART_MAX_RX 100

typedef struct uart_buf
{
  char rx_buf[1000];
  int rx_index;
} UART_BUF;

typedef struct vlsi_esp
{
  DEV_UART *uart;
  UART_BUF rx;
} VLSI_ESP;


int vlsi_esp8266_init(VLSI_ESP *obj, int baudrate);

int vlsi_esp8266_read(VLSI_ESP *obj, char *buf, int cnt);

int vlsi_esp8266_get_repley(VLSI_ESP *obj, char* buf, int time_out);

// int vlsi_esp8266_set_baud(VLSI_ESP *obj, int baud);

int vlsi_esp8266_test(VLSI_ESP *obj);

int vlsi_esp8266_get_ip(VLSI_ESP *obj);

int vlsi_esp8266_mode_set(VLSI_ESP *obj);

int vlsi_esp8266_connect(VLSI_ESP *obj, char* ssid, char* key);

int vlsi_esp8266_server(VLSI_ESP *obj);

int vlsi_esp8266_send(VLSI_ESP *obj, char *buf, char idp);

int vlsi_esp8266_close(VLSI_ESP *obj, char idp);

int vlsi_esp8266_send_pose(VLSI_ESP *obj, char *buf, char idp);

int vlsi_esp8266_send_datas(VLSI_ESP *obj, char* cmd, int cmd_size, char* buf, int buf_size);

#endif
