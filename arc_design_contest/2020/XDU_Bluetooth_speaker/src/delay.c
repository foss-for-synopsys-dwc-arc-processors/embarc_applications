#include "delay.h"

/**
 * \brief	get current passed us since timer init
 * \retval	us count in 64bit format
 */

extern void delay_us(uint32_t us)
{
	uint64_t start_us, us_delayed;

	us_delayed = (uint64_t) us;
	start_us = board_get_cur_us();
	while ((board_get_cur_us() - start_us) < us_delayed)
		;
}

extern void delay_ms(volatile uint32_t ms)
{				// 1ms real time delay
	board_delay_ms(ms, OSP_DELAY_OS_COMPAT_DISABLE);
}

extern void delay_s(volatile uint32_t s)
{				// 1s real time delay
	board_delay_ms(1000 * s, OSP_DELAY_OS_COMPAT_DISABLE);
}
