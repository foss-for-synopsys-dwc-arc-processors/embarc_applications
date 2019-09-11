/*author:* Ninh DH */

#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"
#include "mathh.h"
#include <string.h>
#include "arc.h"
DEV_UART *dev_uart0 = NULL;
DEV_UART *dev_uart2 = NULL;


static void delay_us(volatile int time);
static void delay_ms(volatile int z);
static void timer1_isr(void *ptr);
static void timer0_isr(void *ptr);
void uart0_init(void);
void uart2_init(void);
void readSensor1(void);
void readSensor2(void);
void flameLocation(void);
void moveNozzel(void);
void home(void);
void pumpOn(void);	
void pumpOff(void);
void stepMotor(uint8_t number, uint8_t direction, uint32_t step, int stepTime);

uint8_t maxTemper1, xMax1, yMax1;
uint8_t maxTemper2, xMax2, yMax2;
float alpha1, alpha2;
float beta2;
int fx, fy;
float a1, a2;
#define A 30
#define H 60
#define D 35
#define Hn 23

int currentStepX = 0;
int currentStepY = 0;

int main(void)
{
    cpu_lock(); 
    board_init();
    int_disable(INTNO_TIMER1);  
    timer_stop(INTNO_TIMER1);
    int_handler_install(INTNO_TIMER1, timer1_isr);
    int_enable(INTNO_TIMER1);
    timer_start(TIMER_1, TIMER_CTRL_IE, BOARD_CPU_CLOCK);
    cpu_unlock();
    uart0_init();
    uart2_init();
    if (timer_present(TIMER_0))
    {
        timer_stop(TIMER_0);
        int_handler_install(INTNO_TIMER0,timer0_isr);
        int_enable(INTNO_TIMER0);
    }
    timer_start(TIMER_0,1,BOARD_CPU_CLOCK/2000);

    while (1)
    {
    	readSensor1();
    	readSensor2();
    	flameLocation();

    	if (fx == 0 || fy == 0)
    	{
    		pumpOff();
    		home();
    	}
    	else
    	{
    		moveNozzel();
    		pumpOn();
    	}
    	delay_ms(1500);
    }
    return E_SYS;   
}

static void delay_us(volatile int time) //1us
{
    volatile uint32_t i;
    for(i=(int)(time*1.4);i>0;i--);
}

static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}

static void timer1_isr(void *ptr)
{
    timer_int_clear(TIMER_1);
}

static void timer0_isr(void *ptr)
{
    timer_int_clear(TIMER_0);
}

void uart0_init(void)
{
	uint32_t baudrate0 = 115200;	
	dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0->uart_open(baudrate0);
}

void uart2_init(void)
{
	uint32_t baudrate2 = 115200;	
	dev_uart2 = uart_get_dev(DW_UART_2_ID);
	dev_uart2->uart_open(baudrate2);
}

void readSensor1(void)
{
	char send_message_1[1];
	send_message_1[0] = '1';
	dev_uart0->uart_write(send_message_1, 1);
	uint8_t rcv_buf_0[768];
	for (int i = 0; i < 768; i++)
	{
		rcv_buf_0[i] = 0;
	}
	uint32_t rcv_cnt_0;
	uint32_t baudrate0 = 115200;
	uint32_t rd_avail_0 = 0;
	dev_uart0->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail_0));
	if(rd_avail_0 > 0)
	{
		rcv_cnt_0 = dev_uart0->uart_read(rcv_buf_0, 768);		
	}

	maxTemper1 = rcv_buf_0[0];
	xMax1 = 0; yMax1 = 0;
	uint8_t count1 = 1;

	for (int i = 0; i < 768; i++)
	{
		if (rcv_buf_0[i] == maxTemper1)
		{
			count1++;
			xMax1 = xMax1 + i%32;
			yMax1 = yMax1 + (int)(i/24);
		}
		else if (rcv_buf_0[i] > maxTemper1)
		{
			maxTemper1 = rcv_buf_0[i];
			count1 = 1;
			xMax1 = i%32;
			yMax1 = (int)(i/24);
		}
	}

	for (int i = 0; i < 768; i++)
	{
		EMBARC_PRINTF("%d,", rcv_buf_0[i]);
	}
	EMBARC_PRINTF("\n");
}

