#include "mic.h"

/***********    mic operation     ********/
static DEV_IIC *adc_iic_port;
/**
 * \brief  Adc value get of mic
 * \details Get the adc value of mic using iic0.  
 * \param[out] the value of adc convert  
 */
uint8_t adc_get(void)
{
	uint8_t REG_data;
	adc_iic_port -> iic_read((void*) (&REG_data), 1);
	EMBARC_PRINTF("ADC_DATA:%d\r\n",REG_data);
	return REG_data;
}
/**
 * \brief  Initial the mic 
 * \details Initial the mic using iic0.
 */
void mic_init(void)
{
	uint8_t point ;
	point = (0x90 >> 1);
	adc_iic_port = iic_get_dev(DW_IIC_0_ID);
	// adc_iic_port -> iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	// adc_iic_port -> iic_control(IIC_CMD_ENA_DEV, (void *) point);
	// adc_iic_port -> iic_control(IIC_CMD_MST_SET_TAR_ADDR, (void *) point);
	// point = IIC_MODE_STOP;
	// adc_iic_port -> iic_control(IIC_CMD_MST_SET_NEXT_COND, (void *) point);
	// point = 0x40;
	// adc_iic_port -> iic_write((void*) point,1);
	//adc_get();
}
/**
 * \brief  IIC address point to mic 
 * \details IIC address point to mic.
 */
void iic_point_mic(void)
{
	uint8_t point = (0x90 >> 1);
	if (adc_iic_port -> iic_control(IIC_CMD_MST_SET_TAR_ADDR, (void *) point) == E_OK) {
		EMBARC_PRINTF("Target reset mic OK!\n");
	}
}
