/* ----------------------------------------------
 * writer:WangShuai
 * version 3.0
 * date 2017-6-20
 * \brief	Light control drivers.
 * \details
 		Some functions in this file are used to initialize the GPIO which is used to control the lamp beats.
	The other functions are encapsulated bottom light drivers.
 * \file light
*/

#include "light_driver.h"

uint8_t color_row;
uint8_t light_num;
/*
 *Light control array definition.
 *Middle row of the lamp beads.
 */
static RGB_T ilight_middle[LIGHT_NUM_MIDDLE];
/*
 *Light control array definition.
 *Both sides of the lamp beads.
 */
static RGB_T ilight_side[LIGHT_NUM_SIDE];
static RGB_T *piLight;

/*
 *Some variables definition related gpio initialization .
 *Middle row of lights.
 */
static DEV_GPIO_PTR port_gpio_middle;
static DW_GPIO_PORT_PTR port_middle;
static DEV_GPIO_INFO_PTR port_info_ptr_middle;
/*
 *Some variables definition related gpio initialization .
 *Middle row of lights.
 */
static DEV_GPIO_PTR port_gpio_side;
static DW_GPIO_PORT_PTR port_side;
static DEV_GPIO_INFO_PTR port_info_ptr_side;

/*
 * \brief Initialize light control gpio.
 * \details 2 pins will be initialized in this function,and output low after initialization.
*/
void led_ctr_gpio_init()
{
	uint32_t val_middle,val_side;

	port_gpio_middle = gpio_get_dev(GPIO_OUT_PORT_MIDDLE);			
	port_gpio_middle->gpio_open(GPIO_OUT_MASK_MIDDLE);

	port_gpio_side = gpio_get_dev(GPIO_OUT_PORT_SIDE);			
	port_gpio_side->gpio_open(GPIO_OUT_MASK_SIDE);

	port_gpio_middle->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_OUT_MASK_MIDDLE));
	port_gpio_middle->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_OUT_MASK_MIDDLE));

	port_gpio_side->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_OUT_MASK_SIDE));
	port_gpio_side->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_OUT_MASK_SIDE));

	port_info_ptr_middle =& (port_gpio_middle->gpio_info);	
	port_middle = (DW_GPIO_PORT_PTR)(port_info_ptr_middle->gpio_ctrl);

	port_info_ptr_side =& (port_gpio_side->gpio_info);	
	port_side = (DW_GPIO_PORT_PTR)(port_info_ptr_side->gpio_ctrl);
	
	low_val_middle &= GPIO_H_CFG_MIDDLE;
	low_val_middle |= low_val_middle;

	low_val_side &= GPIO_H_CFG_SIDE;
	low_val_side |= low_val_side;

	val_middle = GPIO_H_CFG_MIDDLE;
	temp_reg_middle = port_middle->regs->SWPORTS[port_middle->no].DR;
	temp_reg_middle &= ~GPIO_H_CFG_MIDDLE;
	val_middle &= GPIO_H_CFG_MIDDLE;
	temp_reg_middle |= val_middle;

	val_side = GPIO_H_CFG_SIDE;
	temp_reg_side = port_side->regs->SWPORTS[port_side->no].DR;
	temp_reg_side &= ~GPIO_H_CFG_SIDE;
	val_side &= GPIO_H_CFG_SIDE;
	temp_reg_side |= val_side;

	port_middle->regs->SWPORTS[port_middle->no].DR=low_val_middle;
	port_side->regs->SWPORTS[port_side->no].DR=low_val_side;		
}

/*
 * \brief The light control function sends one bit data,0.
 * \details The duration of the high level is 0.35us,the error is 150ns.The duration of the low level is 0.8us,the error is 150ns.
 * \parameter[in] row.Indicates which pin is going to send data.If it is not LIGHT_ROW_MIDDLE,it will be LIGHT_SIDE by default.
 */
void light_send_bit_0(uint16_t row)												
{
	uint8_t j;
	uint32_t value_high,value_low;
	DW_GPIO_PORT_PTR row_send;
	if (row == LIGHT_ROW_MIDDLE) {
		row_send = port_middle;
		value_high = temp_reg_middle;
		value_low = low_val_middle;
	} else {
			row_send = port_side;
			value_high = temp_reg_side;
			value_low = low_val_side;
	}
	/*Raise the pin level*/
	row_send->regs->SWPORTS[row_send->no].DR = value_high;
	/*Delay for 0.35us.*/
	for (j = 0;j<6;j++) {
		asm("nop");
	}
	asm("nop");
	//asm("nop");
	/*Pull the pin low.*/
	row_send->regs->SWPORTS[row_send->no].DR=value_low;
	/*Delay for 0.8us.*/
	//for (j = 0;j<3;j++) {
	//	asm("nop");
	//}
}
void light_ctr_reset()
{
	port_side->regs->SWPORTS[port_side->no].DR=low_val_side;
	delay_10us(50);
}
/*
 * \brief The light control function sends one bit data,1.
 * \details The duration of the high level is 0.7us,the error is 150ns.The duration of the low level is 0.6us,the error is 150ns.
 * \parameter[in] row.Indicates which pin is going to send data.If it is not LIGHT_ROW_MIDDLE,it will be LIGHT_SIDE by default.
*/
void light_send_bit_1(uint16_t row)
{
	uint8_t j;
	uint32_t value_high,value_low;
	DW_GPIO_PORT_PTR row_send;
	if (row == LIGHT_ROW_MIDDLE) {
		row_send = port_middle;
		value_high = temp_reg_middle;
		value_low = low_val_middle;
	} else {
			row_send = port_side;
			value_high = temp_reg_side;
			value_low = low_val_side;
		   }
	row_send->regs->SWPORTS[row_send->no].DR = value_high;
	for (j=0; j<6; j++) {
		asm("nop");
	}
	for (j=0; j<6; j++) {
		asm("nop");
	}
	for (j=0; j<2; j++) {
		asm("nop");
	}
	asm("nop");
	row_send->regs->SWPORTS[row_send->no].DR = value_low;
}

