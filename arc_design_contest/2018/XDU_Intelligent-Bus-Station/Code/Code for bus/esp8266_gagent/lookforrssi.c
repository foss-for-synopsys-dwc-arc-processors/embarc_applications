

//PMOD5下行端口，接AT-wifi指令，扫描station发出的Rssi。
#include "embARC.h"
#include "embARC_debug.h"
#include <string.h>
#include <stdlib.h>

#include "dev_uart.h"	 //UART API 头文件`
#include "../../../board/emsk/drivers/mux/mux.h"	// Pmod端口配置API`
#include "../../../board/emsk/drivers/mux/mux_hal.h"


#define BUTTON_MASK 						0x7
#define MAX_READ_CNT						32


int32_t lookforrssi1 (void)
{
	DEV_UART_PTR		uart2;

	set_uart_map (0xE4);
	set_pmod_mux (PM1_UR_UART_0 | PM1_LR_GPIO_A | PM2_I2C_HRI | PM3_GPIO_AC | PM4_I2C_GPIO_D | PM5_UR_GPIO_C | PM5_LR_UART_2 | PM6_UR_SPI_M0 | PM6_LR_GPIO_A);
	int32_t 				ercd;

	uart2 							= uart_get_dev (DW_UART_2_ID);
	ercd								= uart2->uart_open (115200);
	char						send_string1[64] = "AT+CWMODE=1\r\n";

	ercd								= uart2->uart_write (send_string1, sizeof (send_string1));
	board_delay_ms (1000, 0);
	char						receive_string1[64] =
		{
			0
		};
	uint32_t				rd_avail = 0;

	while (1)
		{
			board_delay_ms (1000, 0);
			uart2->uart_control (UART_CMD_GET_RXAVAIL, (void *) (&rd_avail));

			if (rd_avail > 0)
				{
					EMBARC_PRINTF ("rd_avail is %d\r\n", rd_avail);

					uart2->uart_read (receive_string1, rd_avail);
					EMBARC_PRINTF ("receive_string1=%s\r\n", receive_string1); //打印出模式置位1的回复
					break;
				}
		}

	char						send_string2[64] = "AT+CWLAP=\"S1\"\r\n";
	char						receive_string2[64] =
		{
			0
		};
	rd_avail						= 0;
	uint32_t				rssi;

	while (1)
		{
			ercd								= uart2->uart_write (send_string2, sizeof (send_string2));

			while (1)
				{

					board_delay_ms (1000, 0);
					uart2->uart_control (UART_CMD_GET_RXAVAIL, (void *) (&rd_avail));

					if (rd_avail > 0)
						{
							EMBARC_PRINTF ("rd_avail is %d\r\n", rd_avail);

							uart2->uart_read (receive_string2, rd_avail);
							EMBARC_PRINTF ("receive_string2=%s\r\n", receive_string2); //打印出含rssi的一串字符

							char *					temp = strtok (receive_string2, ",");
							uint32_t				c;

							for (c = 0; c < 3; c++)
								{
									temp								= strtok (NULL, ",");

									if (c == 1)
										{
											EMBARC_PRINTF ("temp=%s\r\n", temp);

											if (temp != "0")
												{
													rssi								= atoi (temp);

													if (rssi != 0)
														{
															EMBARC_PRINTF ("rssi=%d\r\n", rssi);

															if (rssi > -50 || rssi < 0)
																{
																	EMBARC_PRINTF ("rssi=%d\r\n", rssi);
																	return 1;
																}
															else 
																;
														}
													else 
														;
												}
											else 
												;
										}
									else 
										;
								}
						}
					else 
						{
							board_delay_ms (1000, 0);
							break;																//返回外循环，重新扫描RSSI	
						}
				}
		}

}


int32_t lookforrssi2 (void)
{
	DEV_UART_PTR		uart2;

	set_uart_map (0xE4);
	set_pmod_mux (PM1_UR_UART_0 | PM1_LR_GPIO_A | PM2_I2C_HRI | PM3_GPIO_AC | PM4_I2C_GPIO_D | PM5_UR_GPIO_C | PM5_LR_UART_2 | PM6_UR_SPI_M0 | PM6_LR_GPIO_A);
	int32_t 				ercd;

	uart2 							= uart_get_dev (DW_UART_2_ID);
	ercd								= uart2->uart_open (115200);
	char						send_string1[64] = "AT+CWMODE=1\r\n";

	ercd								= uart2->uart_write (send_string1, sizeof (send_string1));
	board_delay_ms (1000, 0);
	char						receive_string1[64] =
		{
			0
		};
	uint32_t				rd_avail = 0;

	while (1)
		{
			board_delay_ms (1000, 0);
			uart2->uart_control (UART_CMD_GET_RXAVAIL, (void *) (&rd_avail));

			if (rd_avail > 0)
				{
					EMBARC_PRINTF ("rd_avail is %d\r\n", rd_avail);

					uart2->uart_read (receive_string1, rd_avail);
					EMBARC_PRINTF ("receive_string1=%s\r\n", receive_string1); //打印出模式置位1的回复
					break;
				}
		}

	char						send_string2[64] = "AT+CWLAP=\"S2\"\r\n";
	char						receive_string2[64] =
		{
			0
		};
	rd_avail						= 0;
	uint32_t				rssi;

	while (1)
		{
			ercd								= uart2->uart_write (send_string2, sizeof (send_string2));

			while (1)
				{

					board_delay_ms (1000, 0);
					uart2->uart_control (UART_CMD_GET_RXAVAIL, (void *) (&rd_avail));

					if (rd_avail > 0)
						{
							EMBARC_PRINTF ("rd_avail is %d\r\n", rd_avail);

							uart2->uart_read (receive_string2, rd_avail);
							EMBARC_PRINTF ("receive_string2=%s\r\n", receive_string2); //打印出含rssi的一串字符

							char *					temp = strtok (receive_string2, ",");
							uint32_t				c;

							for (c = 0; c < 3; c++)
								{
									temp								= strtok (NULL, ",");

									if (c == 1)
										{
											EMBARC_PRINTF ("temp=%s\r\n", temp);

											if (temp != "0")
												{
													rssi								= atoi (temp);

													if (rssi != 0)
														{
															EMBARC_PRINTF ("rssi=%d\r\n", rssi);

															if (rssi > -50 || rssi < 0)
																{
																	EMBARC_PRINTF ("rssi=%d\r\n", rssi);
																	return 2;
																}
															else 
																;
														}
													else 
														;
												}
											else 
												;
										}
									else 
										;
								}
						}
					else 
						{
							board_delay_ms (1000, 0);
							break;																//返回外循环，重新扫描RSSI	
						}
				}
		}

}


int32_t lookforrssi3 (void)
{
	DEV_UART_PTR		uart2;

	set_uart_map (0xE4);
	set_pmod_mux (PM1_UR_UART_0 | PM1_LR_GPIO_A | PM2_I2C_HRI | PM3_GPIO_AC | PM4_I2C_GPIO_D | PM5_UR_GPIO_C | PM5_LR_UART_2 | PM6_UR_SPI_M0 | PM6_LR_GPIO_A);
	int32_t 				ercd;

	uart2 							= uart_get_dev (DW_UART_2_ID);
	ercd								= uart2->uart_open (115200);
	char						send_string1[64] = "AT+CWMODE=1\r\n";

	ercd								= uart2->uart_write (send_string1, sizeof (send_string1));
	board_delay_ms (1000, 0);
	char						receive_string1[64] =
		{
			0
		};
	uint32_t				rd_avail = 0;

	while (1)
		{
			board_delay_ms (1000, 0);
			uart2->uart_control (UART_CMD_GET_RXAVAIL, (void *) (&rd_avail));

			if (rd_avail > 0)
				{
					EMBARC_PRINTF ("rd_avail is %d\r\n", rd_avail);

					uart2->uart_read (receive_string1, rd_avail);
					EMBARC_PRINTF ("receive_string1=%s\r\n", receive_string1); //打印出模式置位1的回复
					break;
				}
		}

	char						send_string2[64] = "AT+CWLAP=\"S3\"\r\n";
	char						receive_string2[64] =
		{
			0
		};
	rd_avail						= 0;
	uint32_t				rssi;

	while (1)
		{
			ercd								= uart2->uart_write (send_string2, sizeof (send_string2));

			while (1)
				{

					board_delay_ms (1000, 0);
					uart2->uart_control (UART_CMD_GET_RXAVAIL, (void *) (&rd_avail));

					if (rd_avail > 0)
						{
							EMBARC_PRINTF ("rd_avail is %d\r\n", rd_avail);

							uart2->uart_read (receive_string2, rd_avail);
							EMBARC_PRINTF ("receive_string2=%s\r\n", receive_string2); //打印出含rssi的一串字符

							char *					temp = strtok (receive_string2, ",");
							uint32_t				c;

							for (c = 0; c < 3; c++)
								{
									temp								= strtok (NULL, ",");

									if (c == 1)
										{
											EMBARC_PRINTF ("temp=%s\r\n", temp);

											if (temp != "0")
												{
													rssi								= atoi (temp);

													if (rssi != 0)
														{
															EMBARC_PRINTF ("rssi=%d\r\n", rssi);

															if (rssi > -50 || rssi < 0)
																{
																	EMBARC_PRINTF ("rssi=%d\r\n", rssi);
																	return 3;
																}
															else 
																;
														}
													else 
														;
												}
											else 
												;
										}
									else 
										;
								}
						}
					else 
						{
							board_delay_ms (1000, 0);
							break;																//返回外循环，重新扫描RSSI	
						}
				}
		}

}


int32_t lookforrssi4 (void)
{
	DEV_UART_PTR		uart2;

	set_uart_map (0xE4);
	set_pmod_mux (PM1_UR_UART_0 | PM1_LR_GPIO_A | PM2_I2C_HRI | PM3_GPIO_AC | PM4_I2C_GPIO_D | PM5_UR_GPIO_C | PM5_LR_UART_2 | PM6_UR_SPI_M0 | PM6_LR_GPIO_A);
	int32_t 				ercd;

	uart2 							= uart_get_dev (DW_UART_2_ID);
	ercd								= uart2->uart_open (115200);
	char						send_string1[64] = "AT+CWMODE=1\r\n";

	ercd								= uart2->uart_write (send_string1, sizeof (send_string1));
	board_delay_ms (1000, 0);
	char						receive_string1[64] =
		{
			0
		};
	uint32_t				rd_avail = 0;

	while (1)
		{
			board_delay_ms (1000, 0);
			uart2->uart_control (UART_CMD_GET_RXAVAIL, (void *) (&rd_avail));

			if (rd_avail > 0)
				{
					EMBARC_PRINTF ("rd_avail is %d\r\n", rd_avail);

					uart2->uart_read (receive_string1, rd_avail);
					EMBARC_PRINTF ("receive_string1=%s\r\n", receive_string1); //打印出模式置位1的回复
					break;
				}
		}

	char						send_string2[64] = "AT+CWLAP=\"S4\"\r\n";
	char						receive_string2[64] =
		{
			0
		};
	rd_avail						= 0;
	uint32_t				rssi;

	while (1)
		{
			ercd								= uart2->uart_write (send_string2, sizeof (send_string2));

			while (1)
				{

					board_delay_ms (1000, 0);
					uart2->uart_control (UART_CMD_GET_RXAVAIL, (void *) (&rd_avail));

					if (rd_avail > 0)
						{
							EMBARC_PRINTF ("rd_avail is %d\r\n", rd_avail);

							uart2->uart_read (receive_string2, rd_avail);
							EMBARC_PRINTF ("receive_string2=%s\r\n", receive_string2); //打印出含rssi的一串字符

							char *					temp = strtok (receive_string2, ",");
							uint32_t				c;

							for (c = 0; c < 3; c++)
								{
									temp								= strtok (NULL, ",");

									if (c == 1)
										{
											EMBARC_PRINTF ("temp=%s\r\n", temp);

											if (temp != "0")
												{
													rssi								= atoi (temp);

													if (rssi != 0)
														{
															EMBARC_PRINTF ("rssi=%d\r\n", rssi);

															if (rssi > -50 || rssi < 0)
																{
																	EMBARC_PRINTF ("rssi=%d\r\n", rssi);
																	return 4;
																}
															else 
																;
														}
													else 
														;
												}
											else 
												;
										}
									else 
										;
								}
						}
					else 
						{
							board_delay_ms (1000, 0);
							break;																//返回外循环，重新扫描RSSI	
						}
				}
		}

}


