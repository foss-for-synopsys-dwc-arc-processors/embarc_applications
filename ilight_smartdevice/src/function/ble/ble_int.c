/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief	function for bluetooth initial and interupt to devide mode
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"
/*custom HAL*/
#include "task.h"
#include "ble_int.h"
#include "light_mode.h"

#ifdef	BLE_UART_EN

#define BLE_RX_HEAD_FIRST		'@' /* protocol of data transmission, the first head sign */
#define BLE_RX_HEAD_SECOND		'#' /* protocol of data transmission, the second head sign */

#define BLE_RX_WEATHER_MODE		'0' /* protocol of data transmission, the sign of mode */
#define BLE_RX_RUNNING_MODE		'1'
#define BLE_RX_ALARM_MODE		'2'
#define BLE_RX_RIDING_MODE		'3'
#define BLE_RX_LIGHTING_MODE		'4'
#define BLE_RX_CLOCK_MODE		'5'
#define BLE_RX_FANSE_MODE		'6'
#define BLE_RX_TIME_MODE		'7'
#define BLE_RX_MUSIC_MODE		'8'
#define BLE_RX_COMFOR_MODE		'9'
#define BLE_RX_MIN_MODE			'0'

#define BLE_FREQUENCY_SLOW		'0' /* protocol of data transmission, the frequency of lighting mode  */
#define BLE_FREQUENCY_NORMAL		'1'
#define BLE_FREQUENCY_FAST		'2'
#define BLE_FREQUENCY_VERY_FAST		'3'

#define BLE_TAG_BRIGHT			'1' /* protocol of data transmission, the brightness of lighting mode  */
#define BLE_TAG_FREQUENCE		'2'
#define BLE_TAG_COOL_OR_WARM		'2'
#define BLE_TAG_ANIMATION		'3'

#define BLE_TAG_LIGHT_WARM		'1' /* protocol of data transmission, the chroma of lighting mode  */
#define BLE_TAG_LIGHT_COOL		'0'

#define BLE_TAG_LIGHT_ANIM		'1' /* protocol of data transmission, the flag of lighting mode to dispaly animated light */
#define BLE_TAG_LIGHT_NOT_ANIM		'0'

#define INITIAL_STATE			0 /* the stage sign, means the bluetooth stage */
#define FIRST_STATE			1
#define SECOND_STATE			2
#define THIRD_STATE			3
#define FOURTH_STATE			4

#define BLE_STATE_BYTE			(BLE_RX_HANDLE_BUF_MAX-1)
#define BLE_DATA_LEN_BYTE		(BLE_RX_HANDLE_BUF_MAX-2)
#define BLE_DATA_CNT_BYTE		(BLE_RX_HANDLE_BUF_MAX-3)
#define BLE_MODE_TAG_BYTE		(BLE_RX_HANDLE_BUF_MAX-4)
#define BLE_MODE_AUX_TAG_BYTE		(BLE_RX_HANDLE_BUF_MAX-5)
#define BLE_LIGHT_TAG_BYTE		(BLE_RX_HANDLE_BUF_MAX-6)
#define BLE_COLOR_TAG_BYTE		(BLE_RX_HANDLE_BUF_MAX-7)

#define BLE_MODE_TIME_RX_LEN		4 /* protocol of data transmission, the data of time mode is 4 bytes */
#define BLE_MODE_WEATHER_RX_LEN		4 /* protocol of data transmission, the data of weather mode is 4 bytes */

#define BLE_RX_HANDLE_BUF_MAX		25 /* the max length of bluetooth buffer handle */

static DEV_UART *ble_uart;
static uint8_t ble_rx_buffer;
static DEV_BUFFER ble_uart_int_buffer;

/**
 * \brief	Function for deviding music mode and comfortable mode.
 * \param[in]	ble_mode_info_ptr. used to trans data received.
 * \param[in]	ble_rx_handle_ptr.
 * \param[in]	data.used to judge mode.
 */
static void ble_rx_state_second(mode_info *ble_mode_info_ptr, uint8_t *ble_rx_handle_ptr, uint8_t data)
{
	if (data == BLE_RX_MUSIC_MODE) {
		ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;
		ble_mode_info_ptr->mode = LIGHT_MODE_MUSIC;
		xQueueOverwrite(mode_info_queue, ble_mode_info_ptr);
		xTaskResumeFromISR( pattern_change_task_handle );
	} else if (data == BLE_RX_COMFOR_MODE) {
		ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;
		ble_mode_info_ptr->mode = LIGHT_MODE_COMFOR;
		xQueueOverwrite(mode_info_queue, ble_mode_info_ptr);
		xTaskResumeFromISR( pattern_change_task_handle );
	} else {
		ble_rx_handle_ptr[BLE_STATE_BYTE] = THIRD_STATE;
		ble_rx_handle_ptr[BLE_MODE_TAG_BYTE] = data;
	}
}

