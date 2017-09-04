#ifndef LIGHT_MODE_H_
#define LIGHT_MODE_H_

#include "light_driver.h"
#include "stdlib.h"
#include "math.h"
#include "imu.h"
#include "mic.h"
#include "task.h"
#include "word.h"

#define LIGHT_RIDE_ACC_SUM		(960)

#define DEFAULT_BRI_DIM			(0x10)
#define DEFAULT_BRI_LIGHT		(0x26)	

#define LIGHT_MASK_ALL_MIDDLE	(0xffff)
#define LIGHT_MASK_ALL_SIDE1	(0x3ffffffff)
#define LIGHT_MASK_ALL_SIDE2	(0x1fffcfffe)
#define LIGHT_MASK_ALL_SIDE3	(0x3fffcffff)
#define LIGHT_MASK_ALL_SUP1		(0x00000fffe)
#define LIGHT_MASK_ALL_SDW1		(0x1fffe0000)
#define LIGHT_MASK_MIDDLE_M1	(0x0080)			//The mask of middle of the row of middle.
#define LIGHT_MASK_MIDDLE_M2	(0x0100)
#define LIGHT_MASK_MIDDLE_FST	(0x0001)
#define LIGHT_MASK_MIDDLE_LST	(0x8000)
#define LIGHT_MASK_MIDDLE_12	(0x9249)
#define LIGHT_MASK_MIDDLE_21	(~LIGHT_MASK_MIDDLE_12)
#define LIGHT_MASK_MIDDLE_11	(0x5555)
#define LIGHT_MASK_SIDE_UPM		(0x000000100)		//The mask of middle of the row of up sides.
#define LIGHT_MASK_SIDE_DWM		(0x002000000)
#define LIGHT_MASK_SIDE_MID		(0x002000100)
#define LIGHT_MASK_SIDE_UP 		(0x00001ffff)
#define LIGHT_MASK_SIDE_DW 		(0x3fffe0000)
#define LIGHT_MASK_SIDE_UPLST	(0x000010000)
#define LIGHT_MASK_SIDE_DWLST	(0x000020000)
#define LIGHT_MASK_SIDE_UPFST	(0x000000001)
#define LIGHT_MASK_SIDE_DWFST	(0x200000000)
#define LIGHT_MASK_SIDE_21		(0x36db7b6db)
#define LIGHT_MASK_SIDE_12		(~LIGHT_MASK_SIDE_21)

#define LIGHT_MASK_SIDE_UPHALF1	(0x0000000ff)
#define LIGHT_MASK_SIDE_UPHALF2	(0x00001fe00)
#define LIGHT_MASK_SIDE_DWHALF1	(0x001fe0000)
#define LIGHT_MASK_SIDE_DWHALF2	(0x3fc000000)

#define LIGHT_COLOR_OFF (0x000000)

#define LIGHT_MODE_RUN_MBLU1	(0x4a52)
#define LIGHT_MODE_RUN_MBLU2	(0x318c)
#define LIGHT_MODE_RUN_SBLU1	(0x0a5285294)
#define LIGHT_MODE_RUN_SBLU2	(0x042102108)
#define LIGHT_MODE_RUN_MWHT		(0x8421)
#define LIGHT_MODE_RUN_SWHT		(0x318c78c63)

#define LIGHT_MODE_ALARM_MBLU	(0x00f0)
#define LIGHT_MODE_ALARM_MRED	(0x0f00)
#define LIGHT_MODE_ALARM_SRED1	(0x0e00)
#define LIGHT_MODE_ALARM_SRED2	(0x1c00000)
#define LIGHT_MODE_ALARM_SBLU1	(0x00e0)
#define LIGHT_MODE_ALARM_SBLU2	(0x01c000000)

#define LIGHT_MODE_SHA_MUX		(0x06)

#define LIGHT_MODE_TIMING_UNIT	(3740)

#define LIGHT_MODE_MUS_NUM		(8) 
#define LIGHT_MODE_MUS_UNIT_JUD	(6)

/******Light mode number macro definition*********/
#define LIGHT_MODE_CLOCK		 0x10
#define LIGHT_MODE_RUNNING 		 0x11
#define LIGHT_MODE_ALARM         0X12
#define LIGHT_MODE_RIDING        0x13
#define LIGHT_MODE_WARM_LIGHTING 0x14
#define LIGHT_MODE_CO_LIGHTING   0x15
#define LIGHT_MODE_TIMING_FIT    0x16
#define LIGHT_MODE_TIMING_LAND   0x17
#define LIGHT_MODE_COMFOR        0x18
#define LIGHT_MODE_MUSIC         0x19
#define LIGHT_MODE_ANIMATION     0x1a
#define LIGHT_MODE_WEATHER       0X1b
#define LIGHT_MODE_SHAKING       0x1c
#define LIGHT_MODE_SOS			 0x1d

#define LIGHT_MODE_WEATHER_SUN   1	//sunny and cloudy
#define LIGHT_MODE_WEATHER_WINDY 2	//windy
#define LIGHT_MODE_WEATHER_RAIN  3	//rain

/******Frequency macro definition*********/
#define FREQUENCY_SLOW 			4
#define FREQUENCY_NORMAL		3
#define FREQUENCY_FAST          2
#define FREQUENCY_VERY_FAST     1

#define LIGHT_MASK_MFIRST_1 0x0001
#define LIGHT_MASK_MFIRST_2 0x0010
#define LIGHT_MASK_MFIRST_3 0x0100
#define LIGHT_MASK_MFIRST_4 0x0001

/******Some threshold settings******/
#define RIDE_STOP_THRESHOLD -150
#define RIDE_START_THRESHOLD 100
#define RIDE_Z_ACC_JUDGE 200
#define RIDE_Y_FILTER_FACTOR 0.3
#define PI 3.1416



#define LIGHT_UNIT_BRI (0x04)

void light_ctr_gpio_init(void);

void light_mode_change_success(void);

void light_mode_alloff(void);

void light_mode_music(uint8_t *fftOut);

void light_mode_test();

void light_mode_change_flag(uint8_t mode);

void light_mode_top_pack(uint8_t mode,uint8_t bri,uint8_t freq,uint8_t time,uint8_t *string,uint8_t weather);

void light_mode_clock(void);

#endif 