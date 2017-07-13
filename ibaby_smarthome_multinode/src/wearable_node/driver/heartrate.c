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

#include "arc.h"
#include "arc_builtin.h"
#include "embARC_toolchain.h"
#include "embARC_error.h"

/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"
#include "dev_iic.h"
#include "board.h"
#include <stdio.h>

#include "heartrate.h"
#include "IIR.h"

/*! MAX30102 registers */
#define MAX30102_REG_INT_STATUS_1            0x00 /*!< Interrupt status 1 */       
#define MAX30102_REG_INT_STATUS_2            0x01 /*!< Interrupt status 2 */
#define MAX30102_REG_INT_ENABLE_1 			 0x02 /*!< Interrupt enable 1 */
#define MAX30102_REG_INT_ENABLE_2 			 0x03 /*!< Interrupt enable 2 */
#define MAX30102_REG_FIFO_WR_PTR			 0x04 /*!< FIFO write pointer */
#define MAX30102_REG_OVF_COUNTER    		 0x05 /*!< FIFO overflow counter */
#define MAX30102_REG_FIFO_RD_PTR			 0x06 /*!< FIFO read pointer */
#define MAX30102_REG_FIFO_DATA 				 0x07 /*!< FIFO data */
#define MAX30102_REG_FIFO_CONFIG			 0x08 /*!< FIFO configuration */
#define MAX30102_REG_MODE_CONFIG			 0x09 /*!< Mode configuration */
#define MAX30102_REG_SPO2_CONFIG			 0x0A /*!< SpO2 configuration */
#define MAX30102_REG_LED_PULSE_AMP_1  		 0x0C /*!< LED pulse amplitude 1 */
#define MAX30102_REG_LED_PULSE_AMP_2  		 0x0D /*!< LED pulse amplitude 2 */
#define MAX30102_REG_PM_LED_PULSE_AMP		 0x10 /*!< Proximity mode led pulse amplitude */
#define MAX30102_REG_MULTI_LED_MODE_CRTL_1	 0x11 /*!< Multi-LED mode control registers 1 */
#define MAX30102_REG_MULTI_LED_MODE_CRTL_2	 0x12 /*!< Multi-LED mode control registers 2 */
#define MAX30102_REG_DIE_TEMP_INTEGER		 0x1F /*!< Die temp integer */
#define MAX30102_REG_DIE_TEMP_FRACTION		 0x20 /*!< Die temp fraction */
#define MAX30102_REG_DIE_TEMP_CONFIG		 0x21 /*!< Die temp configuration */
#define MAX30102_REG_PM_INT_THRESHOLD  		 0x30 /*!< Proximity interrupt threshold */

/* MAX30102_REG_STATUS definition */
#define MAX30102_STATUS_PWR_RDY     	  (1)
#define MAX30102_STATUS_PROX_INT    	  (1 << 4)
#define MAX30102_STATUS_ALC_OVF     	  (1 << 5)
#define MAX30102_STATUS_PPG_RDY        	  (1 << 6)
#define MAX30102_STATUS_A_FULL      	  (1 << 7)
#define MAX30102_STATUS_DIE_TEMP_RDY      (1 << 1)


/* MAX30102_REG_CONFIG definition */
#define MAX30102_CONFIG_FIFO_A_FULL(x)    		(x & 0xf)
#define MAX30102_CONFIG_FIFO_ROL_LOVER_EN       (1 << 4)
#define MAX30102_CONFIG_SMP_AVE(x)          	((x & 0x7) << 4)
#define MAX30102_CONFIG_MODE(x)      			(x & 0x7)
#define MAX30102_CONFIG_RESET			        (1 << 6)
#define MAX30102_CONFIG_SHDN				    (1 << 7)
#define MAX30102_CONFIG_LED_PW(x)				(x & 0x3)
#define MAX30102_CONFIG_SPO2_SR(x)				((x & 0x7) << 2)
#define MAX30102_CONFIG_SPO2_ADC_REG(x)			((x & 0x3) << 5)
#define MAX30102_CONFIG_LED_PA_1(x)				(x & 0xFF)
#define MAX30102_CONFIG_LED_PA_2(x)				(x & 0xFF)
#define MAX30102_CONFIG_PM_LED_PA(x)			(x & 0xFF)



/* MAX30102_CONFIG_FIFO_A_FULL(x) options */
#define MAX30102_CONFIG_FIFO_A_0_FULL     0      //EMPTY DATA SAMPLES IN FIFO:0 / UNREAD:32  
#define MAX30102_CONFIG_FIFO_A_1_FULL     1		 //EMPTY DATA SAMPLES IN FIFO:1 / UNREAD:31
#define MAX30102_CONFIG_FIFO_A_2_FULL     2		 //EMPTY DATA SAMPLES IN FIFO:2 / UNREAD:30
#define MAX30102_CONFIG_FIFO_A_3_FULL     3		 //EMPTY DATA SAMPLES IN FIFO:3 / UNREAD:29
//define  ...									 //...
#define MAX30102_CONFIG_FIFO_A_F_FULL     15	 //EMPTY DATA SAMPLES IN FIFO:15 / UNREAD:17


