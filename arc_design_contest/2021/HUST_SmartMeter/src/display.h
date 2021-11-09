#ifndef _DISPLAY_H_
#define _DISPLAY_H_


#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "stdio.h"
#include "data_provider.h"
#include "board.h"
/* middleware level*/
#include "u8g.h"

void m_u8g_init(void);
void display(uint16_t kind_index);
void display(uint16_t kind_index,uint16_t display_power);
void display_app_name(uint16_t kind_index);
void display_app_power(uint16_t app_power);
void PrintEvent(sample_data_node_ptr front, int windows_len);
#endif
