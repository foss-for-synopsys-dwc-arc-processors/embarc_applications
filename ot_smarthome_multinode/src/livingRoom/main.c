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
 * \version 2017.03
 * \date 2016-11-14
 * \author Qiang Gu(Qiang.Gu@synopsys.com)
--------------------------------------------- */

/**
 * \defgroup	EMBARC_APP_BAREMETAL_OPENTHREAD	embARC OpenThread example on MRF24J40
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_BAREMETAL
 * \ingroup	EMBARC_APPS_MID_OPENTHREAD
 * \brief	OpenThread command line interface example on MRF24J40
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *     * 2 x EMSK
 *     * 2 x Digilent PMOD RF2 (MRF24J40)
 *     * 1 x SD card
 *
 * ### Design Concept
 *     This example is an OpenThread Command Line Interface (CLI) application on PMOD RF2 (MRF24J40).
 *     The mesh network is established, and IPv6 is configured with using bi-/multi-boards as Thread nodes.
 *     The node status can be shown on the terminal via UART. There are dozens of commands supported in the CLI example.
 *     The OpenThread CLI reference is in README.md.
 *
 * ### Usage Manual
 *     - See \ref EMBARC_BOARD_CONNECTION "EMSK" to connect PMOD RF2 (MRF24J40).
 *
 *     - How to use this example
 *
 *       * Program the secondary bootloader application into onboard SPI flash of EMSKs.
 *       * Generate boot.bin of the Openthread CLI example using "make bin".
 *       * Run Openthread CLI example with boot.bin from SD card. Make sure Bit 4 of the onboard DIP switch is ON to enable
 *         the secondary bootloader.
 *         - Insert SD Card back to one EMSK. Press the reset button to reboot it. Wait for loading boot.bin from SD card.
 *         - Get SD card from one EMSK and insert it to the other EMSK. Press the reset button to reboot it. Wait for loading boot.bin from SD card.
 *         - Enter “1” and press Enter button in one Tera Term. Enter “2” and press Enter button in the other one.
 *           The number will not be shown directly in the Tera Term until pressing Enter button from the keyboard.
 *         - Enter the following commands in the two terminal windows, "panid 0x1234", "ifconfig up", "thread start", to start
 *           start Thread process.
 *         - Wait 20 seconds for completing Thread configuration. Enter “state” to see the state of the node, one leader and one router.
 *         - Enter other commands in the OpenThread CLI reference (README.md) to get more information. For example,
 *           “ipaddr” is used to show the IP address of the Thread node. Enter “ipaddr” in one ternimal to get the Thread node IP address,
 *           such as **fdde:ad00:beef:0:0:ff:fe00:ec00**. Enter “ping fdde:ad00:beef:0:0:ff:fe00:ec00” in the other ternimal to ping the Thread node.
 *           The Internet Control Messages Protocol (ICMP) is implemented in the OpenThread for **ping** command.
 *
 * ![ScreenShot of Thread nodes and OpenThread startup](pic/images/example/emsk/emsk_openthread_connection.jpg)
 * ![ScreenShot of 'start' and 'ping' in OpenThread](pic/images/example/emsk/emsk_openthread_configuration.jpg)
 *
 *
 * ### Extra Comments
 *     * A few seconds are required to make connections of Thread nodes.
 *     * Use AC adapter to ensure a steady power supply.
 *     * Open two Tera Term emulators to connect EMSKs with different COM ports.
 *     * The self-boot mode preparation is included in the above steps.
 *     * “make run” is not supported because EMSK boards are both v2.x and it can lead to conflict.
 *       See \ref sect_example_usage_HowToDebugMultiBoards "how to debug multiple boards in embARC" for more information.
 *     * Enter number to generate the pseudo-random number for OpenThread. Recommend enter number in order, such as “1”, “2” and “3”.
 *       Using same number in different nodes may lead error.
 *     * Make sure the compiler configuration (TOOLCHAIN, BD_VER, CUR_CORE) of the secondary bootloader and bin file.
 *       For example, the bootloader for EMSK 2.3/ARCEM7D cannot boot the program for EMSK 2.3/ARCEM11D.
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_BAREMETAL_OPENTHREAD
 * \brief  example of how to use OpenThread command-line interface
 *   This example demonstrates a minimal OpenThread application to
 *   show the OpenThread configuration and management interfaces via a basic command-line interface.
 */

/**
 * \addtogroup	EMBARC_APP_BAREMETAL_OPENTHREAD
 * @{
 */
