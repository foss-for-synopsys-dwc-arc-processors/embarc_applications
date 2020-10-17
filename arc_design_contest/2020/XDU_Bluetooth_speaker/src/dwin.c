#include "dwin.h"

DEV_IIC * iic_master_1;

uint8_t dwin_cmd[MAX_DWIN_CMD_DATA_LEN] = {0};

/* all characters are in unicode form */
const uint8_t space_unicode[2] = {0x00, 0x20};
const uint8_t text_mode_standby[6] = {0x7a, 0x7a, 0x95, 0xf2, 0x00, 0x20};
const uint8_t text_mode_usb[6]     = {0x00, 0x55, 0x00, 0x53, 0x00, 0x42};
const uint8_t text_mode_sd[6]      = {0x00, 0x53, 0x00, 0x44, 0x53, 0x61};
const uint8_t text_mode_bt[6]      = {0x84, 0xdd, 0x72, 0x59, 0x00, 0x20};
const uint8_t text_ctrl_mode_ges[8]   = {0x62, 0x4b, 0x52, 0xbf, 0x6a, 0x21, 0x5f, 0x0f};
const uint8_t text_ctrl_mode_audio[8] = {0x8b, 0xed, 0x97, 0xf3, 0x6a, 0x21, 0x5f, 0x0f};

const uint8_t text_ges_enable[12]   = {0x62, 0x4b, 0x52, 0xbf, 0x5f, 0x00, 0x54, 0x2f, 0x00, 0x20, 0x00, 0x20};
const uint8_t text_ges_disable[12]  = {0x62, 0x4b, 0x52, 0xbf, 0x51, 0x73, 0x95, 0xed, 0x00, 0x20, 0x00, 0x20};
const uint8_t text_song_prev[12]    = {0x4e, 0x0a, 0x4e, 0x00, 0x66, 0xf2, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
const uint8_t text_song_next[12]    = {0x4e, 0x0b, 0x4e, 0x00, 0x66, 0xf2, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
const uint8_t text_song_play[12]    = {0x64, 0xad, 0x65, 0x3e, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
const uint8_t text_song_pause[12]   = {0x66, 0x82, 0x50, 0x5c, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20};
const uint8_t text_vol_up[12]       = {0x52, 0xa0, 0x59, 0x27, 0x97, 0xf3, 0x91, 0xcf, 0x00, 0x20, 0x00, 0x20};
const uint8_t text_vol_down[12]     = {0x51, 0xcf, 0x5c, 0x0f, 0x97, 0xf3, 0x91, 0xcf, 0x00, 0x20, 0x00, 0x20};
const uint8_t text_sw_ctrl_mode[12] = {0x63, 0xa7, 0x52, 0x36, 0x6a, 0x21, 0x5f, 0x0f, 0x52, 0x07, 0x63, 0x62};
const uint8_t text_sw_play_mode[12] = {0x64, 0xad, 0x65, 0x3e, 0x6a, 0x21, 0x5f, 0x0f, 0x52, 0x07, 0x63, 0x62};

/*****************
 * I2C_1 to UART
 * Connection:
 * J4.6 3V3
 * J4.5 GND
 * J4.4 SDA
 * J4.3 SCL
 ****************/
void Master1_slvdev_init(uint32_t slv_addr)
{
	iic_master_1 = iic_get_dev(IIC_MASTER_1);/* get IIC Master_0 device */

	iic_master_1->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);/* open IIC device in master mode */

	iic_master_1->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));/* Set target slave device address */
}


/* must use global variable to aviod cpu crash */
void dwin_write_cmd(uint16_t addr, uint8_t *data, uint32_t len)
{
	uint32_t i;

	/* Set next condition for following transmit operation */
	iic_master_1->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));

	dwin_cmd[0] = 0x5a;		// head high
	dwin_cmd[1] = 0xa5;		// head low
	dwin_cmd[2] = len + 3;	// data length
	dwin_cmd[3] = 0x82;		// write: 0x82;  read: 0x83
	dwin_cmd[4] = (uint8_t)(addr >> 8);			// addr(high)
	dwin_cmd[5] = (uint8_t)(addr & 0x00ff);		// addr(low)

	for (i = 0; i < len; i++) {
		dwin_cmd[6+i] = data[i];
	}

	iic_master_1->iic_write(dwin_cmd, len+6);
}



