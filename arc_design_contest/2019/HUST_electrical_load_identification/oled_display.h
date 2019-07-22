#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H


#include "embARC.h"
#include "embARC_debug.h"
#include "ssd1306_app_config.h"
#include "u8g.h"



void display_welcome(u8g_t *u8g_t);
void display_result(u8g_t *u8g_t, uint8_t res);






#endif