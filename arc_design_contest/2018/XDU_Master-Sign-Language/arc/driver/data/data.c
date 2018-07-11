#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "dev_iic.h"
#include "dev_uart.h"
#define I2C_SLAVE_ADDRAD  0x28
#define CHANNEL_0  0x10
#define CHANNEL_1  0x20
#define CHANNEL_2  0x40
#define CHANNEL_3  0x80
DEV_IIC *dev_iicAD=NULL;
DEV_UART *dev_uart0 = NULL;
const char ad_channels[] = { CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3 };
unsigned char ucStra[6],ucStrw[6],ucStrAngle[6];
char write_buf[20];
char read_buf[20];
uint8_t rcv_buf[33];
unsigned int i;
unsigned int res;
char strbuf[256];
uint32_t rcv_cnt;
uint32_t baudrate = 230400;
uint32_t rd_avail = 0;
double aValue[3];
double wValue[3];
double AValue[3];
int adc[4];
//*************************************Processing attitude sensor data*************************************************
void mpu6050_isr(unsigned char ucstra[6] ,unsigned char ucStrw[6],unsigned char ucStrAngle[6],uint8_t Re_buf[33])
{    
	    if(Re_buf[0]!=0x55) return;      //No. 0 data is not a frame header
	                              
			switch(Re_buf [1])
			{
			case 0x51:
			ucStra[0]=Re_buf[2];
			ucStra[1]=Re_buf[3];
			ucStra[2]=Re_buf[4];
			ucStra[3]=Re_buf[5];
			ucStra[4]=Re_buf[6];
			ucStra[5]=Re_buf[7];
			break;
			}
			
			switch(Re_buf [12])
			{
			case 0x52:	 
			ucStrw[0]=Re_buf[13];
			ucStrw[1]=Re_buf[14];
			ucStrw[2]=Re_buf[15];
			ucStrw[3]=Re_buf[16];
			ucStrw[4]=Re_buf[17];
			ucStrw[5]=Re_buf[18];
			break;
			}
			switch(Re_buf [23])
			{
			case 0x53: 
			ucStrAngle[0]=Re_buf[24];
			ucStrAngle[1]=Re_buf[25];
			ucStrAngle[2]=Re_buf[26];
			ucStrAngle[3]=Re_buf[27];
			ucStrAngle[4]=Re_buf[28];
			ucStrAngle[5]=Re_buf[29];	
			break;
			} 
	      
}
//************************************************************************************

//*********************************
void data( int adc[4],double aValue[3],double wValue[3],double AValue[3])
{
//****************************Collecting bending sensor data****************************************	     
    dev_iicAD = iic_get_dev(DW_IIC_1_ID	);
    dev_iicAD->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_iicAD->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDRAD));
    dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0->uart_open(baudrate);
		 for (i = 0; i < 4; i++) 
		 {
            adc[i] = 0;
            write_buf[0] = ad_channels[i];
			res=dev_iicAD->iic_write(write_buf, 1);	
		    rcv_cnt = dev_iicAD->iic_read(read_buf, 2);	
	      if(rcv_cnt)
	       {
	            res = read_buf[0];
			    res = ((res << 8) & 0x0F00) | read_buf[1];
			    adc[i] = res;
	        }
		 }
	       printf("ch0: data = 0x%.3X, ch1: data = 0x%.3X, ch2: data = 0x%.3X, ch3: data = 0x%.3X \r\n", 
	    		adc[0], adc[1], adc[2], adc[3]);
//**************************************************************************************************

//***************************************Collecting attitude sensor data*****************************************
          dev_uart0->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
		  if(rd_avail > 0)
		  {
		   rcv_cnt = dev_uart0->uart_read(rcv_buf, rd_avail);	
			if (rcv_cnt) 
			{
				mpu6050_isr(ucStra,ucStrw,ucStrAngle,rcv_buf);	
		 	aValue[0] = ((short)(ucStra[1]<<8| ucStra[0]))/32768.0*16;
			aValue[1] = ((short)(ucStra[3]<<8| ucStra[2]))/32768.0*16;
			aValue[2] = ((short)(ucStra[5]<<8| ucStra[4]))/32768.0*16;
			printf("a:%4.3f %4.3f %4.3f  ",aValue[0],aValue[1],aValue[2]); 
			
		 	wValue[0] = ((short)(ucStrw[1]<<8| ucStrw[0]))/32768.0*2000;
			wValue[1] = ((short)(ucStrw[3]<<8| ucStrw[2]))/32768.0*2000;
			wValue[2] = ((short)(ucStrw[5]<<8| ucStrw[4]))/32768.0*2000;
			printf("w:%4.3f %4.3f %4.3f  ",wValue[0],wValue[1],wValue[2]); 

		 	AValue[0] = ((short)(ucStrAngle[1]<<8| ucStrAngle[0]))/32768.0*180;
			AValue[1] = ((short)(ucStrAngle[3]<<8| ucStrAngle[2]))/32768.0*180;
			AValue[2] = ((short)(ucStrAngle[5]<<8| ucStrAngle[4]))/32768.0*180;
			printf("A:%4.2f %4.2f %4.2f\r\n",AValue[0],AValue[1],AValue[2]); 
			}
		  }
//**************************************************************************************************
}
 
 
