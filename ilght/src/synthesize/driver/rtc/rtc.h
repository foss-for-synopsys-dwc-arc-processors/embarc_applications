#ifndef _RTC_H_
#define _RTC_H_

#include "embARC.h"
#include "embARC_debug.h"

/*****Some macro definitions of date register.*****/
/*The macro definition of IIC address of RTC device.*/
#define RTC_ADDR_IIC  0xa2
/*The macro definition of register date of week.*/
#define RTC_ADDR_WEEK 0x06
/*The macro definition of register date of hour.*/
#define RTC_ADDR_HOUR 0x04
/*The macro definition of register date of minute.*/
#define RTC_ADDR_MIN  0x03
/*The macro definition of register date of second.*/
#define RTC_ADDR_SEC  0x02

#define CLOCK_START 0x11
/*****Some macro definitions of week's date.*****/
#define RTC_WEEK_MON  	0
#define RTC_WEEK_TUES 	1
#define RTC_WEEK_WEDNES 2
#define RTC_WEEK_THURS  3
#define RTC_WEEK_FRI    4
#define RTC_WEEK_SATUR  5
#define RTC_WEEK_SUN    6

typedef struct _rtc {
  uint8_t day;  //The day of the week.
  uint8_t year;
  uint8_t month;
  uint8_t date;
  uint8_t hour;		
  uint8_t min;
  uint8_t sec;
}_RTC;

extern _RTC rtc;

/*Initialize the RTC's IIC device*/
extern void pcf8563_iic_init(void);
/*Update the date of week,hour,minute,and second*/
uint8_t pcf8563_date_updata(uint8_t clock_day,uint8_t clock_hour,uint8_t clock_min);
/*Reset RTC data!
 *parameter:The date that you want to set.
*/
extern void pcf8653_time_reset(uint8_t week,uint8_t hour,uint8_t minute,uint8_t sec);

extern void iic0_point_rtc(void);
extern void cnt_start(uint8_t time);;
extern void check_cnt();


#endif