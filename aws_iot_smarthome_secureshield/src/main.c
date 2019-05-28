/*
 * Copyright 2010-2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 * Modified by Qiang Gu(Qiang.Gu@synopsys.com) for embARC
 */

/**
 * \defgroup	EMBARC_APP_FREERTOS_IOT_AWS_SMARTHOME_DEMO	embARC IoT Amazon Web Services(AWS) Smart Home Demo
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_OS_FREERTOS
 * \ingroup	EMBARC_APPS_MID_LWIP
 * \ingroup	EMBARC_APPS_MID_FATFS
 * \ingroup	EMBARC_APPS_MID_MBEDTLS
 * \ingroup	EMBARC_APPS_MID_AWS
 * \brief	embARC IoT Amazon Web Services(AWS) Smart Home Demo
 *
 * \details
 * ### Extra Required Tools
 *
 * ### Extra Required Peripherals
 *     * Digilent PMOD WIFI(MRF24WG0MA)
 *     * Digilent PMOD TMP2
 *
 * ### Design Concept
 *     The Pmod modules should be connected to \ref EMBARC_BOARD_CONNECTION "EMSK".
 *     This example is designed to show how to connect EMSK and [AWS IoT](https://aws.amazon.com/iot/?nc1=h_ls) with using embARC.
 *
 * ### Usage Manual
 *     Before compiling this example, you need to change macro **AWS_IOT_MQTT_HOST** in aws_iot_config.h to your own aws iot cloud mqtt host.
 *     And create an aws iot thing named **SmartHome**, you can refer to [Gettting Started with AWS IoT](https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs.html)
 *     for details, and generate certificates and download them and rename them to the ones under aws/smarthome_demo/cert.
 *
 *     Copy aws/smarthome_demo/cert to root folder of the EMSK SD card and run this example.
 *     It can report the following parameters to the AWS IoT Shadow.
 *     - the Room Temperature (float) - PMOD TMP2 connected to J2
 *     - the FrontDoor Lock (locked or unlocked) - Button X <-> LED 4-5
 *     - the LivingRoom Light (on or off) - Button L <-> Upper LED 2-3
 *     - the Kitchen Light (on or off) - Button R <-> Upper LED 0-1
 *     - (Optional)The OLED(SSD1306) connected to J4 upper row, RES pin connected to Port D[5] Pin 2, using I2C1 device.
 *        You can change the DEMO_HAVE_OLED in makefile to 1 to enable this option.
 *
 *     EMSK can send the status of the Room Temperature, FrontDoor Lock status, LivingRoom Light status and Kitchen Light status to the AWS IoT and interact
 *     with AWS IoT with using embARC.
 *
 *     Open [dashboard website](http://foss-for-synopsys-dwc-arc-processors.github.io/freeboard/), and load dashboard-smarthome-singlething.json
 *     dashboard configuration in current folder, and then you can control and monitor the single node.
 *
 *     After load the configuration, you also need to click at the setting icon, and then click on the aws datasource, and then
 *     change the AWS IOT ENDPOINT, REGION, ACCESS KEY, SECRET KEY to your own aws ones.
 *     ![embARC AWS Smart Home Dashboard Setting - Step1](emsk_freertos_iot_aws_smarthome_webapp_setting1.jpg)
 *     ![embARC AWS Smart Home Dashboard Setting - Step2](emsk_freertos_iot_aws_smarthome_webapp_setting2.jpg)
 *
 *     The JSON Document in the AWS IoT cloud is shown below.
 *
 *     \code{.unparsed}
 *     {
 *       "reported": {
 *         "temperature": 0,
 *         "DoorLocked": false,
 *         "KitchenLights": false,
 *         "LivingRoomLights": false
 *       },
 *       "desired": {
 *         "DoorLocked": false,
 *         "KitchenLights": false,
 *         "LivingRoomLights": false
 *       }
 *     }
 *     \endcode
 *
 *     ![embARC AWS Smart Home Demo using EMSK](/doc/documents/pic/images/example/emsk/emsk_freertos_iot_aws_smarthome_demo.jpg)
 *     ![ScreenShot of embARC AWS Smart Home Demo using EMSK](/doc/documents/pic/images/example/emsk/emsk_freertos_iot_aws_smarthome_demo_uart.jpg)
 *     ![ScreenShot of embARC IoT Web App for Smart Home Demo](/doc/documents/pic/images/example/emsk/emsk_freertos_iot_aws_smarthome_demo_webapp.jpg)
 *
 * ### Extra Comments
 *     - [Related Application Note](https://www.embarc.org/pdf/embARC_appnote_how_to_use_smart_home_iot.pdf)
 *     - You may need to wait several times for connecting to aws iot cloud if everything is setup correctly.
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_FREERTOS_IOT_AWS_SMARTHOME_DEMO
 * \brief	embARC IoT Amazon Web Services(AWS) smart home demo
 */

