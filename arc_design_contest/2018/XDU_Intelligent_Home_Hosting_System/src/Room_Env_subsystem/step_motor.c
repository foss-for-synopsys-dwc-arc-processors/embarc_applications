#include "embARC.h"
#include "embARC_debug.h"

#include "dev_gpio.h"
#include "step_motor.h"

#define SPEED_FACT 1400

static DEV_GPIO_PTR step_motor_port; 
static uint32_t port_bit_mask;

static void delay_us( uint32_t us) 
{
	uint32_t volatile x,y;
	for(x=2;x>0;x--)
		for(y=us;y>0;y--);
	
}

void step_motor_init(uint32_t gpio_port, uint32_t gpio_port_mask)
{
	step_motor_port = gpio_get_dev(gpio_port);
	port_bit_mask = gpio_port_mask;

	step_motor_port->gpio_open(GPIO_DIR_OUTPUT);
	step_motor_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(port_bit_mask));
	step_motor_port->gpio_write(0x00,port_bit_mask);
}



void step_motor_active(uint8_t step_dir, uint16_t step_angle, uint8_t step_speed)
{
	switch(step_dir)
	{
		case CW_DIR:	
			for(uint16_t angle = step_angle; angle > 0; angle--)
			{
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x77777777,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x33333333,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0xBBBBBBBB,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x99999999,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0xDDDDDDDD,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0xCCCCCCCC,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0xEEEEEEEE,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x66666666,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x00000000,port_bit_mask);
			};
			break;
		case CCW_DIR:
			for(uint16_t angle = step_angle; angle > 0; angle--)
			{
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x66666666,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0xEEEEEEEE,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0xCCCCCCCC,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT);
				step_motor_port->gpio_write(0xDDDDDDDD,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x99999999,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0xBBBBBBBB,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x33333333,port_bit_mask);
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x77777777,port_bit_mask);	
				delay_us( (255 - step_speed)*SPEED_FACT );
				step_motor_port->gpio_write(0x00000000,port_bit_mask);			
			};
			break;
		default: break;
	}
}