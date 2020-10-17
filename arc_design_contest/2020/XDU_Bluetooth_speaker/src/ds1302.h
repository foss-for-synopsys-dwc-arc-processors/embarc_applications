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
/*
 * GPIO macros
 * PM3 Upper row
 * J3.6  = 3V3
 * J3.5  = GND
 * J3.10 = Port A[19] = RTC_PIN_CLK
 * J3.9  = Port A[18] = RTC_PIN_DAT
 * J3.8  = Port A[17] = RTC_PIN_RST
 */
#define RTC_PORT_ID     DW_GPIO_PORT_A
#define RTC_PIN_CLK     19
#define RTC_PIN_DAT     18
#define RTC_PIN_RST     17

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
