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
--------------------------------------------- */

#ifndef __WM8978_H
#define __WM8978_H
#include "embARC.h"
#include "embARC_debug.h"
#include "dev_iic.h"
#include "stdio.h"

#define WM8978_RECORDER_EN		1		/*!< Enable the recorder function */
#define WM8978_PLAYER_EN		0		/*!< Disable the audio player function */
#define WM8978_OUT_DEV			0		/*!< Choose the headphone as the out device */
#define WM8978_CLK_SEL			1		/*!< Choose the mclk to generate 256fs */
#define WM8978_MIC_GAIN 		40		/*!< Set the gain of mic */

/** The frequency of the main clock signal in multiples of the sampling frequency  */
typedef enum wm8978_mclk_fs {
	WM8978_MCLK_256FS = 256,	/*!< 256fs */
	WM8978_MCLK_384FS = 384,	/*!< 384fs */
	WM8978_MCLK_512FS = 512,	/*!< 512fs */
	WM8978_MCLK_640FS = 640,	/*!< 640fs */
	WM8978_MCLK_768FS = 768,	/*!< 768fs */
	WM8978_MCLK_1024FS = 1024,	/*!< 1536fs */
	WM8978_MCLK_1536FS = 1536,	/*!< 1536fs */
} WM8978_MCLK_FS;

/** The format of I2S  */
typedef enum wm8978_fmt_i2s {
	WM8978_FMT_I2S_LSB = 0,	/*!< Left-justified format */
	WM8978_FMT_I2S_MSB = 1,	/*!< Right-justified format */
	WM8978_FMT_I2S_PHI = 2,	/*!< Philips mode */
	WM8978_FMT_I2S_PCM = 3,	/*!< PCM/DSP mode */
} WM8978_FMT_I2S;

/** The resolution of I2S  */
typedef enum wm8978_len_i2s {
	WM8978_LEN_I2S_16 = 0,	/*!< 16bits */
	WM8978_LEN_I2S_20 = 1,	/*!< 20bits */
	WM8978_LEN_I2S_24 = 2,	/*!< 24bits */
	WM8978_LEN_I2S_32 = 3,	/*!< 32bits */
} WM8978_LEN_I2S;

typedef struct wm8978_init_str
{
	bool recorder_en;		/* !<Weather set the wm8978 as a recorder,1,enable.0,disable */
	bool player_en;			/* !<Weather set the wm8978 as a player,1,enable.0,disable */
	bool out_dev;			/* !<Choose the device of output.1,headphone.0,speaker */
	bool clk_sel;			/* !<Select the clock for A/D and D/A.1,mclk.0,pll. */
	uint8_t gain_mic;		/* !<Set the gain of mic */
	uint16_t mclk;			/* !<The multiple of fs */
	uint8_t sample_rate;	/* !<The sample rate of A/D and D/A ,khz*/
	uint8_t volume_dev;		/* !<Set the volume of headhone/speaker */
	uint8_t fmt_i2s;		/* !<fmt.The format of I2S operating mode.0,LSB. 1,MSB. 2,Philips. 3,PCM/DSP */
	uint8_t len_i2s;		/* !<Tlen;The length of data.0,16bits.1,20bits.2,24bits.3,32bits. */
}WM8978_INIT_STR;

/* Init wm8978 according to the data of WM8978_INIT_STR  */
extern uint8_t wm8978_init(WM8978_INIT_STR *init_str);
extern uint8_t wm8978_init_func(void);
#endif
