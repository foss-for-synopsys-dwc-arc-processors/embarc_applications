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
 * \version 2017.08
 * \date 2017-08-15
 * \author Xiangcai Huang(xiangcai@synopsys.com)
--------------------------------------------- */
 
/**
 * \defgroup	EMBARC_APP_BAREMETAL_OT_SMARTHOME_MULTINODE_NCP	embARC OpenThread application on MRF24J40
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_BAREMETAL
 * \ingroup	EMBARC_APPS_MID_OPENTHREAD
 * \brief	OpenThread Network Co-Processor(NCP) example on MRF24J40
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *     * 1 x EMSK
 *     * 1 x Digilent PMOD RF2 (MRF24J40)
 *     * 1 x SD card
 *     * 1 x microUSB-USB cable
 *
 * ### Design Concept
 *     This example is an OpenThread NCP application on PMOD RF2 (MRF24J40).
 *     The Thread NCP example demonstrates an OpenThread application that can serve as a low-power wireless NCP
 *       to communicate with Userspace WPAN Network Daemon (wpantund) on Unix-like operating systems.
 *     This example has similar functionality as the NCP example available in the official OpenThread repository,
 *       but has been adapted to use with the embARC OSP.
 *
 * ### Usage Manual
 *     - See \ref EMBARC_BOARD_CONNECTION "EMSK" to connect PMOD RF2 (MRF24J40).
 *     - Connect the board to the Gateway(Raspberry Pi) through USB.
 *     - Run the Gateway(wpantund and wpanctl).
 *
 *     - How to use this example
 *
 *       * Program the secondary bootloader application into onboard SPI flash of EMSK.
 *       * Generate boot.bin of the Openthread NCP example using "make bin".
 *       * Run Openthread NCP example with boot.bin from SD card. Make sure Bit 4 of the onboard DIP switch is ON to enable
 *         the secondary bootloader.
 *         - Insert SD Card back to one EMSK. Press the reset button to reboot it. Wait for loading boot.bin from SD card.
 *         - Initialize Thread NCP process automatically.
 *         - Wait 20 seconds for completing Thread configuration. Then, operation in the wpanctl:
 *           Enter "status" to see the state of the NCP, offline or associated. Enter "scan" to find out the Thread network available.
 *           If you'd like to join a new network, enter "leave" to disconnect the primary network. Then, enter "set Network:Key --data [masterkey]",
 *             and enter "join [number]" to join the new network.
 *         - Enter other commands of the OpenThread NCP CLI to get more information.
 *
 * ![ScreenShot of Thread nodes and OpenThread startup](./doc/screenchots/emsk_openthread_connection.jpg)
 * ![ScreenShot of 'start' and 'ping' in OpenThread](./doc/screenchots/emsk_openthread_configuration.jpg)
 *
 *
 * ### Extra Comments
 *     * Use AC adapter to ensure a steady power supply.
 *     * This example can be used along with OpenThread Smarthome Application’s Gateway solution 
 *       to provide Internet connectivity to the Thread network.
 *     * The NCP application starts in disconnected state, and remains so until it is appropriately configured by the Gateway.
 *       After NCP is connected to the Gateway, NCP will try to connect to the existing network or form a new one.
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_BAREMETAL_OT_SMARTHOME_MULTINODE_NCP
 * \brief  application of how to use OpenThread NCP API and CLI.
 */

/**
 * \addtogroup	EMBARC_APP_BAREMETAL_OT_SMARTHOME_MULTINODE_NCP
 * @{
 */
#include "embARC.h"
#include "embARC_debug.h"

#include <openthread/openthread.h>
#include <openthread/thread_ftd.h>
#include <openthread/ncp.h>
#include <openthread/platform/platform.h>
#include <openthread/platform/settings.h>

#define ROUTER_SELECTION_JITTER  5       /**< A value of router selection jitter. */

typedef struct
{
	otInstance * p_ot_instance;
} application_t;

static application_t m_app =
{
	.p_ot_instance = NULL
};

static void thread_init(void)
{
	otInstance *p_instance;

	PlatformInit(0, NULL);

	otPlatSettingsWipe(NULL);

	p_instance = otInstanceInit();
	assert(p_instance);

	otNcpInit(p_instance);

	otThreadSetRouterSelectionJitter(p_instance, ROUTER_SELECTION_JITTER);

	m_app.p_ot_instance = p_instance;
}

/**
 * \brief  main entry
 */
int main(void)
{
	thread_init();

	EMBARC_PRINTF("OpenThread NCP Start\r\n");

	while (true)
	{
		/* run all queued OpenThread tasklets at the time this is called */
		otTaskletsProcess(m_app.p_ot_instance);
		/* performs all platform-specific processing */
		PlatformProcessDrivers(m_app.p_ot_instance);
	}
}

/** @} */