#include "embARC.h"
#include "embARC_debug.h"

#include <openthread/openthread.h>
#include <openthread/diag.h>
#include <openthread/coap.h>
#include <openthread/cli.h>
#include <openthread/thread_ftd.h>
#include <openthread/platform/alarm.h>
#include <openthread/platform/platform.h>


#define LED0 (0x01)
#define LED1 (0x02)
#define LED2 (0x04)
#define LED3 (0x08)
#define LED4 (0x10)
#define LED5 (0x20)
#define LED6 (0x40)
#define LED7 (0x80)

#define LED_ON  (0xff)
#define LED_OFF (0x0)

#define BTN_L_BIT_OFFSET (0)
#define BTN_R_BIT_OFFSET (1)
#define BTN_A_BIT_OFFSET (2)

#define BTN_BIT_MASK     (7)

#define BTN_ACTIVE_LOW   (0)
#define BTN_ACTIVE_HIGH  (1)


#define LIGHT_ON  (1)
#define LIGHT_OFF (0)
#define LED_LIGHT_STA (LED0)

#define REQUEST_SEND (1)
#define REQUEST_NONE (0)

#define GATEWAY_ADDR ("::1")

static uint8_t light_sta;
static uint32_t temp;
static uint8_t flag_send_light_sta, flag_send_temp;

static void light_sta_request_handler(void                * p_context,
                                      otCoapHeader        * p_header,
                                      otMessage           * p_message,
                                      const otMessageInfo * p_message_info);

typedef struct
{
	otInstance     * p_ot_instance;         /**< A pointer to the OpenThread instance. */
	otCoapResource   light_sta_resource;     /**< CoAP light_sta resource. */
} application_t;

application_t m_app =
{
	.p_ot_instance     = NULL,
	.light_sta_resource = {"light_sta", light_sta_request_handler, NULL, NULL},
};


/**
 * \brief  Interrupt service routine for buttons
 */
static void btn_l_isr(void *ptr)
{
	if (light_sta == LIGHT_ON) {
		livingroom_light_off();
	} else {
		livingroom_light_on();
	}

	flag_send_light_sta = REQUEST_SEND;
	EMBARC_PRINTF("\nbutton L pressed\r\n");
}

static void btn_r_isr(void *ptr)
{
	flag_send_temp = REQUEST_SEND;
	EMBARC_PRINTF("\nbutton R pressed\r\n");
}

static void btn_a_isr(void *ptr)
{
	EMBARC_PRINTF("\nbutton A pressed\r\n");
}

/**
 * \brief  Initialize GPIO for button
 */
static void btn_init(void)
{
	DEV_GPIO_BIT_ISR bit_isr; 
	DEV_GPIO_INT_CFG int_cfg;

	DEV_GPIO_PTR port = gpio_get_dev(EMSK_BUTTON_PORT);
	int_cfg.int_bit_mask = BTN_BIT_MASK;
	int_cfg.int_bit_type = BTN_BIT_MASK; 
	int_cfg.int_bit_polarity = BTN_ACTIVE_LOW; 
	int_cfg.int_bit_debounce = BTN_BIT_MASK;

	port->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);

	bit_isr.int_bit_ofs = BTN_L_BIT_OFFSET;
	bit_isr.int_bit_handler = btn_l_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	bit_isr.int_bit_ofs = BTN_R_BIT_OFFSET;
	bit_isr.int_bit_handler = btn_r_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	bit_isr.int_bit_ofs = BTN_A_BIT_OFFSET;
	bit_isr.int_bit_handler = btn_a_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	port->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)BTN_BIT_MASK);

}

/**
 * \brief  Use LED ON/OFF status simulates livingRoom's light ON/OFF status
 */
static void livingroom_light_on(void)
{
	light_sta = light;
	led_write(LED_ON, LED_LIGHT_STA);
}

static void livingroom_light_off(void)
{
	light_sta = UNlight;
	led_write(LED_OFF, LED_LIGHT_STA);
}

/**
 * \brief  CoAP Server functions
 */
