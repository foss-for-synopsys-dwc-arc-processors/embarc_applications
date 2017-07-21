#ifndef _FFT_H_
#define _FFT_H_

#define HRATE_SAMPLING_FREQ   (50) /* heartrate data sampling frequency: 50Hz */
#define HRATE_DATA_SIZE_POWER (8)  /* determines the size of heartrate data group: 2^HRATE_DATA_SIZE_POWER */
#define HRATE_DATA_SIZE       (1 << HRATE_DATA_SIZE_POWER)   /* amount of input data */
#define FFT_DELTA             (1 / (HRATE_DATA_SIZE * 0.02)) /* delta value of heartrate data after fft */

/* complex number */
typedef struct
{
	int real; /* real part */
	int img;  /* imaginary part */
} complex_int;

/* rotation factor */
complex_int rota_fac[HRATE_DATA_SIZE];

/* the rotation factor initialize function */
extern void rotation_factor_init(void);

/* reverse each bit of index number, arrange index number of input data */
extern void reverse(complex_int *data);

/* fast fourier transformation function(fft) */
extern void fft(complex_int *data);

/* find the frequency corresponding to maximum amplitude in the data group after fft */
extern float find_freq_max(complex_int *data);

/** @} end of name */

#endif/* _WEARABLE_NODE_FFT_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_FFT */