/**
 * \addtogroup	EMBARC_APP_FREERTOS_IOT_AWS_SMARTHOME_DEMO
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include <signal.h>
#include <sys/time.h>
#include <limits.h>

#include "aws_iot_config.h"
#include "aws_iot_log.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"
#include "aws_iot_shadow_interface.h"

#include "embARC.h"
#include "device_container.h"
#include "secure/crypt_container.h"

#if defined(AWSDEMO_HAVE_OLED) && defined(MID_U8GLIB)
#include "u8g.h"
#endif

#ifdef PATH_MAX
#undef PATH_MAX
#endif
#define PATH_MAX                                256

#define ROOMTEMPERATURE_UPPERLIMIT              35.0
#define ROOMTEMPERATURE_LOWERLIMIT              10.0
#define STARTING_ROOMTEMPERATURE                ROOMTEMPERATURE_LOWERLIMIT

#define MAX_LENGTH_OF_UPDATE_JSON_BUFFER        512

// use crypt container to encrypt upload data
#define ENCRYPT_UPLOAD_DATA	true

//#define SIMULATE_TEMPERATURE

#define CERT_ROOTDIR                            "cert/smarthome"

// initialize the mqtt client
AWS_IoT_Client mqttClient;

static bool temperature_updated = false;
static bool DoorLocked_updated = false;
static bool KitchenLights_updated = false;
static bool LivingRoomLights_updated = false;

static char JsonDocumentBuffer[MAX_LENGTH_OF_UPDATE_JSON_BUFFER];
static size_t sizeOfJsonDocumentBuffer = sizeof(JsonDocumentBuffer) / sizeof(JsonDocumentBuffer[0]);
#if ENCRYPT_UPLOAD_DATA
static char JsonDocumentBufferCipher[MAX_LENGTH_OF_UPDATE_JSON_BUFFER];
static char JsonDocumentBufferDeCipher[MAX_LENGTH_OF_UPDATE_JSON_BUFFER];
#endif

static float temperature = STARTING_ROOMTEMPERATURE;
static bool DoorLocked = false;
static bool KitchenLights = false;
static bool LivingRoomLights = false;

static bool last_DoorLocked = false;
static bool last_KitchenLights = false;
static bool last_LivingRoomLights = false;

static bool ConnectionStatus = false;

static jsonStruct_t FrontDoorActuator;
static jsonStruct_t KitchenLightsActuator;
static jsonStruct_t LivingRoomLightsActuator;
static jsonStruct_t temperatureHandler;

static jsonStruct_t *curActuator[4];

static char rootCA[PATH_MAX + 1];
static char clientCRT[PATH_MAX + 1];
static char clientKey[PATH_MAX + 1];

static char cafileName[] = AWS_IOT_ROOT_CA_FILENAME;
static char clientCRTName[] = AWS_IOT_CERTIFICATE_FILENAME;
static char clientKeyName[] = AWS_IOT_PRIVATE_KEY_FILENAME;

// Source code for u8g display using OLED SSD1306
static void u8g_draw(void);
static void u8g_prepare(void);
static void init_display(void);

#if defined(AWSDEMO_HAVE_OLED)
static u8g_t u8g;

static void u8g_prepare(void)
{
	u8g_SetFont(&u8g, u8g_font_6x10);		/* set the current font and reset the font reference position to "Baseline" */
	u8g_SetFontRefHeightExtendedText(&u8g);		/* define the calculation method for the ascent and descent of the current font */
	u8g_SetDefaultForegroundColor(&u8g);		/* assign one of the default colors as current color index */
	u8g_SetFontPosTop(&u8g);			/* set the reference position for the character and string draw procedure */
}

