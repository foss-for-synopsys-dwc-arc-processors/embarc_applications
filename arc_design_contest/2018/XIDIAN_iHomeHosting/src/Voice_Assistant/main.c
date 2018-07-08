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
/**************定义UART2及其函数**************/
//串口
//XFS5152的RX接口----J5-pin8 TXD
//LD3320 的TX接口----J5-pin9 RXD
DEV_UART_PTR uart2;
#define UART2_ID DW_UART_2_ID
#define UART2_BAUDRATE UART_BAUDRATE_9600

/**********UART2串口初始化函数***************/
void uart2_init(void) 
{

	uart2 = uart_get_dev(UART2_ID);
	uart2->uart_close();
	if(uart2->uart_open(UART2_BAUDRATE) == E_OPNED)
		{
			uart2->uart_control(UART_CMD_SET_BAUD, (void *)(UART2_BAUDRATE));
		}
}
/**********UART2串口接收函数***************/
uint32_t uart2_read(uint8_t *buf, uint32_t cnt)
{
	if (uart2 == NULL) return 0;

	uint32_t rd_avail;
	uart2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
	cnt = (cnt > rd_avail)?rd_avail:cnt;
	if (cnt > 0) {
		return uart2->uart_read(buf, cnt);
	}
	return cnt;
}
/**********UART2串口发送函数***************/
uint32_t uart2_write(uint8_t *buf, uint32_t cnt)
{
	if (uart2 == NULL) return 0;

	return uart2->uart_write(buf, cnt);
}


// //忙等待函数
// DEV_GPIO_PTR busy;
// #define BUSY_PORT DW_GPIO_PORT_C
// #define BUSY_MASK 0x01000000

// void busy_wait(void)
// {
// 	uint32_t read_data;
// 	busy = gpio_get_dev(BUSY_PORT);
// 	busy->gpio_open(BUSY_MASK);
// 	busy->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)BUSY_MASK);
// 	busy->gpio_read(&read_data, BUSY_MASK);
// 	while(read_data)
// 	{
// 		busy->gpio_read(&read_data, BUSY_MASK);
// 		printf("busy!\r\n");
// 	}
// }




/**********定时器1 中断服务子程序***********/
void timer0_isr(void *ptr)
{
	//获取网络时间 每30秒一次
	timer_int_clear(TIMER_0);
	mcu2wifi_request_time(esp8266_uart);
}

uint8_t huanxin[] = {0xFD, 0x00, 0x01, 0xFF};
uint8_t shuimian[] = {0xFD, 0x00, 0x01, 0x88};
uint8_t wozai[] = {0xFD, 0x00, 0x06, 0x01, 0x01, 0xCE, 0xD2, 0xD4, 0xDA};
uint8_t kaideng[] = {0xFD,0x00, 0x0A, 0x01, 0x01, 0xB5, 0xC6, 0xD2, 0xD1, 0xB4, 0xF2, 0xBF, 0xAA};
uint8_t guandeng[] = {0xFD, 0x00, 0x0A, 0x01, 0x01, 0xB5, 0xC6, 0xD2, 0xD1, 0xB9, 0xD8, 0xB1, 0xD5};
uint8_t dklijiamoshi[] = {0xFD, 0x00, 0x10, 0x01, 0x01, 0xC0, 0xEB, 0xBC, 0xD2, 0xC4, 0xA3, 0xCA, 0xBD, 0xD2, 0xD1, 0xB4, 0xF2, 0xBF, 0xAA};
uint8_t dkyugangdeng[] = {0xFD,0x00,0x12,0x01,0x01,0xD3,0xE3,0xB8,0xD7,0xD5,0xD5,0xC3,0xF7,0xB5,0xC6,0xD2,0xD1,0xB4,0xF2,0xBF,0xAA};
uint8_t gbyugangdeng[] = {0xFD,0x00,0x12,0x01,0x01,0xD3,0xE3,0xB8,0xD7,0xD5,0xD5,0xC3,0xF7,0xB5,0xC6,0xD2,0xD1,0xB9,0xD8,0xB1,0xD5};
uint8_t riqi[] = {0xFD, 0x00, 0x12, 0x01, 0x01, 0xBD, 0xF1, 0xCC, 0xEC, 0xCA, 0xC7, 0x32, 0x30, 0x31, 0x38, 0x2D, 0x30, 0x37, 0x2D, 0x30, 0x33};
uint8_t shijian[] = {0xFD, 0x00, 0x0D, 0x01, 0x01, 0xCF, 0xD6, 0xD4, 0xDA, 0xCA, 0xC7, 0x31, 0x33, 0x3A, 0x34, 0x39};
uint8_t dkfengshan[] = {0xFD, 0x00, 0x0E, 0x01, 0x01, 0xC5, 0xC5, 0xB7, 0xE7, 0xC9, 0xC8, 0xD2, 0xD1, 0xB4, 0xF2, 0xBF, 0xAA};
uint8_t gbfengshan[] = {0xFD, 0x00, 0x0E, 0x01, 0x01, 0xC5, 0xC5, 0xB7, 0xE7, 0xC9, 0xC8, 0xD2, 0xD1, 0xB9, 0xD8, 0xB1, 0xD5};
uint8_t nansheng[] = {0xFD,0x00,0x07,0x01,0x01,0x5B,0x6D,0x35,0x31,0x5D};

