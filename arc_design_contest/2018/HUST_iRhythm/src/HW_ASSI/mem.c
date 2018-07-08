/**
 * API CODE FOR FILE READ
 * DDK
 * 2018 05 10
 */

#include "embARC.h"

#define DBG_MORE
#include "embARC_debug.h"
#include "stdlib.h"

#include "include.h"

#define NUM_BYTE_READ 4096


FIL fp;

/**
 * \brief       Read out File Form SD Card to DDR2 15MB buff
 *
 * \param[in]   music_name             Pointer of File Name in SD Card Need to Read Out
 *
 */
void readout_file(char *music_name)
{
	char filename[50] = "0:/music/";			//the Mp3 File Should in "root"/music/
	uint32_t num_read;
	uint8_t *fbuff_p = file_buff;
	uint32_t read_sum = 0;

	strcat(filename, music_name);
	error_num = f_open(&fp, filename, FA_READ);

	/* Sometimes May Open File Fail,Stop Program */
	if ( error_num != FR_OK) {
		dbg_printf(DBG_LESS_INFO, "File %s open failed!\r\nContinue!\r\n", filename);

		while (1);
	}

	dbg_printf(DBG_LESS_INFO, "Start To Read file %s !!!\r\n", filename);
	memset( file_buff, 0, sizeof(int8_t) * BUFF_SPACE );
	num_read = 1;

	while ( num_read != 0 ) {
		cpu_lock();								//No Interruption Should Happen Here
		error_num = f_read(&fp, fbuff_p, NUM_BYTE_READ, &num_read);
		cpu_unlock();

		fbuff_p += num_read;
		read_sum += num_read;


		if ( gui_info.flag_next != 1) {
			;
		} else {
			break;								//Gui Ask for Next Song,Break Out
		}
	}

	dbg_printf(DBG_LESS_INFO, "readout %d!!!\r\n", read_sum);
	dbg_printf(DBG_LESS_INFO, "File %s Read End\r\n", filename);
	f_close(&fp);
}