/**
 * \brief	Function for deviding weather mode、running mode、alarm mode、riding mode、lighting mode、clock mode、time mode and fans mode.
 */
static void ble_rx_state_third(mode_info *ble_mode_info_ptr, uint8_t *ble_rx_handle_ptr, uint8_t data)
{
	switch (ble_rx_handle_ptr[BLE_MODE_TAG_BYTE]) {
	case BLE_RX_WEATHER_MODE:
		if ( data == '0') {
			ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;
			ble_mode_info_ptr->mode = LIGHT_MODE_WEATHER;
			xQueueOverwrite(mode_info_queue, ble_mode_info_ptr);
			xTaskResumeFromISR( pattern_change_task_handle );
		} else {
			ble_rx_handle_ptr[BLE_STATE_BYTE] = FOURTH_STATE;
			ble_mode_info_ptr->weather = data - '0';
		}
		break;

	case BLE_RX_RUNNING_MODE:
	case BLE_RX_ALARM_MODE:
	case BLE_RX_RIDING_MODE:
	case BLE_RX_LIGHTING_MODE:
		ble_rx_handle_ptr[BLE_STATE_BYTE] = FOURTH_STATE;
		ble_rx_handle_ptr[BLE_MODE_AUX_TAG_BYTE] = data;
		break;

	case BLE_RX_CLOCK_MODE:
	case BLE_RX_TIME_MODE:
		ble_rx_handle_ptr[ ble_rx_handle_ptr[BLE_DATA_CNT_BYTE]++ ] = data - '0';

		if (ble_rx_handle_ptr[BLE_DATA_CNT_BYTE] == BLE_MODE_TIME_RX_LEN) {
			ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;
			ble_rx_handle_ptr[BLE_DATA_CNT_BYTE] = 0;

			if (ble_rx_handle_ptr[BLE_MODE_TAG_BYTE] == BLE_RX_CLOCK_MODE) {
				ble_mode_info_ptr->mode = LIGHT_MODE_CLOCK;
			} else {
				ble_mode_info_ptr->mode = LIGHT_MODE_TIMING_LAND;
			}

			ble_mode_info_ptr->time = ble_rx_handle_ptr[0] * 600 + ble_rx_handle_ptr[1] * 60 + \
			                          ble_rx_handle_ptr[2] * 10 + ble_rx_handle_ptr[3];
			xQueueOverwrite(mode_info_queue, ble_mode_info_ptr);
			xTaskResumeFromISR( pattern_change_task_handle );
		}
		break;

	case BLE_RX_FANSE_MODE:
		ble_rx_handle_ptr[BLE_STATE_BYTE] = FOURTH_STATE;
		ble_rx_handle_ptr[BLE_DATA_LEN_BYTE] = data;
		break;

	default:
		ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;
		EMBARC_PRINTF("Wrong input %d in state 3.\r\n", ble_rx_handle_ptr[BLE_MODE_TAG_BYTE]);
		break;
	}
}

/**
 * \brief	Function for deviding weather mode、running mode、alarm mode、riding mode、
 *		lighting mode、clock mode、time mode and fans mode.
 */
