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
#include "function.h"


#define DELAY_TIME_SLICE (29)/**< consumption time 4.2ms + 29ms = sampling frequency : 33.3ms(30Hz) */

#define THOLD_CNT_AW (150)   /**< threshold of counter(5s) for executing awake event detecting algorithm */
#define THOLD_CNT_SL (1760)  /**< threshold of counter(1min：1760) for executing sleep monitoring algorithm */

#define WARN_BTEMP_L (350)   /**< lower value of warning body temperature */
#define WARN_BTEMP_H (380)   /**< upper value of warning body temperature */
#define WARN_HR_MIN  (50)    /**< lower value of warning heartrate */
#define WARN_HR_MAX  (150)   /**< upper value of warning heartrate */


/**
 * \brief  main entry, call Freertos API, create and start functional task
 */
int main(void)
{
	int  svm_val;         /* SVM : signal vector magnitude for difference */
	int  cnt_aw;          /* executing algorithm counter */
	bool flag_start_aw;   /* flag of awake event detecting start */
	int  cnt_sl;          /* counter for sleep monitoring */
	bool flag_start_sl;   /* flag of sleep monitoring start */
	acc_values acc_vals;  /* accleration storage */

	EMBARC_PRINTF("\r\n\
		*********************************************************\r\n\
		*                 iBaby Smarthome Nodes                 *\r\n\
		*                     Wearable Node                     *\r\n\
		*********************************************************\r\n");

	/* initialize body temperature sensor */
	if (btemp_sensor_init(TMP112_ADDRESS) != E_OK) {
		EMBARC_PRINTF("Error: body temperature sensor init failed\r\n");
		return E_SYS;
	}

	/* initialize heartrate sensor */
	if (hrate_sensor_init(MAX30102_ADDRESS) != E_OK) {
		EMBARC_PRINTF("Error: heartrate sensor init failed\r\n");
		return E_SYS;
	}

	/* initialize acceleration sensor */
	if (acc_sensor_init(IMU_I2C_SLAVE_ADDRESS) != E_OK) {
		EMBARC_PRINTF("Error: acceleration sensor init failed\r\n");
		return E_SYS;
	}
	
	#if LWM2M_CLIENT
	/* try to start lwm2m client */
	lwm2m_client_start();
	#endif

/*
**************************************************************
*  This part will be deleted in release version
*/
	vTaskDelay(DELAY_TIME_SLICE * 10); 

	/*  initialize accelerometer before read */
	acc_sensor_init(IMU_I2C_SLAVE_ADDRESS);
	vTaskDelay(DELAY_TIME_SLICE * 2); 

	/* initialize heartrate sensor before read */
	hrate_sensor_init(HEART_RATE_I2C_SLAVE_ADDRESS);
	vTaskDelay(DELAY_TIME_SLICE * 2);
/*
*  end of this part
**************************************************************
*/

	for(;;) {
/*
**************************************************************
*  This part will be deleted in release version
*/
		/* start timer1 for calculating the time of task running */
		#if USED_TIMER1
		timer1_start();
		#endif
/*
*  end of this part
**************************************************************
*/

		/* read acceleration data every 33ms */
		acc_sensor_read(&acc_vals);

		/* process raw accelerationdata and calculate SVM(representation of motion intensity) in 33ms */
		svm_val = process_acc(acc_vals);

		/* awake event detecting algorithm */
		if (cnt_aw < THOLD_CNT_AW) {
			/* summation of SVM in 5s */
			sum_svm_5s += svm_val;
			cnt_aw++;
		} else {
			/* remove the error value in the beginning */
			if (!flag_start_aw) {
				sum_svm_5s = 0;
				flag_start_aw = true;
			}

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
			/* summation of SVM in 1 min */
			data_report_wn.motion_intensity += svm_val;
			cnt_sl++;
		} else {
			/* remove the error value in the beginning */
			if (!flag_start_sl) {
				data_report_wn.motion_intensity = 0;
				flag_start_sl = true;
			}

			/* sleep-wake state monitoring */
			data_report_wn.state = func_detect_state(data_report_wn.motion_intensity);

			data_report_wn.motion_intensity = 0;
			cnt_sl = 0;
		}

		/* initialize the flag_warn */
		data_report_wn.warn_hrate    = false;
		data_report_wn.warn_btemp    = false;
		data_report_wn.warn_downward = false;

		/* 
		* detect warn of sleep on his stomach
		* detect sleep downward event
		*/
		data_report_wn.warn_downward = func_detect_downward(acc_vals.accl_z);

		/* read body temperature data */
		btemp_sensor_read(&data_report_wn.btemp);

		if (data_report_wn.btemp > WARN_BTEMP_H || data_report_wn.btemp < WARN_BTEMP_L) {
			data_report_wn.warn_btemp = true;
		}

		/* read heartrate data and process them by fft and filter */
		process_hrate(&data_report_wn.hrate);

		if (data_report_wn.hrate < WARN_HR_MIN || data_report_wn.hrate > WARN_HR_MAX) {
			data_report_wn.warn_hrate = true;
		}
	
		vTaskDelay(DELAY_TIME_SLICE); 

/*
**************************************************************
*  This part will be deleted in release version
*/
		/* stop timer1 and print out the time of task running */		
		#if USED_TIMER1
		timer1_stop();
		#endif
/*
*  end of this part
**************************************************************
*/

	}
	
	return E_OK;
}

/** @} */