/* MAX30102_CONFIG_SMP_AVE(x) options */
#define MAX30102_CONFIG_SMP_1_AVE   0     //NO 1.of samples averaged per FIFO sample
#define MAX30102_CONFIG_SMP_2_AVE   1	  //NO 2.of samples averaged per FIFO sample
#define MAX30102_CONFIG_SMP_4_AVE   2	  //NO 4.of samples averaged per FIFO sample
#define MAX30102_CONFIG_SMP_8_AVE   3	  //NO 8.of samples averaged per FIFO sample
#define MAX30102_CONFIG_SMP_16_AVE  4	  //NO 16.of samples averaged per FIFO sample
#define MAX30102_CONFIG_SMP_32_AVE  5	  //NO 32.of samples averaged per FIFO sample

/* MAX30102_CONFIG_MODE(x) options */
#define MAX30102_CONFIG_HR_MODE         2 //Heart Rate mode  LED CHANNELS:Red only
#define MAX30102_CONFIG_SPO2_MODE       3 //SpO2 mode        LED CHANNELS:Red and IR
#define MAX30102_CONFIG_MUTLI_LED_MODE  7 //Multi-LED mode   LED CHANNELS:Red and IR

/* MAX30102_CONFIG_LED_PW(x) options */
#define MAX30102_CONFIG_LED_69_PW       0 //ADC resolution(bits):15
#define MAX30102_CONFIG_LED_118_PW      1 //ADC resolution(bits):16
#define MAX30102_CONFIG_LED_215_PW  	2 //ADC resolution(bits):17
#define MAX30102_CONFIG_LED_411_PW  	3 //ADC resolution(bits):18

/* MAX30102_CONFIG_SPO2_SR(x) options */
#define MAX30102_CONFIG_SPO2_50_SR       0 //SAMPLE PER SECOND:50
#define MAX30102_CONFIG_SPO2_100_SR      1 //SAMPLE PER SECOND:100
#define MAX30102_CONFIG_SPO2_200_SR  	 2 //SAMPLE PER SECOND:200
#define MAX30102_CONFIG_SPO2_400_SR  	 3 //SAMPLE PER SECOND:400
#define MAX30102_CONFIG_SPO2_800_SR  	 4 //SAMPLE PER SECOND:800							   //...
#define MAX30102_CONFIG_SPO2_1000_SR  	 5 //SAMPLE PER SECOND:1000						   //...
#define MAX30102_CONFIG_SPO2_1600_SR  	 6 //SAMPLE PER SECOND:1600
#define MAX30102_CONFIG_SPO2_3200_SR  	 7 //SAMPLE PER SECOND:3200

/* MAX30102_CONFIG_SPO2_ADC_REG(x) options */
#define MAX30102_CONFIG_SPO2_ADC_0_REG      0 //ADC LSB SIZE:7.81  FULL SCALE:2048
#define MAX30102_CONFIG_SPO2_ADC_1_REG      1 //ADC LSB SIZE:15.63 FULL SCALE:4096
#define MAX30102_CONFIG_SPO2_ADC_2_REG  	2 //ADC LSB SIZE:31.25 FULL SCALE:8192
#define MAX30102_CONFIG_SPO2_ADC_3_REG  	3 //ADC LSB SIZE:62.5  FULL SCALE:16384


static DEV_IIC *emsk_heart_rate_sensor;
static uint32_t heart_rate_sensor_slvaddr;

#define EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)

#define MAX30102_DRIVER_FILTER  1

/*  	store register data 	*/
union _fifo_data
{
    uint8_t buf[3];
    struct
    {
    	uint8_t red_h, red_m, red_l;
    };
} fifo_data;

/* 	 configure fifo register,using the auto increase function	*/
uint8_t hrate_init_fifo_seq[] = {
        MAX30102_REG_FIFO_WR_PTR,
        0x00,
        0x00,
        0x00
};

/* 	 configure related register,using the auto increase function	*/
uint8_t hrate_init_seq[] = {
		MAX30102_REG_FIFO_CONFIG,
        MAX30102_CONFIG_FIFO_A_FULL(15) | MAX30102_CONFIG_SMP_AVE(0), //0x08 A_FULL int:0 average:32 
        MAX30102_CONFIG_MODE(2), // 0x09: Heart rate mode
        MAX30102_CONFIG_SPO2_ADC_REG(2) | MAX30102_CONFIG_LED_PW(3) | MAX30102_CONFIG_SPO2_SR(0), 
		// 0x0A: ADC resolution 16 bits, sps 50
        0x00,  // 0x0B: reserved
		0x28   //0x0C: LED current 6.4mA
};

/* 	 configure interrupt register,using the auto increase function	*/
uint8_t hrate_int_enable[] = {
	MAX30102_REG_INT_ENABLE_1,
	MAX30102_STATUS_PPG_RDY | MAX30102_STATUS_A_FULL,
	0x00,

};

