                                                                                                                                                                                                                   #include "dev_gpio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "ultradrive.h"
#include "board.h"
#include "stdio.h"

uint32_t echo,current,now,count,count_point;
uint32_t i;
double dis,distance;
 void delay_us(uint32_t z)

{
	volatile uint32_t x,y;
	for(x=20;x>0;x--)
		for(y=z;y>0;y--);
}

void UltrasonicWave_Init(void)
{
  PMOD_O = gpio_get_dev(DW_GPIO_PORT_C);
  PMOD_O->gpio_open(ULTRA_1_MASK);
  PMOD_O->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)ULTRA_2_MASK);
  PMOD_O->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)ULTRA_3_MASK);
}

void UltrasonicWave_StartMeasure(void)
{
	PMOD_O->gpio_write(0x1000000,ULTRA_3_MASK);
	delay_us(20);
	PMOD_O->gpio_write(0x0000000,ULTRA_3_MASK);	          
}

float UltrasonicWave_Measure(void)   
{
   i=0;
   distance=0;
  while(i<2){
  	while(!(echo==0x0));
  timer_stop(INTNO_TIMER1);
  board_delay_ms(1,1);
	UltrasonicWave_StartMeasure();
		count=0;count_point=0;
		while(!(echo==0x2000000))
		{
		 PMOD_O->gpio_read(&echo,ULTRA_2_MASK);
		 count++;
		 if(count>40000)
			{
				count_point=1;
				break;
			}
		}	
		timer_start(TIMER_1, TIMER_CTRL_IE, BOARD_CPU_CLOCK);
		timer_current(TIMER_1, &current);
		
			while(!(echo==0x0))
			{
				PMOD_O->gpio_read(&echo,ULTRA_2_MASK);
				
			}
		timer_current(TIMER_1, &now);
		if(count_point==0)
		{
			dis=(now-current)/1000000.0*340/2.0;
			
		}	
		distance=distance+dis;
		i++;
  	}
  printf("distance:%5.2f\n", distance/2);

  return distance/2;
}

	

