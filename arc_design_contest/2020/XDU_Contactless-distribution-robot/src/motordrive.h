#ifndef __MOTORDRIVE_H
#define __MOTORDRIVE_H 

#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "dev_gpio.h"

//PMOD6 PORT
#define MOTOR_1_MASK 0xF0000000
#define MOTOR_2_MASK 0xF0000000

static DEV_GPIO_PTR PMOD_1;
static DEV_GPIO_PTR PMOD_2;

void Motor_Init(void);
void Motor_Forward(void);
void Motor_Back(void);
void Motor_Stop(void);
void Motor_TurnRight(void);
void Motor_TurnLeft(void);
void Avoid(void);
void car_control(uint8_t x);
#endif