static void u8g_draw(void)
{
	static char strbuf[30];
	u8g_FirstPage(&u8g); /* marks the beginning of the picture loop; it cannot be used inside the picture loop */
	do {
		u8g_prepare();

		u8g_DrawStr(&u8g, 35, 0, "SmartHome");

		sprintf(strbuf, "Connection: %s", ConnectionStatus ? "ok" : "lost");
		u8g_DrawStr(&u8g, 15, 10, (const char*)strbuf);

		sprintf(strbuf, "Temperature: %.1fC", temperature);
		u8g_DrawStr(&u8g, 15, 20, (const char*)strbuf);

		sprintf(strbuf, "FrontDoor: %s", DoorLocked ? "locked" : "unlocked");
		u8g_DrawStr(&u8g, 15, 32, (const char*)strbuf);

		sprintf(strbuf, "LivingRoom: %s", LivingRoomLights ? "on" : "off");
		u8g_DrawStr(&u8g, 15, 44, (const char*)strbuf);

		sprintf(strbuf, "Kitchen: %s", KitchenLights ? "on" : "off");
		u8g_DrawStr(&u8g, 25, 56, (const char*)strbuf);

	} while (u8g_NextPage(&u8g)); /* marks the end of the body of the picture loop */
}

static void init_display(void)
{
	u8g_InitComFn(&u8g, &u8g_dev_ssd1306_128x64_2x_i2c, U8G_COM_SSD_I2C);
	u8g_Begin(&u8g);
	u8g_draw();
}
#else
static void u8g_prepare(void)
{
}

static void u8g_draw(void)
{
}

static void init_display(void)
{
}
#endif

void ShadowUpdateStatusCallback(const char *pThingName, ShadowActions_t action, Shadow_Ack_Status_t status,
                                const char *pReceivedJsonDocument, void *pContextData)
{
	IOT_UNUSED(pThingName);
	IOT_UNUSED(action);
	IOT_UNUSED(pReceivedJsonDocument);
	IOT_UNUSED(pContextData);

	if (SHADOW_ACK_TIMEOUT == status) {
		IOT_INFO("Update Timeout--");
	} else if (SHADOW_ACK_REJECTED == status) {
		IOT_INFO("Update RejectedXX");
	} else if (SHADOW_ACK_ACCEPTED == status) {
		IOT_INFO("Update Accepted !!");
	}
}

void FrontDoorActuate_Callback(const char *pJsonString, uint32_t JsonStringDataLen, jsonStruct_t *pContext)
{
	if (pContext != NULL) {
		bool temp = *(bool *)(pContext->pData);
		IOT_INFO("Delta - FrontDoor state changed to %d", temp);
		vTaskSuspendAll();
		container_call(device_container, controlFrontDoor, DoorLocked);
		xTaskResumeAll();
	}
}

void KitchenLights_Callback(const char *pJsonString, uint32_t JsonStringDataLen, jsonStruct_t *pContext)
{
	if (pContext != NULL) {
		bool temp = *(bool *)(pContext->pData);
		IOT_INFO("Delta - KitchenLights light state changed to %d", temp);
		vTaskSuspendAll();
		container_call(device_container, controlKitchenLights, KitchenLights);
		xTaskResumeAll();
	}
}

