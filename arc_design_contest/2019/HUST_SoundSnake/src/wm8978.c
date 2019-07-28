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

#include "wm8978.h"

#define WM8978_ADDR				0X1A
#define WM8978_IIC_ID			0
#define WM8978_REG_RESET		0x00
#define WM8978_REG_POWER_1		0x01
#define WM8978_REG_POWER_2		0x02
#define WM8978_REG_POWER_3		0x03
#define WM8978_REG_AUDIO		0x04
#define WM8978_REG_COMPAND		0x05
#define WM8978_REG_CLOCK		0x06
#define WM8978_REG_DAC 			0x0A
#define WM8978_REG_ADC			0x0E
#define WM8978_REG_3D			0x29
#define WM8978_REG_BEEP			0x2B
#define WM8978_REG_INPUT		0X2C
#define WM8978_REG_LPGA			0x2D
#define WM8978_REG_RPGA			0x2E
#define WM8978_REG_LADC			0x2F
#define WM8978_REG_RADC			0x30
#define WM8978_REG_OUT			0x31
#define WM8978_REG_LMIX			0x32
#define WM8978_REG_RMIX			0x33
#define WM8978_REG_HPL			0x34
#define WM8978_REG_HPR			0x35
#define WM8978_REG_SPKL			0x36
#define WM8978_REG_SPKR			0x37

static DEV_IIC *wm8978_iic_p;

/**
 * \brief	The buffer is used to store values in registers of WM8979 locally because the IIC of WM8978 can't \
 			read the register value.
 * \notes	The length of WM8978'register is 9 and you have to store the value in type of uint16_t.
*/
static uint16_t wm8978_regval_tbl[58]=
{
	0X0000,0X0000,0X0000,0X0000,0X0050,0X0000,0X0140,0X0000,
	0X0000,0X0000,0X0000,0X00FF,0X00FF,0X0000,0X0100,0X00FF,
	0X00FF,0X0000,0X012C,0X002C,0X002C,0X002C,0X002C,0X0000,
	0X0032,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
	0X0038,0X000B,0X0032,0X0000,0X0008,0X000C,0X0093,0X00E9,
	0X0000,0X0000,0X0000,0X0000,0X0003,0X0010,0X0010,0X0100,
	0X0100,0X0002,0X0001,0X0001,0X0039,0X0039,0X0039,0X0039,
	0X0001,0X0001
};

/**
 * \brief	Init the IIC device.
 * \retval 	0. Init successfully.
 * \retval	1.Init failed.
 */
static uint8_t wm8978_iic_init(void)
{
	wm8978_iic_p = iic_get_dev(WM8978_IIC_ID);
	if(wm8978_iic_p == NULL)
	{
		EMBARC_PRINTF("The IIC of WM8978 init failed!\n");
		return 1;
	}
	wm8978_iic_p->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	return 0;
}

/**
 * \brief	Write the register of wm8978
 * \param	reg;The address of register which you want to write.
 * \param	val;The value you want to write in register and 9 bits is effective
 * \retval	0,write successfully.
 * \retval  !=0,write failed.
 */
static int8_t wm8978_write_reg(uint8_t reg,uint16_t val)
{
	int32_t ercd = E_PAR;
	uint8_t data[2];

	data[0] = (uint8_t)((reg << 1) | ((val >> 8) & 0X01));
	data[1] = (uint8_t)(val & 0xff);

	/** make sure set the temp sensor's slave address */
	wm8978_iic_p->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(WM8978_ADDR));

	//if(ercd = 2)EMBARC_PRINTF("The wm8978 data write 1%d\n",ercd);

	ercd = wm8978_iic_p->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));

	//if(ercd = 2)EMBARC_PRINTF("The wm8978 data write 2%d\n",ercd);

	ercd = wm8978_iic_p->iic_write(data, 2);
	if(ercd != 2)
	{
		EMBARC_PRINTF("The wm8978 data write is failed!%d\n",ercd);
		return ercd;
	}
	wm8978_regval_tbl[reg] = val;
	return 0;
}

