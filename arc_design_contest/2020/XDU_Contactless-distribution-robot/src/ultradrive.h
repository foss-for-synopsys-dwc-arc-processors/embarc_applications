#ifndef __ULTRADRIVE_H
#define __ULTRADRIVE_H	 

#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "dev_gpio.h"

//PMOD5 PORT
#define ULTRA_1_MASK 0x3000000
#define ULTRA_2_MASK 0x2000000//pin1 trig //pin2 echo
#define ULTRA_3_MASK 0x1000000

//PORT A
static DEV_GPIO_PTR PMOD_O;
void UltrasonicWave_Init(void);
void UltrasonicWave_StartMeasure(void);
float UltrasonicWave_Measure(void);		 				    
#endif
