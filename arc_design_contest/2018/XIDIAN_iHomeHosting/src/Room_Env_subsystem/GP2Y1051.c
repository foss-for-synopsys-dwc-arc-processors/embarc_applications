/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"
#include <stdio.h>
#include "board.h"
#include "dev_gpio.h"    //GPIO API 头文件
#include "dev_uart.h"    //UART API 头文件
#include "dev_iic.h"     //I2C  API 头文件
#include "dev_spi.h"     //SPI  API 头文件

#include "GP2Y1051.h"


/**********GP2Y1051串口初始化函数***************/
static void GP2Y1051_init(void)
{
	GP2Y1051_uart = uart_get_dev(GP2Y1051_UART_ID);
	GP2Y1051_uart-> uart_open(GP2Y1051_UART_BAUDRATE);	
	GP2Y1051_uart-> uart_control(UART_CMD_SET_BAUD, (void *)(GP2Y1051_UART_BAUDRATE));
			
}
static void delay_us( uint32_t us) 
{
	uint32_t volatile x,y;
	for(x=2;x>0;x--)
		for(y=us;y>0;y--);
	
}
static uint16_t GP2Y1051_rd_once(void)
{
	GP2Y1051_init();
	if (GP2Y1051_uart == NULL) return 0;

	uint32_t rd_avail;
	uint8_t rd_data[20];
	uint8_t rd_cnt = 20;
	uint32_t buf=0;	
	uint8_t buf_cnt=0;
	uint8_t count = 0;
	for(;(buf_cnt<2) | (count < 10); count++)
	{
		board_delay_ms(110,0);
		GP2Y1051_uart->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
		rd_cnt = ( 20 > rd_avail)?rd_avail:rd_cnt;
		if (rd_cnt > 0) {
			GP2Y1051_uart->uart_read(rd_data, rd_cnt);
			for(uint8_t i=0;i<rd_cnt;i++) 
				{
					if ( rd_data[i] == 0xAA && rd_data[i+6] == 0xFF && 
						(rd_data[i+1] + rd_data[i+2] + rd_data[i+3] + rd_data[i+4] == rd_data[i+5]))
					{
						//printf("%2x %2x %2x %2x %2x %2x %2x\r\n",rd_data[i],rd_data[i+1],rd_data[i+2],rd_data[i+3],rd_data[i+4],rd_data[i+5],rd_data[i+6]);
						buf += (rd_data[i+1]*256+rd_data[i+2]) ;
						buf_cnt++;
						break;
					}
					
				}
		}
	}
	return (uint16_t)(buf * 0.6958); //buf * 5.0 / 1024.0 * 285.0 / 2.0
	//static 
}

uint16_t GP2Y1051_read(void)
{
	static uint8_t pm25_val_buf[10]={0};
	static uint8_t i;
	uint8_t count=0;
	uint16_t sum=0;
	pm25_val_buf[i++] = GP2Y1051_rd_once();
	if(10 == i) i=0;
	for (count = 0; count < 10; count++)
	{
		sum += pm25_val_buf[count]; 
		//printf("%d  ", pm25_val_buf[count]);
	}		
		
	//printf("\r\nThe Concentration of PM2.5 in the air is %d ug/m3.\r\n",(int)(sum/10.0) );
	return (uint16_t)(sum/10.0);
}