/**
 * \brief	Read the register of wm8978
 * \param	reg;The address of register which you want to read
 * \retval	The value of register
 */
static uint16_t wm8978_read_reg(uint8_t reg)
{
	return wm8978_regval_tbl[reg];
}


/**
 * \brief	Set the gain of mic except the 20db gain of boost
 * \param	gain.0~63,-12dB~35.25dB and 0.75db each step
 */
static void wm8978_mic_gain(uint8_t gain)
{
	gain &= 0X3F;
	wm8978_write_reg(WM8978_REG_LPGA,gain);
	wm8978_write_reg(WM8978_REG_RPGA,gain | 1 << 8);
}

/**
 * \brief	Set the gain of linin(LEFT AND RIGHT).
 * \param	gain.0~7.0,disable the channel.1~7,-12db~6db,3db each step.
 */
static void wm8978_linein_gain(uint8_t gain)
{
	uint16_t regval;
	gain &= 0X07;
	regval = wm8978_read_reg(WM8978_REG_LADC);
	/* clean old value */
	regval &= ~(7 << 4);
 	wm8978_write_reg(WM8978_REG_LADC,regval | (gain << 4));

	regval = wm8978_read_reg(WM8978_REG_RADC);
	regval &= ~(7 << 4);
 	wm8978_write_reg(WM8978_REG_RADC,regval | (gain << 4));
}

/**
 * \brief	Set the gain of aux.
 * \param	gain.0~7.0,disable the channel.1~7,-12db~6db,3db each step.
 */
static void wm8978_aux_gain(uint8_t gain)
{
	uint16_t regval;
	gain &= 0X07;
	regval = wm8978_read_reg(WM8978_REG_LADC);
	regval &= ~(7 << 0);
 	wm8978_write_reg(WM8978_REG_LADC,regval | gain << 0);

	regval = wm8978_read_reg(WM8978_REG_RADC);
	regval &= ~(7 << 0);
 	wm8978_write_reg(WM8978_REG_RADC,regval | gain << 0);
}

/**
 * \brief	Configure the clock.
 * \param	mclk_en.Select the mclk as the main clock.
 * \param	The multiple of sample rate.
 */
static void wm8978_clk_set(uint8_t mclk_en,uint16_t mclk_fs)
{
	uint16_t regval;

	regval=wm8978_read_reg(WM8978_REG_CLOCK);
	if (mclk_en == 1)
	{
		regval &= ~(1 << 8);
	} else
	{
		EMBARC_PRINTF("The mclk_en is error ,can't choose pll.\n");
	}
	regval &= ~(0x07 << 5);
	switch (mclk_fs)
	{
		case  256 : regval |= (0x00 << 5);
				break;
		case  364 : regval |= (0x01 << 5);
				break;
		case  512 : regval |= (0x02 << 5);
				break;
		case  768 : regval |= (0x03 << 5);
				break;
		case 1024 : regval |= (0x04 << 5);
				break;
		case 1536 : regval |= (0x05 << 5);
				break;
		default : regval |= (0x02 << 5);
				break;
	}
	wm8978_write_reg(WM8978_REG_CLOCK,regval);
}

/**
 * \brief	Config the A/D and D/A block
 * \param	dacen;Enable the D/A block by 1 and disable this block by 0.
 * \param	adcen;Enable the A/D block by 1 and disable this block by 0.
 */
static void wm8978_adda_cfg(uint8_t dacen,uint8_t adcen)
{
	uint16_t regval;

	regval=wm8978_read_reg(WM8978_REG_POWER_3);
	if(dacen)
	{
		regval |= 3 << 0;
	} else {
		regval &= ~(3 << 0);
	}
	wm8978_write_reg(WM8978_REG_POWER_3,regval);

	regval = wm8978_read_reg(WM8978_REG_POWER_2);
	if(adcen)
	{
		regval |= 3 << 0;
	} else {
		regval &= ~(3 << 0);
	}
	wm8978_write_reg(WM8978_REG_POWER_2,regval);
}

