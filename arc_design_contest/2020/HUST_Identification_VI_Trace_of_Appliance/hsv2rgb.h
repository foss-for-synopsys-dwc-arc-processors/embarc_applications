#ifndef _HSV_TO_RGB_H_
#define _HSV_TO_RGB_H_

typedef struct RGBColor {
	int r;
	int g;
	int b;
} RGBColor;

RGBColor hsv2rgb(float H, float S, float V);

#endif
