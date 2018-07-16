#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include "embARC.h"

#include "inc_task.h"

#include "esp8266.h"
#include "fft.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/**
 * Global Definition
 *
 */
/* Define Net Buff and File Buff Size to 15MB */
#define BUFF_SPACE 15728640

/* Package Option */
#define NET_LIST 0
#define FILE_LIST 1

/* Package Option */
#define IN_FILE 0
#define IN_NET 1

/* Package Option */
#define SONG_ID 0
#define SONG_INFO 1

/* Package Events Bit */
#define BIT_0	( 1 << 0 )
#define BIT_1	( 1 << 1 )
#define BIT_2	( 1 << 2 )
#define BIT_3	( 1 << 3 )

/**
 * Define in filelist.c
 *
 */
struct filelist {
	char data[50];				//Save Song ID or Song Name in different Function
	int32_t lenth;				//Save length of Song File Buff For Decoder
	uint8_t location;			//Mark Where Song Buff is,in SD Card or in Net Buff
	struct filelist *next;		//Pointer of Linked List to Find Next Node
};

// extern struct filelist *Songid_HEAD ;
// extern struct filelist *Songid_END ;

extern struct filelist *Playlist_HEAD;
extern struct filelist *Playlist_END;

// extern void filelist_init();
extern void filelist_add(char *id_data, int32_t lenth, uint8_t location);
extern void filelist_delete();

/**
 * Define in main.c
 *
 */
extern int32_t error_num ;		//Used to Get Error Num

/**
 * Define in mem.c
 *
 */
extern void readout_file(char *music_name);

/**
 * Define in music.c
 *
 */
extern DEV_SPI_PTR spi;
extern int8_t file_buff[ BUFF_SPACE ];
extern void play_init();

/**
 * Define in dma2spi.c
 * 
 */
extern void spi_dma_prepare(void);
extern int32_t spi_writeraw(const void *data);
extern int32_t Start_playing();

/**
 * Define in mp3api.c
 *
 */
// extern bool flag_songend;
extern int32_t play_mp3(int32_t filelenth, uint8_t location);

/**
 * Define in iosignal.c
 *
 */
extern void iosignal_init();
extern void iosignal_ctrl(uint8_t val, uint8_t num);
extern uint8_t iosignal_read(uint8_t num);
extern void net_rst();
extern void led_row_ctl(uint8_t rows, uint8_t data);

/**
 * Define in net.c
 * 
 */
extern bool flag_net;
extern char dllink[500];
extern char songpoint[50];
// extern int8_t net_buff[ BUFF_SPACE ];
extern ESP8266_DEF_PTR ESP8266_A;
extern int net_init();
extern int socket_request(uint8_t option);
extern void download_mp3();

extern bool flag_netend;
extern uint8_t songid_cnt;
extern DEV_UART_PTR uart_obj;					//Pointer to Get UART State in Task



/**
 * Define in GUI.c
 * 
 */
struct _gui_info {
	uint8_t screen_point;				//Mark Which Page or Which Song is Pointed
	int16_t network_speed;				//Pass Net Work Speed From Net Task to Gui Task
	int16_t decord_speed;				//Pass Decoder Time From Music Task to Gui Task
	int16_t main_cycle;

	bool perf_update;					//Comfirm to update a part of screen

	char *song_name;					//Pass Song Name From Music Task to Gui Task
	char *next_song;					//Pass Next Song Name From Music Task to Gui Task

	// uint8_t KEEP_ID3V2;
	uint8_t flag_next;					//Ask Music Task to Play Next Song

	uint8_t *fft;
	uint8_t fft_show[7];
	uint32_t delay_cnt;					//Count Delay Tick to Reset Screen Pointer

};

extern struct _gui_info gui_info;
extern void gui_init();
extern void reflash_screen();

/**
 * Define in key.c
 *
 */

extern void key1_isr();
extern void key2_isr();
extern void key3_isr();


/* fft data input */
extern uint8_t fft_in[fft_N];
/* fft data output */
extern uint8_t fft_out[fft_N / 2];

#endif










