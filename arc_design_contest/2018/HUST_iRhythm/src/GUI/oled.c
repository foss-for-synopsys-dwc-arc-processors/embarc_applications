
#include "embARC.h"
#include "embARC_debug.h"

#include "OLED.h"

#include "stdio.h"

#include "include.h"

#define OLED_ADDRESS 0x78

DEV_IIC *OLED_I2C;




/**********************************************
// IIC Write Command
**********************************************/
inline void Write_IIC_Command(uint8_t IIC_Command)
{
	uint8_t send_data[2];

	send_data[0] = 0x00;
	send_data[1] = IIC_Command;

	OLED_I2C->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void *)(IIC_MODE_STOP));
	OLED_I2C->iic_write((void *)send_data, 2);

}

/**********************************************
// IIC Write Data
**********************************************/
inline void Write_IIC_Data(uint8_t IIC_Data)
{
	uint8_t send_data[2];

	send_data[0] = 0x40;
	send_data[1] = IIC_Data;

	OLED_I2C->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void *)(IIC_MODE_STOP));
	OLED_I2C->iic_write((void *)send_data, 2);
}

inline static void OLED_init()
{
	Write_IIC_Command(0xae);				/* display off, sleep mode */
	Write_IIC_Command(0xd5); Write_IIC_Command(0x80);		/* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
	Write_IIC_Command(0xa8); Write_IIC_Command(0x3f);		/* */

	Write_IIC_Command(0xd3); Write_IIC_Command(0x00);		/*  */

	Write_IIC_Command(0x40);				/* start line */

	Write_IIC_Command(0x8d); Write_IIC_Command(0x14);		/* [1] charge pump setting (p62): 0x014 enable, 0x010 disable */

	Write_IIC_Command(0x20); Write_IIC_Command(0x00);		/* */
	Write_IIC_Command(0xa1);				/* segment remap a0/a1*/
	Write_IIC_Command(0xc8);				/* c0: scan dir normal, c8: reverse */
	Write_IIC_Command(0xda);
	Write_IIC_Command(0x12);		/* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
	Write_IIC_Command(0x81); Write_IIC_Command(0xcf);		/* [1] set contrast control */
	Write_IIC_Command(0xd9); Write_IIC_Command(0xf1);		/* [1] pre-charge period 0x022/f1*/
	Write_IIC_Command(0xdb); Write_IIC_Command(0x40);		/* vcomh deselect level */

	Write_IIC_Command(0x2e);				/* 2012-05-27: Deactivate scroll */
	Write_IIC_Command(0xa4);				/* output ram to display */
	Write_IIC_Command(0xa6);				/* none inverted normal display mode */
	Write_IIC_Command(0xaf);				/* display on */
}


inline void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	Write_IIC_Command(0xb0 + y);
	Write_IIC_Command(((x & 0xf0) >> 4) | 0x10);
	Write_IIC_Command((x & 0x0f) | 0x01);
}

inline void OLED_Fill(uint8_t bmp_data)
{
	uint8_t y, x;

	for (y = 0; y < 8; y++) {
		Write_IIC_Command(0xb0 + y);
		Write_IIC_Command(0x01);
		Write_IIC_Command(0x10);

		for (x = 0; x < X_WIDTH; x++) {
			Write_IIC_Data(bmp_data);
		}
	}
}

void OLED_CLS(void)
{
	uint8_t y, x;

	for (y = 0; y < 8; y++) {
		Write_IIC_Command(0xb0 + y);
		Write_IIC_Command(0x01);
		Write_IIC_Command(0x10);

		for (x = 0; x < X_WIDTH; x++) {
			Write_IIC_Data(0);
		}
	}
}


