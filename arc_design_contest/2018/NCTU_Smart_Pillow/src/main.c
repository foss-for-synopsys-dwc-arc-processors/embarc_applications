/* ------------------------------------------
 * Copyright (c) 2016, Synopsys, Inc. All rights reserved.

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
 * \version 2016.05
 * \date 2016-04-20
 * \author Qiang Gu(Qiang.Gu@synopsys.com)
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
#include <stdio.h>
#include <string.h>

static void breath_task(void * par);
static void turn_task(void * par);
static void mp3_task(void * par);
// static void ble_task(void * par);
static void start_task(void * par);

/**
 * \brief	handle of breath_task
 * \details	If task handle is not used, set Null.
 */
static TaskHandle_t breath_task_handle = NULL;
static TaskHandle_t turn_task_handle = NULL;
static TaskHandle_t mp3_task_handle = NULL;
// static TaskHandle_t ble_task_handle = NULL;
static TaskHandle_t start_task_handle = NULL;

static DEV_GPIO *gpio;
static DEV_UART *mp3;  //
static DEV_UART *ble;
static DEV_IIC *iic_0;
static DEV_IIC *iic_1;
static uint32_t iic_slvaddr = 0x28;

static const float b[4] = { 1.7633802731897852E-7, 5.2901408195693556E-7,
			5.2901408195693556E-7, 1.7633802731897852E-7 };

static const float a[4] = { 1.0, -2.9774853715209537, 2.9552234840417082,
			-0.97773670181653594 };

static int v1_init = 0;
static int v2_init = 4096;
static int v3_init = 8192;
static int lstate = 0;
static int mstate = 0;
static int rstate = 0;
static int prev_state = 0;
static int turn_count = 0;
static float raw_data[50000];
static float filtered_data[50000];
static int Hard = 0, Soft = 0, Mild = 0;
static int respiration;
static bool led_off = 1;
#define EMSK_IIC_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)
#define EMSK_GPIO_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)

void my_emsk_gpio_init(void);
int32_t my_emsk_iic_init(uint32_t slv_addr);
void my_emsk_ble_init(void);
void my_emsk_mp3_init(void);
void cal(void);
void brightness(int k);

void init();    //
void volume(uint8_t vol);
void play(uint8_t num);
void next();
void pause();
void start();  //

/**
 * \brief  call FreeRTOS API, create and start tasks
 */
int main(void)
{	
	EMBARC_PRINTF("Part 1 \n");
	cpu_lock();   //
	board_init();          //
	cpu_unlock();            //
	
	EMBARC_PRINTF("Part 2 \n");
	my_emsk_gpio_init();
	my_emsk_iic_init(iic_slvaddr);
	my_emsk_ble_init();
	my_emsk_mp3_init();       //
	init();          //
	volume(10);      //
	
	EMBARC_PRINTF("Part 3 \n");
	uint8_t config[2];
	config[0] = 0x08; // configuration of the I2C communication in HIGH SPEED Mode
	config[1] = 0x10; // configuration of Pmod AD2 (read of V1)
	iic_0->iic_write(config,2);
	
	EMBARC_PRINTF("Part 4 \n");
	config[0] = 0x08; // configuration of the I2C communication in HIGH SPEED Mode
	config[1] = 0x70; // configuration of Pmod AD2 (read of V1 to V3)
	iic_1->iic_write(config,2);
	
	/*
	EMBARC_PRINTF("Part 5 \n");
	char start_message[5];
	ble->uart_read(start_message, 5);
	if(strstr(start_message, "start") == NULL)
	{
		return 0;
	}*/
	
	
	
	EMBARC_PRINTF("Part 6 \n");
	if (xTaskCreate(start_task, "start_task", 128, (void *)NULL, 1, &start_task_handle)
		!= pdPASS) {	/*!< FreeRTOS xTaskCreate() API function */
		EMBARC_PRINTF("create start_task error\r\n");
		return -1;
	}
	EMBARC_PRINTF("Part 7 \n");
	vTaskStartScheduler();
error_exit:
	return 0;
}

