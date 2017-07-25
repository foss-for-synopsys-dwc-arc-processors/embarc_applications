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
 * \version 2017.07
 * \date 2017-07-11
 * \author Xiangcai Huang(xiangcai@synopsys.com)
--------------------------------------------- */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * \brief	function for heartrate data processing
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE
 * @{
 */
/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "common.h"
#include "process_hrate.h"
#include "timer1.h"
#include "heartrate.h"


#define STACK_DEPTH_HRATE (1024) /* stack depth for heartrate detector : word(4*bytes) */
#define TIME_DELAY_HRATE_MS (19) /* IR sampling frequency: 50Hz */

#define HRATE_SIZE (4)           /* increase this for more averaging, 4 is good */

#define BEATS_PER_MIN_MAX (255)  /* maximum value of heartrate */
#define BEATS_PER_MIN_MIN (20)   /* minimum value of heartrate */

#define AMPL_DIFF_MAX (1000)     /* maximum value of finger detecting amplitude */
#define AMPL_DIFF_MIN (100)      /* minimum value of finger detecting amplitude */

#define WARN_HR_MIN   (50)       /* lower value of warning heartrate */
#define WARN_HR_MAX   (150)      /* upper value of warning heartrate */


static uint16_t rates[HRATE_SIZE]; /* array of heart rates */
static uint8_t  rate_spot = 0;     /* index number of rates[] */

static float beats_per_min;        /* heartrate: beats per 1min */
static uint16_t beat_aver;         /* heartrate: average of 4 values */

static bool flag_timer_stop = true;

static int16_t ir_ac_max = 20;
static int16_t ir_ac_min = -20;

static int16_t val_cur = 0;    /* current ir value */
static int16_t val_pre;        /* previous ir value */
static int16_t val_min = 0;
static int16_t val_max = 0;
static int16_t aver_estimated; /* average ir value */

static int16_t pos_edge = 0;   /* positive edge */
static int16_t neg_edge = 0;   /* negative edge */
static int32_t ir_avg_reg = 0;

static int16_t cbuf[32];
static uint8_t offset = 0;

static const uint16_t fir_coeffs[12] = {172, 321, 579, 927, 1360, 1858, 2390, 2916, 3391, 3768, 4012, 4096};


static bool check_beat(int32_t sample);
static int16_t aver_dc_estimator(int32_t *p, uint16_t x);
static int16_t low_pass_fir_filter(int16_t din);
static int32_t mul16(int16_t x, int16_t y);

static void task_process_hrate(void *par);
static TaskHandle_t task_process_hrate_handle = NULL;

extern void hrate_detector_start(void)
{
	/* start timer1, timing for transform IR cycle into heartrate(beats per 1min) */
	timer1_start();

	/* create task for heartrate detector */
	if (xTaskCreate(task_process_hrate, "heartrate detector", STACK_DEPTH_HRATE, NULL, TSKPRI_HIGH,
	                &task_process_hrate_handle) != pdPASS) {
		EMBARC_PRINTF("Error: Create task_process_hrate failed\r\n");
	}
}


/** function for deal with heartrate by filter */
static void task_process_hrate(void *par)
{
	int32_t ir_value;   /* IR value of heartrate sensor(max30102) */
	int32_t data_rdy;   /* flag of IR data ready */
	uint32_t delta = 1; /* time interval between 2 heartbeats */

	for (;;) {
		ir_value = 0;

		/* read IR data from heartrate sensor */
		data_rdy = hrate_sensor_read(&ir_value);

		/* IR data ready and detect a heartbeat */
		if (data_rdy == E_OK && ir_value != 0 && check_beat(ir_value) == true)
		{
			flag_timer_stop != flag_timer_stop;

			if (flag_timer_stop == true)
			{
				delta = t1_cnt; /* get heartbeat's cycle time(uint: 0.1ms) */
				t1_cnt = 1;
				beats_per_min = 600000.0 / delta; /* 60/(delta/10000.0), get heartrate */

				if (beats_per_min < BEATS_PER_MIN_MAX && beats_per_min > BEATS_PER_MIN_MIN)
				{
					/* sliding average filtering */
					rates[rate_spot++] = (uint16_t)beats_per_min; /* store this reading in the array */
					rate_spot %= HRATE_SIZE;                      /* wrap variable */

					/* take average of readings */
					beat_aver = 0;
					for (uint8_t x = 0 ; x < HRATE_SIZE ; x++)
						beat_aver += rates[x];
					beat_aver /= HRATE_SIZE;
				}
			} else {
				t1_cnt = 1; /* reset the counter of timer1 */
			}
		}

		data_report_wn.hrate = beat_aver;

		data_report_wn.warn_hrate = false;
		if (beat_aver < WARN_HR_MIN || beat_aver > WARN_HR_MAX) {
			data_report_wn.warn_hrate = true;
		}

		vTaskDelay(TIME_DELAY_HRATE_MS);
	}
}

/**
 * Heart Rate Monitor functions takes a sample value and the sample number
 * returns true if a beat is detected
 * a running average of four samples is recommended for display on the screen
 */
static bool check_beat(int32_t sample)
{
	bool beat_detected = false;

	/* save current state */
	val_pre = val_cur;

	/* process next data sample */
	aver_estimated = aver_dc_estimator(&ir_avg_reg, sample);
	val_cur = low_pass_fir_filter(sample - aver_estimated);

	/* detect positive zero crossing (rising edge) */
	if ((val_pre < 0) & (val_cur >= 0))
	{
		ir_ac_max = val_max; /* adjust our AC max and min */
		ir_ac_min = val_min;

		pos_edge = 1;
		neg_edge = 0;
		val_max = 0;

		if ((ir_ac_max - ir_ac_min) > AMPL_DIFF_MIN & (ir_ac_max - ir_ac_min) < AMPL_DIFF_MAX)
		{
			/* heart beat detected */
			beat_detected = true;
		}
	}

	/* detect negative zero crossing (falling edge) */
	if ((val_pre > 0) & (val_cur <= 0))
	{
		pos_edge = 0;
		neg_edge = 1;
		val_min  = 0;
	}

	/* find maximum value in positive cycle */
	if (pos_edge & (val_cur > val_pre))
	{
		val_max = val_cur;
	}

	/* find minimum value in negative cycle */
	if (neg_edge & (val_cur < val_pre))
	{
		val_min = val_cur;
	}

	return (beat_detected);
}

/** average DC estimator */
static int16_t aver_dc_estimator(int32_t *p, uint16_t x)
{
	*p += ((((long) x << 15) - *p) >> 4);
	return (*p >> 15);
}

/** low pass FIR filter */
static int16_t low_pass_fir_filter(int16_t din)
{
	cbuf[offset] = din;

	int32_t z = mul16(fir_coeffs[11], cbuf[(offset - 11) & 0x1F]);

	for (uint8_t i = 0 ; i < 11 ; i++)
	{
		z += mul16(fir_coeffs[i], cbuf[(offset - i) & 0x1F] +
		           cbuf[(offset - 22 + i) & 0x1F]);
	}

	offset++;
	offset %= 32; /* wrap condition */

	return (z >> 15);
}

/** integer multiplier */
static int32_t mul16(int16_t x, int16_t y)
{
	return ((long)x * (long)y);
}

/** @} */