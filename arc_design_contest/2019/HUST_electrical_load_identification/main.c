/* ------------------------------------------
 * Copyright (c) 2019, Synopsys, Inc. All rights reserved.

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
#include "ad7991.h"
#include "u8g.h"

#include "input_buffer.h"
#include "process.h"
#include "ssd1306_app_config.h"
#include "oled_display.h"

#include "load_identification_model.h"


#define AD7991_IIC_ID DFSS_IIC_0_ID

u8g_t u8g;

static AD7991_DEFINE(ad7991_sensor, AD7991_IIC_ID, ADC_I2C_SLAVE_ADDRESS);
int main(void)
{

    float ad_data[4] = {0};
	uint8_t proc_flag = 0;
	RESULT my_result = 0;
    /* initializing the Sensor Hub */
    ad7991_adc_init(ad7991_sensor);
    u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, U8G_COM_SSD_I2C);
    u8g_Begin(&u8g); /* reset display and put it into default state */

    display_welcome(&u8g);
    board_delay_ms(2000, 1);

	EMBARC_PRINTF("Load Identification\r\n");
    /* configuring GPIO to handle the "data ready" interrupt */
    while (1) {
        board_delay_ms(1, 1);
		EMBARC_PRINTF("Hello ARC\r\n");
		if(proc_flag == 0){
			if(one_frame_finish_flag == 0){
				ad7991_adc_read(ad7991_sensor, ad_data);   
				input_buffer_push_data(ad_data);     
			}
			else{
				my_result = Cnn_Net_Classify();
				display_result(&u8g, my_result);
				one_frame_finish_flag = 0;
				proc_flag = 1;
			}
		}
		else{
			proc_flag = 0;
			board_delay_ms(100, 1);
		}
    }
    return E_SYS;
}
