#include "dev_gpio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "ultrasonic.h"
uint32_t dis_l,dis_r,dis_m;
uint32_t data_m,data_l,data_r,val,echo_m,echo_l,echo_r,rx,jishu,ji1_m,ji2_m,ji1_l,ji2_l,ji1_r,ji2_r;	
uint32_t icount,icount_flag;
static DEV_GPIO_PTR PMOD1;//超声波
static DEV_GPIO_PTR PMOD3_R;//超声波

 void delay(uint32_t z)
{
	volatile uint32_t x,y;
	for(x=20;x>0;x--)
		for(y=z;y>0;y--);
}

 void hc_sr04_init(void)
{
	PMOD1 = gpio_get_dev(DW_GPIO_PORT_A);
	PMOD1->gpio_open(0xf00);
	PMOD1->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x100);//输入 J1*7/A8 Em 
	PMOD1->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x200);//输出 J1*8/A9 Tm
	PMOD1->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x400);//输入 J1*9/A10 El
	PMOD1->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x800);//输出 J1*10/A11 Tl	
	
	PMOD3_R = gpio_get_dev(DW_GPIO_PORT_D);
	PMOD3_R->gpio_open(0x30);
	PMOD3_R->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x10);//输入 J3*1/C16 ER 
	PMOD3_R->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x20);//输出 J3*2/C17 TR
}


 float hcsr_m(void)                   //m
{  
   //     timer_stop(INTNO_TIMER1);	
		
		PMOD1->gpio_read(&data_m, 0x800);
		PMOD1->gpio_write(0x800, 0x800);//输出高电平
		PMOD1->gpio_read(&data_m, 0x800);//读高电平
	    delay(20);
		PMOD1->gpio_read(&data_m, 0x800);
		PMOD1->gpio_write(0x000, 0x800);
		PMOD1->gpio_read(&data_m, 0x800);
		delay(30);
		
		icount=0;icount_flag=0;
		while(!(echo_m==0x400))
		{
		 PMOD1->gpio_read(&echo_m,0x400);
		 icount++;
		 if(icount>400000)//可以不用这么大的数值。3米对应的值就可以
			{
				icount_flag=1;
				break;
			}
		}
	//	timer_start(TIMER_1, TIMER_CTRL_IE, BOARD_CPU_CLOCK/2000);
		timer_current(TIMER_1, &ji1_m);
			while(!(echo_m==0x0))
			{
				PMOD1->gpio_read(&echo_m,0x400);
			}
		timer_current(TIMER_1, &ji2_m);
		ji2_m=ji2_m-ji1_m;
		if(icount_flag==0)
		{
			dis_m=ji2_m*0.00085;
			EMBARC_PRINTF("dis_m:%d\n", dis_m);//检验
		}	
		return dis_m;
}

 float hcsr_l(void)     //l
{
    //    timer_stop(INTNO_TIMER1);	
		PMOD1->gpio_read(&data_l, 0x200);
		PMOD1->gpio_write(0x200, 0x200);//输出高电平
		PMOD1->gpio_read(&data_l, 0x200);//读高电平
        delay(20);
		PMOD1->gpio_read(&data_l, 0x200);	
		PMOD1->gpio_write(0x000, 0x200);//输出高电平
		PMOD1->gpio_read(&data_l, 0x200);//读高电平	
		delay(30);
		icount=0;icount_flag=0;
		while(!(echo_l==0x100))
		{
		 PMOD1->gpio_read(&echo_l,0x100);
		 icount++;
		 if(icount>400000)//可以不用这么大的数值。3米对应的值就可以
			{
				icount_flag=1;
				break;
			}
		}
	///	timer_start(TIMER_1, TIMER_CTRL_IE, BOARD_CPU_CLOCK/2000);
		timer_current(TIMER_1, &ji1_l);
			while(!(echo_l==0x000))
			{
				PMOD1->gpio_read(&echo_l,0x100);
				//EMBARC_PRINTF("echo:%d\n", echo_l);//检验
			}
		timer_current(TIMER_1, &ji2_l);	
		ji2_l=ji2_l-ji1_l;
		if(icount_flag==0)
		{
			dis_l=ji2_l*0.00085;
			EMBARC_PRINTF("dis_l:%d\n", dis_l);//检验
		}	
		return dis_l;
}

 float hcsr_r(void)
{
        //   timer_stop(INTNO_TIMER1);	
	       PMOD3_R->gpio_read(&data_r, 0x20);
	       PMOD3_R->gpio_write(0x20, 0x20);//输出高电平
	       PMOD3_R->gpio_read(&data_r, 0x20);//读高电平
	       delay(20);
	       PMOD3_R->gpio_read(&data_r, 0x20);
	       PMOD3_R->gpio_write(0x00, 0x20);
	       PMOD3_R->gpio_read(&data_r, 0x20);
	       delay(30);
			icount=0;icount_flag=0;
			while(!(echo_r==0x10))
			{
			 PMOD3_R->gpio_read(&echo_r,0x10);
			 icount++;
			 if(icount>400000)//可以不用这么大的数值。3米对应的值就可以
				{
					icount_flag=1;
					break;
				}
			}
		//	timer_start(TIMER_1, TIMER_CTRL_IE, BOARD_CPU_CLOCK/2000);
			timer_current(TIMER_1, &ji1_r);
				while(!(echo_r==0x0))
				{
					PMOD3_R->gpio_read(&echo_r,0x10);
				}
			timer_current(TIMER_1, &ji2_r);
        	ji2_r=ji2_r-ji1_r;	
			if(icount_flag==0)
			{
				dis_r=ji2_r*0.00085;
				EMBARC_PRINTF("dis_r:%d\n", dis_r);//检验
			}	
			return dis_r;
}


