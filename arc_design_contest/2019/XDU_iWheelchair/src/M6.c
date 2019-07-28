#include "M6.h"
#include "drive_control.h"

/*Voice control init*/
/*Pmod5_pin8<-->M6_RX,Pmod5_pin9<-->M6_TX*/
void M6_init(void)
{
	M6 = uart_get_dev(DW_UART_2_ID);	
	M6->uart_open(UART_BAUDRATE_9600);
	M6->uart_control(UART_CMD_SET_BAUD,(void*)(UART_BAUDRATE_9600));
}

void voice_control(uint8_t M6_rcv_buf[],uint32_t M6_rcv_cnt)
{		
	switch(M6_rcv_buf[M6_rcv_cnt-1])
		{

			case 1:
				car_set(0,PWM_L,PWM_R);//stop
				EMBARC_PRINTF("ble command is %s，停下\r\n",M6_rcv_buf);
				break;
			case 2:
				car_set(1,PWM_L,PWM_R);//forward   
				EMBARC_PRINTF("ble command is %s，向前\r\n",M6_rcv_buf);
				break;
			case 3:		
				car_set(2,PWM_L,PWM_R);//left
				EMBARC_PRINTF("ble command is %s，向左\r\n",M6_rcv_buf);
				break;
			case 4:		
				car_set(3,PWM_L,PWM_R);//right
				EMBARC_PRINTF("ble command is %s，向右\r\n",M6_rcv_buf);
				break;
			case 5:
				car_set(6,PWM_L,PWM_R);//backward  
				EMBARC_PRINTF("ble command is %s，向前\r\n",M6_rcv_buf);
				break;
			default:
				EMBARC_PRINTF("ble command error\r\n");
				break;
		}
		
}