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
 * \defgroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE		embARC IOT iBaby Smarthome Node Wearable Node
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_OS_FREERTOS
 * \ingroup	EMBARC_APPS_MID_LWIP
 * \ingroup	EMBARC_APPS_MID_WAKAAMA
 * \ingroup	EMBARC_APPS_MID_FATFS
 * \ingroup	EMBARC_APPS_MID_AWS
 * \brief	embARC IOT iBaby Smarthome Node Wearable Node
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *     [Digilent PMOD WIFI(MRF24WG0MA)](http://www.digilentinc.com/Products/Detail.cfm?NavPath=2,401,884&Prod=PMOD-WIFI)
 *     [MPU6050](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/)
 *     [MAX30102](http://www.electronics-lab.com/max30102/)
 *     [MLX90614](https://developer.mbed.org/components/MLX90614-I2C-Infrared-Thermometer/)
 *
 * ### Design Concept
 *     All sensor modules should be connected to \ref EMBARC_BOARD_CONNECTION "EMSK".
 *     This application is designed to show how to connect EMSK and iBaby Smarthome Gateway using embARC.
 *
 * ### Usage Manual
 *     Before compiling this example, you need to change some macro in value.h to customize your own project.
 *
 *     It can report the following information to the iBaby Smarthome Gateway.
 *     - the Body Temperature (uint16_t) - Wearable module connected to J4 (Based on Temperature sensor)
 *     - the Heartrate (uint16_t) - Wearable module connected to J4 (Based on Heartrate sensor)
 *     - the Motion Intensity in 1min (uint32_t) - Wearable module connected to J4 (Based on Acceleration sensor)
 *     - the Sleep-Wake state (uint8_t) - Wearable module connected to J4 (Based on Acceleration sensor)
 *     - the Awake Event (uint8_t) - Wearable module connected to J4 (Based on Acceleration sensor)
 *     - 3 flags of Warning (bool) - Warning of Body Temperature abnormal, Heartrate abnormal, sleep on his stomach
 *
 *     EMSK can send the above data to iBaby Gateway Smarthome with LwM2M.
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE
 * \brief	main source of iBaby Smarthome Node Wearable Node
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE
 * @{
 */
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "common.h"
#include "lwm2m.h"
#include "process_acc.h"
#include "process_hrate.h"

#include "acceleration.h"
#include "heartrate.h"
#include "body_temperature.h"


/**
 * \brief  main entry
 */
int main(void)
{
	vTaskDelay(200); /* delay time to ensure sensors initialize normally */
	btemp_sensor_init(BTEMP_SENSOR_ADDR); /* initialize body temperature sensor */

	vTaskDelay(50);
	hrate_sensor_init(HRATE_SENSOR_ADDR); /* initialize heartrate sensor */

	vTaskDelay(50);
	acc_sensor_init(ACC_SENSOR_ADDR);     /* initialize acceleration sensor */
	vTaskDelay(50);

#if LWM2M_CLIENT
	/* try to start lwm2m client */
	lwm2m_client_start();
#endif
	/* try to start heartrate detector */
	hrate_detector_start();

	/* try to start sleep monitor and body temperature detecting */
	sleep_monitor_start();

	while(1) {
		vTaskSuspend(NULL);
	}

	/* never run to here */
	return E_SYS;
}

/** @} */