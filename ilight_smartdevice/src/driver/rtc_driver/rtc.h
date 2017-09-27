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
 * \version 2017.03
 * \date 2017-08-20
 * \author Zhiwei Zhang(1812816853@qq.com)
--------------------------------------------- */


/**
 * \file
 * \ingroup EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * \brief emsk rtc for iLight
 */

/**
 * \addtogroup  EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE
 * @{
 */


#ifndef _RTC_H_
#define _RTC_H_



/**
 *Some macro definitions of date register
 *The macro definition of IIC address of RTC device.
 */
#define RTC_ADDR_IIC		0xa2
/* The macro definition of register date of week. */
#define RTC_ADDR_WEEK		0x06
/* The macro definition of register date of hour. */
#define RTC_ADDR_HOUR		0x04
/* The macro definition of register date of minute. */
#define RTC_ADDR_MIN		0x03
/* The macro definition of register date of second. */
#define RTC_ADDR_SEC  		0x02

#define CLOCK_START 		0x11
/* Some macro definitions of week's date. */
#define RTC_WEEK_MON		0
#define RTC_WEEK_TUES 		1
#define RTC_WEEK_WEDNES 	2
#define RTC_WEEK_THURS		3
#define RTC_WEEK_FRI 		4
#define RTC_WEEK_SATUR		5
#define RTC_WEEK_SUN		6

typedef struct _rtc {
	uint8_t day;
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
} _RTC;

extern _RTC rtc;

/* Initialize the RTC's IIC device */
extern void pcf8563_iic_init(void);

/* Update the date of week,hour,minute,and second */
uint8_t pcf8563_date_updata(uint8_t clock_day, uint8_t clock_hour, uint8_t clock_min);

/* Reset RTC data!
 * \parameter	The date that you want to set.
 */
extern void pcf8653_time_reset(uint8_t week, uint8_t hour, uint8_t minute, uint8_t sec);

extern void iic0_point_rtc(void);
extern void cnt_start(uint8_t time);
extern void check_cnt(void);

#endif  /* _RTC_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IOT_ILIGHT_SMARTDEVICE */