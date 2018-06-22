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
--------------------------------------------- */

/**
 * \file
 * \ingroup 	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief 	function for calculate the frequency data capture from microphone
 */

/**
 * \addtogroup  EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */

/* embARC HAL */
#include "embARC.h"

/* CUSTOM HAL */
#include "math.h"
#include "fft.h"

/**
 * \brief	fast fourier transform step1 (prepar).
 * \details	Transform real number data to complex number data struct.  
 * \param[in]	the pointer of adc data array.
 * \param[in]	the pointer of output complex number data struct.
 */
void fft_Data(uint16_t *fftIn, compx *fftData)
{
	int i = 0;
	for ( i = 0; i < fft_N; i++ ) {
		fftData[List[i]].real = (float)fftIn[i]; /* Copy the data to fft,using the method of search the table */
		fftData[List[i]].imag = 0;               /* Clear the imaginary part */       
	}
}

/**
 * \brief	fast fourier transform step2 (transform).
 * \details	fast fourier transform the data in array.  
 * \param[in]	the pointer of input data array and the result would be store in this array after transform.
 */
void fft_Convert(compx *xin)
{
	uint16_t i, j, L, p, le, B, ip;
	compx t, ws;
	for (L = 1; L <= M; L++) {
		le = (2 << L) >> 1;
		B = le / 2;

		for (j = 0; j <= B - 1; j++) {
			p = (((2 << M) >> L) >> 1) * j;
			ws.real = cos_tab[p];
			ws.imag = sin_tab[p];

			for (i = j; i <= fft_N - 1; i = i + le) {
				ip = i + B;
				t.real = xin[ip].real * ws.real - xin[ip].imag * ws.imag;
				t.imag = xin[ip].real * ws.imag + xin[ip].imag * ws.real;
				xin[ip].real = xin[i].real - t.real;
				xin[ip].imag = xin[i].imag - t.imag;
				xin[i].real = xin[i].real + t.real;
				xin[i].imag = xin[i].imag + t.imag;
			}
		}
	}
}

/**
 * \brief	fast fourier transform step3 (compress).
 * \details	reduce the size of data by calculate the absolute value of complex data.  
 * \param[in]	the pointer of the complex data after transform.
 * \param[in]	the pointer of the array storing the compress data.
 */
void fft_powerMag(compx *fftData, uint8_t *fftOut)
{
	uint8_t i;
	uint16_t tempfft = 0;

	for (i = 1; i < fft_N / 2; i++) {
		fftData[i].real = (float)(sqrt(fftData[i].real * fftData[i].real + fftData[i].imag * fftData[i].imag)
			  / (i == 0 ? fft_N : (fft_N / 2)));
		/* compress the data */	
		tempfft = (uint8_t)fftData[i].real;
		fftOut[i] = tempfft;
	}
}

/** @} */