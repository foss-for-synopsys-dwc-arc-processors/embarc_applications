#ifndef _MIC_H_
#define _MIC_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "fft.h"

/** adc value get of mic */
extern uint8_t adc_get(void);

/** Initial the mic */
extern void mic_init(void);

/** IIC address point to mic */
extern void iic_point_mic(void);

#endif