/**
 * \brief	Config the input block.
 * \param	micen;Enable mic block by 1 and disable this block by 0.
 * \param	lineinen;Enable the line input block by 1 and disable this block by 0.
 * \param	auxen.Enable the aux block.1,enable.0,disable.
 */
static void wm8978_input_cfg(uint8_t micen,uint8_t lineinen,uint8_t auxen)
{
	uint16_t regval;
	regval=wm8978_read_reg(WM8978_REG_POWER_2);
	if(micen)
	{
		/*  Enable the PGA for mic */
		regval |= 3 << 2;
	} else {
		regval &= ~(3 << 2);
	}
 	wm8978_write_reg(WM8978_REG_POWER_2,regval);

	regval=wm8978_read_reg(WM8978_REG_INPUT);
	if(micen)
	{
		/* Enable the LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA */
		regval |= 3 << 4 | 3 << 0;
	} else {
		regval &= ~(3 << 4 | 3 << 0);
	}
	wm8978_write_reg(WM8978_REG_INPUT,regval);

	if(lineinen)
	{
		wm8978_linein_gain(5);
	} else {
		wm8978_linein_gain(0);
	}
	if(auxen)
	{
		wm8978_aux_gain(7);
	} else {
		wm8978_aux_gain(0);
	}
}

/**
 * \brief	Config the output block.
 * \param	dacen;Enable D/A block.1,enable.0,disable.
 * \param	bpsen;Enable the out of bypass.1,enable.0,disable.
 */
static void wm8978_output_cfg(uint8_t dacen,uint8_t bpsen)
{
	uint16_t regval = 0;
	if(dacen)
	{
		regval |= 1 << 0;
	}
	if(bpsen)
	{
		regval |= 1 << 1;
		regval |= 5 << 2;
	}
	wm8978_write_reg(WM8978_REG_LMIX,regval);
	wm8978_write_reg(WM8978_REG_RMIX,regval);
}

/**
 * \brief	Config the mode of I2S
 * \param	fmt.The format of I2S operating mode.0,LSB. 1,MSB. 2,Philips. 3,PCM/DSP.
 * \param	len;The length of data.0,16bits.1,20bits.2,24bits.3,32bits.
 */
static void wm8978_i2s_cfg(uint8_t fmt,uint8_t len)
{
	fmt &= 0x03;
	len &= 0x03;
	wm8978_write_reg(WM8978_REG_AUDIO,(fmt << 3) | (len << 5));	//R4,WM8978工作模式设置
}

/**
 * \brief	Config the mode of I2S
 * \param	mono.Config the channels of audio data.0,stereo;1,mono
 * \param	adclrswap;config the data of ADC to
 */
static void wm8978_mono_cfg(uint8_t mono,uint8_t adclrswap)
{
	mono &= 0x01;
	adclrswap &= 0x01;
	wm8978_write_reg(WM8978_REG_AUDIO,(mono << 0) | (adclrswap << 1));	//R4,WM8978工作模式设置
}

/**
 * \brief	Set the volume of headphone.
 * \param	voll.The volume of left channel.0~63.
 * \param	volr;The volume of right channel.0~63.
 */
static void wm8979_hpvol_set(uint8_t voll,uint8_t volr)
{
	voll &= 0x3F;
	volr &= 0x3F;

	/*  if volume is 0,mute the headphone */
	if (voll == 0)
	{
		voll |= 1 << 6;
	}
	if (volr == 0)
	{
		volr |= 1 << 6;
	}
	wm8978_write_reg(WM8978_REG_HPL,voll);
	wm8978_write_reg(WM8978_REG_HPR,volr | (1 << 8));
}

/**
 * \brief	Set the volume of speaker.
 * \param	voll.The volume of speaker.0~63.
 */
static void wm8978_spkvol_set(uint8_t volx)
{
	volx &= 0X3F;
	if (volx == 0)
	{
		volx |= 1 << 6;
	}
 	wm8978_write_reg(WM8978_REG_SPKL,volx);
	wm8978_write_reg(WM8978_REG_SPKR,volx | (1 << 8));
}

