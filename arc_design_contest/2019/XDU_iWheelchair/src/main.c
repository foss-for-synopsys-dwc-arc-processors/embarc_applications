
#include "embARC.h"
#include "embARC_debug.h"

#include "board.h"
#include "dev_gpio.h"
#include "dev_uart.h" 

#include "stdio.h"

#include "ad7991.h"
#include "hm1x.h"

#include "ble.h"
#include "M6.h"
#include "drive_control.h"
#include "buzzer.h"


uint32_t baudrate_ble = 9600;

static AD7991_DEF_PTR PmodAD2;

static float ad7991_data[4];
static float a0 = 0.0;
static float t = 0.0;

static uint8_t flag_f = 0;
static uint8_t flag_b = 0;

volatile static int t0 = 0;

static void ad7991_init(void);

static void delay_us(uint32_t us)
{
	uint32_t i;
	for(i=us;i>0;i--);
}

/********pmodAD2 初始化*********/
void ad7991_init(void)
{
		
	ad7991_data[0]=0;
	ad7991_data[1]=0;
	ad7991_data[2]=0;
	ad7991_data[3]=0;
}

/********遇到前障碍后退躲避********/
void back_up(void)
{
	car_set(6,PWM_L,PWM_R);
	board_delay_ms(4000,1);
	car_set(0,PWM_L,PWM_R);
}

/********遇到后障碍前进躲避********/
void fore_up(void)
{
	car_set(1,PWM_L,PWM_R);
	board_delay_ms(4000,1);
	car_set(0,PWM_L,PWM_R);
}

/********TIMER0 实现pmodAD2采样********/
void timer0_isr(void *ptr)
{
	timer_int_clear(TIMER_0);
	AD7991_DEFINE(PmodAD2,DW_IIC_0_ID,ADC_I2C_SLAVE_ADDRESS);	
	
	ad7991_adc_init(PmodAD2);
	ad7991_adc_read(PmodAD2,ad7991_data);
	
	//printf("a0 = %f\n",ad7991_data[0]);
	
	/*通道0为前距离传感器采样，通道1为后距离传感器采样，通道2为倾斜开关采样*/	
	if(ad7991_data[0]>1840.0|ad7991_data[0]<1140.0) //1.84V<-->29cm,1.14V<-->53cm
	{
		//EMBARC_PRINTF("STOP\n");
		car_set(0,PWM_L,PWM_R);
		buzzer_open();
		flag_f = 1;
	}

	if(ad7991_data[1]>1840.0|ad7991_data[1]<1140.0) 
	{
		//EMBARC_PRINTF("STOP\n");
		car_set(0,PWM_L,PWM_R);
		flag_b = 1;
		//buzzer_open();
		
	}
	/*
	if(ad7991_data[2]>500.0) //倾斜开关打开
	{
		EMBARC_PRINTF("STOP\n");
		car_set(0,PWM_L,PWM_R);
		buzzer_open();
	}
	*/
}

/*******TIMER1 实现PWM调速********/
void timer1_isr(void *ptr)
{
	timer_int_clear(TIMER_1);
	//left_PWM <---> Pmod6_pin9 <---> EN
	if(t0 < PWM_L) 
		{		
			PMOD_L->gpio_write(0x40000000,L298N_L_MASK);
		} 
	else 
		{
			PMOD_L->gpio_write(0x00000000,L298N_L_MASK);
		}
	//right_PWM <---> Pmod6_pin10 <---> EN
	if(t0 < PWM_R)
		{		
			PMOD_L->gpio_write(0x80000000,L298N_R_MASK);
		} 
	else 
		{
			PMOD_L->gpio_write(0x00000000,L298N_R_MASK);
		}
	t0++;
	if(t0 >= 10)  t0 = 0;
}

void time_init(void)
{
	timer_stop(TIMER_0);
	int_handler_install(INTNO_TIMER0,timer0_isr);
	int_enable(INTNO_TIMER0);
	timer_start(TIMER_0,TIMER_CTRL_IE,BOARD_CPU_CLOCK/2);
	
	timer_stop(TIMER_1); 
	int_handler_install(INTNO_TIMER1, timer1_isr);
	int_enable(INTNO_TIMER1);
	timer_start(TIMER_1,TIMER_CTRL_IE,BOARD_CPU_CLOCK/2000); 
}


int main(void)
{
	static uint8_t ble_rcv_buf[20];	
	static int32_t ble_rcv_cnt = 0;

	static uint8_t M6_rcv_buf[33];
	static uint32_t M6_rcv_cnt=20;
	uint32_t rd_avail;
	
	cpu_lock();
	board_init();
	time_init();
	cpu_unlock();

	//ble_init();	
	HM1X_DEFINE(hm1x, HM_1X_UART_ID);
	hm1x_init(hm1x, baudrate_ble);
	M6_init();
	L298N_init();
	buzzer_init();

	car_set(0,PWM_L,PWM_R);
	PWM_L = speed;
	PWM_R = speed;
	
	while(1)
	{
		//EMBARC_PRINTF("start\n");

		ble_rcv_cnt = hm1x_read(hm1x, ble_rcv_buf, sizeof(ble_rcv_buf));
		ble_rcv_buf[ble_rcv_cnt] = '\0';
		
		//检测避障状态为前障
		if(flag_f&(!flag_b))
		{
			board_delay_ms(2000,1);			
			flag_f = 0;			
			buzzer_close();
			back_up();
		}
		
		//检测避障状态为后障
		if(flag_b&(!flag_f))
		{
			board_delay_ms(2000,1);			
			flag_b = 0;			
			fore_up();
		}

		if(flag_f&flag_b)
		{
			car_set(0,PWM_L,PWM_R);
			flag_f = 0;
			flag_b = 0;
		}		

		if(ble_rcv_cnt) 
		{
			EMBARC_PRINTF("%s", ble_rcv_buf);
			ble_control(ble_rcv_buf);	
		} 

		M6->uart_control(UART_CMD_GET_RXAVAIL,(void *)(&rd_avail));
		M6_rcv_cnt = (20 > rd_avail)?rd_avail:M6_rcv_cnt;

		//EMBARC_PRINTF("M6_rcv_cnt=%d\n",M6_rcv_cnt);	

		if(M6_rcv_cnt>0)
		{
			M6->uart_read(M6_rcv_buf,M6_rcv_cnt);	
			EMBARC_PRINTF("%d\n",M6_rcv_buf[M6_rcv_cnt-1]);
			
			voice_control(M6_rcv_buf,M6_rcv_cnt);
		}
	}
	
	return E_SYS;
}
