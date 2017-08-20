#include "task.h"

#define led_display_task_prioroties 		( (configMAX_PRIORITIES - 1) | portPRIVILEGE_BIT )
#define pattern_change_task_prioroties 		( (configMAX_PRIORITIES - 2) | portPRIVILEGE_BIT )
#define light_mode_task_prioroties 			( (configMAX_PRIORITIES - 3) | portPRIVILEGE_BIT )
#define light_mode_music_task_prioroties 	( (configMAX_PRIORITIES - 3) | portPRIVILEGE_BIT )
#define mpu_task_prioroties					( (configMAX_PRIORITIES - 4) | portPRIVILEGE_BIT )
#define light_mode_alarm_task_prioroties	( (configMAX_PRIORITIES - 3) | portPRIVILEGE_BIT )

static void led_display_task(void * par);
static void pattern_change_task(void * par);
static void light_mode_task(void * par);
static void light_mode_music_task(void * par);
static void mpu_task(void * par);
static void light_mode_alarm_task(void * par);

static TaskHandle_t led_display_task_handle = NULL;
static TaskHandle_t light_mode_task_handle = NULL;
static TaskHandle_t light_mode_music_task_handle = NULL;
static TaskHandle_t mpu_task_handle = NULL;
static TaskHandle_t light_mode_alarm_task_handle = NULL;
TaskHandle_t pattern_change_task_handle = NULL;
SemaphoreHandle_t recognition_semaphore = NULL;
QueueHandle_t led_queue = NULL;
QueueHandle_t mode_info_queue = NULL;

static void perf_init(unsigned int id);
static void perf_start(void);
static unsigned int perf_end(void);
static void led_turn(uint8_t led);

static uint32_t mpu_cycle;
static uint32_t time_clock;

static volatile uint32_t start = 0;
static uint32_t perf_id = 0xFF;

/**
 * \brief  led display task in FreeRTOS
 * \details led display.
 * \param[in] *par
 */
static void led_display_task(void * par)
{
	RGB_T *pled_rgb = NULL;
	int led_num;

	led_queue = xQueueCreate( 1, sizeof(RGB_T *) );
	if( led_queue == NULL ){
		EMBARC_PRINTF("Failed to create the led queue.\r\n");
		vTaskSuspend(NULL); 
	}

	EMBARC_PRINTF("Light ready\r\n");
	while(1) {
		xQueueReceive( led_queue, &pled_rgb, (TickType_t) LED_WAIT_TIME_MAX );
		taskENTER_CRITICAL();
		delay_10us(5);
		for (led_num = 0; led_num < light_num; led_num++ ) {
			light_send_colordata(pled_rgb[led_num].color,color_row);
		}
		taskEXIT_CRITICAL();
	}
}
/**
 * \brief  pattern change task in FreeRTOS
 * \details Pattern change.
 * \param[in] *par 
 */
static void pattern_change_task(void * par)
{
	mode_info mode_info_now = INIT_MODE_INFO; 
	mode_info mode_info_old = INIT_MODE_INFO;
	uint8_t mode_flag = ACTION_START;

	recognition_semaphore = xSemaphoreCreateBinary();
	if (recognition_semaphore == NULL) {
        EMBARC_PRINTF("Failed to create the recognition semaphore.\r\n");
		vTaskSuspend(NULL); 
    }

	mode_info_queue = xQueueCreate( 1, sizeof( mode_info ) );
	if (mode_info_queue == NULL) {
		EMBARC_PRINTF("Failed to create the mode information queue.\r\n");
		vTaskSuspend(NULL); 
	}

	xQueueOverwrite(mode_info_queue, &mode_info_now);

	EMBARC_PRINTF("pattern change ready\r\n");
	vTaskSuspend(NULL);
	while(1) {

		if (mode_info_old.mode == LIGHT_MODE_MUSIC)
			vTaskSuspend(light_mode_music_task_handle);
		else if (mode_info_old.mode >= MIN_MODE && mode_info_old.mode <= MAX_MODE)
			vTaskSuspend(light_mode_task_handle);
		else
			EMBARC_PRINTF("mode error.\r\n");

		if (xSemaphoreTake( recognition_semaphore, (TickType_t) 0 ) == pdTRUE) {
			EMBARC_PRINTF("recognitioning\r\n");
			vTaskSuspend(NULL);
		}
		light_ctr_reset();
		xQueuePeek(mode_info_queue, &mode_info_now , (TickType_t) 0 );
		/* restart light mode task */
		if (mode_info_now.mode == LIGHT_MODE_TIMING_FIT) {
			time_clock = mode_info_now.time;
			mode_info_now.mode = mode_info_old.mode;
		}
		else if ( mode_info_now.mode != mode_info_old.mode 
			 || mode_info_now.time != mode_info_old.time
			 || mode_info_now.frequence != mode_info_old.frequence
			 || mode_info_now.mode == LIGHT_MODE_SHAKING
			 || mode_info_now.mode == LIGHT_MODE_TIMING_FIT) {	
					
			vTaskDelete(light_mode_task_handle);
			if (xTaskCreate(light_mode_task, "light_mode_task", 128, (void *)1, 
					light_mode_task_prioroties, &light_mode_task_handle) != pdPASS) {
				EMBARC_PRINTF("Restart light mode task error\r\n");
			}
			vTaskSuspend(light_mode_task_handle);
		}

		if (mode_info_now.mode == LIGHT_MODE_MUSIC) {
			vTaskSuspend(mpu_task_handle);
			iic_point_mic();
			vTaskResume(light_mode_music_task_handle);
			EMBARC_PRINTF("Enable light mode music.\r\n");
		}
		else if (mode_info_now.mode >= MIN_MODE && mode_info_now.mode <= MAX_MODE) {
			iic_point_mpu();
			vTaskResume(mpu_task_handle);
			vTaskResume(light_mode_task_handle);
			EMBARC_PRINTF("Enable light mode.\r\n");
		}
		else
		EMBARC_PRINTF("The mode isn't changed.\r\n");
		mode_info_old = mode_info_now;

		led_turn(0);
		vTaskSuspend(NULL);
	}
}
/**
 * \brief  light mode task in FreeRTOS
 * \details Give the signal of light.
 * \param[in] *par
 */
