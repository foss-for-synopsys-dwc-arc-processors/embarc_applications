/**
 * GUI API CORE CODE
 * DDK
 * 2018 05 10
 */


#include "embARC.h"
#include "embARC_debug.h"

#include "include.h"

#include "oled.h"

struct _gui_info gui_info;	//GUI Control Infomation

/**
 * \brief       Gui Screen 1 Reflash Function to Show Debug Preformace Message
 *
 */
static void perform_screen()
{

	if ( gui_info.network_speed < 0) {
		OLED_P6x8Str( 70, 0, "Stop!   ");
	} else {
		OLED_PrintInt16( 70, 0, gui_info.network_speed );
		OLED_P6x8Str( 88, 0, "KB/s");
	}
	if ( gui_info.decord_speed < 0) {
		OLED_P6x8Str( 70, 3, "Stop!   ");
	} else {
		OLED_PrintInt16( 70, 3, gui_info.decord_speed );
		OLED_P6x8Str( 88, 3, "ms");
	}

	if ( 0 == gui_info.perf_update ) {
		/* Display Net Speed */
		OLED_CLS();
		OLED_P6x8Str(0, 0, "Net Speed:");

		/* Display Decord time */
		OLED_P6x8Str( 0, 1, "Decord:");

		/* Display Song Name Playing Now */
		OLED_P6x8Str(0, 5, "SONG:");
		OLED_P6x8Str( 40, 5, gui_info.song_name);

		OLED_P6x8Str(0, 6, "Next Song:");
		OLED_P6x8Str( 20, 6, gui_info.next_song);
		gui_info.perf_update = 1;
	}
}

/**
 * \brief       This function used to show song name to select song
 *
 */
static void FFT_screen()
{
	gui_info.perf_update = 0;

	draw_fft();
}


/**
 * \brief       This function control LED fft board to flash 
 * 
 */
uint8_t led_fft_show[8];
static void LED_FFT()
{
	uint8_t i = 0;
	uint8_t real_val;
	uint8_t val;

	for ( i = 0; i < 8; i++ ) {
		real_val = gui_info.fft[i] / 8 ;

		if ( led_fft_show[i] <= real_val ) {
			led_fft_show[i] = real_val;
		} else {
			led_fft_show[i] --;
		}

		if ( led_fft_show[i] > 8 ) {
			led_fft_show[i] = 8;
		}

		/* convent from 0~8 to 0~255 */
		val = (uint8_t)(( 0x01 << led_fft_show[i]  ) - 1);
		led_row_ctl( i + 1, val);
		// EMBARC_PRINTF("led_fft_show[%d]=%x\n\r",i,led_fft_show[i] );
	}
}

/**
 * \brief       Main Loop function of GUI task
 *
 */
void reflash_screen()
{
	/* if No Operation Appears in 5 Seconds,then Return to FFT Page */
	if ( (gui_info.delay_cnt < ( xTaskGetTickCount () - 5000 ))
		 && ( 0 != gui_info.screen_point ) ) {
		gui_info.screen_point = 0;
		gui_info.fft_show[0] = 0;
		gui_info.fft_show[1] = 0;
		gui_info.fft_show[2] = 0;
		gui_info.fft_show[3] = 0;
		OLED_CLS();
	}

	switch (gui_info.screen_point) {
		case (0):
			FFT_screen();			//show fft in common situation
			break;

		default:
			perform_screen();		//show preform data when key click
			break;
	}

	LED_FFT();						//update LED fft
}


