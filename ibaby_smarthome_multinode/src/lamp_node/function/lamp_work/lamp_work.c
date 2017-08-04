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
 * be used

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
 * \ingroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_LAMP_NODE
 * \brief	function for control lamp to work
 *              lamp turn on or turn off
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_LAMP_NODE
 * @{
 */
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "common.h"
#include "print_msg.h"
#include "lamp_work.h"

#include "lamp.h"


#define STACK_DEPTH_LAMP (128) /* stack depth for lamp work : word(4*bytes) */
#define TIME_DELAY_LAMP_MS (1000)

static void task_lamp(void *par);
static TaskHandle_t task_lamp_handle = NULL;


/** function for lamp working */
extern void lamp_start(void)
{
	/* create task for lamp */
	if (xTaskCreate(task_lamp, "lamp", STACK_DEPTH_LAMP, NULL, TSKPRI_MID,
	                &task_lamp_handle) != pdPASS) {
		EMBARC_PRINTF("Error: Create task_lamp failed\r\n");
	}
}

/** task for lamp working */
static void task_lamp(void *par)
{
	while(1) {
		if (data_report_ln.flag_lamp_work)
		{
			lamp_on();
		} else {
			lamp_off();
		}

#if PRINT_DEBUG_FUNC
		print_msg_func();
#endif

		vTaskDelay(TIME_DELAY_LAMP_MS);
	}
}

/** @} */