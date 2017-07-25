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

/* custom HAL */
#include "common.h"
#include "lwm2m.h"
#include "print_msg.h"
#include "process_acc.h"
#include "process_hrate.h"

#include "timer1.h"
#include "acceleration.h"
#include "heartrate.h"
#include "body_temperature.h"


#define TIME_DELAY_MS (18)   /* acceleration sampling frequency: 30Hz */

#define THOLD_CNT_AW  (150)  /* threshold of counter(5s) for executing awake event detecting algorithm */
#define THOLD_CNT_SL  (1760) /* threshold of counter(1min：1760) for executing sleep monitoring algorithm */
#define WARN_BTEMP_L  (350)  /* lower value of warning body temperature */
#define WARN_BTEMP_H  (380)  /* upper value of warning body temperature */
#define WARN_HR_MIN   (50)   /* lower value of warning heartrate */
#define WARN_HR_MAX   (150)  /* upper value of warning heartrate */


/**
 * \brief  main entry
 */
int main(void)
{
	uint32_t svm_val;     /* SVM : signal vector magnitude for difference */
	uint32_t cnt_aw;      /* executing algorithm counter */
	uint32_t cnt_sl;      /* counter for sleep monitoring */
	acc_values acc_vals;  /* accleration storage */

	vTaskDelay(200);

	/* initialize body temperature sensor */
	btemp_sensor_init(BTEMP_SENSOR_ADDR);
	vTaskDelay(50);

	/* initialize heartrate sensor */
	hrate_sensor_init(HRATE_SENSOR_ADDR);
	vTaskDelay(50);

	/* initialize acceleration sensor */
	acc_sensor_init(ACC_SENSOR_ADDR);
	vTaskDelay(50);

#if LWM2M_CLIENT
	/* try to start lwm2m client */
	lwm2m_client_start();
#endif

	/* start timer1, timing for transform IR cycle into heartrate(beats per 1min) */
	timer1_start();

	for (;;) {
		/* read acceleration data */
		acc_sensor_read(&acc_vals);

		/* process acceleration data and get SVM(representation of motion intensity) in 1 sample time */
		svm_val = process_acc(acc_vals);

		/* awake event detecting algorithm */
		if (cnt_aw < THOLD_CNT_AW) {
			sum_svm_5s += svm_val; /* summation of SVM in 5s */
			cnt_aw++;
		} else {
			/* detect awake event */
			data_report_wn.event_awake = func_detect_awake(sum_svm_5s);

			/* print out messages for primary function */
#if PRINT_DEBUG_FUNC
			print_msg_func();
#endif/* PRINT_DEBUG_FUNC */

			sum_svm_5s = 0;
			cnt_aw = 0;
		}

		/* sleep monitoring algorithm based on indigital integration method */
		if (cnt_sl < THOLD_CNT_SL) {
			data_report_wn.motion_intensity += svm_val; /* summation of SVM in 1 min */
			cnt_sl++;
		} else {
			/* sleep-wake state monitoring */
			data_report_wn.state = func_detect_state(data_report_wn.motion_intensity);

			data_report_wn.motion_intensity = 0;
			cnt_sl = 0;
		}

		/* initialize the flag of warning */
		data_report_wn.warn_hrate    = false;
		data_report_wn.warn_btemp    = false;
		data_report_wn.warn_downward = false;

		/* detect sleep downward event */
		data_report_wn.warn_downward = func_detect_downward(acc_vals.accl_z);

		/* read body temperature data */
		btemp_sensor_read(&data_report_wn.btemp);

		if (data_report_wn.btemp > WARN_BTEMP_H || data_report_wn.btemp < WARN_BTEMP_L) {
			data_report_wn.warn_btemp = true;
		}

		/* read and process IR value and get heartrate */
		process_hrate(&data_report_wn.hrate);

		if (data_report_wn.hrate < WARN_HR_MIN || data_report_wn.hrate > WARN_HR_MAX) {
			data_report_wn.warn_hrate = true;
		}

		vTaskDelay(TIME_DELAY_MS);
	}

	/* never run to here */
	return E_SYS;
}

/** @} */