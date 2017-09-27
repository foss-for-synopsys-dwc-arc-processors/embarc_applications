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
 * \ingroup	EMBARC_APP_FREERTOS_ILIGHT_SMARTDEVICE
 * \brief	light control mode for ilight
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_ILIGHT_SMARTDEVICE
 * @{
 */

/* standard C HAL */
#include "stdlib.h"
#include "math.h"
#include "word.h"

/* custom HAL */
#include "light_mode.h"
#include "light_driver.h"
#include "imu.h"
#include "mic.h"
#include "task.h"

#define LIGHT_RIDE_ACC_SUM		(960)

#define DEFAULT_BRI_DIM			(0x10)
#define DEFAULT_BRI_LIGHT		(0x26)

#define LIGHT_MASK_ALL_MIDDLE		(0xffff)
#define LIGHT_MASK_ALL_SIDE1		(0x3ffffffff)
#define LIGHT_MASK_ALL_SIDE2		(0x1fffcfffe)
#define LIGHT_MASK_ALL_SIDE3		(0x3fffcffff)
#define LIGHT_MASK_ALL_SUP1		(0x00000fffe)
#define LIGHT_MASK_ALL_SDW1		(0x1fffe0000)
#define LIGHT_MASK_MIDDLE_M1		(0x0080)      //The mask of middle of the row of middle.
#define LIGHT_MASK_MIDDLE_M2		(0x0100)
#define LIGHT_MASK_MIDDLE_FST		(0x0001)
#define LIGHT_MASK_MIDDLE_LST		(0x8000)
#define LIGHT_MASK_MIDDLE_12		(0x9249)
#define LIGHT_MASK_MIDDLE_21		(~LIGHT_MASK_MIDDLE_12)
#define LIGHT_MASK_MIDDLE_11		(0x5555)
#define LIGHT_MASK_SIDE_UPM		(0x000000100) //The mask of middle of the row of up sides.
#define LIGHT_MASK_SIDE_DWM		(0x002000000)
#define LIGHT_MASK_SIDE_MID		(0x002000100)
#define LIGHT_MASK_SIDE_UP 		(0x00001ffff)
#define LIGHT_MASK_SIDE_DW 		(0x3fffe0000)
#define LIGHT_MASK_SIDE_UPLST		(0x000010000)
#define LIGHT_MASK_SIDE_DWLST		(0x000020000)
#define LIGHT_MASK_SIDE_UPFST		(0x000000001)
#define LIGHT_MASK_SIDE_DWFST		(0x200000000)
#define LIGHT_MASK_SIDE_21		(0x36db7b6db)
#define LIGHT_MASK_SIDE_12		(~LIGHT_MASK_SIDE_21)

#define LIGHT_MASK_SIDE_UPHALF1		(0x0000000ff)
#define LIGHT_MASK_SIDE_UPHALF2		(0x00001fe00)
#define LIGHT_MASK_SIDE_DWHALF1		(0x001fe0000)
#define LIGHT_MASK_SIDE_DWHALF2		(0x3fc000000)

#define LIGHT_COLOR_OFF 		(0x000000)

#define LIGHT_MODE_RUN_MBLU1		(0x4a52)
#define LIGHT_MODE_RUN_MBLU2		(0x318c)
#define LIGHT_MODE_RUN_SBLU1		(0x0a5285294)
#define LIGHT_MODE_RUN_SBLU2		(0x042102108)
#define LIGHT_MODE_RUN_MWHT		(0x8421)
#define LIGHT_MODE_RUN_SWHT		(0x318c78c63)

#define LIGHT_MODE_ALARM_MBLU		(0x00f0)
#define LIGHT_MODE_ALARM_MRED		(0x0f00)
#define LIGHT_MODE_ALARM_SRED1		(0x0e00)
#define LIGHT_MODE_ALARM_SRED2		(0x1c00000)
#define LIGHT_MODE_ALARM_SBLU1		(0x00e0)
#define LIGHT_MODE_ALARM_SBLU2		(0x01c000000)

#define LIGHT_MODE_SHA_MUX		(0x06)

#define LIGHT_MODE_TIMING_UNIT		(3740)

#define LIGHT_MODE_MUS_NUM		(8)
#define LIGHT_MODE_MUS_UNIT_JUD		(6)

#define LIGHT_MODE_WEATHER_SUN		1	//sunny and cloudy
#define LIGHT_MODE_WEATHER_WINDY	2	//windy
#define LIGHT_MODE_WEATHER_RAIN		3	//rain

#define LIGHT_MASK_MFIRST_1		0x0001
#define LIGHT_MASK_MFIRST_2		0x0010
#define LIGHT_MASK_MFIRST_3		0x0100
#define LIGHT_MASK_MFIRST_4		0x0001

/* ride mode threshold settings */
#define RIDE_STOP_THRESHOLD		-150
#define RIDE_START_THRESHOLD		100
#define RIDE_Z_ACC_JUDGE		200
#define RIDE_Y_FILTER_FACTOR		0.3

#define PI				3.1416

#define LIGHT_UNIT_BRI			(0x04)

/**
 * \brief	Init those Pmods which is used to control lights.
 */
void light_ctr_init()
{
	light_ctr_gpio_init();
}

/**
 * \brief	Turn off all the lights.
 * \details	Send 0x00,0x00,0x00 to all the lights to turn off lights.
 */
void light_mode_alloff()
{
	light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);
	light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
	vTaskDelay(1);
}

/**
 * \brief	Running mode.
 * \details 	The blue light is on and the white light flashes.
 * \param	freq	The frequency of  white flashing, One unit represents 500ms.
 * \param	bri	The brightness of lights,0~16.
 */
void light_mode_run(uint8_t freq, uint8_t bri)
{
	uint8_t i;
	uint8_t brightness = LIGHT_UNIT_BRI * bri;

	light_mode_alloff();
	light_ctr_rgb_update(LIGHT_MODE_RUN_MBLU1, LIGHT_ROW_MIDDLE, 0x04, 0x1b, 0x2a);
	light_ctr_rgb(LIGHT_MODE_RUN_MBLU2, LIGHT_ROW_MIDDLE, 0x06, 0x44, 0x5a);
	light_ctr_rgb_update(LIGHT_MODE_RUN_SBLU1, LIGHT_ROW_SIDE, 0x02, 0x0d, 0x15);
	light_ctr_rgb(LIGHT_MODE_RUN_SBLU2, LIGHT_ROW_SIDE, 0x03, 0x22, 0x2d);

	for (i = 0; i < brightness / 2; i++) {
		light_ctr_rgb(LIGHT_MODE_RUN_MWHT, LIGHT_ROW_MIDDLE, 2 * i, 2 * i, 2 * i);
		light_ctr_rgb(LIGHT_MODE_RUN_SWHT, LIGHT_ROW_SIDE, 2 * i, 2 * i, 2 * i);
		vTaskDelay(1);
	}

	for (i = 0; i < brightness / 4; i++) {
		light_ctr_rgb(LIGHT_MODE_RUN_MWHT, LIGHT_ROW_MIDDLE, brightness - 4 * i, brightness - 4 * i, brightness - 4 * i);
		light_ctr_rgb(LIGHT_MODE_RUN_SWHT, LIGHT_ROW_SIDE, brightness - 4 * i, brightness - 4 * i, brightness - 4 * i);
		vTaskDelay(4);
	}

	vTaskDelay(200 * freq);
}