//==============================================================
//函数名： void LCD_PutPixel(uint8 x,uint8 y)
//功能描述：绘制一个点（x,y）
//参数：真实坐标值(x,y),x的范围0～127，y的范围0～64
//返回：无
//==============================================================
void OLED_PutPixel(uint8_t x, uint8_t y)
{
	uint8_t data1;  //data1当前点的数据

	OLED_Set_Pos(x, y);
	data1 = 0x01 << (y % 8);
	Write_IIC_Command(0xb0 + (y >> 3));
	Write_IIC_Command(((x & 0xf0) >> 4) | 0x10);
	Write_IIC_Command((x & 0x0f) | 0x00);
	Write_IIC_Data(data1);
}
//==============================================================
//函数名： void LCD_Rectangle(uint8 x1,uint8 y1,
//                   uint8 x2,uint8 y2,uint8 color,uint8 gif)
//功能描述：绘制一个实心矩形
//参数：左上角坐标（x1,y1）,右下角坐标（x2，y2）
//      其中x1、x2的范围0～127，y1，y2的范围0～63，即真实坐标值
//返回：无
//==============================================================
void OLED_Rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t gif)
{
	uint8_t n;

	OLED_Set_Pos(x1, y1 >> 3);

	for (n = x1; n <= x2; n++) {
		Write_IIC_Data(0x01 << (y1 % 8));

		// if (gif == 1) { OLED_DLY_ms(50); }
	}

	OLED_Set_Pos(x1, y2 >> 3);

	for (n = x1; n <= x2; n++) {
		Write_IIC_Data(0x01 << (y2 % 8));
		// if(gif == 1)
		// 	OLED_DLY_ms(5);
	}

}
/**==============================================================
函 数 名: LED_P6x8Char
功能描述: 显示一个6x8标志ASCII字符串
输入数据:  uint8 ucIdxX       显示横坐标0~122
uint8 ucIdxY       页范围0~7
uint8 ucData       显示的字符串
输出数据: NONE
返 回 值: NONE
==============================================================**/
void OLED_P6x8Char(uint8_t ucIdxX, uint8_t ucIdxY, uint8_t ucData)
{
	uint8_t i, ucDataTmp;

	ucDataTmp = ucData - 32;

	if (ucIdxX > 122) {
		ucIdxX = 0;
		ucIdxY++;
	}

	OLED_Set_Pos(ucIdxX, ucIdxY);

	for (i = 0; i < 6; i++) {
		Write_IIC_Data(F6x8[ucDataTmp][i]);
	}
}

void OLED_P8x16Char(uint8_t x, uint8_t y, uint8_t ch)
{
	uint8_t c = 0, i = 0, j = 0;


	c = ch - 32;

	if (x > 120) {x = 0; y++;}

	OLED_Set_Pos(x, y);

	for (i = 0; i < 8; i++) {
		Write_IIC_Data(F8X16[c * 16 + i]);
	}

	OLED_Set_Pos(x, y + 1);

	for (i = 0; i < 8; i++) {
		Write_IIC_Data(F8X16[c * 16 + i + 8]);
	}

	x += 8;
	j++;

}

//==============================================================
//函数名：LCD_P6x8Str(uint8 x,uint8 y,uint8 *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================
void OLED_P6x8Str(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t c = 0, i = 0, j = 0;

	while (ch[j] != '\0') {
		c = ch[j] - 32;

		if (x > 126) {x = 0; y++;}

		OLED_Set_Pos(x, y);

		for (i = 0; i < 6; i++) {
			Write_IIC_Data(F6x8[c][i]);
		}

		x += 6;
		j++;
	}
}
//==============================================================
//函数名：LCD_P8x16Str(uint8 x,uint8 y,uint8 *p)
//功能描述：写入一组标准ASCII字符串
//参数：显示的位置（x,y），y为页范围0～7，要显示的字符串
//返回：无
//==============================================================
void OLED_P8x16Str(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t c = 0, i = 0, j = 0;

	while (ch[j] != '\0') {
		c = ch[j] - 32;

		if (x > 120) {x = 0; y++;}

		OLED_Set_Pos(x, y);

		for (i = 0; i < 8; i++) {
			Write_IIC_Data(F8X16[c * 16 + i]);
		}

		OLED_Set_Pos(x, y + 1);

		for (i = 0; i < 8; i++) {
			Write_IIC_Data(F8X16[c * 16 + i + 8]);
		}

		x += 8;
		j++;
	}
}
//输出汉字字符串
void OLED_P14x16Str(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t wm = 0, ii = 0;
	uint16_t adder = 1;

	while (ch[ii] != '\0') {
		wm = 0;
		adder = 1;

		while (F14x16_Idx[wm] > 127) {
			if (F14x16_Idx[wm] == ch[ii]) {
				if (F14x16_Idx[wm + 1] == ch[ii + 1]) {
					adder = wm * 14;
					break;
				}
			}

			wm += 2;
		}

		if (x > 118) {x = 0; y++;}

		OLED_Set_Pos(x , y);

		if (adder != 1) { // 显示汉字
			OLED_Set_Pos(x , y);

			for (wm = 0; wm < 14; wm++) {
				Write_IIC_Data(F14x16[adder]);
				adder += 1;
			}

			OLED_Set_Pos(x, y + 1);

			for (wm = 0; wm < 14; wm++) {
				Write_IIC_Data(F14x16[adder]);
				adder += 1;
			}
		} else {		 //显示空白字符
			ii += 1;
			OLED_Set_Pos(x, y);

			for (wm = 0; wm < 16; wm++) {
				Write_IIC_Data(0);
			}

			OLED_Set_Pos(x, y + 1);

			for (wm = 0; wm < 16; wm++) {
				Write_IIC_Data(0);
			}
		}

		x += 14;
		ii += 2;
	}
}
//输出汉字和字符混合字符串
void OLED_Print(uint8_t x, uint8_t y, uint8_t ch[])
{
	uint8_t ch2[3];
	uint8_t ii = 0;

	while (ch[ii] != '\0') {
		if (ch[ii] > 127) {
			ch2[0] = ch[ii];
			ch2[1] = ch[ii + 1];
			ch2[2] = '\0';			//汉字为两个字节
			OLED_P14x16Str(x , y, ch2);	//显示汉字
			x += 14;
			ii += 2;
		} else {
			ch2[0] = ch[ii];
			ch2[1] = '\0';			//字母占一个字节
			OLED_P8x16Str(x , y , ch2);	//显示字母
			x += 8;
			ii += 1;
		}
	}
}

