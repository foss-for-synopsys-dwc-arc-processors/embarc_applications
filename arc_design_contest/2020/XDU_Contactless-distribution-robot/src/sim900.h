#ifndef _SIM900_H_
#define _SIM900_H_

#define DW_UART_ID DW_UART_0_ID

void sim900_init(void);
void call(uint8_t *pho);
uint8_t *send(uint8_t *phoneNum);
#endif 