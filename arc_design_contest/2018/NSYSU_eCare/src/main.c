/* ------------------------------------------
 * Copyright (c) 2016, Synopsys, Inc. All rights reserved.
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
 * \version 2016.05
 * \date 2016-04-20
 * \author Qiang Gu(Qiang.Gu@synopsys.com)
--------------------------------------------- */

/**
 * \defgroup	EMBARC_APP_BAREMETAL_BLE_HM1X	embARC BLE_HM1X Example
 * \ingroup	EMBARC_APPS_TOTAL
 * \ingroup	EMBARC_APPS_BAREMETAL
 * \brief	embARC example for BLE HM10 device via UART
 *
 * \details
 * ### Extra Required Tools
 *     * BLE APP on SmartPhone:
 *       - Android: [BLE Android ComAssistant](http://www.jnhuamao.cn/HMBLEComAssistant.rar)
 *       - iOS: [LightBlue](http://itunes.apple.com/us/app/lightblue-bluetooth-low-energy/id557428110?mt=8)
 *
 * ### Extra Required Peripherals
 *     * UART Based BLE Device [HM-10](http://www.jnhuamao.cn/bluetooth.asp?ID=1)
 *
 * ### Design Concept
 *     This example is designed to test how to operate UART-based BLE device HM10 via UART
 *     and demonstrate how to use HM-10 BLE API defined in hm1x.h.
 *
 * ### Usage Manual
 *     - How to connect [HM-10](http://www.jnhuamao.cn/bluetooth.asp?ID=1) to \ref EMBARC_BOARD_CONNECTION "EMSK".
 *       \code
 *       HM10-DV33 <----> J1-PIN6
 *       HM10-GND  <----> J1-PIN5
 *       HM10-TXD  <----> J1-PIN4
 *       HM10-RXD  <----> J1-PIN3
 *       \endcode
 *
 *     - How to use this example
 *
 *       This example demonstrates how to use the API of [HM-10](http://www.jnhuamao.cn/bluetooth.asp?ID=1)
 *        located in *board/emsk/drivers/ble*, the APIs contains information about send AT command and receive response,
 *        and read or write data to BLE device. \n
 *       The example performs tests on the HM-10 BLE device using different uart baudrate,
 *        which take approximately 2 minutes, then the device will enter to receive and send mode.
 *        Users can use a BLE-UART APP on their smartphone which must support BLE,
 *        for android please use [BLE Android ComAssistant](http://www.jnhuamao.cn/HMBLEComAssistant.rar),
 *        for iOS please use [LightBlue](http://itunes.apple.com/us/app/lightblue-bluetooth-low-energy/id557428110?mt=8).
 *
 * ![ScreenShot of ble_hm1x-uart message under baremetal](pic/images/example/emsk/emsk_ble_hm1x.jpg)
 * ![ScreenShot of ble_hm1x-android app under baremetal](pic/images/example/emsk/emsk_ble_hm1x_android.jpg)
 *
 * ### Extra Comments
 *
 */

/**
 * \file
 * \ingroup	EMBARC_APP_BAREMETAL_BLE_HM1X
 * \brief  example of how to use HM-10 BLE device
 *  This example shows how to use hm1x api declared in hm1x.h in baremetal.
 */

/**
 * \addtogroup	EMBARC_APP_BAREMETAL_BLE_HM1X
 * @{
 */

#include "embARC.h"
#include "embARC_debug.h"
#include "stdio.h"

#include "board.h"
#include "dev_uart.h"

uint32_t baudrate_list[] = {4800, 9600, 19200, 38400, 57600, 115200};

static DEV_UART *ble_uart;

//initial BLE_UART
uint32_t ble_uart_init(uint32_t baudrate)
{
	ble_uart = uart_get_dev(DW_UART_0_ID);
	return 0;
}

//ADC Code

#define I2C_SLAVE_ADDR  0x28

#define CHANNEL_0  0x10
#define CHANNEL_1  0x20
#define CHANNEL_2  0x40
#define CHANNEL_3  0x80

//#define MAX_DELAY	10000 /* max period ms */
//#define MAX_TIMES	1000  /* max repeated times */

#define TIMEOUT 	0x10000


static const char ad_channels[] = { CHANNEL_0, CHANNEL_1, CHANNEL_2, CHANNEL_3 };
static char write_buf[20];
static char read_buf[20];

static DEV_IIC *adc_iic;

