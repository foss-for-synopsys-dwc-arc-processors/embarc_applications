/*
 * bright.h
 *
 *  Created on: 2018年5月24日
 *      Author: Administrator
 */

#ifndef BRIGHT_BRIGHT_H_
#define BRIGHT_BRIGHT_H_

#include "app_config.h"
#include "task_config.h"


typedef enum
{
 LIGHT_SWITCH ,//切换档位 渐变切换
 LIGHT_DIRECT ,//直接到某个档位，不渐变
 LIGHT_AUTO   ,
 LIGHT_SLEEP  ,

}bright_mode_enum;


void bright_mode(bright_mode_enum mode,uint8_t light);
void bright_switch_process(uint8_t light);
void bright_direct_process(uint8_t light);
void bright_auto_process(uint8_t light);
void period_Bright_Callback(TimerHandle_t xTimer);
void brightchange_send2server_touch(uint32_t compare_temp);


#endif /* BRIGHT_BRIGHT_H_ */
