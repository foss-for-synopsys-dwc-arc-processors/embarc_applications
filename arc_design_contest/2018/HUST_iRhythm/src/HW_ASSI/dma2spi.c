/**
 * DMA for SPI API CODE
 * DDK Modify from Spiflash Example
 * 2018 03 10
 */

#include "embARC.h"
#include "embARC_debug.h"

#include "arc_udma.h"
#include "dw_spi_hal.h"
#include "dw_spi.h"

#include "include.h"

static volatile DW_SPI_REG *spi_reg = (DW_SPI_REG *)0xF0006000U;

static dma_state_t udma;
static dma_channel_t dma_chn_tx, dma_chn_rx;

#define SPI_TX_UDMA_CHN		0
#define SPI_RX_UDMA_CHN		1

#define SPI_XFER_ARB_SZ		16

#define _MEMORY_FENCE()				_arc_sync()
#define _DCACHE_FLUSH_MLINES(addr, size)	dcache_flush_mlines((uint32_t)(addr), (uint32_t)(size))
#define _DCACHE_INVALIDATE_MLINES(addr, size)	dcache_invalidate_mlines((uint32_t)(addr), (uint32_t)(size))
#define _ICACHE_INVALIDATE_MLINES(addr, size)	icache_invalidate_mlines((uint32_t)(addr), (uint32_t)(size))

typedef struct spi_xfer SPI_XFER, *SPI_XFER_PTR;

/**
 * \brief	spi read and write data structure used by \ref SPI_CMD_TRANSFER
 * 	        spi write then read data
 *
 */
struct spi_xfer {
	/* Set by user */
	uint8_t *tx_buf;
	// uint8_t *rx_buf;
	uint32_t len;
	/* Should auto set to proper value during set buffer value */
	uint32_t tx_idx;
	uint32_t rx_idx;
};


SPI_XFER data_xfer;
static void flush_xfer_data(SPI_XFER *xfer)
{
	SPI_XFER *cur_xfer = xfer;
	uint8_t i = 0;

	if (cur_xfer->tx_buf) {
		_MEMORY_FENCE();
		_DCACHE_FLUSH_MLINES((void *)(cur_xfer->tx_buf), cur_xfer->len);
	}


}

static void invalidate_xfer_data(SPI_XFER *xfer)
{
	SPI_XFER *cur_xfer = xfer;

	if (cur_xfer->tx_buf) {
		_MEMORY_FENCE();
		_DCACHE_INVALIDATE_MLINES((void *)(cur_xfer->tx_buf), cur_xfer->len);
	}

}


/**
 * \brief       Send music transfer complete to music task to continue
 *
 * \param[in]   param                  Interrupt param transmit by base function
 *
 */
void spi_xfer_callback(void *param)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	spi_reg->SSIENR = DW_SPI_SSI_DISABLE;
	spi_reg->DMACR = 0;
	spi_reg->DMATDLR = 0;
	spi_reg->DMARDLR = 0;
	spi_reg->SER = 0;
	spi_reg->SSIENR = DW_SPI_SSI_ENABLE;

	// invalidate_xfer_data(&data_xfer);

	/* Set the First Bit of Music Task Event */
	xEventGroupSetBitsFromISR(
		evt1_cb,	// The event group being updated.
		BIT_0 ,   // The bits being set.
		&xHigherPriorityTaskWoken );
}

/**
 * \brief       Set DMA configration to start DMA transfer
 *
 * \param[in]   xfer                   Transfer configration struct
 *
 * \retval      0                      Function complete success
 */
static int32_t spi_xfer(SPI_XFER *xfer)
{
	static __attribute__((aligned(16))) dma_desc_t dma_desc_tx;
	static __attribute__((aligned(16))) dma_desc_t dma_desc_rx;
	static dma_ctrl_t dma_ctrl_tx, dma_ctrl_rx;
	dma_callback_t xfer_cb = NULL;

	SPI_XFER *cur_xfer = xfer;

	uint32_t txtemp = 0xFFFFFFFF, rxtemp  = 0;


	DMA_CTRL_SET_OP(&dma_ctrl_tx, DMA_MANUAL_LINKED_TRANSFER);
	DMA_CTRL_SET_RT(&dma_ctrl_tx, DMA_MANUAL_REQUEST);
	DMA_CTRL_SET_DTT(&dma_ctrl_tx, DMA_MEM2MEM);
	DMA_CTRL_SET_DWINC(&dma_ctrl_tx, DMA_DW1INC1);
	DMA_CTRL_SET_ARB(&dma_ctrl_tx, SPI_XFER_ARB_SZ);
	DMA_CTRL_SET_INT(&dma_ctrl_tx, DMA_INT_DISABLE);

	if (cur_xfer->tx_buf) {
		DMA_CTRL_SET_AM(&dma_ctrl_tx, DMA_AM_SRCINC_DSTNOT);
		dmac_config_desc(&dma_desc_tx, (void *)(cur_xfer->tx_buf), (void *)(&spi_reg->DATAREG), cur_xfer->len, &dma_ctrl_tx);
	} else {
		DMA_CTRL_SET_AM(&dma_ctrl_tx, DMA_AM_SRCNOT_DSTNOT);
		dmac_config_desc(&dma_desc_tx, (void *)(&txtemp), (void *)(&spi_reg->DATAREG), cur_xfer->len, &dma_ctrl_tx);
	}

	DMA_CTRL_SET_OP(&dma_ctrl_rx, DMA_MANUAL_LINKED_TRANSFER);
	DMA_CTRL_SET_RT(&dma_ctrl_rx, DMA_MANUAL_REQUEST);
	DMA_CTRL_SET_DTT(&dma_ctrl_rx, DMA_MEM2MEM);
	DMA_CTRL_SET_DWINC(&dma_ctrl_rx, DMA_DW1INC1);
	DMA_CTRL_SET_ARB(&dma_ctrl_rx, SPI_XFER_ARB_SZ);
	DMA_CTRL_SET_INT(&dma_ctrl_rx, DMA_INT_DISABLE);

	DMA_CTRL_SET_AM(&dma_ctrl_rx, DMA_AM_SRCNOT_DSTNOT);
	dmac_config_desc(&dma_desc_rx, (void *)(&spi_reg->DATAREG), (void *)(&rxtemp), cur_xfer->len, &dma_ctrl_rx);

	dmac_desc_add_linked(&dma_desc_tx, NULL);
	dmac_desc_add_linked(&dma_desc_rx, NULL);

	/* Init and configure dma channel transfer with transfer descriptor */
	dmac_config_channel(&dma_chn_tx, &dma_desc_tx);
	dmac_config_channel(&dma_chn_rx, &dma_desc_rx);

	dmac_stop_channel(&dma_chn_rx);
	dmac_stop_channel(&dma_chn_tx);

	xfer_cb = spi_xfer_callback;

	/* select device */
	spi_reg->SSIENR = DW_SPI_SSI_DISABLE;
	spi_reg->DMACR = 0;
	spi_reg->DMATDLR = 32 - SPI_XFER_ARB_SZ;
	spi_reg->DMARDLR = SPI_XFER_ARB_SZ - 1;
	flush_xfer_data(xfer);
	// dmac_start_channel(&dma_chn_rx, xfer_cb, DMA_CHN_HIGH_PRIO);
	dmac_start_channel(&dma_chn_tx, xfer_cb, DMA_CHN_HIGH_PRIO);
	spi_reg->SER = 1 << EMSK_SPI_LINE_0;
	spi_reg->SSIENR = DW_SPI_SSI_ENABLE;
	/* enable rx and tx dma */
	spi_reg->DMACR = 3;
	// dmac_wait_channel(&dma_chn_tx);
	// dmac_wait_channel(&dma_chn_rx);
	/* deselect device */
	// spi_reg->SSIENR = DW_SPI_SSI_DISABLE;
	// spi_reg->DMACR = 0;
	// spi_reg->DMATDLR = 0;
	// spi_reg->DMARDLR = 0;
	// spi_reg->SER = 0;
	// spi_reg->SSIENR = DW_SPI_SSI_ENABLE;

	// invalidate_xfer_data(&data_xfer);

	return 0;
}

/**
 * \brief       Initialization DMA chnnal
 *
 */
void spi_dma_prepare(void)
{
	/** Must init uDMA before use it */
	dmac_init(&udma);

	dmac_init_channel(&dma_chn_tx);
	dmac_init_channel(&dma_chn_rx);
	dmac_reserve_channel(SPI_TX_UDMA_CHN, &dma_chn_tx, DMA_REQ_PERIPHERAL);
	dmac_reserve_channel(SPI_RX_UDMA_CHN, &dma_chn_rx, DMA_REQ_PERIPHERAL);
}

/**
 * \brief       Write Data to SPI to Exchange to I2S by Outside Circuit (FPGA)
 *
 * \param[in]	data	Pointer to Data that need be sent
 *
 * \retval	=0		finish
 * \retval 	<0 		error
 */
int32_t spi_writeraw(const void *data)
{

	uint8_t *xfer_buf = (uint8_t *)data;

	data_xfer.tx_buf = (uint8_t *)(xfer_buf);
	data_xfer.len = 2304;

	/* Ask Dma to Send Here*/
	if (spi_xfer(&data_xfer) != 0) {
		return -1;
		EMBARC_PRINTF("SPI XFER ERROR!\r\n");
	}

	return 0;
}