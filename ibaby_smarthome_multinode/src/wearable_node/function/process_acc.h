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
 * \brief	Header File of acceleration data processing function
 *              awake event detecting, sleep downward detecting, sleep monitoring
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * @{
 */
#ifndef PROCESS_ACC_H
#define PROCESS_ACC_H


#include "acceleration.h"


/*!< parameters of awake event detecting */
#define AWAKE   (1)        /*!< wake up event */
#define NOEVENT (0)        /*!< no event */

#define LEN_STA_QUEUE (20) /*!< length of state queue used to detect awake event */

/*!< parameters of sleep monitoring algorithm */
#define SLEEP   (1)        /*!< sleep state */
#define WAKE    (0)        /*!< wake state */


/*!< variable of detecting awake event */
int  state_aw[LEN_STA_QUEUE]; /*!< state for LEN_STA_QUEUE * 5s */

/*!< variable of sleep monitoring */
int   inten_sl[5];            /*!< motion intensity for 5 * 1min */
float score_sl;               /*!< score of motion */
int   sum_svm_5s;             /*!< motion intensity in 5s */


/** function for processing acceleration data */
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