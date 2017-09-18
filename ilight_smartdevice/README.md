# iLight SmartDevice Application
This application is designed to show how to develop a **Smart Device** using embARC. iLight can be controlled by gestures and iOS App.It has lots of working modes and can make great convenience for you in your daily life.The connection between EMSK and the SmartPhone is based on bluetooth. Let's start a interesting trip with iLight!

* [Introduction](#introduction)
	* [Function](#function)
	* [iOS App](#ios-app)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## Introduction

**iLight** 
 
 iLight is a smart device which can be used to make convenience for you in your daily life.

### Function

* Running mode. It is used when you are running in the night and can provide you safety.
* Alarm mode.Blue light and red light alternating flashing.
* Riding mode.It will light in red when you decrease you speed and in colors when you ride in uniform speed.
* Timing mode.You can set the time by your mobilephone and turn off the lights in same time.
* Music mode.It can get the data of music by mic and switch lights according to the rhythm of music.
* Weather mode.The iLight can get the data of weather by ble and show the data by color of lights.
* Shaking mode.You can set those words which you want to show and shake the iLight.
* 

### iOS App

* 

## Hardware and Software Setup
### Required Hardware

- 1 [DesignWare ARC EM Starter Kit(EMSK)][30]
- 1 [BLE HM-10 module][31]
- 1 [iLight bar(Homemade)][32]
- 1 SD Card
- 1 iOS SmartPhone

The iLight bar is a integrated module made by ourselves. The physical picture of it is as follows:

![ilight_bar][0]

### Required Software
- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom
- [iLight iOS App][35]

### Hardware Connection
1. The EMSK implement **iLight** SmartDevice, it will publish the iLight connected to the App via bluetooth, including mic, acceleration sensor,We can view all data on the App UI. And the SmartPhone will send instructions to it.
   - Connect **BLE HM-10 module** to **J1**(Using UART interface), connect **Middle control pin** to **J1**,connect **MPU050 and PCF8591** to **J3**.
2. Configure your EMSKs with proper core configuration.

## User Manual
### Before Running This Application
Firstly, download source code of **iLight SmartDevice** from github, and install **iLight iOS App** in your iOS smartphone.

Secondly,you have to install our app by downloading it in APPLESTORE if you want to have a better experience.

The hardware resources are allocated as following table.

|  Hardware Resource  |            Function                                           |   
| ------------------- | ------------------------------------------------------------- |
|  iLight bar         |        integrated module                                      |
|  BLE HM-10 module   |        Provide Bluetooth Connection                           |

### Run This Application

Modify the settings for connecting to the App, as shown below:

Open your ble and app after thie application is running.

Here take **EMSK2.2 - ARC EM11D** with GNU Toolset for example to show how to run this application.

1. We need to use embARC 2nd bootloader to automatically load application binary for different EMSK and run. See *embARC Secondary Bootloader Example* for reference.

2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.

3. Interact using EMSK and App.


#### Makefile

- Selected FreeRTOS here, then you can use [FreeRTOS API][39] in your application:

		# Selected OS
		OS_SEL ?= freertos

- Target options about EMSK and toolchain:

		BOARD ?= emsk
		BD_VER ?= 22
		CUR_CORE ?= arcem11d
		TOOLCHAIN ?= gnu

- The relative series of the root directory, here the path of the Makefile is `./embarc_osp/application/ibaby_smarthome_multinode/src/lamp_node/makefile`:

		#
		# root dir of embARC
		#
		EMBARC_ROOT = ../../../..

- Directories of source files and header files, notice that it **is not recursive**:

		# application source dirs
		APPL_CSRC_DIR = . ./function/light_mode ./driver/mpu6050 ./function/imu ./function/scope ./function/interrupt ./driver/word ./function/mic ./driver/rtc ./driver/light
		APPL_ASMSRC_DIR = .

		# application include dirs
		APPL_INC_DIR = . ./function/light_mode ./driver/mpu6050 ./function/imu ./function/scope ./function/interrupt ./driver/word ./function/mic ./driver/rtc ./driver/light
		
		APPL_DEFINES =

See [ embARC Example User Guide][40], **"Options to Hard-Code in the Application Makefile"** for more detailed information about **Makefile Options**.

#### Driver

Placing the drivers' source code in `driver` folder, you can see there are subfolders for light,mpu6050,rtc and word drivers.
Placing the C source file and header file in the corresponding subfolder.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  light_driver       |        light(ws2812) driver   |
|  mpu6050            |        mpu6050 driver         |
|  rtc                |     rtc(pcf8563t) driver      |
|  word               |    models of english alphabet |

#### Function Module

- The `function` folder contains the API implementations of functions.

	|  folder/file        |            Function                                         |
	| ------------------- | ------------------------------------------------------------|
	|  imu                |        action recongnition                                  |
	|  interrupt          |        get and deal with data from ble                      |
	|  light_mode         |        working modes.                                       |
	|  mic                |        get data of voice and ouput                          |
    |  scope              |        output data and build it in visual scope             |
[0]: ./doc/img/hard_ware.JPG        "iLight_hardware"
[31]:http://www.huamaosoft.cn/bluetooth.asp?id=0
[32]:http://pan.baidu.com/s/1geX2nNt
[30]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[35]:https://itunes.apple.com/cn/app/i-lighting/id1273641607?mt=8
[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"
[39]: http://www.freertos.org/a00106.html   "FreeRTOS API"