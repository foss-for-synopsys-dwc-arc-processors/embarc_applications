/**
*Copyright: Copyright (c) 2019
*Created on 2019-8-12
*Author: YuaniLee
*/
#include "embARC.h"
#include "embARC_debug.h"

#include "board.h"
#include "dev_uart.h" 

#include "stdio.h"

static DEV_UART_PTR M6;

extern void M6_init(void);
extern void voice_control(uint8_t M6_rcv_buf[],uint32_t M6_rcv_cnt);

