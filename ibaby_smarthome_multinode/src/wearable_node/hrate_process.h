#ifndef HRATE_PROCESS_H
#define HRATE_PROCESS_H

/*
 * Heart Rate Monitor functions takes a sample value and the sample number
 * Returns true if a beat is detected
 * A running average of four samples is recommended for display on the screen
 */
extern int check_beat(int sample);

/* Average DC Estimator */
static int aver_dc_estimator(int *p, int x);

/* Low Pass FIR Filter */
static int low_pass_fir_filter(int din);

/* Integer multiplier */
static int mul16(int x, int y);

#endif
