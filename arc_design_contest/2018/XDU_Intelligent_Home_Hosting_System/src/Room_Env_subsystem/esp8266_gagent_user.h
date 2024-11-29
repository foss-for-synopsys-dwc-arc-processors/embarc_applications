#ifndef _ESP8266_GAGENT_USER_H_
#define _ESP8266_GAGENT_USER_H_

#include "dev_uart.h"

#include "esp8266_gagent_common.h"

DEV_GPIO_PTR led;
#define LED_PORT DW_GPIO_PORT_B
#define LED_MASK 0x1ff
#define LED_DIR  0x1ff


#define MAX_READ_CNT 32


// 
uint8_t Room_Env_Tepm;   
uint8_t Room_Env_Humidity;  
uint8_t Room_Smoke;       
uint8_t Room_Light;    
// uint8_t Room_curtain;   
uint8_t Room_Lamp;   
uint16_t Room_PM25;    
uint8_t Body_infrared; 
uint8_t Leave_home;    
uint8_t Fan_switch;    






#define ATTR_FLAGS_BYTES  (1)
// 
#define ATTR_VALS_BYTES   (1)
extern void mcu2wifi_wifi_ctrl_dev(DEV_UART_PTR esp8266_uart,uint8_t *receive_data);


#define DEV_STATUS_BYTES (8)
extern void mcu2wifi_wifi_read_dev(DEV_UART_PTR esp8266_uart);


extern void mcu2wifi_dev_report_status(DEV_UART_PTR esp8266_uart);

#endif