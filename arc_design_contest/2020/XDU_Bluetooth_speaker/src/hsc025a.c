#include "hsc025a.h"

DEV_UART * dev_uart0 = NULL;

int hsc025a_init(uint32_t baudrate)
{
	/**
	 * initialize the uart0 which is on PMOD1, pin config:
	 * J1.3 TXD
	 * J1.4 RXD
	 * J1.5 GND
	 * J1.6 3V3
	 */
	dev_uart0 = uart_get_dev(HSC025A_UART_ID);

	if (dev_uart0 == NULL)
		return -1;

	if (dev_uart0->uart_open(baudrate) == E_OPNED) {
		dev_uart0->uart_control(UART_CMD_SET_BAUD, (void *)(baudrate));
	}

	return 0;
}

/* send command without return */
uint32_t hsc025a_write(uint8_t len, uint8_t cmd, uint16_t param)
{
	uint8_t send_dat[6] = {0};

	send_dat[0] = HSC025A_CMD_PREFIX;
	send_dat[1] = len;
	send_dat[2] = cmd;

	switch (len) {
	case 2:
		send_dat[3] = HSC025A_CMD_SUFFIX;
		break;
	case 3:
		send_dat[3] = (uint8_t)param;           // param1: lower 8-bit
		send_dat[4] = HSC025A_CMD_SUFFIX;
		break;
	case 4:
		send_dat[3] = (uint8_t)(param >> 8);    // param1: higher 8-bit
		send_dat[4] = (uint8_t)param;           // param2: lower 8-bit
		send_dat[5] = HSC025A_CMD_SUFFIX;
		break;
	}

	if (dev_uart0 == NULL)
		return 0;

	return dev_uart0->uart_write(send_dat, len+2);
}

/* receive data from hsc025a */
uint32_t hsc025a_read(uint8_t *buf, uint32_t cnt)
{
	if (dev_uart0 == NULL)
		return 0;

	uint32_t rd_avail;

	dev_uart0->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
	cnt = (cnt > rd_avail) ? rd_avail : cnt;
	if (cnt > 0) {
		return dev_uart0->uart_read(buf, cnt);
	}
	return cnt;
}

/*
 * send command to hsc025a with return, keep receiving data when time < wait_ms
 * all the returing data follows command's format: {prefix, len, cmd, char1, char2, ..., suffix}
 * if len = 0x02 && cmd = 0xee, means that received data is not correct
 *   in this case, send the previous cmd again by excute this function recursivly
 */
int hsc025a_exec_command(uint8_t len, uint8_t cmd, uint16_t param, uint8_t *resp, uint32_t wait_ms)
{
	uint32_t cur_ofs = 0;
	uint32_t read_cnt;
	uint32_t cur_time;

	if (((len < 2) || (len > 4)) || (resp == NULL))
		return -1;

	if (resp == NULL)
		return -1;

	if (hsc025a_write(len, cmd, param) > 0) {
		cur_time = HSC025A_TIMENOW();
		do {
			read_cnt = hsc025a_read((uint8_t *)(&resp[cur_ofs]), 2);
			cur_ofs += read_cnt;
		} while ((HSC025A_TIMENOW()-cur_time) < wait_ms);

		resp[cur_ofs] = '\0';

		return cur_ofs;
	}


	return -1;
}


uint32_t rcv_dat_extract(uint8_t *rcv_buf, uint8_t *ext_dat)
{
	uint32_t i = 0;
	uint32_t ext_dat_len = 0;

	for (i = 0; i < MAX_RESP_LENGTH; i++) {
		if (i == 0) {
			if (rcv_buf[i] != HSC025A_CMD_PREFIX) {
				ext_dat_len = 0;
				break;
			} else {
				ext_dat_len++;
			}
		} else if (rcv_buf[i] != HSC025A_CMD_SUFFIX) {
			ext_dat_len++;

			if (i > 2) {
				ext_dat[i-3] = rcv_buf[i];
			}
		} else {
			ext_dat_len = ext_dat_len - 3;
			break;
		}
	}

	return ext_dat_len;
}



/* ------------------------------
 *  HSC025A command
 * ------------------------------ */

/* check current hsc025a software version */
int version_check(uint8_t *rcv_buf)
{
	hsc025a_exec_command(0x02, 0x10, 0x0000, rcv_buf, HSC025A_READ_TMOUT);
	while (rcv_buf[2] != 0x10) {
		hsc025a_exec_command(0x02, 0x10, 0x0000, rcv_buf, HSC025A_READ_TMOUT);
	}

	if (rcv_buf[2] == 0x10) {
		return rcv_buf[3];
	} else {
		return -1;
	}
}

