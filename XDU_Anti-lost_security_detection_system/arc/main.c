/* ------------------------------------------
 * \version 2016-01
 * \date 2019-05-20
 * \author Renjie Cao
--------------------------------------------- */
#include "string.h"
#include "stdio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "board.h"
#include "dev_iic.h"
#include "dev_gpio.h"
#define I2C_SLAVE_ADDR1  0x01
#define uchar unsigned char
#define uint unsigned int
#define HEADLEN       5  //数据包头的长度
#define TH0   25  //举起安全距离阈值
#define TH1   1.25  //和加速度的大小
#define TH2   1.18  //加速度均值的大小
#define TH30    55  //正向倾角的阈值
#define TH31    -55  //反向倾角的阈值

static DEV_GPIO_PTR port_stop;//暂停键
static DEV_GPIO_PTR port_voice;//中英切换
double aValue[3];//手部运动加速度
double wValue[3];//手部运动角速度
double AValue[3];//手部运动角度
double AWM,Sawm,Z1,A1,A2;
int    BLESTATE;

DEV_IIC *dev_iicyu=NULL;

uint8_t *phoneNum = "00310035003000300032003900380036003400360036"; //监护人手机号码
uint8_t *msg1 = "7528623753EF80FD88AB62B18D77 "; //用户可能被抱起
uint8_t *msg2 = "752862378DCC5012"; //用户跌倒


//********************延迟单元************************************
static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}
//*****************************************************************

//*********************语音合成*************************************
void  yuyin(char text[]) 
{    
    dev_iicyu = iic_get_dev(DW_IIC_0_ID	);
    dev_iicyu->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_iicyu->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
/****************需要发送的文本**********************************/ 
	uint8_t  length=0x00;  
    char  ecc  = 0x00;  			//定义校验字节
	uint8_t i=0; 
    //length =sizeof(text); 			//需要发送文本的长度
	while(text[length++]);
/*****************发送过程**************************************/           
		// headOfFrame[0] = 0xFD ; 			//构造帧头FD
		// headOfFrame[1] = 0x00 ; 			//构造数据区长度的高字节
		// headOfFrame[2] = length + 3; 		//构造数据区长度的低字节
		// headOfFrame[3] = 0x01 ; 			//构造命令字：合成播放命令
/*****************带背景音乐**************************************/  		 
		// headOfFrame[4] = 0x00 ; 			//构造命令参数：编码格式为GBK
       char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
/*****************不带背景音乐**************************************/  		 
		//headOfFrame[4] = 0x01 ; 			//构造命令参数：编码格式为GBK
      
		
		
		head[2] = length + 3;
		
		 for(i = 0; i<5; i++)   				//依次发送构造好的5个帧头字节
	     {  
	         ecc=ecc^(head[i]); 		//对发送的字节进行异或校验	
	         //SBUF = headOfFrame[i]; 
			// printf("head[%d]=%d\r\n",i,head[i]);
			 
		 }   
		    // printf("length=%d\r\n",length);
	     
             dev_iicyu->iic_write(head, 5);
	   	 
		 
		 for(i = 0; i<length+1; i++)   		//依次发送待合成的文本数据
	     {  
	          				//对发送的字节进行异或校验	
	         //SBUF = text[i]; 
			 if(i==length) 
				 text[i] = ecc;
			 else 
				 ecc=ecc^(text[i]);
			//printf("text[%d]=%d\r\n",i,text[i]);
	      
	     }
		   //printf("ecc=%d\r\n",ecc);
            dev_iicyu->iic_write(text, length+1);
			
           delay_ms(200);
            	   
}
//***********************************************************************

