
#include "embARC.h"
#include "embARC_debug.h"
#include "dev_gpio.h"
#include "dev_uart.h"
#include "../../../board/emsk/drivers/mux/mux.h"
#include "../../../board/emsk/drivers/mux/mux_hal.h"


static DEV_GPIO_PTR esp8266_rst;

#define ESP8266_RST_PORT				DW_GPIO_PORT_A
#define ESP8266_RST_MASK				(0x100)
#define ESP8266_RST_DIR 				(0x100)

static DEV_GPIO_PTR led;

#define LED_PORT								DW_GPIO_PORT_B
#define LED_MASK								0x1ff
#define LED_DIR 								0x1ff

static DEV_UART_PTR esp8266_uart;

#define ESP8266_UART_ID 				DW_UART_0_ID
#define ESP8266_UART_BAUDRATE 	UART_BAUDRATE_9600

#define MAX_READ_CNT						32

#include "./esp8266_gagent/esp8266_gagent_common.c"
#include "./esp8266_gagent/esp8266_gagent_user.c"
#include "./esp8266_gagent/ledrgb.c"
#include "./esp8266_gagent/setupAP4.c"


static void delay_ms (int z) //1ms
{
	uint32_t volatile x, y;

	for (x = 1400; x > 0; x--)
		for (y = z; y > 0; y--)
			;
}


void esp8266_init (void)
{
	// set pmod mux and uart map
	set_uart_map (0xe4);
	set_pmod_mux (PM1_UR_UART_0 | PM1_LR_GPIO_A);

	// Get devices structure pointer
	esp8266_uart				= uart_get_dev (ESP8266_UART_ID);
	esp8266_rst 				= gpio_get_dev (ESP8266_RST_PORT);
	int32_t 				ercd;

	// Open devices
	esp8266_rst->gpio_close ();

	if ((ercd = esp8266_rst->gpio_open (ESP8266_RST_DIR)) == E_OK)
		{
			EMBARC_PRINTF ("ESP8266_RST_PORT open successfully!!\r\n");
		}
	else if (ercd == E_OPNED)
		{
			EMBARC_PRINTF ("ESP8266_RST_PORT open failed,because opend with different direction!!\r\n");
			esp8266_rst->gpio_control (GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *) (ESP8266_RST_MASK));

		}
	else 
		{
			EMBARC_PRINTF ("ESP8266_RST_PORT open failed, unknow why!!\r\n");
		}

	esp8266_uart->uart_close ();

	if (esp8266_uart->uart_open (ESP8266_UART_BAUDRATE) != E_OK)
		{
			EMBARC_PRINTF ("ESP8266_UART open failed!!\r\n");
		}
	else 
		{
			EMBARC_PRINTF ("ESP8266_UART open successfully!!\r\n");
		}

}


int main (void)
{
	cpu_lock ();

	//Board initialization;
	board_init ();

	//esp8266 initialization;
	esp8266_init ();
	cpu_unlock ();
	uint8_t 				StaNum = 0;
	uint8_t 				Crowd = 0;
	uint16_t				rd_cnt = 0;
	uint8_t 				read_data[MAX_READ_CNT];
	uint32_t				rd_avail;

	setupAP4 ();

	while (1) //ARC板子读取wifi发送的数据函数
		{
			esp8266_uart->uart_control (UART_CMD_GET_RXAVAIL, (void *) (&rd_avail));
			rd_cnt							= (MAX_READ_CNT > rd_avail) ? rd_avail: MAX_READ_CNT;
			board_delay_ms (100, 0);

			if (rd_avail > 0)
				{
					esp8266_uart->uart_read ((void *) read_data, rd_cnt);

					for (uint8_t i = 0; i < rd_cnt; i++)
						EMBARC_PRINTF ("%2x ", read_data[i]);

					EMBARC_PRINTF ("\r\n");

					//判断是否为合法数据包，合法数据包以固定包头0xFFFF开始。
					if (read_data[0] == 0xFF && read_data[1] == 0xFF)
						{
							sn									= read_data[5];

							switch (read_data[4]) //读取命令位的代码
								{
									case 0x01:
										mcu2wifi_product_info ();
										break; //MCU传入产品的Key与Secret

									case 0x03: //Wifi控制设备，MCU读入改变的数据点
											{
												if (read_data[9] == 01)
													{
														StaNum							= read_data[10];
													}
												else if (read_data[9] == 02)
													{
														Crowd 							= read_data[11];
													}

												//EMBARC_PRINTF("SN=%d,CR=%d\r\n",StaNum,Crowd);	//临时显示debug						
												LEDRGB (StaNum, Crowd); 		//云端数据改变对应引起指示灯的变化
												mcu2wifi_wifi_ctrl_dev (read_data); //MCU回复wifi的控制指令
												break;
											}

									case 0x06:
										break;

									case 0x07:
										mcu2wifi_heartbeat ();
										break; //MCU回复心跳包

									case 0x0a:
										break;

									case 0x0c:
										break;

									case 0x0d:
										mcu2wifi_wifi_statu (read_data);
										break; //wifi将最新的工作状态推送至MCU

									default:
										EMBARC_PRINTF ("Undefine commond or error!!!\r\n");
										break;
								}
						}

				}
		}

	return E_SYS;
}

