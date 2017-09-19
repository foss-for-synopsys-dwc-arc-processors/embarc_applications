# iLight SmartDevice Application
This application is designed to show how to develop a **Smart Device** using embARC. iLight can be controlled by **gestures** and **iOS App**.It has lots of working modes and can make great convenience for you in your daily life.The connection between EMSK and the SmartPhone is based on **bluetooth**. Let's start a interesting trip with iLight!

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

* **Running mode**

	**The blinking light** will give you a more safe sports environment in your running at night.

	![Running_mode][2]

* **Alarm mode**

	The Blue light and the red light blinking alternately when it alarms.

	![Alarm_mode][3]

* **Riding mode**

	Turn to red light to remind other guys when you slow down or stop in your riding.
 
    ![Riding_mode][4]

* **Timing mode**

	Set time by the iOS App, it will show you the progress bar at timing and blink when the time is up.

	![Timing_mode][5]

* **Music mode**

	Blinking with the **rhythm** of music. So cool! Isn't it?

	![music_mode][6]

* **Weather mode**

	Show you the current **weather** by the light's color. It can be updated in real time via the iOS App.

* **Fans mode**

	**"Say" words** to you by shaking the iLight, and you can set the words via the iOS App.

### iOS App
iLight can be controlled by the [iLight IOS App][35]. Download it from the Appstore. Then, you can change the iLight's working mode using it, and it will be easier to modify the configuratons of iLight. For example, change the brightness in Running mode, Riding mode or Alarm mode, etc.

![app_pic][1]

#### Q&A
* How to **connect** it to the iLight?

	The App will connect the iLight automatically as soon as starting up, entering the default password: *000000*. Restart it when it fails.

* How to refresh the **weather**？

	The iLight gets weather information by the App in weather mode. Click **City select button** to choose the city, and the default is *Wuhan, CN*. Then, The App will get weather information automatically. If you want more information about the weather, click **Refresh button**. Then, the app will send it to the iLight to show.

* Can it show **Chinese character** in the fans mode?

	It doesn't support Chinese character now.

## Hardware and Software Setup
### Required Hardware

- 1 [DesignWare ARC EM Starter Kit(EMSK)][30]
- 1 [BLE HM-10 module][31]
- 1 [iLight bar(Homemade)][32]
- 1 SD Card
- 1 iOS SmartPhone

![ilight_hardware][7]

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
Download source code of **iLight SmartDevice** from github, and install [**iLight iOS App**][35] in your iOS smartphone.

The hardware resources are allocated as following table.

|  Hardware Resource  |            Function                                           |   
| ------------------- | ------------------------------------------------------------- |
|  iLight bar         |        integrated module                                      |
|  BLE HM-10 module   |        Provide Bluetooth Connection                           |

### Run This Application

Modify the settings for connecting iLight to the App, as shown below:

Start the bluetooth and app after iLight running on the EMSK.

Here take **EMSK2.2 - ARC EM11D** with GNU Toolset for example to show how to run this application.

1. We need to use embARC 2nd bootloader to automatically load application binary for different EMSK and run. See *embARC Secondary Bootloader Example* for reference.

2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.

3. Interact using EMSK and App.


#### Makefile

- Select FreeRTOS here, then you can use [FreeRTOS API][39] in your application:

		# Selected OS
		OS_SEL ?= freertos

- Target options about EMSK and toolchain:

		BOARD ?= emsk
		BD_VER ?= 22
		CUR_CORE ?= arcem11d
		TOOLCHAIN ?= gnu

- The relative series of the root directory, here the path of the Makefile is `./embarc_osp/application/ilight_smartdevice/src/makefile`:

		#
		# root dir of embARC
		#
		EMBARC_ROOT = ../../..

- Directories of source files and header files, notice that it **is not recursive**:

		# application source dirs
		APPL_CSRC_DIR = . ./function/os_task ./function/light_mode ./driver/mpu6050 ./function/imu ./function/scope ./function/interrupt ./driver/word ./function/mic ./driver/rtc ./driver/light
		APPL_ASMSRC_DIR = .

		# application include dirs
		APPL_INC_DIR = . ./function/os_task ./function/light_mode ./driver/mpu6050 ./function/imu ./function/scope ./function/interrupt ./driver/word ./function/mic ./driver/rtc ./driver/light

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
[1]: ./doc/screenshots/app_weather.jpg		"app_pic"
[2]: ./doc/screenshots/running_mode.gif
[3]: ./doc/screenshots/alarming_mode.gif
[4]: ./doc/screenshots/riding_mode.gif
[5]: ./doc/screenshots/timing_mode.gif
[6]: ./doc/screenshots/music_mode.gif
[7]: ./doc/screenshots/equipment.png

[31]:http://www.huamaosoft.cn/bluetooth.asp?id=0
[32]:http://pan.baidu.com/s/1geX2nNt
[30]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[35]:https://itunes.apple.com/cn/app/i-lighting/id1273641607?mt=8
[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"
[39]: http://www.freertos.org/a00106.html   "FreeRTOS API"