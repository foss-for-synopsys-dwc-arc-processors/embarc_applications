#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_


 extern void delay(uint32_t z);
 extern void hc_sr04_init(void);
 extern float hcsr_m(void);                   //m
 extern float hcsr_l(void);     //l
 extern float hcsr_r(void);
#endif /* _ULTRASONIC_H_ */
