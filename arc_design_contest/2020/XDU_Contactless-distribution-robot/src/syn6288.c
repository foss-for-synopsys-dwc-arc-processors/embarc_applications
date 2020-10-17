#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "dev_uart.h"
#include "string.h"
#include "arc.h"
#include "syn6288.h"

DEV_UART *dev_uart2 = NULL;
uint32_t rd_avail2 = 0;

void SYN6288_info(char *infoData)
{
	char FrameInfo[500];
	unsigned char infoLength;
	char ecc = 0;
	unsigned int i = 0;
	uint32_t writecode;
	infoLength = strlen((char*)infoData); 
	FrameInfo[0] = 0xFD; 
	FrameInfo[1] = 0x00; 
	FrameInfo[2] = infoLength + 3;
	FrameInfo[3] = 0x01;
	FrameInfo[4] = 0x01; // GBK

	for(i = 0; i < 5; i++)
	{
		ecc = ecc ^ (FrameInfo[i]);
	}
	for(i = 0; i < infoLength; i++)
	{
		ecc = ecc ^ (infoData[i]);
	}
	memcpy(&FrameInfo[5], infoData, infoLength);
	FrameInfo[5 + infoLength] = ecc;
	writecode = dev_uart2->uart_write(FrameInfo, 6 + infoLength);
	EMBARC_PRINTF("SYN wirte code: 0x%x\r\n", writecode);
}

void SYN6288_init(void) 
{        
    dev_uart2 = uart_get_dev(2);
	dev_uart2->uart_open(UART_BAUDRATE_9600);
	EMBARC_PRINTF("SYN6288 init complate.\r\n");	
}
