/* Copyright (c) 2017, Synopsys, Inc. All rights reserved.

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
 * \date 2017-09-1
 * \author WangShuai(mrwangshuai@hust.edu.cn)
--------------------------------------------- */

/**
 * \defgroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE	embARC IOT iLight synthesize
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_OS_FREERTOS
 * \ingroup	EMBARC_APPS_MID_FATFS
 * \brief	embARC IOT iLight smartdevice
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *     [PCF8563](http://www.nxp.com/products/interfaces/ic-bus/ic-real-time-clocks-rtc/real-time-clock-calendar:PCF8563?fsrch=1&sr=1&pageNum=1)
 *     [MPU6050](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/)
 *     [HM-10](http://www.huamaosoft.cn/bluetooth.asp?id=0)
 *     [MLX90614](http://www.nxp.com/products/interfaces/ic-bus/ic-dacs-and-adcs/8-bit-a-d-and-d-a-converter:PCF8591?lang_cd=zh-Hans)
 *
 * ### Design Concept
 *     All sensor modules should be connected to \ref EMBARC_BOARD_CONNECTION "EMSK".
 *     This application is designed to make a smart device to facilitate the daily life.
 *
 * ### Usage Manual
 *
 *     It can report the following information to the iLight.
 *     - the Motion Intensity (uint16_t) - Connected to J3 (Based on Acceleration sensor)
 *     - the MIC (uint8_t) - Connected to J3 (Based on AD conversion)
 *     - the Middle lights(uint16_t) - Connected to J1 
 *     - the Side lights (uint64_t) - Connected to J4 (Based on Acceleration sensor)
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief	main source of iLight Smartdevice 
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"
/* custom HAL*/
#include "light_mode.h"
#include "imu.h"
#include "scope.h"
#include "ble_int.h"
#include "mic.h"
#include "task.h"
 
void main()
{
#ifdef SCOPE_EN
	scope_init(UART_BAUDRATE_115200);
#else
	ble_uart_init(UART_BAUDRATE_9600);
#endif
	light_ctr_init();
	mic_init();
	imu_init(GYRO_RNG,ACCEL_RNG);
	task_init();
	while(1) {
		vTaskSuspend(NULL);
	}
}