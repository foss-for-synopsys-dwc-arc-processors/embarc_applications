#include "embARC.h"
#include "embARC_debug.h"
#include <stdio.h>
#include "dev_gpio.h"
#include "dev_uart.h"
#include "dev_iic.h"
#include "dev_spi.h"

#include "../../../board/emsk/drivers/mux/mux.h"
#include "../../../board/emsk/drivers/mux/mux_hal.h"

#include "esp8266_gagent_common.h"
#include "esp8266_gagent_user.h"
#include "step_motor.h"


/**************定义esp8266的接口**************/
//串口
//esp8266的TX接口----J1-pin3
//esp8266的RX接口----J1-pin2
DEV_UART_PTR esp8266_uart;
#define ESP8266_UART_ID DW_UART_0_ID
#define ESP8266_UART_BAUDRATE UART_BAUDRATE_9600

/**********ESP8266串口初始化函数***************/
void esp8266_init(void) 
{
	// set pmod mux and uart map		
	set_uart_map(0xe4);
	esp8266_uart = uart_get_dev(ESP8266_UART_ID);
	esp8266_uart->uart_close();
	if(esp8266_uart->uart_open(ESP8266_UART_BAUDRATE) == E_OPNED)
		{
			esp8266_uart->uart_control(UART_CMD_SET_BAUD, (void *)(ESP8266_UART_BAUDRATE));
		}
}
/**********ESP8266串口接收函数***************/
uint32_t esp8266_read(uint8_t *buf, uint32_t cnt)
{
	if (esp8266_uart == NULL) return 0;

	uint32_t rd_avail;
	esp8266_uart->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
	cnt = (cnt > rd_avail)?rd_avail:cnt;
	if (cnt > 0) {
		return esp8266_uart->uart_read(buf, cnt);
	}
	return cnt;
}
/**********ESP8266串口发送函数***************/
uint32_t esp8266_write(uint8_t *buf, uint32_t cnt)
{
	if (esp8266_uart == NULL) return 0;

	return esp8266_uart->uart_write(buf, cnt);
}



/*********定义步进电机的接口、数据和函数********/
#define STEP_MOTER_PORT  DW_GPIO_PORT_C
#define STEP_MOTER_MASK  (0xF0000)



/*********定义水泵的接口、数据和函数********/
DEV_GPIO_PTR pump;
#define PUMP_PORT DW_GPIO_PORT_C
#define PUMP_MASK (0X800000)
#define PUMP_DIR  (0X800000)
/*********打开水泵*************************/
void pump_open(void)
{
	pump = gpio_get_dev(PUMP_PORT);
	pump->gpio_open(PUMP_DIR);
	pump->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(PUMP_MASK));
	pump->gpio_write(0xFFFFFFFF & PUMP_MASK,PUMP_MASK);
}
/*********关闭水泵*************************/
void pump_close(void)
{
	pump = gpio_get_dev(PUMP_PORT);
	pump->gpio_open(PUMP_DIR);
	pump->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(PUMP_MASK));
	pump->gpio_write(0x00000,PUMP_MASK);
}

/*********定义灯光的接口、数据和函数********/
//J5 - pin4
DEV_GPIO_PTR light;
#define LIGHT_PORT DW_GPIO_PORT_C
#define LIGHT_MASK (0X8000000)
#define LIGHT_DIR  (0X8000000)
/*********打开水泵*************************/
void lignt_open(void)
{
	light = gpio_get_dev(LIGHT_PORT);
	light->gpio_open(LIGHT_DIR);
	light->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(LIGHT_MASK));
	light->gpio_write(0xFFFFFFFF & LIGHT_MASK,LIGHT_MASK);
}
/*********关闭水泵*************************/
void light_close(void)
{
	light = gpio_get_dev(LIGHT_PORT);
	light->gpio_open(LIGHT_DIR);
	light->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(LIGHT_MASK));
	light->gpio_write(0x00000,LIGHT_MASK);
}


