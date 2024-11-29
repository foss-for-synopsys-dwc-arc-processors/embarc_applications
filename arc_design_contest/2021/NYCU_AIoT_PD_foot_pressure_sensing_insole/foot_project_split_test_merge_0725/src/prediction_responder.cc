/* SPDX-License-Identifier: BSD-3-Clause*/
/*
 *Copyright 2019 The TensorFlow Authors. All Rights Reserved.
 *
 *Licensed under the Apache License, Version 2.0 (the "License");
 *you may not use this file except in compliance with the License.
 *You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing, software
 *distributed under the License is distributed on an "AS IS" BASIS,
 *WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *See the License for the specific language governing permissions and
 *limitations under the License.
 *==============================================================================
 */

#include "prediction_responder.h"

#include "hx_drv_tflm.h"

/*
 * This dummy implementation writes co and pt scores to the error
 * console. Real applications will want to take some custom action instead, and
 * should implement their own versions of this function.
 */

void Prediction(volatile float pt_score_l, volatile float co_score_l,
		volatile float pt_score_r, volatile float co_score_r)
{
	volatile int pt_l_1000, co_l_1000, pt_r_1000, co_r_1000, pt_l_100,co_l_100, pt_r_100, co_r_100;
	pt_l_100 = 100 * pt_score_l;
	co_l_100 = 100 * co_score_l;
	pt_r_100 = 100 * pt_score_r;
	co_r_100 = 100 * co_score_r;

	pt_l_1000 = 1000 * pt_score_l - 10 * pt_l_100;
	co_l_1000 = 1000 * co_score_l - 10 * co_l_100;
	pt_r_1000 = 1000 * pt_score_r - 10 * pt_r_100;
	co_r_1000 = 1000 * co_score_r - 10 * co_r_100;

	volatile float pt_score, co_score;
	pt_score = pt_score_l + pt_score_r;
	co_score = co_score_l + co_score_r;

	if (pt_score > co_score) {
		hx_drv_led_off(HX_DRV_LED_GREEN);
		hx_drv_led_on(HX_DRV_LED_RED);
	} else {
		hx_drv_led_off(HX_DRV_LED_RED);
		hx_drv_led_on(HX_DRV_LED_GREEN);
	}

	hx_drv_uart_print("\n");
	hx_drv_uart_print("right : pt score: %d.%d", pt_r_100, pt_r_1000);
	hx_drv_uart_print("%c", '%');
	hx_drv_uart_print("\n");
	hx_drv_uart_print("right : co score: %d.%d", co_r_100, co_r_1000);
	hx_drv_uart_print("%c", '%');
	hx_drv_uart_print("\n");
	hx_drv_uart_print("left : pt score: %d.%d", pt_l_100, pt_l_1000);
	hx_drv_uart_print("%c", '%');
	hx_drv_uart_print("\n");
	hx_drv_uart_print("left : co score: %d.%d", co_l_100, co_l_1000);
	hx_drv_uart_print("%c", '%');
	hx_drv_uart_print("\n");

	hx_drv_uart_print("%d.%d\n", pt_l_100, pt_l_1000);
	hx_drv_uart_print("%d.%d\n", pt_r_100, pt_r_1000);
}
