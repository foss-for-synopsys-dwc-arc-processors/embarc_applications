# iBaby Smarthome Application
This application is designed to show how to connect 1 or more EMSKs and **iBaby Smarthome Gateway** using embARC. The connection between EMSK and Gateway is based on **LwM2M** protocol as shown in the following figure. All the iBaby nodes interact with AWS IoT through the Gateway indirectly. There are only 2 nodes available now, but you can add more nodes easily to iBaby Smarthome by taking these 2 examples as a reference. And the lamp node is more simple and easy to learn for beginner.

* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#system-architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

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

![wearable_node][2]

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
Firstly, download source code of **iBaby Smarthome Gateway** and **iBaby Freeboard UI** from github. Then you need an AWS account, and create things for ibaby nodes, generate and save the certs for different nodes, and modify specific `config.js`(path: `./ibaby_smarthome_gateway/config.js`) for your project.

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

[0]: ./doc/screenshots/ibaby_function.PNG         "ibaby_function"
[1]: ./doc/screenshots/system_architecture.PNG    "system_architecture"
[2]: ./doc/screenshots/wearable_node.jpg          "wearable_node"

[30]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[31]: http://store.digilentinc.com/pmodwifi-wifi-interface-802-11g/    "Digilent PMOD WiFi(MRF24WG0MA)"
[32]: https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/    "Acceleration sensor(MPU6050)"
[33]: http://www.electronics-lab.com/max30102/    "Heartrate sensor(MAX30102)"
[34]: https://developer.mbed.org/components/MLX90614-I2C-Infrared-Thermometer/    "Temperature sensor(MLX90614)"
[35]: https://github.com/XiangcaiHuang/ibaby.git    "iBaby Smarthome Gateway"
[36]: https://github.com/XiangcaiHuang/ibaby.git    "iBaby Freeboard UI"

