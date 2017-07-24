/* standard C HAL */
#include <stdio.h>

/* embARC HAL */
#include "arc.h"
#include "arc_builtin.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"

#include "embARC.h"
#include "embARC_debug.h"
#include "dev_iic.h"
#include "board.h"

#include "common.h"
#include "timer1.h"


/** arc timer 1 interrupt routine */
static void timer1_isr(void *ptr)
{
	timer_int_clear(TIMER_1);
	t1_cnt++;
}

/** print message for debug major function */
extern void timer1_start(void)
{
	t1_cnt = 0;

	int_handler_install(INTNO_TIMER1, timer1_isr);
	int_enable(INTNO_TIMER1);
	timer_start(TIMER_1, TIMER_CTRL_IE, T1_COUNT_UNIT_LMS);
}

/** print message for debug major function */
extern uint32_t timer1_stop(void)
{
	uint32_t dec;

	timer_stop(TIMER_1);
	dec = t1_cnt - (t1_cnt/10) * 10;

	EMBARC_PRINTF("************     timing     ************\r\n");
	EMBARC_PRINTF("* timer1 counter : %d.%d ms\r\n", t1_cnt/10, dec);
	EMBARC_PRINTF("****************************************\r\n\r\n");

	return dec;
}