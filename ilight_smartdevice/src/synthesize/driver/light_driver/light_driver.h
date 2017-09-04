/**
	*Some lamp control functions.
	*writer: Mr.WangS
	*Date:2017-3-20
*/
#ifndef _LIGHT_DRIVER_H_
#define _LIGHT_DRIVER_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "dev_gpio.h"
#include "dw_gpio.h"
#include "arc_builtin.h"
#include "task.h"

#define GPIO_OUT_PORT_MIDDLE (DW_GPIO_PORT_A)
#define GPIO_OUT_OFFSET_MIDDLE (11)
#define GPIO_OUT_MASK_MIDDLE (1<<GPIO_OUT_OFFSET_MIDDLE)
#define GPIO_H_CFG_MIDDLE (1<<GPIO_OUT_OFFSET_MIDDLE)

#define GPIO_OUT_PORT_SIDE (DW_GPIO_PORT_C)
#define GPIO_OUT_OFFSET_SIDE (19)
#define GPIO_OUT_MASK_SIDE (1<<GPIO_OUT_OFFSET_SIDE)
#define GPIO_H_CFG_SIDE (1<<GPIO_OUT_OFFSET_SIDE)

#define LIGHT_ROW_MIDDLE 0x010
#define LIGHT_ROW_SIDE   0x101

#define LIGHT_NUM_MIDDLE 16
#define LIGHT_NUM_SIDE 34

/*
  *Llight color data structure
  *r,g,b reresents the RGB color data
*/
typedef union {
	uint32_t color;
	struct rgb_s {
		uint8_t b;
		uint8_t r;
		uint8_t g;
		uint8_t reserved;
	} rgb;
} RGB_T;

static uint32_t low_val_middle=0,low_val_side=0;
static uint32_t temp_reg_middle,temp_reg_side;

volatile uint32_t delay_num; 
extern uint8_t color_row;
extern uint8_t light_num;
/**The underlying lighting control module function*/

//extern void light_mode_test(void);

void led_ctr_gpio_init(void);
void light_ctr_reset(void);
uint64_t light_ctr_mask_lshift(uint64_t aim_num,uint64_t shift_num);
extern void delay_10us(uint32_t mul);
void light_ctr_rgb(uint64_t mask,uint16_t row,uint8_t red,uint8_t green,uint8_t blue);
void light_send_colordata(uint32_t color_data,uint16_t row);
void light_ctr_rgb_update(uint64_t mask,uint16_t row,uint8_t red,uint8_t green,uint8_t blue);

#endif
