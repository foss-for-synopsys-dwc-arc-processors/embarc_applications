 /*------------------------------------------
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
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief	temperature sensor driver related header file
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * @{
 */
#ifndef _BODY_TEMPERATURE_H_
#define _BODY_TEMPERATURE_H_

/*!< using IIC 1 interface */
#define BTEMP_SENSOR_IIC_ID     DW_IIC_1_ID

/*!< tmp112 registers */
#define TMP112_REG_TMP          0x00 /*!< Temperature Register */
#define TMP112_REG_CONG         0x01 /*!< Configuration Register */

/*!< ADT7420_REG_CONFIG definition */
#define TMP112_CONFIG_OS        (1 << 7)
#define TMP112_CONFIG_SD        (1 << 0)

/*!< TMP112 address */
#define TMP112_ADDRESS          0x48
/*!< body temperature sensor address */
#define BTEMP_SENSOR_ADDR       TMP112_ADDRESS

#define EMSK_TMP_SENSOR_CHECK_EXP_NORTN(EXPR)    CHECK_EXP_NOERCD(EXPR, error_exit)


/*!< store register data */
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


/* function for body temperature sensor initialize */
extern int32_t btemp_sensor_init(uint32_t slv_addr);

/* function for reading body temperature sensor data */
extern int32_t btemp_sensor_read(uint32_t *btmp);

#endif /* _WEARABLE_NODE_DRIVER_BODY_TEMPERATURE_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */