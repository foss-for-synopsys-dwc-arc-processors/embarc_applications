#ifndef _ESP8266_GAGENT_USER_H_
#define _ESP8266_GAGENT_USER_H_

#include "dev_uart.h"
#include "DS18B20.h"
#include "esp8266_gagent_common.h"


DEV_GPIO_PTR     ds18b20_port_ptr;
DS18B20_DEF_PTR  ds18b20_data;
#define DS18B20_PORT DW_GPIO_PORT_C
#define DS18B20_BIT  15

#define MAX_READ_CNT 32


// data point
uint8_t Aquarius_Oxy_Freq;    
uint8_t Aquarius_Feed_Freq;   
uint8_t Aquarius_Temp;        
uint8_t Aquarius_Feed_Amount; 
uint8_t Aquarius_Light;       


#define ATTR_FLAGS_BYTES  (1)

#define ATTR_VALS_BYTES   (2)
extern void mcu2wifi_wifi_ctrl_dev(DEV_UART_PTR esp8266_uart,uint8_t *receive_data);


#define DEV_STATUS_BYTES (3)
extern void mcu2wifi_wifi_read_dev(DEV_UART_PTR esp8266_uart);

extern void mcu2wifi_dev_report_status(DEV_UART_PTR esp8266_uart);

#endif