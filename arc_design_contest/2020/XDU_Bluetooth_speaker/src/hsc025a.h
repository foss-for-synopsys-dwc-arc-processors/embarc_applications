#ifndef _HSC025A_H_
#define _HSC025A_H_

#include "stdio.h"
#include "string.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

#include "board.h"
#include "dev_common.h"
#include "delay.h"

// HSC025A definition
#define HSC025A_UART_ID      DW_UART_0_ID
#define HSC025A_TIMENOW()	 OSP_GET_CUR_MS()
#define HSC025A_READ_TMOUT   500
#define MAX_RESP_LENGTH		 160
#define SONG_NAME_LENGTH     (MAX_RESP_LENGTH-4)

#define HSC025A_CMD_PREFIX   0x7e
#define HSC025A_CMD_SUFFIX   0xef

#define HSC025A_MODE_STANDBY 0x00
#define HSC025A_MODE_USB     0x01
#define HSC025A_MODE_SD      0x02
#define HSC025A_MODE_BT      0x04

// HSC025A function declaration
int      hsc025a_init(uint32_t baudrate);
uint32_t hsc025a_write(uint8_t len, uint8_t cmd, uint16_t param);
uint32_t hsc025a_read(uint8_t *buf, uint32_t cnt);
int      hsc025a_exec_command(uint8_t len, uint8_t cmd, uint16_t param, uint8_t *resp, uint32_t wait_ms);
uint32_t rcv_dat_extract(uint8_t *rcv_dat, uint8_t *ext_dat);

// HSC025A control commands
int      version_check(uint8_t *uart_rcv_buf);
int      mode_check(uint8_t *uart_rcv_buf);
void     mode_set(uint32_t mode);
void     mode_switch_auto(uint8_t *uart_rcv_buf);
void     bluetooth_open(void);
void     bluetooth_close(void);
void     song_pause(void);
void     song_play(void);
void     song_play_or_pause(void);
void     song_stop(void);
void     song_next(void);
void     song_prev(void);
void     volumn_up(void);
void     volumn_down(void);
void     volumn_set(uint8_t vol);
void     song_pause(void);
void     song_play(void);

int song_name_extr(uint8_t *rcv_buf, uint8_t *name_extr);

#endif