/**
 * \brief  start_task in FreeRTOS
 * \details 
 * \param[in] *par
 */
static void start_task(void * par)
{
	
	#if(usercfgCPU_USAGE_CALCULATE==1)
    uTaskCPUUsageInit();
    #endif
	
	
	if (xTaskCreate(breath_task, "breath_task", 256, (void *)NULL, 2, &breath_task_handle)
		!= pdPASS) {	/*!< FreeRTOS xTaskCreate() API function */
		EMBARC_PRINTF("create breath_task error\r\n");
		return ;
	}
	if (xTaskCreate(turn_task, "turn_task", 128, (void *)NULL, 3, &turn_task_handle)
		!= pdPASS) {	/*!< FreeRTOS xTaskCreate() API function */
		EMBARC_PRINTF("create turn_task error\r\n");
		return ;
	}
	if (xTaskCreate(mp3_task, "mp3_task", 128, (void *)NULL, 4, &mp3_task_handle)
		!= pdPASS) {	/*!< FreeRTOS xTaskCreate() API function */
		EMBARC_PRINTF("create mp3_task error\r\n");
		return ;
	}
	/*
	if (xTaskCreate(ble_task, "ble_task", 128, (void *)NULL, 5, &ble_task_handle)
		!= pdPASS) {	//!< FreeRTOS xTaskCreate() API function 
		EMBARC_PRINTF("create ble_task error\r\n");
		return ;
	}*/
	vTaskDelete(start_task_handle);
}

/**
 * \brief  breath_task in FreeRTOS
 * \details 
 * \param[in] *par
 */
static void breath_task(void * par)
{
	while(1)
	{
		int i;
		int val;
		uint8_t data[1];
		EMBARC_PRINTF("Start\n");
		for(i = 0; i <36000; i++){
			iic_0->iic_read(data,1);
			val = data[0] << 8;
			iic_0->iic_read(data,1);
			val = val + data[0];
			float tmp = ((float)val)/4096.0*2 - 1;
			if(tmp < 0) tmp = -tmp;
			raw_data[i] = tmp;
			respiration = val;
			filtered_data[i] = 0;
			vTaskDelay(1);
		}
		EMBARC_PRINTF("Collect data completed\n");
		Hard = 0;
		Soft = 0;
		Mild = 0;
		cal();
		EMBARC_PRINTF("Hard: %d Soft: %d Mild: %d\n", Hard, Soft, Mild);
		char send_message[9];
		if(prev_state == 0)
			sprintf(send_message, "%02d%02d%02d%02d0", Hard, Soft, Mild,turn_count);
		else
			sprintf(send_message, "%02d%02d%02d%02d1", Hard, Soft, Mild,turn_count);
		ble->uart_write(send_message, 9);
		for(i = 0; i < 9; i++)
			EMBARC_PRINTF("%c",send_message[i]);
		EMBARC_PRINTF("\n");
	}
}

void cal(void){
	int x, i, j, k;
	float tmp;
	for (k = 0; k < 36000; k++) {
		x = 36000 - k;
		if (x >= 4) {
			x = 4;
		}
		for (j = 0; j < x; j++) {
			filtered_data[k + j] += raw_data[k]*b[j];
		}
		x = 35999 - k;
		if (x >= 3) {
			x = 3;
		}
		tmp = -filtered_data[k];
		for (j = 1; j <= x; j++) {
			filtered_data[k + j] += tmp*a[j];
		}
	}
	
	float filtered_max = 0;
	for (k = 0; k < 36000; k++) {
		if (filtered_max < filtered_data[k])
			filtered_max = filtered_data[k];
	}
	for (k = 0; k < 36000; k++) filtered_data[k] /= filtered_max;
	
	float max[60];
	int time[60];
	
	for(i = 0; i < 60; i++){
		max[i] = -1;
		time[i] = 0;
		for (j = i*600 + 1; j < (i+1)*600 - 1; j++){
			if(filtered_data[j] > filtered_data[j-1] && filtered_data[j] > filtered_data[j+1])
			{
				if (max[i] < filtered_data[j])
				{
					time[i] = j;
					max[i] = filtered_data[j];
				}
			}
		}
		if(max[i] >= 0.7) Hard++;
		else if (max[i] < 0.7 && max[i] >= 0.3) Soft++;
		else if (max[i] > 0) Mild++;
	}
}
/**
 * \brief  turn_task in FreeRTOS
 * \details 
 * \param[in] *par
 */
