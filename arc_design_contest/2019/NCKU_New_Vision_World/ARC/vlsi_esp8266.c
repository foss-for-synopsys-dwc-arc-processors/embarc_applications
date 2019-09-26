#include "embARC.h"
#include "embARC_debug.h"
#include "stdlib.h"
#include "embARC_error.h"
#include "board.h"
#include "dev_uart.h"

#include "vlsi_esp8266.h"

char esp_rx_buf[256] = { 0 };

int esp_rx_flag;

static void rx_cb(void *ptr)
{
	DEV_UART *uart_obj = (DEV_UART *) ptr;
	int rd_avail = 0;
	int cnt = 0;

	uart_obj->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));

	while (rd_avail > 0) {
		cnt = (rd_avail < ESP_MAX_RX) ? rd_avail : (ESP_MAX_RX - 1);
		uart_obj->uart_read((void *)(esp_rx_buf + esp_rx_flag), cnt);
		esp_rx_flag += cnt;
		uart_obj->uart_control(UART_CMD_GET_RXAVAIL,
				       (void *)(&rd_avail));
	}
}

int vlsi_esp_waiting(VLSI_ESP *obj)
{
	int f = -2;

	while (f == -2) {
		for (int i = 0; i < 20; i++) {
			board_delay_ms(50, 1);

			if ((strstr(esp_rx_buf, "OK") != NULL)
			    || (strstr(esp_rx_buf, "ERROR") != NULL)) {
				if ((strstr(esp_rx_buf, "OK") != NULL))
					f = 0;	/** OK */
				else
					f = -1;	/** ERROR */

				for (int j = 0; j < 256; ++j)
					esp_rx_buf[j] = 0;

				esp_rx_flag = 0;
				return f;
			}
		}
		EMBARC_PRINTF("time out\r\n");
	}

	return f;
}

int vlsi_esp_uart_init(VLSI_ESP *obj, int baudrate)
{
	esp_rx_flag = 0;

	obj->uart = uart_get_dev(1);	/** UART1 in ARDUINO */
	int32_t ercd = obj->uart->uart_open(baudrate);
	if (ercd != E_OK && ercd != E_OPNED) {
		EMBARC_PRINTF("ESP8266 UART INIT ERROR\r\n");
		return -1;
	}

	EMBARC_PRINTF("ESP8266 UART INIT OK\r\n");

	obj->uart->uart_control(UART_CMD_SET_RXCB, (void *)rx_cb);
	obj->uart->uart_control(UART_CMD_SET_RXINT, 1);

	for (int i = 0; i < 256; ++i)
		obj->rx.esp_rx_buf[i] = 0;

	obj->rx.esp_rx_index = 0;

	DEV_UART_INFO_SET_EXTRA_OBJECT(&(obj->uart->uart_info), &(obj->rx));

	return 0;
}

int vlsi_esp_test(VLSI_ESP *obj)
{
	int f = -2;

	while (f == -2) {
		obj->uart->uart_write("AT\r\n", 4);

		for (int i = 0; i < 20; i++) {
			board_delay_ms(50, 1);

			if ((strstr(esp_rx_buf, "OK") != NULL)
			    || (strstr(esp_rx_buf, "ERROR") != NULL)) {
				if ((strstr(esp_rx_buf, "OK") != NULL)) {
					f = 0;	/** OK */
					EMBARC_PRINTF("OK\r\n");
				} else {
					f = -1;	/** ERROR */
					EMBARC_PRINTF("ERROR\r\n");
				}
				EMBARC_PRINTF("--------------------------\r\n");
				EMBARC_PRINTF("reply rxbuf: %s\r\n",
					      esp_rx_buf);
				EMBARC_PRINTF("--------------------------\r\n");

				for (int j = 0; j < 256; ++j)
					esp_rx_buf[j] = 0;

				esp_rx_flag = 0;
				return f;
			}
		}
		EMBARC_PRINTF("time out\r\n");
	}

	return f;
}

