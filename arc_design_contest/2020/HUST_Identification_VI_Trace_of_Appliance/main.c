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
#include "VI_image.h"
#include "spi_att7053.h"
#include "VI_image_display.h"
#include "board.h"
#include "recognize_algorithm.h"

#define ATT7053_SPI0 		0								//ARC SPI0
#define COUNT 				144								//count 144 = 1us

/*
*	global variable definition
*/
uint8_t rgb_image[32*32*3]={0};

uint8_t kind_index=0XFF;

vi_data_t *data;

uint32_t Power = 0;

ATT7053_DEF(att7053,ATT7053_SPI0,DEV_SPI_CS_LINE_1);		//define att7053 device ptr



/** arc timer 1 interrupt routine
 * read att7053 current and votage register values
*/
static void att7053_read_isr(void *ptr)
{
	timer_int_clear(TIMER_1);
	Read_Reg(att7053,Current1_Waveform);
	Read_Reg(att7053,Votage_Waveform);

}

/** arc time 1 init
 * configure timer1 500us produce interrupt
*/
static void timer1_init(void){
	timer_stop(TIMER_1);

	int_disable(INTNO_TIMER1);

	int_handler_install(INTNO_TIMER1, att7053_read_isr);
	int_pri_set(INTNO_TIMER1, INT_PRI_MAX);

	int_enable(INTNO_TIMER1);

	timer_start(TIMER_1, TIMER_CTRL_IE | TIMER_CTRL_NH, 500*COUNT);   //500us timer
}


static void read_vi_value(void){
	for(int k=0;k<720;k++){
		data->i[k] = Read_Reg(att7053,Current1_Waveform);
		data->v[k] = Read_Reg(att7053,Votage_Waveform);
		usleep(100);
	}
}



int main(void)
{

	m_u8g_init();

	int32_t ret = ATT7053_init(att7053, 1200000);
	if(ret != E_OK)
		EMBARC_PRINTF(" Error!!!!\r\n");
	EMU_Init(att7053);

	model_init();

	timer1_init();

	while (1) {
		Power = Read_Reg(att7053,Active_Power_Register);
		if(Power > 10){
			read_vi_value();
			generate_VI_image(data,rgb_image);
			recognize_algorithm((void*)rgb_image,&kind_index);
			display(rgb_image,kind_index);
		}

	}

	return E_SYS;
}
