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
 * \ingroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE
 * \brief	function for acceleration data processing
 *              awake event detecting, sleep downward detecting, sleep monitoring
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE
 * @{
 */
/* standard C HAL */
#include <math.h>

/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "common.h"
#include "print_msg.h"
#include "process_acc.h"

#include "acceleration.h"
#include "body_temperature.h"


#define STACK_DEPTH_ACC (1024) /* stack depth for heartrate detector : word(4*bytes) */
#define TIME_DELAY_ACC_MS (31) /* acceleration sampling frequency: 30Hz */

#define THOLD_CNT_AW    (150)  /* threshold of counter(5s) for executing awake event detecting algorithm */
#define THOLD_CNT_SL    (1760) /* threshold of counter(1min：1760) for executing sleep monitoring algorithm */
#define THOLD_CNT_BTEMP (1760) /* threshold of counter(1min：1760) for body temperature sampling time */

#define WARN_BTEMP_L  (350)  /* lower value of warning body temperature */
#define WARN_BTEMP_H  (380)  /* upper value of warning body temperature */
#define WARN_ACCL_Z    (-8)  /* lower value of warning acceleration */

/* parameters of low pass filter for latest acceleration value */
#define PAR_ACC_BASE   (30)  /* base value of low pass filter coefficient */
#define PAR_ACC_STEP   (5)   /* step of coefficient enlarge */
#define CNT_ACC_STEP   (5)   /* step of counter enlarge */
#define THOLD_ACC_DIFF (300) /* threshold of acceleration value variation range, counter enlarge */
#define THOLD_ACC_CNT  (30)  /* threshold of counter, coefficient enlarge  */

/* parameters of low pass filter for latest motion intersity */
#define PAR_SVM_BASE   (80)  /* base value of low pass filter coefficient */
#define PAR_SVM_STEP   (20)  /* step of coefficient enlarge */
#define CNT_SVM_STEP   (5)   /* step of counter enlarge */
#define THOLD_SVM_DIFF (30)  /* threshold of acceleration value variation range, counter enlarge */
#define THOLD_SVM_CNT  (50)  /* threshold of counter, coefficient enlarge */

/* parameters of limiting filter */
#define MAX_LIMIT_SVM (200)  /* upper limit of svm */
#define MIN_LIMIT_SVM (5)    /* lower limit of svm */

#define THOLD_INTEN_AW (1000) /* threshold of motion intensity */
#define LEN_STA_WAKE   (5)    /* length of state queue indicated baby awake */
#define LEN_STA_SLEEP  (LEN_STA_QUEUE - LEN_STA_WAKE)
#define PAR_STA_AW     (0.75) /* proportion of wake-state in total queue indicated baby awake */

/* sparameters of Webster sleep-wake determine method */
#define P  (0.0046) /* coefficient of scale */
#define K0 (0.04)   /* coefficient of relationship */
#define K1 (0.22)
#define K2 (1)
#define K3 (0.24)
#define K4 (0.06)

/* variable of low pass filter for raw acceleration data */
static char cnt_x, cnt_y, cnt_z;                /* low pass filter counter */
static unsigned char par_x, par_y, par_z;       /* low pass filter coefficient */
static bool flag_old_x, flag_old_y, flag_old_z; /* flag of last change direction of value */

/* variable of calculating intensity of motion in one sampling period */
static int16_t  x_old, y_old, z_old; /* last value */
static uint32_t svm_old;             /* SVM : signal vector magnitude for difference */

/* variable of low pass filter for svm data */
static char cnt_v;          /* low pass filter counter */
static unsigned char par_v; /* low pass filter coefficient */
static bool flag_old_v;     /* flag of change direction of value */

static uint32_t process_acc(acc_values acc_temp);
static uint8_t  func_detect_awake(uint32_t inten_temp);
static uint8_t  func_detect_state(uint32_t inten_temp);
static bool     func_detect_downward(float acc_temp);

static void task_sleep_monitor(void *par);
static TaskHandle_t task_sleep_monitor_handle = NULL;


/** function for starting sleep monitor and body temperature detecting */
extern void sleep_monitor_start(void)
{
	/* create task for sleep monitor */
	if (xTaskCreate(task_sleep_monitor, "sleep monitor", STACK_DEPTH_ACC, NULL, TSKPRI_MID,
	                &task_sleep_monitor_handle) != pdPASS) {
		EMBARC_PRINTF("Error: Create task_sleep_monitor failed\r\n");
	}
}

/** task for sleep monitoring */
static void task_sleep_monitor(void *par)
{
	uint32_t svm_val;       /* SVM : signal vector magnitude for difference */
	uint32_t cnt_aw = 0;    /* executing algorithm counter */
	uint32_t cnt_sl = 0;    /* counter for sleep monitoring */
	uint32_t cnt_btemp = 0; /* counter for body temperature sampling */
	acc_values acc_vals;    /* accleration storage */

	while (1) {
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

		data_report_wn.warn_downward = false;

		/* detect sleep downward event */
		data_report_wn.warn_downward = func_detect_downward(acc_vals.accl_z);


		/* read body temperature data */
		if (cnt_btemp < THOLD_CNT_BTEMP) {
			cnt_btemp++;
		} else {
			btemp_sensor_read(&data_report_wn.btemp);

			data_report_wn.warn_btemp = false;
			if (data_report_wn.btemp > WARN_BTEMP_H || data_report_wn.btemp < WARN_BTEMP_L) {
				data_report_wn.warn_btemp = true;
			}

			cnt_btemp = 0;
		}

		vTaskDelay(TIME_DELAY_ACC_MS);
	}
}

/** function for deal with acclerate by filter */
static int filter_acc(int val_new,
                      int val_old,
                      bool *flag_old,
                      char *cnt,
                      unsigned char *par)
{
	int  val_diff;
	bool flag_new;

	/* calculate low pass filter coefficient for x axis acceleration */
	val_diff = val_new - val_old;

	if (val_diff > 0) {
		flag_new = true;
	} else {
		flag_new = false;
	}

	if (flag_new == *flag_old) {
		(*cnt)++;
		if (fabs(val_diff) > THOLD_ACC_DIFF) {
			*cnt += CNT_ACC_STEP;
		}
		if (*cnt > THOLD_ACC_CNT) {
			*par += PAR_ACC_STEP;
			*cnt = 0;
		}
	} else {
		*cnt  = 0;
		*par = PAR_ACC_BASE;
	}

	*flag_old = flag_new;

	/* calculate output of low pass filter */
	if (!val_diff) {
		val_new = val_old;
	} else {
		val_new = val_old + val_diff * (*par) / 256;
	}

	return val_new;
}

/** function for deal with SVM by filter */
static int filter_svm(int val_new,
                      int val_old,
                      bool *flag_old,
                      char *cnt,
                      unsigned char *par)
{
	int val_diff;
	bool flag_new;

	/* deal with SVM by limiting filter */
	if (val_new > MAX_LIMIT_SVM) {
		val_new = MAX_LIMIT_SVM;
	}

	/* deal with SVM by low pass filter */
	val_diff = val_new - val_old;

	if (val_diff) {
		/* calculate low pass filter coefficient for SVM */
		if (val_diff > 0) {
			flag_new = true;
		} else {
			flag_new = false;
		}

		if (flag_new == *flag_old) {
			(*cnt)++;
			if (fabs(val_diff) > THOLD_SVM_DIFF) {
				*cnt += CNT_SVM_STEP;
			}
			if (*cnt > THOLD_SVM_CNT) {
				*par += PAR_SVM_STEP;
				*cnt = 0;
			}
		} else {
			*cnt = 0;
			*par = PAR_SVM_BASE;
		}

		*flag_old = flag_new;
	} else {
		*cnt = 0;
		*par = PAR_SVM_BASE;
	}

	/* calculate output of low pass filter */
	if (!val_diff) {
		val_new = val_old;
	} else {
		val_new = val_old + val_diff * (*par) / 256;
	}

	/* deal with SVM by limiting filter */
	if (val_new < MIN_LIMIT_SVM) {
		val_new = 0;
	}

	return val_new;
}

/** function for processing accelerate raw data */
static uint32_t process_acc(acc_values acc_temp)
{
	int16_t  x_new, y_new, z_new; /* latest value */
	uint32_t  svm_new;            /* SVM : signal vector magnitude for difference */

	x_new = (int16_t)(100 * acc_temp.accl_x);
	y_new = (int16_t)(100 * acc_temp.accl_y);
	z_new = (int16_t)(100 * acc_temp.accl_z);

	/* deal with raw accelerate data by filter */
	x_new = filter_acc(x_new, x_old, &flag_old_x, &cnt_x, &par_x);
	y_new = filter_acc(y_new, y_old, &flag_old_y, &cnt_y, &par_y);
	z_new = filter_acc(z_new, z_old, &flag_old_z, &cnt_z, &par_z);

	/* calculate SVM using data fusion */
	svm_new = sqrt((x_new - x_old) * (x_new - x_old) +
	               (y_new - y_old) * (y_new - y_old) +
	               (z_new - z_old) * (z_new - z_old));

	x_old = x_new;
	y_old = y_new;
	z_old = z_new;

	/* deal with SVM by filter */
	svm_new = filter_svm(svm_new, svm_old, &flag_old_v, &cnt_v, &par_v);
	svm_old = svm_new;

	return svm_new;
}

/** function for awake event detecting */
static uint8_t func_detect_awake(uint32_t inten_temp)
{
	bool flag_break_aw = false;
	uint8_t cnt_sl_aw = 0;
	uint8_t cnt_wk_aw = 0;
	uint8_t event = NOEVENT; /* flag of event : NOEVENT or AWAKE */

	/* judge current status */
	if (inten_temp > THOLD_INTEN_AW) {
		state_aw[0] = 0; /* in wake state now */
	} else {
		state_aw[0] = 1; /* in sleep state now */
	}

	/* the number of sleep state in front of the state queue */
	for (uint8_t i = LEN_STA_QUEUE - 1; i > LEN_STA_WAKE - 1; --i) {
		if (flag_break_aw) {
			flag_break_aw = false;
			break;
		}

		if (state_aw[i]) {
			cnt_sl_aw++;
		}

		if (cnt_sl_aw > LEN_STA_SLEEP * PAR_STA_AW) {
			cnt_sl_aw = 0;

			/* statistics the number of wake state in back of the state queue */
			for (uint8_t i = 0; i < LEN_STA_WAKE; ++i) {
				if (!state_aw[i]) {
					cnt_wk_aw++;
				}

				if (cnt_wk_aw > LEN_STA_WAKE * PAR_STA_AW) {
					event = AWAKE;        /* wake up event detected */
					flag_break_aw = true; /* go exit from for loop */
				} else {
					event = NOEVENT;
				}
			}
		} else {
			event = NOEVENT;
		}
	}

	cnt_sl_aw = 0;
	cnt_wk_aw = 0;

#if PRINT_DEBUG_AWAKE
	print_msg_awake(); /* print out message for debug */
#endif

	/* update motion intensity for LEN_STA_QUEUE * 5s */
	for (uint8_t i = LEN_STA_QUEUE - 1; i > 0; --i) {
		state_aw[i] = state_aw[i - 1];
	}

	return event;
}

/** function for sleep-wake state detecting */
static uint8_t func_detect_state(uint32_t inten_temp)
{
	uint8_t state; /* state : SLEEP or WAKE */

	inten_sl[0] = inten_temp; /* motion intensity in 1min */

	/* calculate the score of 2min ago(inten_sl[2]) by Webster sleep-wake determine method */
	score_sl = P * (K4 * inten_sl[4] + K3 * inten_sl[3] +
	                K2 * inten_sl[2] + K1 * inten_sl[1] +
	                K0 * inten_sl[0]) / 100;

	/* determine the state of 2min ago by score */
	if (score_sl > 1) {
		state = WAKE;
	} else {
		state = SLEEP;
	}

#if PRINT_DEBUG_SLEEP
	print_msg_sleep(state); /* print out message for debug */
#endif

	/* update motion intensity for 5 * 1min */
	for (uint8_t i = 4; i > 0; --i) {
		inten_sl[i] = inten_sl[i - 1];
	}

	return state;
}

/** function for sleep downward state detecting */
static bool func_detect_downward(float acc_temp)
{
	bool warn;

	if (acc_temp < WARN_ACCL_Z) {
		warn = true;
	} else {
		warn = false;
	}

	return warn;
}

/** @} */