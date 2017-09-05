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
 * \author Jingru Wang(nuannuan12016@outlook.com)
--------------------------------------------- */
/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief	Header File of task function
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */
#ifndef _TASK_H_
#define _TASK_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "light_mode.h"
#include "light_driver.h"
#include "imu.h"
#include "scope.h"
#include "mic.h"
#include "mpu6050.h"
#include "rtc.h"

#define MPU_PERIOD_TICK		15
#define REC_PERIOD_TICK		30
#define MIC_PERIOD_TICK		5
#define ALARM_PERIOD_TICK	15000

#define INIT_MODE		0x11
#define INIT_BRIGHT		0
#define INIT_FREQUENCE		0
#define INIT_TIME		0
#define INIT_TEMPERTURE		0
#define INIT_HUMIDITY		0
#define INIT_WEATHER		0

#define FANCE_TXT_LEN_MAX	20
#define LED_WAIT_TIME_MAX	1000

#define MIN_MODE		LIGHT_MODE_RUNNING
#define MAX_MODE		LIGHT_MODE_SOS
#define MIN_BRIGHT		1
#define MAX_BRIGHT		127

#define INIT_MODE_INFO {	\
	INIT_MODE,		\
	INIT_BRIGHT,		\
	INIT_FREQUENCE,		\
	INIT_TIME,		\
	INIT_WEATHER,		\
	INIT_TEMPERTURE,	\
	INIT_HUMIDITY,		\
	{0}			\
}

typedef struct {
	uint8_t mode;
	uint8_t bright;
	uint8_t frequence;
	uint32_t time;
	uint8_t weather;
	uint8_t temperture;
	uint8_t humidity;
	uint8_t fance_txt[FANCE_TXT_LEN_MAX];
} mode_info;

extern TaskHandle_t pattern_change_task_handle;
extern SemaphoreHandle_t recognition_semaphore;
extern QueueHandle_t led_queue;
extern QueueHandle_t mode_info_queue;

extern uint8_t color_row;
extern uint8_t light_num;

extern uint8_t task_init(void);


#endif /* _TASK_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE */