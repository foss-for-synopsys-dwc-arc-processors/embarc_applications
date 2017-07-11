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

#include "arc.h"
#include "arc_builtin.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"

#include "dev_iic.h"
#include "tmp.h"
#include "board.h"

/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"
#include "dev_iic.h"
#include "board.h"
#include <stdio.h>

#define TMP112_DRIVER_TEST 		0
/*! tmp112 registers */
#define TMP112_REG_TMP			0x00	//Temperature Register
#define TMP112_REG_CONG			0x01	//Configuration Register
#define TMP112_REG_TLOW			0x02	//T Low Register
#define TMP112_REG_THIGH		0x03	//T High Register

/* ADT7420_REG_CONFIG definition */
#define TMP112_CONFIG_OS			   	(1 << 7)
#define TMP112_CONFIG_CC(x)           	((x & 0x3) << 5)
#define TMP112_CONFIG_SD          		(1 << 0)

static DEV_IIC *emsk_tmp_sensor;
static uint32_t tmp_sensor_slvaddr;

#define EMSK_TMP_SENSOR_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)

/*  	store register data 	*/
union _fifo_data						
{
    uint8_t buf[2];
    struct
    {
    	uint8_t tmp_h, tmp_l;
    };
} fifo_data;

/* 	 configure oneshot mode 	*/
uint8_t tmp_oneshot_enable[] = {
	TMP112_REG_CONG,
	TMP112_CONFIG_OS,
	0x00
};

/* 	 configure shutdown mode 	*/
uint8_t tmp_shutdown_enable[] = {
	TMP112_REG_CONG,
	TMP112_CONFIG_SD,
	0x00
};

/**
 * \brief	write tmp112 register
 * \param 	*seq 	register address and value to be written
 * \param 	len     the length of seq
 * \retval	>=0	write success, return bytes written
 * \retval	!E_OK	write failed
 */
int32_t tmp112_reg_write(uint8_t *seq, uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_tmp_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(emsk_tmp_sensor!=NULL);

	/** make sure set the temp sensor's slave address */
	emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(tmp_sensor_slvaddr));

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
int32_t tmp112_reg_read(uint8_t seq,uint8_t *val,uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_tmp_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(emsk_tmp_sensor!=NULL);

	/** make sure set the temp sensor's slave address */
	emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(tmp_sensor_slvaddr));
	/* write register address then read register value */
	ercd = emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	ercd = emsk_tmp_sensor->iic_write(&seq, 1);
	emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_tmp_sensor->iic_read(val, len);

error_exit:
	return ercd;
}


/**
 * \brief	temperature sensor init
 * \param[in]	slv_addr	temperature sensor iic slave address
 * \retval	E_OK	init success
 * \retval	!E_OK	init failed
 */
int32_t tmp_sensor_init(uint32_t slv_addr)
{
	int32_t ercd = E_OK;

	emsk_tmp_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(emsk_tmp_sensor!=NULL);

	ercd = emsk_tmp_sensor->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = emsk_tmp_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));
		tmp_sensor_slvaddr = slv_addr;
		
		/******set tmp112 reg ******/
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
int32_t tmp_sensor_read(uint32_t *tmp)
{
	int32_t ercd = E_OK;
	int i = 0;

	emsk_tmp_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(emsk_tmp_sensor!=NULL);
	EMSK_TMP_SENSOR_CHECK_EXP_NORTN(tmp!=NULL);
	
	ercd = tmp112_reg_write(tmp_oneshot_enable,3);

	ercd += tmp112_reg_read(TMP112_REG_TMP, fifo_data.buf, 2);
	
	if(ercd != 5) {
		ercd = E_OBJ;
	} else {
		ercd = E_OK;
		*tmp = ((fifo_data.tmp_h << 4) | (fifo_data.tmp_l >> 4) ) * tmp112_unit * 10 + 2;
	}

error_exit:
	return ercd;
}

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */