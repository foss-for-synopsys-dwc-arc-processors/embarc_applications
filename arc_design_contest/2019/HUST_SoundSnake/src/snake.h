/**
 * \brief	the snake game and led matrix.
 */
#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "dev_gpio.h"

/* MAX7219 pins*/
#define CS_1	gpio_led1->gpio_write(1<<4, 1<<4)
#define CS_0	gpio_led1->gpio_write(0<<4, 1<<4)
#define DOUT_1	gpio_led1->gpio_write(1<<5, 1<<5)
#define DOUT_0	gpio_led1->gpio_write(0<<5, 1<<5)
#define CLK_1	gpio_led2->gpio_write(1<<4, 1<<4)
#define CLK_0	gpio_led2->gpio_write(0<<4, 1<<4)

typedef struct{
	int8_t x;
	int8_t y;
}snake_body;

void led_matrix_init(void);
void snake_init(void);
void snake_move(uint8_t seed);//input: seed of the position of the food.

#endif