/*
 * timer.h
 *
 *  Created on: 2018Äê5ÔÂ9ÈÕ
 *      Author: Administrator
 */
#include "stdint.h"
#include "task_config.h"
#ifndef HARDWARE_TIMER_TIMER_H_
#define HARDWARE_TIMER_TIMER_H_

extern uint16_t compare_pwm;

void ALL_TIM_init(void);

void TIM3_Int_Init(uint16_t arr,uint16_t psc);
void TIM2_Int_Init(uint16_t arr,uint16_t psc);
void TIM7_Int_Init(void);

//extern uint32_t RTC_SEC;
extern volatile unsigned long long FreeRTOSRunTimeTicks;
void ConfigureTimeForRunTimeStats(void);
void wifi_tim7(void);
void timer0_isr(TimerHandle_t xTimer);
void set_rtc_sec(uint32_t rtc_time);
void delay_us(uint32_t us);
uint32_t get_rtc_sec(void);
void pwm_gpio_int(void);
//extern void timertest_int(void);
//void timertest_isr(void);
#endif /* HARDWARE_TIMER_TIMER_H_ */
