# Smart Home Monitoring System

The smart home monitoring system collects video information through a camera, and processes the data locally to monitor the situation of the elderly. The hardware modules mainly used in this project are IoT DK development board, OV7670 camera module, buzzer and display. The system can be used in more interesting applications in the future. For example, different behaviors of people can be judged according to different movement patterns, so as to realize the recognition of human activities.

* [Introduction](#introduction)
	* [System Architecture](#system-architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## Introduction
The smart home monitoring system has the functions of real-time monitoring of human activities, recognition of daily activities and falling actions, and real-time alarms for falling actions.


### System Architecture
![][4]

## Hardware and Software Setup
### Required Hardware
- [ARC IoT Development Kit][1]
- [OV7670 Camera Module][2]
- Buzzer Module
- Arduino

### Required Software
- ARC GNU Toolchain 2020.03
- Serial port terminal, such as putty, tera-term or minicom
- [embarc_osp(Branch:embarc_mli)][3]
- Python3.6
- Pycharm
- Tensorflow 2.0

### Hardware Connection
1. Connect OV7670 camera module to ARC following below instructions

        # ARC: 2x18 Pin Extension Header
        SIOC   ->     I2C0_SCL (need pull-up 10K resistor)
        SIOD   ->    I2C0_SDA (need pull-up 10K resistor)

        # ARC: Arduino PIN
        VSYNC  ->    PIN IO0
        OE         ->    PIN IO1
        WEN     ->    PIN IO2
        WRST    ->    PIN IO3
        D7~D0  ->    PIN IO4~11
        RCK      ->    PIN IO12
        RRST    ->    PIN IO13

        3V3    -> +3.3V
        RESET  -> +3.3V
        GND    -> GND
        PWDN   -> GND

2.  Connect Buzzer module to IoT DK 2x18 Pin Extension Header

3. Boot up ARC IoT Development Kit Board to start

4. Connect the USB port of ARC to the laptop to recieve  results

## User Manual
### Before Running This Application
* Download source code of SHMS from github
* Make sure all connection is correct again

### Run This Application
Here take IoT DK, CUR_CORE = arcem9d with ARC GNU Toolchain 2020.03 for example to show how to run this application.  
1. To build this applicaiton, select the proper board version, core configuration and build with selected toolchain using this command `make BOARD=iotdk BD_VER=10 CUR_CORE=arcem9d TOOLCHAIN=gnu run`.  
2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and 115200bps.  
3. Interact using IoT DK and serial port terminal. 
4. After ARC connect with the laptop via USB, type `python python/helper.py` in the terminal to start recieving the results.

[1]: https://embarc.org/embarc_osp/doc/build/html/board/iotdk.html "ARC IoT Development Kit"
[2]: https://www.voti.nl/docs/OV7670.pdf "OV7670 Camera Module"
[3]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp.git "embarc_osp(Branch:embarc_mli)"
[4]: ./doc/system.png "System Architecture"
