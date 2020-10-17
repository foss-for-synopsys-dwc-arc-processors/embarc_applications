#ifndef _VI_GENERATE_H_
#define _VI_GENERATE_H_

#include "spi_att7053.h"


#define IMAGE_SIZE (32 * 32 * 3)
#define PERIOD_SIZE 144
#define N 16

typedef struct vi_data{
	float v[720];
	float i[720];
}vi_data_t;

typedef struct img_coor{
	int x_v;
	int y_i;
}img_coor_t;

void generate_VI_image(vi_data_t* data,unsigned char* image);

#endif
