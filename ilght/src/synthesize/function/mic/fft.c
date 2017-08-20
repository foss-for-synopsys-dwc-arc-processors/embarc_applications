#include "embARC.h"
#include "fft.h"
#include "math.h"

/***********    fft operation     ********/
const int List[fft_N];          /** the list table of fft */
const float sin_tab[fft_N/2];   /** the sin table of fft */
const float cos_tab[fft_N/2];   /** the cos table of fft */

/** fft step1 */
void fft_Data(uint16_t *fftIn, compx *fftData)/*1*/
{
	int i = 0;    
	for (i = 0; i < fft_N; i++ ) {
		/** Copy the data to fft,using the method of search the table */
    fftData[List[i]].real = (float)fftIn[i];
    /** Clear the imaginary part */
    fftData[List[i]].imag = 0;              
	}   
}
/** fft step2 */
/** the size of input array is N+1 */
void fft_Convert(compx *xin)
{
  uint16_t i, j, L, p, le, B, ip;    		   
  compx t, ws;

  /** Fourier transform */
	for(L = 1; L <= M; L++) {  																     
		le = (2 << L) >> 1;
    B = le / 2;	   	 
    for (j = 0; j <= B - 1; j++) {	   
      p = (((2 << M) >> L) >> 1)*j;
      ws.real = cos_tab[p];
      ws.imag = sin_tab[p];
      for (i = j; i <= fft_N-1; i = i + le) { 
        ip = i+B;		 
       	t.real = xin[ip].real * ws.real - xin[ip].imag * ws.imag;
        t.imag = xin[ip].real * ws.imag + xin[ip].imag * ws.real;
        xin[ip].real = xin[i].real - t.real;
        xin[ip].imag = xin[i].imag - t.imag;
        xin[i].real  = xin[i].real + t.real;
        xin[i].imag  = xin[i].imag + t.imag;
      }
    }
  }
}
/** fft step3 */
void fft_powerMag(compx *fftData, uint8_t *fftOut)
{
  uint8_t i;
  uint16_t tempfft = 0;
  
	for (i = 1; i < fft_N/2; i++) {
    fftData[i].real = (float)(sqrt( fftData[i].real * fftData[i].real + fftData[i].imag * fftData[i].imag )
                      / (i == 0 ? fft_N:( fft_N / 2 )));
	  /** compress the data */
	  tempfft = (uint8_t)fftData[i].real; 
	    
    //if (tempfft > fftOut[i] ) {
	    fftOut[i] = tempfft;
	  //}
	  //else {
	    /** Don't missing 0 */
	    //if (fftOut[i] > 0) {
	    //fftOut[i]-=1;
	   // }
    //}
  }
}

	