int adc(void)
{
	uint32_t i;
	//uint32_t k;
	//uint32_t t;
	uint32_t delay;
	uint32_t res;
	uint32_t adc[4];
	uint32_t count = 0;
	uint32_t period_ms, measure_times;
	uint32_t param;
	char send_data=0;
	//uint8_t send_array[10];
	long val;
	uint32_t arg_err_flag = 0;
	static int32_t running = 0;
	char **argv_tp;
	int32_t ercd = E_OK;

	period_ms = 10;


	// get adc iic object
	adc_iic = iic_get_dev(DW_IIC_0_ID);
	// open as master device in poll method @ standard speed
	adc_iic->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
	param = I2C_SLAVE_ADDR;
	adc_iic->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(param));

	while(1) {

		for (i = 0; i < 1; i++) {

			adc[i] = 0;

			write_buf[0] = ad_channels[0];
			res = adc_iic->iic_write(write_buf, 1);

			// read data from i2c
			if (adc_iic->iic_read(read_buf, 2) < 0) {
				running = 0;
				adc_iic->iic_close();
		 		goto error_exit;
			} else {
				res = read_buf[0];
				res = ((res << 8) & 0x0F00) | read_buf[1];
				adc[i] = res;
			}
		}
				//ble_uart->uart_write(adc[0],1);
				send_data = adc[0] /4;
				ble_uart->uart_write(&send_data, 1);
				EMBARC_PRINTF("send_data=%d  ", send_data);

				/*for(t = 0; t < 255; t++){
					//send_data = 0xF0 ;
					send_data = t;
					ble_uart->uart_write(&send_data, 1);
					EMBARC_PRINTF("send_data=%d  ", send_data);
					//EMBARC_PRINTF("send_data=%d  ", ble_uart->uart_write(&send_data,1));
					board_delay_ms(period_ms, OSP_DELAY_OS_COMPAT_ENABLE);
				}*/


		board_delay_ms(period_ms, OSP_DELAY_OS_COMPAT_ENABLE);

	 	if(((++count) >= measure_times)) {
			running = 0;
	 		break;
	 	}
	}
	adc_iic->iic_close();
	return E_OK;
error_exit:
	return ercd;
}