void readSensor2(void)
{
	char send_message_2[1];
	send_message_2[0] = '1';
	dev_uart2->uart_write(send_message_2, 1);
	uint8_t rcv_buf_2[768];
	for (int i = 0; i < 768; i++)
	{
		rcv_buf_2[i] = 0;
	}
	uint32_t rcv_cnt_2;
	uint32_t baudrate2 = 115200;
	uint32_t rd_avail_2 = 0;
	dev_uart2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail_2));
	if(rd_avail_2 > 0)
	{
		rcv_cnt_2 = dev_uart2->uart_read(rcv_buf_2, 768);		
	}

	maxTemper2 = rcv_buf_2[0];
	xMax2 = 0; yMax2 = 0;
	uint8_t count2 = 1;

	for (int i = 0; i < 768; i++)
	{
		if (rcv_buf_2[i] == maxTemper2)
		{
			count2++;
			xMax2 = xMax2 + i%32;
			yMax2 = yMax2 + (int)(i/24);
		}
		else if (rcv_buf_2[i] > maxTemper2)
		{
			maxTemper2 = rcv_buf_2[i];
			count2 = 1;
			xMax2 = i%32;
			yMax2 = (int)(i/24);
		}
	}
	xMax2 = (int)(xMax2/count2);
	yMax2 = (int)(yMax2/count2);
}

void flameLocation(void)
{
	if (maxTemper1 < 40 || maxTemper2 < 40)
	{
		alpha1 = alpha2 = beta2 = fx = fy = 0;
		return;
	}
	alpha1 = (xMax1 - 16)*(55/32);
	alpha2 = (xMax2 - 16)*(55/32);
	beta2 = (yMax2 - 12)*(35/24);

	if (alpha1 >= 0 && alpha2 <= 0)
	{
		a1 = PI*(90 - alpha1)/180;
		a2 = PI*(90 - fabs(alpha2))/180;
		double x = ((A*tan1(a2))/(tan1(a1) + tan1(a2)));
		// double y = A - x;
		fx = (int)(x*tan1(a1));
		fx = (int)(fx*tan1(PI*fx/180))/2;
	}
	else if (alpha1 >= 0 && alpha2 >= 0)
	{
		a1 = PI*(90 - alpha1)/180;
		a2 = PI*(90 - alpha2)/180;
		double x = ((A*tan1(a2))/(tan1(a2) - tan1(a1)));
		// double y = x - A;
		fx = (int)(x*tan1(a1));
		fx = (int)fx/2;
	}
	else if (alpha1 <= 0 && alpha2 <= 0)
	{
		a1 = PI*(90 - fabs(alpha1))/180;
		a2 = PI*(90 - fabs(alpha2))/180;
		double x = ((A*tan1(a2))/(tan1(a2) - tan1(a1)));
		// double y = x - A;
		fx = (int)(x*tan1(a1));
		fx = (int)-fx/2;
	}

	if (beta2 < 0)
	{
		fy = H - (int)(fx*tan1(PI*fabs(beta2)/180));
		fy = (int)fy/2;
	}
	else
	{
		fy = H + (int)(fx*tan1(PI*fabs(beta2)/180));
		fy = (int)fy/2;
	}
}