int main(void)
{
	uint32_t DIP1,DIP2;
	double   Max,Sum,Aver;
	int i,j,k,m;
	
	cpu_lock();	/* lock cpu to do initializations */
    board_init(); /* board level init */
    cpu_unlock();	/* unlock cpu to let interrupt work */
    
	
	
	while(1)
    {

//*****************************************************************************

//**********************************待机模式************************************
	  
	   port_stop = gpio_get_dev(DW_GPIO_PORT_A);
	   port_stop->gpio_open(0x100);
	   port_stop->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x100);
	   port_stop->gpio_read(&DIP1, 0x100);
	   printf("DIP1=0x%x\r\n",DIP1);

	//   port_stop->gpio_write(0x100, 0x100);
	//   port_stop->gpio_read(&DIP1, 0x100);
	//   printf("DIP1=0x%x\r\n",DIP1);
//	   
		
	   if(DIP1==0x0)
	   {
		  printf("你已经进入待机阶段\r\n");
	   }
        else  if(DIP1==0x100)
	   {   
//*****************************************************************************
		    printf("你已经进入采样阶段\r\n");
			data0(aValue,wValue,AValue,&Z1,&AWM,&A1,&A2);
		    printf("AWM:%4.3f,",AWM);
			

//**********************************一键求助************************************
            
            port_voice = gpio_get_dev(DW_GPIO_PORT_C);  
	        port_voice -> gpio_open(0x10000);	
	        port_voice -> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x10000);		
	        port_voice -> gpio_read(&DIP2,0x10000);
			printf("DIP2=0x%x\r\n",DIP2);
         
			delay_ms(100);
			BLESTATE=ble();
			printf("ble state is %d\r\n",BLESTATE);
		
		if(DIP2==0x0)
		  {	
			   EMBARC_PRINTF("自动播放\r\n");

			   char texta[]={"你好,我是张三"};
		   	   yuyin(texta);
			   delay_ms(6000);
			   char textb[]={"我家人在附近"};
		   	   yuyin(textb);
			   delay_ms(5000);
			   char textc[]={"联系电话15236578965"};
		   	   yuyin(textc);
			   delay_ms(9000);

		   }
//*****************************************************************************
        
//**********************************脱落检查************************************
		else if(AWM == 0 | BLESTATE  ==0 )
		   {
			printf("传感器脱落警告\r\n");
			beep();
			//send(phoneNum,msg);
	        delay_ms(500); 
		   }
//*****************************************************************************

//**********************************姿态检测************************************
			
		else
		{
			data0(aValue,wValue,AValue,&Z1,&AWM,&A1,&A2);
			printf("AWM:%4.3f,\r\n",AWM);
			m=0;
			j=0;
		    Max = Z1;
			k=4;
			if(AWM > 0.990 & AWM < 1.050){
				printf("可能处于静止\r\n");
			}
			if(AWM > TH1 | Z1>=1.25){
				if(Z1>=3.8)  continue;
				else if(Z1>=2.5 & Z1 <3.8) {
					printf("直接被抱起\r\n");
					send(phoneNum,msg1);
					delay_ms(5000); 
					continue;
				}
				else {
					Sum=0;
					printf("进入判断环节\r\n");
					Max = (Max > Z1)? Max : Z1;
					for(i = 0; i< 18; i++) {
						data0(aValue,wValue,AValue,&Z1,&AWM,&A1,&A2);
						delay_ms(150);  
						Max = (Max > Z1)? Max : Z1;
						Sum += AWM;   
					//三次判断是否恢复稳定
						if(0.93<=Z1 & Z1<=1.08 & (k-1)>0)  
						{
							j=i;
							k=k-1;
						}
						
						printf("Sum:%4.3f,\r\n",Sum);
					}			
					Aver = Sum/18;
					printf("Aver:%4.3f,\r\n",Aver);//求出平均加速度
					
					m = (1 < j< 11)? (j-1) : 18; //得到恢复稳定需要的时间
					printf("m:%d,\r\n",m);
					
					if(Aver < TH2 ){
						printf("进入第三阶段\r\n");					
						printf("A1:%4.3f,\r\n",A1);
						printf("A2:%4.3f,\r\n",A2);
						
						if(A1>=TH30 | A1<=TH31 | A2>=TH30 | A2<=TH31 ) {
							printf("跌倒\r\n");
							send(phoneNum,msg2);
							delay_ms(3000); 
							continue;
						}
						 else if(m > 1 & m < 12 & ((Max-1)*m*m >=TH0)) //判断是否超出举起安全距离
						{
						  printf("被抱起\r\n");
						  printf("m:%d,\r\n",m);
						  send(phoneNum,msg1);
						  delay_ms(7000); 
						  continue;
						}
						else
						{
							printf("返回第一阶段\r\n");
						}
				}
			}
			}
			
		}	
        printf("结束运行\r\n");
//*****************************************************************************

//**********************************定位功能************************************
		if(BLESTATE == 1 & !AWM ==0){
		printf("超出运行范围\r\n");	
		send_msg();
		delay_ms(3000); 
	    }
		
	}
	
}
	return E_SYS;
	
	}