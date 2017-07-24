/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "common.h"
#include "process_acc.h"
#include "print_msg.h"


#if PRINT_DEBUG_FUNC
/** print message for primary function */
extern void print_msg_func(void)
{
	char str[150];

	EMBARC_PRINTF("\r\n************ Primary function ************\r\n");
	sprintf(str,
                "* Body heartrate   : %dbpm\r\n* Body temperature : %d.%d'C\r\n* Motion intensity : %d\r\n", 
                data_report_wn.hrate,
                data_report_wn.btemp/10, data_report_wn.btemp%10,
                data_report_wn.motion_intensity);
	EMBARC_PRINTF(str);
	
	if (data_report_wn.state == SLEEP) {
		EMBARC_PRINTF("* State : sleep\r\n");
	} else {
		EMBARC_PRINTF("* State : wake\r\n");
	}

	if (data_report_wn.warn_btemp) {
		EMBARC_PRINTF("* Warn of abnormal body temperature\r\n");
	}

	if (data_report_wn.warn_hrate) {
		EMBARC_PRINTF("* Warn of abnormal body heartrate\r\n");
	}

	if (data_report_wn.warn_downward) {
		EMBARC_PRINTF("* Warn of sleep on his stomach\r\n");
	}

	if (data_report_wn.event_awake == AWAKE) {
		EMBARC_PRINTF("* Baby awake!\r\n");
	}

	EMBARC_PRINTF("\r\n");
}
#endif /* PRINT_DEBUG_FUNC */

#if PRINT_DEBUG_AWAKE
/** print message for debug awake event detecting function */
extern void print_msg_awake(void)
{
	char str[50];

	EMBARC_PRINTF("\r\n************ Awake detecting ************\r\n");
	sprintf(str, "* Motion intensity in 5s : %d\r\n", sum_svm_5s);
	EMBARC_PRINTF(str);

	for (uint i = 0; i < LEN_STA_QUEUE; ++i) {
		if (i != 0 && !(i%3)) {
			EMBARC_PRINTF("\r\n");
		}
		
		if (state_aw[i]) {
			sprintf(str, "* state %d : sleep\t", i);
			EMBARC_PRINTF(str);
		} else {
			sprintf(str, "* state %d : wake\t", i);
			EMBARC_PRINTF(str);
		}
	}
	EMBARC_PRINTF("\r\n");

	if (data_report_wn.event_awake == AWAKE) {
		EMBARC_PRINTF("* Baby awake!\r\n\r\n");
	}
}
#endif /* PRINT_DEBUG_AWAKE */

#if PRINT_DEBUG_SLEEP
/** print message for debug Sleep-Wake state monitoring function */
extern void print_msg_sleep(uint state)
{
	char str[50];

	EMBARC_PRINTF("\r\n************ Sleep monitoring ************\r\n");
	for (uint i = 0; i < 5; ++i) {
		if (i == 2 || i == 3) {
			EMBARC_PRINTF("\r\n");
		}
		sprintf(str, "* Intensity %d : %d\t\t", i, inten_sl[i]/100);
		EMBARC_PRINTF(str);
	}

	sprintf(str, "\r\n* Intensity score in 5min : %f\r\n", score_sl);
	EMBARC_PRINTF(str);

	if (!state) {
		EMBARC_PRINTF("* State of 2min ago : wake\r\n\r\n");
	} else {
		EMBARC_PRINTF("* State of 2min ago : sleep\r\n\r\n");
	}
}
#endif /* PRINT_DEBUG_SLEEP */