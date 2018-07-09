/*
 *Protocols
 *CPLD: 16bits datas continuous transfer
 *EMSK: 8bits datas transfer, alternating reader and writing
 */

#include "embARC.h"
#include "embARC_debug.h"
#include "spi.h"

/*
 *	16bits data, upper 3-bit : command
 *	----------------------------------
 *    15  |	   1/0 -> start transfer
 *  ------|---------------------------
 *    14  |	   1/0 -> qei/pwm
 *  ------|---------------------------
 *    13  |	   1/0 -> L/R
 *  ------|---------------------------
 *   12-0 |    data
 *  ----------------------------------
 */
#define QEI_L   0xC0
#define QEI_R   0xE0
#define PWM_L	0x80
#define	PWM_R   0xA0

#define SPI_ID  DW_SPI_0_ID
#define CS_PORT	DEV_GPIO_PORT_A
#define CS_PIN  DEV_GPIO_PIN_28
#define CS_MASK 1<<CS_PIN

static DEV_SPI*  spi;
static DEV_GPIO* gpio;

int32_t spi_open(void)
{
	int32_t ercd = 0;

	spi = spi_get_dev(SPI_ID);
	configASSERT(spi != NULL);

	/*spi frequence:2M*/
	ercd = spi->spi_open(DEV_MASTER_MODE, 1000000);

	if (ercd != E_OK && ercd != E_OPNED) {
		return ercd;
	}

	/*
	 * Set designware spi device data frame size:16 bits
	 * However struct dev_spi_transfer tx_buf,rx_buf -> uint8_t
	 * it doesn't work as expected
	 */
	//spi->spi_control(SPI_CMD_SET_DFS, CONV2VOID(16));
	spi->spi_control(SPI_CMD_SET_CLK_MODE, CONV2VOID(SPI_CPOL_0_CPHA_1));
	/*
	 * avoid starting transmission
	 */
	spi->spi_control(SPI_CMD_SET_DUMMY_DATA, CONV2VOID(0x00));

	return E_OK;
}

int32_t gpio_open(void)
{
	int32_t ercd = 0;

	gpio = gpio_get_dev(CS_PORT);
	configASSERT(gpio != NULL);

	ercd = gpio->gpio_open(CS_MASK | (1<<DEV_GPIO_PIN_18));
	if (ercd == E_OPNED) {
		gpio->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(CS_MASK | (1<<DEV_GPIO_PIN_18)));
	}
	return E_OK;
}

/*
 * \brief	open spi interface, use J6 spi0 (CS0)
 * \retval	0	success
 * \retval	-1	fail
 */

int32_t cpld_spi_init(void)
{
	int32_t ercd = 0;

	ercd = spi_open();
	if (ercd != E_OK) {
		return ercd;
	}

	ercd = gpio_open();
	if (ercd != E_OK) {
		return ercd;
	}

	/* write 1 to CS pin, pull-up */
	gpio->gpio_write(CS_MASK, CS_MASK);

	return E_OK;
}

static void spi_select(void)
{
	/*select spi slave device*/
	spi->spi_control(SPI_CMD_MST_SEL_DEV, CONV2VOID(EMSK_SPI_LINE_0));
	/* write 0 to CS pin, pull-down */
	gpio->gpio_write((~CS_MASK), CS_MASK);
}

static void spi_deselect(void)
{
	/* write 1 to CS pin, pull-up */
	gpio->gpio_write(CS_MASK, CS_MASK);
	/*deselect spi slave device*/
	spi->spi_control(SPI_CMD_MST_DSEL_DEV, CONV2VOID(EMSK_SPI_LINE_0));
}

/*
 * \brief	read qei[2](16bits)(continuous 2x8bits)
 * \param   array to store data
 * \retval  spi status
 */

// int32_t spi_read_qei(int16_t *qei)
int32_t spi_read_qei(int16_t *qei, int dir)
{
	int32_t ercd = 0;
	static uint64_t cnt;

	DEV_SPI_TRANSFER xfer;
	char qeiTmp[2]={0, 0};
	unsigned int cs_cpu_status;

	DEV_SPI_XFER_SET_TXBUF(&xfer, NULL, 0, 0);
	DEV_SPI_XFER_SET_RXBUF(&xfer, qeiTmp, 0, 2);
	DEV_SPI_XFER_SET_NEXT(&xfer, NULL);

	spi_select();

	switch(dir){
		case 0:
			spi->spi_control(SPI_CMD_SET_DUMMY_DATA, CONV2VOID(QEI_L));
			break;
		case 1:
			spi->spi_control(SPI_CMD_SET_DUMMY_DATA, CONV2VOID(QEI_R));
			break;
	}
	ercd = spi->spi_control(SPI_CMD_TRANSFER_POLLING, CONV2VOID(&xfer));

	spi_deselect();

	*qei = (qeiTmp[0] << 8) | (qeiTmp[1] & 0xFF);
	if(*qei & 0x0800) *qei = *qei | 0xF000;
	
	return ercd;

}

/*
 * \brief	write pwm[2](data:signed 12bits) (split 4x8bits datas)
 			Format:
 			-----------------------------------------
 			  L  |      data[0]        |   data[1]
 			-----|---------------------|--------------
 				 | pwm_l | (pwm[0].hi) | (pwm[1].lo)
			-----|---------------------|--------------
 			  R  |      data[0]        |   data[1]
 			-----|---------------------|--------------
 				 | pwm_r | (pwm[1].hi) | (pwm[2].lo)	
 			-----------------------------------------
 * \param   array to transfer
 * \retval  spi status
 */

int32_t spi_write_pwm(int16_t *pwm, int dir)
{
	int32_t ercd = 0;

	DEV_SPI_TRANSFER xfer;

	int8_t local_buf[2];

	switch(dir){
		/*
		 * convert pwm to 8bits
		 */
		case 0:
			local_buf[0] = (int8_t)((pwm[0] >> 8) & 0x0F | PWM_L);
			local_buf[1] = (int8_t)((pwm[0] << 8) >> 8);
			break;
		case 1:
			local_buf[0] = (int8_t)((pwm[0] >> 8) & 0x0F | PWM_R);
			local_buf[1] = (int8_t)((pwm[0] << 8) >> 8);
			break;

	}

	DEV_SPI_XFER_SET_TXBUF(&xfer, local_buf, 0, 2);
	DEV_SPI_XFER_SET_RXBUF(&xfer, NULL, 2, 0);
	DEV_SPI_XFER_SET_NEXT(&xfer, NULL);

	spi_select();

	ercd = spi->spi_control(SPI_CMD_TRANSFER_POLLING, CONV2VOID(&xfer));
	
	spi_deselect();

	return ercd;

}