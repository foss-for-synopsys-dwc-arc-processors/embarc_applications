#include "yuyin.h"

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
void  yuyin_llt()
{    
/****************需要发送的文本**********************************/   
	char  text[]={"abcde这是李林涛"};
    uint8_t  length;  
    char  ecc  = 0x00;  			//定义校验字节
	uint8_t i=0; 
    length =sizeof(text) - 5; 			//需要发送文本的长度
	uint32_t baudrate = 9600;
    dev_uart2 = uart_get_dev(DFSS_UART_1_ID);
	dev_uart2->uart_open(baudrate);
 
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};

			head[2] = length + 2;
		 for(i = 0; i<5; i++)   				//依次发送构造好皿个帧头字芿
	     {  
		     text[i] = head[i];
	         ecc=ecc^(head[i]); 		//对发送的字节进行异或校验	
			 
		 }   
//		     printf("length=%d\r\n",length);	     
//             dev_uart2->uart_write(head, 5);
	   	 for(i = 5; i<length+5; i++)   		//依次发送待合成的文本数捿
	     {  

			 if(i==length+4) 
				 text[i] = ecc;
			 else 
				 ecc=ecc^(text[i]);
//			 printf("text[%d]=%d\r\n",i,text[i]);
	      
	     }
//		    printf("ecc=%d\r\n",ecc);
            dev_uart2->uart_write(text, length+5);
            delay_ms(500);			
}

void  yuyin_lx() 
{        
/****************需要发送的文本**********************************/ 
	char  text[]={"abcde这是刘轩"};
    uint8_t  length;  
    char  ecc  = 0x00;  			//定义校验字节
	uint8_t i=0; 
    length =sizeof(text) - 5; 			//需要发送文本的长度
	uint32_t baudrate = 9600;
    dev_uart2 = uart_get_dev(DFSS_UART_1_ID);
	dev_uart2->uart_open(baudrate);
    //dfss_uart_1_install();
       char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00}; 		 
       int m;
//      while (1) 
//	  {
		ecc  = 0x00;
		
			head[2] = length + 2;
		 for(i = 0; i<5; i++)   				//依次发送构造好皿个帧头字芿
	     {  
			 text[i] = head[i];
	         ecc=ecc^(head[i]); 		//对发送的字节进行异或校验	
	         //SBUF = headOfFrame[i]; 
//			 printf("head[%d]=%d\r\n",i,head[i]);
			 
		 }   
//		     printf("length=%d\r\n",length);
	     
            // m = dev_uart2->uart_write(head, 5);
//			 printf("m=%d\r\n",m);
	   	 for(i = 5; i<length+5; i++)   		//依次发送待合成的文本数捿
	     {  
	          				//对发送的字节进行异或校验	
	         //SBUF = text[i]; 
			 if(i==length+4) 
				 text[i] = ecc;
			 else 
				 ecc=ecc^(text[i]);
//			 printf("text[%d]=%d\r\n",i,text[i]);
	      
	     }
//		    printf("ecc=%d\r\n",ecc);
            dev_uart2->uart_write(text, length+5);
			//dev_uart2->uart_write(ecc, 1);
            delay_ms(500);			

//	  }
}

void  yuyin_szw()
{    
/****************需要发送的文本**********************************/   
	char  text[]={"abcde这是苏昭伟"};
    uint8_t  length;  
    char  ecc  = 0x00;  			//定义校验字节
	uint8_t i=0; 
    length =sizeof(text) - 5; 			//需要发送文本的长度
	uint32_t baudrate = 9600;
    dev_uart2 = uart_get_dev(DFSS_UART_1_ID);
	dev_uart2->uart_open(baudrate);
 
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};

			head[2] = length + 2;
		 for(i = 0; i<5; i++)   				//依次发送构造好皿个帧头字芿
	     {  
		     text[i] = head[i];
	         ecc=ecc^(head[i]); 		//对发送的字节进行异或校验	
			 
		 }   
//		     printf("length=%d\r\n",length);	     
//             dev_uart2->uart_write(head, 5);
	   	 for(i = 5; i<length+5; i++)   		//依次发送待合成的文本数捿
	     {  

			 if(i==length+4) 
				 text[i] = ecc;
			 else 
				 ecc=ecc^(text[i]);
//			 printf("text[%d]=%d\r\n",i,text[i]);
	      
	     }
//		    printf("ecc=%d\r\n",ecc);
            dev_uart2->uart_write(text, length+5);
            delay_ms(500);			
}

void  yuyin_wfsb() 
{        
/****************需要发送的文本**********************************/ 
	char  text[]={"abcde无法识别"};
    uint8_t  length;  
    char  ecc  = 0x00;  			//定义校验字节
	uint8_t i=0; 
    length =sizeof(text) - 5; 			//需要发送文本的长度
	uint32_t baudrate = 9600;

    dev_uart2 = uart_get_dev(DFSS_UART_1_ID);

	dev_uart2->uart_open(baudrate);
    //dfss_uart_1_install();

       char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};	 
       int m;
//       while (1) 
//	  {
		ecc  = 0x00;
		
			head[2] = length + 2;
		 for(i = 0; i<5; i++)   				//依次发送构造好皿个帧头字芿
	     {  
			 text[i] = head[i];
	         ecc=ecc^(head[i]); 		//对发送的字节进行异或校验	
//			 printf("head[%d]=%d\r\n",i,head[i]);
			 
		 }   
//		     printf("length=%d\r\n",length);
	     
            // m = dev_uart2->uart_write(head, 5);
//			 printf("m=%d\r\n",m);
	   	 for(i = 5; i<length+5; i++)   		//依次发送待合成的文本数捿
	     {  
	          				//对发送的字节进行异或校验	
			 if(i==length+4) 
				 text[i] = ecc;
			 else 
				 ecc=ecc^(text[i]);
//			 printf("text[%d]=%d\r\n",i,text[i]);
	      
	     }
//		    printf("ecc=%d\r\n",ecc);
            dev_uart2->uart_write(text, length+5);
			//dev_uart2->uart_write(ecc, 1);
            delay_ms(500);			
//	  }
}
