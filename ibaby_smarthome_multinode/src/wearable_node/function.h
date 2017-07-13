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
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * \brief	Header File of Functio Task for iBaby Wearable Node
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
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

/* custom HAL */
#include "acceleration.h"
#include "heartrate.h"
#include "body_temperature.h"
#include "value.h"


/*
**************************************************************
*  This part will be deleted in release version
*/
/**
 * \name	macro of timer1
 * @{
 */
#if USED_TIMER1
//#define T1_COUNT_UNIT_MS  (20000) /*!< counting accuracy of Timer1: 1ms */
#define T1_COUNT_UNIT_LMS (2000)  /*!< counting accuracy of Timer1: 0.1ms */

static int t1_count; /*!< counter for timer1 */

extern void timer1_start(void);
extern void timer1_stop(void);
#endif/* USED_TIMER1 */
/** @} end of name */
/*
*  end of this part
**************************************************************
*/


/**
 * \name	local macro for lwm2m client
 * @{
 */
static lwm2m_client_info c_info;        /*!< lwM2M client info */
static int lwm2m_client_conn_stat = 0;  /*!< flag of connect status lwM2M client */
static int lwm2m_client_start_flag = 0; /*!< flag of start of lwM2M client */

static void task_lwm2m_client(void *par);
static TaskHandle_t task_lwm2m_client_handle = NULL;
/** @} end of name */


/**
 * \name	local macro for Freertos task
 * @{
 */
#define STACK_DEPTH_LWM2M (20480) /*!< stack depth for lwm2mClient : word(4*bytes) */

/*!< priority */
#define TSKPRI_HIGH (configMAX_PRIORITIES-2) /*!< high priority */
#define TSKPRI_MID  (configMAX_PRIORITIES-3) /*!< middle priority */
#define TSKPRI_LOW  (configMAX_PRIORITIES-4) /*!< low priority */
/** @} end of name */


/**
 * \name	local macro for function of wearable node
 * @{
 */
#define WARN_ACCL_Z (-8) /*!< lower value of warning acceleration */

/*!< parameters of fast fourier transform function */
#define FFT_DELTA (1 / (FFT_LEN * DTT))
#define DTT       (0.02)                 /*!< error coefficient */
#define FFT_M 	  (9)                    /*!< series of fast fourier transform */
#define FFT_LEN   (1 << FFT_M)           /*!< amount of discrete points */
#define NUM_TAPS  (FFT_LEN)
#define S16MAX	  (32767)                /*!< upper value of 16bits */
#define S16MIN	  (-32767)               /*!< lower value of 16bits */
#define PI        (3.1415926535897932385)/*!< value of pi */

/*!< parameters of heartrate sensor processing function */
#define MIN_HRATE_VAL     (10000)  /*!< upper limit of heartrate */
#define MAX_HRATE_VAL     (120000) /*!< lower limit of heartrate */
#define THOLD_HRATE_DIFF  (1000)   /*!< threshold of heartrate value variation range */
#define DEFAULT_HRATE_VAL (750)    /*!< default value of heartrate */

/*!< parameters of low pass filter for latest acceleration value */
#define PAR_ACC_BASE   (30) /*!< base value of low pass filter coefficient */
#define PAR_ACC_STEP   (5)  /*!< step of coefficient enlarge */
#define CNT_ACC_STEP   (5)  /*!< step of counter enlarge */
#define THOLD_ACC_DIFF (300)/*!< threshold of acceleration value variation range, counter enlarge */
#define THOLD_ACC_CNT  (30) /*!< threshold of counter, coefficient enlarge  */

/*!< parameters of low pass filter for latest motion intersity */
#define PAR_SVM_BASE   (80) /*!< base value of low pass filter coefficient */
#define PAR_SVM_STEP   (20) /*!< step of coefficient enlarge */
#define CNT_SVM_STEP   (5)  /*!< step of counter enlarge */
#define THOLD_SVM_DIFF (30) /*!< threshold of acceleration value variation range, counter enlarge */
#define THOLD_SVM_CNT  (50) /*!< threshold of counter, coefficient enlarge */

/*!< parameters of limiting filter */
#define MAX_LIMIT_SVM (200) /*!< upper limit of svm */
#define MIN_LIMIT_SVM (5)   /*!< lower limit of svm */

/*!< parameters of awake event detecting */
#define AWAKE   (1) /*!< wake up event */
#define NOEVENT (0) /*!< no event */

