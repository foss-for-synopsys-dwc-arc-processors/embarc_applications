#include "embARC.h"
#include "board.h"
#include "dev_uart.h"
#include "dev_pinmux.h"

#include "vlsi_esp8266.h"
#include "vlsi_ftdi.h"
#include "vlsi_svm.h"

#include <stdio.h>
#include <stdlib.h>
#include "embARC_debug.h"

struct vlsi_ftdi *vlsi_ftdi_obj;
struct vlsi_esp *vlsi_esp_obj;

static volatile unsigned int start;
static unsigned int perf_id = 0xFF;

static void perf_init(unsigned int id)
{
	if (timer_start(id, TIMER_CTRL_NH, 0xFFFFFFFF) < 0)
		EMBARC_PRINTF("perf timer init failed\r\n");
	perf_id = id;
}

/** Performance timer start */
static void perf_start(void)
{
	if (timer_current(perf_id, (void *)(&start)) < 0)
		start = 0;
}

/** Performance timer end, and return the time passed */
static unsigned int perf_end(void)
{
	unsigned int end = 0;

	if (timer_current(perf_id, (void *)(&end)) < 0)
		return 0;
	if (start < end)
		end = end - start;
	else
		end = 0xFFFFFFFF - start + end;
	return end;
}

int main(void)
{
	unsigned int exe_time;

	float predict_data[80];

	int real_data[84];

	int r_hand_y = 0;
	int l_hand_y = 0;
	int hand_top_point[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	int final_anw = 0;
	int anw_fifo_0 = 0;
	int anw_fifo_1 = 0;
	int anw_fifo_2 = 0;
	int info_index;
	unsigned int voice_info;

	start = 0;

    /** PMOD_C = UART1    mux.c   line 281 */
	io_pmod_config(PMOD_C, PMOD_UART, IO_PINMUX_ENABLE);

    /** ARDUINO = UART2   mux.c   line 255 */
	io_arduino_config_uart(IO_PINMUX_ENABLE);

	vlsi_ftdi_obj = (VLSI_FTDI *) malloc(sizeof(VLSI_FTDI));
	vlsi_esp_obj = (VLSI_ESP *) malloc(sizeof(VLSI_ESP));

	vlsi_ftdi_uart_init(vlsi_ftdi_obj, 115200);
	vlsi_esp_uart_init(vlsi_esp_obj, 115200);

	perf_init(TIMER_1);

	int f = 0;

	EMBARC_PRINTF("ESP8266 TEST\r\n");
	f = vlsi_esp_test(vlsi_esp_obj);
	EMBARC_PRINTF("test %d \r\n", f);
	board_delay_ms(1000, 1);

	EMBARC_PRINTF("ESP8266 MODE SET\r\n");
	f = vlsi_esp_mode_set(vlsi_esp_obj);
	EMBARC_PRINTF("test %d \r\n", f);
	board_delay_ms(1000, 1);

	EMBARC_PRINTF("ESP8266 CONNECT\r\n");
	f = vlsi_esp_connect(vlsi_esp_obj);
	EMBARC_PRINTF("test %d \r\n", f);
	if (f == 0)
		EMBARC_PRINTF("Connect WIFI OK!\r\n");
	else
		EMBARC_PRINTF("Connect WIFI ERROR!\r\n");
	board_delay_ms(1000, 1);

	EMBARC_PRINTF("ESP8266 MULTI\r\n");
	f = vlsi_esp8266_multi(vlsi_esp_obj);
	EMBARC_PRINTF("test %d \r\n", f);
	board_delay_ms(1000, 1);

	EMBARC_PRINTF("ESP8266 SERVER\r\n");
	f = vlsi_esp8266_server(vlsi_esp_obj);
	EMBARC_PRINTF("test %d \r\n", f);
	board_delay_ms(1000, 1);

	EMBARC_PRINTF("ESP8266 GET IP\r\n");
	f = vlsi_esp8266_get_ip(vlsi_esp_obj);
	EMBARC_PRINTF("test %d \r\n", f);
	board_delay_ms(1000, 1);

	EMBARC_PRINTF("ESP8266 LINK\r\n");
	f = vlsi_esp8266_link(vlsi_esp_obj);
	EMBARC_PRINTF("test %d \r\n", f);
	board_delay_ms(1000, 1);

	EMBARC_PRINTF("FTDI LINK\r\n");
	f = vlsi_ftdi_test(vlsi_ftdi_obj);
	EMBARC_PRINTF("test %d \r\n", f);
	board_delay_ms(10, 1);

	perf_start();		/** point 1 */

	while (1) {
		/** Receive data from OpenPose */
		vlsi_ftdi_receive(vlsi_ftdi_obj, real_data);

		exe_time = perf_end();
		EMBARC_PRINTF("OpenPose Time : %d cycles\r\n", exe_time);
		perf_start();	/** point 2 */

		/** Pre-Process */
		{
			/** right hand */
			for (int i = 0; i < 20; i++) {	/** 0~39 */
				if ((i == 4) || (i == 8) || (i == 12)
				    || (i == 16)) {
					predict_data[2 * i] =
					    ((float)real_data[2 * i + 2] -
					     (float)real_data[0]) / 10;
					predict_data[2 * i + 1] =
					    ((float)real_data[2 * i + 3] -
					     (float)real_data[1]) / 10;
				} else {
					predict_data[2 * i] =
					    ((float)real_data[2 * i + 2] -
					     (float)real_data[2 * i]) / 10;
					predict_data[2 * i + 1] =
					    ((float)real_data[2 * i + 3] -
					     (float)real_data[2 * i + 1]) / 10;
				}
			}
			/** left hand */
			for (int i = 21; i < 41; i++) {	/** 40~79 */
				if ((i == 25) || (i == 29) || (i == 33)
				    || (i == 37)) {
					predict_data[2 * i - 2] =
					    ((float)real_data[2 * i + 2] -
					     (float)real_data[42]) / 10;
					predict_data[2 * i - 1] =
					    ((float)real_data[2 * i + 3] -
					     (float)real_data[43]) / 10;
				} else {
					predict_data[2 * i - 2] =
					    ((float)real_data[2 * i + 2] -
					     (float)real_data[2 * i]) / 10;
					predict_data[2 * i - 1] =
					    ((float)real_data[2 * i + 3] -
					     (float)real_data[2 * i + 1]) / 10;
				}
			}
		}

		exe_time = perf_end();
		EMBARC_PRINTF("Pre-Process Time : %d cycles\r\n", exe_time);
		perf_start();	/** point 3 */

		/** SVM Predict */
		anw_fifo_2 = anw_fifo_1;
		anw_fifo_1 = anw_fifo_0;
		anw_fifo_0 = vlsi_svm_predict(predict_data);

		if ((anw_fifo_2 == anw_fifo_1) && (anw_fifo_2 == anw_fifo_0))
			final_anw = anw_fifo_2;

		exe_time = perf_end();
		EMBARC_PRINTF("SVM Time : %d cycles\r\n", exe_time);
		perf_start();	/** point 4 */

		/** Voice and Information */
		{
			voice_info = 0;
			/** voice_info 31,30
			*   2'b00 = none / 2'b01 = piano / 2'b10 = drum / 2'b11 = guitar
			*   drum,guitar
			*   29,28 : voice
			*   27~21 : left hand x_index
			*   20~14 : left hand y_index
			*   13~ 7 : right hand x_index
			*    6~ 0 : right hand x_index
			*   piano
			*   29,28 : don't care
			*   27~ 0 : voice per key
			*   Piano
			*/
			if (final_anw == 0) {
				voice_info |= 0x1 << 30;
				for (int i = 0; i < 10; i++) {
					int j = 0;
					if (i >= 5)
						j = 2;
					if (((real_data[8 * i + 9 + j] -
					      hand_top_point[i]) > 8)
					    &&
					    ((real_data[8 * i + 9 + j] -
					      hand_top_point[i]) < 50)) {
						/** bound */
						if (real_data[8 * i + 8 + j] <
						    54)
							info_index = 0;
						else if (real_data
							 [8 * i + 8 + j] > 567)
							info_index = 27;
						else
							info_index =
							    (real_data
							     [8 * i + 8 + j] -
							     54) / 19;
						voice_info |= 0x1 << info_index;
					}
				}
			} else {
				/** Drum */
				if (final_anw == 1) {
					voice_info |= 0x1 << 31;
					if ((real_data[9] - r_hand_y) > 50)
						voice_info |= 0x1 << 28;
					if ((real_data[51] - l_hand_y) > 50)
						voice_info |= 0x1 << 29;
				}
				/** Guitar */
				else {
					voice_info |= 0x1 << 31;
					voice_info |= 0x1 << 30;
					if ((real_data[9] - r_hand_y) > 50)
						voice_info |= 0x1 << 28;
				}
				/** hand information */
				{
					/** right hand x_index */
					if (real_data[0] > 570)
						info_index = 100;
					else if (real_data[0] < 70)
						info_index = 0;
					else {
						info_index =
						    (real_data[0] - 70) / 5;
					}
					voice_info += info_index << 7;
					/** right hand y_index */
					if (real_data[1] > 490)
						info_index = 100;
					else {
						info_index =
						    (real_data[1] + 10) / 5;
					}
					voice_info += info_index;
					/** left hand x_index */
					if (real_data[42] > 570)
						info_index = 100;
					else if (real_data[42] < 70)
						info_index = 0;
					else {
						info_index =
						    (real_data[42] - 70) / 5;
					}
					voice_info += info_index << 21;
					/** left hand y_index */
					if (real_data[43] > 490)
						info_index = 100;
					else {
						info_index =
						    (real_data[43] + 10) / 5;
					}
					voice_info += info_index << 14;
				}
			}
		}

		exe_time = perf_end();
		EMBARC_PRINTF("Voice time : %d cycles\r\n", exe_time);
		perf_start();	/** point 5 */

		vlsi_ftdi_send_uint(vlsi_ftdi_obj, voice_info);
		vlsi_esp_send_uint(vlsi_esp_obj, voice_info);

		exe_time = perf_end();
		EMBARC_PRINTF("Transfer time : %d cycles\r\n", exe_time);
		perf_start();	/** point 6 and 1 */

		EMBARC_PRINTF("voice information : %d %d\r\n", final_anw,
			      voice_info);

		/** last data update */
		r_hand_y = real_data[9];
		l_hand_y = real_data[51];
		hand_top_point[0] = real_data[9];
		hand_top_point[1] = real_data[17];
		hand_top_point[2] = real_data[25];
		hand_top_point[3] = real_data[33];
		hand_top_point[4] = real_data[41];
		hand_top_point[5] = real_data[51];
		hand_top_point[6] = real_data[59];
		hand_top_point[7] = real_data[67];
		hand_top_point[8] = real_data[75];
		hand_top_point[9] = real_data[83];
	}

	return E_OK;
}
