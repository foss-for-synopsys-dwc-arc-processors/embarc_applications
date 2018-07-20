#ifndef _TIMER_H_
#define _TIMER_H_

extern SemaphoreHandle_t SemMotTick;
static void base_timer_isr(void *ptr);
void base_timer_init(void);

#endif