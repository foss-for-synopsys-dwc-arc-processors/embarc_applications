#ifndef _DWIN_H_
#define _DWIN_H_

#include "embARC.h"
#include "embARC_debug.h"

#define IIC_MASTER_1 DFSS_IIC_0_ID
#define I2C_SLAVE_UART_ADDR 0x01

#define MAX_DWIN_CMD_DATA_LEN 100

#define DWIN_RTC_ADDR 0x0010
#define DWIN_TEMP_ADDR 0x1002
#define DWIN_HUMI_ADDR 0x1001
#define DWIN_PLAY_MODE_ADDR 0x2000
#define DWIN_CTRL_MODE_ADDR 0x2010
#define DWIN_BTN_PREV_ADDR 0x2100
#define DWIN_BTN_PLAY_ADDR 0x2102
#define DWIN_BTN_NEXT_ADDR 0x2104

#define DWIN_CMD_ADDR 0x2300
#define DWIN_SONG_NAME_ADDR 0x3001

#define DWIN_BTN_PREV_VALUE 0x01
#define DWIN_BTN_PLAY_VALUE 0x02
#define DWIN_BTN_NEXT_VALUE 0x03

#define DWIN_ICO_PREV_ADDR 0x2200
#define DWIN_ICO_PREV_NORM 0x00
#define DWIN_ICO_PREV_SHADE 0x01

#define DWIN_ICO_PLAY_ADDR 0x2202
#define DWIN_ICO_PAUSE_NORM 0x00
#define DWIN_ICO_PAUSE_SHADE 0x01
#define DWIN_ICO_PLAY_NORM 0x02
#define DWIN_ICO_PLAY_SHADE 0x03

#define DWIN_ICO_NEXT_ADDR 0x2204
#define DWIN_ICO_NEXT_NORM 0x00
#define DWIN_ICO_NEXT_SHADE 0x01

#define DWIN_LINE_1_ADDR 0x2010
#define DWIN_LINE_2_ADDR 0x2020
#define DWIN_LINE_3_ADDR 0x2030
#define DWIN_LINE_4_ADDR 0x2040
#define DWIN_LINE_5_ADDR 0x2050
#define DWIN_LINE_6_ADDR 0x2060
#define DWIN_LINE_7_ADDR 0x2070

#define DWIN_MODE_ADDR   0x2080

#define DWIN_LINE_POINT_1_ADDR 0x2110
#define DWIN_LINE_POINT_2_ADDR 0x2120
#define DWIN_LINE_POINT_3_ADDR 0x2130
#define DWIN_LINE_POINT_4_ADDR 0x2140
#define DWIN_LINE_POINT_5_ADDR 0x2150
#define DWIN_LINE_POINT_6_ADDR 0x2160
#define DWIN_LINE_POINT_7_ADDR 0x2170


#define WRITE_DATA 'w'
#define CHANGE_MODE 'c' 
#define ERASE_DATA  0x02
#define ERASE_LINE  0x01
#define UP_LINE     0x04 
#define DOWN_LINE   0x08 
#define COPY_LINE   0x10
#define PASTE_LINE  0x20
#define CANNOT_OPERATE 'n'




void Master1_slvdev_init(uint32_t slv_addr);
void dwin_write_cmd(uint16_t addr, uint8_t *data, uint32_t len);
uint8_t dwin_read_btn(uint16_t addr);
void dwin_disp_btn(uint16_t addr, uint8_t *ico, uint8_t num);

void dwin_disp_space(uint16_t addr, uint8_t len);

void dwin_disp_mode_standby(void);
void dwin_disp_mode_usb(void);
void dwin_disp_mode_sd(void);
void dwin_disp_mode_bt(void);
void dwin_disp_ctrl_mode_ges(void);
void dwin_disp_ctrl_mode_audio(void);
void dwin_disp_ges_enable(void);
void dwin_disp_ges_disable(void);
void dwin_disp_song_prev(void);
void dwin_disp_song_next(void);
void dwin_disp_song_play(void);
void dwin_disp_song_pause(void);
void dwin_disp_vol_up(void);
void dwin_disp_vol_down(void);
void dwin_disp_sw_ctrl_mode(void);
void dwin_disp_sw_play_mode(void);
void dwin_display(void);

void mode_erase_data(void);
void mode_erase_line(void);
void mode_number(void);
void mode_alpha(void);
void mode_down(void);
void mode_up(void);
void dwin_display_init(void);
void mode_full(void);
void mode_empty(void);
void mode_record(void);
void mode_init(void);
void mode_copy(void);
void mode_paste(void);
void mode_next_line(void);

void iic_change_address_paj(void);
void iic_write_paj(const void *data, uint32_t len);
void Cannot_write(void);
void Cannot_erase(void);

uint8_t * Next_line(uint8_t line,uint8_t *write_line_next,uint8_t *tmp_buf);
#endif
