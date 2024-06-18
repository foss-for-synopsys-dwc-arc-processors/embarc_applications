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
#define HEADLEN       5  //���ݰ�ͷ�ĳ���

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
	//char  text[]={"��ӭ����"};
    uint8_t  length;  
    char  ecc  = 0x00;  			//����У���ֽ�
	uint8_t i=0; 
    length =sizeof(text); 			//��Ҫ�����ı��ĳ���

	uint32_t baudrate = 115200;
    dev_uart2 = uart_get_dev(DW_UART_2_ID);
	dev_uart2->uart_open(baudrate);
 
	while(text[length++]);
	{
        char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
		head[2] = length + 2;
		 for(i = 0; i<5; i++)   				//���η��͹�������֡ͷ���e
	     {  
	         ecc=ecc^(head[i]); 		//�Է��͵��ֽڽ������У��	
			 printf("head[%d]=%d\r\n",i,head[i]);
			 
		 }   
		     printf("length=%d\r\n",length);
	     
             dev_uart2->uart_write(head, 5);
	   	 for(i = 0; i<length; i++)   		//���η��ʹ��ϳɵ��ı�����
	     {  
	          				//�Է��͵��ֽڽ������У��	
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
	/////////////////////////////////////////////////////////////////////////��C�� ������
	port_example = gpio_get_dev(DW_GPIO_PORT_C);//����port_c�ӿ�
	port_example->gpio_open(0x10000);//open C[16]
	port_example->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10000);//����portC[16]Ϊ���
   
    	
	/////////////////////////////////////////////////////////////////////////��C�� ��������1 2
	port_huoyan = gpio_get_dev(DW_GPIO_PORT_C);//����GPIO C[17]���ƻ���˿ڵ�����
	port_huoyan->gpio_open(0x20000);//open C[17]
	port_huoyan->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x20000);//����C[17]����
	
	
	/////////////////////////////////////////////////////////////////////////��C�� ��������
	port_yanwu = gpio_get_dev(DW_GPIO_PORT_C);//����GPIO C[18]��������˿ڵ�����
	port_yanwu->gpio_open(0x40000);//open C[18]
	port_yanwu->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x40000);//����C[17]����
	
/////////////////////////////////////////////////////////////////////////��C�� ���ƺ����Ӧģ�������	
	port_hongwai = gpio_get_dev(DW_GPIO_PORT_C);//����GPIO C[19]����
	port_hongwai->gpio_open(0x80000);//open C[19]  0x80000
	port_hongwai->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x80000);
	
	/////////////////////////////////////////////////////////////////////////��A�� ���Ƽ̵��������	
	port_jidianqi = gpio_get_dev(DW_GPIO_PORT_A);//����GPIO A[19]����
	port_jidianqi->gpio_open(0x80000);//open A[19] 0x80000 
	port_jidianqi->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x80000);
	
	//initialize the uart0 which is on PMOD1 J3 for TXD J4 for RXD

	 	while(1)
		{
			    port_huoyan->gpio_read(&hdata, 0x20000);   //��ȡ����Ķ˿�1
				
				port_yanwu->gpio_read(&ywdata, 0x40000);   //��ȡ����Ķ˿�1
				
				port_hongwai->gpio_read(&hwdata, 0x80000);   //��ȡ����Ķ˿� �˲���
	//**************************������*****************************************			
				if(hdata==0||ywdata==0)                    //��⵽����������ʱ���͵�ƽ��Ч������������
				{	
				    delay_ms(5000);
					port_example->gpio_write(0x00000, 0x10000);
					port_example->gpio_read(&data, 0x10000);
					EMBARC_PRINTF(" gpio read after write is :%x\n", data);
					port_jidianqi->gpio_write(0x80000, 0x80000);
					delay_ms(7000);

					char texta1[]={" �����Ż�,���� "};
					yuyin(texta1);	
					send(phoneNum, msg);  //������
					delay_ms(1000);
				}
				else
				{
					//**************************������Ƿ���*****************************************				
						if(hwdata==0)    //����
							{
								port_example->gpio_write(0x10000, 0x10000);  	//	����������
								port_example->gpio_read(&data, 0x10000);
								EMBARC_PRINTF(" gpio read after write is :%x\n", data);
								port_jidianqi->gpio_write(0x00000, 0x80000);   //�̵�������
								port_jidianqi->gpio_read(&jiddata, 0x80000);
								EMBARC_PRINTF(" gpio read after write is :%x\n", jiddata);
								cmd1=0;
				
							}
						else                           //�˲��ڣ���������
							{
								delay_ms(5000);
								char texta2[]={" �뿪��غ�̨ "};
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
  	
		

