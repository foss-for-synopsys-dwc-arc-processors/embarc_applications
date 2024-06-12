/*
 * initsys.h
 *
 *  Created on: 2018年5月4日
 *      Author: Administrator
 */

#ifndef INITSYS_INITSYS_H_
#define INITSYS_INITSYS_H_

#include "app_config.h"

#include "FreeRTOS.h"					//FreeRTOS使用
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

extern uint8_t DEVICE_ID[8];

void init_system(void);
void sys_config_init(void);
void default_sys_config(void);


#endif /* INITSYS_INITSYS_H_ */
