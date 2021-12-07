#ifndef _SYN6288_
#define _SYN6288_


#include "string.h"
#include "stdio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "delay.h"


int audio_synth_init();
void audio_synth(uint8_t *text);
void audio_synth_num(int num);
char* Int2String(int num,char *str);
#endif