int main(void)
{
	uint8_t rcv_buf[20];
	uint32_t rcv_cnt;
	uint32_t baudrate = UART_BAUDRATE_9600;
	uint32_t i = 0;

	/** change the init baudrate according to your ble module settings */
	EMBARC_PRINTF("Init HM1X with baudrate %dbps\r\n", baudrate);
	hm1x_init(baudrate);

	/**
	 * For Detail use of HM1X, please refer to its documentation,
	 * but take notice of its wrong description
	 * Links:
	 * Chinese: http://www.jnhuamao.cn/download.asp?ID=0
	 * English: http://www.jnhuamao.cn/index_en.asp?ID=1
	 * English: https://github.com/danasf/hm10-android-arduino/blob/master/HM-10/datasheet.pdf
	 * Android BLE Test APP: http://www.jnhuamao.cn/HMBLEComAssistant.rar
	 * Androud BLE Test APP Source Code: http://www.jnhuamao.cn/BluetoothLeGatt.rar
	 */

	do {
		EMBARC_PRINTF("++++Do following test with baudrate %dbps++++\r\n", baudrate);
		/** Here is a test sequence for HM1X, no need to add into HM1X init process */
		hm1x_flush();

		if (hm1x_disconnect() == 0) {
			EMBARC_PRINTF("Disconnect Successfully\r\n");
		} else {
			EMBARC_PRINTF("Disconnect Failed\r\n");
		}

		if (hm1x_restart() == 0) {
			EMBARC_PRINTF("Restart Successfully\r\n");
		} else {
			EMBARC_PRINTF("Restart Failed\r\n");
		}

		if (hm1x_set_mode(BLE_HM1X_MODE_0) == 0) {
			EMBARC_PRINTF("SET MODE to %d\r\n", BLE_HM1X_MODE_0);
		} else {
			EMBARC_PRINTF("SET MODE Failed\r\n");
		}

		if (hm1x_set_type(BLE_HM1X_TYPE_0) == 0) {
			EMBARC_PRINTF("SET TYPE to %d\r\n", BLE_HM1X_TYPE_0);
		} else {
			EMBARC_PRINTF("SET TYPE Failed\r\n");
		}

		if (hm1x_set_role(BLE_HM1X_SLAVE_ROLE) == 0) {
			EMBARC_PRINTF("SET ROLE to %d\r\n", BLE_HM1X_SLAVE_ROLE);
		} else {
			EMBARC_PRINTF("SET ROLE Failed\r\n");
		}

		if ((rcv_cnt=hm1x_get_param("ROLE", (char *)rcv_buf)) != -1) {
			EMBARC_PRINTF("GET ROLE:%s\r\n", rcv_buf);
		} else {
			EMBARC_PRINTF("GET ROLE Failed\r\n");
		}

		if ((rcv_cnt=hm1x_get_param("MODE", (char *)rcv_buf)) != -1) {
			EMBARC_PRINTF("GET MODE:%s\r\n", rcv_buf);
		} else {
			EMBARC_PRINTF("GET MODE Failed\r\n");
		}

		if ((rcv_cnt=hm1x_get_param("TYPE", (char *)rcv_buf)) != -1) {
			EMBARC_PRINTF("GET TYPE:%s\r\n", rcv_buf);
		} else {
			EMBARC_PRINTF("GET TYPE Failed\r\n");
		}

		/** how to use hm1x_exec_cmd_chkresp */
		if ((rcv_cnt=hm1x_exec_cmd_chkresp("AT+BATT?", "OK+Get:", (char *)rcv_buf, 2)) != -1) {
			EMBARC_PRINTF("GET Battery Level:%s\r\n", rcv_buf);
		} else {
			EMBARC_PRINTF("GET Battery Failed\r\n");
		}
		if ((rcv_cnt=hm1x_exec_cmd_chkresp("AT+ADDR?", "OK+ADDR:", (char *)rcv_buf, 2)) != -1) {
			EMBARC_PRINTF("GET HM1X MAC Address:%s\r\n", rcv_buf);
		} else {
			EMBARC_PRINTF("GET HM1X MAC Failed\r\n");
		}
		if ((rcv_cnt=hm1x_exec_cmd_chkresp("AT+NAME?", "OK+NAME:", (char *)rcv_buf, 2)) != -1) {
			EMBARC_PRINTF("GET HM1X NAME:%s\r\n", rcv_buf);
		} else {
			EMBARC_PRINTF("GET HM1X NAME Failed\r\n");
		}

		/** how to use hm1x_exec_command */
		if ((rcv_cnt=hm1x_exec_command("AT+VERS?", (char *)rcv_buf, 500)) != -1) {
			EMBARC_PRINTF("GET HM1X Version:%s\r\n", rcv_buf);
		} else {
			EMBARC_PRINTF("GET HM1X Version Failed\r\n");
		}

		//EMBARC_PRINTF("++++Test with baudrate %dbps finished++++\r\n", baudrate);
		/**
		 * Here Set Baudrate to another one,
		 * but you also need to reconfigure the HM1X UART
		 * Then go through the test again
		 */
		/*if (hm1x_set_baud(baudrate_list[i]) != -1) {
			if (hm1x_restart() == 0) {
				baudrate = baudrate_list[i];
				EMBARC_PRINTF("Re-Init the HM1X BLE Baudrate to %dbps, and restart the test\r\n", baudrate);
				hm1x_init(baudrate);
			} else {
				break;
			}
		} else {
			break;
		}*/
		/** Test sequence end */
		i++;
	} while (i < 1);

	/*
	EMBARC_PRINTF("\r\nTry to reset the baudrate to %dbps\r\n", UART_BAUDRATE_9600);
	if (hm1x_set_baud(UART_BAUDRATE_9600) != -1) {
		if (hm1x_restart() == 0) {
			hm1x_init(UART_BAUDRATE_9600);
			baudrate = UART_BAUDRATE_9600;
		}
	}
	*/
	if (baudrate != UART_BAUDRATE_9600) {
		EMBARC_PRINTF("!!!CAUTION:!!!Next Time run this test may fail due to initial baudrate changed\r\n");
	}
	EMBARC_PRINTF("Do send and receive test under baudrate:%dbps\r\n", baudrate);

	EMBARC_PRINTF("Start HM1X-BLE Send Receive Test, please connect to HM1X-BLE using its app.\r\n");
	EMBARC_PRINTF("The main loop will receive data from HM1X, and send back what it received.\r\n");


	uint8_t j=0;

	//initial BLE
	ble_uart_init(baudrate);

	while (1) {
		//rcv_cnt = hm1x_read(rcv_buf, sizeof(rcv_buf));
		/*rcv_buf[1] = '\0';
		rcv_buf[0] = j+'0';
		if (j<100) {
			//EMBARC_PRINTF("%s", rcv_buf);
			//hm1x_write(rcv_buf, 1);
			EMBARC_PRINTF("j=%d rcv_buf=%s", j, rcv_buf);
			//sent cnt=1 bytes using bluetooth in uart
			ble_uart->uart_write(rcv_buf,1);
			j=j+1;
		}*/

		adc();
	}

	return E_SYS;	/* system error */
}

/** @} */
