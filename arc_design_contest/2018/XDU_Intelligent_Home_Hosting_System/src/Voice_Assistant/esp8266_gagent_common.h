#ifndef _ESP8266_GAGENT_COMMON_H_
#define _ESP8266_GAGENT_COMMON_H_

#include "dev_uart.h"

uint8_t sn;
uint16_t year;
uint8_t month;
uint8_t day;
uint8_t hour;
uint8_t minute;
uint8_t second;


DEV_GPIO_PTR led;
#define LED_PORT DW_GPIO_PORT_B
#define LED_MASK 0x1ff
#define LED_DIR  0x1ff



extern void mcu2wifi_product_info(DEV_UART_PTR esp8266_uart, uint8_t *product_key, uint8_t *product_secret);

extern void mcu2wifi_heartbeat(DEV_UART_PTR esp8266_uart);


extern void mcu2wifi_set_cfg_mode(DEV_UART_PTR esp8266_uart,uint8_t mode);


extern void mcu2wifi_reset_wifi(DEV_UART_PTR esp8266_uart);


extern void mcu2wifi_wifi_statu(DEV_UART_PTR esp8266_uart,uint8_t *receive_data);


extern void mcu2wifi_receive_error(DEV_UART_PTR esp8266_uart);
/
extern void mcu2wifi_request_time(DEV_UART_PTR esp8266_uart);

extern void wifi2mcu_get_time(uint8_t *receive_data);

extern void mcu2wifi_restart_wifi(DEV_UART_PTR esp8266_uart);

extern void mcu2wifi_request_into_binding(DEV_UART_PTR esp8266_uart);

#endif