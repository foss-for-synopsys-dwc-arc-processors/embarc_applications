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
 * \date 2017-07-11
 * \author Xiangcai Huang(xiangcai@synopsys.com)
--------------------------------------------- */

/**
 * \defgroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE		embARC iBaby Smarthome Node Wearable Node
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_OS_FREERTOS
 * \ingroup	EMBARC_APPS_MID_LWIP
 * \ingroup	EMBARC_APPS_MID_WAKAAMA
 * \ingroup	EMBARC_APPS_MID_FATFS
 * \brief	embARC iBaby Smarthome Node Wearable Node
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *     [Digilent PMOD WIFI(MRF24WG0MA)](http://www.digilentinc.com/Products/Detail.cfm?NavPath=2,401,884&Prod=PMOD-WIFI)
 *     [MPU6050](https://detail.tmall.com/item.htm?spm=a230r.1.14.34.Ys6tOc&id=25475512927&ns=1&abbucket=11)
 *     [MAX30102](https://detail.tmall.com/item.htm?spm=a230r.1.14.27.opR0Vh&id=538927057021&ns=1&abbucket=11)
 *     [MLX90614](https://detail.tmall.com/item.htm?spm=a230r.1.14.1.HWvJo4&id=530583718297&cm_id=140105335569ed55e27b&abbucket=15)
 *
 * ### Design Concept
 *     All sensor modules should be connected to \ref EMBARC_BOARD_CONNECTION "EMSK".
 *     This application is designed to show how to connect EMSK and iBaby Smarthome Gateway using embARC. 
 *
 * ### Usage Manual
 *     Before compiling this example, you need to change some macro in value.h to customize your own project.
 *
 *     It can report the following information to the iBaby Smarthome Gateway.
 *     - the Body Temperature (uint32_t) - Wearable module connected to J4 (Based on Temperature sensor)
 *     - the Heartrate (uint32_t) - Wearable module connected to J4 (Based on Heartrate sensor)
 *     - the Motion Intensity in 1min (int) - Wearable module connected to J4 (Based on Acceleration sensor)
 *     - the Sleep-Wake state (uint) - Wearable module connected to J4 (Based on Acceleration sensor)
 *     - the Awake Event (uint) - Wearable module connected to J4 (Based on Acceleration sensor)
 *     - 3 flags of Warning (bool) - Warning of Body Temperature abnormal, Heartrate abnormal, sleep on his stomach
 *
 *     EMSK can send the above data to iBaby Gateway Smarthome with LwM2M.
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * \brief	main source of iBaby Smarthome Node Wearable Node
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * @{
 */
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#include "value.h"
#include "task_function.h"

/**
 * \brief  main entry, call Freertos API, create and start functional task
 */
int main(void)
{
	EMBARC_PRINTF("*********************************************************\r\n\
				   *                 iBaby Smarthome Nodes                 *\r\n\
				   *                     Wearable Node                     *\r\n\
				   *********************************************************\r\n");

	/* initialize body temperature sensor */
	if (btemp_sensor_init(TMP112_ADDRESS) != E_OK)
	{
		EMBARC_PRINTF("Error: body temperature sensor init failed\r\n");
		return E_SYS;
	}

	/* initialize heartrate sensor */
	if (hrate_sensor_init(MAX30102_ADDRESS) != E_OK)
	{
		EMBARC_PRINTF("Error: heartrate sensor init failed\r\n");
		return E_SYS;
	}

	/* initialize acceleration sensor */
	if (acc_sensor_init(IMU_I2C_SLAVE_ADDRESS) != E_OK)
	{
		EMBARC_PRINTF("Error: acceleration sensor init failed\r\n");
		return E_SYS;
	}
	
	/* create task for function */
	if (xTaskCreate(task_function, "task_function", STACK_DEPTH_FUNC, NULL, TSKPRI_MID, 
		NULL) != pdPASS){
		EMBARC_PRINTF("Error: Create task_function failed\r\n");
		return E_SYS;
	}
	
	return E_OK;
}

/** @} */
