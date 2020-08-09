#include "cc2541.h"
#include "board.h"

static cc2541_def CC2541;

cc2541_def* cc2541_init(uint32_t uartId, uint32_t uartBaudRate, uint32_t txBuffSize, uint32_t rxBuffSize, int32_t gpio_id, uint32_t gpio_offset){
  io_mux_init();
  io_arduino_config(ARDUINO_PIN_0, ARDUINO_UART, IO_PINMUX_ENABLE);
  cc2541_def* cc2541;
  cc2541 = &CC2541;
  cc2541->uart = ez_sio_open(uartId, uartBaudRate, txBuffSize, rxBuffSize);
  cc2541->rxBuff = malloc(sizeof(char)*rxBuffSize);

  cc2541->rstPort = gpio_get_dev(gpio_id);
  cc2541->rstPin = (1<<gpio_offset);
  cc2541->rstPort->gpio_open(cc2541->rstPin);
  cc2541->rstPort->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void*)cc2541->rstPin);
  cc2541->rstPort->gpio_write(0, cc2541->rstPin);
  board_delay_ms(100, 1);
  cc2541->rstPort->gpio_write(cc2541->rstPin, cc2541->rstPin);
  board_delay_ms(1000, 1);
  ez_sio_write(cc2541->uart, "AT\r\n", 4);
  if(cc2541_read(cc2541) == 0)
    return cc2541;
  else return NULL;
}

int cc2541_get_connection_role(cc2541_def* cc2541){
  ez_sio_write(cc2541->uart, "AT+ROLE\r\n", 10);
  cc2541_read(cc2541);
  int role;
  if(cc2541->rxBuff[0] == NULL)
    return -1;
  else {
    sscanf(cc2541->rxBuff, "+ROLE=%d", &role);
    return role;
  }
}

int cc2541_set_connection_role(cc2541_def* cc2541, int role){
  if(role == 0){
    ez_sio_write(cc2541->uart, "AT+ROLE0\r\n", 11);
    cc2541_read(cc2541);
    if(cc2541->rxBuff[0] == NULL){
      return -2;
    }
    if(strstr(cc2541->rxBuff, "ROLE0")){
      return 0;
    }
    return -1;
  }
  else if(role == 1){
    ez_sio_write(cc2541->uart, "AT+ROLE1\r\n", 11);
    cc2541_read(cc2541);
    if(cc2541->rxBuff[0] == NULL){
      return -2;
    }
    if(strstr(cc2541->rxBuff, "ROLE1")){
      return 0;
    }
    return 1;
  }
}

uint32_t cc2541_test_AT(cc2541_def* cc2541){
  ez_sio_write(cc2541->uart, "AT\r\n", 5);
  return cc2541_read(cc2541);
}

void cc2541_inq_dev(cc2541_def* cc2541){
  cc2541->uart = ez_sio_open(DFSS_UART_2_ID, UART_BAUDRATE_9600, 200, 200);
  EMBARC_PRINTF("%d", ez_sio_write(cc2541->uart, "AT+INQ\r\n", 9));
  memset(cc2541->rxBuff, 0, 200);
  board_delay_ms(5000, 1);
  ez_sio_read(cc2541->uart, cc2541->rxBuff, 200);
}

uint32_t cc2541_read(cc2541_def* cc2541){
  uint32_t cur_ofs = 0;
	uint32_t read_cnt;
	uint32_t cur_time = OSP_GET_CUR_MS();
  memset(cc2541->rxBuff, 0, 200);
  board_delay_ms(200, 1);
  ez_sio_read(cc2541->uart, cc2541->rxBuff, 200);
	/*do {
		read_cnt = ez_sio_read(cc2541->uart, &cc2541->rxBuff[cur_ofs], 1);
    if(cc2541->rxBuff[cur_ofs] == '\n')
      break;
		cur_ofs += read_cnt;
	} while ((OSP_GET_CUR_MS()-cur_time) < CC2541_TIMEOUT);
  if(cur_ofs > 0){
    cc2541->rxBuff[cur_ofs - 1] = '\0';
    
  }*/
  
  if (cc2541->rxBuff[0] == 0){
    EMBARC_PRINTF("CC2541: EMPTY STRING\n");
    return -2;
  }
	else if (strstr(cc2541->rxBuff, "OK")!= NULL){
    EMBARC_PRINTF("CC2541: \"%s\" (%d)\r\n", cc2541->rxBuff, strlen(cc2541->rxBuff));
    return 0;
  }
	else return -1;
}

int cc2541_disconnect(cc2541_def* cc2541){
  cc2541->rstPort->gpio_write(0, cc2541->rstPin);
  board_delay_ms(100, 1);
  cc2541->rstPort->gpio_write(cc2541->rstPin, cc2541->rstPin);
  board_delay_ms(1000, 1);
  ez_sio_write(cc2541->uart, "AT\r\n", 4);
  return cc2541_read(cc2541);
}