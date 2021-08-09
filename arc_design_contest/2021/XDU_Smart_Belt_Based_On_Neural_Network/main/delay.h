#ifndef _DELAY_H_
#define _DELAY_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

#include "board.h"
#include "dev_common.h"

void     delay_ms(volatile uint32_t ms);
void     delay_s(volatile uint32_t s);
void     delay_us(uint32_t us);

#endif