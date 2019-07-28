# Face Recognition System
The design is used to implement face recognition, and face recognition is implemented on the Arc IotDK development board by deep machine learning convolutional neural network algorithm.

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
Face Recognition System can implement face recognition.
## Function
   - **Image display mode**<br/>
     The processor recognizes the **Real-time image display**, then the image captured by the OV7670 camera is displayed in real time using the LCD display.
     
      ![image](https://github.com/lx960125/arc/blob/master/screenshot/lx.jpg)
   - **Recognition mode**<br/>
     The processor recognizes the **Recognition function**, then the voice module broadcasts the recognition result.
     
      ![image](https://github.com/lx960125/arc/blob/master/project%20picture/Overall%20appearance.png)
## System Architecture
   As shown in the figure below, the system consists of **three parts: input, master, and output**.
   - The **input** include oollecting image data.
   - The **master** is composed of an ARC processor. It mainly performs data processing, implementing CNN convolutional neural network algorithm, and recognizes and processes acquired images.
   - The **output** includes real-time display of the captured image and broadcast of the recognition result
   ![image](https://github.com/lx960125/arc/blob/master/project%20picture/architecture.jpg)
# Hardware and Software Setup
## Required Hardware   
   - 1 DesignWare ARC Iot Development Kit(IotDK)
   - 1 Voice composite module(SYN6288)
   - 1 LCD display module(ILI9341)
   - 1 Acquisition image module(OV7670)
   
## Required Software
   - ARC GNU Toolset
   - Serial port terminal, such as putty, tera-term or minicom

## Hardware Connection
   1. The IotDK implements Face Recognition,it will processe the image data collected by camera, display images with LCD and we can get the recognition result by making voice.
      - Connect **Acquisition image module** to **Pmod_B**(Using GPIO interface) and **Pmod_C**(Using B7 and B8) and **ARDUINO**(using IO4~IO11 GPIO interface)
      - Connect **Voice composite module** to **Pmod_C**(Using UART interface)
      - Connect **LCD display module** to **ARDUINO**(Using AD0~AD4 GPIO interface)
   2. Configure your IotDK with proper core configuration
   - IotDK connection
   ![image](https://github.com/lx960125/arc/blob/master/project%20picture/connection.png)
   
# User Manual
## Before Running This Application
  Download source code of Face Recognition System from github.
  - The hardware resources are allocated as following table.
  
| Hardware Resource  | Functioon                      |
| ------------------ | -----------------------------  |
| ILI9341            | show image                     |
| OV7670             | Camera                         |
| SYN6288            | Voice composite                |
  
## Run This Application
  Here take IotDK1.0 - ARC EM9D with GNU Toolset for example to show how to run this application.
  * Modify iotdk_init.c (/board/iotdk/common/iotdk_init.c)
 ```
line 62: change 
	static void iotdk_mux_init(void)
{
	io_mux_init();
	io_pmod_config(PMOD_B, PMOD_SPI, IO_PINMUX_ENABLE);
	io_pmod_config(PMOD_C, PMOD_UART, IO_PINMUX_ENABLE);
}
 to 
 	static void iotdk_mux_init(void)
{
	io_mux_init();
	io_pmod_config(PMOD_B, PMOD_GPIO, IO_PINMUX_ENABLE);
	io_pmod_config(PMOD_C, PMOD_UART, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_4, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_5, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_6, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_7, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_8, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_9, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_10, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_11, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_AD0, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_AD1, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_AD2, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_AD3, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_AD4, ARDUINO_GPIO, IO_PINMUX_ENABLE);
	io_arduino_config(ARDUINO_PIN_AD5, ARDUINO_GPIO, IO_PINMUX_ENABLE);
}
```
### Makefile
   - Target options about iotdk and toolchain

```
         BOARD ?= iotdk
         BD_VER ?= 10
         CUR_CORE ?= arcem9d
         TOOLCHAIN ?= gnu
```
   - The relative series of the root directory, here the path of the Makefile is .\embarc_osp-master\example\baremetal\cnn/makefile:
```
         #
         # root dir of embARC
         #
         EMBARC_ROOT = ../../..
         MID_SEL = common 
```  
  See [ embARC Example User Guide][40], "Options to Hard-Code in the Application Makefile" for more detailed information about Makefile Options
  
  - Driver<br/>
  Placing the drivers' source code in driver folder, you can see there are subfolders for cnn,ov7670,lcd and yuyin drivers. Placing the C source file and header file in the corresponding subfolder.

| folder/file      | Function                                           |
| ---------------- | -------------------------------------------------  |
| cnn              | convolutional neural network algorithm             |
| ov7670           | camera driver                                      |
| lcd              | show image                                         |
| yuyin            | broadcast recognition result                       |

# Demo Video
[Link](http://v.youku.com/v_show/id_XNDI4OTkxMDM1Ng==.html?spm=a2h0k.11417342.soresults.dposter)


[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"
