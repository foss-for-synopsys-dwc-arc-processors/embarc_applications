#include "stdlib.h"
#include "math.h"

//#define __EXTERN_W__

typedef struct
{
	float real;
	float imag;
} Complex;

#ifdef __EXTERN_W__
//========Paste the definitions for W here========
const Complex FFT_W[15] = 
{
	{0.98078528040323,-0.195090322016128},
	{0.923879532511287,-0.38268343236509},
	{0.831469612302545,-0.555570233019602},
	{0.707106781186547,-0.707106781186547},
	{0.555570233019602,-0.831469612302545},
	{0.38268343236509,-0.923879532511287},
	{0.195090322016128,-0.98078528040323},
	{5.55111512312578E-17,-1},
	{-0.195090322016128,-0.98078528040323},
	{-0.38268343236509,-0.923879532511287},
	{-0.555570233019602,-0.831469612302545},
	{-0.707106781186547,-0.707106781186547},
	{-0.831469612302545,-0.555570233019602},
	{-0.923879532511286,-0.38268343236509},
	{-0.98078528040323,-0.195090322016128}
};
const Complex *w = FFT_W;
//================================================
#endif

unsigned char fftx(Complex *d,int m)
{
#ifndef __EXTERN_W__
	static Complex *w;
	static int mw = 0;
	float arg, w_real, w_imag, wr_real, wr_imag, wtemp;
#endif
	static int n = 1;
	Complex temp, tmp1, tmp2;
	Complex *di, *dip, *dj, *wp;
	int i, j, k, l, le, wi;
#ifndef __EXTERN_W__
	if(m != mw)
	{
		if(mw != 0)
			free(w);
		mw = m;
		if(m == 0)
			return 0;
		n = 1 << m;
		le = n >> 1;
		w = (Complex*)calloc(le - 1,sizeof(Complex));
		if(!w)
			return 0;
		arg = 4.0 * atan(1.0) / le;
		wr_real = w_real = cos(arg);
		wr_imag = w_imag = -sin(arg);
		dj = w;
		for(j = 1; j < le; j++)
		{
			dj->real = (float)wr_real;
			dj->imag = (float)wr_imag;
			dj++;
			wtemp = wr_real * w_real - wr_imag * w_imag;
			wr_imag = wr_real * w_imag + wr_imag * w_real;
			wr_real = wtemp;
		}
	}
#else
	n = 1 << m;
#endif
	le = n;
	wi = 1;
	for(l = 0; l < m; l++)
	{
		le >>= 1;
		for(i = 0; i < n; i += (le << 1))
		{
			di = d + i;
			dip = di + le;
			temp.real = (di->real + dip->real);
			temp.imag = (di->imag + dip->imag);
			dip->real = (di->real - dip->real);
			dip->imag = (di->imag - dip->imag);
			*di = temp;                       
		}
		wp = (Complex*)w + wi - 1;
		for(j = 1; j < le; j++)
		{
			tmp1 = *wp;
			for(i = j; i < n; i += (le << 1))
			{
				di = d + i;
				dip = di + le;
				temp.real = (di->real + dip->real);
				temp.imag = (di->imag + dip->imag);
				tmp2.real = (di->real - dip->real);
				tmp2.imag = (di->imag - dip->imag);
				dip->real = (tmp2.real * tmp1.real - tmp2.imag * tmp1.imag);
				dip->imag = (tmp2.real * tmp1.imag + tmp2.imag * tmp1.real);
				*di = temp;
			}
			wp += wi;
		}
		wi <<= 1;
	}
	
	for(i = 0; i < n; i++)
	{
		j = 0;
		for(k = 0; k < m; k++)
			j = (j << 1) | ((i >> k) & 1);
		if(i < j)
		{
			di = d + i;
			dj = d + j;
			temp = *dj;
			*dj = *di;
			*di = temp;
		}
	}
	return 1;
}

void AmpSpectrum(Complex *pData,int m,float* pDCAmp,float* pDistortion)
{
   // Amp = Abs / (1.414213562 * N)
	int i;
	int N = 1 << (m - 1);
	int N2 = 1 << m;
	float BasePower;     //exactly BasePower*2*N*N;
	float SyntPower = 0; //exactly SyntPower*2*N*N;
	float ModelSquare;
	*pDCAmp = sqrt(pData[0].real * pData[0].real + pData[0].imag * pData[0].imag) / (N << 1);
	BasePower = pData[1].real * pData[1].real + pData[1].imag * pData[1].imag;
	pData[N2 - 1].real = sqrt(BasePower);
	pData[N2 - 1].imag = asin(pData[N2 - 1].imag / pData[N2 - 1].real);
	pData[N2 - 1].real /= N;
	for(i = 2; i < N; i++)
	{
		ModelSquare = pData[i].real * pData[i].real + pData[i].imag * pData[i].imag;
		pData[N2 - i].real = sqrt(ModelSquare);
		pData[N2 - i].imag = asin(pData[N2-i].imag/pData[N2-i].real);
		pData[N2 - i].real /= N;
		SyntPower += ModelSquare;
	}
	*pDistortion = sqrt(SyntPower / BasePower);
}
