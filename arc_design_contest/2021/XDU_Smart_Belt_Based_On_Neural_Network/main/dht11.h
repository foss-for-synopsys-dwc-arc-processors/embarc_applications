#ifndef _DHT11_H_
#define _DHT11_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "delay.h"

#include <stdio.h>
// #include "board.h"
// #include "dev_common.h"


// DHT11 definition
#define DHT11_PORT_ID       DFSS_GPIO_8B0_ID
#define DHT11_PIN_DAT       4

// DHT11 function declaration
void     dht11_init(void);
uint32_t dht11_bit_out(uint32_t num, uint32_t bit);
uint32_t dht11_bit_in(uint32_t num);
uint8_t  dht11_read_byte(void);
uint8_t  dht11_read_data(uint8_t *dht11_data);
uint8_t dht11_read_pre(uint8_t *dht11_data);

#endif
