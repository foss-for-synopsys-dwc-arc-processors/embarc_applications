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
#include "u8g.h" 
#include "dev_uart.h"

#define BOARD_TEMP_I2C_SLAVE_ADDRESS  TEMP_I2C_SLAVE_ADDRESS

static DEV_GPIO_PTR port_yuyin;
static DEV_GPIO_PTR port_move_detect;
static DEV_GPIO_PTR port_infrared;
static DEV_GPIO_PTR port_call;

static uint16_t CH2O_Value;
static int32_t  TEMP_Value;
static uint32_t time_value;

static uint32_t count;
static uint32_t a;
static uint32_t start_measure_CH2O;


uint8_t *phoneNum  ="00310038003700310030003800390035003300390033"; //18710895393 
uint8_t *msg_child ="30108D1D62A45B9D30118BF74E0D89815C06513F7AE56EDE75598F665185FF0C4EE5514D53D1751F53719669FF01"; //请不要将儿童滞留车内
uint8_t *msg_temp  ="30108D1D62A45B9D30118F6651856E295EA68FC79AD8FF0C8BF75C3D5FEB8FD456DEFF01";//车内温度过高

u8g_t u8g;
 
static void delay_ms(volatile int z) 
{
	volatile uint32_t x,y;
	for(x=1400;x>0;x--)
       for(y=z;y>0;y--);
}

static void timer0_isr(void *ptr)
{
	timer_int_clear(TIMER_0);
	
	if(start_measure_CH2O==1)
	{
       CH2O_Value = _CH2O_();		
	}	
	TEMP_Value=temp_sensor_read();	
}

static void timer1_isr(void *ptr)
{
	uint32_t move_signal;
	uint32_t infrared_signal;
	
	timer_int_clear(TIMER_1);	
	port_move_detect = gpio_get_dev(DW_GPIO_PORT_C);
	port_move_detect-> gpio_open(0xf0000000);
	port_move_detect-> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0xf0000000);
	port_move_detect-> gpio_read(&move_signal, 0xf0000000);
	move_signal=move_signal>>28;
	
	port_infrared = gpio_get_dev(DW_GPIO_PORT_A);
	port_infrared-> gpio_open(0x80000000);
	port_infrared-> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x80000000);
	port_infrared-> gpio_read(&infrared_signal, 0x80000000);	
	infrared_signal=infrared_signal>>28;
	
    if((move_signal==0x5)&&(infrared_signal==0x8)||(move_signal==0x5))
    {
		count=count+1;
        open_led5();
        if(TEMP_Value>290)	
		{
			send(phoneNum, msg_temp);			
		}			
	}
	if(count>=5)
	{
		time_value=time_value+1;		
	}
	if(time_value==10)
	{
		send(phoneNum, msg_child);   //检测到儿童10秒后发送短信
		open_led6();		
	}
	if(time_value==30)
	{
		call_gh();                      //检测到儿童30秒后拨打电话
		open_led7();
	}
	if(time_value>60)                     //60秒后启动求救系统
	{
		if(a<=10)
		{
			help();                      //鸣笛和灯光闪烁进行报警，实际会一直向，此处响10次
            a=a+1;	
		}
	}	
	if(time_value == 3600)
	{
		count=0; 		//一小时后，恢复。
        a=0;
        time_value	=0;	
	}    
	
}

void u8g_prepare(void) {
	u8g_SetFont(&u8g, u8g_font_unifont);		/* set the current font and reset the font reference position to "Baseline" */
	u8g_SetFontRefHeightExtendedText(&u8g);		/* define the calculation method for the ascent and descent of the current font */
	u8g_SetDefaultForegroundColor(&u8g);		/* assign one of the default colors as current color index */
	u8g_SetFontPosTop(&u8g);			       /* set the reference position for the character and string draw procedure */
}
static void draw(void) {	

	u8g_prepare();
	char s1[]="TEMP:  C";	
	s1[5]=(TEMP_Value/100)+48;
	s1[6]=((TEMP_Value/10)%10)+48;				
  	u8g_DrawStr(&u8g, 0, 20, s1);
	
    char s2[]="CH2O: .  mg/m3";		
	s2[5]=(CH2O_Value/100)+48;
	s2[7]=((CH2O_Value/10)%10)+48;	
	s2[8]=(CH2O_Value%10)+48;			
  	u8g_DrawStr(&u8g, 0, 40, s2);
}

