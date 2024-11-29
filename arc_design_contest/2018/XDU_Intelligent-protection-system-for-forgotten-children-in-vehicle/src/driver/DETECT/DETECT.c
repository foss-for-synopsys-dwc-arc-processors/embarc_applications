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
#include "dev_uart.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "DETECT.h"

static DEV_GPIO_PTR port_help;
static DEV_GPIO_PTR port_child_detect_open;
static DEV_GPIO_PTR port_child_detect_close;

static void delay_ms(volatile int z) 
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
       for(y=z;y>0;y--);
}


void led_initial()
{
	port_help = gpio_get_dev(DW_GPIO_PORT_A);//定义port_a接口
	port_help ->gpio_open(0x300);//打开PortA[8]=蜂鸣器和PortA[9]=LED闪烁
	port_help ->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x300);//设置portA[11]为输出		
	port_help ->gpio_write(0x000, 0x300);
}




void help(void)  
{	 
    port_help = gpio_get_dev(DW_GPIO_PORT_A);//定义port_a接口
	port_help ->gpio_open(0x300);//打开PortA[8]=蜂鸣器和PortA[9]=LED闪烁
	port_help ->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x300);//设置portA[11]为输出		
	port_help ->gpio_write(0x300, 0x300);	
    delay_ms(1000);
	port_help ->gpio_write(0x000, 0x300);
	delay_ms(1000);
}


void open_child_detect(void)  
{	 
    port_child_detect_open = gpio_get_dev(DW_GPIO_PORT_C);//定义port_C接口
	port_child_detect_open->gpio_open(0x80000);
	port_child_detect_open ->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x80000);//设置portC[19]为输出		
	port_child_detect_open ->gpio_write(0x80000, 0x80000);	
	
}
 
 
void close_child_detect(void)  
{	 
    port_child_detect_close  = gpio_get_dev(DW_GPIO_PORT_C);//定义port_C接口
	port_child_detect_close ->gpio_open(0x80000);
	port_child_detect_close ->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x80000);//设置portC[19]为输出		
	port_child_detect_close ->gpio_write(0x00000, 0x80000);	
}


