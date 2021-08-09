#include "LD3320a.h"

DEV_UART *dev_uart1;

// Get detection result
uint8_t Ars_Result(void)
{
	uint8_t result[1] = {0};
	uint8_t buf[1] = {0};
	int32_t ercd;

	// dev_uart1->uart_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));

	ercd = dev_uart1->uart_read(result, 1); /* Read data from uart */

	return result[0];
}

void LD3320A_init(void)
{
	dev_uart1 = uart_get_dev(DFSS_UART_1_ID);
	dev_uart1->uart_open(UART_BAUDRATE_9600);
	printf("LD3320A initialize and interrup finished!\n");
}

uint8_t get_audio_cmd(void)
{
	uint8_t result = 0;

	result = Ars_Result(); //reture the detecting result ¡ª¡ª the words number ID

	if (result) {
		return result;
	}
}
