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
#include "GPRS.h"
#include "dev_uart.h"

#include <string.h>
#include <stdio.h>
#include "embARC.h"
#include "embARC_debug.h"

#define INTV 20
uint8_t rcv_buf[20];

static void delay_ms(volatile int z)
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}


void call_gh(void)
{	
	DEV_UART *dev_uart0 = NULL;
	uint32_t baudrate = 115200;	
	dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0-> uart_open(baudrate);
	uint8_t i;
	int8_t *cmd[2]={"ATD18710895393;\r\n",};
	for(i = 0; i < 4; i++)
	{
		dev_uart0->uart_write(cmd[i], strlen(cmd[i]));
		delay_ms(100);
	}	
	uint8_t end[]={0x1a,0x00};
	dev_uart0->uart_write(end, strlen(end));
}

void call_lpf(void)
{	
	DEV_UART *dev_uart0 = NULL;
	uint32_t baudrate = 115200;	
	dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0-> uart_open(baudrate);
	uint8_t i;
	int8_t *cmd[2]={"ATD18829210907;\r\n",};
	for(i = 0; i < 4; i++)
	{
		dev_uart0->uart_write(cmd[i], strlen(cmd[i]));
		delay_ms(100);
	}	
	uint8_t end[]={0x1a,0x00};
	dev_uart0->uart_write(end, strlen(end));
}




void send(uint8_t *phoneNum, uint8_t *msg)
{	
	DEV_UART *dev_uart0 = NULL;
	uint32_t baudrate = 115200;	
	dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0-> uart_open(baudrate);
	
	uint8_t i;
	uint8_t pnum[64];
	int8_t *cmd[5]={"AT+CMGF=1\r\n", "AT+CSCS=\"UCS2\"\r\n",
				  "AT+CSCA?\r\n",  "AT+CSMP=17,167,0,25\r\n",};
	snprintf(pnum, 64, "AT+CMGS=\"%s\"\r\n", phoneNum);
	for(i = 0; i < 4; i++)
	{
		dev_uart0->uart_write(cmd[i], strlen(cmd[i]));
		delay_ms(100);
	}	
	dev_uart0->uart_write(pnum, strlen(pnum));
	delay_ms(100);
	dev_uart0->uart_write(msg, strlen(msg));
	delay_ms(100);
	uint8_t end[]={0x1a,0x00};
	dev_uart0->uart_write(end, strlen(end));
}




void send_data1(void)
{
	DEV_UART *dev_uart0 = NULL;
	uint32_t baudrate = 115200;
	dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0->uart_open(baudrate);

	uint8_t *aa="AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n";
    uint8_t *bb="AT+SAPBR=3,1,\"APN\",\"CMNET\"\r\n";
    uint8_t *cc="AT+SAPBR=1,1\r\n";
    uint8_t *dd="AT+HTTPINIT\r\n" ;  //初始化Http
    uint8_t *ee="AT+HTTPPARA=\"URL\",\"http://123.206.81.226:8080/boxes/take/box001/1\"\r\n" ;//设置URL	
    uint8_t *ff="AT+HTTPACTION=0\r\n" ; //0表示get,1表示post,2表示head


	dev_uart0->uart_write(aa, strlen(aa));
	delay_ms(INTV);
	
	dev_uart0->uart_write(bb, strlen(bb));
	delay_ms(INTV);
	
	dev_uart0->uart_write(cc, strlen(cc));
	delay_ms(INTV);
	
	dev_uart0->uart_write(dd, strlen(dd));
	delay_ms(INTV);	
	
	dev_uart0->uart_write(ee, strlen(ee));
	delay_ms(INTV);
	
	dev_uart0->uart_write(ff, strlen(ff));
	delay_ms(INTV);

}





