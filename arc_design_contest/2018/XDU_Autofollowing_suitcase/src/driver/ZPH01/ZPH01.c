#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "arc.h"
#include "ZPH01.h"
DEV_UART *dev_uart2 = NULL;

void uart2_init(void)
{
	uint32_t baudrate1 = 9600;	
	dev_uart2 = uart_get_dev(DW_UART_2_ID);
	dev_uart2->uart_open(baudrate1);
}

uint32_t receive_pm(void)
{  
	uint8_t rcv_buf[9];
    uint8_t PPM[4];

    uint16_t p = 0;
	uint16_t p1 = 0;
	float mconcentration;
	uint32_t rcv_cnt;
	uint32_t i = 0;
	uint32_t baudrate1 = 9600;
	uint32_t rd_avail = 0;
     uint8_t*mm;
	dev_uart2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
		if(rd_avail > 0){
			rcv_cnt = dev_uart2->uart_read(rcv_buf, rd_avail);
			if((rcv_buf[0]==0xff)&&(rcv_buf[1]==0x18))
					{
					    mconcentration=((float)(rcv_buf[3])+((float)(rcv_buf[4]))/100.0)*20;
					    p=(uint16_t)mconcentration;
					    p1=p;
						PPM[0] =p / 1000 + '0';		
						p= p % 1000;
						PPM[1] = p / 100 + '0';
						p = p % 100;
						PPM[2] = p/ 10 + '0';
						PPM[3] = p % 10 + '0';
                        mm=PPM;
					//	printf("ppm=%s\r\n", mm);				
					}
	         }
	return p1;
}

