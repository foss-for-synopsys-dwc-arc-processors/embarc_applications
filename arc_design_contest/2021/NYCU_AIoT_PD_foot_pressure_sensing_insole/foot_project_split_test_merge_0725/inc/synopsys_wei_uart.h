/* SPDX-License-Identifier: BSD-3-Clause*/
#include "hx_drv_tflm.h"

#ifndef ARC_UART_H
#define ARC_UART_H

typedef enum{
	HAL_OK = 0,
	HAL_ERROR = 1
}
HAL_RETURN;
void hal_test(uint8_t *char_data);
HAL_RETURN hal_uart_get_char(uint8_t *char_data);
#endif
/*
 * ARC_UART_H
 *
 */