void LivingRoomLights_Callback(const char *pJsonString, uint32_t JsonStringDataLen, jsonStruct_t *pContext)
{
	if (pContext != NULL) {
		bool temp = *(bool *)(pContext->pData);
		IOT_INFO("Delta - LivingRoomLights light state changed to %d", temp);
		vTaskSuspendAll();
		container_call(device_container, controlLivingRoomLights, LivingRoomLights);
		xTaskResumeAll();
	}
}

#define TIME_DELAY_UPDATE		(1)

#ifdef MID_NTSHELL
#define MAX_RETRY_TIMES			(20)
#else
#define MAX_RETRY_TIMES			(1000)
#endif

static void flash_leds(uint32_t delay_ms)
{
	for (int i = 0; i < 256; i ++) {
		led_write(i, 0xFF);
		vTaskDelay(delay_ms);
	}
}

static void smarthome_error(void)
{
#ifdef MID_NTSHELL
	led_write(0x1AA, BOARD_LED_MASK);
#else
	while (1) {
		flash_leds(100);
	}
#endif
}

static void smarthome_startup(void)
{
	IOT_INFO("++++Smarthome Startup++++");

#ifndef MID_NTSHELL
	flash_leds(15);
#else
	flash_leds(3);
#endif
	led_write(0x0, BOARD_LED_MASK);
}

#define NETWORK_LEDMASK		0x80
static void controlNetworkLed(bool state)
{
	if (state) {
		IOT_INFO("Network is ok");
		led_write(NETWORK_LEDMASK, NETWORK_LEDMASK);
	} else {
		IOT_INFO("Network is lost");
		led_write(0x0, NETWORK_LEDMASK);
	}
        ConnectionStatus = state;
        u8g_draw();
}

#define HEARTBEAT_LEDMASK	0x100
static bool heartbeat_state = true;
static void toggleHeartbeatLed(void)
{
	heartbeat_state = !heartbeat_state;
	if (heartbeat_state) {
		led_write(HEARTBEAT_LEDMASK, HEARTBEAT_LEDMASK);
	} else {
		led_write(0x0, HEARTBEAT_LEDMASK);
	}
}


static DEV_WNIC *p_wnic;
static void checkAndWaitNetwork(void)
{
	p_wnic = wnic_get_dev(BOARD_PMWIFI_0_ID);

	while (1) {
		if (p_wnic->wnic_info.conn_status == WNIC_CONNECTED) {
			controlNetworkLed(true);
			break;
		}
		controlNetworkLed(false);
		vTaskDelay(5000);
	}
}

void disconnectCallbackHandler(AWS_IoT_Client *pClient, void *data)
{
	IOT_WARN("MQTT Disconnect");
	IoT_Error_t rc = FAILURE;

	if (NULL == pClient) {
		return;
	}

	IOT_UNUSED(data);

	ConnectionStatus = false;

	if (aws_iot_is_autoreconnect_enabled(pClient)) {
		IOT_INFO("Auto Reconnect is enabled, Reconnecting attempt will start now");
	} else {
		IOT_WARN("Auto Reconnect not enabled. Starting manual reconnect...");
		rc = aws_iot_mqtt_attempt_reconnect(pClient);
		if (NETWORK_RECONNECTED == rc) {
			IOT_WARN("Manual Reconnect Successful");
		} else {
			IOT_WARN("Manual Reconnect Failed - %d", rc);
		}
	}
}


