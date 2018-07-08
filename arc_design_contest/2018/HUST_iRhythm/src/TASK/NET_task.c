/**
 * NET TASK
 * DDK
 * 2018 03 10
 */

#include "embARC.h"

#define DBG_MORE
#include "embARC_debug.h"

#include "include.h"


/**
 * \brief       Main function of Net Task
 */
void net_task()
{
	EMBARC_PRINTF("NET_TASK START\r\n");

	while (1) {
		/* Check Song ID List is not Empty ?*/
		if ( songid_cnt == 0 ) {
			/* Request Song ID to Request Download Url */
			dbg_printf(DBG_MORE_INFO, "\r\nrequest songid\r\n");

			if ( -1 == socket_request(SONG_ID) ) {
				dbg_printf(DBG_LESS_INFO, "\r\nRequest Songid Fail,Continue!\r\n");
				continue;
			}
		}

		if ( IN_NET == flag_net ) {
			/* Request Song Information Including Download Url */
			dbg_printf(DBG_MORE_INFO, "\r\nRequest Songurl\r\n");

			if ( -1 == socket_request(SONG_INFO)) {
				dbg_printf(DBG_LESS_INFO, "\r\nRequest Songurl Fail,Continue!\r\n");
				continue;
			}

			/* Check Song Download Url It's Unnecessary Now */
			if ( *dllink == '\0'  ) {
				dbg_printf(DBG_LESS_INFO, "\r\nNo Url,Reset\r\n");
				continue;
			}

			/* Download Song */
			dbg_printf(DBG_LESS_INFO, "\r\nDownload Song\r\n");
			download_mp3();
		}
		/* There is a Song in Buff and has not been Played yet */
		else {
			/* Just Release Task */
			_Rtos_Delay(5000);
		}
	}

	return ;
}


