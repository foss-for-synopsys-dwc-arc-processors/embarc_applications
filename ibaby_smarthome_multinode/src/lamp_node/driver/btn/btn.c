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
* \version 2017.07
* \date 2017-07-26
* \author Xiangcai Huang(xiangcai@synopsys.com)
--------------------------------------------- */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE
 * \brief	emsk temperature sensor driver for ibaby wearable node
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE
 * @{
 */

/* standard C HAL */
#include <stdio.h>

/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "btn.h"
#include "common.h"


#define BTN_L_BIT_OFFSET (0)
#define BTN_R_BIT_OFFSET (1)
#define BTN_A_BIT_OFFSET (2)

#define BTN_BIT_MASK     (7)

#define BTN_ACTIVE_LOW   (0)
#define BTN_ACTIVE_HIGH  (1)

static void btn_l_isr(void *ptr);
static void btn_r_isr(void *ptr);
static void btn_a_isr(void *ptr);


/** interrupt service routine for button L*/
static void btn_l_isr(void *ptr)
{
	data_report_ln.flag_lamp_work = !data_report_ln.flag_lamp_work;
	EMBARC_PRINTF("\nbutton L pressed\r\n");
}

/** interrupt service routine for button R*/
static void btn_r_isr(void *ptr)
{
	EMBARC_PRINTF("\nbutton R pressed\r\n");
}

/** interrupt service routine for button A*/
static void btn_a_isr(void *ptr)
{
	EMBARC_PRINTF("\nbutton A pressed\r\n");
}

/**
 * \brief	initialize GPIO for button
 */
extern void btn_init(void)
{
	DEV_GPIO_BIT_ISR bit_isr; 
	DEV_GPIO_INT_CFG int_cfg;

	DEV_GPIO_PTR port = gpio_get_dev(EMSK_BUTTON_PORT);
	int_cfg.int_bit_mask = BTN_BIT_MASK;
	int_cfg.int_bit_type = BTN_BIT_MASK; 
	int_cfg.int_bit_polarity = BTN_ACTIVE_LOW; 
	int_cfg.int_bit_debounce = BTN_BIT_MASK;

	port->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);

	bit_isr.int_bit_ofs = BTN_L_BIT_OFFSET;
	bit_isr.int_bit_handler = btn_l_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	bit_isr.int_bit_ofs = BTN_R_BIT_OFFSET;
	bit_isr.int_bit_handler = btn_r_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	bit_isr.int_bit_ofs = BTN_A_BIT_OFFSET;
	bit_isr.int_bit_handler = btn_a_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	port->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)BTN_BIT_MASK);

}

/** @} end of group EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE */