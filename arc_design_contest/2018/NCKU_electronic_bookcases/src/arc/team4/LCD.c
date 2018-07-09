#include "LCD.h"

static DEV_IIC *LCD;
/*****************************************************************************
 Lcd_Init  - setup spi port for LCD driver
 *****************************************************************************/
void Lcd_Init(DEV_IIC *LCD_obj) {
	LCD = LCD_obj;
}

/*****************************************************************************
 Lcd_ClearScreen  - CLear screen
 *****************************************************************************/
void Lcd_ClearScreen(void) {
	uint8_t iic_cmd[4] = { 0x1B, '[', '0', 'j' };
	LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	LCD->iic_write(iic_cmd, 4);
	//LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
}

/*****************************************************************************
 Lcd_Home  - set cursor at home position
 *****************************************************************************/
void Lcd_Home(void) {
	uint8_t iic_cmd[6] = { 0x1B, '[', '0', ';', '0', 'H' };
	LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	LCD->iic_write(iic_cmd, 6);
	//LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
}

/*****************************************************************************
 Lcd_Cursor  - set cursor at postion row:col
 *****************************************************************************/
void Lcd_Cursor(uint8_t row, uint8_t col) {
	unsigned char col_first_digit = col / 10 + '0';
	unsigned char col_second_digit = col % 10 + '0';
	uint8_t iic_cmd[MAX_LCD_COL + 7] = { 0x1B, '[', row + '0', ';',
			col_first_digit, col_second_digit, 'H' };

	LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	LCD->iic_write(iic_cmd, 7);
	//LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
}

/*****************************************************************************
 Lcd_CursorOff  - swtch cursor off
 *****************************************************************************/
void Lcd_CursorOff(void) {
	uint8_t iic_cmd[4] = { 0x1B, '[', '0', 'c' };
	LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	LCD->iic_write(iic_cmd, 4);
	//LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
}
/*****************************************************************************
 Lcd_ClearRow  - clear selected row
 *****************************************************************************/
void Lcd_ClearRow(uint8_t row) {
	// set cursor to start position of the line
	Lcd_Cursor(row, 0);
	//erase  from current pos to end of line
	uint8_t iic_cmd[4] = { 0x1B, '[', '0', 'K' };
	LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	LCD->iic_write(iic_cmd, 4);
	//LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
}

/*****************************************************************************
 Lcd_DisplayMode  - Set  16-byte of 40 bytes stings length
 *****************************************************************************/
void Lcd_DisplayMode(int onoff) {
	uint8_t iic_cmd[4] = { 0x1B, '[', '0', 'h' };

	if (onoff == 1) {
		iic_cmd[2] = '0'; //wrap @ 16 chars
	} else {
		iic_cmd[2] = '1'; //wrap @ 40 chars
	}
	LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	LCD->iic_write(iic_cmd, 4);
	//LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
}

/*****************************************************************************
 Lcd_DisplayOnOff  - On/Off display
 *****************************************************************************/
void Lcd_DisplayOnOff(int onoff) {
	uint8_t iic_cmd[4] = { 0x1B, '[', '3', 'e' };

	if (onoff == 1) {
		iic_cmd[2] = '1'; //on
	} else {
		iic_cmd[2] = '0'; //off
	}
	LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	LCD->iic_write(iic_cmd, 4);
	//LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
}
void Lcd_DisplayString(uint8_t row, uint8_t col, char *strng) {
	unsigned int length = 0;
	unsigned char byte;
	unsigned int i;
	uint8_t *bufptr;
	uint8_t iic_cmd[MAX_LCD_COL];

	Lcd_ClearRow(row);
	Lcd_Cursor(row, col);

	bufptr = iic_cmd;
	for (i = length = 0; i < MAX_LCD_COL; i++) {
		byte = *strng++;
		if (byte) {
			if (byte != '\r' && byte != '\n') {
				*bufptr++ = byte;
				length++;
			}
		} else
			break;
	}
	LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	LCD->iic_write(iic_cmd, length);
	//LCD->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
}
