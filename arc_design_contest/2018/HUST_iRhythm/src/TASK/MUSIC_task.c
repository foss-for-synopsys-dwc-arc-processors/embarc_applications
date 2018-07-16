/**
 * MUSIC TASK
 * DDK
 * 2018 03 10
 */

#include "embARC.h"
#include "embARC_debug.h"
#include "stdlib.h"

#include "include.h"


/**
 * \brief       Main function of Music Task
 */
void music_task()
{

	EMBARC_PRINTF("MUSIC_TASK START\r\n");

	play_init();

	while (1) {
		Start_playing();
	}
}



