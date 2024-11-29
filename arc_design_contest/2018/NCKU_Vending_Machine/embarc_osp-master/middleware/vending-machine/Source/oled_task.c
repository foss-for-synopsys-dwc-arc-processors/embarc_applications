/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* middleware level */
#include "u8g.h"
#include "vm_task.h"

u8g_t u8g;

// ********************************************************************************
// variables used for vending machine, indicate the info for all 4 items
char name[7] = "Chips";
char type[7] = {0};
char temp[5] = {"31.75"};
char user[4] = {"5013"};
int number[4] = {1, 2, 3, 4};
int oled_status = 0;
int user_receive_status = 0;
vm_data oled_data = {0};

enum oled_status_list {
	normal = 0, input_1, input_2, input_3, input_4, recommand, check, confirm
};
// ********************************************************************************

void u8g_prepare(void) {
	u8g_SetFont(&u8g, u8g_font_6x10);		    /* set the current font and reset the font reference position to "Baseline" */
	u8g_SetFontRefHeightExtendedText(&u8g);		/* define the calculation method for the ascent and descent of the current font */
	u8g_SetDefaultForegroundColor(&u8g);		/* assign one of the default colors as current color index */
	u8g_SetFontPosTop(&u8g);			        /* set the reference position for the character and string draw procedure */
}

/** first page in OLED */
void u8g_box_frame(uint8_t a) {
	u8g_DrawStr(&u8g, 0, 0, "embARC");		   /* draws a string at the specified x/y position */
	u8g_DrawBox(&u8g, 5,10,20,10);			   /* draw a box (filled frame), starting at x/y position (upper left edge) */
	u8g_DrawBox(&u8g, 10+a,15,30,7);
	u8g_DrawStr(&u8g, 0, 30, "drawFrame");
	u8g_DrawFrame(&u8g, 5,10+30,20,10);		   /* draw a frame, starting at x/y position (upper left edge) */
	u8g_DrawFrame(&u8g, 10+a,15+30,30,7);
}

/** second page in OLED */
void u8g_string(uint8_t a) {
	u8g_DrawStr(&u8g, 30+a,31, " 0");
	u8g_DrawStr90(&u8g, 30,31+a, " 90");		/* rotate string output by 90 degree */
	u8g_DrawStr180(&u8g, 30-a,31, " 180");		/* rotate string output by 180 degree */
	u8g_DrawStr270(&u8g, 30,31-a, " 270");		/* rotate string output by 270 degree */
}

/** third page in OLED */
void u8g_line(uint8_t a) {
	u8g_DrawStr(&u8g, 0, 0, "drawLine");
	u8g_DrawLine(&u8g, 7+a, 10, 40, 55);		/* draw a line from (x1, y1) to (x2, y2) */
	u8g_DrawLine(&u8g, 7+a*2, 10, 60, 55);
	u8g_DrawLine(&u8g, 7+a*3, 10, 80, 55);
	u8g_DrawLine(&u8g, 7+a*4, 10, 100, 55);
}

/** forth page in OLED */
void u8g_ascii_1(void) {
	char s[2] = " ";
	uint8_t x, y;
	u8g_DrawStr(&u8g, 0, 0, "ASCII page 1");
	for( y = 0; y < 6; y++ ) {
		for( x = 0; x < 16; x++ ) {
			s[0] = y*16 + x + 32;
			u8g_DrawStr(&u8g, x*7, y*10+10, s);
		}
	}
}

/** fifth page in OLED */
void u8g_ascii_2(void) {
	char s[2] = " ";
	uint8_t x, y;
	u8g_DrawStr(&u8g, 0, 0, "ASCII page 2");
	for( y = 0; y < 6; y++ ) {
		for( x = 0; x < 16; x++ ) {
			s[0] = y*16 + x + 160;
			u8g_DrawStr(&u8g, x*7, y*10+10, s);
		}
	}
}

uint8_t draw_state = 0;

/** test page in OLED 
void u8g_test(int index) {
	static char buffer[30];
	u8g_SetFont(&u8g, u8g_font_gdr30);
	u8g_DrawStr(&u8g, 0, 35, name[index]);
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_DrawStr(&u8g, 0, 50, strcat("type: ", type[index]));
	xsprintf(buffer, "left: %d, temp: %s", number[index], temp);
	u8g_DrawStr(&u8g, 0, 60, buffer);
}
*/

void u8g_cover(void) {
	static char buffer[30];
	u8g_SetFont(&u8g, u8g_font_gdr20);
	u8g_DrawStr(&u8g, 0, 25, "NCKUEE");
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_DrawStr(&u8g, 0, 40, "Vending Machine...");
	u8g_DrawStr(&u8g, 0, 50, "Please enter your");
	u8g_DrawStr(&u8g, 0, 60, "user account.");
}

void u8g_recommand(void) {
	static char buffer[30];
	u8g_SetFont(&u8g, u8g_font_gdr20);
	u8g_DrawStr(&u8g, 0, 25, name);
	u8g_SetFont(&u8g, u8g_font_6x10);
	xsprintf(buffer, "(%s) recommanded.", type);
	u8g_DrawStr(&u8g, 0, 40, buffer);
	u8g_DrawStr(&u8g, 0, 50, "Please enter your");
	u8g_DrawStr(&u8g, 0, 60, "choice of item.");
}

void u8g_check(void) {
	static char buffer[30];
	u8g_SetFont(&u8g, u8g_font_gdr20);
	u8g_DrawStr(&u8g, 0, 35, name);
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_DrawStr(&u8g, 0, 50, "purchase?");
	u8g_DrawStr(&u8g, 0, 60, "Press # to confirm.");
}

