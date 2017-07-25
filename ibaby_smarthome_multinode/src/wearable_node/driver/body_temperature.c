/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * \version 2017.06
 * \date 2017-06-26
 * \author dbHu(wwmhu@outlook.com)
--------------------------------------------- */

/**
 * \defgroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER        embARC iBaby Smarthome Node Wearable Node Driver
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief	emsk temperature sensor driver for ibaby wearable node
 * \details
 *		realize driver for temperature sensor using designware iic driver
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief	emsk temperature sensor driver for ibaby wearable node
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * @{
 */

/* standard C HAL */
#include <stdio.h>

/* embARC HAL */
#include "arc.h"
#include "arc_builtin.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "dev_iic.h"
#include "board.h"

#include "embARC.h"
#include "embARC_debug.h"
#include "dev_iic.h"
#include "board.h"

/* custom HAL */
#include "body_temperature.h"


/*!< tmp112 registers */
#define TMP112_REG_TMP          0x00 /*!< Temperature Register */
#define TMP112_REG_CONG         0x01 /*!< Configuration Register */
#define TMP112_REG_TLOW         0x02 /*!< T Low Register */
#define TMP112_REG_THIGH        0x03 /*!< T High Register */

/*!< ADT7420_REG_CONFIG definition */
#define TMP112_CONFIG_OS     (1 << 7)
#define TMP112_CONFIG_CC(x)  ((x & 0x3) << 5)
#define TMP112_CONFIG_SD     (1 << 0)

/*!< body temperature data received buffer */
union _btemp_data
{
	uint8_t buf[2];
	struct {
		uint8_t btemp_h, btemp_l;
	};
} btemp_data;

/*!< configure oneshot mode */
static uint8_t tmp_oneshot_enable[] = {
	TMP112_REG_CONG,
	TMP112_CONFIG_OS,
	0x00
};

/*!< configure shutdown mode */
static uint8_t tmp_shutdown_enable[] = {
	TMP112_REG_CONG,
	TMP112_CONFIG_SD,
	0x00
};

static const float btemp_unit = 0.0625; /*!< convert to ℃ */

static DEV_IIC  *emsk_tmp_sensor;  /*!< TMP112 sensor object */
static uint32_t btemp_sensor_addr; /*!< variable of body temperature sensor address */


/**
 * \brief	write tmp112 register
 * \param 	*seq 	register address and value to be written
 * \param 	len     the length of seq
 * \retval	>=0	write success, return bytes written
 * \retval	!E_OK	write failed
 */
static int32_t tmp112_reg_write(uint8_t *seq, uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_tmp_sensor = iic_get_dev(BTEMP_SENSOR_IIC_ID);

	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(emsk_tmp_sensor != NULL);

	/* make sure set the temp sensor's slave address */
	emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(btemp_sensor_addr));

	ercd = emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_tmp_sensor->iic_write(seq, len);

error_exit:
	return ercd;
}

/**
 * \brief	read tmp112 register
 * \param 	seq		register address
 * \param 	*val    returned value
 * \param 	len     the length of returned value
 * \retval	>=0	read success, return bytes read
 * \retval	!E_OK	read failed
 */
static int32_t tmp112_reg_read(uint8_t seq, uint8_t *val, uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_tmp_sensor = iic_get_dev(BTEMP_SENSOR_IIC_ID);

	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(emsk_tmp_sensor != NULL);

	/* make sure set the temp sensor's slave address */
	emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(btemp_sensor_addr));
	/* write register address then read register value */
	ercd = emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));

	ercd = emsk_tmp_sensor->iic_write(&seq, 1);
	emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_tmp_sensor->iic_read(val, len);

error_exit:
	return ercd;
}


/**
 * \brief	body temperature sensor initialize
 * \param[in]	slv_addr  body temperature sensor iic slave address
 * \retval	E_OK	initialize success
 * \retval	!E_OK	initialize failed
 */
extern int32_t btemp_sensor_init(uint32_t slv_addr)
{
	int32_t ercd = E_OK;

	emsk_tmp_sensor = iic_get_dev(BTEMP_SENSOR_IIC_ID);

	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(emsk_tmp_sensor != NULL);

	/* the device working as master */
	ercd = emsk_tmp_sensor->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));
		btemp_sensor_addr = slv_addr;

		/* write value to tmp112 to set registers */
		tmp112_reg_write(tmp_shutdown_enable, 3);
	}

error_exit:
	return ercd;
}

/** val is the return temperature in degrees Celsius (1/10 C) */
/**
 * \brief	read temperature sensor value in 0.1c unit
 * \param[out]	val	return temperature in degrees Celsius (1/10 C)
 * \retval	E_OK	read success
 * \retval	!E_OK	read failed
 */
extern int32_t btemp_sensor_read(uint16_t *btemp)
{
	int32_t ercd = E_OK;

	emsk_tmp_sensor = iic_get_dev(BTEMP_SENSOR_IIC_ID);

	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(emsk_tmp_sensor != NULL);
	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(btemp != NULL);

	ercd = tmp112_reg_write(tmp_oneshot_enable, 3);

	/* read 5 data from tmp112 */
	ercd += tmp112_reg_read(TMP112_REG_TMP, btemp_data.buf, 2);

	if (ercd != 5) {
		ercd = E_OBJ;
	} else {
		ercd = E_OK;
		*btemp = ((btemp_data.btemp_h << 4) | (btemp_data.btemp_l >> 4) ) * btemp_unit * 10 + 2;
	}

error_exit:
	return ercd;
}

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */