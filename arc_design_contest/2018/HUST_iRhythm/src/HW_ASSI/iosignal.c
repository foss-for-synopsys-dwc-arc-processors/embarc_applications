/**
 * IO API CODE
 * DDK
 * 2018 03 10
 */

#include "embARC.h"
#include "embARC_debug.h"


#include "../../board/emsk/drivers/mux/mux_hal.h"
#include "../../board/emsk/drivers/mux/mux.h"


#include "include.h"

/*       A[31:28]    PMOD6[10:7]    */
#define BOARD_SIGNOUT_MASK 0x30ff0100
#define BOARD_SIGNIN_MASK 0xC0000007
#define LED_PIX_MASK 0x00ff0000


DEV_GPIO *io_signal;
DEV_GPIO *LED_signal;

/**
 * \brief       IO Interrupt that Trigger Music Event to Switch back Music Task
 *
 */
void empty_isr()
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* Set the Second Bit of Music Task Event */
	xEventGroupSetBitsFromISR(
		evt1_cb,	// The event group being updated.
		BIT_1,   // The bits being set.
		&xHigherPriorityTaskWoken );
}


/**
 * \brief       IO Port Initalization
 *              3 Key with Interruption\
 *              2 Output for Performance Test during Debug\
 *              2 Input IO with interruption:
 *              One set for Music Task to Monitoring FPGA FIFO State,another is a Backup\
 *              1 Output IO for Reset Pin of ESP8266
 *
 */
void iosignal_init()
{
	DEV_GPIO_INT_CFG gpio_int_cfg;
	DEV_GPIO_BIT_ISR gpio_bit_isr;

	/**
	 * Change Low Row Function of PM1 to GPIOA
	 * Change PM4 to GPIOA GPIO C
	 */
	uint32_t val = get_pmod_mux();
	val &= ~(PM1_LR_SPI_S | PM4_I2C_GPIO_D);

	set_pmod_mux(val);

	gpio_int_cfg.int_bit_mask = BOARD_SIGNIN_MASK;
	gpio_int_cfg.int_bit_type = BOARD_SIGNIN_MASK;
	gpio_int_cfg.int_bit_polarity = GPIO_INT_RISING_EDGE_ALL;
	gpio_int_cfg.int_bit_debounce = 0;

	gpio_bit_isr.int_bit_ofs = 30;
	gpio_bit_isr.int_bit_handler = empty_isr;

	io_signal = gpio_get_dev(DEV_GPIO_PORT_A);
	LED_signal = gpio_get_dev(DEV_GPIO_PORT_C);

	if (io_signal->gpio_open(0xf0ff0000) == E_OPNED) {
		io_signal->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(BOARD_SIGNOUT_MASK));
		io_signal->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(BOARD_SIGNIN_MASK));
		io_signal->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(BOARD_SIGNOUT_MASK));

		/*************Interrupt enable********************/
		io_signal->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(BOARD_SIGNIN_MASK));

		gpio_bit_isr.int_bit_ofs = 30;
		gpio_bit_isr.int_bit_handler = empty_isr;
		io_signal->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&gpio_bit_isr));


		gpio_bit_isr.int_bit_ofs = 0;
		gpio_bit_isr.int_bit_handler = key1_isr;
		io_signal->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&gpio_bit_isr));

		gpio_bit_isr.int_bit_ofs = 1;
		gpio_bit_isr.int_bit_handler = key2_isr;
		io_signal->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&gpio_bit_isr));

		gpio_bit_isr.int_bit_ofs = 2;
		gpio_bit_isr.int_bit_handler = key3_isr;
		io_signal->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&gpio_bit_isr));

		io_signal->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, (void *)(&gpio_int_cfg));
		io_signal->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)(BOARD_SIGNIN_MASK));
	}

	if (LED_signal->gpio_open(LED_PIX_MASK) == E_OPNED) {
		LED_signal->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(LED_PIX_MASK));
	}

	iosignal_ctrl(0, 0);
	iosignal_ctrl(0, 1);

	io_signal->gpio_write(0x00000100, 0x00000100);
	// iosignal_ctrl(0,2);
	// iosignal_ctrl(0,3);
error_exit:
	return;
}

/**
 * \brief       IO Output Control, just Defined For Debug
 *
 * \param[in]   val                    1:IO LOGIC HIGH 0:IO LOGIC LOW
 *
 * \param[in]   num                    0 or 1 for two special IO for debug popuse
 *
 */
void iosignal_ctrl(uint8_t val, uint8_t num)
{
	uint32_t temp = 0;

	temp = val ? 0x10000000 : 0;

	switch (num) {
		case 0:
			io_signal->gpio_write(temp << 0, 0x10000000);
			break;

		case 1:
			io_signal->gpio_write(temp << 1, 0x20000000);
			break;

		default:
			break;
	}

	return;
}

/**
 * \brief       Read IO Logical Lever
 *
 * \param[in]   num                    0 or 1 for two special IO
 *
 * \retval      0                      Logical LOW
 *
 * \retval      1                      Logical HIGH
 *
 */
uint8_t iosignal_read(uint8_t num)
{
	uint32_t value = 0, mask;

	if ( num == 0 ) {
		mask = 0x40000000;
		io_signal->gpio_read(&value, mask);
		value = (~value) & mask;
	} else {
		mask = 0x80000000;
		io_signal->gpio_read(&value, mask);
		value = (~value) & mask;
	}

	return (value ? 0 : 1);
}

/**
 * \brief       Pull Logical Low for a Period for ESP8266 Reset
 *
 */
void net_rst()
{
	/*          RST = 0          */
	io_signal->gpio_write(0x00000000, 0x00000100);

	_Block_Delay( 1000 );

	/*          RST = 1          */
	io_signal->gpio_write(0x00000100, 0x00000100);
}

/**
 * [set_LE description]
 * @param rows 0,all LE are 0 to lock
 * 1~8,rows1~8 LE are 1 to write and other are 0 to lock
 */
inline void set_LE(uint8_t rows)
{
	uint32_t val = 0x00008000;
	val = val << rows;
	io_signal->gpio_write(val, LED_PIX_MASK);
}

inline void set_LED(uint8_t data)
{
	// EMBARC_PRINTF("DATA = %d\r\n",data);
	uint32_t val = ((uint32_t)(data)) << 16;
	LED_signal->gpio_write(val, LED_PIX_MASK);
	// EMBARC_PRINTF("VAL = %d\r\n",val);
}

void led_row_ctl(uint8_t rows, uint8_t data)
{
	set_LED(data);
	set_LE(rows);
	set_LE(0x00);
}