#define THOLD_INTEN_AW (1000)/*!< threshold of motion intensity */
#define LEN_STA_QUEUE  (20)  /*!< length of state queue used to detect awake event */
#define LEN_STA_WAKE   (5)   /*!< length of state queue indicated baby awake */
#define LEN_STA_SLEEP  (LEN_STA_QUEUE - LEN_STA_WAKE)
#define PAR_STA_AW     (0.75)/*!< proportion of wake-state in total queue indicated baby awake */

/*!< parameters of sleep monitoring algorithm */
#define SLEEP   (1) /*!< sleep state */
#define WAKE    (0) /*!< wake state */

/*!< sparameters of Webster sleep-wake determine method */
#define P  (0.0046) /*!< 0.0046:coefficient of scale */
#define K0 (0.04)   /*!< coefficient of relationship */
#define K1 (0.22)
#define K2 (1)
#define K3 (0.24)
#define K4 (0.06)


/*!< typedef for heartrate data processing */
typedef struct
{ 
	float real;
	float imag;
}compx;

typedef struct _Cplx16
{
	int R;
	int I;
}Cplx16;


/*!< variable of heartrate data processing */
Cplx16 fft_que[FFT_LEN];   /*!< value of discrete points after fft */
Cplx16 par_w[FFT_LEN / 2]; /*!< parameters of fft */

static int  cnt_hrate;           /*!< number of heartrate data counter */
static bool flag_hrate;          /*!< flag of starting to report heartrate */
static int  hrate_group[FFT_LEN];/*!< temporary value of heartrate */
static int  hrate_temp;          /*!< temporary value of heartrate */
static int  sum_hrate;           /*!< summation of heartrate value */

/*!< variable of low pass filter for raw acceleration data */
static char cnt_x, cnt_y, cnt_z;                /*!< low pass filter counter */
static unsigned char par_x, par_y, par_z;       /*!< low pass filter coefficient */
static bool flag_old_x, flag_old_y, flag_old_z; /*!< flag of last change direction of value */

/*!< variable of calculating intensity of motion in one sampling period */
static int x_old, y_old, z_old; /*!< last value */
static int svm_old;             /*!< SVM : signal vector magnitude for difference */

/*!< variable of low pass filter for svm data */
static char cnt_v;          /*!< low pass filter counter */
static unsigned char par_v; /*!< low pass filter coefficient */
static bool flag_old_v;     /*!< flag of change direction of value */

/*!< variable of detecting awake event */
int  sum_svm_5s;                     /*!< motion intensity in 5s */
static int  state_aw[LEN_STA_QUEUE]; /*!< state for LEN_STA_QUEUE * 5s */

/*!< variable of sleep monitoring */
static int   inten_sl[5]; /*!< motion intensity for 5 * 1min */
static float score_sl;    /*!< score of motion */

/* function for deal with acclerate by filter */
static int filter_acc(int val_new, int val_old, bool *flag_old, char *cnt, unsigned char *par);

/* function for deal with SVM by filter */
static int filter_svm(int val_new, int val_old, bool *flag_old, char *cnt, unsigned char *par);


#if PRINT_DEBUG_FUNC
/*!< print message for debug major function */
extern void  print_msg_func(void);
#endif/* PRINT_DEBUG_FUNC */

#if PRINT_DEBUG_AWAKE
/*!< print message for debug awake event detecting function */
extern void  print_msg_awake(void);
#endif/* PRINT_DEBUG_AWAKE */

#if PRINT_DEBUG_SLEEP
/*!< print message for debug sleep-wake state monitoring function */
extern void  print_msg_sleep(uint state);
#endif/* PRINT_DEBUG_SLEEP */

/** function for starting lwm2mClient */
extern int   lwm2m_client_start(void);

/** function for calculate parameters for fast fourier transform */
extern void  calc_par_w(Cplx16 *W);

/** function for fast fourier transform */
extern void  fft(Cplx16 *D, Cplx16 *W);

/** function for find the max value */
extern float find_max(Cplx16 *D);

/** function for deal with heartrate by filter */
extern void  process_hrate(uint32_t* hrate);

/** function for processing acceleration raw data */
extern int   process_acc(acc_values acc_temp);

/** function for awake event detecting */
extern uint  func_detect_awake(int inten_temp);

/** function for sleep-wake state detecting */
extern uint  func_detect_state(int inten_temp);

/** function for sleep downward state detecting */
extern bool  func_detect_downward(float acc_temp);

/** @} end of name */


#endif/* _EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE */