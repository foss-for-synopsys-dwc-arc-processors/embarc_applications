#ifndef __OLED_H_
#define __OLED_H_



#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64

extern const uint8_t mushroom[];
extern const uint8_t sky1[];
extern const uint8_t sky2[];
extern const uint8_t sky3[];

extern const uint8_t F6x8[][6];
extern const uint8_t F14x16_Idx[];
extern const uint8_t F14x16[];
extern const uint8_t F8X16[];

extern const uint8_t bitmap_su[];
extern const uint8_t bitmap_du[];
extern const uint8_t bitmap_xia[];
extern const uint8_t bitmap_zai[];
extern const uint8_t bitmap_jie[];
extern const uint8_t bitmap_ma[];
extern const uint8_t bitmap_ge[];
extern const uint8_t bitmap_qu[];
extern const uint8_t bitmap_yi[];
extern const uint8_t bitmap_shou[];
extern const uint8_t bitmap_ting[];
extern const uint8_t bitmap_zhi[];

extern void OLED_CLS();
extern void OLED_P6x8Char(uint8_t ucIdxX, uint8_t ucIdxY, uint8_t ucData);
extern void OLED_P8x16Char(uint8_t x, uint8_t y, uint8_t ch);
extern void OLED_P6x8Str(uint8_t x, uint8_t y, uint8_t ch[]);
extern void OLED_P8x16Str(uint8_t x, uint8_t y, uint8_t ch[]);
extern void OLED_P14x16Str(uint8_t x, uint8_t y, uint8_t ch[]);
extern void draw_fft();

#endif


