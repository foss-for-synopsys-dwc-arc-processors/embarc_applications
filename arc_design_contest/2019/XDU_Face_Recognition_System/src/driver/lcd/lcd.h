#include "embARC.h"
#include "board.h"
#include <stdio.h>
#include "embARC_debug.h"


#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define LCD_W            320
#define LCD_H            240

//lcd
static DEV_GPIO_PTR port_clk;     //AD0  gpio8b_3[7]
static DEV_GPIO_PTR port_mosi;     //AD1  gpio8b_3[6]
static DEV_GPIO_PTR port_rst;     //AD2   gpio8b_3[5]
static DEV_GPIO_PTR port_dc;     //AD3    gpio8b_3[4]
static DEV_GPIO_PTR port_cs1;     //AD4   gpio8b_3[3]

uint16_t BACK_COLOR, POINT_COLOR;   //±³¾°É«£¬»­±ÊÉ«

static void delay_us(volatile int z);
static void delay_ms(volatile int z);
void OLED_CS_Clr(void);
void OLED_CS_Set(void);
void OLED_RST_Clr(void);
void OLED_RST_Set(void);
void OLED_DC_Clr(void);
void OLED_DC_Set(void);
void OLED_CLK_Clr(void);
void OLED_CLK_Set(void);
void OLED_MOSI_Clr(void);
void OLED_MOSI_Set(void);
void LCD_Writ_Bus(char dat);
void LCD_WR_DATA8(char da);
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);
void LCD_WR_REG_DATA(int reg,int da);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void Lcd_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_DrawPoint(uint16_t x,uint16_t y);