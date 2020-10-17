#ifndef __SR501_H__
#define	__SR501_H__

#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "dev_gpio.h"

//PMOD1 PORT
#define SR_1_MASK 0x100

static DEV_GPIO_PTR PMOD_P;//pin 7

void HC_SR501Configuration(void);
int HC_SR501_Status(void);
#endif
