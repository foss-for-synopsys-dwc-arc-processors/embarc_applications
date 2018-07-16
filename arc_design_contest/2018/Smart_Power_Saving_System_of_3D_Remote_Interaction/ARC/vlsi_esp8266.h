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

/** \brief esp8266 recieve buffer, the communication port is UART */
typedef struct uart_buf
{
  char rx_buf[1000];
  int rx_index;
} UART_BUF;

/** \brief esp8266 device object */
typedef struct vlsi_esp
{
  DEV_UART *uart;
  UART_BUF rx;
} VLSI_ESP;

/** \brief esp8266 initial
*          Register the uart device, and initial the device
*   \param baudrate: esp8266 baudrate, suggest 115200
*/
int vlsi_esp8266_init(VLSI_ESP *obj, int baudrate);


/** \brief esp8266 read
*          read the esp8266 read buffer, and copy the value from the buffer.
*   \param buf: the input buffer.
*   \param cnt: the length you want to copy.
*/
int vlsi_esp8266_read(VLSI_ESP *obj, char *buf, int cnt);


/** \brief esp8266 get repley
*          This should be private function. Wait for the ESP8266 repley OK or ERROR. Not suggest to use outside of esp8266 driver.
*/
int vlsi_esp8266_get_repley(VLSI_ESP *obj, char* buf, int time_out);


/** \brief esp8266 test
*          Test function. Sent to AT command for test. If the ESP8266 not replry, try to fix.
*/
int vlsi_esp8266_test(VLSI_ESP *obj);


/** \brief esp8266 get ip
*          Print the current IP address informations
*/
int vlsi_esp8266_get_ip(VLSI_ESP *obj);


/** \brief esp8266 mode set
*          We use this to set our esp8266 as tcp socket server.
*/
int vlsi_esp8266_mode_set(VLSI_ESP *obj);


/** \brief esp8266 connect
*          Connect to the AP by given it ssid and key.
*     This function still not very robust, make sure the ssid and key in this function are correct.
*/
int vlsi_esp8266_connect(VLSI_ESP *obj, char* ssid, char* key);


/** \brief esp8266 server
*          We use this to set our esp8266 as tcp socket server.
*/
int vlsi_esp8266_server(VLSI_ESP *obj);


/** \brief esp8266 sent
*          Sent a fixed size of buffer to the specified idp.
*/
int vlsi_esp8266_send(VLSI_ESP *obj, char *buf, char idp);


/** \brief esp8266 close
*          Close the specified idp connection.
*/
int vlsi_esp8266_close(VLSI_ESP *obj, char idp);


/** \brief esp8266 sent pose
*          Sent a fixed size of buffer to the specified idp.
*/
int vlsi_esp8266_send_pose(VLSI_ESP *obj, char *buf, char idp);


/** \brief esp8266 sent data
*          Sent a dynamic size of buffer to the specified idp. The size shoud specified in cmd and buf_size.
*/
int vlsi_esp8266_send_datas(VLSI_ESP *obj, char* cmd, int cmd_size, char* buf, int buf_size);

#endif
