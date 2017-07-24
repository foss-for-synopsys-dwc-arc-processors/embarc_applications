# iBaby Smarthome Application
This application is designed to show how to connect 1 or more EMSKs and **iBaby Smarthome Gateway** using embARC. The connection between EMSK and Gateway is based on **LwM2M** protocol as shown in the following figure. All the iBaby nodes interact with AWS IoT through the Gateway indirectly.

## Introduction
**iBaby Infant Sleep Monitoring System**

### Function

![ibaby_function](http://i.imgur.com/tspan1C.png)

### System architecture

![system_architecture](http://i.imgur.com/3hqxrJJ.png)

### iBaby Freeboard UI

![freeboard_ui](http://i.imgur.com/FCIUjcn.png)

## Hardware and Software Setup
### Required Hardware
- 1 [DesignWare ARC EM Starter Kit(EMSK)](https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit)
- 1 [Digilent PMOD WiFi(MRF24WG0MA)](http://store.digilentinc.com/pmodwifi-wifi-interface-802-11g/)
- 1 [Acceleration sensor MPU6050](https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/)
- 1 [Heartrate sensor MAX30102](http://www.electronics-lab.com/max30102/)
- 1 [Temperature sensor MLX90614](https://developer.mbed.org/components/MLX90614-I2C-Infrared-Thermometer/)
- 1 SD Card
- WiFi Hotspot(SSID:**embARC**, Password:**qazwsxedc**, WPA/WPA2 encypted)
- PC or Raspberry Pi for running iBaby Smarthome Gateway program

We designed integrated module for 3 sensors above, I'll call it **Foot Ring Module**. The physical picture of wearable node is as follows:

![wearable_node](http://i.imgur.com/QofeNaG.jpg)

### Required Software
- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom
- [iBaby Smarthome Gateway](https://github.com/XiangcaiHuang/ibaby.git)
- [iBaby Freeboard UI](https://github.com/XiangcaiHuang/ibaby.git)

### Hardware Connection
1. EMSK 1 implement **wearable** node, as an intelligent foot ring for baby, it will publish baby's status to AWS IoT Cloud via the Gateway, including body temperature, heartrate, sleep-wake state, motion intensity and some warning information. We can view all data on the Freeboard UI.
   - Connect PMOD WiFi to J5, connect Foot Ring Module to J4.
2. Configure your EMSKs with proper core configuration.

## User Manual
### Before running this application
Firstly, download source code of **iBaby Smarthome Gateway** and **iBaby Freeboard UI** from github. Then you need an AWS account, and create things for ibaby nodes, generate and save the certs for different nodes, and modify specific `config.js`(path: ./ibaby_smarthome_gateway/config.js) for your project.

Secondly, run the iBaby Smarthome Gateway, open the browser and type IP address of the Gateway to access user interface. You can push iBaby Freeboard UI to github so that it can support remote access data.

|  EMSK Implemented Node   |    AWS IoT Thing      |     Source Folder      |
| ------------------------ | --------------------- | ---------------------- |
|      wearable node       |        ibaby          |   src/wearable_node    |

The hardware resources are allocated as following table.

|  Hardware Resource  |            Function                                           |
| ------------------- | ------------------------------------------------------------- |
|  MPU6050            | Acceleration sensor                                           |
|  MAX30102           | Heartrate sensor                                              |
|  MLX90614           | Body temperature sensor                                       |
|  PMOD WiFi          | Provide WiFi Connection                                       |

### Run this application

Modify the setting for connecting to the LwM2M Server(Gateway), as shown below (path: src/wearable_node/function/lwm2m.c) :

		const static char *p_port   = (char *)"5683";    /* lwm2mServer's port and IP */
		const static char *p_server = (char *)"192.168.43.199";
		const static char *p_client_name = (char *)"wn"; /* name of lwm2m client node */

Here take **EMSK2.2 - ARC EM11D** with Metaware Toolset for example to show how to run this application.

1. We need to use embARC 2nd bootloader to automatically load application binary for different EMSK and run.

2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.

3. Interact using EMSK and Freeboard.
