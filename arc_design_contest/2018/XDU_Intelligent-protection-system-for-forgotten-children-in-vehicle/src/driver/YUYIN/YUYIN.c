/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "stdio.h"
#include "string.h"
#include "board.h"

#include "dev_uart.h"
#include "dev_iic.h" 

#include "YUYIN.h"

#define HEADLEN       5 
#define I2C_SLAVE_ADDR1 0x01

static DEV_IIC *dev_IIC_YY;
static DEV_GPIO_PTR port_open_con;
static DEV_GPIO_PTR port_close_con;


static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
       for(y=z;y>0;y--);
}



void  Txt2YY_temp(int32_t TEMP_Value )
{
 	uint8_t  length;
    char  ecc  = 0x00;  			//定义校验字节
 	uint8_t i=0;
    char *text; 
	if(TEMP_Value==10) text="当前温度为10摄氏度";
	if(TEMP_Value==11) text="当前温度为11摄氏度";
	if(TEMP_Value==12) text="当前温度为12摄氏度";
	if(TEMP_Value==13) text="当前温度为13摄氏度";
	if(TEMP_Value==14) text="当前温度为14摄氏度";
	if(TEMP_Value==15) text="当前温度为15摄氏度";
	if(TEMP_Value==16) text="当前温度为16摄氏度";
	if(TEMP_Value==17) text="当前温度为17摄氏度";
	if(TEMP_Value==18) text="当前温度为18摄氏度";
	if(TEMP_Value==19) text="当前温度为19摄氏度";
	if(TEMP_Value==20) text="当前温度为20摄氏度";
	if(TEMP_Value==21) text="当前温度为21摄氏度";
	if(TEMP_Value==22) text="当前温度为22摄氏度";
	if(TEMP_Value==23) text="当前温度为23摄氏度";
	if(TEMP_Value==24) text="当前温度为24摄氏度";
	if(TEMP_Value==25) text="当前温度为25摄氏度";
	if(TEMP_Value==26) text="当前温度为26摄氏度";
	if(TEMP_Value==27) text="当前温度为27摄氏度";
	if(TEMP_Value==28) text="当前温度为28摄氏度";
	if(TEMP_Value==29) text="当前温度为29摄氏度";
	if(TEMP_Value==30) text="当前温度为30摄氏度";
	if(TEMP_Value==31) text="当前温度为31摄氏度";
	if(TEMP_Value==32) text="当前温度为32摄氏度";
	if(TEMP_Value==33) text="当前温度为33摄氏度";
	if(TEMP_Value==34) text="当前温度为34摄氏度";
	if(TEMP_Value==35) text="当前温度为35摄氏度";	
	if(TEMP_Value==36) text="当前温度为36摄氏度";
	if(TEMP_Value==37) text="当前温度为37摄氏度";
	if(TEMP_Value==38) text="当前温度为38摄氏度";
	if(TEMP_Value==39) text="当前温度为39摄氏度";
	if(TEMP_Value==40) text="当前温度为40摄氏度";
	
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//需要发送文本的长度
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //依次发送构造好的5个帧头字节
 	{
 	    ecc=ecc^(head[i]); 		    //对发送的字节进行异或校 	         
    }             
	dev_IIC_YY->iic_write(head, 5);
 	for(i = 0; i<length+1; i++)   		
 	{
 	   if(i==length)
 		  text[i] = ecc;
 	   else
 		  ecc=ecc^(text[i]);
	  
 	}
    dev_IIC_YY->iic_write(text, length+1);
	delay_ms(1000);
}

void  Txt2YY_command1()
{
 	uint8_t  length;
    char  ecc  = 0x00;  			//定义校验字节
 	uint8_t i=0;
	char  text[]={"主人您有什么吩咐"};
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//需要发送文本的长度
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //依次发送构造好的5个帧头字节
 	{
 	    ecc=ecc^(head[i]); 		    //对发送的字节进行异或校 	         
    }             
	dev_IIC_YY->iic_write(head, 5);
 	for(i = 0; i<length+1; i++)   		
 	{
 	   if(i==length)
 		  text[i] = ecc;
 	   else
 		  ecc=ecc^(text[i]);
	  
 	}
    dev_IIC_YY->iic_write(text, length+1);
	delay_ms(1000);
}

void  Txt2YY_command2()
{
 	uint8_t  length;
    char  ecc  = 0x00;  			//定义校验字节
 	uint8_t i=0;
	char  text[]={"空调已打开"};
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//需要发送文本的长度
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //依次发送构造好的5个帧头字节
 	{
 	    ecc=ecc^(head[i]); 		    //对发送的字节进行异或校 	         
    }             
	dev_IIC_YY->iic_write(head, 5);
 	for(i = 0; i<length+1; i++)   		
 	{
 	   if(i==length)
 		  text[i] = ecc;
 	   else
 		  ecc=ecc^(text[i]);
	  
 	}
    dev_IIC_YY->iic_write(text, length+1);
	delay_ms(1000);
}

void  Txt2YY_command3()
{
 	uint8_t  length;
    char  ecc  = 0x00;  			//定义校验字节
 	uint8_t i=0;
	char  text[]={"空调已关闭"};
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//需要发送文本的长度
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //依次发送构造好的5个帧头字节
 	{
 	    ecc=ecc^(head[i]); 		    //对发送的字节进行异或校 	         
    }             
	dev_IIC_YY->iic_write(head, 5);
 	for(i = 0; i<length+1; i++)   		
 	{
 	   if(i==length)
 		  text[i] = ecc;
 	   else
 		  ecc=ecc^(text[i]);
	  
 	}
    dev_IIC_YY->iic_write(text, length+1);
	delay_ms(1000);
}

void  Txt2YY_command4()
{
 	uint8_t  length;
    char  ecc  = 0x00;  			//定义校验字节
 	uint8_t i=0;
	char  text[]={"对不起我还在学习中"};
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//需要发送文本的长度
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //依次发送构造好的5个帧头字节
 	{
 	    ecc=ecc^(head[i]); 		    //对发送的字节进行异或校 	         
    }             
	dev_IIC_YY->iic_write(head, 5);
 	for(i = 0; i<length+1; i++)   		
 	{
 	   if(i==length)
 		  text[i] = ecc;
 	   else
 		  ecc=ecc^(text[i]);
	  
 	}
    dev_IIC_YY->iic_write(text, length+1);
	delay_ms(1000);
}


void open_conditioner(void)  
{	 
    port_open_con = gpio_get_dev(DW_GPIO_PORT_A);//定义port_a接口
	port_open_con ->gpio_open(0x800);//打开PortA[11]
	port_open_con ->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x800);//设置portA[11]为输出
	port_open_con ->gpio_write(0x800, 0x800);		
}



void close_conditioner(void)  
{	 
    port_close_con = gpio_get_dev(DW_GPIO_PORT_A);//定义port_a接口
	port_close_con ->gpio_open(0x800);//打开PortA[11]
	port_close_con ->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x800);//设置portA[11]为输出
	port_close_con ->gpio_write(0x000, 0x800);		
}








