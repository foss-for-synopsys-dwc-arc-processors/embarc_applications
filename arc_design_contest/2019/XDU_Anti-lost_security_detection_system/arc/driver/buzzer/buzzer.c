#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "arc.h"
DEV_GPIO_PTR port_alarm;


static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}

void beep(void)
{


	port_alarm = gpio_get_dev(DW_GPIO_PORT_A);
	port_alarm->gpio_open(0x10000);
	port_alarm->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000);

	
	port_alarm->gpio_write(0x00000, 0x10000);
	delay_ms(8000);
	port_alarm->gpio_write(0x10000, 0x10000);
	delay_ms(11000);
	EMBARC_PRINTF("beep \r\n");

}
