/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */

#include "embARC.h"
#include "embARC_debug.h"

/* middleware level*/
#include "u8g.h"

extern "C" {
#include "wm8978.h"
#include "wm8978i2s.h"
}
#include "codec.h"
#include "mfcc.h"

#include <stdlib.h>
#include "tensorflow/lite/micro/examples/voice_detection_experimental/main_functions.h"

u8g_t u8g;

void u8g_prepare(void) {
	u8g_SetFont(&u8g, u8g_font_6x10);		/* set the current font and reset the font reference position to "Baseline" */
	u8g_SetFontRefHeightExtendedText(&u8g);		/* define the calculation method for the ascent and descent of the current font */
	u8g_SetDefaultForegroundColor(&u8g);		/* assign one of the default colors as current color index */
	u8g_SetFontPosTop(&u8g);			/* set the reference position for the character and string draw procedure */
}

void u8g_box_frame_path() {
        u8g_DrawStr(&u8g, 20, 20, "Detection Result:");         /* draws a string at the specified x/y position */
        u8g_DrawStr(&u8g, 30, 40, "Pathological");
}

void u8g_box_frame_heal() {
        u8g_DrawStr(&u8g, 20, 20, "Detection Result:");         /* draws a string at the specified x/y position */
        u8g_DrawStr(&u8g, 45, 40, "Healthy");
}

/** draw five pages in OLED */
void draw_path() {
	u8g_prepare();
	u8g_box_frame_path();
}

void draw_heal() {
	u8g_prepare();
	u8g_box_frame_heal(); 
}

static MFCC_STR mfcc_str;

void run_mic(MFCC_STR* mfcc_str, int8_t* mfcc_buffer)
{
	
	wm8978_init_func();//Config the WM8978 through I2C.

	printf("wm8978_init_func init\n");

	int16_t* rx_buffer1 = (int16_t*)malloc(sizeof(int16_t) * 640*18);

	if (rx_buffer1 == NULL)
	{
		printf("The rx_buffer1 malloc failed\n");
	}

	int16_t* audio_buf = (int16_t*)malloc(sizeof(int16_t) * audio_buffer_len);
	if (audio_buf == NULL)
	{
		printf("The audio_buf malloc failed\n");
	}

  	printf("codec init\n");

	CODEC_init(audio_buf, rx_buffer1);//Start I2S transfer and interrupts.

  	printf("codec end\n");

	extract_features(mfcc_str, audio_buf, mfcc_buffer);

	for(int i = 0; i < NUM_FRAMES * NUM_MFCC_COEFFS; i++)
		printf("%d ", mfcc_buffer[i]);
	printf("\n");

	
	free(rx_buffer1);
	free(audio_buf);
}

void oled_init()
{
	EMBARC_PRINTF("oled init\n");
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, U8G_COM_SSD_I2C);
	//EMBARC_PRINTF("u8glib\r\n");
	EMBARC_PRINTF("Display Width: %u, Display Height: %u\r\n" , u8g_GetWidth(&u8g), u8g_GetHeight(&u8g));
	u8g_Begin(&u8g); 

}

void show_pathological_result()
{
	u8g_FirstPage(&u8g); 
	do
	{
		draw_path();
	} while (u8g_NextPage(&u8g));

}

void show_healthy_result()
{
	u8g_FirstPage(&u8g); 
	do
	{
		draw_heal();
	} while (u8g_NextPage(&u8g));

}


/** main entry for running ntshell */
int main(void)
{

	mfcc_init(&mfcc_str, NUM_MFCC_COEFFS, FRAME_LEN, MFCC_DEC_BITS);
	printf("mfcc init\n");

	int8_t* mfcc_buffer = (int8_t*)malloc(sizeof(int8_t) * NUM_FRAMES * NUM_MFCC_COEFFS);
	if (mfcc_buffer == NULL)
	{
		printf("The mfcc_buffer malloc failed\n");
	}

	run_mic(&mfcc_str, mfcc_buffer);

	int result = run_model(mfcc_buffer);

	printf("result = %d\n", result);
	
	oled_init();

	if(result == 1 )
	{
		show_pathological_result();
	}else if(result == 0)
	{
		show_healthy_result();
	}
	
	free(mfcc_buffer);

	return E_SYS;
}

