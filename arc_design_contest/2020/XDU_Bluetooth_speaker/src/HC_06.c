#include "HC_06.h"

DEV_UART * UART2 = NULL;

uint32_t UART2_init(uint32_t baudrate)
{
	/*
     * UART 2 initialization
	 * Modify \embARC\board\emsk\common\emsk_init.c
	 * PM5_LR_GPIO_A    to   PM5_LR_SPI_M2 -name BUG
	 * J5.12 3V3
	 * J5.11 GND
	 * J5.9  UART2_RXD
	 * J5.8  UART2_TXD
	 */

	UART2 = uart_get_dev(HC_06_UART_ID);

	int32_t uart2_stat = UART2->uart_open(baudrate);

	if (UART2 == NULL)
		return -1;
	else if (uart2_stat == E_OPNED || uart2_stat == E_OK) {
		UART2->uart_control(UART_CMD_SET_BAUD, (void *)(baudrate)) ;
		return 1;
	} else
		return 0;
}

uint32_t UART2_write(uint8_t *buf, uint32_t cnt)
{
	if (UART2 == NULL)
		return  0;

	return UART2->uart_write(buf, cnt);
}

uint32_t UART2_read(uint8_t *buf, uint32_t cnt)
{
	if (UART2 == NULL)
		return 0;

	uint32_t rd_avail;

	UART2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
	cnt = (cnt > rd_avail)?rd_avail:cnt;
	{
		if (cnt > 0)
			return UART2->uart_read(buf, cnt);

	}
	return cnt;
}

uint8_t get_ges_cmd(void)
{
	uint8_t  RECbuf;
	uint32_t data_avail = 0;

	UART2->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&data_avail));
		if (data_avail > 0) {
			UART2_read(&RECbuf, 1);
			return RECbuf;
		}
}
