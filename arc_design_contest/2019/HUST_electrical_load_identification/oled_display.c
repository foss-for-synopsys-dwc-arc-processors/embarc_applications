#include "oled_display.h"


void u8g_prepare(u8g_t *u8g) {
	u8g_SetFont(u8g, u8g_font_6x10);		/* set the current font and reset the font reference position to "Baseline" */
	u8g_SetFontRefHeightExtendedText(u8g);		/* define the calculation method for the ascent and descent of the current font */
	u8g_SetDefaultForegroundColor(u8g);		/* assign one of the default colors as current color index */
	u8g_SetFontPosTop(u8g);			/* set the reference position for the character and string draw procedure */
}
void u8g_welcome(u8g_t *u8g_t) {
	u8g_DrawStr(u8g_t, 38, 0, "2019 CPIPC");		/* draws a string at the specified x/y position */
	u8g_DrawStr(u8g_t, 8, 20, "Load Identification");
    u8g_DrawStr(u8g_t, 10,40, "By WLDMT from HUST");
}

void u8g_result(u8g_t *u8g_t,uint8_t res){
	u8g_DrawStr(u8g_t, 45, 0, "Result");
    switch (res)
    {
    case 0:
        u8g_DrawStr(u8g_t, 40, 30, "KIND NO");
        break;
    case 1:
        u8g_DrawStr(u8g_t, 40, 30, "KIND A");
        break;
    case 2:
        u8g_DrawStr(u8g_t, 40, 30, "KIND B");
        break;
    case 3:
        u8g_DrawStr(u8g_t, 40, 30, "KIND C");
        break;
    case 4:
        u8g_DrawStr(u8g_t, 40, 30, "KIND AB");
        break;
    case 5:
        u8g_DrawStr(u8g_t, 30, 30, "KIND AC");
        break;
    case 6:
    	u8g_DrawStr(u8g_t, 30, 30, "KIND BC");
    	break;
    default:
        break;
    }
}



static void first_draw(u8g_t *u8g_t) {
  // graphic commands to redraw the complete screen should be placed here  
    u8g_prepare(u8g_t);
    u8g_welcome(u8g_t);
}

static void second_draw(u8g_t *u8g_t,uint8_t res) {
  // graphic commands to redraw the complete screen should be placed here  
    u8g_prepare(u8g_t);
    u8g_result(u8g_t,res);
}


void display_welcome(u8g_t *u8g_t){
    u8g_FirstPage(u8g_t);  
    do {
        first_draw(u8g_t);
    } while( u8g_NextPage(u8g_t));
}


void display_result(u8g_t *u8g_t, uint8_t res){
    u8g_FirstPage(u8g_t);  
    do {
        second_draw(u8g_t, res);
    } while( u8g_NextPage(u8g_t));
}
