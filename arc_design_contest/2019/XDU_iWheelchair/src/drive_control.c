#include "drive_control.h"


/*L298N_init*/
void L298N_init(void)
{
	
	PMOD_L = gpio_get_dev(DW_GPIO_PORT_A);
	PMOD_L->gpio_open(L298N_L_MASK);
	PMOD_L->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)L298N_L_MASK);
	
	PMOD_R = gpio_get_dev(DW_GPIO_PORT_A);
	PMOD_R->gpio_open(L298N_R_MASK);
	PMOD_R->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)L298N_R_MASK);

	PMOD_F = gpio_get_dev(DW_GPIO_PORT_A);
	PMOD_F->gpio_open(L298N_F_MASK);
	PMOD_F->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)L298N_F_MASK);

	PMOD_H = gpio_get_dev(DW_GPIO_PORT_C);
	PMOD_H->gpio_open(L298N_H_MASK);
	PMOD_H->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)L298N_H_MASK);

}


/**
 * \brief Control move.
 * \param dire.Direction of move.
 * \param pwm_left.Left PWM control.
 * \param pwm_right.Right PWM control.
*/
void car_set(uint32_t dire,uint32_t pwm_left, uint32_t pwm_right)
{
	switch(dire)
	{
		//stop		
		case 0: 
			PWM_L = 0;
			PWM_R = 0;
			break;
		//forward
		case 1: 
			PMOD_F->gpio_write(0x30000000,L298N_F_MASK);
			PMOD_H->gpio_write(0x00000000,L298N_H_MASK);
			PWM_L = speed;
			PWM_R = speed;
			break;
		//left
		case 2: 
			PMOD_F->gpio_write(0x30000000,L298N_F_MASK);
			PMOD_H->gpio_write(0x00000000,L298N_H_MASK);

			PMOD_R->gpio_write(0x80000000,L298N_R_MASK);
			PWM_L = 0;
			PWM_R = speed;
			break;
		//right
		case 3: 
			PMOD_F->gpio_write(0x30000000,L298N_F_MASK);
			PMOD_H->gpio_write(0x00000000,L298N_H_MASK);

			PMOD_L->gpio_write(0x40000000,L298N_L_MASK);
			PWM_L = speed;
			PWM_R = 0;
			break;
		case 4:		//speed up
			if(speed < 9)
			{
				speed = speed + 1;
				PWM_L = speed;
				PWM_R = speed;
			}		
			break;
		case 5:		//speed down
			if(speed > 0) 
			{
				speed = speed - 1;
				PWM_L = speed;
				PWM_R = speed;
			}
			break;
		//backward
		case 6:	
			PMOD_F->gpio_write(0x00000000,L298N_F_MASK);
			PMOD_H->gpio_write(0xC0000000,L298N_H_MASK);
			PWM_L = speed;
			PWM_R = speed;
			break;	
	}
}