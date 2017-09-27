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


/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

/* custom HAL */
#include "rtc.h"

DEV_IIC *IIC_port;
uint32_t point = (RTC_ADDR_IIC >> 1);
_RTC rtc;

/**
 * \brief	initialize pcf8563
 */
void pcf8563_iic_init()
{
	IIC_port = iic_get_dev(DW_IIC_0_ID);

	if (IIC_port->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD) != E_OK) {
		EMBARC_PRINTF("error!!!");
	}

	if (IIC_port->iic_control(IIC_CMD_ENA_DEV, (void *)point) != E_OK) {
		EMBARC_PRINTF("error!!!");
	}

	if (IIC_port->iic_control(IIC_CMD_MST_SET_TAR_ADDR, (void *)point) != E_OK) {
		EMBARC_PRINTF("error!!!");
	}
}

/**
 * \brief	Write one byte date to RTC.
 * \parameter	address	Befor write dataake the slave device address point to pcf8653.
 * \parameter	time_data	The time data you want to write
 */
void pcf8653_write_data(uint8_t address, uint8_t time_data)
{
	uint8_t send_data[2];

	send_data[0] = address;
	send_data[1] = time_data;

	if (IIC_port->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP)) != E_OK) {
		EMBARC_PRINTF("error!!!\n");
	}

	EMBARC_PRINTF("return:%d\n", IIC_port->iic_write((void*)&send_data, 2));
}

/**
 * \brief	Converts data from decimal to BCD.
 * \parameter	num	The data you want to convert
 * \retval	num	The data after converting
 */
uint8_t he2bcd(uint8_t num)
{
	uint8_t one, ten;

	one = num % 10;
	ten = (num - one) / 10;
	return num = ((ten & 0x0f) << 4) | one;
}

/**
 * \brief	Read data from pcf8563
 * \param	address	The address of data you want to read
 * \param	count 	The length of data
 * \param	buff 	The data read buff
 */
void pcf8563_data_read(uint8_t address, uint8_t count, uint8_t *buff)
{
	if (IIC_port->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_RESTART)) != E_OK) {
		EMBARC_PRINTF("error!!!\n");
	}

	IIC_port->iic_write((void*)(&address), 1);

	if (IIC_port->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP)) != E_OK) {
		EMBARC_PRINTF("error!!!");
	}

	IIC_port->iic_read((void*)(buff), count);

}

/**
 * \brief	Converts data from BCD to decimal.
 */
void pcf8563_bcd2de()
{
	uint8_t one, ten, hundred, tousand;

	/* hour */
	one = rtc.hour & 0x0f;
	ten = (rtc.hour >> 4) & 0x03;
	rtc.hour = one + ten * 10;

	/* min */
	one = rtc.min & 0x0f;
	ten = (rtc.min >> 4) & 0x07;
	rtc.min = one + ten * 10;

	/* second */
	one = rtc.sec & 0x0f;
	ten = (rtc.sec >> 4) & 0x07;
	rtc.sec = one + ten * 10;
}

/**
 * \brief	make the slave device of iic be rtc.
 */
void iic0_point_rtc(void)
{
	if (IIC_port->iic_control(IIC_CMD_ENA_DEV, (void *)point) != E_OK) {
		EMBARC_PRINTF("error!!!");
	}
	if (IIC_port->iic_control(IIC_CMD_MST_SET_TAR_ADDR, (void *)point) != E_OK) {
		EMBARC_PRINTF("error!!!");
	}
}

/**
 * \brief	updata the time data.
 */
uint8_t pcf8563_date_updata(uint8_t clock_day, uint8_t clock_hour, uint8_t clock_min)
{
	uint8_t time[5];
	iic0_point_rtc();
	pcf8563_data_read(0x02, 0x05, time);

	/* If the num of second is zero,update the date again! */
	if (rtc.sec == 0) {
		pcf8563_data_read(0x02, 0x05, time);
	}

	/* Process the data which read from the register */
	/* The bits from 0 to 6 are effective */
	rtc.sec = *(time + 0) & 0x7f; /* second */
	/* The bits from 0 to 6 are effective */
	rtc.min = *(time + 1) & 0x7f; /* minute */
	/* The bits from 0 to 5 are effective */
	rtc.hour = *(time + 2) & 0x3f; /* hour */
	/* The bits from 0 to 3 are effective */
	rtc.day = *(time + 4) & 0x07; /* week */

	pcf8563_bcd2de();
	//EMBARC_PRINTF("Second:%d. Min:%d. hour:%d. day:%d.\r\n", rtc.sec, rtc.min, rtc.hour, rtc.day);

	if (rtc.hour == clock_hour) {
		if (rtc.min == clock_min) {
			return CLOCK_START;
		}
	}
	return 0;
}

/**
 * \brief	Set the time value 8.
 */
void P8563_settime()
{
	pcf8653_write_data(8, rtc.year);
	pcf8653_write_data(7, rtc.month);
	pcf8653_write_data(5, rtc.date);
	pcf8653_write_data(4, rtc.hour);
	pcf8653_write_data(3, rtc.min);
	pcf8653_write_data(2, rtc.sec);
}

/**
 *\brief	Set initial time of 13:13:30,06,27,TUE,2017.
 */
void P8563_init()
{
	rtc.year = 0x17;
	rtc.month = 0x06;
	rtc.date = 0x27;
	rtc.day = 0x02;
	rtc.hour = 0x13;
	rtc.min = 0x13;
	rtc.sec = 0x30;

	iic0_point_rtc();
	P8563_settime();
}

DEV_GPIO_PTR port_gpio_rtc;
DW_GPIO_PORT_PTR port_rtc;
DEV_GPIO_INFO_PTR port_info_ptr_rtc;

DEV_GPIO_INT_CFG_PTR port_int_ptr_rtc;
DEV_GPIO_BIT_ISR_PTR port_isr_ptr_rtc;

/**
 *\brief	Set initial time of 13:13:30,06,27,TUE,2017.
 */
void cnt_stop()
{
	iic0_point_rtc();
	pcf8653_write_data(0x0e, 0x03); //0000_0011  //1/60hz
	pcf8653_write_data(0x01, 0x13); //0001_0011
}

/**
 *\brief	Set initial time of 13:13:30,06,27,TUE,2017.
 */
void cnt_start(uint8_t time)
{
	/* 1s */
	pcf8653_write_data(0x0e, 0x02); //0000_0010  //1hz
	/* 1min */
	pcf8653_write_data(0x01, 0x01); //0001_0001  //0001_0011
	pcf8653_write_data(0x0f, time);
	pcf8653_write_data(0x0e, 0x82); //1000_0010  //1hz
}

/**
 *\brief	Set initial time of 13:13:30,06,27,TUE,2017.
 */
void check_cnt()
{
	uint8_t read;
	pcf8563_data_read(0x01, 1, &read);

	/* check flag */
	if (read & 0x04) {
		/* times up */

		/* clear_flag */
		pcf8653_write_data(0x01, 0x11); //0001_0000   //0001_0011
	}
}

/** @} */