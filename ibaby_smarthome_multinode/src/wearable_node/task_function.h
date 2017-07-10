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
 * \file
 * \ingroup	TASK_FUNCTION_OBJ
 * \brief	header file of functional task for iBaby wearable node
 */

/**
 * \addtogroup	EMBARC_APP_IBABY
 * @{
 */
#ifndef TASK_FUNCTION_H
#define TASK_FUNCTION_H

/* standard C HAL */
#include <math.h>

/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"
#include "dw_gpio.h"
#include "board.h"

/* custom HAL */
#include "imu.h"
#include "max.h"
#include "tmp.h"
#include "fft.h"
#include "value.h"

/**
 * \name	macro of led
 * @{
 */
#define LED_LEDWARN (0x01)     /*!< Indicator Light of Warning */

#define LED_ON  (0xff)
#define LED_OFF (0x0)
/** @} end of name */

/**
 * \name	macro of timer1
 * @{
 */
#if USED_TIMER1
//#define T1_COUNT_UNIT_MS  (30000) /*!< counting accuracy of Timer1: 1ms */
#define T1_COUNT_UNIT_LMS (3000)  /*!< counting accuracy of Timer1: 0.1ms */

static int t1_count;/*!< counter for timer1 */

static void timer1_start(void);
static void timer1_stop(void);
#endif/* USED_TIMER1 */
/** @} end of name */


/**
 * \name	macro for LiteOS task
 * @{
 */
#define STACK_DEPTH_FUNC  (6000)/*!< stack depth : word(4*bytes) */
#define STACK_DEPTH_LWM2M (20480)/*!< stack depth : word(4*bytes) */

/*!< priority */
#define TSKPRI_HIGHEST (LOS_TASK_PRIORITY_HIGHEST+1)/*!< highest priority */
#define TSKPRI_HIGH    (LOS_TASK_PRIORITY_HIGHEST+2)/*!< high priority */
#define TSKPRI_MID     (LOS_TASK_PRIORITY_HIGHEST+3)/*!< middle priority */
#define TSKPRI_LOW     (LOS_TASK_PRIORITY_HIGHEST+4)/*!< low priority */

extern int LOS_TSKCreate(TSK_ENTRY_FUNC pfnTaskEntry,
				  char *pcName,//const char * const pcName,
                  const uint16_t uwStackSize,
                  UINT32 usTaskPrio,
                  UINT32 *puwTaskID);
/** @} end of name */

/**
 * \name	macro for heartrate data processing
 * @{
 */
#define DELAY_TIME_SLICE (3) /*!< 33：sampling frequency : 30Hz(32.7ms) */
#define THOLD_CNT_SEN (150)  /*!< count value of 5s : 150 * 1/30s */
static int  cnt_sen;         /*!< counter for temperature, heartrate acqusition */
extern void task_function(void * par);
static UINT32 task_function_handle;
/** @} end of name */

/**
 * \name	macro for print function
 * @{
 */
#if PRINT_DEBUG_MA
static void print_msg_ma(void);/*!< print message for debug major function */
#endif/* PRINT_DEBUG_MA */

#if PRINT_DEBUG_AW
static void print_msg_aw(void);/*!< print message for debug awake event detecting function */
#endif/* PRINT_DEBUG_AW */

#if PRINT_DEBUG_SL
static void print_msg_sl(uint state);/*!< print message for debug sleep-wake state monitoring function */
#endif/* PRINT_DEBUG_SL */
/** @} end of name */

/**
 * \name	macro for lwm2m client
 * @{
 */
#if LWM2M_CLIENT
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

static lwm2m_client_info c_info;       /*!< lwM2M client info */
static int lwm2m_client_conn_stat = 0; /*!< flag of connect status lwM2M client */
static int lwm2m_client_start_flag = 0;/*!< flag of start of lwM2M client */

static int lwm2m_client_start(void);
static void task_lwm2m_client(void *par);
static UINT32 task_lwm2m_client_handle;
#endif/* LWM2M_CLIENT */
/** @} end of name */


/**
 * \name	macro for special function of wearable node
 * @{
 */
#define WARN_BTEMP_L  (320)  /*!< lower value of warning body temperature */
#define WARN_BTEMP_H  (380)  /*!< upper value of warning body temperature */
#define WARN_HR_MIN   (50)   /*!< lower value of warning heartrate */
#define WARN_HR_MAX   (150)  /*!< upper value of warning heartrate */
#define WARN_ACCL_Z   (-8)   /*!< lower value of warning accelerate */
 
#define FFT_DELTA 		1 / (FFT_LEN * dtT)

/*!< variable of read heartrate data */
static int cnt_h, flag_h;
static int hrate_group[FFT_LEN],hrate_temp;
static int sum_h;