//******************************************************************************* 
//***********************************主函数**************************************
//*******************************************************************************

int main(void)
{   
	cpu_lock();
	board_init(); 
	cpu_unlock();
	
	int32_t ercd_CH2O;
	int32_t ercd_TEMP;

	uint32_t rd_avail = 0;
	uint32_t rcv_cnt = 0;	
	uint8_t i;
			
	uint32_t status;
	uint32_t call_signal;
	uint32_t switch_signal;
	
	uint16_t mile;
	led_initial();

//******************************************IIC初始化*****************************
    ercd_CH2O = CH2O_IIC_init(CH2O_ADDRESS);
	if (ercd_CH2O != E_OK) {
		EMBARC_PRINTF("CH2O initial failed\r\n");}	
		
	ercd_TEMP = temp_sensor_init(BOARD_TEMP_I2C_SLAVE_ADDRESS);
	if (ercd_TEMP != E_OK) {
		EMBARC_PRINTF("Temperature sensor open failed\r\n");}	
//******************************************定时器0，读取环境信息*****************
	if (timer_present(TIMER_0)) 
	{
		timer_stop(TIMER_0); 
		int_handler_install(INTNO_TIMER0, timer0_isr);
		int_enable(INTNO_TIMER0);
	}
	if (timer_present(TIMER_0)) 
	{
		timer_start(TIMER_0, TIMER_CTRL_IE, 20*BOARD_CPU_CLOCK);	
	}
//******************************************定时器1，儿童检测*********************	
	
	if (timer_present(TIMER_1)) 
	{
		timer_stop(TIMER_1); 
		int_handler_install(INTNO_TIMER1, timer1_isr);
		int_enable(INTNO_TIMER1);
	}		
	if (timer_present(TIMER_1)) 
	{
		timer_start(TIMER_1, TIMER_CTRL_IE, 1*BOARD_CPU_CLOCK);
	}
//******************************************OLED初始化*****************************
    u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, U8G_COM_SSD_I2C); 
 	u8g_prepare();
    u8g_Begin(&u8g); 		
	
	while(1)
	{
	   u8g_FirstPage(&u8g);
		do {
			u8g_prepare();
			draw();			
			} while (u8g_NextPage(&u8g));	

			
//******************************************语音控制*****************************
	port_yuyin = gpio_get_dev(DW_GPIO_PORT_A);
	port_yuyin-> gpio_open(0xf0000);
	port_yuyin-> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0xf0000);
	port_yuyin-> gpio_read(&status, 0xf0000);
	status=status>>16;	
    if(status==0x1)
    {
		Txt2YY_command1();
		delay_ms(1000);	
	}
    if(status==0x2)
    {
		open_conditioner();
		Txt2YY_command2();
		delay_ms(1000);			
	}    
    if(status==0x4)
    {
		close_conditioner();
		Txt2YY_command3();
		delay_ms(1000);	
	}
    if(status==0x8)
    {
		Txt2YY_command4();
		delay_ms(1000);	
	}
	
	if(status==0x7)
    {
		 Txt2YY_temp(TEMP_Value/10);
		 delay_ms(1000); 
	}

	if(status==0x3)
	{
		open_child_detect();
		start_measure_CH2O=0;
        car_stop_led();	
		
	}
	
	if(status==0x0)
	{
		close_child_detect();          
		start_measure_CH2O=1;	
        car_run_led();			
        count=0; 		
        a=0;
        time_value=0;		
         	
	}	
	
//******************************************一键呼救*****************************	
	port_call = gpio_get_dev(DW_GPIO_PORT_A);
	port_call-> gpio_open(0x10000000);
	port_call-> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x10000000);
	port_call-> gpio_read(&call_signal, 0x10000000);		
	call_signal=call_signal>>28;
    if(call_signal==0x1)	
	{
		call_gh();                     		
	} 

	}					
	return E_SYS;		

}
