/*
 * fifo.h
 *
 *  Created on: 2018Äê5ÔÂ17ÈÕ
 *      Author: Administrator
 */

#ifndef FIFO_FIFO_H_
#define FIFO_FIFO_H_

#include "stdio.h"

#include "app_config.h"

void init_fifo(fifo_struct*fifo2init,uint16_t totallen,uint8_t*fifo_data);
uint8_t write_fifo(fifo_struct*fifo2write,uint8_t*byte,uint16_t len);
uint8_t read_fifo(fifo_struct*fifo2read,uint8_t*byte,uint16_t len);
void reset_fifo(fifo_struct*fifo2clear);


#endif /* FIFO_FIFO_H_ */