static void ble_rx_state_forth(mode_info *ble_mode_info_ptr, uint8_t *ble_rx_handle_ptr, uint8_t data)
{
	switch (ble_rx_handle_ptr[BLE_MODE_TAG_BYTE]) {
	case BLE_RX_WEATHER_MODE:
		ble_rx_handle_ptr[ ble_rx_handle_ptr[BLE_DATA_CNT_BYTE]++ ] = data - '0';

		if (ble_rx_handle_ptr[BLE_DATA_CNT_BYTE] == BLE_MODE_WEATHER_RX_LEN) {
			ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;
			ble_rx_handle_ptr[BLE_DATA_CNT_BYTE] = 0;

			ble_mode_info_ptr->temperture = ble_rx_handle_ptr[0] * 10 + ble_rx_handle_ptr[1];
			ble_mode_info_ptr->humidity = ble_rx_handle_ptr[2] * 10 + ble_rx_handle_ptr[3];
			xQueueOverwrite(mode_info_queue, ble_mode_info_ptr);
		}
		break;

	case BLE_RX_RUNNING_MODE:
	case BLE_RX_ALARM_MODE:
	case BLE_RX_RIDING_MODE:
		ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;
		if (ble_rx_handle_ptr[BLE_MODE_AUX_TAG_BYTE] == BLE_TAG_BRIGHT) {
			ble_mode_info_ptr->bright = data;
		}
		else if (ble_rx_handle_ptr[BLE_MODE_AUX_TAG_BYTE] == BLE_TAG_FREQUENCE) {
			switch (data) {
			case BLE_FREQUENCY_SLOW :
				ble_mode_info_ptr->frequence = FREQUENCY_SLOW;
				break;
			case BLE_FREQUENCY_NORMAL :
				ble_mode_info_ptr->frequence = FREQUENCY_NORMAL;
				break;
			case BLE_FREQUENCY_FAST :
				ble_mode_info_ptr->frequence = FREQUENCY_FAST;
				break;
			case BLE_FREQUENCY_VERY_FAST :
				ble_mode_info_ptr->frequence = FREQUENCY_VERY_FAST;
				break;
			default :
				ble_mode_info_ptr->frequence = 0; 	
				break;
			}
		}

		if (ble_rx_handle_ptr[BLE_MODE_TAG_BYTE] == BLE_RX_RUNNING_MODE) {
			if (ble_mode_info_ptr->mode != LIGHT_MODE_RUNNING) {
				ble_mode_info_ptr->frequence = 0;
			}
			ble_mode_info_ptr->mode = LIGHT_MODE_RUNNING;
		} else if (ble_rx_handle_ptr[BLE_MODE_TAG_BYTE] == BLE_RX_ALARM_MODE) {
			if (ble_mode_info_ptr->mode != LIGHT_MODE_ALARM) {
				ble_mode_info_ptr->frequence = 0;
			}
			ble_mode_info_ptr->mode = LIGHT_MODE_ALARM;
		} else if (ble_rx_handle_ptr[BLE_MODE_TAG_BYTE] == BLE_RX_RIDING_MODE) {
			if (ble_mode_info_ptr->mode != LIGHT_MODE_RIDING) {
				ble_mode_info_ptr->frequence = 0;
			}
			ble_mode_info_ptr->mode = LIGHT_MODE_RIDING;
		}

		xQueueOverwrite(mode_info_queue, ble_mode_info_ptr);
		xTaskResumeFromISR( pattern_change_task_handle );
		break;

	case BLE_RX_LIGHTING_MODE:
		ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;

		if (ble_rx_handle_ptr[BLE_MODE_AUX_TAG_BYTE] == BLE_TAG_BRIGHT) {
			ble_mode_info_ptr->bright = data;
		} else if (ble_rx_handle_ptr[BLE_MODE_AUX_TAG_BYTE] == BLE_TAG_COOL_OR_WARM) {
			if (data == BLE_TAG_LIGHT_WARM) {
				ble_rx_handle_ptr[BLE_LIGHT_TAG_BYTE] = BLE_TAG_LIGHT_WARM;
			} else {
				ble_rx_handle_ptr[BLE_LIGHT_TAG_BYTE] = BLE_TAG_LIGHT_COOL;
			}
		} else if (ble_rx_handle_ptr[BLE_MODE_AUX_TAG_BYTE] == BLE_TAG_ANIMATION) {
			if (data == BLE_TAG_LIGHT_ANIM) {
				ble_rx_handle_ptr[BLE_COLOR_TAG_BYTE] = BLE_TAG_LIGHT_ANIM;
			} else {
				ble_rx_handle_ptr[BLE_COLOR_TAG_BYTE] = BLE_TAG_LIGHT_NOT_ANIM;
			}
		}

		if (ble_rx_handle_ptr[BLE_COLOR_TAG_BYTE] == BLE_TAG_LIGHT_ANIM) {
			ble_mode_info_ptr->mode = LIGHT_MODE_ANIMATION;
		} else {
			if (ble_rx_handle_ptr[BLE_LIGHT_TAG_BYTE] == BLE_TAG_LIGHT_WARM) {
				ble_mode_info_ptr->mode = LIGHT_MODE_WARM_LIGHTING;
			} else {
				ble_mode_info_ptr->mode = LIGHT_MODE_CO_LIGHTING;
			}
		}

		xQueueOverwrite(mode_info_queue, ble_mode_info_ptr);
		xTaskResumeFromISR(pattern_change_task_handle);
		break;

	case BLE_RX_FANSE_MODE:
		ble_mode_info_ptr->fance_txt[ ble_rx_handle_ptr[BLE_DATA_CNT_BYTE]++ ] = data;

		if (ble_rx_handle_ptr[BLE_DATA_CNT_BYTE] == ble_rx_handle_ptr[BLE_DATA_LEN_BYTE]) {
			ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;
			ble_mode_info_ptr->fance_txt[ ble_rx_handle_ptr[BLE_DATA_CNT_BYTE] ] = '\0';
			ble_rx_handle_ptr[BLE_DATA_CNT_BYTE] = 0;

			ble_mode_info_ptr->mode = LIGHT_MODE_SHAKING;
			xQueueOverwrite(mode_info_queue, ble_mode_info_ptr);
			xTaskResumeFromISR(pattern_change_task_handle);
		}
		break;

	default:
		ble_rx_handle_ptr[BLE_STATE_BYTE] = INITIAL_STATE;
		EMBARC_PRINTF("Wrong mode input %d in state 4.\r\n", ble_rx_handle_ptr[BLE_MODE_TAG_BYTE]);
		break;
	}
}

