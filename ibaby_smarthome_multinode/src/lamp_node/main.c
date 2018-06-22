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
 * \defgroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_LAMP_NODE		embARC IoT iBaby Smarthome Node Lamp Node
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_OS_FREERTOS
 * \ingroup	EMBARC_APPS_MID_LWIP
 * \ingroup	EMBARC_APPS_MID_WAKAAMA
 * \ingroup	EMBARC_APPS_MID_FATFS
 * \ingroup	EMBARC_APPS_MID_AWS
 * \brief	embARC IoT iBaby Smarthome Node Lamp Node
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *     [Digilent PMOD WIFI(MRF24WG0MA)](http://www.digilentinc.com/Products/Detail.cfm?NavPath=2,401,884&Prod=PMOD-WIFI)
 *
 * ### Design Concept
 *     All modules should be connected to \ref EMBARC_BOARD_CONNECTION "EMSK".
 *     This application is designed to show how to connect EMSK and iBaby Smarthome Gateway using embARC.
 *     This a simple application example than iBaby Wearable Node, it can help user add extensional nodes to iBaby Smarthome.
 *     Here we use the LED0 on emsk to simulate the desk lamp, it makes the application more simple and easy to learn.
 *
 * ### Usage Manual
 *     Before compiling this example, you need to change some macro in common.h to customize your own project.
 *
 *     It can report the following information to the iBaby Smarthome Gateway.
 *     - the flag of Lamp working state (bool) - The Lamp is ON or OFF.
 *
 *     EMSK can send the above data to iBaby Smarthome Gateway with LwM2M.
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_LAMP_NODE
 * \brief	main source of iBaby Smarthome Node Lamp Node
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
#include "lwm2m.h"
#include "lamp_work.h"

#include "btn.h"
#include "lamp.h"


/**
 * \brief  main entry
 */
int main(void)
{
	btn_init();  /* initialize button on emsk */

	lamp_init(); /* initialize led on emsk */

#if LWM2M_CLIENT
	/* try to start lwm2m client */
	lwm2m_client_start();
#endif
	/* lamp start to work */
	lamp_start();

	while(1) {
		vTaskSuspend(NULL);
	}

	/* never run to here */
	return E_SYS;
}

/** @} */