//==============================================================
//函数名： void Draw_BMP(uint8 x,uint8 y)
//功能描述：显示BMP图片128×64
//参数：起始点坐标(x,y),x的范围0～127，y为页的范围0～7
//返回：无
//==============================================================
void Draw_BMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t bmp[])
{
	uint16_t ii = 0;
	uint8_t x, y;

	if (y1 % 8 == 0) { y = y1 / 8; }
	else { y = y1 / 8 + 1; }

	for (y = y0; y <= y1; y++) {
		OLED_Set_Pos(x0, y);

		for (x = x0; x < x1; x++) {
			Write_IIC_Data(bmp[ii++]);
		}
	}
}



void OLED_PrintInt16(uint8_t ucIdxX, uint8_t ucIdxY, int16_t sData)
{
	uint8_t i, j, k;


	if (sData > 99) {
		sData = 99;
	}

	j  = (uint16_t)(sData / 10) % 10;
	k  = (uint16_t) sData % 10;


	//十位
	OLED_P6x8Char(ucIdxX , ucIdxY, (uint8_t)0 + 48);
	OLED_P6x8Char(ucIdxX + 6, ucIdxY, (uint8_t)j + 48);


	//个位
	OLED_P6x8Char(ucIdxX + 12, ucIdxY, (uint8_t)k + 48);


	return;
}

/**
 * 画7个频谱条 0-128
 */

void draw_fft()
{
	uint8_t i = 0;

	for ( i = 0; i < 8; i++ ) {
		gui_info.fft[i] /= 2;
	}

	for ( i = 0; i < 4; i++ ) {

		if ( gui_info.fft[i] < gui_info.fft_show[i] ) {		//下降条

			if ( gui_info.fft_show[i] >= 3 ) {
				gui_info.fft_show[i] -= 3;
			}

			OLED_Set_Pos(gui_info.fft_show[i], i * 2);
			Write_IIC_Data(0x00);
			Write_IIC_Data(0x00);
			Write_IIC_Data(0x00);
			Write_IIC_Data(0x00);
		} else {
			OLED_Set_Pos(gui_info.fft_show[i], i * 2);

			while (gui_info.fft_show[i] < gui_info.fft[i]) {
				Write_IIC_Data(0xff);
				gui_info.fft_show[i] ++;
			}
		}
	}
}




/**
 * \brief       This function used to initialize gui configuration
 *
 */
void gui_init()
{
	uint32_t point = (OLED_ADDRESS >> 1);
	/* Initialize Gui Contorl Infomation to Default*/
	gui_info.screen_point = 0;
	gui_info.network_speed = -1;
	gui_info.decord_speed = -1;
	gui_info.main_cycle = -1;
	gui_info.flag_next = 0;
	gui_info.song_name = NULL;
	gui_info.next_song = NULL;
	gui_info.perf_update = 0;
	gui_info.fft = fft_out;

	OLED_I2C = iic_get_dev(DW_IIC_0_ID);
	OLED_I2C -> iic_open(DEV_MASTER_MODE, IIC_SPEED_ULTRA);

	OLED_I2C -> iic_control(IIC_CMD_MST_SET_TAR_ADDR, (void *)point);
	OLED_I2C -> iic_control(IIC_CMD_ENA_DEV, (void *)point);

	OLED_init();
	OLED_CLS();

	// led_row_ctl( 1,0xff );
	// led_row_ctl( 2,0xff );
	// led_row_ctl( 3,0xff );
	// led_row_ctl( 4,0xff );
	// led_row_ctl( 5,0xff );
	// led_row_ctl( 6,0xff );
	// led_row_ctl( 7,0xff );
	// led_row_ctl( 8,0xff );

}
