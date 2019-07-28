#include <stdlib.h>
#include "mfcc.h"
#include "float.h"
#include <stdio.h>
#include "FFT.h"

static inline float MelScale(float freq) 
{
	return 1127.0f * logf (1.0f + freq / 700.0f);
}

static void create_dct_matrix(int32_t input_length, int32_t coefficient_count, float *outbuf)
{
	int32_t k, n;
	float normalizer;
	normalizer = sqrtf(2.0f/(float)input_length);
	for (k = 0; k < coefficient_count; k++) {
		for (n = 0; n < input_length; n++) {
			outbuf[k*input_length+n] = normalizer * cos( ((double)M_PI)/input_length * (n + 0.5) * k );
		}
	}
}

static void create_mel_fbank(MFCC_STR *mfcc_struct)
{
	int32_t bin, i, j=0;

	float **cmel_fbank = mfcc_struct->mel_fbank;
	int32_t *cfbank_filter_first = mfcc_struct->fbank_filter_first;
	int32_t *cfbank_filter_last = mfcc_struct->fbank_filter_last;
	int32_t num_fft_bins = mfcc_struct->frame_len_padded/2;
	float fft_bin_width = ((float)SAMP_FREQ) / ((float)mfcc_struct->frame_len_padded);
	float mel_low_freq = MelScale(MEL_LOW_FREQ);
	float mel_high_freq = MelScale(MEL_HIGH_FREQ);
	float left_mel, center_mel, right_mel, freq, mel, weight;
	int32_t first_index,last_index;
	float mel_freq_delta = (mel_high_freq - mel_low_freq) / (NUM_FBANK_BINS+1); //mel频率三角滤波器的步长

	float *this_bin = (float *)malloc(sizeof(float)*num_fft_bins);

	for (bin = 0; bin < NUM_FBANK_BINS; bin++) {

		left_mel = mel_low_freq + bin * mel_freq_delta;
		center_mel = mel_low_freq + (bin + 1) * mel_freq_delta;
		right_mel = mel_low_freq + (bin + 2) * mel_freq_delta;

		first_index = -1;
		last_index = -1;
#ifdef PRINTF_DEBUG_EN
		printf("The %d cmel_fbank is :",bin);
#endif
	for (i = 0; i < num_fft_bins; i++) {

		freq = (fft_bin_width * i);  // center freq of this fft bin.
		mel = MelScale(freq);
		this_bin[i] = 0.0;

		if (mel > left_mel && mel < right_mel) {
			weight = 0.0;
			if (mel <= center_mel) {
				weight = (mel - left_mel) / (center_mel - left_mel);
			} else {
				weight = (right_mel - mel) / (right_mel - center_mel);
			}
			this_bin[i] = weight;
			if (first_index == -1) {
				first_index = i;
			}
			last_index = i;
		}
	}

	cfbank_filter_first[bin] = first_index;
	cfbank_filter_last[bin] = last_index;
	cmel_fbank[bin] = (float *)malloc(sizeof(float)*(last_index-first_index+1));

	//copy the part we care about
	j = 0;
	for (i = first_index; i <= last_index; i++) {
		cmel_fbank[bin][j++] = this_bin[i];
#ifdef	PRINTF_DEBUG_EN
		printf("    %.10f",cmel_fbank[bin][j-1]);
#endif
	}
#ifdef	PRINTF_DEBUG_EN
	printf("\n");
	printf("The %d first_index and last_index is:%d  %d\n",bin,first_index,last_index);
#endif
	}
}

int32_t mfcc_init(MFCC_STR *mfcc_str, int num_mfcc_features, int frame_len, int mfcc_dec_bits)
{
	float *window_func_tmp;
	mfcc_str->num_mfcc_features = num_mfcc_features;
	mfcc_str->frame_len = frame_len;
	mfcc_str->mfcc_dec_bits = mfcc_dec_bits;

	/* 需要填充的数据个数，填充至2的n次方，便于做FFT */
	mfcc_str->frame_len_padded = pow(2,ceil((log(mfcc_str->frame_len)/log(2))));

	/* 大小为4kBytes */
	mfcc_str->frame = (float *)malloc(sizeof(float) * mfcc_str->frame_len_padded);
	if (mfcc_str->frame == NULL)
	{
		EMBARC_PRINTF("The mfcc frame malloc failed\r\n");
		return -1;
	}

	/* 大小为4kBytes */
	mfcc_str->buffer = (float *)malloc(2 * sizeof(float) * mfcc_str->frame_len_padded);
	if (mfcc_str->buffer == NULL)
	{
		EMBARC_PRINTF("The mfcc buffer malloc failed\r\n");
		return -1;
	}

	/* 大小约为0.16k */
	mfcc_str->mel_energies = (float *)malloc(sizeof(float) * NUM_FBANK_BINS);
	if(mfcc_str->mel_energies == NULL)
	{
		EMBARC_PRINTF("The mfcc mel_energies malloc failed\r\n");
		return -1;
	}

	/* 大小约为0.16kBytes */
	mfcc_str->fbank_filter_first = (int32_t *)malloc(sizeof(int32_t) * NUM_FBANK_BINS);
	if(mfcc_str->fbank_filter_first == NULL)
	{
		EMBARC_PRINTF("The mfcc mel_energies malloc failed\r\n");
		return -1;
	}

	/* 大小约为0.16kBytes */
	mfcc_str->fbank_filter_last = (int32_t *)malloc(sizeof(int32_t) * NUM_FBANK_BINS);
	if(mfcc_str->fbank_filter_last == NULL)
	{
		EMBARC_PRINTF("The mfcc mel_energies malloc failed\r\n");
		return -1;
	}

	/* 大小约为2.5k */
	mfcc_str->window_func = (float *)malloc(sizeof(float) * mfcc_str->frame_len);
	if(mfcc_str->window_func == NULL)
	{
		EMBARC_PRINTF("The mfcc window_func malloc failed\r\n");
		return -1;
	} else {
		window_func_tmp = mfcc_str->window_func;
	}


	mfcc_str->dct_matrix = (float *)malloc(sizeof(float *)*(NUM_FBANK_BINS*NUM_MFCC_COEFFS));
	if (mfcc_str->dct_matrix == NULL)
	{
		EMBARC_PRINTF("dct_matrix memory malloc failed!\n");
	}

	mfcc_str->mel_fbank =  (float **) malloc(sizeof(float *)*(NUM_FBANK_BINS));
	if (mfcc_str->mel_fbank == NULL)
	{
		EMBARC_PRINTF("mel_fbank memory malloc failed!\n");
	}

	for (int i = 0; i < frame_len; i++){
		window_func_tmp[i] = 0.5 - 0.5 * cos(M_2PI * ((float)i) / (frame_len));
	}

	create_mel_fbank(mfcc_str);
	create_dct_matrix(NUM_FBANK_BINS, NUM_MFCC_COEFFS,mfcc_str->dct_matrix);

	EMBARC_PRINTF("MFCC init success\r\n");
	return 0;
}

