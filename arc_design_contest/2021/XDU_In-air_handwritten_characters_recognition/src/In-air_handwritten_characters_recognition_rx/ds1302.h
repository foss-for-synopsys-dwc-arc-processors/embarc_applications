#ifndef _DS1302_H_
#define _DS1302_H_

#include "stdio.h"
#include "string.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

#include "board.h"
#include "dev_common.h"


// DS1302 definition

#define RTC_PORT_ID     DFSS_GPIO_4B1_ID    //gpio4B1
#define RTC_PIN_CLK     0
#define RTC_PIN_DAT     1
#define RTC_PIN_RST     2
// #define DS1302_PIN_CLK_MASK	    0x01		//gpio4B1[0]-CLK-OUTPUT
// #define DS1302_PIN_CLK_DIR		0x00	
// #define DS1302_PIN_DAT_MASK	    0x02		//gpio4B1[1]-DAT-INOUT
// #define DS1302_PIN_DAT_DIR	    0x01	
// #define DS1302_PIN_RST_MASK	    0x04		//gpio4B1[2]-RST-OUTPUT
// #define DS1302_PIN_RST_DIR	    0x00	

// DS1302 address definition
#define DS1302_SEC_ADD			0x80
#define DS1302_MIN_ADD			0x82
#define DS1302_HR_ADD			0x84
#define DS1302_DATE_ADD			0x86
#define DS1302_MONTH_ADD		0x88
#define DS1302_DAY_ADD			0x8a
#define DS1302_YEAR_ADD			0x8c
#define DS1302_CONTROL_ADD		0x8e
#define DS1302_CHARGER_ADD		0x90
#define DS1302_CLKBURST_ADD		0xbe

#define TIME_HEX_LEN    8

// DS1302 function declaration
void     rtc_init(void);
uint32_t rtc_bit_out(uint32_t num, uint32_t bit);
uint32_t rtc_bit_in(uint32_t num);
void     rtc_write_byte(uint8_t addr, uint8_t data);
uint8_t  rtc_read_byte(uint8_t addr);
void     rtc_write_time(uint8_t *time_raw);
void     rtc_read_time(uint8_t *time_rcv);

void     rtc_set_time(uint8_t *time_rcv);
void     rtc_time_extract(uint8_t *time_rcv, uint8_t *time_extr);
void     rtc_time_extract_hex(uint8_t *time_rcv, uint8_t *time_hex);
char    *day_of_week(uint8_t day_num, char *day_string);

#endif
