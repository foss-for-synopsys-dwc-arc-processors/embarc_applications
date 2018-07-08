#ifndef _GP2Y1051_H
#define _GP2Y1051_H

#include "dev_uart.h"
/**************定义GP2Y1051的接口**************/
//串口
//GP2Y1051的TX接口----J5-pin3
//GP2Y1051的RX接口----J5-pin2
DEV_UART_PTR GP2Y1051_uart;
#define GP2Y1051_UART_ID DW_UART_2_ID
#define GP2Y1051_UART_BAUDRATE UART_BAUDRATE_2400

extern uint16_t GP2Y1051_read(void);

#endif