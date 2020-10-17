#include "delay.h"
#include "ds1302.h"
#include "hsc025a.h"
#include "dht11.h"
#include "IIC.h"
#include "LD3320a.h"
#include "dwin.h"
#include "HC_06.h"

#define CTRL_MODE_GESTURE   0
#define CTRL_MODE_AUDIO     1

bool play_or_pause;		// false = pause; true = play;
uint8_t ico_prev[2] = { 0 };
uint8_t ico_play[2] = { 0 };
uint8_t ico_next[2] = { 0 };

void sys_song_next(void)
{
	song_next();
	play_or_pause = true;
	dwin_disp_btn(DWIN_ICO_PLAY_ADDR, ico_play, DWIN_ICO_PAUSE_NORM);
}

void sys_song_prev(void)
{
	song_prev();
	play_or_pause = true;
	dwin_disp_btn(DWIN_ICO_PLAY_ADDR, ico_play, DWIN_ICO_PAUSE_NORM);
}

int main(void)
{

	uint32_t i;
	uint8_t ctrl_mode;

	/* HSC025A receive buffer */
	uint8_t uart0_rcv_buf[MAX_RESP_LENGTH];
	uint8_t play_mode;
	uint8_t song_name[MAX_RESP_LENGTH];
	uint32_t song_name_len;
	uint32_t UART0_baudrate = UART_BAUDRATE_115200;

	/* DHT11 */
	uint8_t dht11_rcv[5];	// temp/humi data recived from dht11
	uint8_t dht11_hex[4];

	/* DS1302 */
	uint8_t time_rcv[8];	// time data receive from ds1302
	uint8_t time_extr[14];	// extracted time data in bcd format
	uint8_t time_hex[TIME_HEX_LEN];	// extracted time data in hex format
	char *day_string;	// day of week string
	uint8_t sec_buf = 0;
	uint8_t sec_flag = 0;

	/* HC-06 */
	uint32_t UART2_baudrate = UART_BAUDRATE_9600;
	uint8_t ctrl_cmd_ges;
	bool ges_enable;

	/* LD3320A */
	uint8_t ctrl_cmd_audio;

	/* DWIN buttons */
	uint8_t dwin_btn_prev = 0;
	uint8_t dwin_btn_play = 0;
	uint8_t dwin_btn_next = 0;

	/* initialization */
	cpu_lock();
	board_init();		/* board init */
	cpu_unlock();

	ctrl_mode = CTRL_MODE_AUDIO;

	/* HSC025 initial check & setting for debugging */
	hsc025a_init(UART0_baudrate);	// UART0 - HSC025
	play_mode = 0;
	mode_set(HSC025A_MODE_SD);
	mode_check(uart0_rcv_buf);
	volumn_set(0x06);
	play_or_pause = true;

	rtc_init();
	time_rcv[0] = 0x20;
	// rtc_set_time(time_rcv);     // already
	dht11_init();

	UART2_init(UART2_baudrate);	// UART2 - HC-06
	ctrl_cmd_ges = 0;
	ges_enable = false;

	Master1_slvdev_init(I2C_SLAVE_UART_ADDR);	// DWIN I2C to UART init
	ico_prev[1] = DWIN_ICO_PREV_NORM;
	ico_play[1] = DWIN_ICO_PAUSE_NORM;
	ico_next[1] = DWIN_ICO_NEXT_NORM;

	LD3320A_init();
	ctrl_cmd_audio = 0;

	delay_ms(50);

	EMBARC_PRINTF("Initialization Complete!\n\r");
	switch (ctrl_mode) {
	case CTRL_MODE_GESTURE:
		EMBARC_PRINTF("control mode: gesture\n\r");
		break;
	case CTRL_MODE_AUDIO:
		EMBARC_PRINTF("control mode: audio\n\r");
		break;
	default:
		EMBARC_PRINTF("ERROR: Illegal control mode!\n\r");
		break;
	}

	EMBARC_PRINTF("-- enter while ----------------------------\n\r");
	while (1) {
		EMBARC_PRINTF
		    ("\n\r-- while begin ----------------------------\n\r");

		/* DWIN buttons reads and icon changes */
		dwin_btn_prev = dwin_read_btn(DWIN_BTN_PREV_ADDR);
		if (dwin_btn_prev == DWIN_BTN_PREV_VALUE) {
			dwin_disp_btn(DWIN_ICO_PREV_ADDR, ico_prev,
				      DWIN_ICO_PREV_SHADE);
			sys_song_prev();
		} else {
			dwin_disp_btn(DWIN_ICO_PREV_ADDR, ico_prev,
				      DWIN_ICO_PREV_NORM);
		}

		dwin_btn_play = dwin_read_btn(DWIN_BTN_PLAY_ADDR);
		if (dwin_btn_play == DWIN_BTN_PLAY_VALUE) {
			play_or_pause = !play_or_pause;

			if (play_or_pause) {
				dwin_disp_btn(DWIN_ICO_PLAY_ADDR, ico_play,
					      DWIN_ICO_PAUSE_SHADE);
			} else {
				dwin_disp_btn(DWIN_ICO_PLAY_ADDR, ico_play,
					      DWIN_ICO_PLAY_SHADE);
			}

			song_play_or_pause();
		} else {
			if (play_or_pause) {
				// ico_play[1] = DWIN_ICO_PAUSE_NORM;
				dwin_disp_btn(DWIN_ICO_PLAY_ADDR, ico_play,
					      DWIN_ICO_PAUSE_NORM);
			} else {
				// ico_play[1] = DWIN_ICO_PLAY_NORM;
				dwin_disp_btn(DWIN_ICO_PLAY_ADDR, ico_play,
					      DWIN_ICO_PLAY_NORM);
			}
		}

		dwin_btn_next = dwin_read_btn(DWIN_BTN_NEXT_ADDR);
		if (dwin_btn_next == DWIN_BTN_NEXT_VALUE) {
			// ico_next[1] = DWIN_ICO_NEXT_SHADE;
			dwin_disp_btn(DWIN_ICO_NEXT_ADDR, ico_next,
				      DWIN_ICO_NEXT_SHADE);
			sys_song_next();
		} else {
			dwin_disp_btn(DWIN_ICO_NEXT_ADDR, ico_next,
				      DWIN_ICO_NEXT_NORM);
		}

		/* --------------------------
		 * gesture / audio control
		 * -------------------------- */

		switch (ctrl_mode) {
		case CTRL_MODE_GESTURE:
			dwin_disp_ctrl_mode_ges();
			EMBARC_PRINTF("control mode: gesture\n\r");
			break;
		case CTRL_MODE_AUDIO:
			dwin_disp_ctrl_mode_audio();
			EMBARC_PRINTF("control mode: audio\n\r");
			break;
		default:
			dwin_disp_space(DWIN_CTRL_MODE_ADDR, 4);
			EMBARC_PRINTF("ERROR: Illegal control mode!\n\r");
			break;
		}

		switch (ctrl_mode) {
		case CTRL_MODE_GESTURE:
			ctrl_cmd_ges = get_ges_cmd();

			if (ctrl_cmd_ges == GES_BACKWARD) {
				ges_enable = !ges_enable;
			}

			if (ges_enable) {
				// dwin_disp_ges_enable();
				EMBARC_PRINTF("* Gesture control enable\n\r");
			} else {
				dwin_disp_ges_disable();
				EMBARC_PRINTF("* Gesture control disable\n\r");
			}

			if (ges_enable) {
				EMBARC_PRINTF("ctrl_cmd_ges  : %d  \n\r",
					      ctrl_cmd_ges);
				switch (ctrl_cmd_ges) {
				case 0:
					dwin_disp_space(DWIN_CMD_ADDR, 12);
					break;
				case GES_DOWN:
					EMBARC_PRINTF("down              ");
					mode_switch_auto(uart0_rcv_buf);

					play_mode = mode_check(uart0_rcv_buf);
					if (play_mode == HSC025A_MODE_SD) {
						play_or_pause = true;
						dwin_disp_btn
						    (DWIN_ICO_PLAY_ADDR,
						     ico_play,
						     DWIN_ICO_PAUSE_NORM);
					} else if (play_mode == HSC025A_MODE_BT) {
						play_or_pause = false;
						dwin_disp_btn
						    (DWIN_ICO_PLAY_ADDR,
						     ico_play,
						     DWIN_ICO_PAUSE_NORM);
					}

					dwin_disp_sw_play_mode();
					break;
				case GES_UP:
					EMBARC_PRINTF("up            ");
					song_play_or_pause();
					play_or_pause = !play_or_pause;

					if (play_or_pause) {
						// ico_play[1] = DWIN_ICO_PAUSE_NORM;
						dwin_disp_btn
						    (DWIN_ICO_PLAY_ADDR,
						     ico_play,
						     DWIN_ICO_PAUSE_NORM);
						dwin_disp_song_play();
					} else {
						// ico_play[1] = DWIN_ICO_PLAY_NORM;
						dwin_disp_btn
						    (DWIN_ICO_PLAY_ADDR,
						     ico_play,
						     DWIN_ICO_PLAY_NORM);
						dwin_disp_song_pause();
					}

					break;
				case GES_RIGHT:
					EMBARC_PRINTF("right            ");
					sys_song_prev();
					dwin_disp_song_prev();
					break;
				case GES_LEFT:
					EMBARC_PRINTF("left           ");
					sys_song_next();
					dwin_disp_song_next();
					break;
				case GES_CLOCKWISE:
					EMBARC_PRINTF("clockwise       ");
					volumn_up();
					dwin_disp_vol_up();
					break;
				case GES_ANTI_CLOCKWISE:
					EMBARC_PRINTF("anti-clockwise  ");
					volumn_down();
					dwin_disp_vol_down();
					break;
				case GES_FORWARD:
					EMBARC_PRINTF("forward         ");
					ctrl_mode = CTRL_MODE_AUDIO;
					dwin_disp_sw_ctrl_mode();
					ges_enable = false;
					break;
					// case GES_BACKWARD:
					//     EMBARC_PRINTF("backward       ");
					//     // song_pause();
					//     break;
					// case GES_WAVE:
					//     EMBARC_PRINTF("wave           ");
					//     break;
				default:
					break;
				}
			}
			break;
		case CTRL_MODE_AUDIO:
			ges_enable = false;
			ctrl_cmd_audio = get_audio_cmd();
			EMBARC_PRINTF("ctrl_cmd_audio: %d  \n\r",
				      ctrl_cmd_audio);
			switch (ctrl_cmd_audio) {
			case 0:
				dwin_disp_space(DWIN_CMD_ADDR, 12);
				break;
			case AUDIO_SW_CTRL_MODE:
				ctrl_mode = CTRL_MODE_GESTURE;
				ges_enable = false;
				dwin_disp_sw_ctrl_mode();
				break;
			case AUDIO_SW_PLAY_MODE:
				mode_switch_auto(uart0_rcv_buf);
				play_mode = mode_check(uart0_rcv_buf);
				if (play_mode == HSC025A_MODE_SD) {
					play_or_pause = true;
					dwin_disp_btn(DWIN_ICO_PLAY_ADDR,
						      ico_play,
						      DWIN_ICO_PAUSE_NORM);
				} else if (play_mode == HSC025A_MODE_BT) {
					play_or_pause = false;
					dwin_disp_btn(DWIN_ICO_PLAY_ADDR,
						      ico_play,
						      DWIN_ICO_PAUSE_NORM);
				}
				dwin_disp_sw_play_mode();
				break;
			case AUDIO_NEXT_SONG:
				sys_song_next();
				dwin_disp_song_next();
				break;
			case AUDIO_PREV_SONG:
				sys_song_prev();
				dwin_disp_song_prev();
				break;
			case AUDIO_PLAY_SONG:
				// song_play();
				if (play_or_pause == false) {
					song_play_or_pause();
				}
				play_or_pause = true;
				dwin_disp_song_play();
				// ico_play[1] = DWIN_ICO_PAUSE_NORM;
				dwin_disp_btn(DWIN_ICO_PLAY_ADDR, ico_play,
					      DWIN_ICO_PAUSE_NORM);
				break;
			case AUDIO_PAUSE_SONG:
				// song_pause();
				if (play_or_pause == true) {
					song_play_or_pause();
				}
				play_or_pause = false;
				dwin_disp_song_pause();
				// ico_play[1] = DWIN_ICO_PLAY_NORM;
				dwin_disp_btn(DWIN_ICO_PLAY_ADDR, ico_play,
					      DWIN_ICO_PLAY_NORM);
				break;
			case AUDIO_VOL_DOWN:
				volumn_down();
				dwin_disp_vol_down();
				break;
			case AUDIO_VOL_UP:
				volumn_up();
				dwin_disp_vol_up();
				break;
			default:
				break;
			}
			break;
		default:
			EMBARC_PRINTF("Illegal control mode!\n\r");
			break;
		}

		/* --------------------------
		 * playing mode display begin
		 * -------------------------- */
		play_mode = mode_check(uart0_rcv_buf);

		switch (play_mode) {
		case HSC025A_MODE_STANDBY:
		    dwin_disp_mode_standby();
		    break;
		case HSC025A_MODE_USB:
			dwin_disp_mode_usb();
			break;
		case HSC025A_MODE_SD:
			dwin_disp_mode_sd();
			break;
		case HSC025A_MODE_BT:
			dwin_disp_mode_bt();
			break;
		default:
			dwin_disp_space(DWIN_PLAY_MODE_ADDR, 3);
			break;
		}
		/* playing mode display end */

		/* --------------------------
		 * song name display (only available in usb/sd mode) begin
		 * -------------------------- */
		if ((play_mode == HSC025A_MODE_USB)
		    || (play_mode == HSC025A_MODE_SD)) {
			hsc025a_exec_command(0x02, 0x38, 0x0000, uart0_rcv_buf,
					     HSC025A_READ_TMOUT);
			while (uart0_rcv_buf[2] != 0x38) {
				hsc025a_exec_command(0x02, 0x38, 0x0000,
						     uart0_rcv_buf,
						     HSC025A_READ_TMOUT);
			}

			song_name_len =
			    song_name_extr(uart0_rcv_buf, song_name);

			// write twice to avoid cpu crash
			dwin_write_cmd(DWIN_SONG_NAME_ADDR, song_name, SONG_NAME_LENGTH / 2);	// use SONG_NAME_LENGTH/4 addr
			dwin_write_cmd(DWIN_SONG_NAME_ADDR +
				       SONG_NAME_LENGTH / 4,
				       &song_name[SONG_NAME_LENGTH / 2],
				       SONG_NAME_LENGTH / 2);

		} else {
			dwin_disp_space(DWIN_SONG_NAME_ADDR,
					SONG_NAME_LENGTH / 2);
		}

		/* ------------------------------------------
		 * print time, temp and humi on putty for test
		 * ------------------------------------------ */
		// EMBARC_PRINTF("STATUS ---------------------------------------\n\r");
		rtc_read_time(time_rcv);
		rtc_time_extract(time_rcv, time_extr);
		rtc_time_extract_hex(time_rcv, time_hex);

		if (time_extr[13] != sec_buf) {
			sec_flag = 0;
			sec_buf = time_extr[13];

			dwin_write_cmd(DWIN_RTC_ADDR, time_hex, TIME_HEX_LEN);
		}

		dht11_read_data(dht11_rcv, dht11_hex);
		dwin_write_cmd(DWIN_HUMI_ADDR, &dht11_hex[0], 2);
		dwin_write_cmd(DWIN_TEMP_ADDR, &dht11_hex[2], 2);

		/* print test end */

		/* system delay */

		// reset value
		dwin_btn_prev = 0;
		dwin_btn_play = 0;
		dwin_btn_next = 0;
		ctrl_cmd_ges = 0;
		ctrl_cmd_audio = 0;
		play_mode = 0;
		for (i = 0; i < MAX_RESP_LENGTH; i++) {
			uart0_rcv_buf[i] = 0;
		}

	}

	return E_SYS;
}