int main(void)
{
	IoT_Error_t rc = SUCCESS;
	int32_t delay_ms = TIME_DELAY_UPDATE * 1000;
	int32_t retries = 0;

	init_display();

	smarthome_startup();
	temperature_updated = false;
	DoorLocked_updated = false;
	KitchenLights_updated = false;
	LivingRoomLights_updated = false;
	ConnectionStatus = false;

	FrontDoorActuator.cb = FrontDoorActuate_Callback;
	FrontDoorActuator.pData = &DoorLocked;
	FrontDoorActuator.pKey = "DoorLocked";
	FrontDoorActuator.type = SHADOW_JSON_BOOL;

	KitchenLightsActuator.cb = KitchenLights_Callback;
	KitchenLightsActuator.pData = &KitchenLights;
	KitchenLightsActuator.pKey = "KitchenLights";
	KitchenLightsActuator.type = SHADOW_JSON_BOOL;

	LivingRoomLightsActuator.cb = LivingRoomLights_Callback;
	LivingRoomLightsActuator.pData = &LivingRoomLights;
	LivingRoomLightsActuator.pKey = "LivingRoomLights";
	LivingRoomLightsActuator.type = SHADOW_JSON_BOOL;

	temperatureHandler.cb = NULL;
	temperatureHandler.pKey = "temperature";
	temperatureHandler.pData = &temperature;
	temperatureHandler.type = SHADOW_JSON_FLOAT;

	IOT_INFO("\nAWS IoT SDK Version(dev) %d.%d.%d-%s\n", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_TAG);

	sprintf(rootCA, "%s/%s", CERT_ROOTDIR, cafileName);
	sprintf(clientCRT, "%s/%s", CERT_ROOTDIR, clientCRTName);
	sprintf(clientKey, "%s/%s", CERT_ROOTDIR, clientKeyName);

	IOT_INFO("Using rootCA %s", rootCA);
	IOT_INFO("Using clientCRT %s", clientCRT);
	IOT_INFO("Using clientKey %s", clientKey);

	ShadowInitParameters_t sp = ShadowInitParametersDefault;
	ShadowConnectParameters_t scp = ShadowConnectParametersDefault;

	while (retries++ < MAX_RETRY_TIMES) {
		sp.pHost = AWS_IOT_MQTT_HOST;
		sp.port = AWS_IOT_MQTT_PORT;
		sp.pClientCRT = clientCRT;
		sp.pClientKey = clientKey;
		sp.pRootCA = rootCA;
		sp.enableAutoReconnect = true;
		sp.disconnectHandler = disconnectCallbackHandler;

		checkAndWaitNetwork();

		IOT_INFO("Shadow Init");
		rc = aws_iot_shadow_init(&mqttClient, &sp);
		if (SUCCESS != rc) {
			IOT_ERROR("Shadow initialize Error");
			return rc;
		}

		scp.pMyThingName = AWS_IOT_MY_THING_NAME;
		scp.pMqttClientId = AWS_IOT_MQTT_CLIENT_ID;
		scp.mqttClientIdLen = (uint16_t) strlen(AWS_IOT_MQTT_CLIENT_ID);

		IOT_INFO("Shadow Connect to AWS_IOT_MQTT_HOST: %s", sp.pHost);
		rc = aws_iot_shadow_connect(&mqttClient, &scp);

		if (SUCCESS != rc) {
			IOT_ERROR("Failed to connect to AWS_IOT_MQTT_HOST: %s", sp.pHost);
			IOT_ERROR("Shadow Connection Error %d", rc);
			IOT_INFO("\r\nPlease check the following things:")
			IOT_INFO("1. Whether the AWS_IOT_MQTT_HOST in aws_iot_config.h is changed according to your aws account.");
			IOT_INFO("2. Whether the rootCA, clientCRT and clientKey files are replaced according to your aws account.");
			IOT_INFO("3. Whether the modified rootCA(%s), clientCRT(%s) and clientKey(%s) files are copied to the right folder(%s) in your SDCard.",\
			 cafileName, clientCRTName, clientKeyName, CERT_ROOTDIR);
			IOT_INFO("4. If everything is modified and setup correctly, just wait for a while for connection retry.\r\n");
			vTaskDelay(3000);
			continue;
		} else {
			IOT_INFO("Shadow Connection OK");
		}

		/*
		 * Enable Auto Reconnect functionality. Minimum and Maximum time of Exponential backoff are set in aws_iot_config.h
		 *  #AWS_IOT_MQTT_MIN_RECONNECT_WAIT_INTERVAL
		 *  #AWS_IOT_MQTT_MAX_RECONNECT_WAIT_INTERVAL
		 */
		rc = aws_iot_shadow_set_autoreconnect_status(&mqttClient, true);
		if (SUCCESS != rc) {
			IOT_ERROR("Unable to set Auto Reconnect to true - %d", rc);
			return rc;
		}

		/** Register delta actuators */
		rc = aws_iot_shadow_register_delta(&mqttClient, &FrontDoorActuator);
		rc = aws_iot_shadow_register_delta(&mqttClient, &KitchenLightsActuator);
		rc = aws_iot_shadow_register_delta(&mqttClient, &LivingRoomLightsActuator);

		if (SUCCESS != rc) {
			IOT_ERROR("Shadow Register Delta Error");
		} else {
			IOT_INFO("Shadow Register Delta OK");
		}

		vTaskSuspendAll();
#ifndef SIMULATE_TEMPERATURE
		container_call(device_container, getRoomTemperature, &temperature);
#endif
		container_call(device_container, smarthome_init);
		container_call(device_container, controlFrontDoor, DoorLocked);
		container_call(device_container, controlKitchenLights, KitchenLights);
		container_call(device_container, controlLivingRoomLights, LivingRoomLights);
		xTaskResumeAll();

		last_LivingRoomLights = !LivingRoomLights;
		last_KitchenLights = !KitchenLights;
		last_DoorLocked = !DoorLocked;

		uint32_t act_idx = 0;
		while (NETWORK_ATTEMPTING_RECONNECT == rc || NETWORK_RECONNECTED == rc || SUCCESS == rc) {
			rc = aws_iot_shadow_yield(&mqttClient, 200);
		        u8g_draw();
			if (NETWORK_ATTEMPTING_RECONNECT == rc) {
				vTaskDelay(1000);
				// If the client is attempting to reconnect we will skip the rest of the loop.
				continue;
			}
			ConnectionStatus = true;
			/** Update desired messages */
			act_idx = 0;
			if (DoorLocked_updated) {
				curActuator[act_idx] = &FrontDoorActuator;
				act_idx ++;
				DoorLocked = !DoorLocked;
				DoorLocked_updated = false;
				vTaskSuspendAll();
				container_call(device_container, controlFrontDoor, DoorLocked);
				xTaskResumeAll();
			}
			if (KitchenLights_updated) {
				curActuator[act_idx] = &KitchenLightsActuator;
				act_idx ++;
				KitchenLights = !KitchenLights;
				KitchenLights_updated = false;
				vTaskSuspendAll();
				container_call(device_container, controlKitchenLights, KitchenLights);
				xTaskResumeAll();
			}
			if (LivingRoomLights_updated) {
				curActuator[act_idx] = &LivingRoomLightsActuator;
				act_idx ++;
				LivingRoomLights = !LivingRoomLights;
				LivingRoomLights_updated = false;
				vTaskSuspendAll();
				container_call(device_container, controlLivingRoomLights, LivingRoomLights);
				xTaskResumeAll();
			}
			if (act_idx > 0) {
				rc = aws_iot_shadow_init_json_document(JsonDocumentBuffer, sizeOfJsonDocumentBuffer);
				if (rc == SUCCESS) {
					if (act_idx == 1) {
						rc = aws_iot_shadow_add_desired(JsonDocumentBuffer, sizeOfJsonDocumentBuffer, 1, curActuator[0]);
					} else if (act_idx == 2) {
						rc = aws_iot_shadow_add_desired(JsonDocumentBuffer, sizeOfJsonDocumentBuffer, 2, curActuator[0], curActuator[1]);
					} else if (act_idx == 3) {
						rc = aws_iot_shadow_add_desired(JsonDocumentBuffer, sizeOfJsonDocumentBuffer, 3, curActuator[0], curActuator[1], curActuator[2]);
					} else {
						rc = aws_iot_shadow_add_desired(JsonDocumentBuffer, sizeOfJsonDocumentBuffer, 3, &FrontDoorActuator, &KitchenLightsActuator, &LivingRoomLightsActuator);
					}

					if (rc == SUCCESS) {
						rc = aws_iot_finalize_json_document(JsonDocumentBuffer, sizeOfJsonDocumentBuffer);
						if (rc == SUCCESS) {
							IOT_INFO("Update Shadow Desired: %s", JsonDocumentBuffer);
#if ENCRYPT_UPLOAD_DATA
							IOT_INFO("now try to encrypt input:%s len:%d \r\n", JsonDocumentBuffer, sizeOfJsonDocumentBuffer);
							vTaskSuspendAll();
							container_call(crypt_container, operate_encrypt, JsonDocumentBuffer, sizeOfJsonDocumentBuffer, \
												JsonDocumentBufferCipher, &sizeOfJsonDocumentBuffer);
							xTaskResumeAll();
							IOT_INFO("result of encrypt output:%s len:%d \r\n", JsonDocumentBufferCipher, sizeOfJsonDocumentBuffer);
							rc = aws_iot_shadow_update(&mqttClient, AWS_IOT_MY_THING_NAME, JsonDocumentBufferCipher, ShadowUpdateStatusCallback, NULL, 4, true);
#else
							rc = aws_iot_shadow_update(&mqttClient, AWS_IOT_MY_THING_NAME, JsonDocumentBuffer, ShadowUpdateStatusCallback, NULL, 4, true);
#endif
						}
					}
				}
			}
			u8g_draw();

			if (rc != SUCCESS) {
				continue;
			}
			/** Update reported messages */
			vTaskSuspendAll();
			temperature_updated = container_call(device_container, getRoomTemperature, &temperature);
			xTaskResumeAll();

			rc = aws_iot_shadow_init_json_document(JsonDocumentBuffer, sizeOfJsonDocumentBuffer);
			if (rc == SUCCESS) {
				rc = aws_iot_shadow_add_reported(JsonDocumentBuffer, sizeOfJsonDocumentBuffer, 4, &temperatureHandler, &FrontDoorActuator, &KitchenLightsActuator, &LivingRoomLightsActuator);
				if (rc == SUCCESS) {
					rc = aws_iot_finalize_json_document(JsonDocumentBuffer, sizeOfJsonDocumentBuffer);
					if (rc == SUCCESS) {
						IOT_INFO("Update Shadow: %s", JsonDocumentBuffer);
						rc = aws_iot_shadow_update(&mqttClient, AWS_IOT_MY_THING_NAME, JsonDocumentBuffer, ShadowUpdateStatusCallback,
						                           NULL, 4, true);
					}
				}
			}
			u8g_draw();

			toggleHeartbeatLed();

			IOT_INFO("*****************************************************************************************\n");
			vTaskDelay(delay_ms);
		}

		vTaskSuspendAll();
		container_call(device_container, smarthome_close);
		xTaskResumeAll();
		if (SUCCESS != rc) {
			IOT_ERROR("An error occurred in the loop %d", rc);
		}

		checkAndWaitNetwork();

		if (rc == FAILURE) {
			IOT_INFO("Disconnecting");
			rc = aws_iot_shadow_disconnect(&mqttClient);

			if (SUCCESS != rc) {
				IOT_ERROR("Disconnect error %d", rc);
			}
		}
	}
	smarthome_error();

	return rc;
}

/* interrupt service routine for KitchenLights light button */
void KitchenLights_button_isr(void *ptr)
{
	KitchenLights_updated = true;
}

/* interrupt service routine for LivingRoomLights light button */
void LivingRoomLights_button_isr(void *ptr)
{
	LivingRoomLights_updated = true;
}

/* interrupt service routine for FrontDoor control */
void FrontDoor_button_isr(void *ptr)
{
	DoorLocked_updated = true;
}

/** @} */