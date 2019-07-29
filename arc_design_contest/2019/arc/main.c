#include "arc.h"
#include "arc_builtin.h"
#include "string.h"
#include "stdio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"

/* middleware level*/
/* application level */

#include "board.h"
#include "dev_uart.h"



#define uchar unsigned char
#define uint unsigned int
#define HEADLEN       5  //数据包头的长庿

static volatile int t0 = 0;

static DEV_GPIO_PTR port_example;
static DEV_GPIO_PTR port_huoyan;
static DEV_GPIO_PTR port_yanwu;
static DEV_GPIO_PTR port_hongwai;
static DEV_GPIO_PTR port_jidianqi;
DEV_UART *dev_uart0 = NULL;
DEV_UART *dev_uart2 = NULL;
uint8_t *phoneNum = "00310035003600380036003000330038003500350031";
uint8_t *msg ="75914F3C7740706B8BF7901F639267E5";
uint32_t rd_avail = 0;
int cmd;
int cmd1;

static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}

void  yuyin(char text[]) 
{    
	//char  text[]={"欢迎光临"};
    uint8_t  length;  
    char  ecc  = 0x00;  			//定义校验字节
	uint8_t i=0; 
    length =sizeof(text); 			//需要发送文本的长度

	uint32_t baudrate = 115200;
    dev_uart2 = uart_get_dev(DW_UART_2_ID);
	dev_uart2->uart_open(baudrate);
 
	while(text[length++]);
	{
        char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
		head[2] = length + 2;
		 for(i = 0; i<5; i++)   				//依次发送构造好皿个帧头字芿
	     {  
	         ecc=ecc^(head[i]); 		//对发送的字节进行异或校验	
			 printf("head[%d]=%d\r\n",i,head[i]);
			 
		 }   
		     printf("length=%d\r\n",length);
	     
             dev_uart2->uart_write(head, 5);
	   	 for(i = 0; i<length; i++)   		//依次发送待合成的文本数捿
	     {  
	          				//对发送的字节进行异或校验	
	         //SBUF = text[i]; 
			 if(i==length-1) 
				 text[i] = ecc;
			 else 
				 ecc=ecc^(text[i]);
			 printf("text[%d]=%d\r\n",i,text[i]);
	      
	     }
		    printf("ecc=%d\r\n",ecc);
            dev_uart2->uart_write(text, length);
			//dev_uart2->uart_write(ecc, 1);
            delay_ms(300);			
	}	   


}

void send(uint8_t *phoneNum, uint8_t *msg)
{	
	DEV_UART *dev_uart0 = NULL;
	uint32_t baudrate = 115200;	
	dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0-> uart_open(baudrate);
	
	uint8_t i;
	uint8_t pnum[64];
	int8_t *cmd[5]={"AT+CMGF=1\r\n", "AT+CSCS=\"UCS2\"\r\n",
				  "AT+CSCA?\r\n",  "AT+CSMP=17,167,0,25\r\n",};
	snprintf(pnum, 64, "AT+CMGS=\"%s\"\r\n", phoneNum);
	for(i = 0; i < 4; i++)
	{
		dev_uart0->uart_write(cmd[i], strlen(cmd[i]));
		delay_ms(100);
	}	
	dev_uart0->uart_write(pnum, strlen(pnum));
	delay_ms(100);
	dev_uart0->uart_write(msg, strlen(msg));
	delay_ms(100);
	uint8_t end[]={0x1a,0x00};
	dev_uart0->uart_write(end, strlen(end));
}

int  main(void) 
{        
   
	uint32_t data;
	uint32_t hdata;
	uint32_t ywdata;
    uint32_t hwdata;
    uint32_t jiddata;	
	cpu_lock();	/* lock cpu to do initializations */
    board_init(); /* board level init */
    cpu_unlock();	/* unlock cpu to let interrupt work */
	/////////////////////////////////////////////////////////////////////////打开C口 蜂鸣器
	port_example = gpio_get_dev(DW_GPIO_PORT_C);//定义port_c接口
	port_example->gpio_open(0x10000);//open C[16]
	port_example->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000);//设置portC[16]为输出
   
    	
	/////////////////////////////////////////////////////////////////////////打开C口 火焰输入1 2
	port_huoyan = gpio_get_dev(DW_GPIO_PORT_C);//定义GPIO C[17]控制火焰端口的输入
	port_huoyan->gpio_open(0x20000);//open C[17]
	port_huoyan->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x20000);//设置C[17]输入
	
	
	/////////////////////////////////////////////////////////////////////////打开C口 烟雾输入
	port_yanwu = gpio_get_dev(DW_GPIO_PORT_C);//定义GPIO C[18]控制烟雾端口的输入
	port_yanwu->gpio_open(0x40000);//open C[18]
	port_yanwu->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x40000);//设置C[17]输入
	
/////////////////////////////////////////////////////////////////////////打开C口 控制红外感应模块的输入	
	port_hongwai = gpio_get_dev(DW_GPIO_PORT_C);//定义GPIO C[19]输入
	port_hongwai->gpio_open(0x80000);//open C[19]  0x80000
	port_hongwai->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x80000);
	
	/////////////////////////////////////////////////////////////////////////打开A口 控制继电器的输出	
	port_jidianqi = gpio_get_dev(DW_GPIO_PORT_A);//定义GPIO A[19]输入
	port_jidianqi->gpio_open(0x80000);//open A[19] 0x80000 
	port_jidianqi->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x80000);
	
	//initialize the uart0 which is on PMOD1 J3 for TXD J4 for RXD

	 	while(1)
		{
			    port_huoyan->gpio_read(&hdata, 0x20000);   //读取火焰的端口1
				
				port_yanwu->gpio_read(&ywdata, 0x40000);   //读取烟雾的端口1
				
				port_hongwai->gpio_read(&hwdata, 0x80000);   //读取红外的端口 人不在
	//**************************检测火灾*****************************************			
				if(hdata==0||ywdata==0)                    //检测到火焰与烟雾时，低电平有效，触发蜂鸣器
				{	
				    delay_ms(5000);
					port_example->gpio_write(0x00000, 0x10000);
					port_example->gpio_read(&data, 0x10000);
					EMBARC_PRINTF(" gpio read after write is :%x\n", data);
					port_jidianqi->gpio_write(0x80000, 0x80000);
					delay_ms(7000);

					char texta1[]={" 疑似着火,请检查 "};
					yuyin(texta1);	
					send(phoneNum, msg);  //发短信
					delay_ms(1000);
				}
				else
				{
					//**************************检测人是否在*****************************************				
						if(hwdata==0)    //人在
							{
								port_example->gpio_write(0x10000, 0x10000);  	//	蜂鸣器不叫
								port_example->gpio_read(&data, 0x10000);
								EMBARC_PRINTF(" gpio read after write is :%x\n", data);
								port_jidianqi->gpio_write(0x00000, 0x80000);   //继电器正常
								port_jidianqi->gpio_read(&jiddata, 0x80000);
								EMBARC_PRINTF(" gpio read after write is :%x\n", jiddata);
								cmd1=0;
				
							}
						else                           //人不在，语音提醒
							{
								delay_ms(5000);
								char texta2[]={" 离开请关焊台 "};
								yuyin(texta2);		
			                    delay_ms(7000);
			                    port_jidianqi->gpio_write(0x80000, 0x80000);
								port_jidianqi->gpio_read(&jiddata, 0x80000);
								EMBARC_PRINTF(" gpio read after write is :%x\n", jiddata);

							}	
				
				}

			 	delay_ms(5000);
		}
     return E_SYS;	/* system error */
	}  
  	
		