static void light_mode_task(void * par)
{
	mode_info mode_info_now = INIT_MODE_INFO;
	//vTaskSuspend(NULL);
	while(1) {	

		xQueuePeek(mode_info_queue, &mode_info_now , (TickType_t) 0 );
		EMBARC_PRINTF("mode is %d,bright is %d,freq is %d,time is %d,weather is %d \r\n",
				mode_info_now.mode, mode_info_now.bright, mode_info_now.frequence, mode_info_now.time, mode_info_now.weather);
		light_mode_top_pack(mode_info_now.mode, mode_info_now.bright, mode_info_now.frequence, 
					mode_info_now.time, mode_info_now.fance_txt, mode_info_now.weather);
	}
}
/**
 * \brief  light mode music task in FreeRTOS
 * \details mic data update and signal giver to light.
 * \param[in] *par
 */
static void light_mode_music_task(void * par)
{
    TickType_t xLastExecutionTime;
    uint16_t fft_num = 0;
	compx fft_data[fft_N];     /* fft data segment */
	uint16_t fft_in[fft_N];    /* fft data input */
	uint8_t fft_out[fft_N/2];  /* fft data output */

    xLastExecutionTime = xTaskGetTickCount();
   	vTaskSuspend(NULL);

    while(1) {
    	fft_in[fft_num] = adc_get();
		if (++fft_num >= fft_N) {
			fft_num = 0;
	    	fft_Data(fft_in, fft_data);
	    	fft_Convert(fft_data);				/* FFT resolve */
	    	fft_powerMag(fft_data, fft_out);	/* data compress */

	    	light_mode_music(fft_out);
	    	//led_turn(2);														
	 	}
	 	led_turn(2);
        vTaskDelayUntil( &xLastExecutionTime, MIC_PERIOD_TICK);    
        /*!< This task should execute exactly every MIC_PERIOD_TICK. */
    }
}
/**
 * \brief  light mode alarm task in FreeRTOS
 * \details alarm and weather.
 * \param[in] *par
 */
static void light_mode_alarm_task(void * par)
{
    TickType_t xLastExecutionTime;
    mode_info mode_info_now = INIT_MODE_INFO;
    xLastExecutionTime = xTaskGetTickCount();

    while(1) {

    	iic0_point_rtc();
    	//vTaskDelay(10);
    	if ( pcf8563_date_updata( 0, (uint8_t)(time_clock / 60), (uint8_t)(time_clock % 60)) ) {
			xSemaphoreGive( recognition_semaphore );
			vTaskResume( pattern_change_task_handle );
    		light_mode_clock();
    		xQueuePeek(mode_info_queue, &mode_info_now , (TickType_t) 0 );
    		mode_info_now.mode = LIGHT_MODE_WEATHER;
    		xQueueOverwrite( mode_info_queue, &mode_info_now );
    		vTaskResume( pattern_change_task_handle );
    	}

		iic_point_mpu();

    	EMBARC_PRINTF("alarm mode.\r\n");
	 	led_turn(3);
        vTaskDelayUntil( &xLastExecutionTime, ALARM_PERIOD_TICK);    
        /*!< This task should execute exactly every MIC_PERIOD_TICK. */
    }
}
/**
 * \brief  mpu task in FreeRTOS
 * \details MPU read and imu update.
 * \param[in] *par
 */
