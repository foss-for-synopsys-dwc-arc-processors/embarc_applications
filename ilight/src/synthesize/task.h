#ifndef _TASK_H_
#define _TASK_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "light_mode.h"
#include "light_driver.h"
#include "imu.h"
#include "scope.h"
#include "mic.h"
#include "rtc.h"

#define MPU_PERIOD_TICK		15
#define REC_PERIOD_TICK		30
#define MIC_PERIOD_TICK		5
#define ALARM_PERIOD_TICK	15000

#define INIT_MODE			0x11
#define INIT_BRIGHT			0
#define INIT_FREQUENCE		0
#define INIT_TIME			0
#define INIT_TEMPERTURE		0
#define INIT_HUMIDITY		0
#define INIT_WEATHER		0

#define FANCE_TXT_LEN_MAX	20
#define LED_WAIT_TIME_MAX	1000
// #define LED_NUM_MAX			LIGHT_NUM

#define MIN_MODE			LIGHT_MODE_RUNNING
#define MAX_MODE			LIGHT_MODE_SOS
#define MIN_BRIGHT			1
#define MAX_BRIGHT			127

#define INIT_MODE_INFO {	\
	INIT_MODE,				\
	INIT_BRIGHT,			\
	INIT_FREQUENCE,			\
	INIT_TIME,				\
	INIT_WEATHER,			\
	INIT_TEMPERTURE,		\
	INIT_HUMIDITY,			\
	{0}						\
}

typedef struct {
	uint8_t mode;
	uint8_t bright;
	uint8_t frequence;
	uint32_t time;
	uint8_t weather;
	uint8_t temperture;
	uint8_t humidity;
	uint8_t fance_txt[FANCE_TXT_LEN_MAX];
} mode_info;

extern TaskHandle_t pattern_change_task_handle;
extern SemaphoreHandle_t recognition_semaphore;
extern QueueHandle_t led_queue;
extern QueueHandle_t mode_info_queue;

extern uint8_t color_row;
extern uint8_t light_num;

extern uint8_t task_init(void);


#endif