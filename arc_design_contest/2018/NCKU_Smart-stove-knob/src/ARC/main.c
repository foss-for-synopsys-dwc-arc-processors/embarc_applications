#include "embARC.h"
#include "embARC_debug.h"

#include "board.h"
#include "dev_uart.h"

//uint32_t baudrate_list[] = {4800, 9600, 19200, 38400, 57600, 115200};

int main(void)
{
	uint8_t state;
  uint8_t motor;
	uint32_t rcv_cnt;
	uint32_t baudrate = UART_BAUDRATE_9600;
  uint8_t stove_on_cnt = 0;
  int t0 = 0;

	/** change the init baudrate according to your ble module settings */
	EMBARC_PRINTF("Init HM1X with baudrate %dbps\r\n", baudrate);
	hm1x_init(baudrate);

	while (1) {
		rcv_cnt = hm1x_read(&state, sizeof(state));
		if (rcv_cnt) {
      if (state == 'h') { //stove is on
        if (stove_on_cnt < 1) {
          EMBARC_PRINTF("STOVE ON!\n");
          stove_on_cnt++;
        }
        t0 = 0;
        EMBARC_PRINTF("Cook is in the kitchen\n");
      } else if (state == 's') {
          EMBARC_PRINTF("It's smoking\n");        
      } else if (state == 'n') {//no human
          t0++;
          EMBARC_PRINTF("Cook has leaved for %d seconds\n", t0);          
      }
      if((state == 's') || (t0 >= 10)){
        EMBARC_PRINTF("Turn off the stove\n");
        motor = 'r';
        hm1x_write(&motor, sizeof(motor));
        break;
      }
    }
	} 

	return E_SYS;	/* system error */
}

/** @} */