/* 0 = ; 1 = usb; 2 = sd; 3 = spi, 4 = bt; 5 = fm; 6 = aux; 7 = rec */
int mode_check(uint8_t *rcv_buf)
{
	char *mode_string;

	hsc025a_exec_command(0x02, 0x0c, 0x0000, rcv_buf, HSC025A_READ_TMOUT);
	while (rcv_buf[2] != 0x0c) {
		hsc025a_exec_command(0x02, 0x0c, 0x0000, rcv_buf, HSC025A_READ_TMOUT);
	}

	switch (rcv_buf[3]) {
	case 0:
		mode_string = "STANDBY";
		break;
	case 1:
		mode_string = "USB";
		break;
	case 2:
			mode_string = "SD";
		break;
	case 4:
		mode_string = "BLUETOOTH";
		break;
	}

	if (rcv_buf[2] == 0x0c) {
		return rcv_buf[3];
	} else {
		return -1;
	}
}

/* 0 = ; 1 = usb; 2 = sd; 3 = spi, 4 = bt; 5 = fm; 6 = aux; 7 = rec */
void mode_set(uint32_t mode)
{
	char *mode_string;

	switch (mode) {
	case 0:
		mode_string = "STANDBY";
		break;
	case 1:
		mode_string = "USB";
		break;
	case 2:
		mode_string = "SD";
		break;
	case 4:
		mode_string = "BLUETOOTH";
		break;
	}

	if (mode == HSC025A_MODE_BT) {
		bluetooth_open();
	}
	hsc025a_write(0x03, 0x0d, mode);

}

/* auto mode switch */
void mode_switch_auto(uint8_t *uart_rcv_buf)
{
	uint8_t current_mode;

	current_mode = mode_check(uart_rcv_buf);

	if (current_mode == HSC025A_MODE_BT) {
		mode_set(HSC025A_MODE_SD);
	} else if (current_mode == HSC025A_MODE_SD) {
		mode_set(HSC025A_MODE_BT);
	} else {
		mode_set(HSC025A_MODE_SD);
	}
}



/* each bluetooth command will return itself when success */
void bluetooth_open(void)
{
	hsc025a_write(0x03, 0x51, 0x0045);      // turn on bluetooth
	hsc025a_write(0x03, 0x51, 0x000a);      // turn on bluetooth discoverable
	hsc025a_write(0x03, 0x51, 0x000c);      // turn on bluetooth connectable
	delay_ms(500);
}

void bluetooth_close(void)
{
	hsc025a_write(0x03, 0x51, 0x0003);      // disconnect bluetooth
	// hsc025a_write(0x03, 0x51, 0x0009);      // turn off bluetooth discoverable
	// hsc025a_write(0x03, 0x51, 0x000b);      // turn off bluetooth connectable
	delay_ms(500);
}

/* play song*/
void song_play(void)
{
	hsc025a_write(0x02, 0x01, 0x0000);
}

/* pause song */
void song_pause(void)
{
	hsc025a_write(0x02, 0x02, 0x0000);
}

void song_play_or_pause(void)
{
	hsc025a_write(0x02, 0x09, 0x0000);
	// hsc025a_exec_command(0x02, 0x09, 0x0000, uart_rcv_buf, HSC025A_READ_TMOUT);
}

void song_stop(void)
{   // must use with song_play(), rather than song_play_or_pause()
	hsc025a_write(0x02, 0x0a, 0x0000);
}

/* next song */
void song_next(void)
{
	hsc025a_write(0x02, 0x03, 0x0000);
}

/* previous song */
void song_prev(void)
{
	hsc025a_write(0x02, 0x04, 0x0000);
}

void volumn_up(void)
{
	hsc025a_write(0x02, 0x05, 0x0000);
	hsc025a_write(0x02, 0x05, 0x0000);
	hsc025a_write(0x02, 0x05, 0x0000);
}

void volumn_down(void)
{
	hsc025a_write(0x02, 0x06, 0x0000);
	hsc025a_write(0x02, 0x06, 0x0000);
	hsc025a_write(0x02, 0x06, 0x0000);
}


/* set volum, range: 0x00(mute) to 0x1e(max) */
void volumn_set(uint8_t vol)
{
	if (vol > 0x1e) {
		vol = 0x1e;
	}
	hsc025a_write(0x03, 0x0f, (0x0f00 | vol));
}


int song_name_extr(uint8_t *rcv_buf, uint8_t *name_extr)
{
	uint32_t i;
	uint32_t hex_len = 0;

	for (i = 0;  i < SONG_NAME_LENGTH; i = i+2) {
		name_extr[i]   = 0x00;
		name_extr[i+1] = 0x20;
	}

	if (rcv_buf[2] != 0x38) {
		return -1;
	}

	for (i = 0;  i < SONG_NAME_LENGTH; i = i+2) {
		if (rcv_buf[3+i] != HSC025A_CMD_SUFFIX) {
			name_extr[i]   = rcv_buf[3+i+1];
			name_extr[i+1] = rcv_buf[3+i];
			hex_len = hex_len + 2;
		} else {
			break;
		}
	}

	return hex_len;
}