static void light_sta_response_send(void                * p_context,
                                    otCoapHeader        * p_request_header,
                                    const otMessageInfo * p_message_info)
{
	otError      error = OT_ERROR_NONE;
	otCoapHeader header;
	otMessage  * p_response;

	do {
		otCoapHeaderInit(&header, OT_COAP_TYPE_ACKNOWLEDGMENT, OT_COAP_CODE_CHANGED);
		otCoapHeaderSetMessageId(&header, otCoapHeaderGetMessageId(p_request_header));
		otCoapHeaderSetToken(&header,
		                     otCoapHeaderGetToken(p_request_header),
		                     otCoapHeaderGetTokenLength(p_request_header));

		p_response = otCoapNewMessage(p_context, &header);
		if (p_response == NULL) {
			break;
		}

		error = otCoapSendResponse(p_context, p_response, p_message_info);

	} while (false);

	if (error != OT_ERROR_NONE && p_response != NULL) {
		otMessageFree(p_response);
	}
}

static void light_sta_request_handler(void                * p_context,
                                      otCoapHeader        * p_header,
                                      otMessage           * p_message,
                                      const otMessageInfo * p_message_info)
{
	(void)p_message;
	uint8_t sta;

	do {
		if (otCoapHeaderGetType(p_header) != OT_COAP_TYPE_CONFIRMABLE &&
		                otCoapHeaderGetType(p_header) != OT_COAP_TYPE_NON_CONFIRMABLE) {
			EMBARC_PRINTF("err: light_sta handler - bad header\r\n");
			break;
		}

		if (otCoapHeaderGetCode(p_header) != OT_COAP_CODE_PUT) {
			EMBARC_PRINTF("err: light_sta handler - not PUT request\r\n");
			break;
		}

		if (otMessageRead(p_message, otMessageGetOffset(p_message), &sta, 1) != 1) {
			EMBARC_PRINTF("err: light_sta handler - missing status\r\n");
		}

		switch (sta) {
		case LIGHT_ON:
			livingroom_light_on();
			break;

		case LIGHT_OFF:
			livingroom_light_off();
			break;

		default:
			EMBARC_PRINTF("err: light_sta handler - bad status\r\n");
			break;
		}

		if (otCoapHeaderGetType(p_header) == OT_COAP_TYPE_CONFIRMABLE) {
			light_sta_response_send(p_context, p_header, p_message_info);
		}
	} while (false);
}

/**
 * \brief  CoAP Client functions
 */
static void light_sta_response_handler(void                * p_context,
                                       otCoapHeader        * p_header,
                                       otMessage           * p_message,
                                       const otMessageInfo * p_message_info,
                                       otError               result)
{
	(void)p_context;
	(void)p_header;
	(void)p_message;

	if (result == OT_ERROR_NONE) {
		EMBARC_PRINTF("Received light_sta control response.\r\n");
	} else {
		EMBARC_PRINTF("err: Failed to receive response: %d\r\n", result);
	}
}

static void light_sta_request_send(otInstance * p_instance, uint8_t sta)
{
	otError       error = OT_ERROR_NONE;
	otMessage   * p_message;
	otMessageInfo messageInfo;
	otCoapHeader  header;

	do {
		otCoapHeaderInit(&header, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_PUT);
		otCoapHeaderAppendUriPathOptions(&header, "light_sta");
		otCoapHeaderSetPayloadMarker(&header);

		p_message = otCoapNewMessage(p_instance, &header);
		if (p_message == NULL) {
			EMBARC_PRINTF("err: Failed to allocate message for CoAP Request\r\n");
			break;
		}

		error = otMessageAppend(p_message, &sta, sizeof(sta));
		if (error != OT_ERROR_NONE) {
			break;
		}

		memset(&messageInfo, 0, sizeof(messageInfo));
		messageInfo.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
		messageInfo.mPeerPort = OT_DEFAULT_COAP_PORT;

		/* Convert a human-readable IPv6 address string into a binary representation */
		otIp6AddressFromString(GATEWAY_ADDR, &messageInfo.mPeerAddr);
		// memcpy(&messageInfo.mPeerAddr, &m_app.peer_address, sizeof(messageInfo.mPeerAddr));

		error = otCoapSendRequest(p_instance,
					  p_message,
					  &messageInfo,
					  &light_response_handler,
					  p_instance);
	} while (false);

	if (error != OT_ERROR_NONE && p_message != NULL) {
		EMBARC_PRINTF("err: Failed to send CoAP Request: %d\r\n", error);
		otMessageFree(p_message);
	}
}

static void temp_response_handler(void                * p_context,
                                  otCoapHeader        * p_header,
                                  otMessage           * p_message,
                                  const otMessageInfo * p_message_info,
                                  otError               result)
{
	(void)p_context;
	(void)p_header;
	(void)p_message;

	if (result == OT_ERROR_NONE) {
		EMBARC_PRINTF("Received temp control response.\r\n");
	} else {
		EMBARC_PRINTF("err: Failed to receive response: %d\r\n", result);
	}
}