int vlsi_esp_send_uint(VLSI_ESP *obj, unsigned int data_u32)
{
	unsigned char data_u8[4];
	data_u8[0] = (unsigned char)(data_u32 % 256);
	data_u8[1] = (unsigned char)((data_u32 / 256) % 256);
	data_u8[2] = (unsigned char)((data_u32 / 65536) % 256);
	data_u8[3] = (unsigned char)((data_u32 / 16777216) % 256);

	int f = -2;
	char cmd[] = "AT+CIPSEND=0,4\r\n";	/** send data cmd and wait */
	obj->uart->uart_write(cmd, sizeof(cmd));

	for (int i = 0; i < 100; i++) {
		board_delay_ms(50, 1);
		if (strstr(esp_rx_buf, "ERROR") != NULL) {
			EMBARC_PRINTF("ERROR\r\n");

			EMBARC_PRINTF("--------------------------\r\n");
			EMBARC_PRINTF("reply rxbuf: %s\r\n", esp_rx_buf);
			EMBARC_PRINTF("--------------------------\r\n");

			for (int j = 0; j < 256; ++j)
				esp_rx_buf[j] = 0;

			esp_rx_flag = 0;
			return -1;
		} else if (strstr(esp_rx_buf, "OK") != NULL) {
			for (int j = 0; j < 256; ++j)
				esp_rx_buf[j] = 0;

			esp_rx_flag = 0;
			break;
		} else if (i == 99) {
			EMBARC_PRINTF("time out and break\r\n");

			EMBARC_PRINTF("--------------------------\r\n");
			EMBARC_PRINTF("reply rxbuf: %s\r\n", esp_rx_buf);
			EMBARC_PRINTF("--------------------------\r\n");
			return -1;
		}
	}

	obj->uart->uart_write(data_u8, 4);

	return f;
}

int vlsi_esp_mode_set(VLSI_ESP *obj)
{
	int f;

    /** AP+Station mode */
	char cmd[] = "AT+CWMODE_CUR=3\r\n";
	obj->uart->uart_write(cmd, sizeof(cmd));

	f = vlsi_esp_waiting(obj);

	return f;
}

int vlsi_esp_connect(VLSI_ESP *obj)
{
	int f;

    /** AT+CWJAP_CUR="ssid","key"\r\n */
	char cmd[] = "AT+CWJAP=\"DiamondFish\",\"fishfishfish\"\r\n";
	obj->uart->uart_write(cmd, sizeof(cmd));

	f = vlsi_esp_waiting(obj);

	return f;
}

int vlsi_esp8266_multi(VLSI_ESP *obj)
{
	int f;

    /** multi connect */
	char cmd[] = "AT+CIPMUX=1\r\n";
	obj->uart->uart_write(cmd, sizeof(cmd));

	f = vlsi_esp_waiting(obj);

	return f;
}

int vlsi_esp8266_server(VLSI_ESP *obj)
{
	int f;

    /** build server with port 9999 */
	char cmd[] = "AT+CIPSERVER=1,9999\r\n";
	obj->uart->uart_write(cmd, sizeof(cmd));

	f = vlsi_esp_waiting(obj);

	return f;
}

int vlsi_esp8266_get_ip(VLSI_ESP *obj)
{
	int f = -2;

    /** get IP address */
	char cmd[] = "AT+CIFSR\r\n";
	obj->uart->uart_write(cmd, sizeof(cmd));

	while (f == -2) {
		for (int i = 0; i < 20; i++) {
			board_delay_ms(50, 1);

			if ((strstr(esp_rx_buf, "OK") != NULL)
			    || (strstr(esp_rx_buf, "ERROR") != NULL)) {
				EMBARC_PRINTF("break\r\n");

				EMBARC_PRINTF("--------------------------\r\n");
				EMBARC_PRINTF("reply rxbuf: %s\r\n",
					      esp_rx_buf);
				EMBARC_PRINTF("--------------------------\r\n");

				if ((strstr(esp_rx_buf, "OK") != NULL))
					f = 0;	/** OK */
				else
					f = -1;	/** ERROR */

				for (int j = 0; j < 256; ++j)
					esp_rx_buf[j] = 0;

				esp_rx_flag = 0;
				return f;
			}
		}
		EMBARC_PRINTF("time out\r\n");
	}

	return f;
}

int vlsi_esp8266_link(VLSI_ESP *obj)
{
	int f = -2;

	while (f == -2) {
		for (int i = 0; i < 20; i++) {
			board_delay_ms(50, 1);

			if (strstr(esp_rx_buf, "Link") != NULL) {
				EMBARC_PRINTF("Link\r\n");

				EMBARC_PRINTF("--------------------------\r\n");
				EMBARC_PRINTF("reply rxbuf: %s\r\n",
					      esp_rx_buf);
				EMBARC_PRINTF("--------------------------\r\n");

				f = 0;

				for (int j = 0; j < 256; ++j)
					esp_rx_buf[j] = 0;

				esp_rx_flag = 0;
				return f;
			}
		}
		EMBARC_PRINTF("time out\r\n");
	}

	return f;
}