static void turn_task(void * par)
{
	while(1)
	{	
		int val;
		uint8_t data[1];
		iic_1->iic_read(data,1);
		val = data[0] << 8;
		iic_1->iic_read(data,1);
		val = val + data[0];
		if (((val & 0x3000)>> 12) == 0){
			if (val - v1_init > 500)
				lstate = 1;
			else
				lstate = 0;
		}
		else if(((val & 0x3000)>> 12) == 1){
			if (val - v2_init > 500)
				mstate = 2;
			else
				mstate = 0;
		}
		else if(((val & 0x3000)>> 12) == 2){
			if (val - v3_init > 500)
				rstate = 4;
			else
				rstate = 0;
		}
		if (((lstate + mstate + rstate) - prev_state) != 0){
			turn_count++;
			prev_state = (lstate + mstate + rstate);
			EMBARC_PRINTF("turn_count:%d\n",turn_count);
			char send_message[9];
			if(prev_state == 0)
				sprintf(send_message, "000000%02d0", turn_count);
			else
				sprintf(send_message, "000000%02d1", turn_count);
			ble->uart_write(send_message, 9);
			int i;
			for(i = 0; i < 9; i++)
				EMBARC_PRINTF("%c",send_message[i]);
			EMBARC_PRINTF("\n");
		}
		vTaskDelay(1);
	}
}

/**
 * \brief  mp3_task in FreeRTOS
 * \details 
 * \param[in] *par
 */
static void mp3_task(void * par)
{
	while(1) {
		uint32_t rd_avail;
		uint32_t cnt;
		cnt = 1;
		char read_message[1];
		ble->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
		cnt = (cnt > rd_avail)?rd_avail:cnt;
		if (cnt > 0) {
			ble->uart_read(read_message, cnt);
			EMBARC_PRINTF("%c",read_message[0]);
		}
		if(strstr(read_message, "1") != NULL) {
			play(1);
			EMBARC_PRINTF("Get message l \n");
		}
		if(strstr(read_message, "2") != NULL) {
			play(2);
			EMBARC_PRINTF("Get message 2 \n");
		}
		if(strstr(read_message, "3") != NULL) {
			play(3);
			EMBARC_PRINTF("Get message 3 \n");
		}
		if(strstr(read_message, "4") != NULL) {
			play(4);
			EMBARC_PRINTF("Get message 4 \n");
		}
		if(strstr(read_message, "5") != NULL) {
			play(5);
			EMBARC_PRINTF("Get message 5 \n");
		}
		if(strstr(read_message, "6") != NULL) {
			play(6);
			EMBARC_PRINTF("Get message 6 \n");
		}
		if(strstr(read_message, "7") != NULL) {
			play(7);
			EMBARC_PRINTF("Get message 7 \n");
		}
		/*
		if(strstr(read_message, "8") != NULL) {
			play(8);
			EMBARC_PRINTF("Get message 8 \n");
		}*/
		if(strstr(read_message, "p") != NULL) { // pause
			pause();
			EMBARC_PRINTF("Get message p \n");
		}
		if(strstr(read_message, "r") != NULL) { // resume
			start();
			EMBARC_PRINTF("Get message r \n");
		}
		//
		if(strstr(read_message, "l") != NULL) {
			led_off = 0;
			gpio->gpio_write(GPIO_BITS_MASK_ALL,GPIO_BITS_MASK_ALL);
			EMBARC_PRINTF("Get message l \n");
		}
		if(strstr(read_message, "o") != NULL) {
			led_off = 1;
			gpio->gpio_write(GPIO_BITS_MASK_NONE,GPIO_BITS_MASK_ALL);
			EMBARC_PRINTF("Get message o \n");
		}
		if(strstr(read_message, "w") != NULL) {
			led_off = 0;
			play(7);
			gpio->gpio_write(GPIO_BITS_MASK_NONE,GPIO_BITS_MASK_ALL);
			EMBARC_PRINTF("Get message w \n");
		}
		if(strstr(read_message, "d") != NULL) {
			led_off = 0;
			play(8);
			gpio->gpio_write(GPIO_BITS_MASK_NONE,GPIO_BITS_MASK_ALL);
			EMBARC_PRINTF("Get message d \n");
		}
		if(led_off) {
			int k = 0; 
			if(respiration >= 2048)
				k = (respiration - 2048)*8/1200;
			else
				k = (2048 - respiration)*8/1200;
			brightness(k);
			EMBARC_PRINTF("%d",k);
		}
		// EMBARC_PRINTF("Cpu use %d \n", OSCPUusage);
		
		read_message[0] = " ";
		vTaskDelay(1);
	}
}