/* function for deal with heartrate by filter */
static void filter_hrate(uint32_t* hrate);

static acc_values acc_vals;   /*!< accleration */

/*!< variable of read accelerate data and calculate intensity of motion in one sampling period */
static int x_old, y_old, z_old;   /*!< last value */
static int svm_val, svm_old;      /*!< SVM : signal vector magnitude for difference */

/** function for processing accelerate raw data */
static int process_acc_val(acc_values acc_temp);

/* function for deal with acclerate by filter */
static int filter_acc(int val_new, int val_old, bool *flag_old, char *cnt, unsigned char *par);

/* function for deal with SVM by filter */
static int filter_svm(int val_new, int val_old, bool *flag_old, char *cnt, unsigned char *par);

/*!< parameters of low pass filter for latest accelerate value */
#define PAR_ACC_BASE (30)   /*!< base value of low pass filter coefficient */
#define PAR_ACC_STEP (5)    /*!< step of coefficient enlarge */
#define CNT_ACC_STEP (5)    /*!< step of counter enlarge */
#define THOLD_ACC_DIFF (300)/*!< threshold of accelerate value variation range, counter enlarge */
#define THOLD_ACC_CNT  (30) /*!< threshold of counter, coefficient enlarge  */

static char cnt_x, cnt_y, cnt_z;                /* low pass filter counter */
static unsigned char par_x, par_y, par_z;       /* low pass filter coefficient */
static bool flag_old_x, flag_old_y, flag_old_z; /*!< flag of last change direction of value */


/*!< parameters of low pass filter for latest motion intersity */
#define PAR_SVM_BASE (80)   /*!< base value of low pass filter coefficient */
#define PAR_SVM_STEP (20)   /*!< step of coefficient enlarge */
#define CNT_SVM_STEP (5)    /*!< step of counter enlarge */
#define THOLD_SVM_DIFF (30) /*!< threshold of accelerate value variation range, counter enlarge */
#define THOLD_SVM_CNT  (50) /*!< threshold of counter, coefficient enlarge */

static char cnt_v;                 /* low pass filter counter */
static unsigned char par_v;        /* low pass filter coefficient */
static bool flag_old_v;            /*!< flag of change direction of value */


/*!< parameters of limiting filter */
#define MAX_LIMIT_SVM (200)/*!< upper limit of svm */
#define MIN_LIMIT_SVM (5)  /*!< lower limit of svm */


/*!< parameters of awake event detecting */
#if FUNC_DETECT_AWAKE
#define AWAKE   (1)/*!< wake up event */
#define NOEVENT (0)/*!< no event */

#define THOLD_CNT_AW   (150) /*!< threshold of counter(5s) for executing awake event detecting algorithm */
#define THOLD_INTEN_AW (1000)/*!< threshold of motion intensity */
#define LEN_STA_QUEUE  (7)  /*!< length of state queue used to detect awake event */
#define LEN_STA_WAKE   (2)   /*!< length of state queue indicated baby awake */
#define LEN_STA_SLEEP  (3)
#define PAR_STA_AW     (0.75)/*!< proportion of wake-state in total queue indicated baby awake */
static int  inten_motion_aw;        /*!< motion intensity in 5s */
static int  cnt_aw;                 /*!< executing algorithm counter */
static int  state_aw[LEN_STA_QUEUE];/*!< state for LEN_STA_QUEUE * 5s */
static bool flag_start_aw;          /*!< flag of awake event detecting start */

static uint func_detect_aw(int inten_temp);
#endif/* FUNC_DETECT_AWAKE */


/*!< parameters of sleep monitoring algorithm */
#if FUNC_MONITOR_SLEEP
#define SLEEP   (1)/*!< sleep state */
#define WAKE    (0)/*!< wake state */

#define THOLD_CNT_SL (150)/*!< threshold of counter(1min：1760) for executing sleep monitoring algorithm */

/*!< sparameters of Webster sleep-wake determine method */
#define P  (0.01)/*!< 0.0046:coefficient of scale */
#define K0 (0.04)  /*!< coefficient of relationship */
#define K1 (0.22)
#define K2 (1)
#define K3 (0.24)
#define K4 (0.06)

static int   cnt_sl;        /*!< counter for sleep monitoring */
static int   inten_sl[5];   /*!< motion intensity for 5 * 1min */
static float score_sl;      /*!< score of motion */
static bool  flag_start_sl; /*!< flag of sleep monitoring start */

static uint func_detect_sl(int inten_temp);
#endif/* FUNC_MONITOR_SLEEP */


#if FUNC_DETECT_DOWN
static bool func_detect_dw(float acc_temp);
#endif/* FUNC_DETECT_DOWN */

/** @} end of name */


#endif/* _TASK_FUNCTION_OBJ_ */

/** @} end of group TASK_FUNCTION_OBJ */