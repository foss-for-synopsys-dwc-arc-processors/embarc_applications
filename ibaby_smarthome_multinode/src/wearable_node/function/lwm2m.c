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
 * \date 2017-07-11
 * \author Xiangcai Huang(xiangcai@synopsys.com)
--------------------------------------------- */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * \brief	function for LwM2M Client start
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * @{
 */
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#include "lwm2mclient.h"
#include "liblwm2m.h"
#include "connection.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <errno.h>
#include "lwip_pmwifi.h"

/* custom HAL */
#include "common.h"
#include "lwm2m.h"


#define STACK_DEPTH_LWM2M (20480)       /* stack depth for lwm2mClient : word(4*bytes) */


static lwm2m_client_info c_info;        /* lwM2M client info */
static int lwm2m_client_conn_stat = 0;  /* flag of connect status lwM2M client */
static int lwm2m_client_start_flag = 0; /* flag of start of lwM2M client */

static void task_lwm2m_client(void *par);
static TaskHandle_t task_lwm2m_client_handle = NULL;

const static char *p_port   = (char *)"5683";    /* lwm2mServer's port and IP */
const static char *p_server = (char *)"192.168.43.199";
const static char *p_client_name = (char *)"wn"; /* name of lwm2m client node */


#if LWM2M_CLIENT
/** task for lwm2m client */
static void task_lwm2m_client(void *par)
{
	unsigned int cpu_status;

	while (1) {
		lwm2m_client_conn_stat = 1;

		/* get into sub function(lwm2mclient) to do all work about data interaction with gateway(lwm2m server) */
		if (lwm2mclient(&c_info) == 0) {
			EMBARC_PRINTF("LwM2M client end successfully\r\n");
		} else {
			EMBARC_PRINTF("Error: LwM2M client end failed\r\n");
		}

		cpu_status = cpu_lock_save();
		if (c_info.server != NULL)     free((void *)(c_info.server));
		if (c_info.ep_name != NULL)    free((void *)(c_info.ep_name));
		if (c_info.serverPort != NULL) free((void *)(c_info.serverPort));
		lwm2m_client_conn_stat = 0;
		cpu_unlock_restore(cpu_status);

		vTaskSuspend(task_lwm2m_client_handle);
	}
}

/** function for initialize and start lwm2m client */
extern int lwm2m_client_start(void)
{
	int c_quit = 0;
	task_lwm2m_client_handle = 0;

	/* check to see if wifi works */
	if (!lwip_pmwifi_isup()) {
		EMBARC_PRINTF("Error: Wifi is not ready for lwM2M client.\r\n");
		goto error_exit;
	}

	/* exit lwm2mClient */
	if (c_quit == 1) {
		EMBARC_PRINTF("Error: Try to exit existing client.\r\n");
		handle_sigint(2);
		goto error_exit;
	}

	if (lwm2m_client_conn_stat == 1) {
		EMBARC_PRINTF("Error: LwM2M client is already running.\r\n");
		goto error_exit;
	}

	if (p_server == NULL) {
		EMBARC_PRINTF("Error: Server Url is not specified, please check it.\r\n");
		goto error_exit;
	}

	c_info.server = p_server;
	c_info.ep_name = p_client_name;
	c_info.serverPort = p_port;

	lwm2m_client_start_flag = 1;
	EMBARC_PRINTF("Start lwm2m client.\r\n");

	/* create or resume task for lwm2mClient to realize communication with iBaby Smarthome Gateway */
	if (xTaskCreate(task_lwm2m_client, "lwm2m client", STACK_DEPTH_LWM2M, NULL, TSKPRI_HIGH,
	                &task_lwm2m_client_handle) != pdPASS) {
		EMBARC_PRINTF("Error: Create task_lwm2m_client failed\r\n");
		return E_SYS;
	} else {
		vTaskResume(task_lwm2m_client_handle);
		return E_OK;
	}

	return E_OK;

error_exit:
	return E_SYS;
}
#endif/* LWM2M_CLIENT */

/** @} */