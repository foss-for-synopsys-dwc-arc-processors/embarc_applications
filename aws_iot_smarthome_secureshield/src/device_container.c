/* ------------------------------------------
 * Copyright (c) 2019, Synopsys, Inc. All rights reserved.

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
--------------------------------------------- */

#include <string.h>
#include <stdio.h>
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_assert.h"

#include "device_container.h"

#include "adt7420.h"

#ifdef BOARD_EMSK
#define PRESS_BUTTON_GPIO_PORT			EMSK_BUTTON_PORT
#else
#define PRESS_BUTTON_GPIO_PORT			DEV_GPIO_PORT_NC
#endif

#define BUTTON_USED_MASK	(0x7)

CONTAINER_DATA(device_container) \
	static ADT7420_DEFINE(temperature_sensor, BOARD_TEMP_SENSOR_IIC_ID, TEMP_I2C_SLAVE_ADDRESS);


#ifdef SIMULATE_TEMPERATURE
static void simulateRoomTemperature(float *pRoomTemperature)
{
	static float deltaChange;

	if (*pRoomTemperature >= ROOMTEMPERATURE_UPPERLIMIT) {
		deltaChange = -0.5f;
	} else if (*pRoomTemperature <= ROOMTEMPERATURE_LOWERLIMIT) {
		deltaChange = 0.5f;
	}

	*pRoomTemperature += deltaChange;
}
#endif

bool getRoomTemperature(float *pRoomTemperature)
{
#ifdef SIMULATE_TEMPERATURE
	simulateRoomTemperature(pRoomTemperature);
#else
	float cur_temp;
	adt7420_sensor_read(temperature_sensor, &cur_temp);
	if (cur_temp == (*pRoomTemperature)) {
		return false;
	} else {
		*pRoomTemperature = cur_temp;
		return true;
	}
#endif
	return true;
}

#define FRONTDOOR_LEDMASK	0x30
void controlFrontDoor(bool state)
{
	if (state) {
		EMBARC_PRINTF("FrontDoor is locked");
		led_write(FRONTDOOR_LEDMASK, FRONTDOOR_LEDMASK);
	} else {
		EMBARC_PRINTF("FrontDoor is open");
		led_write(0x0, FRONTDOOR_LEDMASK);
	}
}

#define KITCHEN_LEDMASK	0xC
void controlKitchenLights(bool state)
{
	if (state) {
		EMBARC_PRINTF("Turn on KitchenLights");
		led_write(KITCHEN_LEDMASK, KITCHEN_LEDMASK);
	} else {
		EMBARC_PRINTF("Turn off KitchenLights");
		led_write(0x0, KITCHEN_LEDMASK);
	}
}

#define LIVINGROOM_LEDMASK	0x3
void controlLivingRoomLights(bool state)
{
	if (state) {
		EMBARC_PRINTF("Turn on LivingRoomLights");
		led_write(LIVINGROOM_LEDMASK, LIVINGROOM_LEDMASK);
	} else {
		EMBARC_PRINTF("Turn off LivingRoomLights");
		led_write(0x0, LIVINGROOM_LEDMASK);
	}
}

void smarthome_init(void)
{
	DEV_GPIO_BIT_ISR bit_isr;
	DEV_GPIO_INT_CFG int_cfg;

	adt7420_sensor_init(temperature_sensor);

	DEV_GPIO_PTR port = gpio_get_dev(PRESS_BUTTON_GPIO_PORT);

	if (port == NULL) {
		EMBARC_PRINTF("No press button available on this board!\r\n");
		return;
	}
	port->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)BUTTON_USED_MASK);

	int_cfg.int_bit_mask = BUTTON_USED_MASK;
	int_cfg.int_bit_type = BUTTON_USED_MASK; /* edge trigger */
	int_cfg.int_bit_polarity = 0x0; /* falling sensitive */
	int_cfg.int_bit_debounce = BUTTON_USED_MASK; /* with debounce */

	port->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);

	/* KitchenLights button */
	bit_isr.int_bit_ofs = 0;
	bit_isr.int_bit_handler = KitchenLights_button_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	/* LivingRoomLights button */
	bit_isr.int_bit_ofs = 1;
	bit_isr.int_bit_handler = LivingRoomLights_button_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	/* FrontDoor button */
	bit_isr.int_bit_ofs = 2;
	bit_isr.int_bit_handler = FrontDoor_button_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	port->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)BUTTON_USED_MASK);

}

void smarthome_close(void)
{
	DEV_GPIO_PTR port = gpio_get_dev(PRESS_BUTTON_GPIO_PORT);
	if (port != NULL) {
		port->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)BUTTON_USED_MASK);
	}
}