/**
 * \brief	Recive data from bluetooth
 * \details	Transfer the data to the useful format.
 * \param	data:the data received
 */
Inline void ble_data_recesive(uint8_t data)
{
	static mode_info ble_mode_info = INIT_MODE_INFO;
	static uint8_t ble_rx_handle[BLE_RX_HANDLE_BUF_MAX];

	if ((ble_rx_handle[BLE_STATE_BYTE] == INITIAL_STATE) && (data == BLE_RX_HEAD_FIRST)) {
		ble_rx_handle[BLE_STATE_BYTE] = FIRST_STATE;
	} else if (ble_rx_handle[BLE_STATE_BYTE] == FIRST_STATE && data == BLE_RX_HEAD_SECOND) {
		ble_rx_handle[BLE_STATE_BYTE] = SECOND_STATE;
	} else if ((ble_rx_handle[BLE_STATE_BYTE] == SECOND_STATE) && \
	           (data <= (BLE_RX_MIN_MODE + MAX_MODE)) && (data >= BLE_RX_MIN_MODE)) {
		ble_rx_state_second(&ble_mode_info, ble_rx_handle, data);
	} else if (ble_rx_handle[BLE_STATE_BYTE] == THIRD_STATE) {
		ble_rx_state_third(&ble_mode_info, ble_rx_handle, data);
	} else if (ble_rx_handle[BLE_STATE_BYTE] == FOURTH_STATE) {
		ble_rx_state_forth(&ble_mode_info, ble_rx_handle, data);
	} else {
		ble_rx_handle[BLE_STATE_BYTE] = INITIAL_STATE;
	}
}

/**
 * \brief	The isr of bluetooth
 * \details	The callback function of bluetooth isr.
 * \param	ptr: nonsense
 */
static void ble_irs(void *ptr)
{
	static uint8_t i;

	if (i % 2 == 0) {
		/* Recive data from bluetooth */
		ble_data_recesive(ble_rx_buffer);
		EMBARC_PRINTF("ble uart input %d.\r\n", ble_rx_buffer);
	}

	i++;
	if (ble_uart->uart_control(UART_CMD_SET_RXINT_BUF, (void *)&ble_uart_int_buffer) != E_OK) {
		EMBARC_PRINTF("ble_isr_buffer config error\r\n");
	}

	if (ble_uart->uart_control(UART_CMD_SET_RXINT, (void *)(1)) != E_OK) {
		EMBARC_PRINTF("Enable ble interrupt error\r\n");
	}
}

/**
 * \brief	Initialize the bluetooth device
 * \details	Initialize the uart0 driver for bluetooth
 * \param	baudrate: the baudrate of uart0
 * \param	-1 for error,0 for succeed
 */
uint32_t ble_uart_init(uint32_t baudrate)
{
	ble_uart = uart_get_dev(DW_UART_0_ID);

	if (ble_uart == NULL) {
		EMBARC_PRINTF("Failed to get device of uart0 for blueteeth.\r\n");
		return -1;
	}

	if (ble_uart->uart_open(baudrate) == E_OPNED) {
		ble_uart->uart_control(UART_CMD_SET_BAUD, (void *)(baudrate));
		EMBARC_PRINTF("ble_uart open succeed\r\n");
	}

	DEV_BUFFER_INIT(&ble_uart_int_buffer, &ble_rx_buffer, 1);
	if (ble_uart->uart_control(UART_CMD_SET_RXINT_BUF, (void *) & ble_uart_int_buffer) != E_OK) {
		EMBARC_PRINTF("ble_isr_buffer config error\r\n");
		return -1;
	}

	if (ble_uart->uart_control(UART_CMD_SET_RXCB, (void *) & ble_irs) != E_OK) {
		EMBARC_PRINTF("ble_isr config error\r\n");
		return -1;
	}

	if (ble_uart->uart_control(UART_CMD_SET_RXINT, (void *)(1)) != E_OK) {
		EMBARC_PRINTF("Enable ble interrupt error\r\n");
		return -1;
	}

	return 0;
}

/**
 * \brief	Send data by bluetooth
 * \details	Send cnt bytes using bluetooth in uart0.
 * \param	psend: the buffer of date sended in uart0
 *		cnt: the number of bytes to send
 * \param	-1 for error
 *		0 for succeed
 */
uint32_t ble_send(uint8_t *psend, uint8_t cnt)
{
	return ble_uart->uart_write(psend, cnt);
}

#endif /* BLE_UART_EN */

/** @} */