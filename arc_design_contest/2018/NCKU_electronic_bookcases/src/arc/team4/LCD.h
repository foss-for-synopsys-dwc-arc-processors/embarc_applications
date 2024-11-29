#include "embARC.h"
#include "embARC_debug.h"
/***************************FOR PMOD CLS LCD*************************************/
#define MAX_LCD_COL  16
/***************************Fun Declaration******************************/
void Lcd_Init (DEV_IIC *LCD_obj);
void Lcd_ClearScreen (void);
void Lcd_Home (void);
void Lcd_Cursor (uint8_t row, uint8_t col);
void Lcd_CursorOff (void);
void Lcd_ClearRow (uint8_t row);
void Lcd_DisplayMode (int onoff);
void Lcd_DisplayOnOff (int onoff);
void Lcd_DisplayString(uint8_t row, uint8_t col, char *strng);
