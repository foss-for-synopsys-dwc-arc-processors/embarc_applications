/* ------------------------------------------
 * \version 2016-01
 * \date 2019-05-20
 * \author Jinyu Yang
 * \functionGPS定位模块
--------------------------------------------- */
#include "arc.h"
#include "arc_builtin.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "board.h"
#include "dev_uart.h"
#include "dev_iic.h"
#define I2C_SLAVE_ADDR3  0x03
DEV_IIC *dev_iic2=NULL;
uint8_t *phoneNumber = "15002986466";

//static GPS_Information GPS; 
//static UTC_Time_Date UTC;
//static Latitude Lati;
//static Longitude Long;

uint8_t DH_id_sep[20];                            //全局变量数组，最多处理20个逗号索引



struct   GPS_Information
{
    uint8_t Located;           //定位有效位
    uint8_t UTC_Time[7];       //时间
    uint8_t UTC_Date[7];       //日期
    char    Latitude[11];      //纬度
    uint8_t NS_Indicator;      //N=北半球，S=南半球
    char    Longitude[12];     //经度
    uint8_t EW_Indicator;      //E=东经，W=西经
    uint8_t Speed[7];          //地面速率
    uint8_t Course[7];         //地面航向
    uint8_t MSL_Altitude[7];   //MSL海拔高度
    uint8_t Use_EPH_Sum[2];    //使用的卫星数量
	char lat_long[100];        //换算后的经纬度
}*GPS;                                    

struct UTC_Time_Date
{										    
 	uint8_t year;	//年份
	uint8_t month;	//月份
	uint8_t date;	//日期
	uint8_t hour; 	//小时
	uint8_t min; 	//分钟
	uint8_t sec; 	//秒钟
}*UTC;

struct Latitude
{										    
	uint8_t du; 	//度
	uint8_t fen; 	//分
	uint8_t miao; 	//秒
}*Lati;

struct  Longitude
{										    
	uint8_t du; 	//度
	uint8_t fen; 	//分
	uint8_t miao; 	//秒
}*Long;






static void delay_ms(volatile uint32_t z)//延时函数,单位是ms
{
	volatile uint32_t x,y;
	for (x = 1400 ;x > 0; x --)
		for (y = z ;y > 0; y --);
}

void Creat_DH_Index(uint8_t k ,int8_t* buffer )  //存储一帧数据中每个逗号的地址
{
	uint8_t i;
	uint8_t idj = 0;
    for ( i = 0;i < k;i++ )
    {
        if ( buffer[i] == ',' )
        {
            DH_id_sep[idj] = i;
            idj++;
        }
    }
}

int8_t* Real_Process_DH( int8_t* buffer, uint8_t num )  //返回第num个参数第一位数据的地址
{
    if ( num < 1 )
        return  &buffer[0];
	else
        return  &buffer[ DH_id_sep[num - 1] + 1];
}

void Real_GPS_Command_Process(uint8_t k, char *data )
{
	char *rd_pt;
	uint8_t i,t;

	for(i=0;i<k;i++)
	{
		//if( data[4] == 'C' )      //$GPRMC
		  
			rd_pt = Real_Process_DH( data, 1 );//第1个参数为UTC时间，时分秒
			if ( ( *rd_pt >= '0' ) && ( *rd_pt <= '9' ) )//内容是数字0到9
			{
				for(t=0;t<6;t++)
				{
					if( *rd_pt == ',' )  break;
					GPS->UTC_Time[t] = *rd_pt;
					rd_pt++;
			    }
				GPS->UTC_Time[t] = '\0';
//				EMBARC_PRINTF("GPS->UTC_Time=%s\r\n",GPS->UTC_Time);
           //     printf("GPS->UTC_Time=%s\r\n",GPS->UTC_Time);
			}
			
		/*	rd_pt = Real_Process_DH( data, 2 );//第2个参数，定位是否有效
		    if ( *rd_pt == 'A' )
			{
				GPS->Located = *rd_pt;//有效定位
//				EMBARC_PRINTF("GPS->Located=%c\r\n",GPS->Located);
			}
			else
			{
				GPS->Located = 0; //无效定位
				break;            //退出数据处理，重新接收收据
			} */
			
			rd_pt = Real_Process_DH( data, 2 ); //第3个参数为纬度
			if ( ( *rd_pt >= '0' ) && ( *rd_pt <= '9' ) )
			{
				for(t=0;t<9;t++)
				{
					if( *rd_pt == ',' )  break;
					GPS->Latitude[t] = *rd_pt;
					rd_pt++;
				}
			    GPS->Latitude[t] = '\0';  
				printf("GPS->Latitude=%s\r\n",GPS->Latitude);
			}
			
			rd_pt = Real_Process_DH( data, 3 );    //第4个参数为纬度半球，北纬或南纬
			if ( ( *rd_pt == 'N' ) || ( *rd_pt == 'S' ) )
			{
				GPS->NS_Indicator = *rd_pt;
				EMBARC_PRINTF("GPS->NS_Indicator=%c\r\n",GPS->NS_Indicator);
			}
			
			rd_pt = Real_Process_DH( data, 4 );    //第5个参数为经度
			if ( ( *rd_pt >= '0' ) && ( *rd_pt <= '9' ) )
			{
				for(t=0;t<9;t++)
				{
					if( *rd_pt == ',' )  break;
					GPS->Longitude[t] = *rd_pt;
					rd_pt++;
				}
				GPS->Longitude[t] = '\0';
				EMBARC_PRINTF("GPS->Longitude=%s\r\n",GPS->Longitude);
			}
			
			rd_pt = Real_Process_DH( data, 5 );    //第6个参数为经度半球，东经或西经
			if ( ( *rd_pt == 'E' ) || ( *rd_pt == 'W' ) )
			{
				GPS->EW_Indicator = *rd_pt;
				EMBARC_PRINTF("GPS->EW_Indicator=%c\r\n",GPS->EW_Indicator);
			}
			
			rd_pt = Real_Process_DH( data, 7 );     //第7个参数为地面速率
			if ( ( *rd_pt >= '0' ) && ( *rd_pt <= '9' ) )
			{
				for(t=0;t<7;t++)
				{
					if( *rd_pt == ',' )  break;
					GPS->Speed[t] = *rd_pt;
					rd_pt++;
				}
				GPS->Speed[t] = '\0';
				EMBARC_PRINTF("GPS->Speed=%s\r\n",GPS->Speed);
			}
			
			rd_pt = Real_Process_DH( data, 8 );     //第8个参数为地面航向
			if ( ( *rd_pt >= '0' ) && ( *rd_pt <= '9' ) )
			{
				for(t=0;t<7;t++)
				{
					if( *rd_pt == ',' )  break;
					GPS->Course[t] = *rd_pt;
					rd_pt++;
				}
				GPS->Course[t] = '\0';
				EMBARC_PRINTF("GPS->Course=%s\r\n",GPS->Course);
			}
			
			rd_pt = Real_Process_DH( data, 9 );     //第9个参数为UTC时间，日月年
			if ( ( *rd_pt >= '0' ) && ( *rd_pt <= '9' ) )
			{
				for(t=0;t<6;t++)
				{
					if( *rd_pt == ',' )  break;
					GPS->UTC_Date[t] = *rd_pt;
					rd_pt++;
			    }
				GPS->UTC_Date[t] = '\0';
				EMBARC_PRINTF("GPS->UTC_Date=%s\r\n",GPS->UTC_Date);
			}
		
		//if ( ( data[3] == 'G' ) && ( data[4] == 'A' ))       //$GPGGA
		 
			rd_pt = Real_Process_DH( data, 7 );      //第7个参数为正在使用的用于定位的卫星数量
			if ( ( *rd_pt >= '0' ) && ( *rd_pt <= '9' ) )
			{
				for(t=0;t<2;t++)
				{
					if( *rd_pt == ',' )  break;
					GPS->Use_EPH_Sum[t] = *rd_pt;
					rd_pt++;
			    }
				GPS->Use_EPH_Sum[t] = '\0';
				EMBARC_PRINTF("GPS->Use_EPH_Sum=%s\r\n",GPS->Use_EPH_Sum);
			}
			
			rd_pt = Real_Process_DH( data, 9 );        //第9个参数为海拔高度
			if ( ( *rd_pt >= '0' ) && ( *rd_pt <= '9' ) )
			{
				for(t=0;t<6;t++)
				{
					if( *rd_pt == ',' )  break;
					GPS->MSL_Altitude[t] = *rd_pt;
					rd_pt++;
			    }
				GPS->MSL_Altitude[t] = '\0';
				EMBARC_PRINTF("GPS->MSL_Altitude=%s\r\n",GPS->MSL_Altitude);
			}
			
		
    }
}



uint8_t Calc_GPS_Sum( const int8_t* Buffer )
{
    uint8_t i, j, k, sum;
    sum = 0;

    for ( i = 0; i < 100; i++ ) //i从1开始是闪过$开始符
    {
        if ( Buffer[i] != '*' ) //判断结束符
            sum ^= Buffer[i];   //GPS校验和算法为XOR
        else
            break;
    }
	
    j = Buffer[i + 1]; //取结束符后两位字符
    k = Buffer[i + 2]; //取结束符后两位字符

    if ( isalpha( j ) ) //判断字符是否为英文字母，为英文字母时返回非零值，否则返回零
    {
        if ( isupper( j ) ) //判断字符为大写英文字母时，返回非零值，否则返回零
            j -= 0x37;//强制转换为16进制
        else
            j -= 0x57;//强制转换为16进制
    }
    else
    {
        if ( ( j >= 0x30 ) && ( j <= 0x39 ) )
            j -= 0x30;//强制转换为16进制
    }

    if ( isalpha( k ) ) //判断字符是否为英文字母，为英文字母时返回非零值，否则返回零
    {
        if ( isupper( k ) ) //判断字符为大写英文字母时，返回非零值，否则返回零
            k -= 0x37;//强制转换为16进制
        else
            k -= 0x57;//强制转换为16进制
    }
    else
    {
        if ( ( k >= 0x30 ) && ( k <= 0x39 ) )
            k -= 0x30;//强制转换为16进制
    }

    j = ( j << 4 ) + k; //强制合并为16进制

    if ( sum == j )
        return 1; //校验和正常
    else
        return 0; //校验和错误
}

double resolve(char *buf)
{	
	double t;
	int  d;
	t=atof(buf);
	d=(int)(t/100);
	t=d+(t-d*100)/60;
	return t;
}


void sendGPS(uint8_t *phoneNumber, uint8_t *msg)
{	
	dev_iic2 = iic_get_dev(DW_IIC_0_ID	);
    dev_iic2->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	dev_iic2->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_ADDR3));
	//uint32_t baudrate = 115200;	
	//dev_uart0 = uart_get_dev(DW_UART_0_ID);
	//dev_uart0-> uart_open(baudrate);
	
	uint8_t i;
	uint8_t pnum[64];
	int8_t *cmd[5]={"AT+CMGF=1\r\n", "AT+CSCS=\"GSM\"\r\n",
				  "AT+CSCA?\r\n",  "AT+CSMP=17,167,0,240\r\n",
				   };
	snprintf(pnum, 64, "AT+CMGS=\"%s\"\r\n", phoneNumber);
	for(i = 0; i < 4; i++)
	{
		dev_iic2->iic_write(cmd[i], strlen(cmd[i]));
	//	printf("cmd\r\n");
		delay_ms(1000);
	}	
	dev_iic2->iic_write(pnum, strlen(pnum));
	//printf("pnum\r\n");
	delay_ms(1000);
	dev_iic2->iic_write(msg, strlen(msg));
	//printf("msg\r\n");
	delay_ms(1000);
	uint8_t end[]={0x1a,0x00};
	dev_iic2->iic_write(end, strlen(end));
	printf("GPS发送定位信息已经完成\r\n");
	
}

void send_msg(void)
{
	DEV_UART *dev_uart2 = NULL;
	uint8_t rcv_buf[10];
	int8_t data[100];
	uint32_t rcv_cnt;
	uint32_t k;
	uint32_t length;
	uint32_t baudrate = 9600;
	uint32_t rd_avail = 0;
	char  Longitude[]={"11540.5393"};
	char  Latitude[]={"3743.5044"};
	cpu_lock();
	board_init(); 
	cpu_unlock();
	
	dev_uart2 = uart_get_dev(DW_UART_2_ID);
	dev_uart2->uart_open(baudrate);

		k = 0;
		dev_uart2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
		if(rd_avail > 0)
		{
			rcv_cnt = dev_uart2->uart_read(rcv_buf, 1);
			rcv_buf[rcv_cnt] = '\0';
			if (rcv_buf[0]=='$')         //寻找帧命令起始位
			{
				while (1) 
				{
					dev_uart2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
					if(rd_avail > 0)
					{
						rcv_cnt = dev_uart2->uart_read(rcv_buf, 1);
                        
						rcv_buf[rcv_cnt] = '\0';
						//printf("rcv_buf[0]=%c\r\n",rcv_buf[0]);
						if ((rcv_cnt) && (rcv_buf[0] != '\r'))  //成功接收的字节数为1并且接收的字节不是帧结束标志(CR)
							data[k++]= rcv_buf[0];		        //将接收的字节放入数组中					
						
					    if(rcv_buf[0]=='\r')      //接收的字节是帧结束标志(CR)，则此帧结束
							break;
					}
				}
				data[k]='\0';
                if( Calc_GPS_Sum( data ) == 1 )
					//if( data[4] == 'C' )      //$GPRMC
				    if ( ( data[3] == 'G' ) && ( data[4] == 'A' ))       //$GPGGA
				{
					Creat_DH_Index( k,data );
					Real_GPS_Command_Process( k,data );
					//calculate_display();
					            double jingdu, weidu;
			                    char mymsg[32]="the location of user is:";
			                     //char lat_long[128];
			                      // if(GPS.Real_Locate==1)//==1
			                             {
											
			                                jingdu=resolve(GPS->Longitude);
				                            weidu=resolve(GPS->Latitude);
				                            snprintf(GPS->lat_long,100,"%s longitude:%lf latitude:%lf\r\n",mymsg, jingdu, weidu);
				                            EMBARC_PRINTF("lat_long=%s",GPS->lat_long);
				                            //GPS.Real_Locate==0;
				                            EMBARC_PRINTF("\nyouxiao\n ");//
			                             }
				}
			}  
		}
	
	sendGPS(phoneNumber,GPS->lat_long);
	
}

			
	
