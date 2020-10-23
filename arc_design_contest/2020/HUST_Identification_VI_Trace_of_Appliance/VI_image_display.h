#ifndef _VI_IMAGE_DISPLAY_H_
#define _VI_IMAGE_DISPLAY_H_


#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "VI_image.h"




void m_u8g_init(void);
void display(uint8_t* rgb_image,uint8_t kind_index);

#endif
