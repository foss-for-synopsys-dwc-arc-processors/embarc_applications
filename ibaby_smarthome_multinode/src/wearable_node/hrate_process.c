#include "hrate_process.h"

static int ir_ac_max = 20;
static int ir_ac_min = -20;

static int val_cur = 0;    /* current ir value */
static int val_pre;        /* previous ir value */
static int val_min = 0;
static int val_max = 0;
static int aver_estimated; /* average ir value */

static int pos_edge = 0;   /* positive edge */
static int neg_edge = 0;   /* negative edge */
static int ir_avg_reg = 0;

static int cbuf[32];
static int offset = 0;

static const int fir_coeffs[12] = {172, 321, 579, 927, 1360, 1858, 2390, 2916, 3391, 3768, 4012, 4096};

/*
 * Heart Rate Monitor functions takes a sample value and the sample number
 * returns true if a beat is detected
 * a running average of four samples is recommended for display on the screen
 */
extern int check_beat(int sample)
{
	int beat_detected = 0;

	/* save current state */
	val_pre = val_cur;

	/* process next data sample */
	aver_estimated = aver_dc_estimator(&ir_avg_reg, sample);
	val_cur = low_pass_fir_filter(sample - aver_estimated);

	/* detect positive zero crossing (rising edge) */
	if ((val_pre < 0) & (val_cur >= 0))
	{

		ir_ac_max = val_max; /* adjust our AC max and min */
		ir_ac_min = val_min;

		pos_edge = 1;
		neg_edge = 0;
		val_max = 0;

		if ((ir_ac_max - ir_ac_min) > 1000 & (ir_ac_max - ir_ac_min) < 3000)
		{
			/* heart beat!!! */
			beat_detected = 1;
		}
	}

	/* detect negative zero crossing (falling edge) */
	if ((val_pre > 0) & (val_cur <= 0))
	{
		pos_edge = 0;
		neg_edge = 1;
		val_min = 0;
	}

	/* find maximum value in positive cycle */
	if (pos_edge & (val_cur > val_pre))
	{
		val_max = val_cur;
	}

	/* find minimum value in negative cycle */
	if (neg_edge & (val_cur < val_pre))
	{
		val_min = val_cur;
	}

	return (beat_detected);
}

/* average DC estimator */
static int aver_dc_estimator(int *p, int x)
{
	*p += ((((long) x << 15) - *p) >> 4);
	return (*p >> 15);
}

/* low pass FIR filter */
static int low_pass_fir_filter(int din)
{
	cbuf[offset] = din;

	int z = mul16(fir_coeffs[11], cbuf[(offset - 11) & 0x1F]);

	for (int i = 0 ; i < 11 ; i++)
	{
		z += mul16(fir_coeffs[i], cbuf[(offset - i) & 0x1F] + cbuf[(offset - 22 + i) & 0x1F]);
	}

	offset++;
	offset %= 32; /* wrap condition */

	return (z >> 15);
}

/* integer multiplier */
static int mul16(int x, int y)
{
	return ((long)x * (long)y);
}