uint8_t dwin_read_btn(uint16_t addr)
{
	uint32_t i;
	uint8_t  read_btn_cmd[10];

	uint8_t addr_h = (uint8_t)(addr >> 8);
	uint8_t addr_l = (uint8_t)(addr & 0x00ff);

	/* Set next condition for following transmit operation */
	iic_master_1->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));

	dwin_cmd[0] = 0x5a;					// head high
	dwin_cmd[1] = 0xa5;					// head low
	dwin_cmd[2] = 0x04;					// data length
	dwin_cmd[3] = 0x83;					// write: 0x82;  read: 0x83
	dwin_cmd[4] = addr_h;				// addr(high)
	dwin_cmd[5] = addr_l;				// addr(low)
	dwin_cmd[6] = 0x01;					// length of returned key value

	iic_master_1->iic_write(dwin_cmd, 7);


	uint8_t read_buf[16] = {0};

	iic_master_1->iic_read(read_buf, 8);

	while (!((read_buf[3] == addr_h) && (read_buf[4] == addr_l))) {
		iic_master_1->iic_write(dwin_cmd, 7);
		iic_master_1->iic_read(read_buf, 8);
	}


	dwin_cmd[2] = 0x05;							// data length
	dwin_cmd[3] = 0x82;							// write: 0x82;  read: 0x83
	dwin_cmd[6] = 0x00;							// length of returned key value
	dwin_cmd[7] = 0x00;							// length of returned key value

	iic_master_1->iic_write(dwin_cmd, 8);

	return read_buf[7];
}

void dwin_disp_btn(uint16_t addr, uint8_t *ico, uint8_t num)
{
	ico[1] = num;
	dwin_write_cmd(addr, ico, 2);
}

void dwin_disp_space(uint16_t addr, uint8_t len)
{
	uint32_t i;

	for (i = 0; i < len; i++) {
		dwin_write_cmd(addr+i, (uint8_t *)space_unicode, 2);
	}
}

void dwin_disp_mode_standby(void)		{ dwin_write_cmd(DWIN_PLAY_MODE_ADDR, (uint8_t *)text_mode_standby, 6); };
void dwin_disp_mode_usb(void)			{ dwin_write_cmd(DWIN_PLAY_MODE_ADDR, (uint8_t *)text_mode_usb, 6); };
void dwin_disp_mode_sd(void)			{ dwin_write_cmd(DWIN_PLAY_MODE_ADDR, (uint8_t *)text_mode_sd, 6); };
void dwin_disp_mode_bt(void)			{ dwin_write_cmd(DWIN_PLAY_MODE_ADDR, (uint8_t *)text_mode_bt, 6); };
void dwin_disp_ctrl_mode_ges(void)		{ dwin_write_cmd(DWIN_CTRL_MODE_ADDR, (uint8_t *)text_ctrl_mode_ges, 8); };
void dwin_disp_ctrl_mode_audio(void)	{ dwin_write_cmd(DWIN_CTRL_MODE_ADDR, (uint8_t *)text_ctrl_mode_audio, 8); };


void dwin_disp_ges_enable(void) 	{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_ges_enable, 12); }
void dwin_disp_ges_disable(void) 	{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_ges_disable, 12); }
void dwin_disp_song_prev(void) 		{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_song_prev, 12); }
void dwin_disp_song_next(void) 		{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_song_next, 12); }
void dwin_disp_song_play(void) 		{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_song_play, 12); }
void dwin_disp_song_pause(void) 	{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_song_pause, 12); }
void dwin_disp_vol_up(void) 		{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_vol_up, 12); }
void dwin_disp_vol_down(void) 		{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_vol_down, 12); }
void dwin_disp_sw_ctrl_mode(void) 	{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_sw_ctrl_mode, 12); }
void dwin_disp_sw_play_mode(void) 	{ dwin_write_cmd(DWIN_CMD_ADDR, (uint8_t *)text_sw_play_mode, 12); }


