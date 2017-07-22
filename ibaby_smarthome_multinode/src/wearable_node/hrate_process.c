#include "hrate_process.h"

static int ir_ac_max = 20;
static int ir_ac_min = -20;

static int val_cur = 0;
static int val_pre;
static int val_min = 0;
static int val_max = 0;
static int aver_estimated;

static int pos_edge = 0;
static int neg_edge = 0;
static int ir_avg_reg = 0;

static int cbuf[32];
static int offset = 0;

static const int fir_coeffs[12] = {172, 321, 579, 927, 1360, 1858, 2390, 2916, 3391, 3768, 4012, 4096};

/*
 * Heart Rate Monitor functions takes a sample value and the sample number
 * Returns true if a beat is detected
 * A running average of four samples is recommended for display on the screen
 */
extern int check_beat(int sample)
{
	int beat_detected = 0;

	/* Save current state */
	val_pre = val_cur;

	/* Process next data sample */
	aver_estimated = aver_dc_estimator(&ir_avg_reg, sample);
	val_cur = low_pass_fir_filter(sample - aver_estimated);

	/* Detect positive zero crossing (rising edge) */
	if ((val_pre < 0) & (val_cur >= 0))
	{

		ir_ac_max = val_max; /* Adjust our AC max and min */
		ir_ac_min = val_min;

		pos_edge = 1;
		neg_edge = 0;
		val_max = 0;

		if ((ir_ac_max - ir_ac_min) > 1000 & (ir_ac_max - ir_ac_min) < 3000)
		{
			/* Heart beat!!! */
			beat_detected = 1;
		}
	}

	/* Detect negative zero crossing (falling edge) */
	if ((val_pre > 0) & (val_cur <= 0))
	{
		pos_edge = 0;
		neg_edge = 1;
		val_min = 0;
	}

	/* Find Maximum value in positive cycle */
	if (pos_edge & (val_cur > val_pre))
	{
		val_max = val_cur;
	}

	/* Find Minimum value in negative cycle */
	if (neg_edge & (val_cur < val_pre))
	{
		val_min = val_cur;
	}

	return (beat_detected);
}

/* Average DC Estimator */
static int aver_dc_estimator(int *p, int x)
{
	*p += ((((long) x << 15) - *p) >> 4);
	return (*p >> 15);
}

/* Low Pass FIR Filter */
static int low_pass_fir_filter(int din)
{
	cbuf[offset] = din;

	int z = mul16(fir_coeffs[11], cbuf[(offset - 11) & 0x1F]);

	for (int i = 0 ; i < 11 ; i++)
	{
		z += mul16(fir_coeffs[i], cbuf[(offset - i) & 0x1F] + cbuf[(offset - 22 + i) & 0x1F]);
	}

	offset++;
	offset %= 32; /* Wrap condition */

	return (z >> 15);
}

/* Integer multiplier */
static int mul16(int x, int y)
{
	return ((long)x * (long)y);
}