/*
 * \brief Send 24 bits data to control one light.
 * \details According to parameter send 24 bits color data to light one beat.
 * \parameter[in] color_data.The low 24 bits of the parameter is effective.
 * \parameter[in] row.Indicates which pin is going to send data.If it is not LIGHT_ROW_MIDDLE,it will be LIGHT_SIDE by default.
 */
void light_send_colordata(uint32_t color_data,uint16_t row)
{
	int8_t i;
	for (i = 23; i>=0; i--) {
		if ((color_data>>i)&0x01) {
			light_send_bit_1(row);
		} else {
			light_send_bit_0(row);
		}
	}
}
/*
 * \brief Delay 10 us.
 * \details Implement the delay effect by executing empty statement.
 * parameter[in] mul.One unit represents 10 us.
*/
void delay_10us(uint32_t mul)
{
	for (delay_num = 0; delay_num<25*mul; delay_num++) {
		
	}
}

/*
 * \brief Lighting control function.
 * \details Update specify arrays and send those arrays to control lamp beats. 
 * \parameter[in] mask.Those bits which are setted one mark those lamp beats that are going to be changed.
 * \parameter[in] row.Indicates which pin is going to send data.If it is not LIGHT_ROW_MIDDLE,it will be LIGHT_SIDE by default.
 * \parameter[in] rgb.The data of clolor.
*/
void light_ctr_rgb(uint64_t mask,uint16_t row,uint8_t red,uint8_t green,uint8_t blue)
{
	uint8_t i;
	uint32_t colordata = 0;
	color_row=row;
	RGB_T *ilight_color;
	if (row == LIGHT_ROW_MIDDLE) {
		light_num = LIGHT_NUM_MIDDLE;
		ilight_color = &ilight_middle[0];
		piLight = ilight_middle;
	} else {
		light_num = LIGHT_NUM_SIDE;
		ilight_color = &ilight_side[0];
		piLight = ilight_side;
	}
	/*In order to avoid the shift operation is optimized,set the 64 bit one.*/
	/** Place the data of RGB colors into a 32-bit variable*/
	colordata |= green;
	colordata = (colordata<<8)|red;
	colordata = (colordata<<8)|blue;
	/**Update the array element data*/
	for (i = 0; i < light_num; i++) {
		if ((mask>>i)&0x01) {
			ilight_color[i].color = colordata;
		}
	}
	xQueueSendToFront( led_queue, ( void * ) &piLight, ( TickType_t ) 0 );
	delay_10us(5);
}

/*
 * \brief Perform a left shift operation on the parameter.
 * \details Because the translater will set those bits which is higher than 32 high,so if you want to avoid this bug you have to set at least one bit high wich is higher than 32. 
 * \parameter[in] aim_num.The number which you want to perform.
 * \parameter[in] shift num.How many bits you want to shift.
*/
uint64_t light_ctr_mask_lshift(uint64_t aim_num,uint64_t shift_num)
{
	aim_num |= 0x1000000000000000;
	return aim_num = aim_num<<shift_num;
}

/*
 * \brief Lighting control function.
 * \details Update specify arrays and send those arrays to control lamp beats. 
 * \parameter[in] mask.Those bits which are setted one mark those lamp beats that are going to be changed.
 * \parameter[in] row.Indicates which pin is going to send data.If it is not LIGHT_ROW_MIDDLE,it will be LIGHT_SIDE by default.
 * \parameter[in] rgb.The data of clolor.
*/
void light_ctr_rgb_update(uint64_t mask,uint16_t row,uint8_t red,uint8_t green,uint8_t blue)
{
	uint8_t i;
	uint32_t colordata = 0;
	RGB_T *ilight_color;
	if (row == LIGHT_ROW_MIDDLE) {
		light_num = LIGHT_NUM_MIDDLE;
		ilight_color = &ilight_middle[0];
	} else {
		light_num = LIGHT_NUM_SIDE;
		ilight_color = &ilight_side[0];
	}
	/*In order to avoid the shift operation is optimized,set the 64 bit one.*/
	colordata |= green;
	colordata = (colordata<<8)|red;
	colordata = (colordata<<8)|blue;
	/**Update the array element data*/
	for (i = 0;i < light_num;i++) {
		if ((mask>>i)&0x01) {
			ilight_color[i].color = colordata;
		}
	}
}