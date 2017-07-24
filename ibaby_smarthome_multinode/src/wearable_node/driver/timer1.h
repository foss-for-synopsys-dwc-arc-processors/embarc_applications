#ifndef TIMER1_H
#define TIMER1_H

/**
 * \name	macro of timer1
 * @{
 */
//#define T1_COUNT_UNIT_MS  (20000) /*!< counting accuracy of Timer1: 1ms */
#define T1_COUNT_UNIT_LMS (2000)  /*!< counting accuracy of Timer1: 0.1ms */

extern void timer1_start(void);
extern uint32_t timer1_stop(void);
/** @} end of name */

#endif