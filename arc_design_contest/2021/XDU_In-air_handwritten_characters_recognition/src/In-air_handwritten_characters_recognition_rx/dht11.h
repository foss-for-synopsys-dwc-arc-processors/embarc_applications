#ifndef _DHT11_H_
#define _DHT11_H_


#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

#include "board.h"
#include "dev_common.h"
// #include "delay.h"
#include "dfss_gpio_obj.h"


// DHT11 definition
// J3.4
#define DHT11_PORT_ID       DFSS_GPIO_4B1_ID
#define DHT11_PIN_DAT       3


// DHT11 function declaration
void     dht11_init(void);
uint32_t dht11_bit_out(uint32_t num, uint32_t bit);
uint32_t dht11_bit_in(uint32_t num);
uint8_t  dht11_read_byte(void);
void     dht11_read_data(uint8_t *dht11_data, uint8_t *dht11_hex);

#endif