//语音助手的产品标识码和产品密匙
uint8_t *product_key = "dfc5e956f7724cb4a050c892dbef0748";
uint8_t *product_secret = "bb67c47127244067a3392b5deff3e651";
int main(void)
{
	cpu_lock();  /* 锁定CPU以初始化定时器 */	
	board_init();  /* 板级初始化 */		

	timer_stop(TIMER_0); /* Stop it first since it might be enabled before */
	int_handler_install(INTNO_TIMER0, timer0_isr);
	int_enable(INTNO_TIMER0);
	timer_start(TIMER_0, TIMER_CTRL_IE, 10*BOARD_CPU_CLOCK);	
	
	set_pmod_mux(
		PM1_UR_UART_0|PM1_LR_GPIO_A | 
		PM2_GPIO_AC | 
		PM3_GPIO_AC | 
		PM4_GPIO_AC |
		PM5_UR_GPIO_C | PM5_LR_SPI_M2 |
		PM6_UR_GPIO_C | PM6_LR_GPIO_A
		);
	button_init();
	cpu_unlock();    /* 解锁CPU */

	esp8266_init();	  /* ESP8266初始化 */
	uart2_init();

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
		if(LeaveModel_switch)
		{
			LeaveModel_switch = 0;
			mcu2wifi_dev_report_status(esp8266_uart);
		}
		rd_cnt = uart2_read(read_data,1);
		if(rd_cnt)
		{
			switch(read_data[0])
			{
				case 0x01 : 
						{
							// uart2_write(huanxin, sizeof(huanxin));							
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(wozai, sizeof(wozai));
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				case 0x02 : 
						{
							Lamp_switch = 1;
							mcu2wifi_dev_report_status(esp8266_uart);
							// uart2_write(huanxin, sizeof(huanxin));
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(kaideng, sizeof(kaideng));
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				case 0x03 : 
						{
							Lamp_switch = 0;
							mcu2wifi_dev_report_status(esp8266_uart);
							// uart2_write(huanxin, sizeof(huanxin));
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(guandeng, sizeof(guandeng));
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				case 0x04 : 
						{
							LeaveModel_switch = 1;
							mcu2wifi_dev_report_status(esp8266_uart);
							// uart2_write(huanxin, sizeof(huanxin));
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(dklijiamoshi, sizeof(dklijiamoshi));							
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				case 0x05 : 
						{
							AquariusLight_switch = 1;
							mcu2wifi_dev_report_status(esp8266_uart);
							// uart2_write(huanxin, sizeof(huanxin));
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(dkyugangdeng, sizeof(dkyugangdeng));
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				case 0x06 : 
						{
							AquariusLight_switch = 0;
							mcu2wifi_dev_report_status(esp8266_uart);
							// uart2_write(huanxin, sizeof(huanxin));
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(gbyugangdeng, sizeof(gbyugangdeng));
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				case 0x07 : 
						{
							riqi[11] = (year/1000)%10 + 0x30;
							riqi[12] = (year/100) %10 + 0x30;
							riqi[13] = (year/10)  %10 + 0x30;
							riqi[14] = year       %10 + 0x30;
							riqi[16] = (month/10) %10 + 0x30;
							riqi[17] = month      %10 + 0x30;
							riqi[19] = (day/10)   %10 + 0x30;
							riqi[20] = (day)      %10 + 0x30;
							// uart2_write(huanxin, sizeof(huanxin));
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(riqi, sizeof(riqi));
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				case 0x08 : 
						{							
							shijian[11] = (hour/10) %10 + 0x30;
							shijian[12] = hour      %10 + 0x30;
							shijian[14] = (minute/10)   %10 + 0x30;
							shijian[15] = (minute)      %10 + 0x30;
							// uart2_write(huanxin, sizeof(huanxin));
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(shijian, sizeof(shijian));
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				case 0x09 : 
						{
							Fan_switch = 1;
							mcu2wifi_dev_report_status(esp8266_uart);
							// uart2_write(huanxin, sizeof(huanxin));
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(dkfengshan, sizeof(dkfengshan));
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				case 0x0A : 
						{
							Fan_switch = 0;
							mcu2wifi_dev_report_status(esp8266_uart);
							// uart2_write(huanxin, sizeof(huanxin));
							uart2_write(nansheng, sizeof(nansheng));
							uart2_write(gbfengshan, sizeof(gbfengshan));
							// busy_wait();
							// uart2_write(shuimian, sizeof(shuimian));
							break;
						}
				default :  break;
			}
			
		}
		
	}
	return E_SYS;
}