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

extern TaskHandle_t IRF_app_Handler;//

void IRF_app_task(void *pvParameters);
void irf_init(void);

#endif /* IRF_IRF_H_ */
