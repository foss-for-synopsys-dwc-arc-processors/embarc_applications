/**
 * GUI TASK HERE
 * DDK
 * 2018 05 10
 */
#include "embARC.h"
#include "embARC_debug.h"


#include "include.h"


/**
 * \brief       Main function of Gui Task
 */
void gui_task()
{

	while (1) {
		reflash_screen();

		xEventGroupWaitBits(
			GUI_Ev,
			BIT_1 , 		//Wait BIT0 to reflash
			pdFALSE, 		//BIT_0 Should not be cleared after Returning.
			pdFALSE,
			portMAX_DELAY );
		_Rtos_Delay(100);
	}

	return ;
}