void mfcc_compute(MFCC_STR *mfcc_str, const int16_t * audio_data, int8_t* mfcc_out) 
{
	int32_t i, j, bin;
	float sqrt_data;
	int32_t half_dim;
	float mel_energy;
	int32_t first_index;
	int32_t last_index;
	float real;
	float im;
	float sum;
	float first_energy, last_energy;

	/* 临时变量 */
	float *frame_tmp = mfcc_str->frame;
	float *buffer_tmp = mfcc_str->buffer;
	float **mel_fbank_tmp = mfcc_str->mel_fbank;
	float *mel_energies_tmp = mfcc_str->mel_energies;
	float *dct_matrix_tmp = mfcc_str->dct_matrix;

	//将wav数据压缩到 (-1,1)
	for (i = 0; i < FRAME_LEN; i++) {
		frame_tmp[i] = (float)audio_data[2*i] / (1<<15); 
	}

	//FFT变换需要2的n次方，这里将多于一帧的数据点填充0
	memset(&frame_tmp[FRAME_LEN], 0, sizeof(float) * (mfcc_str->frame_len_padded - FRAME_LEN));

	for (i = 0; i < FRAME_LEN; i++) {
		frame_tmp[i] *= mfcc_str->window_func[i];
	}

	//做傅里叶变换
	//arm_rfft_fast_f32(mfcc_str->rfft, frame_tmp, buffer_tmp, 0);
	memset(buffer_tmp, 0, sizeof(float) * 1024 * 2);
	for(i=0;i<1024;i++)
		buffer_tmp[2*i] = frame_tmp[i];

	fftx((Complex*)buffer_tmp, 10);

	//获取各个频率分量的实际幅值
	//[real0, realN/2-1, real1, im1, real2, im2, ...]
	half_dim = mfcc_str->frame_len_padded / 2;
	first_energy = buffer_tmp[0] * buffer_tmp[0];
	last_energy =  buffer_tmp[1] * buffer_tmp[1];  // handle this special case
	for (i = 1; i < half_dim; i++) 
	{
		real = buffer_tmp[i*2];
		im = buffer_tmp[i*2+1];
		buffer_tmp[i] = real * real + im * im;
	}
	buffer_tmp[0] = first_energy;
	buffer_tmp[half_dim] = last_energy;  

	//使用梅尔滤波器进行滤波
	for (bin = 0; bin < NUM_FBANK_BINS; bin++)
	{
		j = 0;
		mel_energy = 0;
		first_index = mfcc_str->fbank_filter_first[bin];
		last_index = mfcc_str->fbank_filter_last[bin];
		for (i = first_index; i <= last_index; i++) 
		{
			sqrt_data = sqrtf(buffer_tmp[i]);
			mel_energy += (sqrt_data) * mel_fbank_tmp[bin][j++];
	  	}
		mel_energies_tmp[bin] = mel_energy;

		//这是为了下面去对数的时候不为0
		if (mel_energy == 0.0)
			mel_energies_tmp[bin] = FLT_MIN;
	}

	//取对数
	for (bin = 0; bin < NUM_FBANK_BINS; bin++)
		mel_energies_tmp[bin] = logf(mel_energies_tmp[bin]);

	//做离散余弦变换
	for (i = 0; i < NUM_MFCC_COEFFS; i++) 
	{
		sum = 0.0;
		for (j = 0; j < NUM_FBANK_BINS; j++) 
		{
			sum += dct_matrix_tmp[i * NUM_FBANK_BINS + j] * mel_energies_tmp[j];
		}

		//将MFCC特征量化为q7_t格式
		sum *= (0x1<<(mfcc_str->mfcc_dec_bits));
		sum = round(sum); 
		if(sum >= 127)
			mfcc_out[i] = 127;
		else if(sum <= -128)
			mfcc_out[i] = -128;
		else
			mfcc_out[i] = sum; 
	}
}
