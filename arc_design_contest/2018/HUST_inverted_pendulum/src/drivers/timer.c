/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#ifndef BASE_TIMER_HZ
#define BASE_TIMER_HZ		(500)
#endif

/** Tick semaphore for motorTask */
SemaphoreHandle_t SemMotTick;

/** interrupt reset count */
static uint32_t cyc_hz_count = (BOARD_CPU_CLOCK / BASE_TIMER_HZ);

/**
 * \brief	timer interrupt.
 * 	the Interrupt frequency is based on the defined \ref BASE_TIMER_HZ
 */
static void base_timer_isr(void *ptr)
{
	BaseType_t rtn, Wake;
	timer_int_clear(TIMER_1);

	/** 2ms give a semaphore */
	rtn = xSemaphoreGiveFromISR(SemMotTick, &Wake);
	configASSERT((rtn == pdPASS) ||  (rtn == errQUEUE_FULL));
}

/**
 * \brief	init timer and interrupt
 * \details
 * 		This function is called in \ref motorTask, and
 * 	it initializes the 2-MS timer interrupt
 */
void base_timer_init(void)
{

	SemMotTick = xSemaphoreCreateBinary();
    configASSERT(SemMotTick != NULL);
    
	if (timer_present(TIMER_1)) {
		int_disable(INTNO_TIMER1); /* disable first then enable */
		int_handler_install(INTNO_TIMER1, base_timer_isr);
		timer_start(TIMER_1, TIMER_CTRL_IE|TIMER_CTRL_NH, cyc_hz_count);  /* start 2ms timer interrupt */

		int_enable(INTNO_TIMER1);
	}
}
