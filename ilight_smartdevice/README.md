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

* **Running mode**. It is used when you are running in the night and can provide you safety.   
	![Running_mode][2]
* **Alarm mode**.Blue light and red light alternating flashing.   
    ![Alarm_mode][3]
* **Riding mode**.It will light in red when you decrease your speed and in colors when you ride in uniform speed.   
    ![Riding_mode][4]
* **Timing mode**.You can set the time by your mobilephone and turn off the lights in same time.
	![Timing_mode][5]
* **Music mode**.It can get the data of music by mic and switch lights according to the rhythm of music.
	![music_mode][6]
* **Weather mode**.The iLight can get the data of weather by ble and show the data by color of lights.
* **Shaking mode**.You can set those words which you want to show and shake the iLight.

### iOS App
iLight can be controlled by an IOS App [I-Lighting][35], you can download it in Appstore. Basically you can use the App to change modes of iLight, and  you can make some detailed settings to iLight. For example,  you can set the brightness of iLight in Running mode、Riding mode and Alarm mode, etc. You can set the time in Timing mode.   
#### Q&A
* **How to connect to the iLight device?**   
  The App will automatically connect the iLight device when you start the App and please input the default password "000000" . If you can`t control the iLight device,  please restart the App.
* **How to fetch weather information？**   
  Weather mode fetchs weather information by the App, At first you can click the "City select button" to change the city where you are, the default city is "Wuhan,CN" . The App will automatically fetch weather information once when you start it, If you needs more times weather information, please click the "Refresh button". And the App will send the weather data to the iLight device to show.   
* **Can we input chinese in fans mode?**   
  ilight can`t shows chinese in fans mode right now.   
 
![app_pic][1]  
### Control method
![ilight_hardware][8]

## Hardware and Software Setup
### Required Hardware

- 1 [DesignWare ARC EM Starter Kit(EMSK)][30]
- 1 [BLE HM-10 module][31]
- 1 [iLight bar(Homemade)][32]
- 1 SD Card
- 1 iOS SmartPhone    

* **The list of haraware is shown in the picture following.**     
	![ilight_hardware][7]    

* **The iLight bar is a integrated module made by ourselves and the design of the PCB board is shown below.**
    ![ilight_bar][9]   
	* **MCU links**.Connect the signal transmission pin to the EMSK.
	* **USB charging module**.Using tp4056 power management chip to charge the lithium battery.
	* **Regulator module**.Using AS1015, 3.3V switching regulator chip, conversion efficiency, no heat phenomenon.
	* **Audiow information acquisition module**.Collect audio signals and process them.
	* **Motion sensor interface**.Use the MPU6050 motion sensor module to obtain the acceleration signal.

* **The physical picture shown below.**   
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

The `function` folder contains the API implementations of functions.

	|  folder/file        |            Function                                         |
	| ------------------- | ------------------------------------------------------------|
	|  imu                |        action recongnition                                  |
	|  interrupt          |        get and deal with data from ble                      |
	|  light_mode         |        working modes.                                       |
	|  mic                |        get data of voice and ouput                          |
    |  scope              |        output data and build it in visual scope             |
[0]: ./doc/screenshots/hard_ware.JPG        "iLight_hardware"
[1]: ./doc/screenshots/app_weather.JPG		"app_pic"
[2]: ./doc/screenshots/running_mode.gif
[3]: ./doc/screenshots/alarming_mode.gif
[4]: ./doc/screenshots/riding_mode.gif
[5]: ./doc/screenshots/timing_mode.gif
[6]: ./doc/screenshots/music_mode.gif
[7]: ./doc/screenshots/equipment.png
[8]: ./doc/screenshots/control_method.png
[9]: ./doc/screenshots/board_design.png
[30]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[31]:http://www.huamaosoft.cn/bluetooth.asp?id=0
[32]:http://pan.baidu.com/s/1geX2nNt
[35]:https://itunes.apple.com/cn/app/i-lighting/id1273641607?mt=8
[39]: http://www.freertos.org/a00106.html   "FreeRTOS API"
[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"