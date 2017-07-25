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
 * \brief	function for acceleration data processing
 *              awake event detecting, sleep downward detecting, sleep monitoring
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * @{
 */
/* standard C HAL */
#include <math.h>

/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "common.h"
#include "process_acc.h"
#include "print_msg.h"
#include "acceleration.h"


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
extern uint32_t process_acc(acc_values acc_temp)
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

	/*
	**************************************************************
	*  This part will be deleted in release version
	*/
#if SEND_DEBUG_SVM1_5S
	char str[50];
	sprintf(str, "%d.", svm_new); /* send data to matlab */
	EMBARC_PRINTF(str);
#endif
	/*
	*  end of this part
	**************************************************************
	*/

	/* deal with SVM by filter */
	svm_new = filter_svm(svm_new, svm_old, &flag_old_v, &cnt_v, &par_v);

	svm_old = svm_new;

	/*
	**************************************************************
	*  This part will be deleted in release version
	*/
#if SEND_DEBUG_SVM2_5S
	char str[50];
	sprintf(str, "%d.", svm_new); /* send data to matlab */
	EMBARC_PRINTF(str);
#endif
	/*
	*  end of this part
	**************************************************************
	*/

	return svm_new;
}

/** function for awake event detecting */
extern uint8_t func_detect_awake(uint32_t inten_temp)
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
extern uint8_t func_detect_state(uint32_t inten_temp)
{
	uint8_t state; /* state : SLEEP or WAKE */

	/*
	**************************************************************
	*  This part will be deleted in release version
	*/
#if SEND_DEBUG_INTEN_1M
	char str[50];
	sprintf(str, "%d.", inten_temp); /* send data to matlab */
	EMBARC_PRINTF(str);
#endif
	/*
	*  end of this part
	**************************************************************
	*/

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
extern bool func_detect_downward(float acc_temp)
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