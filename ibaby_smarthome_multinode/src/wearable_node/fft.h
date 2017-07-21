#ifndef _FFT_H_
#define _FFT_H_

#define HRATE_SAMPLING_FREQ   (50)
#define HRATE_DATA_SIZE_POWER (8)
#define HRATE_DATA_SIZE       (1 << HRATE_DATA_SIZE_POWER) /* amount of input data */
#define FFT_DELTA             (1 / (HRATE_DATA_SIZE * 0.02))

/* complex number */
typedef struct
{
	int real;
	int img;
} complex_int;

/* rotation factor */
complex_int rota_fac[HRATE_DATA_SIZE];

extern void rotation_factor_init(void);
extern void reverse(complex_int *data);
extern void fft(complex_int *data);
extern float find_freq_max(complex_int *data);

/** @} end of name */

#endif/* _WEARABLE_NODE_FFT_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_FFT */

