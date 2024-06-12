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
--------------------------------------------- */

/**
 * \defgroup	EMBARC_APP_FREERTOS_KERNEL	embARC FreeRTOS Kernel Example
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_OS_FREERTOS
 * \brief	embARC Example for testing FreeRTOS task switch and interrupt/exception handling
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *
 * ### Design Concept
 *     This example is designed to show the functionality of FreeRTOS.
 *
 * ### Usage Manual
 *     Test case for show how FreeRTOS is working by task switching and interrupt/exception processing.
 *     ![ScreenShot of freertos-demo under freertos](pic/images/example/emsk/emsk_freertos_demo.jpg)
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_KERNEL
 * \brief	main source file of the freertos demo
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_KERNEL
 * @{
 */

#include "embARC.h"
#include "embARC_debug.h"
#include <stdlib.h>

static void task1(void *par);
static void task2(void *par);
static void task3(void *par);

#define TSK_PRIOR_1		(configMAX_PRIORITIES-1)
#define TSK_PRIOR_2		(configMAX_PRIORITIES-2)
#define TSK_PRIOR_3		(configMAX_PRIORITIES-3)

// Semaphores
static SemaphoreHandle_t sem1_id;

// Queues
static QueueHandle_t dtq1_id;

// Task IDs
static TaskHandle_t task1_handle = NULL;
static TaskHandle_t task2_handle = NULL;
static TaskHandle_t task3_handle = NULL;

/**
 * \brief  call FreeRTOS API, create and start tasks
 */
int main(void)
{

	vTaskSuspendAll();

	// Create Tasks
	if (xTaskCreate(task1, "task1", 128, (void *)1, TSK_PRIOR_1, &task1_handle)
	    != pdPASS) {	/*!< FreeRTOS xTaskCreate() API function */
		EMBARC_PRINTF("Create task1 Failed\r\n");
		return -1;
	}
	else
	{
		EMBARC_PRINTF("Create task1 Successfully\r\n");
	}

	if (xTaskCreate(task2, "task2", 128, (void *)2, TSK_PRIOR_2, &task2_handle)
	    != pdPASS) {	/*!< FreeRTOS xTaskCreate() API function */
		EMBARC_PRINTF("Create task2 Failed\r\n");
		return -1;
	}
	else
	{
		EMBARC_PRINTF("Create task2 Successfully\r\n");
	}

	if (xTaskCreate(task3, "task3", 128, (void *)3, TSK_PRIOR_3, &task3_handle)
	    != pdPASS) {	/*!< FreeRTOS xTaskCreate() API function */
		EMBARC_PRINTF("Create task3 Failed\r\n");
		return -1;
	}
	else
	{
		EMBARC_PRINTF("Create task3 Successfully\r\n");
	}

	// Create Semaphores
	sem1_id = xSemaphoreCreateBinary();
	xSemaphoreGive(sem1_id);

	// Create Queues
	dtq1_id = xQueueCreate(8, sizeof(uint32_t));

	xTaskResumeAll();

	vTaskSuspend(NULL);

	return 0;
}

static void task1(void *par)
{
	uint32_t led_val = 0;

 	static portTickType xLastWakeTime;
    const portTickType xFrequency = pdMS_TO_TICKS(10);

	// 使用当前时间初始化变量xLastWakeTime ,注意这和vTaskDelay()函数不同
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		/* 调用系统延时函数,周期性阻塞10ms */
        vTaskDelayUntil( &xLastWakeTime,xFrequency );

        xQueuePeek(dtq1_id, (void *)(&led_val), portMAX_DELAY);

        if(led_val >= 0x0100)
        {
        	xSemaphoreGive(sem1_id);
        }
	}
}

static void task2(void *par)
{
	uint32_t led_val = 0x0001;

	static portTickType xLastWakeTime;
    const portTickType xFrequency = pdMS_TO_TICKS(100);

	// 使用当前时间初始化变量xLastWakeTime ,注意这和vTaskDelay()函数不同
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		/* 调用系统延时函数,周期性阻塞100ms */
        vTaskDelayUntil( &xLastWakeTime,xFrequency );

    	xQueueSend(dtq1_id, (void *)(&led_val),  portMAX_DELAY);
		led_val <<= 1;

		if( sem1_id != NULL )
    	{
        	/*如果信号量无效，则最多等待10个系统节拍周期。*/
        	if( xSemaphoreTake( sem1_id, ( TickType_t ) 10 ) == pdTRUE )
        	{
        		xQueueReset(dtq1_id);
        		led_val = 0x0001;
        	}
        }
	}
}

static void task3(void *par)
{
	uint32_t led_val = 0;

	static portTickType xLastWakeTime;
    const portTickType xFrequency = pdMS_TO_TICKS(200);

	// 使用当前时间初始化变量xLastWakeTime ,注意这和vTaskDelay()函数不同
	xLastWakeTime = xTaskGetTickCount();

	while (1)
	{
		/* 调用系统延时函数,周期性阻塞200ms */
        vTaskDelayUntil( &xLastWakeTime,xFrequency );

		xQueueReceive(dtq1_id, (void *)(&led_val), portMAX_DELAY);

		led_write(led_val, BOARD_LED_MASK);

		EMBARC_PRINTF("\r\n The led_val is 0x%x \r\n",led_val);
	}

}

/** @} */