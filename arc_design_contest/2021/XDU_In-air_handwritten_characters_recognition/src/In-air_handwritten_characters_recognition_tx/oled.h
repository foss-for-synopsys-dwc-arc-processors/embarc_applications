#include "stdint.h"
//-----------------OLED port definition----------------

#define OLED_SCL_Clr() OLED_SCL = 0
#define OLED_SCL_Set() OLED_SCL = 1

#define OLED_SDA_Clr() OLED_SDA = 0
#define OLED_SDA_Set() OLED_SDA = 1

#define OLED_RES_Clr() OLED_RES = 0
#define OLED_RES_Set() OLED_RES = 1

#define OLED_CMD 0  //Write command
#define OLED_DATA 1 //Write data

uint32_t oled_pow(uint8_t m, uint8_t n);

void iot_oled_reset(void);
void OLED_Init(void);
void OLED_WR_Byte(uint8_t dat, uint8_t mode);
void OLED_Clear(void);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_DrawBMP(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[]);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no, uint8_t sizey);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t sizey);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Configuration(void);
void OLED_Display(void);