#if MAX30102_DRIVER_FILTER

/* 	 
	 2th IIR filter,sps:50,
	 lowpass filter cutoff frequence:2Hz,
	 highpass filter cutoff frequence:0.2Hz, 	
*/
float iir1num[3] = {0.013359f, 0.026718f, 0.013359f};
float iir1den[2] = {-1.647460f, 0.700897f};
float iir2num[3] = {0.982385f, -1.964770f, 0.982385f};
float iir2den[2] = {-1.964461f, 0.965081f};
IirParams iirs1 = {2, iir1num, iir1den};
IirParams iirs2 = {2, iir2num, iir2den};

float band_pass(float in)
{
	static float zs1[3]={0.f},zs2[3]={0.f};
	float lout,hout;
	lout = IirTick(&iirs1,zs1,in);
	hout = IirTick(&iirs2,zs2,lout);
	return hout;
}
#endif
/**
 * \brief	write max30102 register
 * \param 	*seq 	register address and value to be written
 * \param 	len     the length of seq
 * \retval	>=0	write success, return bytes written
 * \retval	!E_OK	write failed
 */
int32_t max30102_reg_write(uint8_t* seq, uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_heart_rate_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(emsk_heart_rate_sensor!=NULL);

	/** make sure set the temp sensor's slave address */
	emsk_heart_rate_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(heart_rate_sensor_slvaddr));

	ercd = emsk_heart_rate_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_heart_rate_sensor->iic_write(seq, len);

error_exit:
	return ercd;
}

/**
 * \brief	read max30102 register
 * \param 	seq 	regaddr register address
 * \param 	*val 	returned value
 * \param   len     the length of returned value
 * \retval	>=0	read success, return bytes read
 * \retval	!E_OK	read failed
 */
int32_t max30102_reg_read(uint8_t seq,uint8_t* val,uint8_t len)
{
	int32_t ercd = E_PAR;

	emsk_heart_rate_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(emsk_heart_rate_sensor!=NULL);

	/** make sure set the temp sensor's slave address */
	emsk_heart_rate_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(heart_rate_sensor_slvaddr));
	/* write register address then read register value */
	ercd = emsk_heart_rate_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
	ercd = emsk_heart_rate_sensor->iic_write(&seq, 1);
	emsk_heart_rate_sensor->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = emsk_heart_rate_sensor->iic_read(val, len);
	
error_exit:
	return ercd;
}

/**
 * \brief	max30102 sensor init
 * \param[in]slv_addr	imu sensor iic slave address
 * \retval	E_OK	init success
 * \retval	!E_OK	init failed
 */
int32_t hrate_sensor_init(uint32_t slv_addr)
{
	int32_t ercd = E_OK;
	emsk_heart_rate_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(emsk_heart_rate_sensor!=NULL);

	if ((ercd == E_OK) || (ercd == E_OPNED)) {
		ercd = emsk_heart_rate_sensor->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));
		heart_rate_sensor_slvaddr = slv_addr;
		
		// /******set max30102 reg ******/
		max30102_reg_write(hrate_int_enable, 3);
		max30102_reg_write(hrate_init_fifo_seq, 4);
		max30102_reg_write(hrate_init_seq, 6);

	}

error_exit:
	return ercd;
}

/** val is the return light intensity */
/**
 * \brief read max30102 sensor value
 * \param[out] val 	return max30102
 * \retval E_OK  	read success
 * \retval !E_OK 	read failed
 */ 
int data_num = 0, data_rdy = 0;

int32_t hrate_sensor_read(int* hrate)
{
	int32_t ercd = E_OK;
	char str[20];

	//int_rdy 	the interrupt flag to determine whether to read data
	uint8_t int_rdy = 0;

	emsk_heart_rate_sensor = iic_get_dev(BOARD_TEMP_SENSOR_IIC_ID);

	EMSK_HEART_RATE_SENSOR_CHECK_EXP_NORTN(emsk_heart_rate_sensor!=NULL);

	//get the interrupt flag
	max30102_reg_read(MAX30102_REG_INT_STATUS_1, &int_rdy, 1);

	//MAX30102_STATUS_PPG_RDY: New FIFO Data Ready 
	if(int_rdy & MAX30102_STATUS_PPG_RDY)
	{	
		ercd = max30102_reg_read(MAX30102_REG_FIFO_DATA, fifo_data.buf, 3);
	
		if(ercd != 3) {
			ercd = E_OBJ;
		} else {
			ercd = E_OK;
			*hrate = ((fifo_data.red_h&&0x3)<<16) | (fifo_data.red_m<<8) | fifo_data.red_l;
			data_num++;
			data_rdy = 1;
		}
		int_rdy = 0;
	}
	
error_exit:
	return ercd;
}

/** @} end of group EMBARC_APP_FREERTOS_IBABY_SMARTHOME_NODES_WEARABLE_NODE_DRIVER */