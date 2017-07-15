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
 * \brief	emsk heartrate sensor driver for ibaby wearable node
 * \details
 *		realize driver for heartrate sensor using designware iic driver
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief	emsk heartrate sensor driver for ibaby wearable node
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

#include "embARC.h"
#include "embARC_debug.h"
#include "dev_iic.h"
#include "board.h"

/* custom HAL */
#include "heartrate.h"


/* 	 
 * 2th IIR filter, sps:50,
 * lowpass  filter cutoff frequence: 2Hz,
 * highpass filter cutoff frequence: 0.2Hz, 	
 */
static float iir_tick(iir_par *par, float *zs, float in)
{
    int i;
    float out;

    out = zs[0] + in * par->num[0];

    for(i = 0; i < par->order - 1; i++) {
        zs[i] = zs[i + 1] + (in * par->num[i + 1])
        	- (out * par->den[i]);
    }

    zs[i] = (in * par->num[i + 1]) - (out * par->den[i]);

    return out;
}

/* filter function for band pass */
extern float band_pass_filter(float in)
{
	float lout, hout;

	lout = iir_tick(&iirs1, zs1, in);
	hout = iir_tick(&iirs2, zs2, lout);

	return hout;
}


/**
 * \brief	write max30102 register
 * \param 	*seq 	register address and value to be written
 * \param 	len     the length of seq
 * \retval	>=0	write success, return bytes written
 * \retval	!E_OK	write failed
 */
static int32_t max30102_reg_write(uint8_t* seq, uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_max_sensor = iic_get_dev(HRATE_SENSOR_IIC_ID);

	EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(emsk_max_sensor != NULL);

	/* make sure set the max sensor's slave address */
	emsk_max_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(hrate_sensor_addr));

	ercd = emsk_max_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_max_sensor->iic_write(seq, len);

error_exit:
	return ercd;
}

/**
 * \brief	read max30102 register
 * \param 	seq 	regaddr register address
 * \param 	*val 	returned value
 * \param       len     the length of returned value
 * \retval	>=0	read success, return bytes read
 * \retval	!E_OK	read failed
 */
static int32_t max30102_reg_read(uint8_t seq, uint8_t* val, uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_max_sensor = iic_get_dev(HRATE_SENSOR_IIC_ID);

	EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(emsk_max_sensor != NULL);

	/* make sure set the tmp sensor's slave address */
	emsk_max_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(hrate_sensor_addr));
	/* write register address then read register value */
	ercd = emsk_max_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));

	ercd = emsk_max_sensor->iic_write(&seq, 1);

	emsk_max_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	/* read len data from max30102 */
	ercd = emsk_max_sensor->iic_read(val, len);
	
error_exit:
	return ercd;
}

/**
 * \brief	heartrate sensor initialize
 * \param[in]   slv_addr  heartrate sensor iic slave address
 * \retval	E_OK	initialize success
 * \retval	!E_OK	initialize failed
 */
extern int32_t hrate_sensor_init(uint32_t slv_addr)
{
	int32_t ercd = E_OK;

	emsk_max_sensor = iic_get_dev(HRATE_SENSOR_IIC_ID);

	EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(emsk_max_sensor != NULL);

	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = emsk_max_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));
		hrate_sensor_addr = slv_addr;
		
		/* write value to max30102 to set registers */
		max30102_reg_write(hrate_int_enable, 3);
		max30102_reg_write(hrate_init_fifo_seq, 4);
		max30102_reg_write(hrate_init_seq, 6);
	}

error_exit:
	return ercd;
}

/** val is the return heartrate sensor data */
/**
 * \brief read heartrate sensor data
 * \param[out] val 	return heartrate sensor data
 * \retval E_OK  	read success
 * \retval !E_OK 	read failed
 */
extern int32_t hrate_sensor_read(int* hrate)
{
	int32_t ercd = E_OK;

	/* int_rdy    the interrupt flag to determine whether to read data */
	uint8_t int_rdy = 0;

	emsk_max_sensor = iic_get_dev(HRATE_SENSOR_IIC_ID);

	EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(emsk_max_sensor != NULL);

	/* get the interrupt flag */
	max30102_reg_read(MAX30102_REG_INT_STATUS_1, &int_rdy, 1);

	/* MAX30102_STATUS_PPG_RDY: New FIFO Data Ready */
	if(int_rdy & MAX30102_STATUS_PPG_RDY)
	{
		/* read 3 data from max30102 */
		ercd = max30102_reg_read(MAX30102_REG_FIFO_DATA, hrate_data.buf, 3);
	
		if(ercd != 3) {
			ercd = E_OBJ;
		} else {
			ercd = E_OK;
			*hrate = ((hrate_data.red_h && 0x3) << 16) |
				(hrate_data.red_m << 8) | hrate_data.red_l;
			data_num++;
			data_rdy = 1;
		}
		int_rdy = 0;
	}
	
error_exit:
	return ercd;
}

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */