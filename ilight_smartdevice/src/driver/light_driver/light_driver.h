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
 * \version 2017.09
 * \date 2017-09-27
 * \author dbHu(wwmhu@outlook.com)
--------------------------------------------- */
/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief	light control driver for ilight.
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */

#ifndef _LIGHT_DRIVER_H_
#define _LIGHT_DRIVER_H_

/*!< using GPIO A11 & C19 as LED controller Pin */
#define GPIO_OUT_PORT_MIDDLE	(DW_GPIO_PORT_A)
#define GPIO_OUT_OFFSET_MIDDLE	(11)
#define GPIO_OUT_MASK_MIDDLE	(1 << GPIO_OUT_OFFSET_MIDDLE)
#define GPIO_H_CFG_MIDDLE	(1 << GPIO_OUT_OFFSET_MIDDLE)

#define GPIO_OUT_PORT_SIDE	(DW_GPIO_PORT_C)
#define GPIO_OUT_OFFSET_SIDE	(19)
#define GPIO_OUT_MASK_SIDE	(1 << GPIO_OUT_OFFSET_SIDE)
#define GPIO_H_CFG_SIDE		(1 << GPIO_OUT_OFFSET_SIDE)

/*!< define led row to distinguish controller pin */
#define LIGHT_ROW_MIDDLE	0x010
#define LIGHT_ROW_SIDE		0x101

/*!< the led numbers of a row */
#define LIGHT_NUM_MIDDLE	16
#define LIGHT_NUM_SIDE		34

/**
 * Llight color data structure
 * r,g,b reresents the RGB color data
 */
typedef union {
	uint32_t color;
	struct rgb_s {
		uint8_t b;
		uint8_t r;
		uint8_t g;
		uint8_t reserved;
	} rgb;
} RGB_T;

volatile uint32_t delay_num;
extern uint8_t color_row;
extern uint8_t light_num;

/*!< The underlying lighting control module function */
extern void light_ctr_reset(void);
extern void delay_10us(uint32_t mul);
extern void light_ctr_gpio_init(void);
extern uint64_t light_ctr_mask_lshift(uint64_t aim_num,uint64_t shift_num);
extern void light_ctr_rgb(uint64_t mask,uint16_t row,uint8_t red,uint8_t green,uint8_t blue);
extern void light_send_colordata(uint32_t color_data,uint16_t row);
extern void light_ctr_rgb_update(uint64_t mask,uint16_t row,uint8_t red,uint8_t green,uint8_t blue);

#endif /* _LIGHT_DRIVER_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE */