
#include "embARC.h"
#include "embARC_debug.h"

#include "dev_gpio.h" //GPIO API头文件
#include "../../../board/emsk/drivers/mux/mux.h"	
#include "../../../board/emsk/drivers/mux/mux_hal.h"

#define BUTTON_MASK 						0x7

uint8_t 				ct = 0;

uint8_t 				crowd = 0;
uint8_t 				CT = 0;


uint8_t intra_inout (void) 

{
	uint8_t 				cp	= 0;													//下车人数
	uint32_t				cn; 															//人数确认时间计数
	DEV_GPIO *			gpio_read_port;
	DEV_GPIO *			gpio_write_port;

	set_pmod_mux (PM1_UR_UART_0 | PM1_LR_GPIO_A | PM2_GPIO_AC | PM3_GPIO_AC | PM4_I2C_GPIO_D | PM5_UR_SPI_M1 | PM5_LR_UART_2 | PM6_UR_SPI_M0 | PM6_LR_GPIO_A);
	int32_t 				ercd1, ercd2;

	gpio_read_port			= gpio_get_dev (DW_GPIO_PORT_A);
	gpio_write_port 		= gpio_get_dev (DW_GPIO_PORT_C);
	ercd1 							= dw_gpio_open (gpio_read_port, 0x0);
	ercd2 							= dw_gpio_open (gpio_write_port, 0xfffff);

	if (ercd2 == E_OPNED)
		dw_gpio_control (gpio_write_port, GPIO_CMD_SET_BIT_DIR_OUTPUT, 0xfffff);

	uint32_t				data;

	while (1)
		{
			dw_gpio_read (gpio_read_port, &data, 0x4400);
			board_delay_ms (10, 1);

			if (data == 1024) 
				{
					board_delay_ms (10, 1);
					dw_gpio_read (gpio_read_port, &data, 0x4400);

					if (data == 0)
						{
							ct++; 																//此时仅检测到有人上车
							EMBARC_PRINTF ("ct=%d\r\n", ct);
							board_delay_ms (10, 1);
							continue;
						}
					else 
						continue;
				}
			else if (data == 0x4000)
				{
					board_delay_ms (10, 1);
					dw_gpio_read (gpio_read_port, &data, 0x4000);

					if (data == 0)
						{
							cp++; 																//此时仅有人下车
							EMBARC_PRINTF ("cp=%d\r\n", cp);
							board_delay_ms (10, 1);
							continue;
						}
					else 
						continue;
				}
			else if (data == 0)
				{
					dw_gpio_read (gpio_read_port, &data, 0x4400);
					cn									= 0;

					while (1)
						{
							board_delay_ms (10, 1);
							dw_gpio_read (gpio_read_port, &data, 0x4400);

							if (data == 0)
								{
									cn++; 														//计入0的个数

									if (cn > 1000)
										{
											ct									= ct - cp;
											EMBARC_PRINTF ("ct=%d\r\n", ct);
											return ct;
										}
									else 
										continue;
								}
							else 
								{
									break;
								}
						}
				}
			else 
				continue;
		}
}


uint32_t ct2crowd (void)
{
	CT									= intra_inout ();

	if (CT <= 10)
		crowd = 1;
	else 
		{
			if (CT <= 20)
				crowd = 2;
			else 
				crowd = 3;
		}

	return crowd;
}


