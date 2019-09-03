#ifndef VLSI_ESP8266_H
#define VLSI_ESP8266_H

#include "dev_uart.h"

#define ESP_MAX_RX         200

/** \brief ESP8266 recieve buffer, the communication port is UART */
typedef struct esp_buf {
	char esp_rx_buf[256];
	int esp_rx_index;
} ESP_BUF;

/** \brief ESP8266 device object */
typedef struct vlsi_esp {
	DEV_UART *uart;
	ESP_BUF rx;
} VLSI_ESP;

/** \brief esp8266 uart initial
*          Register the uart device, and initial the device
*   \param baudrate: esp8266 baudrate, suggest 115200
*/
int vlsi_esp_uart_init(VLSI_ESP *obj, int baudrate);

/** \brief esp8266 test
*          Test function. Sent to command for test. If the esp8266 not reply, try to fix.
*/
int vlsi_esp_test(VLSI_ESP *obj);

/** \brief esp8266 send unsigned integer
*          Send data. Data Type int -> char .
*   \param data_u32: ESP8266 data, unsigned integer
*/
int vlsi_esp_send_uint(VLSI_ESP *obj, unsigned int data_u32);

/** \brief esp8266 mode set
*          Set ESP8266 AP+Station mode.
*/
int vlsi_esp_mode_set(VLSI_ESP *obj);

/** \brief esp8266 connect WIFI
*          Connect ESP8266 to the AP by given it ssid and key.
*/
int vlsi_esp_connect(VLSI_ESP *obj);

/** \brief esp8266 multi-connect
*          Connect ESP8266 with multi-user.
*/
int vlsi_esp8266_multi(VLSI_ESP *obj);

/** \brief esp8266 server
*          Build ESP8266 server with port 80.
*/
int vlsi_esp8266_server(VLSI_ESP *obj);

/** \brief esp8266 get ip
*          Print the current IP address informations
*/
int vlsi_esp8266_get_ip(VLSI_ESP *obj);

/** \brief esp8266 link
*          Waiting the client link
*/
int vlsi_esp8266_link(VLSI_ESP *obj);

#endif
