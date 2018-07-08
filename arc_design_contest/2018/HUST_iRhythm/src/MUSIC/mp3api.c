/**
 * MP3 DECODE TOP API
 * DDK
 * 2018 02 10
 */

#include "embARC.h"

// #define DBG_MORE
#include "embARC_debug.h"

#include "stdlib.h"

#include "mp3dec.h"
#include "mp3common.h"
#include "coder.h"

#include "perform.h"
#include "include.h"


volatile uint8_t flag_sw = 0; 					//Ping-pong Buff switching Flag

// bool flag_songend = false;

int8_t buf_rec1[2304];						//Ping-pong Buff-A for DMA Transfer
int8_t buf_rec2[2304];						//Ping-pong Buff-A for DMA Transfer


/**
 * \brief       Top Lever Function of MP3 Decord and Play
 *
 * \param[in]   filelenth              BUFF length should be Decord
 *
 * \param[in]   loction                If the Data in 15MB File Buff or In 15MB Net Buff
 *
 * \retval      1                      The Function End Dure to Key Break from Gui Task
 *
 * \retval      0                      The Function End with Mission Ending
 *
 */
int32_t play_mp3(int32_t filelenth, uint8_t location)
{
	int32_t offset;					//Pass the Result of FindSyncWord to Set Buff Pointer
	uint8_t *read_ptr;				//Pointer of MP3 File Buff

	int32_t byte_left = filelenth;	//Lenth of Buff has Not been Processed

	uint32_t res_dec;				//Pass Decode State

	MP3DecInfo *mp3_dec;			//Decoder's Main Struct

	EventBits_t uxBits;				//Event to Hang up or Return the Music Task


	uint32_t buff_ofs = 0;
	uint32_t dec_sum = 0;


	/* code */

	if ( location == IN_FILE ) {
		read_ptr = file_buff;
	} else {
		read_ptr = file_buff;
		flag_netend = false;
		dec_sum = 0;
	}



	/* Prepare to transfer by SPI DMA,Set CS ,Set CLK to 12MHz */
	spi->spi_control(SPI_CMD_MST_SEL_DEV, CONV2VOID((uint32_t)EMSK_SPI_LINE_0));
	spi->spi_control(SPI_CMD_MST_SET_FREQ, CONV2VOID(12000000));

	mp3_dec = (MP3DecInfo *)MP3InitDecoder();

	dbg_printf(DBG_LESS_INFO, "Start to Trace\r\n");

	/*
	reset gui fft bar
	 */
	gui_info.fft_show[0] = 0;
	gui_info.fft_show[1] = 0;
	gui_info.fft_show[2] = 0;
	gui_info.fft_show[3] = 0;


	xEventGroupSetBits( evt1_cb, BIT_0 | BIT_1 | BIT_2 );

	/* Start to Decord MP3 */
	perf_start();

	while (1) {
		if ( location == IN_NET ) {
			buff_ofs = uart_obj -> uart_info.rx_buf.ofs;  		//网络下载的缓冲区大小
		}

		if ( buff_ofs  > (int32_t)(dec_sum) + ( 20 * 1024 )
			 || flag_netend == true
			 || location == IN_FILE ) {	//如果接收到的量比解压完的多20KB或者下载完毕

			offset = MP3FindSyncWord(read_ptr, byte_left);		//Find the Location of Start

			if ( offset >= 0 ) {
				read_ptr += offset;         					//Data Start Location
				byte_left -= offset;        					//Un-used Buff Decrease

				dec_sum += offset;
				//gui_info.main_cycle = perf_end();

				perf_start();									//Get Decode Performance

				/* Start to Decode in Ping-pong Mode */
				if ( flag_sw == 0 ) {
					res_dec = MP3Decode(mp3_dec, &read_ptr, (int *)&byte_left, buf_rec1, 0);
				} else {
					res_dec = MP3Decode(mp3_dec, &read_ptr, (int *)&byte_left, buf_rec2, 0);
				}

				gui_info.decord_speed = perf_end();				//Get Decode Performance

				if (res_dec == ERR_MP3_NONE) {					//Decode End Successfully
					// dbg_printf(DBG_MORE_INFO,"MP3Decode Time :%dMs!\n\r",gui_info.decord_speed);
				} else {
					dbg_printf(DBG_LESS_INFO, "MP3Decode error:%d!\n\r", res_dec);
					read_ptr += 2;
					dec_sum += 2;
					byte_left -= 2;
					continue;
				}

				/* Shedule Here */
				xEventGroupWaitBits(
					evt1_cb,
					BIT_0 | BIT_1 | BIT_2 , 						//Regard BIT0 as Dma Transfer Finish,Regard BIT1 as Outside FIFO Full Flag
					pdFALSE, 								//BIT_0 and BIT_1 Should Not be Cleared manually.
					pdTRUE, 								// Wait for both Bits
					portMAX_DELAY );
				xEventGroupClearBits( evt1_cb, BIT_0 );

				/* End this Song,and Play Next Song make sure spi-dma has completed*/
				if ( gui_info.flag_next != 1) {
					;
				} else {
					gui_info.decord_speed = -1;
					gui_info.main_cycle = -1;
					// flag_netend = true;
					return 1;
				}

				if ( iosignal_read(0) ) {								//Outsize FIFO Not Full ?
					uxBits = 0;
				} else {
					// xEventGroupSetBits( GUI_Ev, BIT_0 );				//Reflash Gui Once
					dbg_printf(DBG_MORE_INFO, "GPIO Clear BIT1\r\n");
					uxBits = xEventGroupClearBits( evt1_cb, BIT_1 );	//Clear BIT_1 and Wait Outsize FIFO Empty
				}

				/* Shedule End Here */

				/* Enable DMA to Transfer in Ping-pong Mode */
				if ( flag_sw == 0 ) {
					spi_writeraw((uint8_t *)buf_rec1);
					flag_sw = 1;
				} else {
					spi_writeraw((uint8_t *)buf_rec2);
					flag_sw = 0;
				}
			} else if ( flag_netend == false && location == IN_NET ) {	//offset < 0
				dbg_printf(DBG_MORE_INFO, "offset overflow!!!\n\r" );
				_Rtos_Delay(1000);
			} else {					//offset <0 && download has complete
				/* Scan Whole File Buff,No Start is End **********/
				dbg_printf(DBG_MORE_INFO, "Decorder Complete!\n\r" );

				xEventGroupWaitBits(
					evt1_cb,
					BIT_0 | BIT_1 | BIT_2, 						//Regard BIT0 as Dma Transfer Finish,Regard BIT1 as Outside FIFO Full Flag
					pdFALSE, 								//BIT_0 and BIT_1 Should Not be Cleared manually.
					pdTRUE, 								// Wait for both Bits
					portMAX_DELAY );
				break;
			}

		} else if ( flag_netend == false ) {
			dbg_printf(DBG_MORE_INFO, "No data wait!\n\r" );
			_Rtos_Delay(1000);
		} else {
			/* Scan Whole File Buff,No Start is End **********/
			dbg_printf(DBG_MORE_INFO, "Decorder Complete!\n\r" );

			xEventGroupWaitBits(
				evt1_cb,
				BIT_0 | BIT_1 | BIT_2, 						//Regard BIT0 as Dma Transfer Finish,Regard BIT1 as Outside FIFO Full Flag
				pdFALSE, 								//BIT_0 and BIT_1 Should Not be Cleared manually.
				pdTRUE, 								// Wait for both Bits
				portMAX_DELAY );
			break;
		}
	}

	gui_info.decord_speed = -1;
	gui_info.main_cycle = -1;

	/* Play Song in NET Buff,Should Reset Flag */
	// if ( location == IN_BUFF ) {
	// 	flag_netbuff = BUFF_EMPTY;
	// }

	dbg_printf(DBG_LESS_INFO, "MP3 file: decorder is over!\n\r" );

	return 0;
}



