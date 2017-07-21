
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fft.h"

#define DATA_SAMPLING_FREQ (HRATE_SAMPLING_FREQ) /* data sampling frequency */
#define DATA_SIZE          (HRATE_DATA_SIZE)     /* data size of data group */

#define STAGE_SIZE         (log(DATA_SIZE)/log(2)) /* stage size of fft */
#define BIT_SIZE           (STAGE_SIZE) /* bit size of index number */
#define PI                 (3.1415926)

#define S16MAX_POWER       (15)         /* power of 16 bits */
#define S16MAX             (32767)      /* maximum value of 16 bits */

#define FREQ_INDEX_NUM     (4)          /* the number of frequency arresponding to the bigger amplitude value */
#define MIN_FREQ_POS       (int)(HRATE_DATA_SIZE * 0.05) /* starting frequency for finding */
#define MAX_FREQ_POS       (int)(HRATE_DATA_SIZE * 0.2)  /* ending frequency for finding */
#define FREQ_SIZE          (MAX_FREQ_POS - MIN_FREQ_POS) /* frequency bindwidth for finding */

static complex_int complex_add(complex_int a, complex_int b);
static complex_int complex_sub(complex_int a, complex_int b);
static complex_int complex_mul(complex_int a, complex_int b);
static void hanning_window(complex_int *data);


/* add function of complex number */
static complex_int complex_add(complex_int a, complex_int b)
{
	complex_int c;
	c.real = a.real + b.real;
	c.img  = a.img  + b.img;
	return c;
}

/* subtraction function of complex number */
static complex_int complex_sub(complex_int a, complex_int b)
{
	complex_int c;
	c.real = a.real - b.real;
	c.img  = a.img  - b.img;
	return c;
}

/* multiplication function of complex number */
static complex_int complex_mul(complex_int a, complex_int b)
{
	complex_int c;
	/* 
	 * shift to right S16MAX_POWER bits
	 * because rotation factor has been shift to left S16MAX_POWER bits in initializing function
	 */
	c.real = (a.real * b.real - a.img * b.img)  >> S16MAX_POWER;
	c.img  = (a.real * b.img  + a.img * b.real) >> S16MAX_POWER;
	return c;
}

/*
 * add hanning window function to data group
 * processing the data within window domain only once time
 */
static void hanning_window(complex_int *data)
{
    int i;
    float win_domain[DATA_SIZE];
    for (i = 0; i < DATA_SIZE; i++)
    {
        win_domain[i] = 0.5 * (1.0 - cos(2.0 * PI * i / (DATA_SIZE - 1)));
        data[i].real  = (int)(win_domain[i] * data[i].real);
    }
}

/* the rotation factor initialize function */
extern void rotation_factor_init(void)
{
	int i;

	/*
	 * in stage m, there are L rotation factors
	 *    rota_fact(k) = W(k, 2^m) = W(N*k/(2L), N)
	 */
	for (i = 0; i < DATA_SIZE; i++) {
		rota_fac[i].real = round(cos(2 * PI * i / DATA_SIZE) * S16MAX);
		rota_fac[i].img  = round(-1 * sin(2 * PI * i / DATA_SIZE) * S16MAX);
	}
}

/* reverse each bit of index number, arrange index number of input data */
extern void reverse(complex_int *data)
{
	unsigned int i, j, k;
	unsigned int t;
	complex_int data_temp;

	for (i = 0; i < DATA_SIZE; i++)
	{
		k = i; /* current index number : i */
		j = 0; /* index number reversed */
		for (t = 0; t < BIT_SIZE; t++) /* index number has BIT_SIZE bits */
		{
			j <<= 1;
			j |= (k & 1); /* j shift to the left 1 bit, then complex_add the last bit of k to it */
			k >>= 1;      /* k shift to the right 1 bit */
		}

		/*
		 * if the index number reversed is bigger than the origin, exchange their value storaged
		 * it can avoid repeated exchange if we compare their(index number reversed and the origin)
		 * size before exchanging
		 */
		if (j > i)
		{
			data_temp = data[i];
			data[i] = data[j];
			data[j] = data_temp;
		}
	}
}

/*
 * fast fourier transformation function(fft)
 * for N points fft computation, there has log2(N) stages
 *    in stage m, there has N/2/L groups, L = 2^(m-1), and L butterfly forms in each group
 */
extern void fft(complex_int *data)
{
	unsigned int i, j, k, l;
	complex_int top, bottom, common;

	// for (i = 0; i < DATA_SIZE; ++i)
	// {
	// 	printf("%lf\t%lf\n", rota_fac[i].real, rota_fac[i].img);
	// }

	/* add hanning window function to data group before fft */
	hanning_window(data);

	for (i = 0; i < STAGE_SIZE; i++) /* loop for stage */
	{
		/*
		 * distance between the two point in butterfly form
		 * it also means the number of butterfly form per group
		 */
		l = 1 << i;

		/* loop for group */
		for (j = 0; j < DATA_SIZE; j += 2 * l)
		{
			/* loop for butterfly form */
			for (k = 0; k < l; k++)
			{
				/* once butterfly computation */
				common = complex_mul(data[j + k + l], rota_fac[DATA_SIZE / (2 * l) * k]);
				top    = complex_add(data[j + k], common);
				bottom = complex_sub(data[j + k], common);

				data[j + k]     = top;
				data[j + k + l] = bottom;
			}
		}
	}
}

/*
 * find the frequency corresponding to maximum amplitude in the data group after fft
 * amplitude = sqrt(data[i].real * data[i].real + 
 *			data[i].img * data[i].img)
 */
extern float find_freq_max(complex_int *data)
{
	int i, j;
	int ampl2_max, ampl2_clac_sum = 0;
	int freq_calc = 0;
	float freq_max;

	int ampl2_val[FREQ_SIZE];
	int ampl2_index[FREQ_SIZE];

	for (i = MIN_FREQ_POS, j = 0; i < MAX_FREQ_POS; ++i, ++j)
	{
		ampl2_val[j] = data[i].real * data[i].real + 
			data[i].img * data[i].img;
		ampl2_index[j] = i;
	}

	for (i = 0; i < FREQ_INDEX_NUM; ++i)
	{
		ampl2_max =0;

		for (j = 0; j < FREQ_SIZE; ++j)
		{
			if (ampl2_max < ampl2_val[j])
			{
				ampl2_max  = ampl2_val[j];
				ampl2_clac_sum += ampl2_max;
				freq_calc  += ampl2_index[j] * ampl2_max;

				ampl2_val[j] = 0;
			}
		}
	}

	freq_max = (((float)freq_calc / ampl2_clac_sum / FREQ_INDEX_NUM) * DATA_SAMPLING_FREQ) / DATA_SIZE;

	// for (i = MIN_FREQ_POS; i < MAX_FREQ_POS; ++i)
	// {
	// 	ampl2_val[0] = data[i].real * data[i].real + 
	// 	data[i].img * data[i].img;

	// 	if (ampl2_max < ampl2_val[0])
	// 	{
	// 		ampl2_max  = ampl2_val[0];
	// 		freq_calc  = i;

	// 		data[i].real = 0;
	// 		data[i].img = 0;
	// 	}
	// }
	// freq_max = ((float)freq_calc * DATA_SAMPLING_FREQ) / DATA_SIZE;

  return freq_max;
}