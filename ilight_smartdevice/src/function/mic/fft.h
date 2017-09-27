/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * \version 2017.09
 * \date 2017-09-27
 * \author Ruige Lee(295054118@whut.edu.cn)
--------------------------------------------- */

/**
 * \file
 * \ingroup 	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief 	Fast Fourier transform of the data
 */

/**
 * \addtogroup  EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */

#ifndef _FFT_H_
#define _FFT_H_

/**
 * @file
 *   This file includes definitions for channel number of fast fourier transform.
 */

/*!< the buffer of input and output for fast fourier transform */
typedef struct { 
	float real;
	float imag;
}compx;

/* choose the number of fast fourier transform channel number */
#define fft_16
//#define fft_64
//#define fft_128
//#define fft_256
//#define fft_512
//#define fft_1024

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

/* fft operation */

/* the list table of fft */
extern const int List[fft_N];

/* the sin table of fft */
extern const float sin_tab[fft_N / 2];

/* the cos table of fft */
extern const float cos_tab[fft_N / 2];

/**
 * \brief  	fast fourier transform step1.
 * \details  	Transform real number data to complex number data struct.  
 * \param[in]	the pointer of adc data array.
 * \param[in]	the pointer of output complex number data struct.
 */
extern void fft_Data(uint16_t *fftIn, compx *fftData);

/**
 * \brief 	fast fourier transform step2.
 * \details 	fast fourier transform the data in array.  
 * \param[in]	the pointer of input data array and the result would be store in this array after transform.
 */
extern void fft_Convert(compx *xin);

/**
 * \brief	fast fourier transform step3.
 * \details	reduce the size of data by calculate the absolute value of complex data.  
 * \param[in]	the pointer of the complex data after transform.
 * \param[in]	the pointer of the array storing the compress data.
 */
extern void fft_powerMag(compx *fftData, uint8_t *fftOut);

#endif/* _FFT_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE */