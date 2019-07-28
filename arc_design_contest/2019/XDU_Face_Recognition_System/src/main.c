#include "embARC.h"
#include "embARC_debug.h"
#include <stdio.h>
#include "cnn.h"
#include "lcd.h"
#include "ov7670.h"
#include "yuyin.h"
#include "board.h"






//btn
static DEV_GPIO_PTR port_btn;     //AD5   gpio8b_3[2]
char imge[imge_len][imge_wid];
unsigned int a,b;
unsigned int c_data;
uint32_t  led_data_h,led_data_l;    //用于led显示
uint8_t ov_frame;
unsigned char ov_sta = 0;
uint16_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色
uint32_t m = 0;

static void delay_us(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=11;x>0;x--)
		for(y=z;y>0;y--);
}

static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=11000;x>0;x--)
		for(y=z;y>0;y--);
}

void compress()
{
	static int data_reg[32] = { 0 };
	char m = 0,c_data=0;
	char pixelR,pixelG,pixelB,pixel256;
	static char n = 0;
	if(n == 32)
		{
				n = 0;
	//			EMBARC_PRINTF("n is 32/n");
		}
	
			if ((b > 79) && (b < 240) && (a>39) && (a < 200))
			{
				c_data = (led_data_l <<8) |led_data_h;
				pixelR = (unsigned char)((c_data & 0xf800) >> 11);//5 bits
			  pixelG = (unsigned char)((c_data & 0x07e0) >> 5);//6 bits
			  pixelB = (unsigned char)(c_data & 0x001f);//5 bits
			  pixelR = (pixelR << 3) | (pixelR>>2);
			  pixelG = (pixelG << 2) | (pixelG>>4);
			  pixelB = (pixelB << 3) | (pixelB>>2);
			  pixel256 = (unsigned char)((pixelR * 299 + pixelG * 587 + pixelB * 114)/1000);
				data_reg[(b - 80) / 5] = data_reg[(b - 80) / 5] + pixel256;
				if ((b == 239) && ((a % 5) == 4) && n < 32)
				{
					//suzw
					for (char m = 0; m < 32; m++)
					{
						imge[n][m] = data_reg[m] / 25;
						data_reg[m] = 0;
					}
					n++;
				}
			}
		}
void pretreat(void)
{
	float gray=0;
	unsigned int L ,k;
	unsigned int counter_table[256] = { 0 };
	for(char x=0;x<32;x++)
		for(char y=0;y<32;y++)
		{
			k=imge[x][y];
			counter_table[k]= counter_table[k]+1;			
		}
		for(int k=1;k<256;k++)
		   counter_table[k]= counter_table[k-1] + counter_table[k];
	for(int k=0;k<256;k++)
	{
		gray=counter_table[k]*255/1024;
		if (gray > 254.5)
			L = 255;
		else
			L = (unsigned int)(gray+0.5);
		counter_table[k]=L;		
	}	
	for(char x=0;x<32;x++)
		for(char y=0;y<32;y++)
		{
			k=imge[x][y];
			L = counter_table[k];
			imge[x][y] = (unsigned char)(L);
		}   
}

int main(void)
{
    cpu_lock();
	board_init(); 
	cpu_unlock();
	
	OV7670_init();
	char m;
	uint32_t sw;

	port_btn = gpio_get_dev(DFSS_GPIO_8B3_ID);	//定义AD5端口
  port_btn -> gpio_open(0x4); //AD5[2]
  port_btn -> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x4);  //设置为输入
 	OV7670_Light_Mode(0);
	OV7670_Color_Saturation(2);
	OV7670_Brightness(1);
	OV7670_Contrast(2);
 	OV7670_Special_Effects(0);
	OV7670_Window_Set(12,176,240,320);	//设置窗口 
	Lcd_Init();			//初始化OLED
	LCD_Clear(WHITE); //清屏

	
	while(1)
	{
	 judge_vsync();	


		
		Address_set(0,0,LCD_W-1,LCD_H-1);
		for(a=0;a<240;a++)
	    {
		   for(b=0;b<320;b++)
		   {
//			 Address_set(a,b,a,b);
       Rclk_l();
///////////////////////////////////读取ov7670中FIFO的值并用其点亮流水灯////////////////////////
			//
       led_data_h = read_data();       	 			 
			 Rclk_h();
			 delay_us(1);
			 Rclk_l();		 
			 led_data_l = read_data();
			 compress();	
			 if(((b == 79 || b == 239) && a>=40 && a<200)||((a == 39 || a == 199)&& b >=80 && b < 240))
			 {
				 LCD_WR_DATA8(0x00);
				 LCD_WR_DATA8(0xf8);
			     
			 }
			 else
			 {
			 LCD_WR_DATA8(led_data_h);
			 LCD_WR_DATA8(led_data_l);
			 }
			 Rclk_h();
			 delay_us(1);
       
		   } 			 
		}
      ov_sta=0;
      port_btn ->gpio_read(&sw, 0x4);
      EMBARC_PRINTF("sw=%x\n",sw);
      pretreat();
      m=cnn();
      EMBARC_PRINTF("m is :%d\n",m);
		if(sw==0x4)
		{     
         
		  if(m==0)
		  {
			yuyin_szw();
		    delay_ms(500);  
		  }
          else if(m==1)
          {
            yuyin_llt();
		       delay_ms(500);
          }
          else if(m==2)
          {
            yuyin_lx();
		      delay_ms(500);
          }
          else
          {
            yuyin_wfsb();
		       delay_ms(500);
          }	  
		}

      
	  ov_frame++;		
     
	 EMBARC_PRINTF("ov_frame is :%x\n",ov_frame);
    }
  	return E_SYS;
}	
