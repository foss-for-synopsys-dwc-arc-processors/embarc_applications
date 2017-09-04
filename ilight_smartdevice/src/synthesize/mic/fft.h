#ifndef _fft_H_
#define _fft_H_

/** Fourier Transform,the buffer of input and output */
typedef struct { 
	float real;
	float imag;
}compx;

#define fft_16

#ifdef fft_16
#define fft_N 16
#define M 4
#endif

#ifdef fft_64
#define fft_N 64
#define M 8
#endif

#ifdef fft_128
#define fft_N 128
#define M 12
#endif

#ifdef fft_256
#define fft_N 256
#define M 16
#endif

#ifdef fft_512
#define fft_N 512
#define M 23
#endif

#ifdef fft_1024
#define fft_N 1024
#define M 32
#endif	 

/** fft step1 */
extern void fft_Data(uint16_t *fftIn, compx *fftData);

/** fft step2 */
extern void fft_Convert(compx *xin);

/** fft step3 */
extern void fft_powerMag(compx *fftData, uint8_t *fftOut);

#endif

