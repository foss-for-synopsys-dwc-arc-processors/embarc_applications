/**
 * TOP LEVER OF MUSIC TASK
 * DDK
 * 2018 03 10
 */

#include "embARC.h"

#define DBG_MORE
#include "embARC_debug.h"

#include "include.h"


DEV_SPI_PTR spi;						//Pointer to Configure SPI

FATFS fs_p;

int8_t file_buff[ BUFF_SPACE ];			//15MB File Buff to Read out from SD Card


/**
 * \brief       Read out the Information of File in SD Card and Store
 *              their infomation into Playlist List
 *
 */
static void playlist_init()
{
	DIR dir;
	FILINFO fileinfo;

	dbg_printf(DBG_LESS_INFO, "\r\nCreate Play List\r\n");

	struct filelist *lists = NULL;
	lists = (struct filelist *)malloc(sizeof(struct filelist));

	if ( NULL == lists ) {
		dbg_printf(DBG_LESS_INFO, "\r\nPlay List Init Error!\r\n");
	}

	/* Open and Checkout the Directory */
	error_num = f_opendir (&dir, "0:/music");

	if ( error_num != FR_OK ) {
		;
	}

	do {
		error_num = f_readdir (&dir, &fileinfo);

		if ( dir.sect == 0 ) { 				//End of Directory
			break;
		}

		if ( fileinfo.fattrib == 32 ) {		//Check File Style
			filelist_add(&(fileinfo.fname[0]), fileinfo.fsize, IN_FILE);
			dbg_printf(DBG_LESS_INFO, "File name: %s  File size:%d   \r\n", fileinfo.fname, fileinfo.fsize);
		}
	} while ( 1 );

	dbg_printf(DBG_LESS_INFO, "\r\nCloseing root directory. \r\n");
	f_closedir(&dir);
}


/**
 * \brief       Initialize and Perpar all Things before Music Decode Start
 *              including Read out Information of File from SD Card and Create playlist
 */
void play_init()
{

	spi = spi_get_dev(DW_SPI_0_ID);		//Initizlize SPI Pointer for later Configuration

	/* mount Fatfs */
	while (1) {
		error_num = f_mount(&fs_p, "0:/", 1);

		if ( error_num == FR_OK) {
			break;
		} else {
			dbg_printf(DBG_LESS_INFO, "File f_mount failed!\r\nstop!\r\n");
		}
	}

	/* checkout directory and init song list to play */
	playlist_init();
}


/**
 * \brief       Read Out One Song Information from Play List
 *              if Song is in SD Card ,Read Out File Play Song
 *
 * \retval      1                      The Function End Dure to Key Break
 *
 * \retval      0                      The Function End with Misiion End
 *
 */
int32_t Start_playing()
{
	char music_filename[50] = {0};
	int file_lenth;
	uint8_t file_location;

	file_lenth = Playlist_HEAD -> lenth;
	file_location = Playlist_HEAD -> location;
	memset( music_filename, 0, sizeof(char) * 50 );
	strcat( music_filename, Playlist_HEAD -> data );

	gui_info.song_name = music_filename;
	gui_info.perf_update = 0;
	gui_info.screen_point = 1;
	gui_info.delay_cnt = xTaskGetTickCount ();			//Update to Get Another 5 Seconds
	_Rtos_Delay(100);

	// xEventGroupSetBits( GUI_Ev, BIT_0 );		//Reflash Gui to Display Song Name
	dbg_printf(DBG_LESS_INFO, "\r\nplay %s\r\n", music_filename);


	/* If the Song File is Bigger than 15MB Buff,Play Next one */
	if ( file_lenth > BUFF_SPACE ) {
		dbg_printf(DBG_LESS_INFO, "\r\nfile too big,play fail!\r\n");
		return -1;
	}

	dbg_printf(DBG_LESS_INFO, "\r\nfile lenth = %d \r\n", file_lenth);


	xEventGroupClearBits( GUI_Ev, BIT_1 );
	_Rtos_Delay(2000);

	/* Read out File to DDR2 from SD Card,if Net Buff is EMPTY */
	if ( file_location == IN_FILE ) {
		/* Slow CLK of SPI to Read SD Card */
		//spi->spi_control(SPI_CMD_MST_SEL_DEV, CONV2VOID((uint32_t)EMSK_SPI_LINE_SDCARD));
		cpu_lock();
		spi->spi_control(SPI_CMD_MST_SET_FREQ, CONV2VOID(2000000));
		cpu_unlock();
		_Rtos_Delay(1000);
		flag_net = IN_FILE;
		readout_file(music_filename);		//Read out File in SD Card

	} else {
		flag_net = IN_NET;
	}

	xEventGroupSetBits( GUI_Ev, BIT_1 );

	if ( gui_info.flag_next != 1 && 0 == play_mp3(file_lenth, file_location)) {
		dbg_printf(DBG_LESS_INFO, "\r\nplay complete!!!\r\n");;
	} else { 									//Play Next Song?
		gui_info.flag_next = 0;
		return 1;
	}

	/* If it is the last Song in Play List,Play it again and again and Never Delete */
	cpu_lock();								//Gui Interruption May Happen Here

	if ( Playlist_HEAD -> next != NULL ) {
		filelist_delete();				//Once Play a Song, delete it from Playlist
	} else {
		dbg_printf(DBG_LESS_INFO, "\r\nNo Song Left!!!\r\n");
	}

	cpu_unlock();							//Gui Interruption May Cause Error
	return 0;
}