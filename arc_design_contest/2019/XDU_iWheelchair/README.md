# Intelligent-Wheelchair

This application is designed to apply to the person who can’t use arms or legs well but
without language barriers. The intelligent wheelchair is based on voice control
and can be operated independently through voice commands. Moreover,
the application has a collision- avoidance and warning system. Wheelchair  also can be control by smartphone APP through
Bluetooth. The feature of this application is front-end processing rather than
cloud computing.

- [Introduction](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#introduction)
  - [Function](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#function)
  - [System Architecture](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#system-architecture)
  - [APP UI](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#app-ui)
- [HW/SW Setup](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#hwsw-setup)
  - [Required Hardware](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#required-hardware)
  - [Required Software](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#required-software)
  - [Hardware Connection](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#hardware-connection)
- [User manual](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#user-manual)
  - [Before Running This Application](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#before-running-this-application)
  - [Run This Application](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/README.md#run-this-application)

## Introduction

![wheelchair.jpg](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/doc/picture/wheelchair.jpg)

### Function

- Voice control through front-end processing
- Communicate with APP 
- Collision auto-avoidance and warning system

### System Architecture

![frame.png](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/doc/picture/frame.png)

### APP UI

![HMSoft.png](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/doc/picture/HMSoft.png)

## HW/SW Setup

### Required Hardware

- 1 DesignWare ARC EM Starter Kit(EMSK)
- 1 WEGASUN-M6(Speech recognition)
- 1 HM-10 Bluetooth
- 1 Pmod AD2: 4-channel 12-bit A/D Converter(AD7991)
- 2 Infrared range sensor
- 1 L298N Two lines of motor driver
- 2 Dc Motor
- 1 Speaker
- 1 Inclination Sensor
- 1 MIC
- 1 SD Card 
- 1 LED Light
- 1 buzzer
- 1 5V Booster module

### Required Software

- embARC Open Software Platform(OSP)
- ARC GNU Tool Chain
- Serial port terminal, such as putty
- [HMSoft](https://github.com/YuaniLee/embarc_applications/blob/master/arc_design_contest/2019/XDU_iWheelchair/app/app-release.apk)
- [M6SE-IDE](https://github.com/YuaniLee/embarc_applications/blob/master/arc_design_contest/2019/XDU_iWheelchair/app/M6SE-IDE.exe)

### Hardware Connection

- Connect Bluetooth to J1
- Connect Pmod AD2 to J2
- Connect Infrared range sensor and Inclination Sensor to Pmod AD2
- Connect buzzer and  LED Light to J3
- Connect WEGASUN-M6 to J5
- Connect L298N motor driver to J6
- Connect Speaker and MIC to WEGASUN-M6

![hardware.png](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/doc/picture/hardware.png)

## User Manual

### Before Running This Application

In order to open  **UART2** we need to modify following files

- Modify mux.c (/board/emsk/common/emsk_init.c)

```
set_pmod_mux(PM1_UR_UART_0 | PM1_LR_SPI_S	\
				| PM2_I2C_HRI		\
				| PM3_GPIO_AC		\
				| PM4_I2C_GPIO_D	\
				| PM5_UR_SPI_M1 | PM5_LR_SPI_M2	\
				| PM6_UR_GPIO_C | PM6_LR_GPIO_A );
```

- Replace following files:

  [dw_uart_obj.h(board\emsk\drivers)](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/dw_uart_obj.h )

  [dw_uart_obj.c(board\emsk\drivers)](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/dw_uart_obj.c)

#### Speech recognition config

Connect WEGASUN-M6 to PC by USB to serial port

![M6.jpg](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/doc/picture/M6.jpg)

Open [M6SE-IDE](https://github.com/YuaniLee/embarc_applications/blob/master/arc_design_contest/2019/XDU_iWheelchair/app/M6SE-IDE.exe) with the following settings:

![M6SE-IDE](https://github.com/YuaniLee/Intelligent-Wheelchair/blob/master/doc/picture/M6SE-IDE.JPG)

| Settings                 | Specification                                                |
| ------------------------ | ------------------------------------------------------------ |
| @Baud#9600$              | Baud rate                                                    |
| @AsrSET#1,4,06000,6000,$ | data1: recognized distance(20 cm)；data2: match degree; data3: waiting time; data4: recording time |

#### Build and run

| file             | function                             |
| :--------------- | ------------------------------------ |
| ble.c            | Bluetooth initialization and control |
| M6.c             | Voice initialization and control     |
| buzzer.c         | Warning system                       |
| driver_control.c | Control L298N drive motor            |
| main.c           | Main entry of embARC Application     |
| makefile         | Makefile of embARC Application       |



##### Makefile

- Target options about EMSK and toolchain:

  ```
  BOARD ?= emsk
  TOOLCHAIN ?= gnu
  OLEVEL ?= 02
  BD_VER ?= 22
  CUR_CORE ?= arcem7d
  ```

- DEV config

  ```
  EXT_DEV_LIST += ble/hm1x
  DEV_CSRCDIR +=  $(EMBARC_ROOT)/device/peripheral/adc/ad7991
  DEV_INCDIR +=  $(EMBARC_ROOT)/device/peripheral/adc/ad7991
  ```

- The middleware used in your application:

  ```
   MID_SEL = common
  ```

- Directories of source files and header files:

  ```
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
  ```

Then`make run`

## Video

[Show video](https://v.youku.com/v_show/id_XNDI5MjAyNzUwOA==.html?spm=a2h0k.11417342.soresults.dtitle
)
