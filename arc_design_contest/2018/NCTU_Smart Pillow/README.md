# Smart Pillow Application 
*Hong-Shuo Chen National Chiao Tung University<br>
Bo-Ying Chen National Tsing Hua University<br>
Kuan-Cheng Chen National Chiao Tung University*

Smart pillow uses the breath frequency and turns of user. The algorithms processes the information and determine what sleep stages are users in. thus we can analyze the sleep cycle, and avoid waking users when they are in deep sleep.  Moreover, for those who suffer from sleep apnea, when smart pillow detect no breath for a period of time, smart pillow will vibrate gently to make user to start to breathe.
The smart pillow is an innovation that are combined with Bluetooth, acoustic sensor, pressure sensor and motor. Acoustic sensor and pressure sensor are used to provide information for analyzing sleep, while motor is used to wake people and for those who have sleep apnea. Bluetooth can send message to smart phone.
We hope we can popularize the smart pillow in the future, smart pillow records and analyzes the sleep habit of users, through the information, is can wake users in proper time, improve the quality of sleep, and reduce sleep problem.


* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#system-architecture)
	* [APP UI](#app-ui)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## Introduction


### Function

- **Analysis Sleeping status and Best Waking up Time**
- **Sleep monitoring** (Alarm of no breath while sleeping)
- Communicate with **APP**
- **Alarm Clock** Setting 
- **Music** based on Uart protocol (Supports file umlimited songs)
- **Alexa** Voice assistant, connecting and communicating with Wifi.
- **CPU Loading** Calculate the power used by the board



### System Architecture

![](https://i.imgur.com/9CH7Vob.png)


### APP UI


| Main page | Clock | Record |
| -------- | -------- | -------- |
| ![](https://i.imgur.com/NNz1bgZ.png)     | ![](https://i.imgur.com/y4yej8B.png)     | ![](https://i.imgur.com/M8xJkWT.png)     |





## Hardware and Software Setup
### Required Hardware
- 1 DesignWare ARC EM Starter Kit(EMSK)
- 1 Sound Detect Sensor
- 3 Pressure sensor
- 2 Pmod AD2: 4-channel 12-bit A/D Converter(AD7991)
- 1 Vibration motor
- 1 DFPlayer Mini
- 1 Speaker
- 1 LED Light
- 1 Bluetooth sensor
- 1 SD Card
- WiFi Hotspot
- Alexa


### Required Software
- ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom


### Hardware Connection
* Connect Bluetooth to J1
* Connect Pmod AD2 to J2 and J4
* Connect pressure sensors and Sound Detect Sensor to Pmod AD2
* Connect DFPlayer Mini to J5
* Connect LED Light and Vibration motor to J3
* Connect speaker to DFPlayer Mini

![](https://i.imgur.com/jDXibCu.jpg)

![](https://i.imgur.com/oZA201y.jpg)

## User Manual
### Before Running This Application
In order to open **UART_2**<br>
We need to modify following files

* emsk_init.c (board\emsk\common)
```
set_pmod_mux(mux_regs, PM1_UR_UART_0 | PM1_LR_SPI_S	\
				| PM2_I2C_HRI			\
				| PM3_GPIO_AC			\
				| PM4_I2C_GPIO_D		\
				| PM5_UR_SPI_M1 | PM5_LR_UART_2	\
				| PM6_UR_SPI_M0 | PM6_LR_GPIO_A );
```
* mux.h (board\emsk\drivers)
```
/**
 * \name	PMOD 5 Multiplexor
 * @{
 */
#define PM5_UR_GPIO_C		((0 << BIT0) << PM5_OFFSET)	/*!< Pmod5[4:1] are connected to DW GPIO Port C[27:24] */
#define PM5_UR_SPI_M1		((1 << BIT0) << PM5_OFFSET)	/*!< Pmod5[4:1] are connected to DW SPI Master signals using CS1_N */

#define PM5_LR_GPIO_A		((0 << BIT2) << PM5_OFFSET)	/*!< Pmod5[10:7] are connected to DW GPIO Port A[27:24] */
#define PM5_LR_UART_2		((1 << BIT2) << PM5_OFFSET)	/*!< Pmod5[10:7] are connected to DW UART Master signals using CS2_N */
/** @} end of name */
```
* dw_uart_obj.h (board\emsk\drivers)
* dw_uart_obj.c (board\emsk\drivers)

In order to calculate **CPU Loading**<br>
We need to modify following files
* task.h (os\freertos)
* task.c (os\freertos\inc)
### Run This Application

Here take **EMSK2.2 - ARC EM7DFPU**

1. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.

2. Interact using EMSK.


|  folder/file        |            Function                                           |
| ------------------- | ------------------------------------------------------------- |
|  FreeRTOSConfig.h   |        header file of FreeRTOS configurations                 |
|  main.c             |        main entry of embARC Application                       |
|  makefile           |        Makefile of embARC Application                         |

#### Makefile

- Selected FreeRTOS here, then you can use [FreeRTOS API][39] in your application:

        # Application name
        APPL ?= freertos_kernel

        # Selected OS
        OS_SEL ?= freertos
        
- Target options about EMSK and toolchain:

		TOOLCHAIN=gnu
        BOARD=emsk
        BD_VER=22
        CUR_CORE=arcem7d

- The relative series of the root directory, here the path of the Makefile is `./embarc_osp/`:

		#
        # root dir of embARC
        #
        EMBARC_ROOT = ../embARC

- The middleware used in your application:

		MID_SEL = common


	*common* for baremetal function

- Directories of source files and header files:

		# application source dirs
        APPL_CSRC_DIR = .
        APPL_ASMSRC_DIR = .

        # application include dirs
        APPL_INC_DIR = .

        # include current project makefile
        COMMON_COMPILE_PREREQUISITES += makefile

        ### Options above must be added before include options.mk ###
        # include key embARC build system makefile
        override EMBARC_ROOT := $(strip $(subst \,/,$(EMBARC_ROOT)))
        include $(EMBARC_ROOT)/options/options.mk

## Video
*   [SMART PILLOW](http://v.youku.com/v_show/id_XMzYxMzUxNjE2NA==.html?spm=a2hzp.8244740.0.0)
*   [SMART PILLOW 1080P](http://v.youku.com/v_show/id_XMzYxMzUxMTI5Ng.html?spm=a2h0j.11185381.listitem_page1.5~A)

For complete source code<br>
Please go to https://github.com/max2468tw/Smart_Pillow