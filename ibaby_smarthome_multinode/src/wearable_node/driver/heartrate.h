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
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * \brief	heartrate sensor driver related header file
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER
 * @{
 */
#ifndef _HEARTRATE_H_
#define _HEARTRATE_H_

/*!< using IIC 1 interface */
#define HRATE_SENSOR_IIC_ID                  DW_IIC_1_ID

/*!< MAX30102 registers */
#define MAX30102_REG_INT_STATUS_1            0x00 /*!< Interrupt status 1 */
#define MAX30102_REG_INT_ENABLE_1            0x02 /*!< Interrupt enable 1 */
#define MAX30102_REG_FIFO_WR_PTR             0x04 /*!< FIFO write pointer */
#define MAX30102_REG_FIFO_DATA               0x07 /*!< FIFO data */
#define MAX30102_REG_FIFO_CONFIG             0x08 /*!< FIFO configuration */

/*!< MAX30102_REG_STATUS definition */
#define MAX30102_STATUS_PPG_RDY              (1 << 6)
#define MAX30102_STATUS_A_FULL               (1 << 7)

/*!< MAX30102_REG_CONFIG definition */
#define MAX30102_CONFIG_FIFO_A_FULL(x)       (x & 0xf)
#define MAX30102_CONFIG_SMP_AVE(x)           ((x & 0x7) << 4)
#define MAX30102_CONFIG_MODE(x)              (x & 0x7)
#define MAX30102_CONFIG_LED_PW(x)            (x & 0x3)
#define MAX30102_CONFIG_SPO2_SR(x)           ((x & 0x7) << 2)
#define MAX30102_CONFIG_SPO2_ADC_REG(x)      ((x & 0x3) << 5)

/*!< max30102 address */
#define MAX30102_ADDRESS                     0x57
/*!< heartrate sensor address */
#define HRATE_SENSOR_ADDR                    MAX30102_ADDRESS

#define EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(EXPR)    CHECK_EXP_NOERCD(EXPR, error_exit)


typedef struct __iir_par
{
	short order;
	float *num;
	float *den;
} iir_par;

union _hrate_data
{
	uint8_t buf[3];
	struct {
		uint8_t red_h, red_m, red_l;
	};
} hrate_data;

/*!< configure fifo register,using the auto increase function */
static uint8_t hrate_init_fifo_seq[] = {
	MAX30102_REG_FIFO_WR_PTR,
	0x00,
	0x00,
	0x00
};

/*!< configure related register,using the auto increase function */
static uint8_t hrate_init_seq[] = {
	MAX30102_REG_FIFO_CONFIG,
	MAX30102_CONFIG_FIFO_A_FULL(15) | MAX30102_CONFIG_SMP_AVE(0), /*!< 0x08 A_FULL int:0 average:32 */
	MAX30102_CONFIG_MODE(2), /*!< 0x09: Heart rate mode */
	MAX30102_CONFIG_SPO2_ADC_REG(2) | MAX30102_CONFIG_LED_PW(3) | MAX30102_CONFIG_SPO2_SR(0), 
	/*!< 0x0A: ADC resolution 16 bits, sps 50 */
	0x00,  /*!< 0x0B: reserved */
	0x28   /*!< 0x0C: LED current 6.4mA */
};

/*!< configure interrupt register,using the auto increase function */
static uint8_t hrate_int_enable[] = {
	MAX30102_REG_INT_ENABLE_1,
	MAX30102_STATUS_PPG_RDY | MAX30102_STATUS_A_FULL,
	0x00,
};

static float iir1num[3] = {0.013359f, 0.026718f, 0.013359f};
static float iir1den[2] = {-1.647460f, 0.700897f};
static float iir2num[3] = {0.982385f, -1.964770f, 0.982385f};
static float iir2den[2] = {-1.964461f, 0.965081f};

static iir_par iirs1 = {2, iir1num, iir1den};
static iir_par iirs2 = {2, iir2num, iir2den};

static float zs1[3]={0.f}, zs2[3]={0.f};

static DEV_IIC  *emsk_max_sensor;  /*!< MAX30102 sensor object */
static uint32_t hrate_sensor_addr; /*!< variable of heartrate sensor address */

int data_num;
bool flag_data_ready;

static float   iir_tick(iir_par *, float *, float);

/* function for band-pass filter */
extern float   band_pass_filter(float in);

/* function for heartrate sensor initialize */
extern int32_t hrate_sensor_init(uint32_t slv_addr);

/* function for reading heartrate sensor data */
extern int32_t hrate_sensor_read(int* heart_rate);

#endif /* _WEARABLE_NODE_DRIVER_HEARTRATE_H_ */

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */