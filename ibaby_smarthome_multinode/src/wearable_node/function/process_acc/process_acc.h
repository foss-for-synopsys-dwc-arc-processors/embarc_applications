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
--------------------------------------------- */
/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE
 * \brief	Header File of acceleration data processing function
 *              awake event detecting, sleep downward detecting, sleep monitoring
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE
 * @{
 */
#ifndef _PROCESS_ACC_H_
#define _PROCESS_ACC_H_


#include "acceleration.h"


/*!< parameters of awake event detecting */
#define AWAKE   (1)        /*!< wake up event */
#define NOEVENT (0)        /*!< no event */

#define LEN_STA_QUEUE (20) /*!< length of state queue used to detect awake event */

/*!< parameters of sleep monitoring algorithm */
#define SLEEP   (1)        /*!< sleep state */
#define WAKE    (0)        /*!< wake state */


/*!< variable of detecting awake event */
uint8_t state_aw[LEN_STA_QUEUE]; /*!< state for LEN_STA_QUEUE * 5s */

/*!< variable of sleep monitoring */
uint32_t inten_sl[5]; /*!< motion intensity for 5 * 1min */
uint32_t sum_svm_5s;  /*!< motion intensity in 5s */
float    score_sl;    /*!< score of motion */


/** function for starting sleep monitor and body temperature detecting */
extern void sleep_monitor_start(void);
/** @} end of name */


#endif/* _PROCESS_ACC_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IOT_IBABY_SMARTHOME_MULTINODE_WEARABLE_NODE */