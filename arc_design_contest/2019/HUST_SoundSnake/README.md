# Voice-operated snake game
This application is an implement of off-line spoken keyword spotting using MLI Lib based on ARC IoT Development Kit.The optimized convolutional neural network can figure out ten basic control commands,even though the processing unit is not that powerful.Then we can use voice to control many things,for example,a snake game.  
DEMO VIDEO:[Video presentation](https://v.youku.com/v_show/id_XNDI5MDI2NjE2NA==.html?spm=a2h3j.8428770.3416059.1)

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

### Function

- Spotting the ten spoken commands:yes/no/up/down/left/right/on/off/stop/go.And the word will be printed through uart.
- Play the game!Saying up/down/left/right commands,you can change the direction of the snake to catch the food or avoid crash with itself.

### System Architecture

![system_architecture][0]

## Hardware and Software Setup
### Required Hardware
- ARC IoT Development Kit

	![ARC IoT Development Kit][1]

- WM8978 CODEC module

	![WM8978 CODEC module][2]

- LED Matrix(MAX7219)

	![LED Matrix][3]

### Required Software
- ARC GNU Toolset 2018.09
- emcARC OSP 2018.12 with MLI Lib
- Serial port terminal, such as putty, tera-term or minicom

### Hardware Connection
1.MAX7219 pins.

|MAX7219 pins|IoT DK pins  |
|------------|-------------|
|CS          |GPIO_8b0_PIN4|
|DATA        |GPIO_8b0_PIN5|
|CLK         |GPIO_8b1_PIN4|

2.Connect I2C pins of WM8978 to I2C0 of IoT DK.

3.Connect I2S pins of WM8978 to I2S_RX of IoT DK.**The REF_CLK should not be connected untile the initialization of WM8978 is finished**.

## User Manual

### Before Running This Application
- Download source code of SoundSnake from github.
- Make sure all the connections are correct.
- The link script file has been modified to ensure the heap and stack are large enough and the addresses of program sections have also been redirected.Make sure the link script is correct.

### Run This Application

- Open serial terminal and configure it to right COM port and 115200bps.
- Download with USB-JTAG or use bootloader to boot the program.
- Connect the MCLK(REF_CLK) wire when seeing "Please connet the mclk !"at serial terminal.
- Speak to the Mic.You'll see the word at the serial terminal and the snake will follow your command to change direction.

#### Makefile

- Selected MLI Lib here, then you can use MLI Lib API in your application:

		LIB_SEL = embarc_mli

- Target options about IoT DK and toolchain:

		BOARD = iotdk
		BD_VER = 10
		TOOLCHAIN = gnu

- The middleware used in your application:

		MID_SEL = common

See [ embARC Example User Guide][40], **"Options to Hard-Code in the Application Makefile"** for more detailed information about **Makefile Options**.

#### Flie structure

|  file               |            Function                      |
| ------------------- | -----------------------------------------|
|  main.c             |  Initialization and main loop            |
|  kws.c/kws.h        |  Keyword spotting(CNN)                   |
|  ds_cnn_weights.h   |  Weights list of CNN                     |
|  mfcc.c/mfcc.h      |  Algorithm of extracting mfcc features   |
|  codec.c/codec.h    |  Recive and store audio frames(interrupt)|        |  snake.c/snake.h    |  Snake game and LED Matrix driver        |
|  FFT.c/FFT.h        |  Fast Fourier transform                  |
|  wm8978.c/wm8978.h  |  Config wm8978 through I2C               |
|  wm8978i2s.c/.h     |  I2S driver                              |

[0]: ./images/System_Architecture.PNG           "system_architecture"
[1]: ./images/ARC_IoT_Development_Kit.jpg       "ARC IoT Development Kit"
[2]: ./images/WM8978_CODEC_module.jpg           "WM8978 CODEC module"
[3]: ./images/LED_Matrix(MAX7219).jpg           "LED Matrix"

[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"
