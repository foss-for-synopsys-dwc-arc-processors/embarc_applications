#ifndef _GP2Y1051_H
#define _GP2Y1051_H

#include "dev_uart.h"

//
DEV_UART_PTR GP2Y1051_uart;
#define GP2Y1051_UART_ID DW_UART_2_ID
#define GP2Y1051_UART_BAUDRATE UART_BAUDRATE_2400

extern uint16_t GP2Y1051_read(void);

#endif