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
#include "arc.h"
#include "arc_builtin.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "board.h"
#include "LED.h"
#include "dev_uart.h"

#include <string.h>
#include <stdio.h>
#include "embARC.h"
#include "embARC_debug.h"

static DEV_GPIO_PTR port_led5; //运动目标检测灯
static DEV_GPIO_PTR port_led6; //发送短信提示灯
static DEV_GPIO_PTR port_led7; //拨打电话提示灯
static DEV_GPIO_PTR port_car_run;  //行车模式
static DEV_GPIO_PTR port_car_stop; //熄火模式

static void delay_ms(volatile int z)
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}


void open_led5() 
{  
    port_led5 =  gpio_get_dev(DW_GPIO_PORT_B);  
	port_led5 -> gpio_open(0x1ff);	//打开PortB[8:0]
	port_led5 -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x1ff);	
	port_led5 -> gpio_write(0x1df, 0x1ff);
	delay_ms(500);
	port_led5 -> gpio_write(0x1ff, 0x1ff);
}


void open_led6() 
{  

	port_led6 =  gpio_get_dev(DW_GPIO_PORT_B);  
	port_led6 -> gpio_open(0x1ff);	//打开PortB[8:0]
	port_led6 -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x1ff);	
	port_led6 -> gpio_write(0x1bf, 0x1ff);
	delay_ms(2000);
	port_led6 -> gpio_write(0x1ff, 0x1ff);				

}


void open_led7() 
{  
	port_led7 =  gpio_get_dev(DW_GPIO_PORT_B);  
	port_led7 -> gpio_open(0x1ff);	//打开PortB[8:0]
	port_led7 -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x1ff);	
	port_led7 -> gpio_write(0x17f, 0x1ff);
	delay_ms(2000);
	port_led7 -> gpio_write(0x1ff, 0x1ff);				
}

void car_stop_led()
{
	port_car_stop = gpio_get_dev(DW_GPIO_PORT_C);
	port_car_stop-> gpio_open(0x3000000);
	port_car_stop-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x3000000);
	port_car_stop-> gpio_write(0x1000000, 0x3000000);		
}


void car_run_led()
{
	port_car_run = gpio_get_dev(DW_GPIO_PORT_C);
	port_car_run-> gpio_open(0x3000000);
	port_car_run-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x3000000);
	port_car_run-> gpio_write(0x2000000, 0x3000000);		
}


