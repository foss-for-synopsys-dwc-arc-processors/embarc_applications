# Smart Fire-Fighter Application

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
* [DemoVideo](#demovideo)

## Introduction
This project uses ARC EM Starter Kit to build a smart Firefighter. A smart firefighter is a system which can detect and locate the fire and control the extinguisher to extinguish the fire early. The proposed system uses two Infrared sensor arrays with ARC EM Starter Kit to create a map of the fire which enables it to detect and locate the fire. After that, a control circuit with the robot arm can be used to direct the extinguisher to the direction of the fire. This smart firefighter can reduce the damages to the furniture. AI can be applied as a future work to build a safer firefighter by identifying the type of fire and selecting the correct extinguisher.

### Function
- Quick and accurate detection of flame location.
- Detect small size of flame (a candle at 3m).
- Spray water flow to the fire correctly, while keeping other areas dry spear.
- Can be mobility.


### System Architecture
![image](https://github.com/mrninhvn/2019-Synopsys-ARC-Firefighter/blob/master/detailed%20picture/system.png  "System architecture")

## Hardware and Software Setup
### Required Hardware
- 1 DesignWare ARC EM Starter Kit(EMSK)
- 2 stepper driver
- 2 stepper motor
- 2 IR camera
- 1 water pump
- 1 relay module
- 1 2-axis robot arm
- 1 12v5A adapter

### Required Software
- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom

### Hardware Connection
1.
   - Connect **2 Stepper driver module** to **J3**.
   - Connect **2 IR camera module** to **J1** & **J5** (using UART).
   - Connect **Relay module** to **J6**.
2. Configure your EMSKs with proper core configuration.

## User Manual
### Before Running This Application
Download source code of from github.                       

- Modify mux.c (/board/emsk/common/emsk_init.c)
```
line 107: change 
	set_pmod_mux(PM1_UR_UART_0 | PM1_LR_SPI_S	\
				| PM2_I2C_HRI		\
				| PM3_GPIO_AC		\
				| PM4_I2C_GPIO_D	\
				| PM5_UR_SPI_M1 | PM5_LR_GPIO_A	\
				| PM6_UR_SPI_M0 | PM6_LR_GPIO_A );
 to 
	set_pmod_mux(PM1_UR_UART_0 | PM1_LR_GPIO_A	\
				| PM2_I2C_HRI			\
				| PM3_GPIO_AC			\
				| PM4_I2C_GPIO_D		\
				| PM5_UR_GPIO_C | PM5_LR_SPI_M2	\
				| PM6_UR_GPIO_C | PM6_LR_GPIO_A );
```
### Run This Application

Here take **EMSK2.3 - ARC EM7D** with GNU Toolset for example to show how to run this application.

#### Makefile

- Target options about EMSK and toolchain:

		BOARD ?= emsk
		BD_VER ?= 23
		CUR_CORE ?= arcem7d
		TOOLCHAIN ?= gnu

- The relative series of the root directory, here the path of the Makefile is 

		#
		# root dir of embARC
		#
		EMBARC_ROOT = ../../../../
		MID_SEL = common u8glib


See [ embARC Example User Guide][40], **"Options to Hard-Code in the Application Makefile"** for more detailed information about **Makefile Options**.

# DemoVideo
https://www.youtube.com/watch?v=aEobmLNDtfw
