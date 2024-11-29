#ifndef _ESP8266_GAGENT_USER_H_
#define _ESP8266_GAGENT_USER_H_

#include "dev_uart.h"

#include "esp8266_gagent_common.h"




#define MAX_READ_CNT 32


// 
uint8_t Plant_Env_Temp;       
uint8_t Plant_Env_Humidity;   
uint8_t Plant_Soil_Moisture;  
uint8_t Plant_Watering;       
uint8_t WaterThreshold;       





#define ATTR_FLAGS_BYTES  (1)

#define ATTR_VALS_BYTES   (2)
extern void mcu2wifi_wifi_ctrl_dev(DEV_UART_PTR esp8266_uart,uint8_t *receive_data);


#define DEV_STATUS_BYTES (6)
extern void mcu2wifi_wifi_read_dev(DEV_UART_PTR esp8266_uart);


extern void mcu2wifi_dev_report_status(DEV_UART_PTR esp8266_uart);

#endif