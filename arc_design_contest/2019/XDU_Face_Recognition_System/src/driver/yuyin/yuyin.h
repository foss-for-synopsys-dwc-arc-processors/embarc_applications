#include "embARC.h"
#include "board.h"
#include <stdio.h>
#include "embARC_debug.h"
#define HEADLEN          5 
static DEV_UART *dev_uart2 = NULL;
static void delay_us(volatile int z);
static void delay_ms(volatile int z);
void  yuyin_llt();
void  yuyin_lx();
void  yuyin_szw();
void  yuyin_wfsb();