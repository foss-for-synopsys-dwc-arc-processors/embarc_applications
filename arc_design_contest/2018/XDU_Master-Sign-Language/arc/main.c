#include "string.h"
#include "stdio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "u8g.h"
#define I2C_SLAVE_ADDR1  0x02
#define uchar unsigned char
#define uint unsigned int
#define HEADLEN       5  //数据包头的长度
DEV_IIC *dev_iicyu=NULL;
static DEV_GPIO_PTR port_swt1;//暂停键
static DEV_GPIO_PTR port_swt2;//中英切换
static DEV_GPIO_PTR port_swt3;//自动播放
static DEV_GPIO_PTR port_call;//一键求助
static DEV_GPIO_PTR port_PA4;
static DEV_GPIO_PTR port_PA5;
static DEV_GPIO_PTR port_PA6;
static DEV_GPIO_PTR port_PA7;
int adc[4];//弯曲传感器数据
double aValue[3];//手部运动加速度
double wValue[3];//手部运动角速度
double AValue[3];//手部运动角度
int cmd;
u8g_t u8g;
//********************延迟单元************************************
static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}
//*****************************************************************

//*****************************语音合成****************************
void  yuyin(char text[]) 
{    
    dev_iicyu = iic_get_dev(DW_IIC_1_ID	);
    dev_iicyu->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_iicyu->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
/****************需要发送的文本**********************************/ 
	uint8_t  length=0x00;  
    char  ecc  = 0x00;  			//定义校验字节
	uint8_t i=0; 
    while(text[length++]);
        char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
		head[2] = length + 3;
		 for(i = 0; i<5; i++)   				//依次发送构造好的5个帧头字节
	     {  
	         ecc=ecc^(head[i]); 		//对发送的字节进行异或校验	
		 }   
	     
             dev_iicyu->iic_write(head, 5);
	   	 
		 
		 for(i = 0; i<length+1; i++)   		//依次发送待合成的文本数据
	     {  
	          				//对发送的字节进行异或校验	
			 if(i==length) 
				 text[i] = ecc;
			 else 
				 ecc=ecc^(text[i]);
	      
	     }
    dev_iicyu->iic_write(text, length+1);
			
    delay_ms(200);			
}
//***********************************************************************

int main (void)
{
	uint32_t DIP1,DIP2,DIP3;
	uint32_t call_read,PA4,PA5,PA6,PA7;
	uint32_t Y[4];
	cpu_lock();	/* lock cpu to do initializations */
    board_init(); /* board level init */
    cpu_unlock();	/* unlock cpu to let interrupt work */
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, U8G_COM_SSD_I2C); /* create a new interface to a graphics display */
	u8g_Begin(&u8g); /* reset display and put it into default state */
	u8g_prepare();
	port_swt1 = gpio_get_dev(DW_GPIO_PORT_A);
	port_swt1->gpio_open(0x100);
	port_swt1->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x100);
	port_PA4 = gpio_get_dev(DW_GPIO_PORT_C);  
    port_PA4 -> gpio_open(0x10000);	
	port_PA4 -> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x10000);		
    port_PA5 = gpio_get_dev(DW_GPIO_PORT_C);  
    port_PA5 -> gpio_open(0x20000);	
	port_PA5 -> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x20000);	
    port_PA6 = gpio_get_dev(DW_GPIO_PORT_C);  
    port_PA6 -> gpio_open(0x40000);	
	port_PA6 -> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x40000);		
    port_PA7 = gpio_get_dev(DW_GPIO_PORT_C);  
    port_PA7 -> gpio_open(0x80000);	
	port_PA7 -> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x80000);		
		 
