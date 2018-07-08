#include "embARC.h"
#include "embARC_debug.h"

#include "board.h"
#include "dev_uart.h"

uint32_t baudrate_list[] = {4800, 9600, 19200, 38400, 57600, 115200};

int main(void)
{
  uint32_t val;
	uint8_t state;
  uint8_t motor;
	uint32_t rcv_cnt;
	uint32_t baudrate = UART_BAUDRATE_9600;
  uint8_t stove_on_cnt = 0;
  uint8_t timer_start_cnt = 0;

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
        EMBARC_PRINTF("Cook is in the kitchen\n", val);
      } else if (state == 'n') {//no human
          if (timer_present(TIMER_0)) {
            if (timer_start_cnt < 1) {
              timer_start(TIMER_0, TIMER_CTRL_IE, BOARD_CPU_CLOCK * 100);
              timer_start_cnt++;
            }
            timer_current(TIMER_0, &val);
            val = val >> 25;
            EMBARC_PRINTF("Cook has leaved for %d seconds\n", val);
          }
          if(val >= 10){
            EMBARC_PRINTF("Turn off the stove\n");
            motor = 'r';
            hm1x_write(&motor, sizeof(motor));
            break;
          }
      }
    }
	} 

	return E_SYS;	/* system error */
}

/** @} */
