/*author:* Qiuwei Wu */

#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "mathh.h"
#include "u8g.h"
#include <string.h>
#include "arc.h"
#define FOLLOW_DISTANCE  120 //跟随距离
#define STOP_DISTANCE  80
#define LOST_DISTANCE 300
#define ALARM_DISTANCE 400
#define BOARD_TEMP_I2C_SLAVE_ADDRESS  TEMP_I2C_SLAVE_ADDRESS
u8g_t u8g;
DEV_UART *dev_uart0 = NULL;
uint32_t dis_l,dis_r,dis_m;
static volatile int pwm1=0;
static volatile int pwm2=0;
static volatile int t0 = 0;
static volatile int t1 = 0;
static volatile int t2 = 0;
static volatile int t3 = 0;
static DEV_GPIO_PTR PMOD3_L;	
static DEV_GPIO_PTR PMOD4_L;
static DEV_GPIO_PTR PMOD5_U;
uint32_t turn_left=0,turn_right=0;
uint32_t right_obstacle,left_obstacle;
static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}
static void car_init(void)////j3 7 8 9 10  //A16 17 18 19  25-31
{
	PMOD3_L = gpio_get_dev(DW_GPIO_PORT_A);
	PMOD3_L->gpio_open(0xf9ff0000);
	PMOD3_L->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0xf9ff0000);
	PMOD4_L  = gpio_get_dev(DW_GPIO_PORT_D);
	PMOD4_L->gpio_open(0xf0);//d7,6
	PMOD4_L->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0xc0);
	PMOD5_U  = gpio_get_dev(DW_GPIO_PORT_C);//c27 26
	PMOD5_U->gpio_open(0xc000000);
	PMOD5_U->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0xc000000);
}
void u8g_prepare(void) 
{
	u8g_SetFont(&u8g, u8g_font_7x14B);		
	u8g_SetFontRefHeightExtendedText(&u8g);		
	u8g_SetDefaultForegroundColor(&u8g);		
	u8g_SetFontPosTop(&u8g);			
}
static void draw(int32_t temp, uint16_t pmm) 
{	
	char s[]="Temperature:  .  C";		//温度
	s[12]=(int)(temp/100)+48;
	s[13]=(int)((temp-((int)(temp/100))*100)/10)+48;
	s[15]=(int)(temp%10)+48;
	u8g_DrawStr(&u8g, 0, 15, s);			
	char s1[]="PM2.5:     ug/m3";			//pm2.5
	//s1[6]=(pmm/1000)+48;	
	pmm= pmm % 1000;
	s1[7]=(pmm/100)+48;
	pmm = pmm % 100;
	s1[8]=(pmm/ 10)+48;
	s1[9]=(pmm% 10)+48;
	u8g_DrawStr(&u8g, 0, 45, s1);	
}
static void timer1_isr(void *ptr)
{
	timer_int_clear(TIMER_1);
        int32_t temp1;
	uint16_t pm;
        int32_t temp_val;
	if (temp_sensor_read(&temp_val) == E_OK) 
	{	   
	   temp1=temp_val;
	}
        else
	{
	   EMBARC_PRINTF("Unable to read temperature sensor value, please check it!\r\n");
	}
        pm=receive_pm();
	u8g_FirstPage(&u8g);
	do {
	      draw(temp1,pm);
	    } while (u8g_NextPage(&u8g));
}
static void timer0_isr(void *ptr)
{
	timer_int_clear(TIMER_0);
	t0++;
	t1++;
	t2++;
	t3++;	
        if(t0>=20)
	   t0=0;
	else
	{
	 if(t0<pwm1)
		 PMOD5_U->gpio_write(0x8000000, 0x8000000);
	 else  
		 PMOD5_U->gpio_write(0x0000000, 0x8000000);
	}	   
        if(t1>=20)
	   t1=0;
	else
	{
	 if(t1<pwm2)
		 PMOD5_U->gpio_write(0x4000000, 0x4000000);    
	 else 
		 PMOD5_U->gpio_write(0x0000000, 0x4000000);
	}
	if(t2>=20)
	   t2=0;
	else
	{
	 if(t2<pwm1)
		 PMOD4_L->gpio_write(0x80, 0x80);
	 else  
		 PMOD4_L->gpio_write(0x00, 0x80);
	}       
        if(t3>=20)
	   t3=0;
	else
	{
	 if(t3<pwm2)
		 PMOD4_L->gpio_write(0x40, 0x40);
	 else  
		 PMOD4_L->gpio_write(0x00, 0x40);
	}
}
static void forward(void)
{   
	pwm1=16;
	pwm2=16;
	PMOD3_L->gpio_write(0xa0ca0000, 0xf0cf0000); 	
}
static void left(void)
{
	pwm1=11;
	pwm2=11;
        PMOD3_L->gpio_write(0x80c20000, 0xf0cf0000);
}
static void right(void)
{   
        pwm1=11;
	pwm2=11;
	PMOD3_L->gpio_write(0x20c80000, 0xf0cf0000); 
}
static void back(void)
{
	pwm1=13;
	pwm2=13;
	PMOD3_L->gpio_write(0x50c50000, 0xf0cf0000);  
}
static void stop(void)
{ 
	PMOD3_L->gpio_write(0x00c00000, 0xf0cf0000); 
}
int main(void)
{
	uint8_t rcv_buf[28];
	int a0_to_a1;//基站0到基站1距离
	int a0_to_a2;//基站0到基站2距离
	int a1_to_a2;//基站1到基站2距离
        int t_to_a0;//标签到基站0距离
	int t_to_a1;//标签到基站1距离
	int t_to_a2;//标签到基站2距离
        double a=20.0;//等边三角形边长
	double b,c,d,f,jiaodu,y;//坐标表示
	uint32_t rcv_cnt;
	uint32_t i = 0;
	uint32_t baudrate = 115200;
	uint32_t rd_avail = 0;             
        int32_t ercd;	
	cpu_lock();	
	board_init();
	ercd = temp_sensor_init(BOARD_TEMP_I2C_SLAVE_ADDRESS);
	if (ercd != E_OK) {
		EMBARC_PRINTF("Temperature sensor open failed\r\n");
	}
        int_disable(INTNO_TIMER1);	
	timer_stop(INTNO_TIMER1);
	int_handler_install(INTNO_TIMER1, timer1_isr);
	int_enable(INTNO_TIMER1);
	timer_start(TIMER_1, TIMER_CTRL_IE, BOARD_CPU_CLOCK);
	cpu_unlock();
	car_init();
	hc_sr04_init();
	uart2_init();
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, U8G_COM_SSD_I2C);
	u8g_Begin(&u8g); 
	u8g_prepare();
	dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0->uart_open(baudrate);
        if (timer_present(TIMER_0))
		{
		  timer_stop(TIMER_0);
		  int_handler_install(INTNO_TIMER0,timer0_isr);
		  int_enable(INTNO_TIMER0);
		 }
	timer_start(TIMER_0,1,BOARD_CPU_CLOCK/2000);
	while (1)
	{ 
	   delay_ms(60);
	   dis_m=hcsr_m();
	   dis_l=hcsr_l();
	   dis_r=hcsr_r();	
	   dev_uart0->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
		if(rd_avail > 0){
			rcv_cnt = dev_uart0->uart_read(rcv_buf, rd_avail);
			if((rcv_buf[0]==0xd6)&&(rcv_buf[1]==0x6d))
				{
					   if(rcv_buf[2]==1)
					   {	
					        a0_to_a1=(rcv_buf[9]<<8) |(rcv_buf[8]) ;
						a0_to_a2=(rcv_buf[11]<<8) |(rcv_buf[10]) ;
						a1_to_a2=(rcv_buf[13]<<8) |(rcv_buf[12]) ;
						t_to_a0=(rcv_buf[21]<<8) |(rcv_buf[20]) ;
						t_to_a1=(rcv_buf[23]<<8) |(rcv_buf[22]) ;
						t_to_a2=(rcv_buf[25]<<8) |(rcv_buf[24]) ;
					   }
					   else if(rcv_buf[2]==2)
					   {
					        t_to_a0=(rcv_buf[7]<<8) |(rcv_buf[6]) ;
					        t_to_a1=(rcv_buf[9]<<8) |(rcv_buf[8]) ;
					        t_to_a2=(rcv_buf[11]<<8) |(rcv_buf[10]);
					        a0_to_a1=(rcv_buf[23]<<8) |(rcv_buf[22]) ;
						a0_to_a2=(rcv_buf[25]<<8) |(rcv_buf[24]) ;
						a1_to_a2=(rcv_buf[27]<<8) |(rcv_buf[26]) ;
					   }					   
			                   y=(t_to_a0*t_to_a0-t_to_a1*t_to_a1)/(2*a);//标签y方向坐标
					   b=2*a*a+2*t_to_a2*t_to_a2-t_to_a0*t_to_a0-t_to_a1*t_to_a1;
					   c=t_to_a0*t_to_a0+t_to_a1*t_to_a1-2*a*a-2*t_to_a2*t_to_a2;					   
					   d=t_to_a0*t_to_a0-t_to_a1*t_to_a1;
					   f=sqrt1(c*c+3*d*d);
					   jiaodu=b/f;  					
            	                           jiaodu=acos1(jiaodu)*180.0/PI;      ////////基于三点定位原理，建立空间直角坐标系，计算方位角
					   printf("jiaodu=%lf\n",jiaodu);
					   if(t_to_a2>FOLLOW_DISTANCE&&t_to_a2<LOST_DISTANCE) //如果距离大于跟随距离
						{   
						   if(right_obstacle>0)
							    right_obstacle--;								 
						   if(left_obstacle>0)
								left_obstacle--;
						   if(turn_left)		 
							{					
							    if(dis_m<80)	//正前方障碍物距离小于80cm，继续转，否则前进。
								   {
								      left();
								   }
							    else
								   {
									  turn_left = 0;
									  right_obstacle = 2;
								   }
						        }
						   else if(turn_right)
							{
								if(dis_m<80)
								   { 
								      right();
								   }
							    else
								   {
									  turn_right = 0;
									  left_obstacle = 2;
								   }
							}
						   else if(dis_l<30||dis_m<30||dis_r<30) //如果障碍物距离小于30，小车后退
							{
								back();
							}
						   else if(dis_l<50||dis_r<50)//如果障碍物距离小于50，小车朝另一边走。
							{
								if(dis_l<50)
								   {
									  right();
									  left_obstacle = 2;
								   }
							    else
								   {
									  left();
									  right_obstacle  =2;
								   }
							}
						   else if(y<=0&&(jiaodu>=40&&dis_l>=80||jiaodu>=70)&&left_obstacle==0)//标签在小车左边40度，并且左边没有障碍物。或者标签在小车左边大于70度。朝标签方向旋转。
							{ 
								left();	  								   
							}
						   else if (y>0&&(jiaodu>40&&dis_r>80||jiaodu>70)&&right_obstacle==0)
							{		   							  
							    right();								  
							}
						   else if(dis_m<50) //如果正前方障碍物小于50cm
							{
								if(dis_r>50||dis_l>50)//左边或者右边障碍物大于50cm，则朝没有障碍物的地方转。
								   {
									  if(dis_r>dis_l)	//1：进入朝右转模式，一直转到正前方没有障碍物，再往前走。
									  {		 
									     turn_right=1; 
									  }
									  else
									  {		 
									     turn_left = 1;	  //进入朝左转模式，一直转到正前方没有障碍物，再往前走。
									  }
								   }
							}
						   else if(right_obstacle==0&&left_obstacle==0) //标签在正前方，并且左右没有障碍物	 
						        {
							    forward();
						        }
						   else
							    stop();
					        }
                                           if(t_to_a2<FOLLOW_DISTANCE)
					       stop(); 
					   if(t_to_a2>=LOST_DISTANCE&&t_to_a2<ALARM_DISTANCE)
						   beep();
					   if(t_to_a2>=ALARM_DISTANCE)
						   {
							    stop();
							    call();
						   }
				}
		}
	}
	return E_SYS;	
}

/** @} */