/****************定义按键的接口和函数*********************/
DEV_GPIO_PTR button;
#define BUTTON_MASK 0x07
void gpio_keyL_isr(void *ptr)
{
	EMBARC_PRINTF("key 1: L\r\n");
	mcu2wifi_request_into_binding(esp8266_uart);
}
void gpio_keyR_isr(void *ptr)
{
	EMBARC_PRINTF("key 2: R\r\n");
	mcu2wifi_reset_wifi(esp8266_uart);
}
void gpio_keyA_isr(void *ptr)
{
	EMBARC_PRINTF("key 3: A\r\n");
}
void button_init(void)
{
	DEV_GPIO_BIT_ISR bit_isr;
	DEV_GPIO_INT_CFG int_cfg;
	button = gpio_get_dev(DW_GPIO_PORT_A);
	button ->gpio_open(BUTTON_MASK);
	button ->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)BUTTON_MASK);
	int_cfg.int_bit_mask = BUTTON_MASK;
	int_cfg.int_bit_type = GPIO_INT_BITS_EDGE_TRIG(BUTTON_MASK);
	int_cfg.int_bit_polarity = GPIO_INT_BITS_POL_FALL_EDGE(BUTTON_MASK);
	int_cfg.int_bit_debounce = GPIO_INT_BITS_ENA_DEBOUNCE(BUTTON_MASK);
	button->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);
	bit_isr.int_bit_ofs = 0;
	bit_isr.int_bit_handler = gpio_keyL_isr;
	button->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&bit_isr));
	bit_isr.int_bit_ofs = 1;
	bit_isr.int_bit_handler = gpio_keyR_isr;
	button->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&bit_isr));
	bit_isr.int_bit_ofs = 2;
	bit_isr.int_bit_handler = gpio_keyA_isr;
	button->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&bit_isr));

	button->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)(0x7));
}


/**********定时器1 中断服务子程序***********/
void timer0_isr(void *ptr)
{
	//每10秒钟测量一次温度，1分钟上报一次设备状态
	timer_int_clear(TIMER_0);
	float temperatuer;
	ds18b20_port_ptr = gpio_get_dev(DS18B20_PORT);
	DS18b20_ReadTemperature(ds18b20_port_ptr,DS18B20_BIT,ds18b20_data);
	temperatuer = (ds18b20_data->temperature_int)+(ds18b20_data->temperature_decimal)*0.0625;
	if(ds18b20_data->temperature_sign) temperatuer = 0-temperatuer;
	Aquarius_Temp = (uint8_t)(temperatuer + 55);
	printf("temperatuer is :%f , send data Aquarius_Temp is %x \r\n",temperatuer,Aquarius_Temp );	
	static uint8_t report_time;
	if(report_time == 6)
	{
		report_time = 0;
		mcu2wifi_dev_report_status(esp8266_uart);
	}
	report_time++;
}

void timer1_isr(void *ptr)
{
	timer_int_clear(TIMER_1);	
	//水族箱投食频率，可写；0.一天一次 1.一天两次 2.一天三次 3.一天四次 4.一天五次 5.关闭自动投食
	switch(Aquarius_Feed_Freq)
	{
		case 0: 
			// if( hour == 7 && minute == 0 ) 
			if( hour == 7 && minute==0 )
				step_motor_active(0, Aquarius_Feed_Amount * 514,253);			
			break;
		case 1: 
			if( (hour == 7 && minute==0) ||(hour == 17 && minute==0) ) 
				step_motor_active(0,Aquarius_Feed_Amount*514,253);			
			break;
		case 2: 
			if( (hour == 7 && minute==0) ||(hour == 12 && minute==0)||(hour == 17 && minute==0) ) 
				step_motor_active(0,Aquarius_Feed_Amount*514,253);			
			break;
		case 3: 
			if( (hour == 7 && minute==0) || (hour == 10 && minute==30) || (hour == 14 && minute==30) || (hour == 17 && minute==0) ) 
				step_motor_active(0,Aquarius_Feed_Amount*514,253);			
			break;
		case 4: 
			//if( (hour == 7 && minute==0) || (hour == 9 && minute==30) || (hour == 12 && minute==00) || (hour == 14 && minute==30)  || (hour == 17 && minute==0) ) 
			if(minute % 2 ==0)
				step_motor_active(0,Aquarius_Feed_Amount*514,253);			
			break;
		case 5: 			
		default: break;
	}

	//获取网络时间，每分钟获取一次	
	mcu2wifi_request_time(esp8266_uart);
}

