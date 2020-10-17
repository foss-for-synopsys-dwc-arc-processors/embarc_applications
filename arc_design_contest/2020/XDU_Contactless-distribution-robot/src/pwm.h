#ifndef __PWM_H
#define __PWM_H

#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "dev_gpio.h"

#define PCA9685_IIC_ID	DW_IIC_0_ID
#define PCA9685_adrr 0x40
#define work_Freq 50
#define PCA9685_SUBADR1 0x02
#define PCA9685_SUBADR2 0x03
#define PCA9685_SUBADR3 0x04
#define PCA9685_MODE1 0x00
#define PCA9685_PRESCALE 0xFE
#define LED0_ON_L 0x06
#define LED0_ON_H 0x07
#define LED0_OFF_L 0x08
#define LED0_OFF_H 0x09
#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD

void PCA9685_init(void);
bool PCA9685_is_good(void);
bool PCA9685_is_ready(void);
uint8_t PCA9685_read(uint8_t reg);
void PCA9685_write(uint8_t reg, uint8_t regval);
void PCA9685_reset(void);
void begin(void);
void pwm_init(void);
void delay_10us(volatile int z);
void setPWMFreq(uint8_t freq);
void setPWM(uint8_t num, uint16_t on, uint16_t off);
uint16_t calculate_PWM(uint8_t angle);
void PWM1_0(void);
void PWM2_0(void);
void PWM1_90(void);
void PWM2_120(void);
void PWM3_90(void);
void PWM3_45(void);
void PWM3_135(void);
#endif
