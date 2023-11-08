/*
 * irf.h
 *
 *  Created on: 2018Äê5ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef IRF_IRF_H_
#define IRF_IRF_H_

#include "app_config.h"
#include "task_config.h"

typedef enum
{
	ADC_IO_SCL,
	ADC_IO_DI,
	ADC_IO_DO,
	ADC_IO_DRY
}ADC_IO_ENUM;

typedef enum
{
	STUDY_FLAG_NOSENSE,
	STUDY_FLAG_NOPERSON,
	STUDY_FLAG_PERSON,
	STUDY_FLAG_FORWARD,
	STUDY_FLAG_RIGHT,
	STUDY_FLAG_BACKWARD
}STUDY_FLAG_ENUM;

typedef struct
{
	unsigned long ch1 ;
	unsigned long ch2 ;
	unsigned long forward;
	unsigned long up;
	unsigned long noperson;
	uint32_t time;
	uint32_t time_pre;
	uint8_t flag;
	uint8_t timeflag;
	uint8_t nearby;
	uint8_t startstudy;


}STUDY_STRUCT;
extern STUDY_STRUCT study_ctr;

//task
extern TaskHandle_t IRF_app_Handler;//
extern TaskHandle_t MP3_TASK_Handler;//
extern QueueHandle_t SuperSound_Queue;

void IRF_app_task(void *pvParameters);
void irf_init(void);

void supersound_IOinit_1(void);
void supersound_exit_config_1(uint8_t edg);
void supersound_tri_config_1(uint8_t level);
void supersound_tri_start_process_1(void);
uint8_t supersound_echo_read_1(void);
void supersound_measure_done_1(void);
void supersound_measure_falied_1(void);

//ADC
void adc_io_init(void);
uint8_t adc_io_process(ADC_IO_ENUM io,uint8_t highorlow);
void WriteByte7705(uint8_t dat);
unsigned long ReadWord7705();
void reset7705();
void ad7705_init1();
void ad7705_init2();
unsigned int ReadData1_7705();
unsigned int ReadData2_7705();

//MP3
void mp3_init(void);
extern void mp3_play(uint8_t mp3_num);
void MP3_task(void *pvParameters);
//study
STUDY_FLAG_ENUM study_check(uint8_t study);


#endif /* IRF_IRF_H_ */
