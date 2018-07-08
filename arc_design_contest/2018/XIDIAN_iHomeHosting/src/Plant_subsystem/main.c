#include "embARC.h"
#include "embARC_debug.h"
#include <stdio.h>
#include "board.h"
#include "dev_iic.h"
#include "dev_gpio.h"

#include "../../../board/emsk/drivers/mux/mux.h"
#include "../../../board/emsk/drivers/mux/mux_hal.h"


#include "esp8266_gagent_common.h"
#include "esp8266_gagent_user.h"

#include "ad7991.h"
#include "DHT11.h"




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



/**************定义DHT11的接口和数据**************/
//DHT11的数据端 ---- J3-pin4
DEV_GPIO_PTR  dht11;
DHT11_DEF_PTR dht11_data;
#define DHT11_PORT DW_GPIO_PORT_C
#define DHT11_BIT  (19)
/**********DHT11初始化函数***************/
void DHT11_init(void)
{
	dht11 = gpio_get_dev(DHT11_PORT);
	dht11->gpio_open(GPIO_DIR_OUTPUT);
}

/******************定义AD的接口和数据*********************/
AD7991_DEF_PTR ad7991;
float ad7991_data[4];
#define AD7991_ID  (0)
/**********AD7991初始化函数***************/
void ad7991_init(void)
{
	ad7991->dev_id = AD7991_ID;
	ad7991->slvaddr = ADC_I2C_SLAVE_ADDRESS;
	ad7991_adc_init(ad7991);
}



/****************定义继电器的接口*********************/
//J4-pin4
DEV_GPIO_PTR relay;
#define RELAY_PORT DW_GPIO_PORT_C
#define RELAY_BIT (23)
#define RELAY_MASK (0x01<<RELAY_BIT)
/*************打开继电器***************************/
void relay_open(void)
{
	relay = gpio_get_dev(RELAY_PORT);
	relay->gpio_open(0x01 << RELAY_BIT);
	relay->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(RELAY_MASK));
	relay->gpio_write(0x01 << RELAY_BIT, RELAY_MASK);
}
/*************关闭继电器***************************/
void relay_close(void)
{
	relay = gpio_get_dev(RELAY_PORT);
	relay->gpio_open(0x01 << RELAY_BIT);
	relay->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(RELAY_MASK));
	relay->gpio_write(0x00 << RELAY_BIT, RELAY_MASK);	
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
//每10秒获取一次温湿度信息
int times=0;
void timer1_isr(void *ptr)
{	
	
	timer_int_clear(TIMER_1);
	
	
	DHT11_Read_Data(dht11, DHT11_BIT, dht11_data);
	Plant_Env_Temp = dht11_data->T_data;
	Plant_Env_Humidity =dht11_data->RH_data;
	EMBARC_PRINTF("Temperature is %d, Humidity is %d%%\r\n", dht11_data->T_data,dht11_data->RH_data);
	
	
	if(times % 2 == 0)
	{
		//每20秒获取一次时间；
		mcu2wifi_request_time(esp8266_uart);
	}
	if(times % 6 == 0)
	{
		//每60秒上报一次数据
		mcu2wifi_dev_report_status(esp8266_uart);		
	}
	times++;	
}
//植物生存维持子系统的产品标识码和产品密匙
uint8_t *product_key = "f2bbf37ccea74a18a08fbfcc2e03818b";
uint8_t *product_secret = "14380ffd6f2c44db87dd441d7d8dca8c";

int main(void)
{
	cpu_lock();	/* 锁定CPU以初始化定时器 */
	board_init();	/* 板级初始化 */	

	timer_stop(TIMER_1); /* Stop it first since it might be enabled before */
	int_handler_install(INTNO_TIMER1, timer1_isr);
	int_enable(INTNO_TIMER1);
	timer_start(TIMER_1, TIMER_CTRL_IE, 10 * BOARD_CPU_CLOCK);

	set_pmod_mux(
		PM1_UR_UART_0|PM1_LR_GPIO_A | 
		PM2_I2C_HRI | 
		PM3_GPIO_AC | 
		PM4_GPIO_AC |
		PM5_UR_GPIO_C | PM5_LR_GPIO_A |
		PM6_UR_GPIO_C | PM6_LR_GPIO_A
		);
	button_init();
	cpu_unlock();	/* 解锁CPU */

	esp8266_init(); /* ESP8266初始化 */
	DHT11_init();   /* DHT11初始化 */
	ad7991_init();  /* AD7991初始化*/	

	

	uint32_t rd_cnt = 0;
	uint8_t read_data[MAX_READ_CNT];
		
	led = gpio_get_dev(LED_PORT);
	led-> gpio_open(0x80);
	led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x80));
	uint8_t led_count = 0;
	

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
					case 0x01: mcu2wifi_product_info(esp8266_uart,product_key,product_secret);break;					
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

		ad7991_init();
		ad7991_data[0] = 0;
		ad7991_adc_read(ad7991, ad7991_data);	
		Plant_Soil_Moisture = (uint8_t)(176.47059 - 0.05348 * ad7991_data[0]);  
		if((led_count)%50 == 0 ) printf("Soil_Moisture Analog voltage is %f, send adta is %d\r\n",ad7991_data[0],  Plant_Soil_Moisture);		
		if(Plant_Watering)
		{
			if(Plant_Soil_Moisture < WaterThreshold)		
				relay_open();		
			else if(Plant_Soil_Moisture > WaterThreshold + 5)
				relay_close();
		}
		else  relay_close();
	}
	return E_SYS;
}