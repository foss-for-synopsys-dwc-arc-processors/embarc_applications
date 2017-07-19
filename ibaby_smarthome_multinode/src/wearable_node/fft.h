#ifndef _FFT_H_
#define _FFT_H_

#define HRATE_DATA_SIZE_POWER (7)
#define HRATE_DATA_SIZE       (1 << HRATE_DATA_SIZE_POWER) /* amount of input data */

/* complex number */
typedef struct
{
	double real;
	double img;
} complex_num;

/* rotation factor */
complex_num rota_fac[HRATE_DATA_SIZE];

extern void rotation_factor_init(void);
extern void reverse(complex_num *data);
extern void fft(complex_num *data);

/** @} end of name */

#endif/* _WEARABLE_NODE_FFT_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_FFT */

