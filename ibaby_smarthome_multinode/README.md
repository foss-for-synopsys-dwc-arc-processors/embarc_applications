# iBaby Smarthome Application
This application is designed to show how to connect 1 or more EMSKs and **iBaby Smarthome Gateway** using embARC. The connection between EMSK and Gateway is based on **LwM2M** protocol as shown in the following figure. All the iBaby nodes interact with AWS IoT through the Gateway indirectly. There are only 2 nodes available now, but you can add more nodes easily to iBaby Smarthome by taking these 2 examples as a reference. And the lamp node is more simple and easy to learn for beginner.

* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#system-architecture)
	* [Freeboard UI](#freeboard-ui)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)
	* [Add a New Node](#add-a-new-node)
		* [Makefile](#makefile)
		* [Main Entry](#main-entry)
		* [Driver](#driver)
		* [Function Module](#function-module)
		* [LwM2M Client](#lwm2m-client)

## Introduction
**iBaby Infant Sleep Monitoring System**

### Function

- **Heartrate and body temperature detecting**
- **Sleep monitoring** (Alarm of face down during sleeping, Sleep-Wake state detecting and notify of baby awake)
- Communicate with Gateway（**LwM2M**）
- **OTA** based on LwM2M protocol (Supports file transfer over 2k size)
- **Multi-node** access, connecting and communicating with each other.
  Control **Lamp Node** to turn on automatically when the **Wearable Node** detecting baby sleep on his stomach.
  ( Indirectly control ! Wearable node sends the warnning information to gateway when it detecting exceptional situations,
   then the gateway transmits the information to lamp node and turns the lamp on.)

![ibaby_function][0]

### System Architecture

![system_architecture][1]

### Freeboard UI

![freeboard_ui][2]

## Hardware and Software Setup
### Required Hardware
- 2 [DesignWare ARC EM Starter Kit(EMSK)][30]
- 2 [Digilent PMOD WiFi(MRF24WG0MA)][31]
- 1 [Acceleration sensor(MPU6050)][32]
- 1 [Heartrate sensor(MAX30102)][33]
- 1 [Temperature sensor(MLX90614)][34]
- 2 SD Card
- WiFi Hotspot(SSID:**embARC**, Password:**qazwsxedc**, WPA/WPA2 encypted)
- PC or Raspberry Pi for running iBaby Smarthome Gateway

We designed integrated module for 3 sensors above, I'll call it **Foot Ring Module**. The physical picture of wearable node is as follows:

![wearable_node][3]

### Required Software
- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom
- [iBaby Smarthome Gateway][35]
- [iBaby Freeboard UI][36]

### Hardware Connection
1. EMSK 1 implement **wearable** node, as an intelligent foot ring for baby, it will publish baby's status to AWS IoT Cloud via the Gateway, including body temperature, heartrate, sleep-wake state, motion intensity and some warning information. We can view all data on the Freeboard UI.
   - Connect **PMOD WiFi** to **J5**, connect **Foot Ring Module** to **J4**(Using IIC1 interface).
2. EMSK 2 implement **lamp** node, as an desk lamp in parents' room, it will publish the working state of lamp to AWS IoT Cloud via the Gateway. We use the LED0 to simulate real desk lamp to make the application more simple and easy to learn. The lamp can be controlled by button A manually, or **wearable** node automatically. We can also view it's state on the Freeboard UI.
   - Connect **PMOD WiFi** to **J5**.
3. Configure your EMSKs with proper core configuration.

## User Manual
### Before Running This Application
Firstly, download source code of **iBaby Smarthome Gateway** and **iBaby Freeboard UI** from github. Then you need an AWS account, and create things for ibaby Gateway, generate and save the certs for it, and modify specific `config.js`(path: `./ibaby_smarthome_gateway/config.js`) for your project.

Secondly, run the iBaby Smarthome Gateway, open the browser and type IP address of the Gateway to access user interface. You can push iBaby Freeboard UI to github so that it can support remote access data.

|  EMSK Implemented Node   |    AWS IoT Thing      |     Source Folder      |
| ------------------------ | --------------------- | ---------------------- |
|      wearable node       |        ibaby          |   src/wearable_node    |
|      lamp node           |        ibaby          |   src/lamp_node        |

The hardware resources are allocated as following table.

|  Hardware Resource  |            Function                                           |
| ------------------- | ------------------------------------------------------------- |
|  MPU6050            |        Acceleration sensor                                    |
|  MAX30102           |        Heartrate sensor                                       |
|  MLX90614           |        Body temperature sensor                                |
|  PMOD WiFi          |        Provide WiFi Connection                                |

### Run This Application

Modify the settings for connecting to the LwM2M Server(Gateway), as shown below:

(path: `src/wearable_node/function/lwm2m/lwm2m.c`):

		const static char *p_port   = (char *)"5683";    /* lwm2mServer's port and IP */
		const static char *p_server = (char *)"192.168.43.199";
		const static char *p_client_name = (char *)"wn"; /* name of lwm2m client node */

(path: `src/lamp_node/function/lwm2m/lwm2m.c`):

		const static char *p_port   = (char *)"5683";    /* lwm2mServer's port and IP */
		const static char *p_server = (char *)"192.168.43.199";
		const static char *p_client_name = (char *)"ln"; /* name of lwm2m client node */

Here take **EMSK2.2 - ARC EM11D** with Metaware Toolset for example to show how to run this application.

1. We need to use embARC 2nd bootloader to automatically load application binary for different EMSK and run.

2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.

3. Interact using EMSK and Freeboard.

### Add a New Node

Seeing **Lamp Node** for reference. It's complete although seems very simple and very helpful to add a new node to iBaby System.

|  folder/file        |            Function                                           |
| ------------------- | ------------------------------------------------------------- |
|  driver             |        source code of drivers                                 |
|  function           |        source code of function modules                        |
|  lwm2m_client       |        source code of LwM2M Client                            |
|  FreeRTOSConfig.h   |        header file of FreeRTOS configurations                 |
|  main.c             |        main entry of embARC Application                       |
|  makefile           |        Makefile of embARC Application                         |

#### Makefile

- Selected FreeRTOS here, then you can use [FreeRTOS API][39] in your application:

		# Selected OS
		OS_SEL ?= freertos

- Target options:

		BOARD ?= emsk
		BD_VER ?= 22
		CUR_CORE ?= arcem11d
		TOOLCHAIN ?= gnu

- Reset the heap and stack size for LwM2M and make sure they are big enough for your application:

		##
		# HEAP & STACK SETTINGS
		# For LwM2M Stack Usage
		##
		HEAPSZ ?= 81920
		STACKSZ ?= 81920

- The relative series of the root directory, here the path of the Makefile is `./embarc_osp/application/ibaby_smarthome_multinode/src/lamp_node/makefile`:

		#
		# root dir of embARC
		#
		EMBARC_ROOT = ../../../..

- The middleware used in your application:

		MID_SEL = common lwip-contrib wakaama fatfs lwip

- Directoties of source files and header files, notice that it can not recursive:

		# application source dirs
		APPL_CSRC_DIR
		APPL_ASMSRC_DIR

		# application include dirs
		APPL_INC_DIR

See `./embarc_osp/doc/embARC_Document/html/page_example.html`, **"Options to Hard-Code in the Application Makefile"** for more detailed information.

#### Main Entry

- Firstly, initializing the hardware, such as buttons on the emsk and GPIO interface for lamp.

- Secondly, try to start LwM2M Client. Before that, modify the ssid and password of WIFI AP in `./embarc_osp/board/emsk/emsk.h`:

		143 #define WF_HOTSPOT_NAME             "embARC"
		    #define WF_HOTSPOT_PASSWD           "qazwsxedc"

	You ought to modify the flag of WIFI module selection if you are using **RW009** not MRF24G, in `./embarc_osp/board/board.mk`:

		16 WIFI_SEL ?= 1

- Finally, starting to run the function moudles. Reading value from sensors, processing it and controlling someting to work according to the results, just like the **wearable node** and **lamp node** do.

#### Driver

Placing the drivers' source code in `driver` folder, you can see there are subfolders for button and lamp drivers.
Placing the C source file and header file in the corresponding subfolder.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  btn                |        button driver          |
|  lamp               |        lamp driver            |

#### Function Module

- The `function` folder contains the API implementations of functions.

|  folder/file        |            Function                                         |
| ------------------- | ------------------------------------------------------------|
|  lamp_work          |        lamp controller                                      |
|  lwm2m              |        LwM2M Client start to work                           |
|  print_msg          |        print out message for debug                          |
|  common.h           |        common variables, settings and reported data         |

- In the `common.h`, set 1 to enable corresponding function, set 0 to disable.

		/**
		 * \name    macros for settings
		 * @{
		 */
		#define LWM2M_CLIENT      (1) /*!< set 1 to be lwm2m client */
	
		#define PRINT_DEBUG_FUNC  (1) /*!< set 1 to print out message for debug major function */
		/** @} end of name */

#### LwM2M Client

- In the `lwm2m_client` folder, you can see several files about LwM2M. See [**LwM2M Protocol**][37] and [**LwM2M Object and Resource**][38] to learn more about it.

- The following objects are nessary, you ought to keep them:

|  file                         |
| ----------------------------- |
|  object_connectivity_stat.c   |
|  object_device.c              |
|  object_firmware.c            |
|  object_security.c            |
|  object_server.c              |

- Only the `object_flag_lamp_work.c` is custom here, you ought to remove it and add the new object definition files for your node. Then, modify `lwm2mclient.c`:

	The number of objects in your node:

		87 #define OBJ_COUNT 7

	Logic of reporting data to LwM2M Server:

		346 /* update the flag of lamp working value */
	            if (data_report_ln.flag_lamp_work != data_report_ln_old.flag_lamp_work)
		    {
			    lwm2m_stringToUri("/3311/0/5850", 12, &uri);
			    valueLength = sprintf(value, "%d", data_report_ln.flag_lamp_work);
			    handle_value_changed(context, &uri, value, valueLength);
			    data_report_ln_old.flag_lamp_work = data_report_ln.flag_lamp_work;
		    }
	
	Register custom objects:
	
		667 objArray[5] = get_lamp_object();
		    if (NULL == objArray[5]) {
			    EMBARC_PRINTF("Failed to create lamp object\r\n");
			    return -1;
		    }
	
	Finally, modify `lwm2mclient.h`:
	
		62   extern lwm2m_object_t * get_lamp_object();


[0]: ./doc/screenshots/ibaby_function.PNG         "ibaby_function"
[1]: ./doc/screenshots/system_architecture.PNG    "system_architecture"
[2]: ./doc/screenshots/freeboard_ui.png           "freeboard_ui"
[3]: ./doc/screenshots/wearable_node.jpg          "wearable_node"

[30]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[31]: http://store.digilentinc.com/pmodwifi-wifi-interface-802-11g/    "Digilent PMOD WiFi(MRF24WG0MA)"
[32]: https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/    "Acceleration sensor(MPU6050)"
[33]: http://www.electronics-lab.com/max30102/    "Heartrate sensor(MAX30102)"
[34]: https://developer.mbed.org/components/MLX90614-I2C-Infrared-Thermometer/    "Temperature sensor(MLX90614)"
[35]: https://github.com/XiangcaiHuang/ibaby.git    "iBaby Smarthome Gateway"
[36]: https://github.com/XiangcaiHuang/ibaby.git    "iBaby Freeboard UI"
[37]: http://www.openmobilealliance.org/release/LightweightM2M/V1_0_1-20170704-A/OMA-TS-LightweightM2M-V1_0_1-20170704-A.pdf    "LwM2M Protocol"
[38]: http://www.openmobilealliance.org/wp/OMNA/LwM2M/LwM2MRegistry.html#omalabel   "LwM2M Object and Resource"
[39]: http://www.freertos.org/a00106.html   "FreeRTOS API"



