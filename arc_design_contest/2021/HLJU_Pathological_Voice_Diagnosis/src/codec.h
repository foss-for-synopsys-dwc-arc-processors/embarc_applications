#ifndef _CODEC_H_
#define _CODEC_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "wm8978i2s.h"
#include "mfcc.h"

void CODEC_init(int16_t* ao_buf, int16_t* rx_buf);
//void CODEC_init();

void extract_features(MFCC_STR *mfcc_str, const int16_t *audio_data, int8_t *mfcc_buffer);

#define audio_buffer_len 16 * 1000 * 2
#endif