void u8g_confirm(void) {
	static char buffer[30];
	u8g_SetFont(&u8g, u8g_font_gdr20);
	u8g_DrawStr(&u8g, 0, 35, "Thanks");
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_DrawStr(&u8g, 0, 50, "Vielen Danke.");
	xsprintf(buffer, "Terima kasih.");
	u8g_DrawStr(&u8g, 0, 60, buffer);
}

void u8g_input(int index) {
	static char buffer[30];
	char output_user[4] = {0};
	for (int i = 0; i <= index; i++)
		output_user[i] = user[i];
	u8g_SetFont(&u8g, u8g_font_gdr30);
	u8g_DrawStr(&u8g, 0, 35, output_user);
	u8g_SetFont(&u8g, u8g_font_6x10);
	u8g_DrawStr(&u8g, 0, 50, "inputted.");
	xsprintf(buffer, "welcome, temp: %s", temp);
	u8g_DrawStr(&u8g, 0, 60, buffer);
}

/** draw five pages in OLED */
void draw(void) {
	u8g_prepare();
	/*
	switch(draw_state >> 3) {
		case 0: u8g_box_frame(draw_state&7); break;
		case 1: u8g_string(draw_state&7); break;
		case 2: u8g_line(draw_state&7); break;
		//case 3: u8g_ascii_1(); break;
		//case 4: u8g_ascii_2(); break;
		case 3: u8g_test_1(); break;
	}*/
	switch (oled_status){
		case normal:    u8g_cover();     break;
		case input_1:   u8g_input(0);    break;
		case input_2:   u8g_input(1);    break;
		case input_3:   u8g_input(2);    break;
		case input_4:   u8g_input(3);    break;
		case recommand: u8g_recommand(); break;
		case check:     u8g_check();     break;
		case confirm:   
			u8g_confirm();
			oled_status = normal;
			vTaskDelay(5000);
		break;
	}
}

void oled_task(void *p_arg)
{
	// decode the queue if there is new data receive

	/*
	if( xOledQueue != 0 )	
	{
		if( xQueueReceive( xOledQueue, &oled_data, 0 ) )
		{
			if (oled_data.source_id == id_temp)
				sprintf(temp, "%*.*lf", 2, 2, oled_data.body[0].f);
		}
	}
	*/


	EMBARC_PRINTF("entering oled task\r\n");
	EMBARC_PRINTF("u8glib\r\n");

	EMBARC_PRINTF("u8g_InitComFn\r\n");
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_hw_spi, U8G_COM_SSD_SPI); /* create a new interface to a graphics display */


	EMBARC_PRINTF("Display Width: %u, Display Height: %u\r\n" , u8g_GetWidth(&u8g), u8g_GetHeight(&u8g));

	u8g_Begin(&u8g); /* reset display and put it into default state */


	while(1) {
		// EMBARC_PRINTF("Update frame [%d]\r\n", draw_state);
		/* picture loop */
		u8g_FirstPage(&u8g); /* marks the beginning of the picture loop; it cannot be used inside the picture loop */
		do {
			draw();
		} while (u8g_NextPage(&u8g)); /* marks the end of the body of the picture loop */

		draw_state++;
		if (draw_state >= 8*8) {	/* refresh 8 times for every "draw_state" */
			draw_state = 0;
		}

		// decode the queue if there is new data receive

		if (oled_status > 7)
			oled_status = 0;

		if( xOledQueue != 0 )	
		{
			if( xQueueReceive( xOledQueue, &oled_data, 0 ) )
			{
				if (oled_data.source_id == id_temp)
				{
					sprintf(temp, "%*.*lf", 2, 2, oled_data.body[0].f);
					// EMBARC_PRINTF("oled receive temp: %s\r\n", temp);
				}
				// else if (oled_data.source_id == id_numpad)
				else if (oled_data.source_id == id_main)
				{
					if (oled_data.target_item == no_item)
					{
						if (user_receive_status >= 4) user_receive_status = 0;

						user[user_receive_status] = (char)oled_data.body[0].i;
						++user_receive_status;
						++oled_status;
					} else if (oled_data.status == sell_recommand)
					// for merchandise recommand
					{
						oled_status = recommand;
						strncpy(name, oled_data.name, 6);
						strncpy(type, oled_data.type, 6);
					} else if (oled_data.status == purchase_check)
					// for user to select the item
					{
						oled_status = check;
						strncpy(name, oled_data.name, 6);
						strncpy(type, oled_data.type, 6);
					} else if (oled_data.status == purchase_confirm)
					// for user to confirm the deal
					{
						oled_status = confirm;
					} 
				}
				/*
				else if (oled_data.source_id == id_wifi)
				{
					if (oled_data.status == sell_recommand)
						oled_status = recommand;
					else if (user_input_1  <= oled_data.status && oled_data.status <= user_input_4)
					{
						user[oled_data.status - user_input_1] = oled_data.user[oled_data.status - user_input_1];
						oled_status = oled_data.status - user_input_1 + 2;
					}
					else
					{
						oled_status = normal;
						for (int i = 0; i <= 4; i++)
						{
							number[i] = oled_data.body[0].i;
							sprintf(type[i], "%s", oled_data.type);
							sprintf(name[i], "%s", oled_data.name);
							EMBARC_PRINTF("oled receive data type: %s\r\n", oled_data.type);
							EMBARC_PRINTF("oled receive data name: %s\r\n", oled_data.name);
						}
					}
				}
				*/
			}
		}
		vTaskDelay(100);
	}
}