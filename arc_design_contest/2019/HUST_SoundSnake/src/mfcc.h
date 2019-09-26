#ifndef __KWS_MFCC_H__
#define __KWS_MFCC_H__

#include "embARC.h"
#include "embARC_debug.h"

#define NUM_FRAMES 49
#define SAMP_FREQ 16000
#define NUM_FBANK_BINS 40
#define MFCC_DEC_BITS 1
#define MEL_LOW_FREQ 20
#define MEL_HIGH_FREQ 4000
#define MFCC_BUFFER_SIZE (NUM_FRAMES*NUM_MFCC_COEFFS)
#define FRAME_LEN_MS 40
#define FRAME_LEN ((int16_t)(SAMP_FREQ * 0.001 * FRAME_LEN_MS))
#define NUM_MFCC_COEFFS 10
#define FRAME_SHIFT_MS 20
#define FRAME_SHIFT ((int16_t)(SAMP_FREQ * 0.001 * FRAME_SHIFT_MS))


#define M_2PI 6.283185307179586476925286766559005f

typedef struct  mfcc_str
{
	int num_mfcc_features;
	int frame_len;
	int frame_len_padded;
	int mfcc_dec_bits;
	float * frame;
	float * buffer;
	float * mel_energies;
	float * window_func;
	int32_t * fbank_filter_first;
	int32_t * fbank_filter_last;
	float ** mel_fbank;
	float * dct_matrix;
}MFCC_STR;

void mfcc_compute(MFCC_STR *mfcc_str, const int16_t * audio_data, int8_t* mfcc_out);
int32_t mfcc_init(MFCC_STR *mfcc_str, int num_mfcc_features, int frame_len, int mfcc_dec_bits);

#endif
