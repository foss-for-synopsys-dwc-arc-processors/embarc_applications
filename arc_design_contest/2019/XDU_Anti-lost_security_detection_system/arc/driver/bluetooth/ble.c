/* ------------------------------------------
 * \version 2016-01
 * \date 2019-05-20
 * \author Jinyu Yang
 * \function蓝牙定位模块
--------------------------------------------- */
#include <embARC.h>
#include "embARC_debug.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "board.h"
#include "dev_uart.h"
#include <string.h>
#include "dev_iic.h"
#define I2C_SLAVE_ADDR2  0x02
DEV_IIC *dev_iicble=NULL;

//DEV_UART *dev_uart2 = NULL;
static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}
int ble_comeback(char re_buf[15])
{
	int ble_state;
 if (re_buf[0]=='<')
	 if(re_buf[1]=='C')
	 ble_state=2;
    else 
		ble_state=1;
	return ble_state;
}

	int ble(void)
	{
		char rcv_buf[15];
		uint32_t rcv_cnt;
		uint32_t rd_avail;
		uint32_t baudrate =19200;//bt02
		int8_t *cmd={"<STATE>"};
	    int8_t *cmdbk={"<DISCONNECTED>"};
		int8_t state;
		unsigned int i;
		
		
        dev_iicble = iic_get_dev(DW_IIC_1_ID	);
        dev_iicble->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	    dev_iicble->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR2));
        delay_ms(200);
		// blue init
		for(i=0;i<2;i++)
		{
		dev_iicble->iic_write(cmd,strlen(cmd));
		dev_iicble->iic_read(rcv_buf, 15);
		
	    state=ble_comeback(rcv_buf);
		
		EMBARC_PRINTF("data is %s\r\n",rcv_buf);
		EMBARC_PRINTF("state is %d\r\n",state);
	    //EMBARC_PRINTF("i is %d\r\n",i);  
		

		delay_ms(10);
	    }
		
		return state;
    }
		 