void moveNozzel(void)
{
	float nozzelX, nozzelY;
	if (fx == 0 || fy == 0)
	{
		return;
	}
	if (fy < Hn)
	{
		float h = sqrt1(pow1(fx - D, 2) + pow1(Hn - fy, 2));
    	nozzelY = 180*asin1((Hn -fy)/h)/PI;
    	nozzelY = 40 - nozzelY;
    }
    else if (fy > Hn)
    {
    	float h = sqrt1(pow1(fx - D, 2) + pow1(fy - Hn, 2));
    	nozzelY = 180*asin1((fy - Hn)/h)/PI;
    	nozzelY = 40 + nozzelY;
    }
    else
    {
    	nozzelY = 0;
    }

    if (a1 == a2)
    {
    	nozzelX = 0;
    }
    else if (a1 > a2)
    {
    	nozzelX = (90 - 180*a2/PI)/2;
    	nozzelX = (90 - 2*nozzelX);
    }
    else
    {
    	nozzelX = (90 - 180*a1/PI)/2;
    	nozzelX = (2*nozzelX + 90);
    }
    nozzelX = (int)(nozzelX - 4);
    EMBARC_PRINTF("nozzelX: %d\n", (int)nozzelX);
    EMBARC_PRINTF("nozzelY: %d\n", (int)nozzelY);

    int stepX = nozzelX/0.1125;
    int stepY = nozzelY/0.1125;
    EMBARC_PRINTF("stepX: %d\n", stepX);
    EMBARC_PRINTF("stepY: %d\n\n", stepY);
    if (stepY > currentStepY)
    {
    	stepMotor(1, 0, stepY - currentStepY, 1500);
    }
    else if (stepY < currentStepY)
    {
    	stepMotor(1, 1, currentStepY - stepY, 1500);
    }
    currentStepY = stepY;
    if (stepX > currentStepX)
    {
    	stepMotor(2, 0, stepX - currentStepX, 1500);
    }
    else if (stepX < currentStepX)
    {
    	stepMotor(2, 1, currentStepX - stepX, 1500);
    }
    currentStepX = stepX;
}

void home(void)
{
	stepMotor(2, 1, currentStepX, 1500);
	stepMotor(1, 1, currentStepY, 1500);
	currentStepX = currentStepY = 0;
}

void pumpOn(void)
{
	DEV_GPIO_PTR port_pump;
	port_pump = gpio_get_dev(DW_GPIO_PORT_C);
	port_pump->gpio_open(0x10000000);
	port_pump->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000000);

	port_pump->gpio_write(0x10000000, 0x10000000);
}
	
void pumpOff(void)
{
	DEV_GPIO_PTR port_pump;
	port_pump = gpio_get_dev(DW_GPIO_PORT_C);
	port_pump->gpio_open(0x10000000);
	port_pump->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000000);

	port_pump->gpio_write(0x00000000, 0x10000000);
}

void stepMotor(uint8_t number, uint8_t direction, uint32_t step, int stepTime)
{
	DEV_GPIO_PTR PMOD3_L;
	PMOD3_L = gpio_get_dev(DW_GPIO_PORT_A);
	PMOD3_L->gpio_open(0xf9ff0000);
	PMOD3_L->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0xf9ff0000);
	switch (number)
	{
		case 1:
			if (direction == 1)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0x30c30000, 0xf0cf0000);
					delay_us(stepTime);
					PMOD3_L->gpio_write(0x20c20000, 0xf0cf0000);
					delay_us(stepTime);
				}
			}
			else if (direction == 0)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0x10c10000, 0xf0cf0000);
					delay_us(stepTime);
					PMOD3_L->gpio_write(0x00c00000, 0xf0cf0000);
					delay_us(stepTime);
				}
			}
			break;
		case 2:
			if (direction == 1)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0xc0cc0000, 0xf0cf0000);
					delay_us(stepTime);
					PMOD3_L->gpio_write(0x80c80000, 0xf0cf0000);
					delay_us(stepTime);
				}
			}
			else if (direction == 0)
			{
				for (uint32_t i = 0; i < step; i++)
				{
					PMOD3_L->gpio_write(0x40c40000, 0xf0cf0000);
					delay_us(stepTime);
					PMOD3_L->gpio_write(0x00c00000, 0xf0cf0000);
					delay_us(stepTime);
				}
			}
			break;
	}
}


/** @} */
