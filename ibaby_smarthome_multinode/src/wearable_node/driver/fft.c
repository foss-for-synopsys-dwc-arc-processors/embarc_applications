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
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

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
 * \version 2017.06
 * \date 2017-06-26
 * \author dbHu(wwmhu@outlook.com)
--------------------------------------------- */

/**
 * \defgroup  EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER        embARC iBaby Smarthome Node Wearable Node Driver
 * \ingroup EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief fast fourier transform function for heartrate data processing
 * \details
 *    fast fourier transform function for heartrate data processing.
 */

/**
 * \file
 * \ingroup EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief fast fourier transform function
 */

/**
 * \addtogroup  EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * @{
 */
#include "fft.h"
#include "math.h"
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

Cplx16 x[FFT_LEN];
Cplx16 w[FFT_LEN / 2];

void hanning(Cplx16 *D)
{
    int i;
    float a[num_taps];
    for (i = 0; i < num_taps; i++)
    {
        a[i] = 0.5 * (1.0 - cos(2.0 * PI * i / (num_taps - 1)));
        D[i].R = a[i] * D[i].R;
    }
}


int bit_reverse(int x)
{
    int i, rtn = 0;
    for (i = 0; i < FFT_M; i++)
        rtn = (rtn << 1) | ((x >> i) & 1);
    return rtn;
}

void calc_w(Cplx16 *W)
{
    int i;
    double arg, argStep;
    argStep = PI / (FFT_LEN >> 1);
    for (arg = 0.0, i = 0; i < (FFT_LEN >> 1); arg -= argStep, i++)
    {
        W[i].R = round(cos(arg) * S16MAX);
        W[i].I = round(sin(arg) * S16MAX);
    }
}

void fft(Cplx16 *D, Cplx16 *W)
{
    int i, j, step, grpLen, grp, w;
    Cplx16 T, Sub;
    hanning(D);
    for (step = 0, grpLen = 1 << (FFT_M - 1); step < FFT_M; step++, grpLen >>= 1)
    {
        for (grp = 0; grp < FFT_LEN; grp += grpLen << 1)
        {
            for (i = grp, j = grp + grpLen, w = 0; i < grp + grpLen; i++, j++, w += (1 << step))
            {
                T.R = ((int)D[i].R + (int)D[j].R) >> 1;
                T.I = ((int)D[i].I + (int)D[j].I) >> 1;
                Sub.R = ((int)D[i].R - (int)D[j].R) >> 1;
                Sub.I = ((int)D[i].I - (int)D[j].I) >> 1;
                D[j].R = (((int)Sub.R * (int)W[w].R) >> 15) - (((int)Sub.I * (int)W[w].I) >> 15);
                D[j].I = (((int)Sub.I * (int)W[w].R) >> 15) + (((int)Sub.R * (int)W[w].I) >> 15);
                D[i] = T;
            }
        }
    }
    for (i = 0; i < FFT_LEN; i++)
    {
        j = bit_reverse(i);
        if (i < j)
        {
            T = D[j];
            D[j] = D[i];
            D[i] = T;
        }
    }
}


float find_max(Cplx16 *D)
{
  int i=0,j=0;
  int index,sum_mag = 0,tmpfft = 0;
  float rtn_freq=0;
  for(i = 0; i < FFT_LEN / 4; i++)
  {
    D[i].R = (sqrt( D[i].R * D[i].R + D[i].I * D[i].I ) / 2 );
  }

  for(j = 0; j < 4; j++)
  {
    for(i = 6; i < 16; i++)
    {
        if(D[i].R > tmpfft)
        {
          tmpfft = D[i].R;
          index = i;
          D[i].R = 0;
        }  
    }
    rtn_freq += tmpfft * index;
    sum_mag += tmpfft;
    tmpfft = 0;
  }
  rtn_freq = (float)(rtn_freq / sum_mag);
  return rtn_freq;
}

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */








