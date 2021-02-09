#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "dev_iic.h"
#include "board.h"
#define I2C_SLAVE_ADDR  0x48
#define CHANNEL_0  0x40
#define CHANNEL_1  0x41
#define CHANNEL_2  0x42
#define CHANNEL_3  0x43
static DEV_GPIO_PTR port_ENA;
static DEV_GPIO_PTR port_PUL; 
static DEV_GPIO_PTR port_DIR;
static void delay_ms(volatile int z)
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}
const char ad_channels[] = { CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3 };
char write_buf[20];
char read_buf[20];
DEV_IIC *dev_iic0=NULL;
static void position_init(void)
{
    dev_iic0 = iic_get_dev(DW_IIC_0_ID	);
    dev_iic0->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_iic0->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR));
	port_ENA =  gpio_get_dev(DW_GPIO_PORT_C);  
	port_ENA -> gpio_open(0x40000);
	port_ENA -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x40000);
	port_PUL =	gpio_get_dev(DW_GPIO_PORT_C);  
	port_PUL -> gpio_open(0x10000); 
	port_PUL -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000);
	port_DIR =	gpio_get_dev(DW_GPIO_PORT_C);  
	port_DIR -> gpio_open(0x20000); 
	port_DIR -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x20000);
	port_ENA -> gpio_write(0x00000, 0x40000);
}
static void laser_on(void)/*激光开启*/
{
	port_ENA -> gpio_write(0x40000, 0x40000);
}
static void laser_off(void)/*激光关闭*/
{
	port_ENA -> gpio_write(0x00000, 0x40000);
}
static void motor(void)/*电机移动*/
{
	int num=10;
	while(num)
	{
		  port_PUL -> gpio_write(0x00000, 0x10000);
		  delay_ms(80);	
		  port_PUL -> gpio_write(0x10000, 0x10000);
		  num--;
		  delay_ms(80);		  
	}
}
static void dir_near(void)/*纠正电机方向为近电机移动*/
{
	port_DIR -> gpio_write(0x20000, 0x20000);
	delay_ms(100);
	port_DIR -> gpio_write(0x00000, 0x20000);
}
static void dir_away(void)/*纠正电机方向为远电机移动*/
{
	port_DIR -> gpio_write(0x00000, 0x20000);
	delay_ms(100);
	port_DIR -> gpio_write(0x20000, 0x20000);
}
int main(void)
{
    unsigned int i;
	unsigned int j;
	unsigned int res;
	char strbuf[256];
	float adc[4];
	float adc0;
	float adc1;
	float adc2;
	float save [1][1000]={};
	float ex [1][2000]={};
	float d;
	float e;
	float f;
	float g;
	float h;
	float k;
	float m;
	float o;
	float p;
	float q;	
	int	N=1;
	uint32_t rcv_cnt;
	cpu_lock();
	board_init();
	position_init();
while(1) 
{
	if(N%2==0)
		{
		laser_off();
		}
	else
		{
		laser_on();
		}
	printf("第N=%d次采样定位*************************************\r\n",N);
	motor();
	for (i = 0; i < 4; i++) 
		 {
          write_buf[0] = ad_channels[i];
		  res = dev_iic0->iic_write(write_buf, 1);	
	      rcv_cnt = dev_iic0->iic_read(read_buf, 2);	
	      if(rcv_cnt)
	       {
				res = read_buf[0];
			    res = ((res << 8) & 0xF0)| read_buf[1];
				adc[i] = res;
	        }
		  }
	      printf("Q1=%.3f,Q2=%.3f,Q3=%.3f,Q4=%.3f\r\n",adc[0]*3.3/255,
	      adc[1]*3.3/255, adc[2]*3.3/255, adc[3]*3.3/255);
	save [1][N] = adc[0]*3.3/255;
	ex [1][N] = adc[1]*3.3/255;	
	e=fabs((save [1][N])-(save [1][N-1]));/*第一象限Q1单差分的一次*/
	f=fabs((save [1][N-1])-(save [1][N-2]));/*第一象限Q1单差分的二次*/
	g=fabs((save [1][N-2])-(save [1][N-3]));/*第一象限Q1单差分的三次*/
	o=fabs((save [1][N-3])-(save [1][N-4]));/*第一象限Q1单差分的四次*/	
	h=fabs((ex [1][N])-(ex [1][N-1]));/*第二象限Q2单差分的一次*/
	k=fabs((ex [1][N-1])-(ex [1][N-2]));/*第二象限Q2单差分的二次*/
	m=fabs((ex [1][N-2])-(ex [1][N-3]));/*第二象限Q2单差分的三次*/
	p=fabs((ex [1][N-3])-(ex [1][N-4]));/*第二象限Q2单差分的四次*/
	printf("Q1一次差值=%.3f\r\n",e);
	printf("Q1二次差值=%.3f\r\n",f);	
	printf("Q1三次差值=%.3f\r\n",g);
	printf("Q1四次差值=%.3f\r\n",o);		
	printf("Q2一次差值=%.3f\r\n",h);
	printf("Q2二次差值=%.3f\r\n",k);	
	printf("Q2三次差值=%.3f\r\n",m);
	printf("Q2四次差值=%.3f\r\n",p);	
	q=0.013;/*根据外界环境预设定的双差分阈值，可变*/
	if(e<q)/*双差分算法的各自四次判定*/
	{
	   	if(f<q)
		{
			if(g<q)
			{
				if(h<q)
				{
					if(k<q)
					{
						if(m<q)
						{
							if(o<q)
							{
								if(p<q)
								{
								delay_ms(1000);
								port_ENA -> gpio_write(0x40000, 0x40000);/*激光常亮*/
								printf("激光定位成功\r\n");								
								break;
								}
							}
						}
					}
				}
			}
		}
	}
	if(fabs((adc[0]*3.3/255-adc0))>0.05)/*Q1象限差分*/
		{
		 dir_away();
		 printf("激光偏离，位于第一象限Q1\r\n");
		}
	if(fabs((adc[1]*3.3/255-adc1))>0.05)/*Q2象限差分*/
		{
		 dir_near();
		 printf("激光偏离，位于第二象限Q2\r\n");
		}
	N++;
	adc0=adc[0]*3.3/255;/*将第一象限输出电压值赋予变量，以作差分*/
	adc1=adc[1]*3.3/255;/*将第二象限输出电压值赋予变量，以作差分*/
}
return E_SYS;	
}
			
			
			
			
			
			
			
			
