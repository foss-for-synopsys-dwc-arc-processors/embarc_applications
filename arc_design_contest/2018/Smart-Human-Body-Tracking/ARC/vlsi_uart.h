#ifndef DF_UART_H
#include "dev_uart.h"
#define DF_UART_H
#define sleep(tick) board_delay_ms(tick, OSP_DELAY_OS_COMPAT_ENABLE) 
#define printf EMBARC_PRINTF
// #define USE_BAUD  (115200)
#define USE_BAUD  (230400)
// #define MAX_UART_BUF_SIZE  (105*4)
#define MAX_UART_BUF_SIZE  (252-1)

extern float pose[105];
extern float T[3*15];
extern float R[4*15 + 3];
extern float* sent_R;

DEV_UART* uart_obj;

void uart_cb(DEV_UART* uart_obj, void* ptr);

void clear_rx_buf(DEV_UART* uart_obj);

#endif
