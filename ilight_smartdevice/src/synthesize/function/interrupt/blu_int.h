#ifndef _BLU_INT_H_
#define _BLU_INT_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "task.h"

#define BLU_RX_HEAD_FIRST		'@'
#define BLU_RX_HEAD_SECOND		'#'

#define BLU_RX_WEATHER_MODE		'0'
#define BLU_RX_RUNNING_MODE		'1'
#define BLU_RX_ALARM_MODE		'2'
#define BLU_RX_RIDING_MODE		'3'
#define BLU_RX_LIGHTING_MODE	'4'
#define BLU_RX_CLOCK_MODE		'5'
#define BLU_RX_FANSE_MODE		'6'
#define BLU_RX_TIME_MODE		'7'
#define BLU_RX_MUSIC_MODE		'8'
#define BLU_RX_COMFOR_MODE		'9'
#define BLU_RX_MIN_MODE			'0'

#define BLU_FREQUENCY_SLOW		'0'
#define BLU_FREQUENCY_NORMAL	'1'
#define BLU_FREQUENCY_FAST		'2'
#define BLU_FREQUENCY_VERY_FAST	'3'

#define BLU_TAG_BRIGHT			'1'
#define BLU_TAG_FREQUENCE		'2'
#define BLU_TAG_COOL_OR_WARM	'2'
#define BLU_TAG_ANIMATION		'3'

#define BLU_TAG_LIGHT_WARM		'1'
#define BLU_TAG_LIGHT_COOL		'0'
#define BLU_TAG_LIGHT_ANIM		'1'
#define BLU_TAG_LIGHT_NOT_ANIM	'0'

#define INITIAL_STATE			0
#define FIRST_STATE				1
#define SECOND_STATE			2
#define THIRD_STATE				3
#define FOURTH_STATE			4

#define BLU_STATE_BYTE			(BLU_RX_HANDLE_BUF_MAX-1)
#define BLU_DATA_LEN_BYTE		(BLU_RX_HANDLE_BUF_MAX-2)
#define BLU_DATA_CNT_BYTE		(BLU_RX_HANDLE_BUF_MAX-3)
#define BLU_MODE_TAG_BYTE		(BLU_RX_HANDLE_BUF_MAX-4)
#define BLU_MODE_AUX_TAG_BYTE	(BLU_RX_HANDLE_BUF_MAX-5)
#define BLU_LIGHT_TAG_BYTE		(BLU_RX_HANDLE_BUF_MAX-6)
#define BLU_COLOR_TAG_BYTE		(BLU_RX_HANDLE_BUF_MAX-7)

#define BLU_MODE_TIME_RX_LEN	4
#define BLU_MODE_WEATHER_RX_LEN	4

#define BLU_RX_HANDLE_BUF_MAX	25

/* Initial the scope using uart0 */
/* Attention: only use without scope */
extern uint32_t blu_uart_init(uint32_t baudrate);

/* Send date to blueteeth */
extern uint32_t blu_send(uint8_t *psend, uint8_t cnt);

#endif /*_BLU_INT_H_ */