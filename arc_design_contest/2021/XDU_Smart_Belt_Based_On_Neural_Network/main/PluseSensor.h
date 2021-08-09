#ifndef _PLUSESENSOR_
#define _PLUSESENSOR_

#include "embARC.h"
#include "embARC_error.h"
#include "embARC_debug.h"
#include "smic_adc.h"

// #include "smic_adc.h"
#include "stdio.h"
#include "delay.h"

void heartRate_init();
int32_t heartRate_read();
int32_t getBPM();
int read_BPM_pre();

#endif /* _AD7991_H_ */