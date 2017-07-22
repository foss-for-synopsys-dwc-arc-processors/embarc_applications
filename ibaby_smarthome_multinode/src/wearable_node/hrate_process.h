#ifndef HRATE_PROCESS_H
#define HRATE_PROCESS_H

/*
 * Heart Rate Monitor functions takes a sample value and the sample number
 * returns true if a beat is detected
 * a running average of four samples is recommended for display on the screen
 */
extern int check_beat(int sample);

/* average DC estimator */
static int aver_dc_estimator(int *p, int x);

/* low pass FIR filter */
static int low_pass_fir_filter(int din);

/* integer multiplier */
static int mul16(int x, int y);

#endif
