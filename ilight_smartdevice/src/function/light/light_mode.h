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
 * \version 2017.09
 * \date 2017-09-27
 * \author dbHu(wwmhu@outlook.com)
--------------------------------------------- */
/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief	light control mode related header file
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */

#ifndef LIGHT_MODE_H_
#define LIGHT_MODE_H_

/*!< Light mode definition */
#define LIGHT_MODE_CLOCK		0x10
#define LIGHT_MODE_RUNNING 		0x11
#define LIGHT_MODE_ALARM 		0X12
#define LIGHT_MODE_RIDING		0x13
#define LIGHT_MODE_WARM_LIGHTING	0x14
#define LIGHT_MODE_CO_LIGHTING		0x15
#define LIGHT_MODE_TIMING_FIT		0x16
#define LIGHT_MODE_TIMING_LAND		0x17
#define LIGHT_MODE_COMFOR 		0x18
#define LIGHT_MODE_MUSIC  		0x19
#define LIGHT_MODE_ANIMATION 		0x1a
#define LIGHT_MODE_WEATHER 		0X1b
#define LIGHT_MODE_SHAKING 		0x1c
#define LIGHT_MODE_SOS			0x1d

/*!< Frequency definition */
#define FREQUENCY_SLOW 			4
#define FREQUENCY_NORMAL		3
#define FREQUENCY_FAST   		2
#define FREQUENCY_VERY_FAST		1

extern void light_mode_test(void);

extern void light_ctr_init(void);

extern void light_mode_alloff(void);

extern void light_mode_change_success(void);

extern void light_mode_music(uint8_t *fftOut);

extern void light_mode_change_flag(uint8_t mode);

extern void light_mode_top_pack(uint8_t mode,uint8_t bri,uint8_t freq,uint8_t time,uint8_t *string,uint8_t weather);

#endif  /* _LIGHT_MODE_H_ */

/** @} end of group EMBARC_APP_FREERTOS_ILIGHT_SMARTDEVICE */