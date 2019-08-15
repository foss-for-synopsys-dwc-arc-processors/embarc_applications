/**
*Copyright: Copyright (c) 2019
*Created on 2019-8-12
*Author: YuaniLee
*/
#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "dev_gpio.h"

#define L298N_L_MASK    0x40000000 //Pmod6_pin9<--->L298N_LEFT_EN
#define L298N_R_MASK    0x80000000 //Pmod6_pin10<--->L298N_RIGHT_EN
#define L298N_F_MASK    0x30000000 //Pmod6_pin7,pin8<--->L298N_FORWORD_EN
#define L298N_H_MASK    0xC0000000 //Pmod6_pin3,pin4<--->L298N_BACK_EN

static DEV_GPIO_PTR PMOD_L;
static DEV_GPIO_PTR PMOD_R;
static DEV_GPIO_PTR PMOD_F;
static DEV_GPIO_PTR PMOD_H;

static uint32_t PWM_L = 0;
static uint32_t PWM_R = 0;

static uint32_t speed = 4;

extern void L298N_init(void);
extern void car_set(uint32_t dire,uint32_t pwm_left, uint32_t pwm_right);