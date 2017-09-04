/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"
#include "task.h"
#include "light_mode.h"
#include "mpu6050.h"
#include "imu.h"
#include "scope.h"
#include "blu_int.h"



void main()
{
	#ifdef SCOPE_EN
		scope_init(UART_BAUDRATE_115200);
	#else
		blu_uart_init(UART_BAUDRATE_9600);
	#endif
	led_ctr_gpio_init();
	mic_init();
    mpu6050_init(2,3);
	task_init();
	while(1) 
	{
		vTaskSuspend(NULL);
	}
}