/**
 * \brief	Light in warm.
 * \param	bri. The brightness of lights.
 */
void light_mode_lighting_warm(uint8_t bri)
{
	light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0x03 * bri, 0x04 * bri, 0x01 * bri);
	light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, 0x03 * bri, 0x04 * bri, 0x01 * bri);
}

/**
 * \brief	Light in cold.
 * \param 	bri	The brightness of lights.
 */
void light_mode_lighting_cold(uint8_t bri)
{
	light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0x03 * bri, 0x04 * bri, 0x04 * bri);
	light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, 0x03 * bri, 0x04 * bri, 0x04 * bri);
}

/**
 * \brief	Light in different weather
 * \details 	different weather with different led graphic & led color.
 * \param	weather sunny,rain,windy.
 */
void light_mode_weather(uint8_t weather)
{
	uint8_t i, j, k;
	light_mode_alloff();

	if (weather == LIGHT_MODE_WEATHER_RAIN) {
		light_ctr_rgb(0xffff, LIGHT_ROW_MIDDLE, 0x04, 0x06, 0x06);

		for (i = 0; i < 4; i++) {
			light_ctr_rgb_update((0x000006186 << i) & 0x00000fffe, LIGHT_ROW_SIDE, 0x04, 0x12, 0x12);
			light_ctr_rgb_update((0x000002082 << i) >> 1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update((0x186180000 >> i) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x04, 0x12, 0x12);
			light_ctr_rgb((0x104100000 >> i) << 1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update(0x5145 << i, LIGHT_ROW_MIDDLE, 0x04, 0x12, 0x12);
			light_ctr_rgb_update((0x5145 << i) >> 1, LIGHT_ROW_MIDDLE, 0x04, 0x06, 0x06);
			light_ctr_rgb(0x2082 << i, LIGHT_ROW_MIDDLE, 0x1f, 0x3f, 0x3f);
			vTaskDelay(100);
		}

		for (i = 2; i > 0; i--) {
			light_ctr_rgb_update((0x000006186 >> i) & 0x00000fffe, LIGHT_ROW_SIDE, 0x04, 0x12, 0x12);
			light_ctr_rgb_update((0x000002082 >> i) >> 1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update((0x186180000 << i) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x04, 0x12, 0x12);
			light_ctr_rgb((0x104100000 << i) << 1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update(0x5145 >> i, LIGHT_ROW_MIDDLE, 0x04, 0x12, 0x12);
			light_ctr_rgb_update((0x5145 >> i) >> 1, LIGHT_ROW_MIDDLE, 0x04, 0x06, 0x06);
			light_ctr_rgb(0x2082 >> i, LIGHT_ROW_MIDDLE, 0x1f, 0x3f, 0x3f);
			vTaskDelay(100);
		}
	} else if (weather == LIGHT_MODE_WEATHER_SUN) {
		for (i = 0; i < 3; i++) {
			light_ctr_rgb_update(0x0080 >> i, LIGHT_ROW_MIDDLE, 0x3f - i * 6, 0x3f - i * 6, 4 * i);
			light_ctr_rgb_update(0x0100 << i, LIGHT_ROW_MIDDLE, 0x3f - i * 6, 0x3f - i * 6, 4 * i);
		}

		light_ctr_rgb_update(0x002000100, LIGHT_ROW_SIDE, 0x1f, 0x1f, 0x00);
		light_ctr_rgb_update(0x005000280, LIGHT_ROW_SIDE, 0x1b, 0x1b, 0x4);
		light_ctr_rgb_update(0x20201010, LIGHT_ROW_SIDE, 0x1f, 0x24, 0x24);
		light_ctr_rgb_update(0x40102008, LIGHT_ROW_SIDE, 0x06, 0x09, 0x09);
		light_ctr_rgb(0x008800440, LIGHT_ROW_SIDE, 0x19, 0x19, 0x08);
		light_ctr_rgb_update(0x1008, LIGHT_ROW_MIDDLE, 0x1f, 0x24, 0x24);
		light_ctr_rgb_update(0x2004, LIGHT_ROW_MIDDLE, 0x18, 0x1f, 0x1f);
		light_ctr_rgb_update(0x4002, LIGHT_ROW_MIDDLE, 0x10, 0x15, 0x15);
		light_ctr_rgb(0x8001, LIGHT_ROW_MIDDLE, 0x8, 0xb, 0xb);
		vTaskDelay(100);
	} else {
		for (i = 0; i < 3; i++) {
			light_ctr_rgb_update(0x0080 >> i, LIGHT_ROW_MIDDLE, 0x3f - i * 6, 0x4f - i * 6, 0x4f - i * 6);
			light_ctr_rgb_update(0x0100 << i, LIGHT_ROW_MIDDLE, 0x3f - i * 6, 0x4f - i * 6, 0x4f - i * 6);
		}

		light_ctr_rgb_update(0x002000100, LIGHT_ROW_SIDE, 0x1a, 0x1f, 0x1f);
		light_ctr_rgb_update(0x005000280, LIGHT_ROW_SIDE, 0x14, 0x18, 0x18);
		light_ctr_rgb_update(0x20201010, LIGHT_ROW_SIDE, 0x10, 0x14, 0x14);
		light_ctr_rgb_update(0x40102008, LIGHT_ROW_SIDE, 0x06, 0x09, 0x09);
		light_ctr_rgb(0x008800440, LIGHT_ROW_SIDE, 0x06, 0x09, 0x09);
		light_ctr_rgb_update(0x1008, LIGHT_ROW_MIDDLE, 0x1f, 0x24, 0x24);
		light_ctr_rgb_update(0x2004, LIGHT_ROW_MIDDLE, 0x18, 0x1f, 0x1f);
		light_ctr_rgb_update(0x4002, LIGHT_ROW_MIDDLE, 0x10, 0x15, 0x15);
		light_ctr_rgb(0x8001, LIGHT_ROW_MIDDLE, 0x8, 0xb, 0xb);
		vTaskDelay(100);
	}
}

/**
 * \brief	Alarm mode.
 * \details	The red and blue lights move to both sides.
 * \param 	freq The speed of lights moving.One unit represents 20ms.
 * \param 	bri The brightness of lights.
 */
void light_mode_alarm(uint8_t freq, uint8_t bri)
{
	uint8_t i;
	uint64_t mask_dw_side;

	for (i = 0; i < 4; i++ ) {
		light_ctr_rgb_update(LIGHT_MASK_MIDDLE_M1 >> i, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x04 * bri);
		light_ctr_rgb(LIGHT_MASK_MIDDLE_M2 << i, LIGHT_ROW_MIDDLE, 0x04 * bri, 0x00, 0x00);

		light_ctr_rgb_update((LIGHT_MASK_SIDE_UPM << i) & LIGHT_MASK_SIDE_UPHALF2, LIGHT_ROW_SIDE, 0x02 * bri, 0x00, 0x00);
		light_ctr_rgb_update((LIGHT_MASK_SIDE_DWM >> i) & LIGHT_MASK_SIDE_DWHALF1, LIGHT_ROW_SIDE, 0x02 * bri, 0x00, 0x00);
		light_ctr_rgb_update((LIGHT_MASK_SIDE_UPM >> i) & LIGHT_MASK_SIDE_UPHALF1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x02 * bri);
		light_ctr_rgb((LIGHT_MASK_SIDE_DWM << i) & LIGHT_MASK_SIDE_DWHALF2, LIGHT_ROW_SIDE, 0x00, 0x00, 0x02 * bri);
		vTaskDelay(20 * freq);
	}

	for (i = 1; i < 3; i++ ) {
		light_mode_alloff();
		light_ctr_rgb_update(LIGHT_MODE_ALARM_MBLU >> i, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x04 * bri);
		light_ctr_rgb(LIGHT_MODE_ALARM_MRED << i, LIGHT_ROW_MIDDLE, 0x04 * bri, 0x00, 0x00);

		light_ctr_rgb_update(LIGHT_MODE_ALARM_SRED1 << i, LIGHT_ROW_SIDE, 0x02 * bri, 0x00, 0x00);
		light_ctr_rgb_update(LIGHT_MODE_ALARM_SBLU1 >> i, LIGHT_ROW_SIDE, 0x00, 0x00, 0x02 * bri);
		light_ctr_rgb_update(LIGHT_MODE_ALARM_SRED2 >> i, LIGHT_ROW_SIDE, 0x02 * bri, 0x00, 0x00);
		light_ctr_rgb(LIGHT_MODE_ALARM_SBLU2 << i, LIGHT_ROW_SIDE, 0x00, 0x00, 0x02 * bri);
		vTaskDelay(20 * freq);
	}

	light_mode_alloff();
	light_ctr_rgb_update(LIGHT_MODE_ALARM_MBLU >> 3, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x04 * bri);
	light_ctr_rgb(LIGHT_MODE_ALARM_MRED << 3, LIGHT_ROW_MIDDLE, 0x04 * bri, 0x00, 0x00);

	light_ctr_rgb_update(LIGHT_MODE_ALARM_SRED2 >> 3, LIGHT_ROW_SIDE, 0x02 * bri, 0x00, 0x00);
	light_ctr_rgb_update(LIGHT_MODE_ALARM_SRED1 << 3, LIGHT_ROW_SIDE, 0x02 * bri, 0x00, 0x00);
	light_ctr_rgb_update(LIGHT_MODE_ALARM_SBLU1 >> 3, LIGHT_ROW_SIDE, 0x00, 0x00, 0x02 * bri);
	light_ctr_rgb(0xe0000000, LIGHT_ROW_SIDE, 0x00, 0x00, 0x02 * bri);
	vTaskDelay(20 * freq);

	for (i = 0; i < 5; i++) {
		light_mode_alloff();

		light_ctr_rgb_update(LIGHT_MODE_ALARM_MBLU >> (i + 4), LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x04 * bri);
		light_ctr_rgb(LIGHT_MODE_ALARM_MRED << (i + 4), LIGHT_ROW_MIDDLE, 0x04 * bri, 0x00, 0x00);

		light_ctr_rgb_update((LIGHT_MODE_ALARM_SRED2 >> (i + 4)) & 0x3fffe0000, LIGHT_ROW_SIDE, 0x02 * bri, 0x00, 0x00);
		light_ctr_rgb_update((LIGHT_MODE_ALARM_SRED1 << (i + 4)) & 0x1ffff, LIGHT_ROW_SIDE, 0x02 * bri, 0x00, 0x00);
		light_ctr_rgb_update(LIGHT_MODE_ALARM_SBLU1 >> (i + 4), LIGHT_ROW_SIDE, 0x00, 0x00, 0x02 * bri);
		light_ctr_rgb(LIGHT_MODE_ALARM_SBLU2 << (i + 4), LIGHT_ROW_SIDE, 0x00, 0x00, 0x02 * bri);
		vTaskDelay(20 * freq);
	}
}

/**
 * \brief	all lights flash in blue and red.
 * \param	freq	The speed of lights moving.One unit represents 300ms.
 */
void light_change_alldouble(uint8_t freq)
{
	uint8_t i;
	light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0x5f, 0x00, 0x3f);
	light_ctr_rgb(0x1fffcfffe, LIGHT_ROW_SIDE, 0x2f, 0x00, 0x1f);
	vTaskDelay(300 * freq);
	light_mode_alloff();
	vTaskDelay(1);
	light_ctr_rgb(0x1fffcfffe, LIGHT_ROW_SIDE, 0x2f, 0x1f, 0x00);
	light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0x5f, 0x3f, 0x00);
	vTaskDelay(300 * freq);
}

/**
 * \brief	Ride mode.
 * \details	Distinguish acceleration & deceleration from uniform velocity.
 * \param 	freq The speed of lights moving.One unit represents 20ms.
 * \param 	bri The brightness of lights.
 */
void light_mode_ride(uint8_t freq, uint8_t bri)
{
	uint8_t i;
	float y_radian, z_radian;
	uint8_t brightness = bri * 0x04;
	int16_t y_cos, z_cos, z_judge_acc, z_acc_radian;
	static int16_t old_y_acc = 0, old_z_acc = LIGHT_RIDE_ACC_SUM, y_acc;
	light_ctr_rgb_update(LIGHT_MASK_SIDE_MID, LIGHT_ROW_SIDE, 0xff, 0xff, 0xff);
	light_ctr_rgb_update(0x0180, LIGHT_ROW_MIDDLE, 0xff, 0xff, 0xff);

	/* Light beats from the middle to both sides in green gradually */
	for (i = 1; i < (LIGHT_NUM_MIDDLE / 2); i++) {
		light_ctr_rgb_update(LIGHT_MASK_MIDDLE_M1 >> i, LIGHT_ROW_MIDDLE, 0x00, brightness, bri);
		light_ctr_rgb_update((0x000000100 >> i) & 0x0000000fe, LIGHT_ROW_SIDE, 0x00, brightness, bri);
		light_ctr_rgb_update((0x002000000 >> i) & 0x001fc0000, LIGHT_ROW_SIDE, 0x00, brightness, bri);
		light_ctr_rgb_update((0x000000100 << i) & 0x00000fe00, LIGHT_ROW_SIDE, 0x00, brightness, bri);
		light_ctr_rgb((light_ctr_mask_lshift(0x002000000, i)) & 0x1fc000000, LIGHT_ROW_SIDE, 0x00, brightness, bri);
		light_ctr_rgb(LIGHT_MASK_MIDDLE_M2 << i, LIGHT_ROW_MIDDLE, 0x00, brightness, bri);
		vTaskDelay(30);
	}

	light_ctr_rgb(0x200030001, LIGHT_ROW_SIDE, 0xff, 0xff, 0xff);
	vTaskDelay(40);
	light_ctr_rgb(0x200030001, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);

	/* Light beats from the middle to both sides in yellow gradually */
	for (i = 1; i < (LIGHT_NUM_MIDDLE / 2); i++) {
		light_ctr_rgb_update(LIGHT_MASK_MIDDLE_M1 >> i, LIGHT_ROW_MIDDLE, brightness, brightness, bri / 4);
		light_ctr_rgb_update((0x000000100 >> i) & 0x0000000fe, LIGHT_ROW_SIDE, brightness, brightness, bri / 4);
		light_ctr_rgb_update((0x002000000 >> i) & 0x001fc0000, LIGHT_ROW_SIDE, brightness, brightness, bri / 4);
		light_ctr_rgb_update((0x000000100 << i) & 0x00000fe00, LIGHT_ROW_SIDE, brightness, brightness, bri / 4);
		light_ctr_rgb((light_ctr_mask_lshift(0x002000000, i)) & 0x1fc000000, LIGHT_ROW_SIDE, brightness, brightness, bri / 4);
		light_ctr_rgb(LIGHT_MASK_MIDDLE_M2 << i, LIGHT_ROW_MIDDLE, brightness, brightness, bri / 4);
		vTaskDelay(30);
	}

	/* Calculate the cosine of the Y axis */
	y_radian = (float)y_acc / (float)LIGHT_RIDE_ACC_SUM;

	/* Calculate the angle between the Y axis and the acceleration of gravity */
	y_cos = acos(y_radian) * 180 / PI;

	/* According to the angle between the Y axis and the acceleration of gravity,caculate the angle of Y axis */
	z_cos = 90 - y_cos;

	/* Caculate the cosine of the Y axis */
	z_radian = cos(z_cos * PI / 180);

	/* Increase the Z-axis cosine by a factor of 1000 */
	z_acc_radian = z_radian * 1000;

	/* Find the actual acceleration on the Z axis */
	z_judge_acc = abs(imu_mpu6050_update_ptr -> acc_z) - z_acc_radian;

	if ((imu_mpu6050_update_ptr -> acc_z < (-RIDE_STOP_THRESHOLD + z_acc_radian)) && z_judge_acc > RIDE_Z_ACC_JUDGE) {

		for (i = 0; i < (LIGHT_NUM_MIDDLE / 2); i++) {
			light_ctr_rgb_update(0x200000000 >> i, LIGHT_ROW_SIDE, brightness / 4, 0x00, 0x00);
			light_ctr_rgb_update(0x000000001 << i, LIGHT_ROW_SIDE, brightness / 4, 0x00, 0x00);
			light_ctr_rgb_update((LIGHT_MASK_MIDDLE_FST << i) & 0x7f, LIGHT_ROW_MIDDLE, brightness, 0x00, 0x00);
			light_ctr_rgb_update(0x000010000 >> i, LIGHT_ROW_SIDE, brightness / 4, 0x00, 0x00);
			light_ctr_rgb(0x000020000 << i, LIGHT_ROW_SIDE, brightness / 4, 0x00, 0x00);
			light_ctr_rgb((LIGHT_MASK_MIDDLE_LST >> i) & 0xfe00, LIGHT_ROW_MIDDLE, brightness, 0x00, 0x00);
			vTaskDelay(20);
		}

		while (1) {
			vTaskDelay(20);

			/*  Low-pass filtering of Y-axis acceleration,the factor of low-pass filtering */
			y_acc = (imu_mpu6050_update_ptr -> acc_y * RIDE_Y_FILTER_FACTOR) + (old_y_acc * (1 - RIDE_Y_FILTER_FACTOR));

			/*  Calculate the cosine of the Y axis */
			y_radian = (float)y_acc / (float)LIGHT_RIDE_ACC_SUM;

			/*  Calculate the angle between the Y axis and the acceleration of gravity */
			y_cos = acos(y_radian) * 180 / PI;

			/*  According to the angle between the Y axis and the acceleration of gravity,caculate the angle of Y axis */
			z_cos = 90 - y_cos;

			/*  Caculate the cosine of the Y axis */
			z_radian = cos(z_cos * PI / 180);

			/*  Increase the Z-axis cosine by a factor of 1000 */
			z_acc_radian = z_radian * 1000;

			/*  Find the actual acceleration on the Z axis */
			z_judge_acc = abs(imu_mpu6050_update_ptr -> acc_z) - z_acc_radian;

			old_y_acc = y_acc;

			if ((imu_mpu6050_update_ptr -> acc_z > (RIDE_START_THRESHOLD - z_acc_radian)) && z_judge_acc > RIDE_Z_ACC_JUDGE) {
				break;
			}
		}
	}
	old_y_acc = y_acc;
}

/**
 * \brief	Countdown time mode.
 * \details	Increase the number of lights with the time going by.
 * \param	time the duration of countdown.
 */
void light_mode_timing_land(uint32_t time)
{
	uint16_t j;
	uint8_t k, i;
	uint64_t dir_flag = 0, num = 20;
	vTaskDelay(1);

	light_mode_alloff();

	while (imu_get_action_shaking() != ACTION_SHAKING) {
		if ((dir_flag == 0) && (num >= 20)) {
			num = 0;
			dir_flag = 1;
			light_mode_alloff();
			light_ctr_rgb(LIGHT_MASK_SIDE_21, LIGHT_ROW_SIDE, 0x00, 0x28, 0x18);
			light_ctr_rgb(LIGHT_MASK_MIDDLE_12, LIGHT_ROW_MIDDLE, 0x00, 0x4f, 0x2f);
		}

		if ((dir_flag == 1) && (num >= 20)) {
			num = 0;
			dir_flag = 0;
			light_mode_alloff();
			light_ctr_rgb(LIGHT_MASK_SIDE_12, LIGHT_ROW_SIDE, 0x18, 0x1f, 0x0a);
			light_ctr_rgb(LIGHT_MASK_MIDDLE_21, LIGHT_ROW_MIDDLE, 0x2f, 0x3f, 0x14);
		}

		EMBARC_PRINTF("FITING WHILE!\r\n");
		num++;
		vTaskDelay(40);
	}

	light_mode_alloff();
	light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0x06, 0x0f, 0x08);
	vTaskDelay(1);

	for (k = 0; k < 16; k++) {
		vTaskDelay(LIGHT_MODE_TIMING_UNIT * time - (16 - k) * 50 - 3);
		/* The execution time of this code is 150ms */
		for (i = 0; i < 16 - k; i++) {
			light_ctr_rgb_update((0x000018000 >> i) << 1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update((light_ctr_mask_lshift(0x000060000, i)) >> 1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update(0x000018000 >> i, LIGHT_ROW_SIDE, 0x1a + k * 0x0a, 0x25, 0x1a);
			light_ctr_rgb((light_ctr_mask_lshift(0x000060000, i)), LIGHT_ROW_SIDE, 0x1a + k * 0x0a, 0x25, 0x1a);
			light_ctr_rgb_update(0x8000 >> i, LIGHT_ROW_MIDDLE, 0x3a + k * 0x0c, 0x4a, 0x3a);
			light_ctr_rgb((0x8000 >> i) << 1, LIGHT_ROW_MIDDLE, 0x06, 0x0f, 0x08);
			vTaskDelay(50);
		}

		light_ctr_rgb(0x0001 << k, LIGHT_ROW_MIDDLE, 0x3a + k * 0x0c, 0x4a, 0x3a);
		light_ctr_rgb_update(0x000000001 << k, LIGHT_ROW_SIDE, 0x1a + k * 0x0a, 0x25, 0x1a);
		light_ctr_rgb(0x200000000 >> k, LIGHT_ROW_SIDE, 0x1a + k * 0x0a, 0x25, 0x1a);
	}
	vTaskDelay(50);

	/*  After the completion of the time , flash all those lights in blue and red */
	while (1) {
		light_change_alldouble(1);
	}
}

/**
 * \brief	Countdown time mode.
 * \details	Decrease the number of lights with the time going by.
 * \param	time the duration of countdown.
 */
void light_mode_fitness(uint32_t time)
{
	uint8_t i;
	uint64_t dir_flag = 0, num = 20;
	light_mode_alloff();

	while (imu_get_action_shaking() != ACTION_SHAKING) {
		if ((dir_flag == 0) && (num >= 20)) {
			num = 0;
			dir_flag = 1;
			light_mode_alloff();
			light_ctr_rgb(LIGHT_MASK_SIDE_21, LIGHT_ROW_SIDE, 0x00, 0x28, 0x18);
			light_ctr_rgb(LIGHT_MASK_MIDDLE_12, LIGHT_ROW_MIDDLE, 0x00, 0x4f, 0x2f);
		}

		if ((dir_flag == 1) && (num >= 20)) {
			num = 0;
			dir_flag = 0;
			light_mode_alloff();
			light_ctr_rgb(LIGHT_MASK_SIDE_12, LIGHT_ROW_SIDE, 0x18, 0x1f, 0x0a);
			light_ctr_rgb(LIGHT_MASK_MIDDLE_21, LIGHT_ROW_MIDDLE, 0x2f, 0x3f, 0x14);
		}

		EMBARC_PRINTF("FITING WHILE!\r\n");
		num++;
		vTaskDelay(40);
	}

	light_mode_alloff();
	/* Light all the lights in green */
	vTaskDelay(1);
	light_ctr_rgb(LIGHT_MASK_ALL_SIDE3, LIGHT_ROW_SIDE, 0x00, 0x28, 0x18);
	light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0x00, 0x4f, 0x2f);
	vTaskDelay(LIGHT_MODE_TIMING_UNIT * time);
	light_ctr_rgb_update(LIGHT_MASK_SIDE_DWLST << 1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
	light_ctr_rgb(LIGHT_MASK_SIDE_UPLST >> 1, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
	light_ctr_rgb(LIGHT_MASK_MIDDLE_LST, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);

	for (i = 1; i < LIGHT_NUM_MIDDLE; i++) {
		vTaskDelay(LIGHT_MODE_TIMING_UNIT * time);
		/* Turn off specify lights */
		light_ctr_rgb(LIGHT_MASK_MIDDLE_LST >> i, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);
		light_ctr_rgb_update(LIGHT_MASK_SIDE_DWLST << (i + 1), LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
		light_ctr_rgb(LIGHT_MASK_SIDE_UPLST >> (i + 1), LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
	}

	/*  After the completion of the time , flash all those lights in blue and red */
	while (1) {
		light_change_alldouble(1);
	}
}

/**
 * \brief	Music mode.
 * \details	LED flash with music rhythm
 * \param 	fftout 		Audio Signal after fft transform
 */
void light_mode_music(uint8_t *fftOut)
{
	static int16_t hsound_amplitude;
	static uint8_t hstall, i, hold_stall, bri = 1;
	static uint8_t range = 1, rgb_green = 1, rgb_red = 1, rgb_blue = 1;
	static uint8_t range_flag_up, range_flag_down, cnt_flag_up, cnt_flag_down, clolor_flag_change = 0;

	/* Get the music amplitude feature */
	hsound_amplitude = fftOut[2] * 10;
	EMBARC_PRINTF("The vol num :%d\r\n", hsound_amplitude);

	/* The range of lighting is obtained based on the volume amplitude */
	for (i = LIGHT_MODE_MUS_NUM; i >= 1; i--) {
		/*  Volume range judgment,threshold is the unit multiplied by the range and gear */
		if (hsound_amplitude > LIGHT_MODE_MUS_UNIT_JUD * i * range) {
			hstall = i;
			/**
			 * If the gear is greater than eleven and the num between the last 11 stalls of the sampling interval is less than 5
			 * Function: Increase the num of range_flag_up
			 */
			if ((i >= (LIGHT_MODE_MUS_NUM - 1)) && (cnt_flag_up <= 5)) {
				range_flag_up++;
				cnt_flag_up = 0;
			} else if ((i >= (LIGHT_MODE_MUS_NUM - 1)) && (cnt_flag_up > 5)) {
				range_flag_up = 0;
				cnt_flag_up = 0;
			}

			/**
			 * If the gear is less than 4 and the num between the last 4 stalls of the sampling interval is less than 5
			 * Function: Increase the num of range_flag_down
			 */
			if ((i <= 4) && (cnt_flag_down <= 5)) {
				range_flag_down++;
				cnt_flag_down = 0;
			} else if ((i <= 4) && (cnt_flag_up > 5)) {
				range_flag_down = 0;
				cnt_flag_down = 0;
				clolor_flag_change = 1;
			}
			break;
		} else if (hsound_amplitude < 6 * range) {
			hstall = 0;
			if (cnt_flag_up > 4) {
				range_flag_down++;
				cnt_flag_up = 0;
			}
			break;
		}
	}

	/* According to the num of range_flag_up,increase range */
	if (range_flag_up >= 3) {
		range++;
		range_flag_up = 0;
		/* If the range changes, change the light color randly */
		if (clolor_flag_change != 0) {
			rgb_green = (rand() % (0x1f - 0 + 1)) + 0;
			rgb_red = (rand() % (0x2f - 0 + 1)) + 0;
			rgb_blue = (rand() % (0x1f - 0 + 1)) + 0;
		}
	}

	/* According to the num of range_flag_up,discrease range */
	if (range >= 5) {
		if (range_flag_down >= 5) {
			range--;
			range_flag_down = 0;
		}
	}
	cnt_flag_up++;
	cnt_flag_down++;

	/* If old stall is greater than the new stall,turn turn off all the lights that bigger than the new stall */
	if (hold_stall > hstall) {
		switch (hstall) {
		case 1:
			light_ctr_rgb_update(0x0240, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update(0x005000280, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
		case 2:
			light_ctr_rgb_update(0x0420, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update(0x008800440, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
		case 3:
			light_ctr_rgb_update(0x0810, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update(0x010400820, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
		case 4:
			light_ctr_rgb_update(0x1008, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update(0x020201010, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
		case 5:
			light_ctr_rgb_update(0x2004, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update(0x040102008, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
		case 6:
			light_ctr_rgb_update(0x4002, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);
			light_ctr_rgb_update(0x080084004, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
		case 7:
			light_ctr_rgb(0x8001, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x00);
			light_ctr_rgb(0x100048002, LIGHT_ROW_SIDE, 0x00, 0x00, 0x00);
			break;
		default:
			light_mode_alloff();
			break;
		}
	} else {
		/* If old stall is less than the new stall,turn on all the lights that bigger than the new stall */
		switch (hstall) {
		case 8:
			light_ctr_rgb_update(0x8001, LIGHT_ROW_MIDDLE, 0x08 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
			light_ctr_rgb_update(0x100048002, LIGHT_ROW_SIDE, 0x04 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
		case 7:
			light_ctr_rgb_update(0x4002, LIGHT_ROW_MIDDLE, 0x08 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
			light_ctr_rgb_update(0x080084004, LIGHT_ROW_SIDE, 0x04 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
		case 6:
			light_ctr_rgb_update(0x2004, LIGHT_ROW_MIDDLE, 0x07 * bri * rgb_red, rgb_green * bri, 0x02 * bri);
			light_ctr_rgb_update(0x040102008, LIGHT_ROW_SIDE, 0x04 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
		case 5:
			light_ctr_rgb_update(0x1008, LIGHT_ROW_MIDDLE, 0x07 * bri * rgb_red, rgb_green * bri, 0x02 * bri);
			light_ctr_rgb_update(0x020201010, LIGHT_ROW_SIDE, 0x04 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
		case 4:
			light_ctr_rgb_update(0x0810, LIGHT_ROW_MIDDLE, 0x06 * bri * rgb_red, rgb_green * bri, 0x03 * bri);
			light_ctr_rgb_update(0x010400820, LIGHT_ROW_SIDE, 0x04 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
		case 3:
			light_ctr_rgb_update(0x0420, LIGHT_ROW_MIDDLE, 0x06 * bri * rgb_red, rgb_green * bri, 0x03 * bri);
			light_ctr_rgb_update(0x008800440, LIGHT_ROW_SIDE, 0x04 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
		case 2:
			light_ctr_rgb_update(0x0240, LIGHT_ROW_MIDDLE, 0x05 * bri * rgb_red, rgb_green * bri, 0x04 * bri);
			light_ctr_rgb_update(0x005000280, LIGHT_ROW_SIDE, 0x04 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
		case 1:
			light_ctr_rgb(0x0180, LIGHT_ROW_MIDDLE, 0x05 * bri * rgb_red, rgb_green * bri, 0x04 * bri);
			light_ctr_rgb(0x002000100, LIGHT_ROW_SIDE, 0x04 * bri * rgb_red, rgb_green * bri, 0x01 * bri);
			break;
		default:
			light_mode_alloff();
			break;
		}
	}

	hold_stall = hstall;
	vTaskDelay(50);
}


/**
 * \brief	Show the mode change successfully
 */
void light_mode_change_success(void)
{
	light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0x1f, 0x6f, 0x00);
	light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, 0x1f, 0x4f, 0x00);
	vTaskDelay(100);
	light_mode_alloff();
	vTaskDelay(100);
}

/**
 * \brief	Colorful flashing lights
 */
void light_mode_animation()
{
	uint8_t i, j;

	for (j = 0; j < 5; j++) {
		for (i = 0; i < 5; i++) {
			light_ctr_rgb_update(LIGHT_MASK_MFIRST_1 << (i + j), LIGHT_ROW_MIDDLE, 0xcc - 0x33 * i, 0x33 * i, 0x00);
			light_ctr_rgb_update((0x000000002 << (i + j)) & 0x00000fffe, LIGHT_ROW_SIDE, 0xcc - 0x33 * i, 0x33 * i, 0x00);
			light_ctr_rgb_update((0x100000000 >> (i + j)) & 0x1fffc0000, LIGHT_ROW_SIDE, 0xcc - 0x33 * i, 0x33 * i, 0x00);
		}
		for (i = 0; i < 6; i++) {
			light_ctr_rgb_update(LIGHT_MASK_MFIRST_2 << (j + i + 1), LIGHT_ROW_MIDDLE, 0x00, 0xff - 0x33 * i, 0x33 * i);
			light_ctr_rgb_update((0x000000020 << (i + j + 1)) & 0x00000fffe, LIGHT_ROW_SIDE, 0x00, 0xff - 0x33 * i, 0x33 * i);
			light_ctr_rgb_update((0x010000000 >> (i + j + 1)) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x00, 0xff - 0x33 * i, 0x33 * i);
		}
		for (i = 0; i < (5 - j); i++) {
			light_ctr_rgb_update(LIGHT_MASK_MFIRST_3 << (j + i + 3), LIGHT_ROW_MIDDLE, 0x33 * i, 0x00, 0xff - 0x33 * i);
			light_ctr_rgb_update((0x000000200 << (i + j + 3)) & 0x00000fffe, LIGHT_ROW_SIDE, 0x33 * i, 0x00, 0xff - 0x33 * i);
			light_ctr_rgb_update((0x001000000 >> (i + j + 3)) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x33 * i, 0x00, 0xff - 0x33 * i);
		}
		for (i = 0; i < j; i++) {
			light_ctr_rgb(LIGHT_MASK_MFIRST_1 << i, LIGHT_ROW_MIDDLE, 0x33 * (i + 5 - j), 0x00, 0xff - 0x33 * (i + 5 - j));
			light_ctr_rgb_update((0x000000002 << i) & 0x00000fffe, LIGHT_ROW_SIDE, 0x33 * (i + 5 - j), 0x00, 0xff - 0x33 * (i + 5 - j));
			light_ctr_rgb((0x100000000 >> i) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x33 * (i + 5 - j), 0x00, 0xff - 0x33 * (i + 5 - j));
		}
		vTaskDelay(20);
	}

	for (j = 0; j < 6; j++) {
		for (i = 0; i < 5; i++) {
			light_ctr_rgb_update(LIGHT_MASK_MFIRST_1 << (j + i), LIGHT_ROW_MIDDLE, 0x33 * i, 0x00, 0xff - 0x33 * i);
			light_ctr_rgb_update((0x000000002 << (i + j)) & 0x00000fffe, LIGHT_ROW_SIDE, 0x33 * i, 0x00, 0xff - 0x33 * i);
			light_ctr_rgb_update((0x100000000 >> (i + j)) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x33 * i, 0x00, 0xff - 0x33 * i);
		}
		for (i = 0; i < 5; i++) {
			light_ctr_rgb_update(LIGHT_MASK_MFIRST_2 << (i + j + 1), LIGHT_ROW_MIDDLE, 0xcc - 0x33 * i, 0x33 * i, 0x00);
			light_ctr_rgb_update((0x000000020 << (i + j + 1)) & 0x00000fffe, LIGHT_ROW_SIDE, 0xcc - 0x33 * i, 0x33 * i, 0x00);
			light_ctr_rgb_update((0x010000000 >> (i + j + 1)) & 0x1fffc0000, LIGHT_ROW_SIDE, 0xcc - 0x33 * i, 0x33 * i, 0x00);
		}
		for (i = 0; i < 6 - j; i++) {
			light_ctr_rgb_update(LIGHT_MASK_MFIRST_3 << (j + i + 2), LIGHT_ROW_MIDDLE, 0x00, 0xff - 0x33 * i, 0x33 * i);
			light_ctr_rgb_update((0x000000200 << (i + j + 2)) & 0x00000fffe, LIGHT_ROW_SIDE, 0x00, 0xff - 0x33 * i, 0x33 * i);
			light_ctr_rgb_update((0x001000000 >> (i + j + 2)) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x00, 0xff - 0x33 * i, 0x33 * i);
		}
		for (i = 0; i < j; i++) {
			light_ctr_rgb(LIGHT_MASK_MFIRST_1 << i, LIGHT_ROW_MIDDLE, 0x00, 0xff - 0x33 * (i + 6 - j), 0x33 * (i + 6 - j));
			light_ctr_rgb_update((0x000000002 << i) & 0x00000fffe, LIGHT_ROW_SIDE, 0x00, 0xff - 0x33 * (i + 6 - j), 0x33 * (i + 6 - j));
			light_ctr_rgb((0x100000000 >> i) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x00, 0xff - 0x33 * (i + 6 - j), 0x33 * (i + 6 - j));
		}
		vTaskDelay(20);
	}

	for (j = 0; j < 5; j++) {
		for (i = 0; i < 6; i++) {
			light_ctr_rgb_update(LIGHT_MASK_MFIRST_1 << (j + i), LIGHT_ROW_MIDDLE, 0x00, 0xff - 0x33 * i, 0x33 * i);
			light_ctr_rgb_update((0x000000002 << (i + j)) & 0x00000fffe, LIGHT_ROW_SIDE, 0x00, 0xff - 0x33 * i, 0x33 * i);
			light_ctr_rgb_update((0x100000000 >> (i + j)) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x00, 0xff - 0x33 * i, 0x33 * i);
		}
		for (i = 0; i < 5; i++) {
			light_ctr_rgb_update(LIGHT_MASK_MFIRST_2 << (i + j + 2), LIGHT_ROW_MIDDLE, 0x33 * i, 0x00, 0xff - 0x33 * i);
			light_ctr_rgb_update((0x000000020 << (i + j + 2)) & 0x00000fffe, LIGHT_ROW_SIDE, 0x33 * i, 0x00, 0xff - 0x33 * i);
			light_ctr_rgb_update((0x010000000 >> (i + j + 2)) & 0x1fffc0000, LIGHT_ROW_SIDE, 0x33 * i, 0x00, 0xff - 0x33 * i);
		}
		for (i = 0; i < 5 - j; i++) {
			light_ctr_rgb_update(LIGHT_MASK_MFIRST_3 << (j + i + 3), LIGHT_ROW_MIDDLE, 0xcc - 0x33 * i, 0x33 * i, 0x00);
			light_ctr_rgb_update((0x000000200 << (i + j + 3)) & 0x00000fffe, LIGHT_ROW_SIDE, 0xcc - 0x33 * i, 0x33 * i, 0x00);
			light_ctr_rgb_update((0x001000000 >> (i + j + 3)) & 0x1fffc0000, LIGHT_ROW_SIDE, 0xcc - 0x33 * i, 0x33 * i, 0x00);
		}
		for (i = 0; i < j; i++) {
			light_ctr_rgb(LIGHT_MASK_MFIRST_1 << i, LIGHT_ROW_MIDDLE, 0xcc - 0x33 * (i + 5 - j), 0x33 * (i + 5 - j), 0x00);
			light_ctr_rgb_update((0x000000002 << i) & 0x00000fffe, LIGHT_ROW_SIDE, 0xcc - 0x33 * (i + 5 - j), 0x33 * (i + 5 - j), 0x00);
			light_ctr_rgb((0x100000000 >> i) & 0x1fffc0000, LIGHT_ROW_SIDE, 0xcc - 0x33 * (i + 5 - j), 0x33 * (i + 5 - j), 0x00);
		}
		vTaskDelay(20);
	}
}

/**
 * \brief	light change softly in yellow
 */
void light_mode_comfor(void)
{
	uint8_t i;

	for (i = 0x00; i < 0x1a; i++) {
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, i, i, i / 2);
		vTaskDelay(80);
	}

	for (i = 0x1a; i >= 1; i--) {
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, i, i, i / 2);
		vTaskDelay(80);
	}
}

/**
 * \brief	Shaking mode.
 * \details	Shaking light beads to show characters
 * \param 	string 		string want to be shown
 */
void light_mode_shaking(char *string)
{
	uint8_t num, i, word_seq[LIGHT_MODE_SHA_MUX];

	/* According to the ASCII,take the string font */
	for (num = 0; * (string + num) != '\0'; num++) {
		word_seq[num] = *(string + num) - 97;
		EMBARC_PRINTF("The wor_seq[%d]:%d\r\n", num, word_seq[num]);
	}

	/* Limit the maximum number of display characters */
	if (num > LIGHT_MODE_SHA_MUX) {
		num = LIGHT_MODE_SHA_MUX;
	}

	while (1) {
		/* If a shaking action is detected. */
		if (imu_get_shaking_start()  ==   ACTION_START) {
			for (i = 0; i < num; i++) {
				vTaskDelay(1);
				light_show_word(word_seq[i]);
			}
		}
		light_mode_alloff();
		vTaskDelay(40);
	}
}

/**
 * \brief		SOS mode
 */
void light_mode_sos()
{
	uint8_t i;

	/* Send the S */
	for (i = 0; i < 3; i++ ) {
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0xef, 0xff, 0xff);
		light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, 0xef, 0xff, 0xff);
		vTaskDelay(200);
		light_mode_alloff();
		vTaskDelay(800);
	}

	/* Send the O */
	for (i = 0; i < 3; i++ ) {
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0xef, 0xff, 0xff);
		light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, 0xef, 0xff, 0xff);
		vTaskDelay(200);
		light_mode_alloff();
		vTaskDelay(1800);
	}

	/* Send the S */
	for (i = 0; i < 3; i++ ) {
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0xef, 0xff, 0xff);
		light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, 0xef, 0xff, 0xff);
		vTaskDelay(200);
		light_mode_alloff();
		vTaskDelay(800);
	}

	vTaskDelay(5000);
}
/**
 * \brief	used to indicates that the ilight is switching mode and different color represents
 		different mode
 * \param 	mode.	The number of mode right now.
 */
void light_mode_change_flag(uint8_t mode)
{
	vTaskDelay(2);
	light_mode_alloff();
	vTaskDelay(2);

	switch (mode) {
	case LIGHT_MODE_RUNNING:
		light_ctr_rgb_update(0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x00, 0x00);
		light_ctr_rgb_update(0x100048002, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x7ff03ff8, LIGHT_ROW_SIDE, 0x1f, 0x00, 0x00);
		break;
	case LIGHT_MODE_ALARM:
		light_ctr_rgb_update(0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x0a, 0x02);
		light_ctr_rgb_update(0x100048002, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x7ff03ff8, LIGHT_ROW_SIDE, 0x1f, 0x0a, 0x02);
		break;
	case LIGHT_MODE_RIDING:
		light_ctr_rgb_update(0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x1f, 0x00);
		light_ctr_rgb_update(0x100048002, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x7ff03ff8, LIGHT_ROW_SIDE, 0x1f, 0x1f, 0x00);
		break;
	case LIGHT_MODE_WARM_LIGHTING:
		light_ctr_rgb_update(0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xc003, LIGHT_ROW_MIDDLE, 0x00, 0x1f, 0x00);
		light_ctr_rgb_update(0x100048002, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x7ff03ff8, LIGHT_ROW_SIDE, 0x00, 0x1f, 0x00);
		break;
	case LIGHT_MODE_CO_LIGHTING:
		light_ctr_rgb_update(0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xc003, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x1f);
		light_ctr_rgb_update(0x100048002, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x7ff03ff8, LIGHT_ROW_SIDE, 0x00, 0x00, 0x1f);
		break;
	case LIGHT_MODE_TIMING_FIT:
		light_ctr_rgb_update(0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xc003, LIGHT_ROW_MIDDLE, 0x00, 0x1f, 0x0f);
		light_ctr_rgb_update(0x100048002, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x7ff03ff8, LIGHT_ROW_SIDE, 0x00, 0x1f, 0x0f);
		break;
	case LIGHT_MODE_TIMING_LAND:
		light_ctr_rgb_update(0xc003, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xc003, LIGHT_ROW_MIDDLE, 0x0f, 0x00, 0x0f);
		light_ctr_rgb_update(0x100048002, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x7ff03ff8, LIGHT_ROW_SIDE, 0x0f, 0x00, 0x0f);
		break;
	case LIGHT_MODE_COMFOR:
		light_ctr_rgb_update(0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x00, 0x00);
		light_ctr_rgb_update(0x1c01ce00e, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x1fc00fe0, LIGHT_ROW_SIDE, 0x1f, 0x00, 0x00);
		break;
	case LIGHT_MODE_MUSIC:
		light_ctr_rgb_update(0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x0a, 0x02);
		light_ctr_rgb_update(0x1c01ce00e, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x1fc00fe0, LIGHT_ROW_SIDE, 0x1f, 0x0a, 0x02);
		break;
	case LIGHT_MODE_ANIMATION:
		light_ctr_rgb_update(0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x1f, 0x00);
		light_ctr_rgb_update(0x1c01ce00e, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x1fc00fe0, LIGHT_ROW_SIDE, 0x1f, 0x1f, 0x00);
		break;
	case LIGHT_MODE_WEATHER:
		light_ctr_rgb_update(0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xf00f, LIGHT_ROW_MIDDLE, 0x00, 0x1f, 0x00);
		light_ctr_rgb_update(0x1c01ce00e, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x1fc00fe0, LIGHT_ROW_SIDE, 0x00, 0x1f, 0x00);
		break;
	case LIGHT_MODE_SHAKING:
		light_ctr_rgb_update(0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xf00f, LIGHT_ROW_MIDDLE, 0x00, 0x00, 0x1f);
		light_ctr_rgb_update(0x1c01ce00e, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x1fc00fe0, LIGHT_ROW_SIDE, 0x00, 0x00, 0x1f);
		break;
	case LIGHT_MODE_SOS:
		light_ctr_rgb_update(0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(~0xf00f, LIGHT_ROW_MIDDLE, 0x1f, 0x00, 0x1f);
		light_ctr_rgb_update(0x1c01ce00e, LIGHT_ROW_SIDE, 0x1f, 0x0f, 0x00);
		light_ctr_rgb(0x1fc00fe0, LIGHT_ROW_SIDE, 0x1f, 0x00, 0x1f);
		break;
	default:
		light_change_alldouble(0x01);
		light_change_alldouble(0x01);
		break;
	}
}

/**
 *\brief	Light control function Top pack.
 *\param	mode	The light mode that you want to choose.
 *\param	bri	The brightness of all lights.
 *\param	freq	The speed of lights moving.
 *\param	time	Used in timing mode.
 *\param	string	Used in shaking mode.
 */
void light_mode_top_pack(uint8_t mode, uint8_t bri, uint8_t freq, uint8_t time, uint8_t *string, uint8_t weather)
{
	static uint8_t freq1, bri1, time1, mode1 = 0x1a;

	/* If there is no brightness ,set the brightness as 20 */
	if (bri == 0) {
		if (mode ==  LIGHT_MODE_TIMING_FIT) {
			bri1 = DEFAULT_BRI_DIM;
		} else{
			bri1 = DEFAULT_BRI_LIGHT;
		}
	} else{
		bri1 = bri;
	}

	if (freq == 0) {
		freq1 = 0x02;
	} else{
		freq1 = freq;
	}

	if (time == 0) {
		time1 = 1;
	} else{
		time1 = time;
	}

	if (mode != mode1) {
		light_mode_change_success();
		mode1 = mode;
	}

	switch (mode) {
	case LIGHT_MODE_RUNNING:
		light_mode_run(freq1, bri1);
		break;
	case LIGHT_MODE_ALARM:
		light_mode_alarm(freq1, bri1);
		break;
	case LIGHT_MODE_RIDING:
		light_mode_ride(freq1, bri1);
		vTaskDelay(50);
		break;
	case LIGHT_MODE_WARM_LIGHTING:
		light_mode_lighting_warm(bri1);
		vTaskDelay(50);
		break;
	case LIGHT_MODE_CO_LIGHTING:
		light_mode_lighting_cold(bri1);
		vTaskDelay(50);
		break;
	case LIGHT_MODE_TIMING_FIT:
		light_mode_fitness(time1);
		vTaskDelay(50);
		break;
	case LIGHT_MODE_TIMING_LAND:
		light_mode_timing_land(time1);
		vTaskDelay(50);
		break;
	case LIGHT_MODE_COMFOR:
		light_mode_comfor();
		vTaskDelay(50);
		break;
	case LIGHT_MODE_MUSIC:
		break;
	case LIGHT_MODE_ANIMATION:
		light_mode_animation(2);
		break;
	case LIGHT_MODE_SHAKING:
		light_mode_shaking("lsnps");
		break;
	case LIGHT_MODE_SOS:
		light_mode_sos();
		break;
	case LIGHT_MODE_WEATHER:
		light_mode_weather(weather);
		break;
	default:
		light_change_alldouble(0x01);
		break;
	}
}

/**
 *\brief		Test light in different color
 */
void light_mode_test()
{
	int16_t i, j = 0;

	/* From dim white to light yellow */
	for (i = 0; i <= 0x2f; i++) {
		j = (i * i) / 0x2f;
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, i, i, i - j);
		light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, i, i, i - j);
		vTaskDelay(8);
	}

	/* From light yellow to light red */
	for (i = 0x2f; i >= 0; i--) {
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, (0x5e) - i, i, 0);
		light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, (0x5e) - i, i, 0);
		vTaskDelay(8);
	}

	/* From light red to light purple */
	for (i = 0; i <= 0x2f; i++) {
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, (0x5e) - i, 0, i);
		light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, (0x5e) - i, 0, i);
		vTaskDelay(8);
	}

	for (i = 0x2f; i >= 0; i--) {
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, i, 0, (0x5e) - i);
		light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, i, 0, (0x5e) - i);
		vTaskDelay(8);
	}

	for (i = 0; i <= 0x2f; i++) {
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, 0, i, (0x5e) - i);
		light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, 0, i, (0x5e) - i);
		vTaskDelay(8);
	}

	/* From light purple to dim white */
	for (i = 0x2f; i >= 0; i--) {
		j = (i * i) / 0x2f;
		light_ctr_rgb(LIGHT_MASK_ALL_MIDDLE, LIGHT_ROW_MIDDLE, i - j, i, i);
		light_ctr_rgb(LIGHT_MASK_ALL_SIDE1, LIGHT_ROW_SIDE, i - j, i, i);
		vTaskDelay(8);
	}

	vTaskDelay(800);
}

/** @} end of group EMBARC_APP_FREERTOS_ILIGHT_SMARTDEVICE */