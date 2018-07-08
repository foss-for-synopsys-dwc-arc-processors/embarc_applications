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
/*
	步进电机初始化程序
	指定步进电机连接的GPIO端口和端口mask
	参数： 	gpio_port  输入，GPIO端口，可选值为 DW_GPIO_PORT_A 
												DW_GPIO_PORT_B
												DW_GPIO_PORT_C
												DW_GPIO_PORT_D
			gpio_port_mask  输入，GPIO端口MASK，可选值为0x00 --- 0xFFFFFFFF
							需要用到哪一位就把相应的位置位

	用例：step_motor_init( DW_GPIO_PORT_A, 0xF000)  代表步进电机的in1~in4分别连接PORT A的12,13,14，15位
*/
void step_motor_init(uint32_t gpio_port, uint32_t gpio_port_mask)
{
	step_motor_port = gpio_get_dev(gpio_port);
	port_bit_mask = gpio_port_mask;

	step_motor_port->gpio_open(GPIO_DIR_OUTPUT);
	step_motor_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(port_bit_mask));
	step_motor_port->gpio_write(0x00,port_bit_mask);
}


/*
	步进电机控制程序
	控制步进电机的旋转方向，旋转角度，旋转速度
	参数：	step_dir	输入，旋转方向，可选值为	CW_DIR --- 顺时针旋转
													CCW_DIR --- 逆时针旋转
			step_angle	输入，旋转角度，可选值为	1 --- 65534
			step_speed	输入，旋转速度，可选值为	1 --- 254
*/
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