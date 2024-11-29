# Voice Control Meal Delivery Car
This is a car make for Meal Delivery, which can go through the route that been set by voice control or by direct coding. And include two ultra-sonic distance sensor to detect whether the car has been blocked, at the same time keep updating the information about the car to the cloud, and can be seen by the kitchen or the cook. Most of the proses will be calculated on IOTDK.

* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#system-architecture)
	* [Control Method](#control-method)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## Introduction
**Voice Control Meal Delivery**

### Function

- **Set the route that the car can deliver meals to the right table number**
- **Voice control** (To only control the car or set the route step by step with voice.)
- **Cloud management** (The meal provider can know every car's states through online cloud)
- **Front Detection** (There are two distance detecter to stop the car from accidentally crash into something)
- **Controller based**(Through improving the Controller we can make the car move more smoothly and precisely)

![all-sys][0]

### System Architecture

![system_architecture][1]    
![system_architecture_2][7]
**Timer interrupt**
![code-interr][8]

### Control Method

![controlleri][2]

## Hardware and Software Setup
### Required Hardware
- 1 [ARC IoT Development Kit(IOTDK)][30]
- 2 Li-ion Battery 18650 3.7v
- 1 Li-ion Battery Box with MD line(for protect board)
- 1 2S 10A 18650 Lithium Battery Protection Board
- 1 XL6009 Boost Buck DC Adjustable Step Up Down Converter
- 1 L298N DC Motor Driver Dual H Bridge
- 1 SPH0645LM4H I2S Microphone
- 2 Motor with Hall Sensor 
- 2 HC-SR04 Ultrasonic Distance Rangefinder
- 1 NodeMCU ESP8266 Lua WiFi Internet Development Board
- WiFi Hotspot(SSID:**Prophet**, Password:**1mProphet**, WPA/WPA2 encypted)
- PC to write the code onto the board

This is the car been assambled 
![whole_car][10]

![whole_car_2][11]

### Required Software
- embarc_osp
- ARC Gnu IDE
- digilent.adept

### Hardware Connection
Connect the IOTDK through micor USB to write the code.
And the wires connection is down below.
![hardware_connect][12]
![power][13]

## User Manual
### Before Running This Application
You have to download thw embarc_ops and install the IDE and Driver, and download the code from the github,but because the voice recognition model is from **Cyberon** so you have to get licence from it. 

And set up the environment by follow the [guide][31].
To use NodeMCU, you have to set up the arduino IDE according to your NodeMCU WiFi board version.

And download the **wifi** folder for NodeMCU WiFi module.

### Run This Application

Under Win10, put **src/voice** folder in the embarc_osp(from the synopsys or the [link][31]) open powershell at the voice folder,

(path: `embarc_osp/voice`):

		make BOARD=iotdk TOOLCHAIN=gnu run
![screenshot_powershell][14]

And you can see some recognition result with putty through serial port.

![putty][15]





[0]: ./doc/pic/all.png         "all-sys"
[1]: ./doc/pic/code_structure.png    "system_architecture"
[2]: ./doc/pic/controller.png 			  "controller"
[3]: ./doc/screenshots/wearable_node.jpg          "wearable_node"
[4]: ./doc/screenshots/wifi_connected_info.PNG    "wifi_connected_info"
[5]: ./doc/screenshots/lwm2m_started_info.PNG     "lwm2m_started_info"
[6]: ./doc/screenshots/lamp_work_info.PNG         "lamp_work_info"
[7]: ./doc/pic/code_struction_2.png 			  "system_architecture_2"
[8]: ./doc/pic/code-interr.png 			  "code-interr"
[9]: ./doc/pic/controller.png 			  "controller"
[10]: ./doc/pic/whole_car.png 			  "whole_car"
[11]: ./doc/pic/whole_car_2.png 			"whole_car_2"
[12]: ./doc/pic/hardware_connect.png 		"hardware_connect"
[13]: ./doc/pic/power.png 		"power"
[14]: ./doc/pic/powershell.PNG 		"screenshot_powershell"
[15]: ./doc/pic/putty.PNG 		"putty"


[30]: https://embarc.org/embarc_osp/doc/build/html/board/iotdk.html    "v"
[31]: https://embarc.org/embarc_osp/doc/build/html/getting_started/getting_started.html
[32]: https://www.invensense.com/products/motion-tracking/6-axis/mpu-6050/    "Acceleration sensor(MPU6050)"
[33]: http://www.electronics-lab.com/max30102/    "Heartrate sensor(MAX30102)"
[34]: https://developer.mbed.org/components/MLX90614-I2C-Infrared-Thermometer/    "Temperature sensor(MLX90614)"
[35]: https://github.com/XiangcaiHuang/ibaby.git    "iBaby Smarthome Gateway"
[36]: https://github.com/XiangcaiHuang/ibaby.git    "iBaby Freeboard UI"
[37]: http://www.openmobilealliance.org/release/LightweightM2M/V1_0_1-20170704-A/OMA-TS-LightweightM2M-V1_0_1-20170704-A.pdf    "LwM2M Protocol"
[38]: http://www.openmobilealliance.org/wp/OMNA/LwM2M/LwM2MRegistry.html#omalabel   "LwM2M Object and Resource"
[39]: http://www.freertos.org/a00106.html   "FreeRTOS API"
[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"