while(1)
 {
	  
	   port_swt1->gpio_read(&DIP1, 0x100);
	   printf("DIP1=0x%x\r\n",DIP1);
	   if(DIP1==0x0)
	   {
		   char textstop[]={"stop"};
		   u8g_FirstPage(&u8g); /* marks the beginning of the picture loop; it cannot be used inside the picture loop */
		           do 
			       {
			          draw(textstop);
		            } while (u8g_NextPage(&u8g)); 
	   }
        else  if(DIP1==0x100)
	   {   
            port_call = gpio_get_dev(DW_GPIO_PORT_A);  
	        port_call -> gpio_open(0x800);	
	        port_call -> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x800);		
	        port_call -> gpio_read(&call_read, 0x800);	
		if(call_read==0x800)
		  {	
			  delay_ms(400);
			  send_msg();
		   }	
//**************************语音识别********************************************		
	   port_PA4 -> gpio_read(&PA4, 0x10000);	
	   port_PA5 -> gpio_read(&PA5, 0x20000);	
	   port_PA6 -> gpio_read(&PA6, 0x40000);	
	   port_PA7 -> gpio_read(&PA7, 0x80000);	
	   Y[0]=PA7;
	   Y[1]=PA6;
	   Y[2]=PA5;
	   Y[3]=PA4;
	   printf("PA7=0x%x\r\n",PA7);
	   printf("PA6=0x%x\r\n",PA6);
	   printf("PA5=0x%x\r\n",PA5);
	   printf("PA4=0x%x\r\n",PA4);
	   printf("Y[0]=0x%x\r\n",Y[0]);
	   printf("Y[1]=0x%x\r\n",Y[1]);
	   printf("Y[2]=0x%x\r\n",Y[2]);
	   printf("Y[3]=0x%x\r\n",Y[3]);
     if(Y[0]==0x0&Y[1]==0x0&Y[2]==0x0&Y[3]==0x0)	
         {printf("你好\r\n"); 
	      u8g_FirstPage(&u8g);
		  do 
			  {
			        drawBhello();
		      } while (u8g_NextPage(&u8g)); 
		  delay_ms(1);
		 }
	     
	    
	 if(Y[0]==0x80000&Y[1]==0x0&Y[2]==0x0&Y[3]==0x0)		
	     {printf("你叫什么\r\n");	
	      u8g_FirstPage(&u8g); 
		  do 
			  {
			        drawByourname();
		      } while (u8g_NextPage(&u8g));
		  delay_ms(1);
		 }	
		 
	     
	 if(Y[0]==0x0&Y[1]==0x40000&Y[2]==0x0&Y[3]==0x0)	
	     {printf("我叫李四\r\n");	
	      u8g_FirstPage(&u8g); 
		  do 
			  {
			        drawBlisi();
		      } while (u8g_NextPage(&u8g));
		   delay_ms(1);
		  }	
		
	     
	 if(Y[0]==0x80000&Y[1]==0x40000&Y[2]==0x0&Y[3]==0x0)		
	    {printf("我22\r\n");
	     u8g_FirstPage(&u8g); 
		  do 
			  {
			        drawBmyage();
		      } while (u8g_NextPage(&u8g));	
			  delay_ms(1);
		}	
		 
	 if(Y[0]==0x0&Y[1]==0x0&Y[2]==0x20000&Y[3]==0x0)	
	    { printf("你呢\r\n");
	      u8g_FirstPage(&u8g); 
		  do 
			  {
			       drawBnine()	;
		      } while (u8g_NextPage(&u8g));	
		 delay_ms(1);
		 }
		
	     
	 if(Y[0]==0x80000&Y[1]==0x0&Y[2]==0x20000&Y[3]==0x0)		
	    {printf("我也是\r\n");	
	     u8g_FirstPage(&u8g); 
		  do 
			  {
			         drawBmetoo();
		      } while (u8g_NextPage(&u8g));
		 delay_ms(1);
			  }
			
	  
	 if(Y[0]==0x0&Y[1]==0x40000&Y[2]==0x20000&Y[3]==0x0)		
        {printf("我要去台湾\r\n");
	    u8g_FirstPage(&u8g); 
		  do 
			  {
			         drawBgotw() ;
		      } while (u8g_NextPage(&u8g));
		delay_ms(1);
		}
		 
	    
	  if(Y[0]==0x80000&Y[1]==0x40000&Y[2]==0x20000&Y[3]==0x0)	
         {printf("我进决赛了\r\n");
	      u8g_FirstPage(&u8g); 
		  do 
			  {
			        drawBruwei();
		      } while (u8g_NextPage(&u8g));
		 delay_ms(1);
		 }
		 
	    
	 if(Y[0]==0x0&Y[1]==0x0&Y[2]==0x0&Y[3]==0x10000)		
        {printf("你知道台湾有什么名胜\r\n");
	     u8g_FirstPage(&u8g);
		  do 
			  {
			         drawBchat1();
		      } while (u8g_NextPage(&u8g));	
		delay_ms(1);
			  } 
		
	    
	  if(Y[0]==0x80000&Y[1]==0x0&Y[2]==0x0&Y[3]==0x10000)	
	     {printf("好吧\r\n");	
	      u8g_FirstPage(&u8g); 
		  do 
			  {
			         drawBhaoba();
		      } while (u8g_NextPage(&u8g));
			  delay_ms(1);
		}
		
	   
	  if(Y[0]==0x0&Y[1]==0x40000&Y[2]==0x0&Y[3]==0x10000)	
       {printf("谢谢\r\n");		
        u8g_FirstPage(&u8g); 
		  do 
			  {
			         drawBthank();
		      } while (u8g_NextPage(&u8g));
	     delay_ms(1);
	 } 
//************************************************************************

	   port_swt3 = gpio_get_dev(DW_GPIO_PORT_A);
	   port_swt3->gpio_open(0x400);
	   port_swt3->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x400);
	   port_swt3->gpio_read(&DIP3, 0x400);
	   printf("DIP3=0x%x\r\n",DIP3);
