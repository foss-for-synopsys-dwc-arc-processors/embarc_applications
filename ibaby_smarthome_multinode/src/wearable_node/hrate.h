#ifndef HRATE_H
#define HRATE_H

extern int checkForBeat(int sample);
static int averageDCEstimator(int *p, int x);
static int lowPassFIRFilter(int din);
static int mul16(int x, int y);

#endif
