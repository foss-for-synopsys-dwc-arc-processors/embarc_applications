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
    char  ecc  = 0x00;  			//����У���ֽ�
 	uint8_t i=0;
    char *text; 
	if(TEMP_Value==10) text="��ǰ�¶�Ϊ10���϶�";
	if(TEMP_Value==11) text="��ǰ�¶�Ϊ11���϶�";
	if(TEMP_Value==12) text="��ǰ�¶�Ϊ12���϶�";
	if(TEMP_Value==13) text="��ǰ�¶�Ϊ13���϶�";
	if(TEMP_Value==14) text="��ǰ�¶�Ϊ14���϶�";
	if(TEMP_Value==15) text="��ǰ�¶�Ϊ15���϶�";
	if(TEMP_Value==16) text="��ǰ�¶�Ϊ16���϶�";
	if(TEMP_Value==17) text="��ǰ�¶�Ϊ17���϶�";
	if(TEMP_Value==18) text="��ǰ�¶�Ϊ18���϶�";
	if(TEMP_Value==19) text="��ǰ�¶�Ϊ19���϶�";
	if(TEMP_Value==20) text="��ǰ�¶�Ϊ20���϶�";
	if(TEMP_Value==21) text="��ǰ�¶�Ϊ21���϶�";
	if(TEMP_Value==22) text="��ǰ�¶�Ϊ22���϶�";
	if(TEMP_Value==23) text="��ǰ�¶�Ϊ23���϶�";
	if(TEMP_Value==24) text="��ǰ�¶�Ϊ24���϶�";
	if(TEMP_Value==25) text="��ǰ�¶�Ϊ25���϶�";
	if(TEMP_Value==26) text="��ǰ�¶�Ϊ26���϶�";
	if(TEMP_Value==27) text="��ǰ�¶�Ϊ27���϶�";
	if(TEMP_Value==28) text="��ǰ�¶�Ϊ28���϶�";
	if(TEMP_Value==29) text="��ǰ�¶�Ϊ29���϶�";
	if(TEMP_Value==30) text="��ǰ�¶�Ϊ30���϶�";
	if(TEMP_Value==31) text="��ǰ�¶�Ϊ31���϶�";
	if(TEMP_Value==32) text="��ǰ�¶�Ϊ32���϶�";
	if(TEMP_Value==33) text="��ǰ�¶�Ϊ33���϶�";
	if(TEMP_Value==34) text="��ǰ�¶�Ϊ34���϶�";
	if(TEMP_Value==35) text="��ǰ�¶�Ϊ35���϶�";	
	if(TEMP_Value==36) text="��ǰ�¶�Ϊ36���϶�";
	if(TEMP_Value==37) text="��ǰ�¶�Ϊ37���϶�";
	if(TEMP_Value==38) text="��ǰ�¶�Ϊ38���϶�";
	if(TEMP_Value==39) text="��ǰ�¶�Ϊ39���϶�";
	if(TEMP_Value==40) text="��ǰ�¶�Ϊ40���϶�";
	
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//��Ҫ�����ı��ĳ���
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //���η��͹���õ�5��֡ͷ�ֽ�
 	{
 	    ecc=ecc^(head[i]); 		    //�Է��͵��ֽڽ������У 	         
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
    char  ecc  = 0x00;  			//����У���ֽ�
 	uint8_t i=0;
	char  text[]={"��������ʲô�Ը�"};
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//��Ҫ�����ı��ĳ���
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //���η��͹���õ�5��֡ͷ�ֽ�
 	{
 	    ecc=ecc^(head[i]); 		    //�Է��͵��ֽڽ������У 	         
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
    char  ecc  = 0x00;  			//����У���ֽ�
 	uint8_t i=0;
	char  text[]={"�յ��Ѵ�"};
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//��Ҫ�����ı��ĳ���
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //���η��͹���õ�5��֡ͷ�ֽ�
 	{
 	    ecc=ecc^(head[i]); 		    //�Է��͵��ֽڽ������У 	         
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
    char  ecc  = 0x00;  			//����У���ֽ�
 	uint8_t i=0;
	char  text[]={"�յ��ѹر�"};
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//��Ҫ�����ı��ĳ���
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //���η��͹���õ�5��֡ͷ�ֽ�
 	{
 	    ecc=ecc^(head[i]); 		    //�Է��͵��ֽڽ������У 	         
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
    char  ecc  = 0x00;  			//����У���ֽ�
 	uint8_t i=0;
	char  text[]={"�Բ����һ���ѧϰ��"};
	dev_IIC_YY = iic_get_dev(DW_IIC_1_ID);
    dev_IIC_YY-> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_IIC_YY-> iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR1));
    
	length =strlen(text); 			//��Ҫ�����ı��ĳ���
	printf("length=%d\r\n",length);
    char head[HEADLEN] = {0xfd,0x00,0x00,0x01,0x00};
    head[2] = length + 3;
    for(i = 0; i<5; i++)   		    //���η��͹���õ�5��֡ͷ�ֽ�
 	{
 	    ecc=ecc^(head[i]); 		    //�Է��͵��ֽڽ������У 	         
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
    port_open_con = gpio_get_dev(DW_GPIO_PORT_A);//����port_a�ӿ�
	port_open_con ->gpio_open(0x800);//��PortA[11]
	port_open_con ->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x800);//����portA[11]Ϊ���
	port_open_con ->gpio_write(0x800, 0x800);		
}



void close_conditioner(void)  
{	 
    port_close_con = gpio_get_dev(DW_GPIO_PORT_A);//����port_a�ӿ�
	port_close_con ->gpio_open(0x800);//��PortA[11]
	port_close_con ->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x800);//����portA[11]Ϊ���
	port_close_con ->gpio_write(0x000, 0x800);		
}








