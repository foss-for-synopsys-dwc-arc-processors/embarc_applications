#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include <string.h>
#include "arc.h"
#include "board.h"
#include "keypad.h"
#include "sim900.h"

static DEV_UART *sim900;

void sim900_init(void)
	
{
	sim900 = uart_get_dev(DW_UART_ID);
	sim900->uart_open(UART_BAUDRATE_9600);
error_exit:
	return;
}

void call(uint8_t *pho)//input unicode telephone number
{			
	uint8_t *phoner;

	uint8_t psnum[64];
	phoner=(uint8_t *)malloc(12); 
	for(uint8_t i=0;i<11;i++)
   	{
        phoner[i]=pho[4*i+3];
	}
	phoner[11]=0;
EMBARC_PRINTF("keypad:%s\r\n", phoner);
	snprintf(psnum, 64, "ATD%s;\r\n",phoner);
	sim900->uart_write(psnum, strlen(psnum));
	board_delay_ms(100,1);
	
	uint8_t end[]={0x1A,0x00}; 
    sim900->uart_write(end, strlen(end));
printf("end_call\r\n");
}


uint8_t *send(uint8_t *phoneNum)
{	
	uint8_t i = 0;
	uint8_t *passwd;
	uint8_t pnum[64];
	uint8_t msg[128];
	passwd = passwordstring();
	//cmd output
	int8_t cmd1[]={"AT+CMGF=1\r\n"};
	int8_t cmd2[]={"AT+CSCS=\"UCS2\"\r\n"};
	int8_t cmd3[]={"AT+CSCA?\r\n"};
	int8_t cmd4[]={"AT+CSMP=17,167,0,25\r\n"};

	snprintf(pnum, 64, "AT+CMGS=\"%s\"\r\n",phoneNum);
	snprintf(msg, 128, "002060A8597DFF016B228FCE4F7F7528667A80FD673A56684EBAFF0C60A8768453D68D2778014E3AFF1A%s",passwd);
	    sim900->uart_write(cmd1, strlen(cmd1));
		board_delay_ms(300,1);
	    sim900->uart_write(cmd2, strlen(cmd2));
		board_delay_ms(300,1);
		sim900->uart_write(cmd3, strlen(cmd3));
		board_delay_ms(300,1);
	    sim900->uart_write(cmd4, strlen(cmd4));
		board_delay_ms(300,1);
		
        sim900->uart_write(pnum, strlen(pnum));
	    board_delay_ms(500,1);
        sim900->uart_write(msg, strlen(msg)); 
	    board_delay_ms(300,1);
	    uint8_t end[]={0x1A,0x00};
        sim900->uart_write(end, strlen(end)); 
		
	printf("end message\r\n");
	return (uint8_t *)passwd;
}
