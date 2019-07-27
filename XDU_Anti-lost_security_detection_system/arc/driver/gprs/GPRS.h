#ifndef _GPRS_H_
#define _GPRS_H_

static void delay_ms(volatile int z);
extern void call(void);
extern void send(uint8_t *phoneNum, uint8_t *msg);

#endif /* _GPRS_H_ */