#ifndef _FFT_H_
#define _FFT_H_


#define DATA_SIZE  (64) /* amount of input data */
#define STAGE_SIZE (log(DATA_SIZE)/log(2)) /* stage size of fft */
#define BIT_SIZE   (STAGE_SIZE) /* bit size of index number */
#define PI         (3.1415926)

/* complex number */
typedef struct
{
	double real;
	double img;
} complex_num;

/* data group */
complex_num data[DATA_SIZE];

/* rotation factor */
complex_num rota_fac[DATA_SIZE];

extern void rotation_factor_init(void);
extern void reverse(void);
extern void fft(void);
extern void pr_data_fft(void);

/** @} end of name */

#endif/* _WEARABLE_NODE_FFT_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_FFT */

