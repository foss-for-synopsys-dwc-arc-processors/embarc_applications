#include "buzzer.h"

#define BUZZER_MASK 0x00010000

void buzzer_init(void)
{
	buzzer = gpio_get_dev(DW_GPIO_PORT_A);//Pmod3_pin7<--->BUZZER
	buzzer->gpio_open(BUZZER_MASK);
	buzzer->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)BUZZER_MASK);
	buzzer->gpio_write(0x00010000,BUZZER_MASK);
}

void buzzer_open(void)
{
	buzzer->gpio_write(0x00000000,BUZZER_MASK);
}

void buzzer_close(void)
{
	buzzer->gpio_write(0x00010000,BUZZER_MASK);
}