//水族箱子系统的产品标识码和产品密匙
uint8_t *product_key = "96dcc24b14ae4c69927cce0d3903fc54";
uint8_t *product_secret = "6c43c195b1174bdaa9bac97f2db90e90";
int main(void)
{
	cpu_lock();  /* 锁定CPU以初始化定时器 */	
	board_init();  /* 板级初始化 */		

	timer_stop(TIMER_0); /* Stop it first since it might be enabled before */
	int_handler_install(INTNO_TIMER0, timer0_isr);
	int_enable(INTNO_TIMER0);
	timer_start(TIMER_0, TIMER_CTRL_IE, 10*BOARD_CPU_CLOCK);

	timer_stop(TIMER_1); /* Stop it first since it might be enabled before */
	int_handler_install(INTNO_TIMER1, timer1_isr);
	int_enable(INTNO_TIMER1);
	timer_start(TIMER_1, TIMER_CTRL_IE, 60*BOARD_CPU_CLOCK);
	
	set_pmod_mux(
		PM1_UR_UART_0|PM1_LR_GPIO_A | 
		PM2_GPIO_AC | 
		PM3_GPIO_AC | 
		PM4_GPIO_AC |
		PM5_UR_GPIO_C | PM5_LR_GPIO_A |
		PM6_UR_GPIO_C | PM6_LR_GPIO_A
		);
	button_init();
	cpu_unlock();    /* 解锁CPU */

	esp8266_init();	  /* ESP8266初始化 */
	step_motor_init(STEP_MOTER_PORT, STEP_MOTER_MASK);  /* 步进电机初始化 */

	uint32_t rd_cnt = 0;
	uint8_t read_data[MAX_READ_CNT];
		
	led = gpio_get_dev(LED_PORT);
	led-> gpio_open(0x80);
	led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x80));
	uint8_t led_count = 0;
	
	//数据点初始化
	Aquarius_Feed_Amount = 1;
	Aquarius_Light = 0;
	while(1)
	{
		if((led_count)%10 == 0 ) led->gpio_write(0x80,0x80);
		if((led_count)%20 == 0 ) led->gpio_write(0x00,0x80);
		led_count++;

		board_delay_ms(100,0);
		rd_cnt = esp8266_read(read_data,sizeof(read_data));
		if(rd_cnt)
		{			
			for(uint8_t i=0;i<rd_cnt;i++) EMBARC_PRINTF("%2x ",read_data[i]);
			EMBARC_PRINTF("\r\n");
			//判断是否为合法数据包，合法数据包以固定包头0xFFFF开始。
			if(read_data[0]==0xFF && read_data[1]==0xFF) 
			{			
				sn = read_data[5];
				switch(read_data[4]) //判断命令
				{
					case 0x01: mcu2wifi_product_info(esp8266_uart, product_key, product_secret);break;					
					case 0x03: 
						switch(read_data[8])
						{
							case 0x01:mcu2wifi_wifi_ctrl_dev(esp8266_uart,read_data);break;
							case 0x02:mcu2wifi_wifi_read_dev(esp8266_uart);break;
							default : ;
						}
					case 0x06: break;
					case 0x07: mcu2wifi_heartbeat(esp8266_uart);break;
					case 0x0a: break;
					case 0x0c: break;
					case 0x0d: mcu2wifi_wifi_statu(esp8266_uart,read_data);break;
					case 0x11: EMBARC_PRINTF("Data package is not valid!!!\r\n");break;
					case 0x16: break;
					case 0x18: wifi2mcu_get_time(read_data);
					case 0x2a: break;
					default : EMBARC_PRINTF("Undefine commond or error!!!\r\n");break;
				}
			}
			else
			{
				mcu2wifi_receive_error(esp8266_uart);//串口接收数据错误，申请WiFi重新发送
			}
		}

		//水族箱增氧频率，可写；0.常开 1.间隔15分钟 2.间隔30分钟 3.间隔45分钟 4.间隔1小时 5.间隔两小时 6.常关
		switch(Aquarius_Oxy_Freq)
		{
			case 0: pump_open();break;
			case 1: 
				if(((hour*60+minute)/15)%2) pump_open();
				else pump_close();
				break;
			case 2: 
				if(((hour*60+minute)/30)%2) pump_open();
				else pump_close();
				break;
			case 3: 
				if(((hour*60+minute)/45)%2) pump_open();
				else pump_close();
				break;
			case 4: 
				if(((hour*60+minute)/60)%2) pump_open();
				else pump_close();
				break;
			case 5: 
				if(((hour*60+minute)/120)%2) pump_open();
				else pump_close();
				break;
			case 6: pump_close();break;
			default: break;
		}
		if(Aquarius_Light) lignt_open();
		else light_close();
	}
	return E_SYS;
}