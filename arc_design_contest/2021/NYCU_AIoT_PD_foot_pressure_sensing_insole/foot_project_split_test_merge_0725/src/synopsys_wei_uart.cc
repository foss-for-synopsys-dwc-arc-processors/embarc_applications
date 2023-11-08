/* SPDX-License-Identifier: BSD-3-Clause*/

#include "synopsys_wei_uart.h"
HAL_RETURN hal_uart_get_char(uint8_t *char_data)
{
	uint8_t data_buf = 0;
	if(hx_drv_uart_getchar(&data_buf) != HX_DRV_LIB_PASS){
		return HAL_ERROR;
	}else{
		*char_data = data_buf;
		return HAL_OK;
	}
}
