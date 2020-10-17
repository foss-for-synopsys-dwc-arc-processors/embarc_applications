#ifndef _HC_06_H_
#define _HC_06_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

#include "board.h"
#include "dev_common.h"
#include "delay.h"

#include "stdio.h"
#include "string.h"


#define HC_06_UART_ID   DW_UART_2_ID

/*
 * J5.12 3V3
 * J5.11 GND
 * J5.9  UART2_RXD
 * J5.8  UART2_TXD
 */

// gesture definition
#define GES_UP              0x01
#define GES_DOWN            0x02
#define GES_LEFT            0x03
#define GES_RIGHT           0x04
#define GES_CLOCKWISE       0x05
#define GES_ANTI_CLOCKWISE  0x06
#define GES_FORWARD         0x07
#define GES_BACKWARD        0x08
#define GES_WAVE            0x09

uint32_t UART2_init(uint32_t baudrate);
uint32_t UART2_write(uint8_t *buf, uint32_t cnt);
uint32_t UART2_read(uint8_t *buf, uint32_t cnt);
uint8_t  get_ges_cmd(void);

#endif

