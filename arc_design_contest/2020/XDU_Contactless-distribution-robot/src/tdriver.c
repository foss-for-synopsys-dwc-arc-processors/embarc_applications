#include "tdriver.h"

/**
 * \brief	IIC transmit callback function
 * \param[in]	*ptr
 */
static void tdr_iic_txcb(void *ptr)
{
	DEV_IIC *iic_obj;
	TDR *tdr;
	DEV_BUFFER dev_buf;

	iic_obj = (DEV_IIC *)ptr; /* Send master pointer to iic_obj */
	tdr = (TDR *)DEV_IIC_INFO_GET_EXTRA_OBJECT(&(iic_obj->iic_info)); /* Get tdr IIC slave point */

	if (tdr->cur_state == TDR_IN_READ) {
		iic_obj->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
		DEV_BUFFER_INIT(&dev_buf, &(tdr->regval), 1); /* Device buffer initialization */
		iic_obj->iic_control(IIC_CMD_SET_RXINT_BUF, &dev_buf); /* Set buffer for interrupt receive */
		iic_obj->iic_control(IIC_CMD_SET_RXINT, CONV2VOID(1)); /* Enable receive interrupt */
	} else if (tdr->cur_state == TDR_IN_WRITE) {
		tdr->cur_state = TDR_READY;
	}
}

/**
 * \brief	IIC receive callback function
 * \param[in]	*ptr
 */
static void tdr_iic_rxcb(void *ptr)
{
	DEV_IIC *iic_obj;
	TDR *tdr;

	iic_obj = (DEV_IIC *)ptr; /* Send master pointer to iic_obj */
	tdr = (TDR *)DEV_IIC_INFO_GET_EXTRA_OBJECT(&(iic_obj->iic_info)); /* Get TDR IIC slave point */

	if (tdr->cur_state == TDR_IN_READ) {
		tdr->cur_state = TDR_READY;
	}
}

/**
 * \brief	IIC error callback function
 * \param[in]	*ptr
 */
static void tdr_iic_errcb(void *ptr)
{
	DEV_IIC *iic_obj;
	TDR *tdr;

	iic_obj = (DEV_IIC *)ptr; /* Send master pointer to iic_obj */
	tdr = (TDR *)DEV_IIC_INFO_GET_EXTRA_OBJECT(&(iic_obj->iic_info)); /* Get TDR IIC slave point */

	if (tdr->cur_state == TDR_IN_READ) {
		iic_obj->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	}
	EMBARC_PRINTF("Error code:%d\r\n", iic_obj->iic_info.err_state);
	tdr->cur_state = TDR_IN_ERR;
}

/**
 * \brief	Read TDR register
 * \param[in]	regaddr		register address
 * \param[in]	*tdr	TDR device pointer
 */
void tdr_read_reg(TDR *tdr, uint8_t regaddr)
{
	DEV_BUFFER dev_buf;

	if (tdr->cur_state == TDR_IN_ERR) {
		tdr->cur_state = TDR_READY;
	}
	if (tdr->cur_state != TDR_READY) return;
	tdr->cur_state = TDR_IN_READ;
	tdr->regaddr = regaddr;

	DEV_BUFFER_INIT(&dev_buf, &(tdr->regaddr), 1);

	tdr->iic_obj->iic_control(IIC_CMD_SET_TXINT_BUF, &dev_buf); /* Set buffer for interrupt transmit */
	tdr->iic_obj->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART)); /* Set next condition for following operation */
	tdr->iic_obj->iic_control(IIC_CMD_SET_TXINT, CONV2VOID(1)); /* Set buffer for interrupt transmit */
}

/**
 * \brief	Write TDR register
 * \param[in]	*tdr	TDR device pointer
 * \param[in]	regaddr		register address
 * \param[in]	regval		TDR device pointer
 */
void tdr_write_reg(TDR *tdr, uint8_t regaddr, uint8_t regval)
{
	DEV_BUFFER dev_buf;

	if (tdr->cur_state == TDR_IN_ERR) {
		tdr->cur_state = TDR_READY;
	}
	if (tdr->cur_state != TDR_READY) return;
	tdr->cur_state = TDR_IN_WRITE;
	tdr->regaddr = regaddr;
	tdr->regval = regval;
	tdr->buf[0] = regaddr;
	tdr->buf[1] = regval;

	DEV_BUFFER_INIT(&dev_buf, tdr->buf, 2); /* Device buffer initialization */

	tdr->iic_obj->iic_control(IIC_CMD_SET_TXINT_BUF, &dev_buf); /* Set buffer for interrupt transmit */
	tdr->iic_obj->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP)); /* Set next condition for following operation */
	tdr->iic_obj->iic_control(IIC_CMD_SET_TXINT, CONV2VOID(1)); /* Enable transmit interrupt */
}

int32_t tdr_chk_state(TDR *tdr)
{
	return tdr->cur_state;
}

bool tdr_state_ready(TDR *tdr)
{
   if (tdr_chk_state(tdr) == TDR_READY) {
        return true;
   } else {
        return false;
   }
}

bool tdr_state_err(TDR *tdr)
{
   if (tdr_chk_state(tdr) == TDR_IN_ERR) {
    	return true;
   } else {
        return false;
   }
}

// static bool tdr_state_r(TDR *tdr)
// {
//    if(tdr_chk_state(tdr) == TDR_IN_READ) {
//         return true;
//    } else {
//         return false;
//    }
// }

// static bool tdr_state_w(TDR *tdr)
// {
//    if(tdr_chk_state(tdr) == TDR_IN_WRITE) {
//         return true;
//    } else {
//         return false;
//    }
// }

void tdr_clear_error(TDR *tdr)
{
	tdr->cur_state = TDR_READY;
}

/**
 * \brief	TDR initialization
 * \param[in]	*tdr	TDR device pointer
 * \param[in]	addr		TDR slave address
 * \param[in]   iic_id		IIC device ID
 */
void tdr_init(TDR *tdr, uint32_t iic_id, uint32_t addr)
{
	int32_t ercd;

	if (!tdr) return;

	tdr->iic_obj = iic_get_dev(iic_id); /* Get IIC device  */
	if (!tdr->iic_obj) {
		EMBARC_PRINTF("Can't get a IIC device for tdr \r\n");
		goto error_exit;
	}
	ercd = tdr->iic_obj->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD); /* Open IIC device in master mode */
	if (ercd == E_OPNED) {
		tdr->iic_obj->iic_control(IIC_CMD_MST_SET_SPEED_MODE, CONV2VOID(IIC_SPEED_STANDARD)); /* Set IIC master device in standard speed */
	} else if (ercd != E_OK) {
		EMBARC_PRINTF("Can't open selected IIC device\r\n");
		goto error_exit;
	}
	tdr->tar_addr = addr;
	tdr->iic_obj->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(addr)); /* Set target slave device address */

	tdr->iic_obj->iic_control(IIC_CMD_SET_TXCB, tdr_iic_txcb); /* Set IIC transmit callback function */
	tdr->iic_obj->iic_control(IIC_CMD_SET_RXCB, tdr_iic_rxcb); /* Set IIC receive callback function */
	tdr->iic_obj->iic_control(IIC_CMD_SET_ERRCB, tdr_iic_errcb); /* Set IIC error callback function */

	DEV_IIC_INFO_SET_EXTRA_OBJECT(&(tdr->iic_obj->iic_info), tdr); /* Send slave objective to tdr */
	tdr->cur_state = TDR_READY;

error_exit:
	return;
}
