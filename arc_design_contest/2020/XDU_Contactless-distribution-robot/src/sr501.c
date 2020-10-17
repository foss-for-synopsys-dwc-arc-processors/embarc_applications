#include "sr501.h"
#include "dev_gpio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
uint32_t SIGN;
void HC_SR501Configuration(void)
{
  PMOD_P = gpio_get_dev(DW_GPIO_PORT_A);
  PMOD_P->gpio_open(SR_1_MASK);
  PMOD_P->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)SR_1_MASK);
}
int HC_SR501_Status(void)
{
       
	PMOD_P->gpio_read(&SIGN,SR_1_MASK);
	if(SIGN==0x100){
		return 1;
	}
	else{
		return 0;
    }	
}