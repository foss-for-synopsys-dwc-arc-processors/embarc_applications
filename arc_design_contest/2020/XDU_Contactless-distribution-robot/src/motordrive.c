#include "motordrive.h"
#include "pwm.h"
#include "ultradrive.h"




void Motor_Init(void)
{

  PMOD_1 = gpio_get_dev(DW_GPIO_PORT_A);
  PMOD_1->gpio_open(MOTOR_1_MASK);
  PMOD_1->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)MOTOR_1_MASK);

  PMOD_2 = gpio_get_dev(DW_GPIO_PORT_C);
  PMOD_2->gpio_open(MOTOR_2_MASK);
  PMOD_2->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)MOTOR_2_MASK);

} 
void Motor_Forward(void)
{
PMOD_1  ->gpio_write(0xF0000000,MOTOR_1_MASK);
PMOD_2  ->gpio_write(0x00000000,MOTOR_2_MASK);
} 

void Motor_Back(void)
{
 PMOD_1  ->gpio_write(0x00000000,MOTOR_1_MASK);
 PMOD_2  ->gpio_write(0xF0000000,MOTOR_2_MASK);
} 


void Motor_Stop(void)
{
PMOD_1 ->gpio_write(0x00000000,MOTOR_1_MASK);
PMOD_2 ->gpio_write(0x00000000,MOTOR_2_MASK);
} 

void Motor_TurnLeft(void)
{ 
PMOD_1 ->gpio_write(0x30000000,MOTOR_1_MASK);
PMOD_2 ->gpio_write(0xC0000000,MOTOR_2_MASK);
}

void Motor_TurnRight(void)
{
PMOD_1 ->gpio_write(0xC0000000,MOTOR_1_MASK);
PMOD_2 ->gpio_write(0x30000000,MOTOR_2_MASK);
}

void Avoid()
{
	uint8_t i=0;
	do{
	float  a; 
	float  l; 
	float  r; 
	a =UltrasonicWave_Measure(); 
	EMBARC_PRINTF("Mo\r\n");	
		if(a>7.00){
				Motor_Forward();
				board_delay_ms(100,1);	
		}			
		if(a<7.00){
			Motor_Stop();
			PWM3_135(); 	 
			board_delay_ms(700,1);		
			l =UltrasonicWave_Measure();  
			board_delay_ms(100,1);	
			PWM3_45();	   
			board_delay_ms(700,1);	
			r =UltrasonicWave_Measure();				
			PWM3_90();	  
			board_delay_ms(100,1);	
			Motor_TurnLeft();
			board_delay_ms(810,1);
			Motor_Forward();
			board_delay_ms(1000,1);	
			Motor_TurnRight();
			board_delay_ms(810,1);
			Motor_Stop();
			board_delay_ms(300,1);		
		}	
	i++;
	}
while(i<50);
}

void car_control(uint8_t x) {
  switch (x) {
  case 1:
for(uint8_t i=0;i<10;i++ )
{
  Avoid();
  Motor_Forward();
  board_delay_ms(100,1);
}
  Motor_TurnRight();
  board_delay_ms(850,1);
for(uint8_t i=0;i<20;i++ )
{
  Avoid();
  Motor_Forward();
  board_delay_ms(100,1);
}	
	Motor_TurnLeft();
	board_delay_ms(850,1);
for(uint8_t i=0;i<10;i++ )
	{
	  Avoid();
	  Motor_Forward();
	  board_delay_ms(100,1);
	}					
	Motor_Stop();
  break;
  case 2:
   for(uint8_t i=0;i<40;i++ )
{
  Avoid();
  Motor_Forward();
  board_delay_ms(100,1);
}
   Motor_TurnLeft();
	   board_delay_ms(850,1);
for(uint8_t i=0;i<10;i++ )
{
  Avoid();
  Motor_Forward();
  board_delay_ms(100,1);
}	
Motor_TurnRight();
board_delay_ms(850,1);
for(uint8_t i=0;i<200;i++ )
	{
	  Avoid();
	  Motor_Forward();
	  board_delay_ms(100,1);
	}					
	Motor_Stop();
  break;
  default:
  	EMBARC_PRINTF("Error...\r\n"); ;  
  break;
  }
}

