
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

static DEV_UART_PTR esp8266_uart;

#define ESP8266_UART_ID 				DW_UART_0_ID
#define ESP8266_UART_BAUDRATE 	UART_BAUDRATE_9600

#define MAX_READ_CNT						32

#include "./esp8266_gagent/esp8266_gagent_common.c"
#include "./esp8266_gagent/esp8266_gagent_user.c"
#include "./esp8266_gagent/Intrainout.c"
#include "./esp8266_gagent/lookforrssi.c"


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

	//esp8266_uart->uart_control(UART_CMD_SET_RXCB,esp8266_uart_rxcb);
	//esp8266_uart->uart_control(UART_CMD_SET_RXINT,CONV2VOID(1));
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
	uint8_t 				gjm = 0;
	uint8_t 				i 	= 0;
	uint8_t 				j 	= 0;
	uint8_t 				Crowd = 0;
	uint32_t				crowd = 0;
	uint16_t				rd_cnt = 0;
	uint8_t 				read_data[MAX_READ_CNT];
	uint32_t				rd_avail;

	while (1) //机智云设备上线初始化
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
											{
												mcu2wifi_product_info ();
												break;											//MCU传入产品的Key与Secret
											}

									case 0x03: //Wifi控制设备，MCU读入改变的数据点
											{
												mcu2wifi_wifi_ctrl_dev (read_data); //MCU回复wifi的控制指令
												j++;
												break;
											}

									case 0x06:
										break;

									case 0x07:
											{
												mcu2wifi_heartbeat ();			//MCU回复心跳包
												j++;
												break;
											}

									case 0x0a:
										break;

									case 0x0c:
										break;

									case 0x0d:
											{
												mcu2wifi_wifi_statu (read_data);
												i++;
												break;											//wifi将最新的工作状态推送至MCU
											}

									default:
										EMBARC_PRINTF ("Undefine commond or error!!!\r\n");
										break;
								}
						}

				}

			if (i == 4 || j == 1)
				break;
		}


	while (gjm != 1)
		{
			gjm 								= lookforrssi1 ();				//当rssi满足条件，返回gjm=1，跳出循环，进行下一步对于机智云的操作
			StaNum							= 1;											//得到此时的公交所在站点，衡量水平为StaNum，此时值为1。	
		}

	crowd 							= ct2crowd ();								//通过红外脉冲模块计算车载的拥挤程度，衡量水平为crowd

	while (1) //ARC板子读取wifi发送的数据函数,MCU完成回复，必不可少。
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
												mcu2wifi_wifi_ctrl_dev (read_data); //MCU回复wifi的控制指令
												break;
											}

									case 0x06:
										break;

									case 0x07:
											{
												mcu2wifi_heartbeat ();			//MCU回复心跳包
												break;
											}

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

			//crowd=ct2crowd();	//通过红外脉冲模块计算车载的拥挤程度，衡量水平为crowd
			break;																				//得到此时的StaNum与crowd，跳出while循环
		}

	EMBARC_PRINTF ("StaNum=%d,crowd=%d\r\n", StaNum, crowd);
	mcu2wifi_dev_report_status (StaNum, crowd); 			//将数据传入云端
	delay_ms (10000);

	/*————————————————————————第一个公交站牌的数据上传由小
	车完成，第二个，第四个站牌由云端读入改变数据，小灯发生
	相应改变。——————————*/
	while (gjm != 2)
		{
			gjm 								= lookforrssi2 ();				//当rssi满足条件，返回gjm=2，跳出循环，进行下一步对于机智云的操作
			StaNum							= 2;											//得到此时的公交所在站点，衡量水平为StaNum，此时值为2。	
		}

	crowd 							= ct2crowd ();								//通过红外脉冲模块计算车载的拥挤程度，衡量水平为crowd

	while (1) //ARC板子读取wifi发送的数据函数,MCU完成回复，必不可少。
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
												mcu2wifi_wifi_ctrl_dev (read_data); //MCU回复wifi的控制指令
												break;
											}

									case 0x06:
										break;

									case 0x07:
											{
												mcu2wifi_heartbeat ();			//MCU回复心跳包
												break;
											}

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

			break;																				//得到此时的StaNum与crowd，跳出while循环
		}

	EMBARC_PRINTF ("StaNum=%d,crowd=%d\r\n", StaNum, crowd);
	mcu2wifi_dev_report_status (StaNum, crowd); 			//将数据传入云端
	delay_ms (10000);

	/*————————————————————————第二个公交站牌的数据上传由小
	车完成，第二个，第四个站牌由云端读入改变数据，小灯发生
	相应改变。——————————*/
	while (gjm != 3)
		{
			gjm 								= lookforrssi3 ();				//当rssi满足条件，返回gjm=2，跳出循环，进行下一步对于机智云的操作
			StaNum							= 3;											//得到此时的公交所在站点，衡量水平为StaNum，此时值为2。	
		}

	crowd 							= ct2crowd ();								//通过红外脉冲模块计算车载的拥挤程度，衡量水平为crowd

	while (1) //ARC板子读取wifi发送的数据函数,MCU完成回复，必不可少。
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
												mcu2wifi_wifi_ctrl_dev (read_data); //MCU回复wifi的控制指令
												break;
											}

									case 0x06:
										break;

									case 0x07:
											{
												mcu2wifi_heartbeat ();			//MCU回复心跳包
												break;
											}

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

			break;																				//得到此时的StaNum与crowd，跳出while循环
		}

	EMBARC_PRINTF ("StaNum=%d,crowd=%d\r\n", StaNum, crowd);
	mcu2wifi_dev_report_status (StaNum, crowd); 			//将数据传入云端
	delay_ms (10000);

	/*————————————————————————第三个公交站牌的数据上传由小
	车完成，第四个站牌由云端读入改变数据，小灯发生相应改变。
	——————————*/
	while (gjm != 4)
		{
			gjm 								= lookforrssi4 ();				//当rssi满足条件，返回gjm=2，跳出循环，进行下一步对于机智云的操作
			StaNum							= 4;											//得到此时的公交所在站点，衡量水平为StaNum，此时值为2。	
		}

	crowd 							= ct2crowd ();								//通过红外脉冲模块计算车载的拥挤程度，衡量水平为crowd

	while (1) //ARC板子读取wifi发送的数据函数,MCU完成回复，必不可少。
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
												mcu2wifi_wifi_ctrl_dev (read_data); //MCU回复wifi的控制指令
												break;
											}

									case 0x06:
										break;

									case 0x07:
											{
												mcu2wifi_heartbeat ();			//MCU回复心跳包
												break;
											}

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

			break;																				//得到此时的StaNum与crowd，跳出while循环
		}

	EMBARC_PRINTF ("StaNum=%d,crowd=%d\r\n", StaNum, crowd);
	mcu2wifi_dev_report_status (StaNum, crowd); 			//将数据传入云端
	delay_ms (10000);

	/*————————————————————————第四个公交站牌的数据上传由小
	车完成，第四个站牌由云端读入改变数据，小灯发生相应改变。
	——————————*/
	return E_SYS;
}


