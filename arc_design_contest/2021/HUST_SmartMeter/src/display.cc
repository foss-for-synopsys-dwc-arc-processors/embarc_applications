#include "display.h"

u8g_t u8g;


void u8g_prepare(void) {
	u8g_SetFont(&u8g, u8g_font_6x10);		/* set the current font and reset the font reference position to "Baseline" */
	u8g_SetFontRefHeightExtendedText(&u8g);		/* define the calculation method for the ascent and descent of the current font */
	u8g_SetDefaultForegroundColor(&u8g);		/* assign one of the default colors as current color index */
	u8g_SetFontPosTop(&u8g);			/* set the reference position for the character and string draw procedure */
}

void display_app_name(uint16_t kind_index){
	u8g_prepare();
	switch (kind_index){
		case 0:
			u8g_DrawStr(&u8g, 30,20, "ALL OFF");
			break;
		case 1:
			u8g_DrawStr(&u8g, 30,20, "Fan ON");
			break;
		case 2:
			u8g_DrawStr(&u8g, 30,20, "DDS ON");	
			break;
		case 3:
			u8g_DrawStr(&u8g, 30,20, "ALL ON");
			break;
		default:
			u8g_DrawStr(&u8g, 30,20, "None");
			break;
	}	
}

void display_app_power(uint16_t app_power){   //显示实时的功率值
	char s[10][10] ;
	float t;
	t= app_power*0.05;
	sprintf(s[0], "POWER:%4.1fw", t);//将数据转化为字符串
	//u8g_DrawStr(&u8g, 0, 40, "POWER:");
	u8g_DrawStr(&u8g, 30, 40, s[0]);
}

void m_u8g_init(void){
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, U8G_COM_SSD_I2C); /* create a new interface to a graphics display */
	u8g_Begin(&u8g); /* reset display and put it into default state */
}

void display(uint16_t index,uint16_t display_power){


	u8g_FirstPage(&u8g); /* marks the beginning of the picture loop; it cannot be used inside the picture loop */
	do {

		u8g_DrawStr(&u8g, 15, 0, "HUST-SandSilicon");
		display_app_name(index);
		display_app_power(display_power);
	} while (u8g_NextPage(&u8g));
}

void PrintEvent(sample_data_node_ptr front, int windows_len){
	for(uint32_t i=0;i<windows_len;i++){
		front = front->next_ptr;
		EMBARC_PRINTF("%d,%d,%d\n",front->IRMS,-(front->active_power),-(front->reactive_power));
	}
}