void init()
{
	uint8_t _sending[10];
	_sending[0] = 0x7E;
	_sending[1] = 0xFF;
	_sending[2] = 0x06;
	_sending[3] = 0x0C;
	_sending[4] = 0x01;
	_sending[5] = 0x00;
	_sending[6] = 0x00;
	_sending[7] = 0xFE;
	_sending[8] = 0xEE;
	_sending[9] = 0xEF;
	mp3->uart_write(_sending, 10);
	// vTaskDelay(1000);
}
void volume(uint8_t vol) //Set volume value. From 0 to 30
{
	uint8_t _sending[10];
	_sending[0] = 0x7E;
	_sending[1] = 0xFF;
	_sending[2] = 0x06;
	_sending[3] = 0x06;
	_sending[4] = 0x01;
	_sending[5] = 0x00;
	_sending[6] = vol; // volume
	_sending[7] = 0xFE;
	_sending[8] = 0xF4 - vol;
	_sending[9] = 0xEF;
	mp3->uart_write(_sending, 10);
	// vTaskDelay(1000);
}

void play(uint8_t num)
{
	uint8_t _sending[10];
	_sending[0] = 0x7E;
	_sending[1] = 0xFF;
	_sending[2] = 0x06;
	_sending[3] = 0x03;
	_sending[4] = 0x01;
	_sending[5] = 0x00;
	_sending[6] = num; // songs
	_sending[7] = 0xFE;
	_sending[8] = 0xF7-num;
	_sending[9] = 0xEF;
	mp3->uart_write(_sending, 10);
	vTaskDelay(1000);	
}

void next()
{
	uint8_t _sending[10];
	_sending[0] = 0x7E;
	_sending[1] = 0xFF;
	_sending[2] = 0x06;
	_sending[3] = 0x01;
	_sending[4] = 0x01;
	_sending[5] = 0x00;
	_sending[6] = 0x00;
	_sending[7] = 0xFE;
	_sending[8] = 0xF9;
	_sending[9] = 0xEF;
	mp3->uart_write(_sending, 10);
	vTaskDelay(1000);	
}

void pause()
{
	uint8_t _sending[10];
	_sending[0] = 0x7E;
	_sending[1] = 0xFF;
	_sending[2] = 0x06;
	_sending[3] = 0x0E;
	_sending[4] = 0x01;
	_sending[5] = 0x00;
	_sending[6] = 0x00;
	_sending[7] = 0xFE;
	_sending[8] = 0xEC;
	_sending[9] = 0xEF;
	mp3->uart_write(_sending, 10);
	vTaskDelay(1000);
}
void start()
{
	uint8_t _sending[10];
	_sending[0] = 0x7E;
	_sending[1] = 0xFF;
	_sending[2] = 0x06;
	_sending[3] = 0x0D;
	_sending[4] = 0x01;
	_sending[5] = 0x00;
	_sending[6] = 0x00;
	_sending[7] = 0xFE;
	_sending[8] = 0xED;
	_sending[9] = 0xEF;
	mp3->uart_write(_sending, 10);
	vTaskDelay(1000);
}

