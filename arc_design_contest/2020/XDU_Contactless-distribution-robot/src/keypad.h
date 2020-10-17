#ifndef __KEYPAD_H
#define __KEYPAD_H 

#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "dev_gpio.h"

//PMOD3 PORT
#define PA_MASK 0xF0000
#define PC_MASK 0xF0000

#define F1 10
#define Delete 11
#define Esc 12
#define Enter 13

//PORT A
static DEV_GPIO_PTR PMOD_1;

//PORT C
static DEV_GPIO_PTR PMOD_2;

void keypad_init(void);
bool key_down(void);
uint8_t key_perpare(void);
uint8_t keypad_read (void);
uint8_t *VerifyString(uint16_t len);
uint8_t *passwordstring(void);
#endif
