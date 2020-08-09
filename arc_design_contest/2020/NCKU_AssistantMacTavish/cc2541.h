#ifndef _CC2541_H
#define _CC2541_H

#include "stdio.h"
#include "stdlib.h"
#include "embARC_debug.h"
#include "embARC.h"
#include "board.h"
#include "string.h"
#include "ez_sio.h"

typedef struct{
  EZ_SIO* uart;
  char* rxBuff;
  DEV_GPIO* rstPort;
  uint32_t rstPin;
} cc2541_def;

#define CC2541_TIMEOUT  10000

cc2541_def* cc2541_init(uint32_t uartId, uint32_t uartBaudRate, uint32_t txBuffSize, uint32_t rxBuffSize, int32_t gpio_id, uint32_t gpio_offset);
uint32_t cc2541_read(cc2541_def* cc2541);
int cc2541_get_connection_role(cc2541_def* cc2541);
int cc2541_set_connection_role(cc2541_def* cc2541, int role);
uint32_t cc2541_test_AT(cc2541_def* cc2541);
void cc2541_inq_dev(cc2541_def* cc2541);
int cc2541_disconnect(cc2541_def* cc2541);
#endif