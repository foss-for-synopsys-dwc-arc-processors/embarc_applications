# Anti-lost security detection system 

The portable outdoor travel equipment designed by us is based on sensors to collect motion information, intelligently discriminate, realize abnormal posture recognition, and can detect the distance with the guardian in real time through combined positioning to ensure the safety of the user. Compared with the machine vision-based gesture recognition method, it has the advantages of fast speed, good real-time performance and convenient carrying.

<!-- markdown-toc start - Don't edit this section. Run M-x markdown-toc-refresh-toc -->
- [Introduction](#introduction)
    - [Function](#function)
    - [System Architecture](#system-architecture)
- [Hardware and Software Setup](#hardware-and-software-setup)
    - [Required Hardware](#required-hardware)
    - [Required Software](#required-software)
    - [Hardware Connection](#hardware-connection)
- [User Manual](#user-manual)
    - [Before Running This Application](#before-running-this-application)
    - [Run This Application](#run-this-application)
- [Demo Video](#demo-video)
 <!-- markdown-toc end -->

# Introduction
The intelligent monitoring device performs intelligent discrimination based on the sensor to collect motion information, and realizes abnormal posture recognition.
## Function
   - **Shedding check mode**<br/>
     The buzzer sounds an alarm when the sensor comes off.
   
      ![image](https://github.com/Zhaoyang95/Anti-lost-security-detection-system-based-on-ARC-processor/blob/master/Screenshots/Shedding%20check.gif)
   - **Posture detection mode**<br/>
     The device can detect abnormal conditions such as human fall and being picked up.
    
      ![image](https://github.com/Zhaoyang95/Anti-lost-security-detection-system-based-on-ARC-processor/blob/master/Screenshots/Posture%20detection_1.gif)
	  ![image](https://github.com/Zhaoyang95/Anti-lost-security-detection-system-based-on-ARC-processor/blob/master/Screenshots/Posture%20detection_2.gif)
   - **GPS mode**<br/>
     When the user exceeds the safe distance, the location information can be sent to the guardian's mobile phone.
	 
	  ![image](https://github.com/Zhaoyang95/Anti-lost-security-detection-system-based-on-ARC-processor/blob/master/Screenshots/GPS%20mode.gif)
   - **Wisdom broadcast mode**<br/>
     When the user is lost, the voice mode can be switched with one button for playing the user's family information.
	 
	  ![image](https://github.com/Zhaoyang95/Anti-lost-security-detection-system-based-on-ARC-processor/blob/master/Screenshots/Wisdom%20broadcast.gif)
## System Architecture
   As shown in the figure below, the system consists of **three parts: input, master, and output**.
   - The **input** includes a Bluetooth module, a GPS module and a JY901 nine-axis sensor module. The Bluetooth module receives the connection signal and sends it to the ARC processor; The JY901 module is responsible for detecting the motion state, The module mainly detects the acceleration change during motion and relative to the horizontal direction of the deflection condition; the GPS module is responsible for locating the user's location information and sending it to the master.
   - The **master** is composed of an ARC processor. It mainly complete the processing of the data and complete the matching with the database according to the algorithm, and send the corresponding command to the output, the data processing includes through personalized calibration. Different people's movements have their own characteristics. Through personalized calibration, a database with posture characteristic information is established, which makes the system suitable for different users, and the user can customize according to individual usage.
   - The **output** includes a voice synthesis module and a GPRS module. When the user exceeds the safety distance, the position information acquired by the GPS can be sent to the guardian mobile phone through the GPRS module to realize quick rescue and real-time positioning, and the family information can be reproduced in the form of voice. Convenient and timely contact with the guardian.
   ![image](https://github.com/Zhaoyang95/Anti-lost-security-detection-system-based-on-ARC-processor/blob/master/Screenshots/Architecture.png)
# Hardware and Software Setup
## Required Hardware   
   - 1 DesignWare ARC EM Starter Kit(EMSK)
   - 1 Posture sensor(JY-901)
   - 1 Voice composite module(SYN6288)
   - 1 Bluetooth module(E104-BT02)
   - 1 GPS module(C3-470A)
   - 1 GPRS module(SIM900A)
   - 1 Buzzer
   - 1 SD Card
   
## Required Software
   - ARC GNU Toolset
   - Serial port terminal, such as putty, tera-term or minicom

## Hardware Connection
   1. The EMSK implement smart gloves,it will processe the data collected by sensors and translate gestures to corresponding sign language, we can get the corresponding sign language information by displaying text and making voice.
      - Connect **Posture sensor** to **J1**(Using UART interface)
      - Connect **Voice composite module** and **GPRS module** to **J2**(Using I2C interface)
      - Connect **GPS module** to **J5**(Using UART interface)
      - Connect **Buzzer** to **J3**(Using GPIO interface)
      - Connect **Bluetooth module** to **J4**(Using I2C interface)
   2. Configure your EMSKs with proper core configuration
   - EMSK connection
   ![image](https://github.com/Zhaoyang95/Anti-lost-security-detection-system-based-on-ARC-processor/blob/master/Screenshots/connection.jpg)

# User Manual
## Before Running This Application
  Download source code of Anti-lost security detection system based on ARC processor from github.
  - The hardware resources are allocated as following table.
  
| Hardware Resource  | Functioon                      |
| ------------------ | -----------------------------  |
| JY-901             | Acceleration and angle sensor  |
| E104-BT02          | Indoor Positioning             |
| SYN6288            | Voice composite                |
| C3-470A            | position                       |
| SIM900A            | send messages                  |
  
## Run This Application
  Here take EMSK2.0 - ARC EM7D with GNU Toolset for example to show how to run this application.We need to use embARC bootloader to automatically load application binary for different EMSK and run. See embARC Secondary Bootloader Example for reference.
  * Modify emsk_init.c (/board/emsk/common/emsk_init.c)
 ```
line 107: change 
	set_pmod_mux(mux_regs, PM1_UR_UART_0 | PM1_LR_SPI_S	\
				| PM2_I2C_HRI			\
				| PM3_GPIO_AC			\
				| PM4_I2C_GPIO_D		\
				| PM5_UR_SPI_M1 | PM5_LR_GPIO_A	\
				| PM6_UR_SPI_M0 | PM6_LR_GPIO_A );
 to 
 	set_pmod_mux(mux_regs, PM1_UR_UART_0 | PM1_LR_GPIO_A	\
				| PM2_I2C_HRI			\
				| PM3_GPIO_AC			\
				| PM4_I2C_GPIO_D		\
				| PM5_UR_SPI_M1 |PM5_LR_SPI_M2	\
				| PM6_UR_SPI_M0 | PM6_LR_GPIO_A );
```
### Makefile
   - Target options about EMSK and toolchain

```
         BOARD ?= emsk
         BD_VER ?= 22
         CUR_CORE ?= arcem7d
         TOOLCHAIN ?= gnu
```
   - The relative series of the root directory, here the path of the Makefile is .\embARC\example\emsk\sign language/makefile:
```
         #
         # root dir of embARC
         #
         EMBARC_ROOT = ../../..
         MID_SEL = common u8glib
```  
  See [ embARC Example User Guide][40], "Options to Hard-Code in the Application Makefile" for more detailed information about Makefile Options
  
  - Driver<br/>
  Placing the drivers' source code in driver folder, you can see there are subfolders for data,buletooth,buzzer,gps and gprs drivers. Placing the C source file and header file in the corresponding subfolder.

| folder/file      | Function                                                  |
| ---------------- | --------------------------------------------------------- |
| data             | get and deal with data from sensor                        |
| buletooth        | Receive a connection signal and send it to the processor  |
| gprs             | send position information                                 |
| buzzer           | Sensor drop alarm                                         |
| gps              | Detect location information                               |

# Demo Video
[Link](https://v.youku.com/v_show/id_XNDI4NjQ0NTQxMg==.html?spm=a2h3j.8428770.3416059.1)


[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"

 