static void mpu_task(void * par)
{
    TickType_t xLastExecutionTime; 
    uint32_t recognition_cnt = 0;
    mode_info mode_info_now = INIT_MODE_INFO;
    uint8_t mode_flag = 0;
    uint8_t recognition_flag = 0;
    xLastExecutionTime = xTaskGetTickCount();
    while(1) {
        //perf_start();
    	mpu6050_updata();

    	recognition_cnt++;
    	if (recognition_cnt == REC_PERIOD_TICK/MPU_PERIOD_TICK) {
    		recognition_cnt = 0;
    		mode_flag = imu_pattern_recongnition();
    		if (recognition_flag == 0 && mode_flag == ACTION_START) {
    			recognition_flag = 1;
    			light_mode_change_success();
    			xQueuePeek(mode_info_queue, &mode_info_now , (TickType_t) 0 );
    			light_mode_change_flag(mode_info_now.mode);
    			EMBARC_PRINTF("Start of recognition.\r\n");
    			xSemaphoreGive( recognition_semaphore );
				vTaskResume( pattern_change_task_handle );
			}
			else if (recognition_flag == 1 && mode_flag == ACTION_RIGHT) {
				mode_info_now.mode++;
				if ( mode_info_now.mode > MAX_MODE)
					mode_info_now.mode = MIN_MODE;
				light_mode_change_flag(mode_info_now.mode);
				EMBARC_PRINTF("mode ++ %d.\r\n", mode_info_now.mode);
			}
			else if (recognition_flag == 1 && mode_flag == ACTION_LEFT) {
				mode_info_now.mode--;
				if ( mode_info_now.mode < MIN_MODE)
					mode_info_now.mode = MAX_MODE;				
				light_mode_change_flag(mode_info_now.mode);
				EMBARC_PRINTF("mode -- %d.\r\n", mode_info_now.mode);
			}
			else if (recognition_flag == 1 && mode_flag == ACTION_OVER) {
				recognition_flag = 0;
				if (mode_info_now.mode >= MIN_MODE && mode_info_now.mode <= MAX_MODE) {
					xQueueOverwrite( mode_info_queue, &mode_info_now );
					EMBARC_PRINTF("After pattern recognition mode is %d .\r\n", mode_info_now.mode);
				}
				else
					EMBARC_PRINTF("Wrong pattern recognition mode is %d .\r\n", mode_info_now.mode);
				vTaskResume( pattern_change_task_handle );
			}
    	}

        led_turn(1);
        //mpu_cycle = perf_end();
        vTaskDelayUntil(&xLastExecutionTime, MPU_PERIOD_TICK);    
        /*!< This task should execute exactly every MPU_PERIOD_TICK. */
    }
}
/** create all the task at appropriate order */
uint8_t task_init(void)
{
	/* start the timer 1 for */
	perf_init(TIMER_1);

	/* create the pattern change task */
	if (xTaskCreate(pattern_change_task, "pattern_change_task", 128, (void *)1, 
			pattern_change_task_prioroties, &pattern_change_task_handle) != pdPASS) {
		EMBARC_PRINTF("create pattern change task error\r\n");
		return -1;			
	}
	/* create the light display task */
	if (xTaskCreate(led_display_task, "led_display_task", 128, (void *)1, 
			led_display_task_prioroties, &led_display_task_handle) != pdPASS) {
		EMBARC_PRINTF("create light mode running task error\r\n");
		return -1;			
	}
	//light_mode_test();
	/* create the light mode music task */
	if (xTaskCreate(light_mode_music_task, "light_mode_music_task", 128, (void *)1,
			light_mode_music_task_prioroties, &light_mode_music_task_handle) != pdPASS) {
		EMBARC_PRINTF("create light mode music task error\r\n");
		return -1;			
	}
	/* create the light mode task */
	if (xTaskCreate(light_mode_task, "light_mode_task", 128, (void *)1, 
			light_mode_task_prioroties, &light_mode_task_handle) != pdPASS) {
		EMBARC_PRINTF("create light mode task error\r\n");
		return -1;			
	}
	/* create the mpu task */
	if (xTaskCreate(mpu_task, "mpu_task", 128, (void *)1, mpu_task_prioroties, &mpu_task_handle) != pdPASS) {
		EMBARC_PRINTF("create mpu_task error\r\n");
		return -1;			
	}
	// /* create the light mode alarm task */
	// if (xTaskCreate(light_mode_alarm_task, "light_mode_alarm_task", 128, (void *)1,
	// 		light_mode_alarm_task_prioroties, &light_mode_alarm_task_handle) != pdPASS) {
	// 	EMBARC_PRINTF("create light mode alarm task error\r\n");
	// 	return -1;			
	// }	
	
	return 0;
}

/** turn around one bit of led */
static void led_turn(uint8_t led)
{
	uint32_t temp;
	temp = led_read(0x001 << led);
	led_write(~temp, 0x001 << led);
}

/** performance timer initialization */
static void perf_init(unsigned int id)
{
	if (timer_start(id, TIMER_CTRL_NH, 0xFFFFFFFF) < 0) {
		EMBARC_PRINTF("perf timer init failed\r\n");
		while(1);
	}
	perf_id = id;
}

/** performance timer start */
static void perf_start(void)
{
	if (timer_current(perf_id, (void *)(&start)) < 0) {
		start = 0;
	}
}

/** performance timer end, and return the time passed */
static unsigned int perf_end(void)
{
	unsigned int end = 0;

	if (timer_current(perf_id, (void *)(&end)) < 0) {
		return 0;
	}

	if (start < end) {
		return (end - start);
	} else {
		return (0xFFFFFFFF - start + end);
	}
}





