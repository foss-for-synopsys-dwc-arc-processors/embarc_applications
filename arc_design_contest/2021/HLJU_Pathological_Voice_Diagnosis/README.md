# Pathological Voice Diagnosis

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

## Introduction
This application is based on the ARC IoT Development Kit which work for Pathological speech diagnosis at the edge.Firstly, the diagnostic model of pathological speech classification was trained based on lightweight RBK-NET network, and then the model was deployed using TensorFlow Lite for Microcontrollers (TFLM). Finally, the model was imported into ARC IoTDK development board through OSP.To achieve the Pathological speech diagnosis detection at the edge.

### Function

- Pathological Voice Diagnosis。At present, healthy and pathological speech can be recognized

- Display the current voice status

### System Architecture

![system_architecture][0]

## Hardware and Software Setup
### Required Hardware
- ARC IoT Development Kit

	![ARC IoT Development Kit](https://github.com/zhodj/embarc_applications/blob/master/arc_design_contest/2021/HLJU_Pathological_Voice_Diagnosis/images/arc_iot_dev_kit.png)

- WM8978 CODEC module

	![WM8978 CODEC module](https://github.com/zhodj/embarc_applications/blob/master/arc_design_contest/2021/HLJU_Pathological_Voice_Diagnosis/images/WM8978.png)

- OLED MC096GX (0.96inch )

	![OLED MC096GX](https://github.com/zhodj/embarc_applications/blob/master/arc_design_contest/2021/HLJU_Pathological_Voice_Diagnosis/images/oled.png)

### Required Software
- DesignWare ARC MetaWare Development Toolkit
- TensorFlow Lite for Microcontrollers 
- emcARC OSP 
- Serial port terminal, such as putty, tera-term or minicom

### Hardware Connection


1.Connect I2C pins of WM8978 to I2C0 of IoT DK.

2.Connect I2S pins of WM8978 to I2S_RX of IoT DK.**The REF_CLK should not be connected untile the initialization of WM8978 is finished**.

3.MAX7219 pins.

|OLED MC096GX pins|IoT DK pins  |
|-----------------|-------------|
|GND              |GND          |
|VCC              |3.3V         |
|SCL              |I2C0_SCL     |
|SDA              |I2C0_SDA     |

## User Manual

### Before Running This Application
- Download source code of Pathological Voice Diagnosis from github.
- Make sure all the connections are correct.
- The link script file has been modified to ensure the heap and stack are large enough and the addresses of program sections have also been redirected.Make sure the link script is correct.

### Run This Application

- Open serial terminal and configure it to right COM port and 115200bps.
- Download with USB-JTAG or use bootloader to boot the program.
- Connect the MCLK(REF_CLK) wire when seeing "Please connet the mclk !"at serial terminal.
- Speak to the Mic.You'll see pathological or healthy result shows on the OLED screen.

#### Makefile

- Selected MLI Lib here, then you can use MLI Lib API in your application:

		LIB_SEL = embarc_mli

- Target options about IoT DK and toolchain:

		BOARD = iotdk
		BD_VER = 10
		TOOLCHAIN = gnu

- The middleware used in your application:

		MID_SEL = common u8glib

See [ embARC Example User Guide][40], **"Options to Hard-Code in the Application Makefile"** for more detailed information about **Makefile Options**.

#### Flie structure

|  file               |            Function                      |
| ------------------- | -----------------------------------------|
|  main.c             |  Initialization and main loop            |
|  CppAddWrapper.h    |  TensorFlow Lite Wrapper header          |
|  mfcc.c/mfcc.h      |  Algorithm of extracting mfcc features   |
|  codec.c/codec.h    |  Recive and store audio frames(interrupt)|
|  FFT.c/FFT.h        |  Fast Fourier transform                  |
|  wm8978.c/wm8978.h  |  Config wm8978 through I2C               |
|  wm8978i2s.c/.h     |  I2S driver                              |

[0]: ./images/System_Architecture.PNG           "system_architecture"
[1]: ./images/ARC_IoT_Development_Kit.jpg       "ARC IoT Development Kit"
[2]: ./images/WM8978_CODEC_module.jpg           "WM8978 CODEC module"
[3]: ./images/OLED(MC096GX).jpg                 "OLED MC096GX"

[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"
