/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"


#define LED_TOGGLE_MASK		BOARD_LED_MASK

/**
 * \brief	Test hardware board without any peripheral
 */
void LED_task(void *p_arg)
{
	uint16_t led_toggle_val = LED_TOGGLE_MASK;
	uint64_t Test = CLK_CPU;
	while (1) {
		led_write(led_toggle_val, BOARD_LED_MASK);
		led_toggle_val = ~led_toggle_val;
		board_delay_ms(500, 1);
	}
}
/** @} */