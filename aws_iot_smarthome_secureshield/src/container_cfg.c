/*------------------------------------------
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

#include "embARC.h"
#include "device.h"

#define PERIPHERAL_ADDR_BASE 0xf0000000

static CONTAINER_AC_TABLE device_container_act[] = {
	{smarthome_init, 0, SECURESHIELD_AC_INTERFACE},
	{smarthome_close, 0, SECURESHIELD_AC_INTERFACE},
	{getRoomTemperature, 1, SECURESHIELD_AC_INTERFACE},
	{controlFrontDoor, 1, SECURESHIELD_AC_INTERFACE},
	{controlKitchenLights, 1, SECURESHIELD_AC_INTERFACE},
	{controlLivingRoomLights, 1, SECURESHIELD_AC_INTERFACE},
	{(void *)(PERIPHERAL_ADDR_BASE + REL_REGBASE_I2C0), 0x1000, SECURESHIELD_ACDEF_UPERIPH}
 };

static CONTAINER_AC_TABLE g_main_container_act[] = {
	/* PINMUX, UART1 and GPIO0 are required */
	{(void *)(PERIPHERAL_ADDR_BASE + REL_REGBASE_PINMUX), 0x1000, SECURESHIELD_ACDEF_UPERIPH},
	{(void *)(PERIPHERAL_ADDR_BASE + REL_REGBASE_UART1), 0x1000, SECURESHIELD_ACDEF_UPERIPH},
	{(void *)(PERIPHERAL_ADDR_BASE + REL_REGBASE_GPIO0), 0x1000, SECURESHIELD_ACDEF_UPERIPH},
	{(void *)(PERIPHERAL_ADDR_BASE + REL_REGBASE_SPI0), 0x1000, SECURESHIELD_ACDEF_UPERIPH},
	{NULL, INTNO_GPIO, SECURESHIELD_AC_IRQ},
	{NULL, INTNO_UART1, SECURESHIELD_AC_IRQ},
	/* Timer1, Timer 0, RTC and TIMIE BCR are accessible for background container*/
#if SECURESHIELD_VERSION == 1
	{(void *)0x21, 0x3, SECURESHIELD_AC_AUX},
	{(void *)0x100, 0x6, SECURESHIELD_AC_AUX},
	{(void *)0x75, 0x1, SECURESHIELD_AC_AUX},
#endif

	{NULL, INTNO_TIMER0, SECURESHIELD_AC_IRQ}
};

/* set the access control table of background container */
SECURESHIELD_CONTAINER_BACKGROUND(g_main_container_act);

/* configure the other container */
SECURESHIELD_CONTAINER_CONFIG(device_container, device_container_act, 1024);
