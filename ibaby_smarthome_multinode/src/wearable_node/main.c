/* ------------------------------------------
 * Copyright (c) 2016, Synopsys, Inc. All rights reserved.

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
 * \version 2017.06
 * \date 2017-06-26
 * \author Xiangcai Huang(HuangXiangcai666@163.com)
--------------------------------------------- */

/**
 * \defgroup	EMBARC_APP_IBABY		embARC iBaby Wearable Node
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_BOARD_EMSK
 * \ingroup	EMBARC_APPS_OS_LITEOS
 * \ingroup	EMBARC_APPS_MID_LWIP
 * \ingroup	EMBARC_APPS_MID_FATFS
 * \brief	embARC iBaby Wearable Node
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
 *     The Hardware modules should be connected to \ref EMBARC_BOARD_CONNECTION "EMSK".
 *     This example achieves the function of wearable equipment with using embARC.
 *
 * ### Usage Manual
 *     Before compiling this example, you need to change some macro in value.h to customize your own project.
 *
 *     It can report the following information to the iBaby Gateway.
 *     - the Body Temperature (uint32_t) - Wearable module connected to J4 (Based on Temperature sensor)
 *     - the Heartrate (uint32_t) - Wearable module connected to J4 (Based on Heartrate sensor)
 *     - the Motion Intensity in 1min (int) - Wearable module connected to J4 (Based on Accelerometer)
 *     - the Sleep-Wake state (uint) - Wearable module connected to J4 (Based on Accelerometer)
 *     - the Awake Event (uint) - Wearable module connected to J4 (Based on Accelerometer)
 *     - 3 flags of Warning (bool) - Warning of Body Temperature abnormal, Heartrate abnormal, sleep downward
 *
 *     EMSK can send the above data to iBaby Gateway with lwm2m.
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_IBABY
 * \brief	main source of iBaby Wearable node
 */

/**
 * \addtogroup	EMBARC_APP_IBABY
 * @{
 */
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#include "value.h"
#include "task_function.h"

/**
 * \brief  main entry, call LiteOS API, create and start functional task
 */
int main(void)
{
	/* initialize body temperature sensor */
	if (tmp_sensor_init(TMP112_ADDRESS) != E_OK)
	{
		EMBARC_PRINTF("Error: body temperature sensor init failed\r\n");
		return E_SYS;
	}

	/* initialize heartrate sensor */
	if (heart_rate_sensor_init(MAX30102_ADDRESS) != E_OK)
	{
		EMBARC_PRINTF("Error: heartrate sensor init failed\r\n");
		return E_SYS;
	}

	/* initialize acceleration sensor */
	if (imu_sensor_init(IMU_I2C_SLAVE_ADDRESS) != E_OK)
	{
		EMBARC_PRINTF("Error: acceleration sensor init failed\r\n");
		return E_SYS;
	}
	
	/* create task for function */
	if (LOS_TSKCreate((TSK_ENTRY_FUNC)task_function, "task_function", STACK_DEPTH_FUNC, TSKPRI_MID, 
		&task_function_handle) != E_OK)
    {
        EMBARC_PRINTF("Error: Create task_function failed\r\n");
        return E_SYS;
    }
	
	return E_OK;
}

/** @} */
