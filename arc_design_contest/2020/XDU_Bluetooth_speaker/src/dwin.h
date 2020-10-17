#ifndef _DWIN_H_
#define _DWIN_H_

#include "embARC.h"
#include "embARC_debug.h"

#define IIC_MASTER_1				1
#define I2C_SLAVE_UART_ADDR         0x01

#define MAX_DWIN_CMD_DATA_LEN       100

#define DWIN_RTC_ADDR           0x0010
#define DWIN_TEMP_ADDR          0x1001
#define DWIN_HUMI_ADDR          0x1002
#define DWIN_PLAY_MODE_ADDR     0x2000
#define DWIN_CTRL_MODE_ADDR     0x2010
#define DWIN_BTN_PREV_ADDR      0x2100
#define DWIN_BTN_PLAY_ADDR		0x2102
#define DWIN_BTN_NEXT_ADDR	    0x2104

#define DWIN_CMD_ADDR           0x2300
#define DWIN_SONG_NAME_ADDR     0x3001

#define DWIN_BTN_PREV_VALUE	    0x01
#define DWIN_BTN_PLAY_VALUE		0x02
#define DWIN_BTN_NEXT_VALUE		0x03

#define DWIN_ICO_PREV_ADDR      0x2200
#define DWIN_ICO_PREV_NORM      0x00
#define DWIN_ICO_PREV_SHADE     0x01

#define DWIN_ICO_PLAY_ADDR      0x2202
#define DWIN_ICO_PAUSE_NORM     0x00
#define DWIN_ICO_PAUSE_SHADE    0x01
#define DWIN_ICO_PLAY_NORM      0x02
#define DWIN_ICO_PLAY_SHADE     0x03

#define DWIN_ICO_NEXT_ADDR      0x2204
#define DWIN_ICO_NEXT_NORM      0x00
#define DWIN_ICO_NEXT_SHADE     0x01

void    Master1_slvdev_init(uint32_t slv_addr);
void    dwin_write_cmd(uint16_t addr, uint8_t *data, uint32_t len);
uint8_t dwin_read_btn(uint16_t addr);
void    dwin_disp_btn(uint16_t addr, uint8_t *ico, uint8_t num);

void    dwin_disp_space(uint16_t addr, uint8_t len);

void    dwin_disp_mode_standby(void);
void    dwin_disp_mode_usb(void);
void    dwin_disp_mode_sd(void);
void    dwin_disp_mode_bt(void);
void    dwin_disp_ctrl_mode_ges(void);
void    dwin_disp_ctrl_mode_audio(void);
void    dwin_disp_ges_enable(void);
void    dwin_disp_ges_disable(void);
void    dwin_disp_song_prev(void);
void    dwin_disp_song_next(void);
void    dwin_disp_song_play(void);
void    dwin_disp_song_pause(void);
void    dwin_disp_vol_up(void);
void    dwin_disp_vol_down(void);
void    dwin_disp_sw_ctrl_mode(void);
void    dwin_disp_sw_play_mode(void);

#endif
