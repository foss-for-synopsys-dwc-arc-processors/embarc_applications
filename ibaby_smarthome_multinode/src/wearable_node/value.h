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
 * \brief   Header File of Settings and Reported Data
 */

/**
 * \addtogroup    EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * @{
 */
#ifndef VALUE_H
#define VALUE_H

/**
 * \name    macros about common setting
 * @{
 */
#define LWM2M_CLIENT      (1)/*!< set 1 to be lwm2m client */

#define PRINT_DEBUG_FUNC  (1)/*!< set 1 to print message for debug major function */
#define PRINT_DEBUG_AWAKE (1)/*!< set 1 to print message for debug awake event detecting function */
#define PRINT_DEBUG_SLEEP (1)/*!< set 1 to print message for debug sleep-wake state monitoring function */

/*
**************************************************************
*  This part will be deleted in release version
*/
#define USED_TIMER1         (0)/*!< set 1 to use timer1 for task timing */
#define SEND_DEBUG_SVM1_5S  (0)/*!< set 1 to send svm before svm filter to matlab by serial port every 5s */
#define SEND_DEBUG_SVM2_5S  (0)/*!< set 1 to send svm after svm filter to matlab by serial port every 5s */
#define SEND_DEBUG_INTEN_1M (0)/*!< set 1 to send motion intensity to matlab by serial port every 1min */
/*
*  end of this part
**************************************************************
*/

/** @} end of name */

/**
 * \name    macros about iBaby wearable node
 * @{
 */ 
/* lwm2mServer's port and IP */
static char *p_port   = (char *)"5683";
static char *p_server = (char *)"10.120.158.127";
/* name of lwm2m client node */
static char *p_client_name = (char *)"wn";
/* counter for number of files transported in OTA by LwM2M */
static int count_file;


/*!< struct of iBaby wearable node data reporting to iBaby Gateway */
typedef struct{
	uint32_t btemp;       /*!< body temperature */
    uint32_t hrate;       /*!< heartrate */

	int  motion_intensity;/*!< motion intensity in 1min */
	uint state;           /*!< Sleep-Wake state : SLEEP or WAKE */
	uint event_awake;     /*!< flag of Awake Event : NOEVENT or AWAKE */

    bool warn_hrate;      /*!< warning flag of heartrate abnormal */
    bool warn_btemp;      /*!< warning flag of body temperature abnormal */
    bool warn_downward;   /*!< warning flag of sleep downward */
}data_wearable_node;

/* current value of reporting data */
data_wearable_node data_report_wn;
/* the last value of reporting data */
data_wearable_node data_report_wn_old;
/** @} end of name */


#endif/* _EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE */