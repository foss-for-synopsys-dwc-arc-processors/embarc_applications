#include "HC_05.h"
#include "stdarg.h"
#include "stdio.h"
#include<stdarg.h>

DEV_UART * UART2 = NULL;

uint32_t HC_05_init()
{
	/*
     * UART 2 initialization
	 * Modify \embARC\board\emsk\common\emsk_init.c
	 * PM5_LR_GPIO_A    to   PM5_LR_SPI_M2 -name BUG
	 * J5.12 3V3
	 * J5.11 GND
	 * J5.9  UART2_RXD
	 * J5.8  UART2_TXD
	 */

	UART2 = uart_get_dev(HC_05_UART_ID);

 	//UART2 = uart_get_dev(DFSS_UART_1_ID);
	int32_t uart2_stat = UART2->uart_open(UART_BAUDRATE_9600);

	if (UART2 == NULL)
		return -1;
	else if (uart2_stat == E_OPNED || uart2_stat == E_OK) {
		UART2->uart_control(UART_CMD_SET_BAUD, (void *)(UART_BAUDRATE_9600)) ;
		return 1;
	} else
		return 0;
}

uint32_t HC_05_write(uint8_t *buf, uint32_t cnt)
{
	if (UART2 == NULL)
		return  0;

	return UART2->uart_write(buf, cnt);
}

uint32_t HC_05_read(uint8_t *buf, uint32_t cnt)
{
	if (UART2 == NULL)
		return 0;

	uint32_t rd_avail;

	UART2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
	cnt = (cnt > rd_avail)?rd_avail:cnt;
	{
		if (cnt > 0)
			return UART2->uart_read(buf, cnt);

	}
	return cnt;
}


uint8_t get_ges_cmd(void)
{
	uint8_t  RECbuf;
	uint32_t data_avail = 0;

	UART2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&data_avail));
		if (data_avail > 0) {
			HC_05_read(&RECbuf, 1);
	//		UART2_write(&RECbuf, 1);
			return RECbuf;
		}
}

void sendToHC05(char* fmt ,...)
{
	uint32_t err;
	unsigned char i,num;
	char lcd_buf[200];
	va_list ap;
    va_start(ap, fmt); 
	num=vsprintf(lcd_buf,fmt,ap);
	// printf("num=%d\r\n",num);
	for(i=0;i<num;i++){
		err = HC_05_write(&lcd_buf[i],1);
		// printf("char=%c err = %d\n",lcd_buf[i],err);
	}
	va_end(ap);
}