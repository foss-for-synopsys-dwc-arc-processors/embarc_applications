#include "rtc.h"


DEV_IIC *IIC_port;

uint8_t point= (RTC_ADDR_IIC >> 1);

_RTC rtc;

void pcf8563_iic_init()
{
   IIC_port = iic_get_dev(DW_IIC_0_ID);

    if (IIC_port -> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD) != E_OK) 
    {
       EMBARC_PRINTF("error!!!");
    }
    if (IIC_port -> iic_control(IIC_CMD_ENA_DEV, (void *)point)!=E_OK)
    {
      EMBARC_PRINTF("error!!!");
    }
    if (IIC_port -> iic_control(IIC_CMD_MST_SET_TAR_ADDR,(void *)point) != E_OK)
    {
      EMBARC_PRINTF("error!!!");
    }
}
/*Write one byte date to RTC.
 *Befor write dataake the slave device address point to pcf8653.*/
void pcf8653_write_data(uint8_t address,uint8_t time_data)
{
  uint8_t send_data[2];
  send_data[0] = address;
  send_data[1] = time_data;

  if (IIC_port -> iic_control( IIC_CMD_MST_SET_NEXT_COND,(void*)(IIC_MODE_STOP)) !=E_OK)
  {
    EMBARC_PRINTF("error!!!\n");
  }

  EMBARC_PRINTF("return:%d\n",IIC_port->iic_write((void*)&send_data,2));
}
/*Converts data from decimal to BCD.*/
uint8_t he2bcd(uint8_t num)
{
  uint8_t one,ten;

  one = num%10;
  ten = (num-one)/10;
  return num = ((ten&0x0f)<<4)|one;
}
/*Reset RTC data!
 *parameter:The date that you want to set.
*/

// void pcf8653_time_reset(uint8_t week,uint8_t hour,uint8_t minute,uint8_t sec)
// {
//   /*Turn the hour, minute, and second data into BCD*/
//   hour = he2bcd(hour);
//   minute = he2bcd(minute);
//   sec = he2bcd(sec);
//   Send all those date to RTC's register.
//   pcf8653_write_data(RTC_ADDR_WEEK,week);
//   pcf8653_write_data(RTC_ADDR_HOUR,hour);
//   pcf8653_write_data(RTC_ADDR_MIN,minute);
//   pcf8653_write_data(RTC_ADDR_SEC,sec);
// }
/*Read RTC date and deal it.
 *parameter: address,the address of the first register.
 *count:the num witch you want to read,there is 5 and the fouth is date which we will not use.
 *buff:the point of variable that you want to storage date.
 */
void pcf8563_data_read(uint8_t address,uint8_t count,uint8_t *buff) 
{
  if (IIC_port -> iic_control( IIC_CMD_MST_SET_NEXT_COND,(void*)(IIC_MODE_RESTART)) !=E_OK)
  {
    EMBARC_PRINTF("error!!!\n");
  }
  IIC_port->iic_write((void*)(&address),1);

  if (IIC_port -> iic_control( IIC_CMD_MST_SET_NEXT_COND,(void*)(IIC_MODE_STOP)) !=E_OK)
  {
    EMBARC_PRINTF("error!!!");
  }

  IIC_port->iic_read((void*)(buff), count);

}  
/*Converts data from BCD to decimal.*/
void pcf8563_bcd2de()
{
   uint8_t one,ten,hundred,tousand;

   /*hour*/
   one=rtc.hour&0x0f;
   ten=(rtc.hour>>4)&0x03;
   rtc.hour=one+ten*10;
   /*min*/
   one=rtc.min&0x0f;
   ten=(rtc.min>>4)&0x07;
   rtc.min=one+ten*10;
   /*second*/
   one=rtc.sec&0x0f;
   ten=(rtc.sec>>4)&0x07;
   rtc.sec=one+ten*10;
}

void iic0_point_rtc(void)
{
    if (IIC_port -> iic_control(IIC_CMD_ENA_DEV, (void *)point)!=E_OK)
    {
      EMBARC_PRINTF("error!!!");
    }
    if (IIC_port -> iic_control(IIC_CMD_MST_SET_TAR_ADDR,(void *)point) != E_OK)
    {
      EMBARC_PRINTF("error!!!");
    }
}

/*Updata the time data.*/
uint8_t pcf8563_date_updata(uint8_t clock_day,uint8_t clock_hour,uint8_t clock_min)
{   
  uint8_t time[5];
  iic0_point_rtc();
  pcf8563_data_read(0x02,0x05,time);

  /*If the num of second is zero,update the date again!*/
  if (rtc.sec == 0)
  {
    pcf8563_data_read(0x02,0x05,time);
  }

  /*Process the data which read from the register.*/
  /*The bits from 0 to 6 are effective.*/
  rtc.sec = *(time+0) & 0x7f;  /*秒 */
  /*The bits from 0 to 6 are effective.*/
  rtc.min = *(time+1) & 0x7f;  /*分 */
  /*The bits from 0 to 5 are effective.*/
  rtc.hour = *(time+2) & 0x3f; /*时 */
  /*The bits from 0 to 3 are effective.*/
  rtc.day = *(time+4) & 0x07;  /*周*/

  pcf8563_bcd2de();
  EMBARC_PRINTF("Second:%d.   Min:%d.     hour:%d.       day:%d.\r\n",rtc.sec,rtc.min,rtc.hour,rtc.day);
  //if(rtc.day==clock_day)
  //{
    if(rtc.hour==clock_hour)
    {
      if(rtc.min==clock_min)
      {
        return CLOCK_START;
      }
    }
    return 0;
  }
 // return 0;
//}


void P8563_settime()
{
  pcf8653_write_data( 8,rtc.year ); //Äê
  pcf8653_write_data( 7,rtc.month ); //ÔÂ
  pcf8653_write_data( 5,rtc.date ); //ÈÕ
  pcf8653_write_data( 4,rtc.hour ); //Ê± 
  pcf8653_write_data( 3,rtc.min ); //·Ö  
  pcf8653_write_data( 2,rtc.sec ); //Ãë 
}

void P8563_init()
{
  rtc.year = 0x17;
  rtc.month = 0x06;
  rtc.date = 0x27;
  rtc.day = 0x02;
  rtc.hour = 0x13;
  rtc.min = 0x13;
  rtc.sec = 0x30;
  
  iic0_point_rtc();
  P8563_settime();  
}

DEV_GPIO_PTR port_gpio_rtc;
DW_GPIO_PORT_PTR port_rtc;
DEV_GPIO_INFO_PTR port_info_ptr_rtc;

DEV_GPIO_INT_CFG_PTR port_int_ptr_rtc;
DEV_GPIO_BIT_ISR_PTR port_isr_ptr_rtc;


//¹Ø±Õµ¹¼ÆÊ±£¬½µµÍ¹¦ºÄ
void cnt_stop()
{
  iic0_point_rtc();
  pcf8653_write_data( 0x0e,0x03 ); //0000_0011  //1/60hz
  pcf8653_write_data( 0x01,0x13 );//0001_0011
}

//倒计时初始化
void cnt_start(uint8_t time)
{
  //1s
  pcf8653_write_data( 0x0e,0x02 ); //0000_0010  //1hz 
  //1min
  //writeData( 0x0e,0x03 ); //0000_0010  //1/60hz 
  pcf8653_write_data( 0x01,0x01 );  //0001_0001   //0001_0011
  pcf8653_write_data( 0x0f,time );
  pcf8653_write_data( 0x0e,0x82 ); //1000_0010  //1hz               
}

void check_cnt()
{
  uint8_t read;
  pcf8563_data_read(0x01,1,&read);
  //check flag
  if ( read & 0x04 ) //0000_0100
  {
    //times up
    
    //clear_flag
     pcf8653_write_data( 0x01,0x11 );  //0001_0000   //0001_0011
    //cnt_start(10);
  }
  
}





