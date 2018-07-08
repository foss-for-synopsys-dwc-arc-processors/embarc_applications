//PMOD5下行端口，接 AT-wifi指令模块，建立AP，模式为2
#include "embARC.h"
#include "embARC_debug.h"
#include <string.h>
#include <stdlib.h>

#include "dev_uart.h"    //UART API 头文件`
#include "../../../board/emsk/drivers/mux/mux.h"  // Pmod端口配置API`
#include "../../../board/emsk/drivers/mux/mux_hal.h"


#define BUTTON_MASK	0x7
#define MAX_READ_CNT 32
void setupAP2(void)
{
	DEV_UART_PTR  uart2;
	set_uart_map(0xE4);
	set_pmod_mux(PM1_UR_UART_0 | PM1_LR_GPIO_A| PM2_I2C_HRI| PM3_GPIO_AC| PM4_I2C_GPIO_D| PM5_UR_GPIO_C | PM5_LR_UART_2 | PM6_UR_SPI_M0 | PM6_LR_GPIO_A );
	int32_t ercd;
	uart2=uart_get_dev(DW_UART_2_ID);
	ercd=uart2->uart_open(115200);
	char send_string1[64]= "AT+CWMODE=2\r\n";
	ercd =uart2->uart_write(send_string1, sizeof(send_string1));
	board_delay_ms(3000,0);
	char send_string2[64]= "AT+CWSAP=\"S2\",\"123456789\",5,4\r\n";
	ercd =uart2->uart_write(send_string2, sizeof(send_string2));
	
	char receive_string[32]={0};
	uint32_t rd_avail=0;
	uint32_t rssi;
	while(1)
	{
		board_delay_ms(1000,0);
		uart2->uart_control(UART_CMD_GET_RXAVAIL,(void *)(&rd_avail));
		if(rd_avail>0)
		{
			EMBARC_PRINTF("rd_avail is %d\r\n",rd_avail);
			
			uart2->uart_read(receive_string,rd_avail);
			EMBARC_PRINTF("%s\r\n",receive_string);	
			break;
		}
		
	}	
}