/**
 * \brief	Set 3D surround sound
 * \param	depth.0~15.
 */
static void WM8978_3D_Set(uint8_t depth)
{
	depth &= 0XF;
 	wm8978_write_reg(WM8978_REG_3D,depth);
}

/**
 * \brief	Init the IIC device.
 * \retval 	0. Init successfully.
 * \retval	1.Init failed.
 */
uint8_t wm8978_init(WM8978_INIT_STR *init_str)
{
	uint8_t res;

	wm8978_iic_init();

	res = wm8978_write_reg(WM8978_REG_RESET,0);
	if(res != 0) {
		EMBARC_PRINTF("The wm8978 WM8978_REG_RESET failed!\n");
		return 1;
	}

	/*  biase  */
	res = wm8978_write_reg(WM8978_REG_POWER_1,0x1B);
	if(res != 0) {
		EMBARC_PRINTF("The wm8978 WM8978_REG_POWER_1 failed!\n");
		return 1;
	}

	/* Enable boost for right and left channel */
	res = wm8978_write_reg(WM8978_REG_POWER_2,0xb3);
	if(res != 0) {
		EMBARC_PRINTF("The wm8978 WM8978_REG_POWER_2 failed!\n");
		return 1;
	}

	res = wm8978_write_reg(WM8978_REG_POWER_3,0x45);
	if(res != 0) {
		EMBARC_PRINTF("The wm8978 WM8978_REG_POWER_3 failed!\n");
		return 1;
	}
	/* Set the clock */
	wm8978_clk_set(init_str->clk_sel,init_str->mclk);
	/* Set the gain of left mic to be 20db */
	res = wm8978_write_reg(WM8978_REG_LADC,1 << 8);
	if(res != 0) {
		EMBARC_PRINTF("The wm8978 WM8978_REG_LADC failed!\n");
		return 1;
	}

	/* Set the gain of right mic to be 20db*/
	res = wm8978_write_reg(WM8978_REG_RADC,1 << 8);
	if(res != 0) {
		EMBARC_PRINTF("The wm8978 WM8978_REG_RADC failed!\n");
		return 1;
	}

	/* Open overheat protection */
	res = wm8978_write_reg(WM8978_REG_OUT,1 << 1);
	if(res != 0) {
		EMBARC_PRINTF("The wm8978 WM8978_REG_OUT failed!\n");
		return 1;
	}
	/* Set the oversample rate of ADC to be 128fs */
	res = wm8978_write_reg(WM8978_REG_ADC,1 << 3);
	if(res != 0) {
		EMBARC_PRINTF("The wm8978 WM8978_REG_ADC failed!\n");
		return 1;
	}
	/*	Set the format of i2s */
	wm8978_i2s_cfg(init_str->fmt_i2s,init_str->len_i2s);

	if (init_str->recorder_en ==1 )
	{
		wm8978_adda_cfg(1,1);
		wm8978_input_cfg(1,1,0);
		wm8978_mic_gain(init_str -> gain_mic);
		wm8978_i2s_cfg(init_str->fmt_i2s, init_str->len_i2s);
		wm8978_spkvol_set(20);
	}
	//wm8978_output_cfg(1,1);
	return 0;
}

/**
 * \brief	Init the WM8978 according to parameters
 */
uint8_t wm8978_init_func(void)
{
	WM8978_INIT_STR init_str;
	init_str.recorder_en = WM8978_RECORDER_EN;
	init_str.player_en = WM8978_PLAYER_EN;
	init_str.out_dev = WM8978_OUT_DEV;
	init_str.clk_sel = WM8978_CLK_SEL;
	init_str.gain_mic = WM8978_MIC_GAIN;
	init_str.mclk = WM8978_MCLK_1536FS;
	init_str.sample_rate = 16;
	init_str.volume_dev = 0;
	init_str.fmt_i2s = WM8978_FMT_I2S_PHI;
	init_str.len_i2s = WM8978_LEN_I2S_16;

	return wm8978_init(&init_str);
}