static void temp_request_send(otInstance * p_instance, uint32_t val)
{
	otError       error = OT_ERROR_NONE;
	otMessage   * p_message;
	otMessageInfo messageInfo;
	otCoapHeader  header;

	do {
		otCoapHeaderInit(&header, OT_COAP_TYPE_CONFIRMABLE, OT_COAP_CODE_PUT);
		otCoapHeaderAppendUriPathOptions(&header, "temp");
		otCoapHeaderSetPayloadMarker(&header);

		p_message = otCoapNewMessage(p_instance, &header);
		if (p_message == NULL) {
			EMBARC_PRINTF("err: Failed to allocate message for CoAP Request\r\n");
			break;
		}

		error = otMessageAppend(p_message, &val, sizeof(val));
		if (error != OT_ERROR_NONE) {
			break;
		}

		memset(&messageInfo, 0, sizeof(messageInfo));
		messageInfo.mInterfaceId = OT_NETIF_INTERFACE_ID_THREAD;
		messageInfo.mPeerPort = OT_DEFAULT_COAP_PORT;

		/* Convert a human-readable IPv6 address string into a binary representation */
		otIp6AddressFromString(GATEWAY_ADDR, &messageInfo.mPeerAddr);
		// memcpy(&messageInfo.mPeerAddr, &m_app.peer_address, sizeof(messageInfo.mPeerAddr));

		error = otCoapSendRequest(p_instance,
					  p_message,
					  &messageInfo,
					  &temp_response_handler,
					  p_instance);
	} while (false);

	if (error != OT_ERROR_NONE && p_message != NULL) {
		EMBARC_PRINTF("err: Failed to send CoAP Request: %d\r\n", error);
		otMessageFree(p_message);
	}
}

static void request_send_scan(void)
{
	if (flag_send_light_sta == REQUEST_SEND) {
		flag_send_light_sta == REQUEST_NONE;
		light_sta_request_send(m_app.p_ot_instance, light_sta);
	}

	if (flag_send_temp == REQUEST_SEND) {
		flag_send_temp == REQUEST_NONE;
		temp_sensor_read(&temp);
		temp_request_send(m_app.p_ot_instance, temp);
	}
}

/**
 * \brief  Initialization
 */
static void thread_init(void)
{
	otInstance * p_instance;

	PlatformInit(0, NULL);

	p_instance = otInstanceInit();
	assert(p_instance);

	otCliUartInit(p_instance);

	EMBARC_PRINTF("Thread version: %s\r\n", (uint32_t)otGetVersionString());
	EMBARC_PRINTF("Network name:   %s\r\n", (uint32_t)otThreadGetNetworkName(p_instance));

	// ToDo state change showing
	// assert(otSetStateChangedCallback(p_instance, &state_changed_callback, p_instance) == OT_ERROR_NONE);

	if (!otDatasetIsCommissioned(p_instance))
	{
		assert(otLinkSetChannel(p_instance, THREAD_CHANNEL) == OT_ERROR_NONE);
		assert(otLinkSetPanId(p_instance, THREAD_PANID) == OT_ERROR_NONE);
	}

	assert(otIp6SetEnabled(p_instance, true) == OT_ERROR_NONE);
	assert(otThreadSetEnabled(p_instance, true) == OT_ERROR_NONE);

	m_app.p_ot_instance = p_instance;
}

static void coap_init(void)
{
	m_app.light_sta_resource.mContext = m_app.p_ot_instance;

	assert(otCoapStart(m_app.p_ot_instance, OT_DEFAULT_COAP_PORT) == OT_ERROR_NONE);
	assert(otCoapAddResource(m_app.p_ot_instance, &m_app.light_sta_resource) == OT_ERROR_NONE);
}

/**
 * \brief  main entry
 */
int main(void)
{
	thread_init();
	coap_init();
	temp_sensor_init(TEMP_I2C_SLAVE_ADDRESS);

	EMBARC_PRINTF("OpenThread livingRoom Node Start\r\n");

	while (true)
	{
		/* run all queued OpenThread tasklets at the time this is called */
		otTaskletsProcess(m_app.p_ot_instance);
		/* performs all platform-specific processing */
		PlatformProcessDrivers(m_app.p_ot_instance);

		request_send_scan();
	}
}

/** @} */