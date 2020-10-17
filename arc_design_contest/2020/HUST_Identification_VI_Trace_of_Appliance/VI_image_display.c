#include "VI_image_display.h"
#include "board.h"
/* middleware level*/
#include "u8g.h"

u8g_t u8g;


void u8g_prepare(void) {
	u8g_SetFont(&u8g, u8g_font_6x10);		/* set the current font and reset the font reference position to "Baseline" */
	u8g_SetFontRefHeightExtendedText(&u8g);		/* define the calculation method for the ascent and descent of the current font */
	u8g_SetDefaultForegroundColor(&u8g);		/* assign one of the default colors as current color index */
	u8g_SetFontPosTop(&u8g);			/* set the reference position for the character and string draw procedure */
}

void draw_vi_image(u8g_uint_t x, u8g_uint_t y,void* vbimage){
	uint8_t* image_t = (uint8_t*)vbimage;
	int p_i=0,p_j=0;
	for(int i=0;i<64;i++){
		for(int j=0;j<64;j++){
			if(*(image_t+64*i+j)==1)	{
				u8g_DrawLine(&u8g,p_i+x,p_j+y,i+x,j+y);
				p_i=i;
				p_j=j;
			}
		}
	}
}


uint8_t THRESHOLD = 230;
void rgb2bitmap(uint8_t* src,uint8_t* dst){
	uint8_t* rgb_img = (uint8_t*)src;
	uint8_t* binary_img = (uint8_t*)dst;
	for(uint8_t i=0;i<32;i++){
		for(uint8_t j=0;j<32;j++){
			int b = rgb_img[32*i+j];
			int g = rgb_img[32*i+j+32*32];
			int r = rgb_img[32*i+j+32*32*2];
			int grey = (int)((r*30+g*59+b*11)/100);
			unsigned char bit=(32*i+j)%8;
			unsigned char pos=(32*i+j)/8;
			if(grey > THRESHOLD)	{
				*(dst+pos)|= 1 << bit ;
			}
		}
	}
}

void Draw_VI_Image(uint8_t* img){
	uint8_t binary_img[512]={0};
	rgb2bitmap(img,binary_img);
	u8g_DrawXBM (&u8g,0,0,64,64,binary_img);
}
void Display_app_name(uint8_t kind_index){
	u8g_prepare();
	switch (kind_index) {
	case 0:
		u8g_DrawStr(&u8g, 60,30, "Fan");
		break;
	case 1:
		u8g_DrawStr(&u8g, 60,30, "Hairdryer");
		break;
	case 2:
		u8g_DrawStr(&u8g, 60,30, "Fan");
		break;
	case 3:
		u8g_DrawStr(&u8g, 60,30, "Fan");
		break;
	case 4:
		u8g_DrawStr(&u8g, 60,30, "Fan");
		break;
	default:
		u8g_DrawStr(&u8g, 64,30, "None");
		break;
	}

}

void m_u8g_init(void){
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, U8G_COM_SSD_I2C); /* create a new interface to a graphics display */
	u8g_Begin(&u8g); /* reset display and put it into default state */
}

void display(uint8_t* rgb_image,uint8_t kind_index) {
	u8g_FirstPage(&u8g); /* marks the beginning of the picture loop; it cannot be used inside the picture loop */
	do {
		Draw_VI_Image(rgb_image);
		Display_app_name(kind_index);

	} while (u8g_NextPage(&u8g));
}
