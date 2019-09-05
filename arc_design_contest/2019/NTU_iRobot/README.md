# iRobot - Front-Vehicle Detection to Enhance Drivers' Awareness

This application, which implements a image semantic segmentation on ARC IoT board, is designed to show how to use OV7670 camera and Machine Learning in embARC to achieve front-vehicle detection to enhance drivers' awareness. However, ultimately, we want to demonstrate that ARC processor can achieve scene understanding to do much more insteresting application in the future. For example, smart movement robot like household robot cleaner.

* [Introduction](#introduction)
	* [iRobot System Architecture](#system-architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## iRobot Introduction
iRobot is trying to enhance dirver's awarenessby by utilizing image segmentation. We can segment the vehicles in front of the current driver and warn if necessary.


### System Architecture
![][3]

## Hardware and Software Setup
### Required Hardware
- [ARC IoT Development Kit][1]
- [OV7670 Camera Module][2]
- [Arduino]
- [HC-05]
- [Raspberry Pi]
- [Micro-Robot Car]

### Required Software
- ARC GNU Toolset 2019
- Serial port terminal, such as putty, tera-term or minicom
- embARC MLI

### Hardware Connection
1. Connect OV7670 camera module to ARC following below instructions

        # ARC: 2x18 Pin Extension Header
        SIOC   ->     I2C0_SCL (need pull-up 10K resistor)
        SIOD   ->    I2C0_SDA (need pull-up 10K resistor)

        # ARC: Arduino PIN
        VSYBC  ->    arduino IO0
        PCLK   ->    arduino IO1
        XCLK   ->    arduino IO3
        D7~D0  ->    arduino IO4~IO11

        3V3    -> +3.3V
        RESET  -> +3.3V
        GND    -> GND
        PWDN   -> GND

2. (Optional) Connect HC-05 to arduino and prepared the necessary micro-robot car

3. Boot up ARC IoT Development Kit Board to start ML segmentation

4. (Optional) Boot up Raspberry Pi and connect it to ARC IoT Development Kit Board to transmitted results to the laptop via WiFi

5. Connect the USB port of ARC to the laptop to recieve image segmentation results

## User Manual
### Before Running This Application
* Download source code of iRobot from github
* (Optional) Prepare a Raspberry pi and download ***rpi_src*** directory in iRobot
* Make sure all connection is correct again
* (Optional) Make sure iRobot is in the WiFi environment, including WiFi name, password in rpi_src
* Please goto $ARC_ROOT/middleware/common/console_io.c and find xprintf_setup.
* Change `console_uart->uart_open(1500000)` to make sure the baudrate is 1,000,000

### Run This Application
* Type `make run` in the terminal to download program into ARC IoTdk board
* (Optional) After Rpi connect the wifi, the laptop should received the segmentation results in 0.5 fps
* After ARC connect with the laptop via USB, type `sudo python3 python/cameraGrabber.py` in the terminal to start recieving the segmentation results in 0.5 fps
* To test with the simple demo, type `sudo python3 python/car_count.py` or `sudo python3 python/danger_detect.py` in the terminal to start the applications

### Demo Video Link
* https://v.youku.com/v_show/id_XNDM0OTI5Mzk5Mg==.html?spm=a2h0j.11185381.listitem_page1.5~A

[1]: https://embarc.org/embarc_osp/doc/build/html/board/iotdk.html "ARC IoT Development Kit"
[2]: https://www.voti.nl/docs/OV7670.pdf "OV7670 Camera Module"
[3]: ./doc/system.png "System Architecture"
