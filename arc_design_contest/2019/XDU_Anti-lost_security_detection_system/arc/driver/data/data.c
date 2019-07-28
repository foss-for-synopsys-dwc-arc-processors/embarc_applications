/* ------------------------------------------
 * \version 2016-01
 * \date 2019-05-20
 * \author Renjie Cao
 * \functionJY901模块
--------------------------------------------- */
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "board.h"
#include "dev_uart.h"



unsigned char ucStra[6],ucStrw[6],ucStrAngle[6];
double aValue[3];
double wValue[3];
double AValue[3];
DEV_UART *dev_uart0 = NULL;

void mpu6050_isr(unsigned char ucstra[6] ,unsigned char ucStrw[6],unsigned char ucStrAngle[6],uint8_t Re_buf[33])
{    
	    if(Re_buf[0]!=0x55) return;      //第0号数据不是帧头
	                          //重新赋值，准备下一帧数据的接收        
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


static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
		for(y=z;y>0;y--);
}

void data0(double aValue[3],double wValue[3],double AValue[3],double *Z1,double *AWM,double *A1,double *A2)
{
    uint8_t rcv_buf[33];
	unsigned int j;
	uint32_t rcv_cnt;
	uint32_t baudrate =230400 ;
	uint32_t rd_avail = 0;
	
    dev_uart0 = uart_get_dev(DW_UART_0_ID);
	dev_uart0->uart_open(baudrate);
	dev_uart0->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));

    if(rd_avail > 0){
		    rcv_cnt = dev_uart0->uart_read(rcv_buf, rd_avail);
	    if (rcv_cnt) {
				mpu6050_isr(ucStra,ucStrw,ucStrAngle,rcv_buf);	
				aValue[0] = ((short)(ucStra[1]<<8| ucStra[0]))/32768.0*16;
				aValue[1] = ((short)(ucStra[3]<<8| ucStra[2]))/32768.0*16;
				aValue[2] = ((short)(ucStra[5]<<8| ucStra[4]))/32768.0*16;
				printf("{%4.3f,%4.3f,%4.3f,",aValue[0],aValue[1],aValue[2]); 
				
				*Z1 =  aValue[2];
				*AWM = aValue[0]*aValue[0]+aValue[1]*aValue[1]+aValue[2]*aValue[2];
			    printf("AWM:%4.3f,",*AWM);
				
				wValue[0] = ((short)(ucStrw[1]<<8| ucStrw[0]))/32768.0*2000;
				wValue[1] = ((short)(ucStrw[3]<<8| ucStrw[2]))/32768.0*2000;
				wValue[2] = ((short)(ucStrw[5]<<8| ucStrw[4]))/32768.0*2000;
				//printf("%4.3f,%4.3f,%4.3f,",wValue[0],wValue[1],wValue[2]); 

				AValue[0] = ((short)(ucStrAngle[1]<<8| ucStrAngle[0]))/32768.0*180;
				AValue[1] = ((short)(ucStrAngle[3]<<8| ucStrAngle[2]))/32768.0*180;
				AValue[2] = ((short)(ucStrAngle[5]<<8| ucStrAngle[4]))/32768.0*180;
		//		printf("%4.2f,%4.2f,%4.2f},\r\n",AValue[0],AValue[1],AValue[2]);
				*A1=AValue[0];
				*A2=AValue[1];
				printf("A1:%4.3f,",*A1);
				printf("A2:%4.3f,\r\n",*A2);	
				}
			  }	 
}
			
			
			
			
			