void brightness(int k)
{
	if(k <= 8){
		int j;
		for(j = 0; j < 30; j++) 
		{
			gpio->gpio_write(GPIO_BITS_MASK_ALL,GPIO_BITS_MASK_ALL);
			vTaskDelay(k);
			gpio->gpio_write(GPIO_BITS_MASK_NONE,GPIO_BITS_MASK_ALL);
			vTaskDelay(8-k);
		}
	}
}

/**
 * \brief  ble_task in FreeRTOS
 * \details 
 * \param[in] *par
 *//*
static void ble_task(void * par)
{
	while(1)
	{	
		uint32_t rd_avail;
		uint32_t cnt;
		cnt = 1;
		char read_message[1];
		ble->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
		cnt = (cnt > rd_avail)?rd_avail:cnt;
		if (cnt > 0) {
			ble->uart_read(read_message, cnt);
			EMBARC_PRINTF("%c",read_message[0]);
		}
		if(strstr(read_message, "l") != NULL) {
			led_off = 0;
			gpio->gpio_write(GPIO_BITS_MASK_ALL,GPIO_BITS_MASK_ALL);
			EMBARC_PRINTF("Get message l \n");
		}
		if(strstr(read_message, "o") != NULL) {
			led_off = 1;
			gpio->gpio_write(GPIO_BITS_MASK_NONE,GPIO_BITS_MASK_ALL);
			EMBARC_PRINTF("Get message o \n");
		}
		if(led_off) {
			int k = 0; 
			if(respiration >= 2048)
				k = (respiration - 2048)*8/1200;
			else
				k = (2048 - respiration)*8/1200;
			brightness(k);
			EMBARC_PRINTF("%d",k);
		}
		read_message[0] = " ";
		vTaskDelay(1);
	}
}*/

/** emsk on-board gpio init, gpio default off */
void my_emsk_gpio_init(void)
{
	gpio = gpio_get_dev(DW_GPIO_PORT_C);

	EMSK_GPIO_CHECK_EXP_NORTN(gpio != NULL);

	if (gpio->gpio_open(GPIO_BITS_MASK_ALL) == E_OPNED) {
		gpio->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_BITS_MASK_ALL));
		gpio->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_BITS_MASK_ALL));
	}

	gpio->gpio_write(GPIO_BITS_MASK_NONE, GPIO_BITS_MASK_ALL);

error_exit:
	return;
}

/** emsk on-board iic init */
/**
 * \brief	iic init
 * \param[in]	slv_addr	iic slave address
 * \retval	E_OK	init success
 * \retval	!E_OK	init failed
 */
int32_t my_emsk_iic_init(uint32_t slv_addr)
{
	int32_t ercd = E_OK;

	iic_0 = iic_get_dev(DW_IIC_0_ID);

	EMSK_IIC_CHECK_EXP_NORTN(iic_0!=NULL);

	ercd = iic_0->iic_open(DEV_MASTER_MODE, IIC_SPEED_HIGH);
	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = iic_0->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));
		iic_slvaddr = slv_addr;
	}
	
	iic_1 = iic_get_dev(DW_IIC_1_ID);

	EMSK_IIC_CHECK_EXP_NORTN(iic_1!=NULL);

	ercd = iic_1->iic_open(DEV_MASTER_MODE, IIC_SPEED_HIGH);
	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = iic_1->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));
		iic_slvaddr = slv_addr;
	}
error_exit:
	return ercd;
}

/** emsk on-board ble init */
void my_emsk_ble_init(void)
{
	ble = uart_get_dev(DW_UART_0_ID);

	ble->uart_open(UART_BAUDRATE_9600);

error_exit:
	return;
}
/** @} */

/** emsk on-board mp3 init */
void my_emsk_mp3_init(void)
{
	mp3 = uart_get_dev(DW_UART_2_ID);

	mp3->uart_open(UART_BAUDRATE_9600);

error_exit:
	return;
}
/** @} */
