/**
 * Key API CORE CODE
 * DDK
 * 2018 05 10
 */

#include "embARC.h"
#include "embARC_debug.h"

#include "include.h"

/**
 * \brief       IO Interrupt Callback to Realize COMFIRM Key Function
 *
 */
void key1_isr()
{
	//SWITCH 254 SONG Most
	uint8_t i;
	struct filelist *file_pointer = Playlist_HEAD;

	gui_info.delay_cnt = xTaskGetTickCount ();			//Update to Get Another 5 Seconds

	/* In Performance Page,Goto Page 1 */
	if ( gui_info.screen_point == 0 ) {
		gui_info.screen_point ++;
		gui_info.next_song = file_pointer -> data;
	} else {
		/* Not in Performance Page,Restart Playing Decided by pointer */
		gui_info.flag_next = 1;

		/* Delete Playlist List before the Song Need to Play  */
		for ( i = gui_info.screen_point - 1; i > 0; i -- ) {
			/* If it is the last Song in Play List,Play it again and again and Never Delete */
			if ( Playlist_HEAD -> next != NULL ) {
				/* Check if an Online Song in Playlist,then Net Buff should be Reset */
				filelist_delete();				//delete it from Playlist
			} else {
				EMBARC_PRINTF("\r\nNo Song Left!!!\r\n");
			}
		}

		/* Reset Gui Control Infomation */
		gui_info.screen_point = 1;
		gui_info.network_speed = -1;
		gui_info.decord_speed = -1;
		gui_info.main_cycle = -1;
	}

	gui_info.perf_update = 0;
}

/**
 * \brief       IO Interrupt Callback to Realize NEXT Key Function
 *
 */
void key2_isr()
{
	uint8_t i;
	struct filelist *file_pointer = Playlist_HEAD;

	gui_info.delay_cnt = xTaskGetTickCount ();			//Update to Get Another 5 Seconds

	for ( i = gui_info.screen_point ; i > 0 ; i -- ) {
		file_pointer = file_pointer -> next;			//Update Song Pointer

		if ( file_pointer != NULL ) {

		} else {
			return;										//Unnecessary to Reflash in this Case
		}
	}

	gui_info.next_song = file_pointer -> data;			//Update Song Name to Display
	gui_info.screen_point ++;							//Increase Song Pointer
	gui_info.perf_update = 0;
}

/**
 * \brief       IO Interrupt Callback to Realize PAUSE Key Function
 *
 */

void key3_isr()
{
	//EMBARC_PRINTF("key3_isr!\r\n");
	static bool pluse_flag = 0;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if ( pluse_flag == 0 ) {
		xEventGroupClearBitsFromISR( evt1_cb, BIT_2 );
		pluse_flag = 1;

	} else {
		xEventGroupSetBitsFromISR(
			evt1_cb,	// The event group being updated.
			BIT_2,   // The bits being set.
			&xHigherPriorityTaskWoken );
		pluse_flag = 0;
	}

}