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
#define HEADLEN       5  //���ݰ�ͷ�ĳ���
#define TH0   25  //����ȫ������ֵ
#define TH1   1.25  //�ͼ��ٶȵĴ�С
#define TH2   1.18  //���ٶȾ�ֵ�Ĵ�С
#define TH30    55  //������ǵ���ֵ
#define TH31    -55  //������ǵ���ֵ

static DEV_GPIO_PTR port_stop;//��ͣ��
static DEV_GPIO_PTR port_voice;//��Ӣ�л�
double aValue[3];//�ֲ��˶����ٶ�
double wValue[3];//�ֲ��˶����ٶ�
double AValue[3];//�ֲ��˶��Ƕ�
double AWM,Sawm,Z1,A1,A2;
int    BLESTATE;

DEV_IIC *dev_iicyu=NULL;

uint8_t *phoneNum = "00310035003000300032003900380036003400360036"; //�໤���ֻ�����
uint8_t *msg1 = "7528623753EF80FD88AB62B18D77 "; //�û����ܱ�����
uint8_t *msg2 = "752862378DCC5012"; //�û�����


//********************�ӳٵ�Ԫ************************************
static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}
//*****************************************************************

//*********************�����ϳ�*************************************
void  yuyin(char text[]) 
{    
    dev_iicyu = iic_get_dev(DW_IIC_0_ID	);
    dev_iicyu->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_iicyu->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
/****************��Ҫ���͵��ı�**********************************/ 
	uint8_t  length=0x00;  
    char  ecc  = 0x00;  			//����У���ֽ�
	uint8_t i=0; 
    //length =sizeof(text); 			//��Ҫ�����ı��ĳ���
	while(text[length++]);
/*****************���͹���**************************************/           
		// headOfFrame[0] = 0xFD ; 			//����֡ͷFD
		// headOfFrame[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
		// headOfFrame[2] = length + 3; 		//�������������ȵĵ��ֽ�
		// headOfFrame[3] = 0x01 ; 			//���������֣��ϳɲ�������
/*****************����������**************************************/  		 
		// headOfFrame[4] = 0x00 ; 			//������������������ʽΪGBK
       char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
/*****************������������**************************************/  		 
		//headOfFrame[4] = 0x01 ; 			//������������������ʽΪGBK
      
		
		
		head[2] = length + 3;
		
		 for(i = 0; i<5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
	     {  
	         ecc=ecc^(head[i]); 		//�Է��͵��ֽڽ������У��	
	         //SBUF = headOfFrame[i]; 
			// printf("head[%d]=%d\r\n",i,head[i]);
			 
		 }   
		    // printf("length=%d\r\n",length);
	     
             dev_iicyu->iic_write(head, 5);
	   	 
		 
		 for(i = 0; i<length+1; i++)   		//���η��ʹ��ϳɵ��ı�����
	     {  
	          				//�Է��͵��ֽڽ������У��	
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

//**********************************����ģʽ************************************
	  
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
		  printf("���Ѿ���������׶�\r\n");
	   }
        else  if(DIP1==0x100)
	   {   
//*****************************************************************************
		    printf("���Ѿ���������׶�\r\n");
			data0(aValue,wValue,AValue,&Z1,&AWM,&A1,&A2);
		    printf("AWM:%4.3f,",AWM);
			

//**********************************һ������************************************
            
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
			   EMBARC_PRINTF("�Զ�����\r\n");

			   char texta[]={"���,��������"};
		   	   yuyin(texta);
			   delay_ms(6000);
			   char textb[]={"�Ҽ����ڸ���"};
		   	   yuyin(textb);
			   delay_ms(5000);
			   char textc[]={"��ϵ�绰15236578965"};
		   	   yuyin(textc);
			   delay_ms(9000);

		   }
//*****************************************************************************
        
//**********************************������************************************
		else if(AWM == 0 | BLESTATE  ==0 )
		   {
			printf("���������侯��\r\n");
			beep();
			//send(phoneNum,msg);
	        delay_ms(500); 
		   }
//*****************************************************************************

//**********************************��̬���************************************
			
		else
		{
			data0(aValue,wValue,AValue,&Z1,&AWM,&A1,&A2);
			printf("AWM:%4.3f,\r\n",AWM);
			m=0;
			j=0;
		    Max = Z1;
			k=4;
			if(AWM > 0.990 & AWM < 1.050){
				printf("���ܴ��ھ�ֹ\r\n");
			}
			if(AWM > TH1 | Z1>=1.25){
				if(Z1>=3.8)  continue;
				else if(Z1>=2.5 & Z1 <3.8) {
					printf("ֱ�ӱ�����\r\n");
					send(phoneNum,msg1);
					delay_ms(5000); 
					continue;
				}
				else {
					Sum=0;
					printf("�����жϻ���\r\n");
					Max = (Max > Z1)? Max : Z1;
					for(i = 0; i< 18; i++) {
						data0(aValue,wValue,AValue,&Z1,&AWM,&A1,&A2);
						delay_ms(150);  
						Max = (Max > Z1)? Max : Z1;
						Sum += AWM;   
					//�����ж��Ƿ�ָ��ȶ�
						if(0.93<=Z1 & Z1<=1.08 & (k-1)>0)  
						{
							j=i;
							k=k-1;
						}
						
						printf("Sum:%4.3f,\r\n",Sum);
					}			
					Aver = Sum/18;
					printf("Aver:%4.3f,\r\n",Aver);//���ƽ�����ٶ�
					
					m = (1 < j< 11)? (j-1) : 18; //�õ��ָ��ȶ���Ҫ��ʱ��
					printf("m:%d,\r\n",m);
					
					if(Aver < TH2 ){
						printf("��������׶�\r\n");					
						printf("A1:%4.3f,\r\n",A1);
						printf("A2:%4.3f,\r\n",A2);
						
						if(A1>=TH30 | A1<=TH31 | A2>=TH30 | A2<=TH31 ) {
							printf("����\r\n");
							send(phoneNum,msg2);
							delay_ms(3000); 
							continue;
						}
						 else if(m > 1 & m < 12 & ((Max-1)*m*m >=TH0)) //�ж��Ƿ񳬳�����ȫ����
						{
						  printf("������\r\n");
						  printf("m:%d,\r\n",m);
						  send(phoneNum,msg1);
						  delay_ms(7000); 
						  continue;
						}
						else
						{
							printf("���ص�һ�׶�\r\n");
						}
				}
			}
			}
			
		}	
        printf("��������\r\n");
//*****************************************************************************

//**********************************��λ����************************************
		if(BLESTATE == 1 & !AWM ==0){
		printf("�������з�Χ\r\n");	
		send_msg();
		delay_ms(3000); 
	    }
		
	}
	
}
	return E_SYS;
	
	}