//**************************自动播报*****************************************	 
	  if(DIP3==0x400)
		{
			EMBARC_PRINTF("自动播放\r\n");
			data(adc,aValue,wValue,AValue);
		    cmd=judge(aValue,wValue,AValue,adc,cmd);
			if(cmd==1)
		        {
		          char texta1[]={"大家好，我是张三，我来自西安，现在我在西电读研究生"};
                   yuyin(texta1);
		            cmd=0;
		        }
		    else if(cmd==2)
		        {
			      char textb1[]={"您好，我迷路了，您可以帮我一下吗"};
			      yuyin(textb1);
			        cmd=0;
		        }
		    else if(cmd==3)
		        {
		            char textc1[]={"很高兴认识你"};
			        yuyin(textc1);
			        cmd=0;
			    }
		    else if(cmd==4)
		        {
			        char textd1[]={"我们一会儿去哪儿"};
			        yuyin(textd1);
			        cmd=0;
			    }
			else if(cmd==5)
		        {
			        char textE1[]={"今天天气真好啊"};
			        yuyin(textE1);
			         cmd=0;
		         }
			
		}
//*****************************************************************************

//**********************************中英文手势识别*****************************
	  else
		   {
		     port_swt2 = gpio_get_dev(DW_GPIO_PORT_A);
	         port_swt2->gpio_open(0x200);
	         port_swt2->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x200);
	         port_swt2->gpio_read(&DIP2, 0x200);
			 printf("DIP2=0x%x\r\n",DIP2);
	 if(DIP2==0x200)
		{
		       data(adc,aValue,wValue,AValue);
		       cmd=judge(aValue,wValue,AValue,adc,cmd);
			   if(cmd==1)
		        {
		          char texta[]={"A"};
                   yuyin(texta);
			       u8g_FirstPage(&u8g); 
		           do 
			       {
			          draw(texta);
		            } while (u8g_NextPage(&u8g)); 
		            cmd=0;
		        }
		     else if(cmd==2)
		        {
			       char textb[]={"B"};
			       yuyin(textb);
			       u8g_FirstPage(&u8g); 
		           do 
			       {
			         draw(textb);
		            } while (u8g_NextPage(&u8g));  
			        cmd=0;
		        }
		     else if(cmd==3)
		        {
		            char textc[]={"C"};
			        yuyin(textc);
			        u8g_FirstPage(&u8g); 
		            do 
			         {
			           draw(textc);
		              } while (u8g_NextPage(&u8g));  
			        cmd=0;
			    }
		     else if(cmd==4)
		        {
			        char textd[]={"D"};
			        yuyin(textd);
			        u8g_FirstPage(&u8g); 
		            do
			         {
			            draw(textd);
		              } while (u8g_NextPage(&u8g)); 
			        cmd=0;
			    }
			 else if(cmd==5)
		        {
			        char textE[]={"E"};
			        yuyin(textE);
			        u8g_FirstPage(&u8g); 
		            do 
			        {
			          draw(textE);
		            } while (u8g_NextPage(&u8g));  
			         cmd=0;
		         }
		     else if(cmd==6)
		         {
		            char textF[]={"F"};
			        yuyin(textF);
			        u8g_FirstPage(&u8g); 
		            do 
			           {
			              draw(textF);
		                } while (u8g_NextPage(&u8g));  
			            cmd=0;
			      }
		     else if(cmd==7)
		          {
			            char textG[]={"G"};
			            yuyin(textG);
			            u8g_FirstPage(&u8g); 
		                do
			            {
			               draw(textG);
		                } while (u8g_NextPage(&u8g)); 
			             cmd=0;
			       }
			 else if(cmd==8)
		           {
			           char textH[]={"H"};
			           yuyin(textH);
			           u8g_FirstPage(&u8g); 
		               do 
			          {
			             draw(textH);
		               } while (u8g_NextPage(&u8g));  
			          cmd=0;
		            }
		     else if(cmd==9)
		            {
		               char textI[]={"I"};
			            yuyin(textI);
			            u8g_FirstPage(&u8g);
		                do 
			            {
			               draw(textI);
		                 } while (u8g_NextPage(&u8g));  
			              cmd=0;
			        }
		     else if(cmd==10)
		           {
			            char textJ[]={"J"};
			             yuyin(textJ);
			             u8g_FirstPage(&u8g); 
		                 do
			             {
			                draw(textJ);
		                  } while (u8g_NextPage(&u8g)); 
			              cmd=0;
			        }
			 else if(cmd==11)
		            {
			            char textK[]={"K"};
			             yuyin(textK);
			             u8g_FirstPage(&u8g); 
		                 do 
			              {
			                 draw(textK);
		                   } while (u8g_NextPage(&u8g));  
			              cmd=0;
		            }
		     else if(cmd==12)
	             	{
	                 	char textL[]={"L"};
			            yuyin(textL);
			            u8g_FirstPage(&u8g); 
		                do 
			            {
			                 draw(textL);
		                 } while (u8g_NextPage(&u8g));  
			             cmd=0;
			        }
		     else if(cmd==13)
		            {
			           char textM[]={"M"};
			           yuyin(textM);
			           u8g_FirstPage(&u8g); 
		                do
			           {
			             draw(textM);
		                } while (u8g_NextPage(&u8g)); 
			           cmd=0;
			        }
			 else if(cmd==14)
		            {
			           char textN[]={"N"};
			           yuyin(textN);
			          u8g_FirstPage(&u8g); 
		              do 
			           {
			             draw(textN);
		               } while (u8g_NextPage(&u8g));  
			           cmd=0;
		            }
		     else if(cmd==15)
		            {
		              char textO[]={"O"};
		   	          yuyin(textO);
			          u8g_FirstPage(&u8g); 
		               do 
			          {
			               draw(textO);
		              } while (u8g_NextPage(&u8g));  
			           cmd=0;
			        }
		     else if(cmd==16)
		           {
			           char textP[]={"P"};
			           yuyin(textP);
			           u8g_FirstPage(&u8g); 
		                do
			             {
			                draw(textP);
		                } while (u8g_NextPage(&u8g)); 
			           cmd=0;
			        }
			 else if(cmd==17)
		            {
			            char textQ[]={"Q"};
			             yuyin(textQ);
			             u8g_FirstPage(&u8g); 
		                 do 
			             {
			                draw(textQ);
		                  } while (u8g_NextPage(&u8g));  
			             cmd=0;
		            }
		     else if(cmd==18)
		             {
		                 char textR[]={"R"};
			              yuyin(textR);
		                	u8g_FirstPage(&u8g); 
		                  do 
		                  	{
			                  draw(textR);
		                    } while (u8g_NextPage(&u8g));  
			               cmd=0;
			         }
	    }
	 else
	         {
		       data(adc,aValue,wValue,AValue);
		       cmd=judge1(aValue,wValue,AValue,adc,cmd);
			  if(cmd==1)
		        {
		          char text1[]={"多大"};
                   yuyin(text1);
			       u8g_FirstPage(&u8g); 
		           do 
			       {
			          drawhowold();
		            } while (u8g_NextPage(&u8g)); 
		            cmd=0;
		        }
		       else if(cmd==2)
		        {
			      char text2[]={"可以"};
			      yuyin(text2);
			      u8g_FirstPage(&u8g);
		           do 
			       {
			         drawkeyi();
		            } while (u8g_NextPage(&u8g));  
			        cmd=0;
		        }
		       else if(cmd==3)
		        {
		            char text3[]={"有"};
			        yuyin(text3);
			        drawyou();
		            do 
			         {
			           draw(text3);
		              } while (u8g_NextPage(&u8g));  
			        cmd=0;
			    }
		       else if(cmd==4)
		        {
			        char text4[]={"聪明"};
			        yuyin(text4);
			        u8g_FirstPage(&u8g); 
		            do
			         {
			            drawclever();
		              } while (u8g_NextPage(&u8g)); 
			        cmd=0;
			    }
			   else if(cmd==5)
		        {
			        char text5[]={"张"};
			        yuyin(text5);
			        u8g_FirstPage(&u8g); 
		            do 
			        {
			          drawzhang();
		            } while (u8g_NextPage(&u8g));  
			         cmd=0;
		         }
		       else if(cmd==6)
		         {
		              char text6[]={"使用"};
			           yuyin(text6);
			           u8g_FirstPage(&u8g); 
		               do 
			           {
			              drawuse();
		                } while (u8g_NextPage(&u8g));  
			            cmd=0;
			      }
		       else if(cmd==7)
		          {
			            char text7[]={"去"};
			            yuyin(text7);
			            u8g_FirstPage(&u8g); 
		                do
			            {
			               drawgo();
		                } while (u8g_NextPage(&u8g)); 
			             cmd=0;
			       }
			   else if(cmd==8)
		           {
			           char text8[]={"23"};
			           yuyin(text8);
			           u8g_FirstPage(&u8g);
		               do 
			          {
			             draw(text8);
		               } while (u8g_NextPage(&u8g));  
			          cmd=0;
		            }
		       else if(cmd==9)
		            {
		               char text9[]={"很"};
			            yuyin(text9);
			            u8g_FirstPage(&u8g); 
		                do 
			            {
			               drawhen();
		                 } while (u8g_NextPage(&u8g));  
			              cmd=0;
			        }
		       else if(cmd==10)
		           {
			            char text10[]={"漂亮"};
			             yuyin(text10);
			             u8g_FirstPage(&u8g);
		                 do
			             {
			                drawpiaoliang();
		                  } while (u8g_NextPage(&u8g)); 
			              cmd=0;
			        }
			   else if(cmd==11)
		            {
			            char text11[]={"你"};
			             yuyin(text11);
			             u8g_FirstPage(&u8g); 
		                 do 
			              {
			                 drawni();
		                   } while (u8g_NextPage(&u8g));  
			              cmd=0;
		            }
		       else if(cmd==12)
	             	{
	                 	char text12[]={"好"};
			            yuyin(text12);
			            u8g_FirstPage(&u8g); 
		                do 
			            {
			                 drawhao();
		                 } while (u8g_NextPage(&u8g));  
			             cmd=0;
			        }
		       else if(cmd==13)
		            {
			           char text13[]={"祝"};
			           yuyin(text13);
			           u8g_FirstPage(&u8g); 
		                do
			           {
			             drawzhu();
		                } while (u8g_NextPage(&u8g)); 
			           cmd=0;
			        }
			   else if(cmd==14)
		            {
			           char text14[]={"旅途"};
			           yuyin(text14);
			          u8g_FirstPage(&u8g); 
		              do 
			           {
			             drawlvtu();
		               } while (u8g_NextPage(&u8g));  
			           cmd=0;
		            }
		       else if(cmd==15)
		            {
		              char text15[]={"认识"};
		   	          yuyin(text15);
			          u8g_FirstPage(&u8g); 
		               do 
			          {
			               drawrenshi();
		              } while (u8g_NextPage(&u8g));  
			           cmd=0;
			        }
		       else if(cmd==16)
		           {
			           char text16[]={"三"};
			           yuyin(text16);
			           u8g_FirstPage(&u8g); 
		                do
			             {
			                drawsan();
		                } while (u8g_NextPage(&u8g)); 
			           cmd=0;
			        }
			   else if(cmd==17)
		            {
			            char text17[]={"高兴"};
			             yuyin(text17);
			             u8g_FirstPage(&u8g); 
		                 do 
			             {
			                drawhappy();
		                  } while (u8g_NextPage(&u8g));  
			             cmd=0;
		            }
		       else if(cmd==18)
		             {
		                 char text18[]={"愉快"};
			              yuyin(text18);
		                	u8g_FirstPage(&u8g); 
		                  do 
		                  	{
			                  drawyukuai();
		                    } while (u8g_NextPage(&u8g));  
			               cmd=0;
			         }
		       else if(cmd==19)
		        {
			      char text19[]={"不"};
			      yuyin(text19);
			      u8g_FirstPage(&u8g); 
		           do 
			       {
			         drawbu();
		            } while (u8g_NextPage(&u8g));  
			        cmd=0;
		        }
		       else if(cmd==20)
		        {
		            char text20[]={"你好"};
			        yuyin(text20);
			        u8g_FirstPage(&u8g); 
		            do 
			         {
			           drawhello();
		              } while (u8g_NextPage(&u8g));  
			        cmd=0;
			    }
		       else if(cmd==21)
		        {
			        char text21[]={"什么"};
			        yuyin(text21);
			        u8g_FirstPage(&u8g);
		            do
			         {
			            drawwhat();
		              } while (u8g_NextPage(&u8g)); 
			        cmd=0;
			    }
			   else if(cmd==22)
		        {
			        char text22[]={"名胜"};
			        yuyin(text22);
			        u8g_FirstPage(&u8g);
		            do 
			        {
			          drawscene();
		            } while (u8g_NextPage(&u8g));  
			         cmd=0;
		         }
		       else if(cmd==23)
		         {
		              char text23[]={"谢谢"};
			           yuyin(text23);
			           u8g_FirstPage(&u8g); 
		               do 
			           {
			              drawthank();
		                } while (u8g_NextPage(&u8g));  
			            cmd=0;
			      }
		       else if(cmd==24)
		          {
			            char text24[]={"大家"};
			            yuyin(text24);
			            u8g_FirstPage(&u8g); 
		                do
			            {
			               drawdajia();
		                } while (u8g_NextPage(&u8g)); 
			             cmd=0;
			       }
			   else if(cmd==25)
		           {
			           char text25[]={"是"};
			           yuyin(text25);
			           u8g_FirstPage(&u8g);
		               do 
			          {
			             drawshi();
		               } while (u8g_NextPage(&u8g));  
			          cmd=0;
		            }
		       else if(cmd==26)
		            {
		               char text26[]={"为什么"};
			            yuyin(text26);
			            u8g_FirstPage(&u8g); 
		                do 
			            {
			               drawwhy();
		                 } while (u8g_NextPage(&u8g));  
			              cmd=0;
			        }
		       else if(cmd==27)
		           {
			            char text27[]={"打电话"};
			             yuyin(text27);
			             u8g_FirstPage(&u8g); 
		                 do
			             {
			                drawcall();
		                  } while (u8g_NextPage(&u8g)); 
			              cmd=0;
			        }
			   else if(cmd==28)
		            {
			            char text28[]={"知道"};
			             yuyin(text28);
			             u8g_FirstPage(&u8g); 
		                 do 
			              {
			                 drawzhidao();
		                   } while (u8g_NextPage(&u8g));  
			              cmd=0;
		            }
		       else if(cmd==29)
	             	{
	                 	char text29[]={"对不起"};
			            yuyin(text29);
			            u8g_FirstPage(&u8g); 
		                do 
			            {
			                 drawsorry();
		                 } while (u8g_NextPage(&u8g));  
			             cmd=0;
			        }
		       else if(cmd==30)
		            {
			           char text30[]={"我"};
			           yuyin(text30);
			           u8g_FirstPage(&u8g); 
		                do
			           {
			             drawwo();
		                } while (u8g_NextPage(&u8g)); 
			           cmd=0;
			        }
			   else if(cmd==31)
		            { 
			           char text30[]={"叫"};
			           yuyin(text30);
			           u8g_FirstPage(&u8g); 
		                do
			           {
			             drawjiao();
		                } while (u8g_NextPage(&u8g)); 
			           cmd=0;
		            }
		          
	          }
            }
    }
                      
 }
	return E_SYS;
	
	
}