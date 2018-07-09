# mobile_inverted_pendulum_robots
This application, which implements a mobile inverted pendulum robots, is designed to show how to develop a **Motion Control** using embARC.  
VIDEO URL: [DEMO VIDEO][14] 

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
This project was successful in achieving a two-wheeled autonomous robot based on the inverted pendulum model.EMSK works as controller, it will deal with sensor datas and interact with user via bluetooth. We can view all data on serial terminal, and sent instructions to the robot to change its motion mode.

### Function
-  **Upright**  
	The robot can keep itself upright even if there is a great disturbances.   
	[![upright.gif](https://s8.postimg.cc/4lsw5oi1h/upright.gif)](https://postimg.cc/image/6qd96rjo1/)
-  **Linear Motion**  
	Under upright condition, the robot has excellent ability to linear motion.   
	[![linear_motion.gif](https://s8.postimg.cc/fwvjtyovp/linear_motion.gif)](https://postimg.cc/image/gmec6bpf5/)
-  **Angular Rotation**  
	Not only linear motion, but also angular rotation can be implemented.  
	[![angular_rotation.gif](https://s8.postimg.cc/6qd96m6rp/angular_rotation.gif)](https://postimg.cc/image/jhrfd4gjl/)
-  **Over the Seesaw**  
	 Based on the above, the robot can autonomously through the seesaw  
 	[![over_the_seesaw.gif](https://s8.postimg.cc/wlwzpvt6t/over_the_seesaw.gif)](https://postimg.cc/image/hd72c3zi9/)

### System Architecture  
![system architecture][4] 

## Hardware and Software Setup
### Required Hardware
- 1 [DesignWare ARC EM Starter Kit(EMSK V2.3)][5]
- 2 [BLE module(HC-05)][6]  
- 1 [USB To TTL module(CP2102)][7]
- 1 [IMU module(MPU6050)][8]  
- 1 [Motor Driver][9]  
- 1 [Voltage Conversion module(7805)][10] 
- 1 NiCd Battery
- 1 SD Card  
- 1 SPI to QEI AND PWM Expanding-board 
- 1 Robot Platform  
	![Robot Platform][11]   
### Required Software
- ARC GNU Toolset 2017.03
- Serial port terminal, such as putty, tera-term or minicom

### Hardware Connection
1. Connect BLE HC-05 module to Pmod J1(Using UART interface)  
2. Connect IMU MPU6050 module to Pmode J2(Using I2C interface)  
3. Connect Expansion Board to Pmode J6, J5(Using J6 SPI interface, J5 using for mechanical fixing)

## User Manual
### Before running this application
Download source code of invert-pendulum from github, and ensure install a serial port terminal and arc-gnu toolset in your PC.
The hardware resources are allocated as following table.

| Hardware resource |              Function             |
|-------------------|-----------------------------------|
| MPU6050           | Gyroscope and acceleration sensor |
| HC-05             | Provide bluetooth connection      |
| CP2102            | USB TO TTL                        |
| MC7805            | Linear Voltage Regulator          |
| ROBOTS PLATFORM   | integrated module                 |

### Run This Application
Here take EMSK2.3, CUR_CORE = arcem7d with GNU Toolset for example to show how to run this application.  
1. We need to use embARC 2nd bootloader to automatically load application binary for different EMSK and run. See **example/bootloader Example** for reference.  
2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and 115200bps.  
3. Interact using EMSK and serial port terminal.  

#### makefile  
- Selected FreeRTOS in **/application/invered_pendulum/scr/makefile**, then you can use [FreeRTOS API][13] in your application:  

		#Selected OS
		OS_SEL ?= freertos

- Target options about EMSK and toolchain:

		BOARD ?= emsk
		BD_VER ?= 23
		CUR_CORE ?= arcem7d
		TOOLCHAIN = gnu

- The relative series of the root directory, here the path of the makefile is
**/application/invered_pendulum/scr/makefile**:
		
		#
		# root dir of embarc
		#
		EMBARC_ROOT = ../../../..

- Directories of source files and header files, notice that it **is not recursive**:

		# application source dirs
		APPL_CSRC_DIR = . ./TskDbg ./Queue ./TskMotor ./drivers ./TskTop ./FastMath_Table 
		APPL_ASMSRC_DIR = .

		# application include dirs
		APPL_INC_DIR = . ./TskDbg ./Queue ./TskTop ./drivers ./TskMotor ./FastMath_Table 

See [embARC Example User Guide][14], **"Options to Hard-Code in the Application Makefile"** for more detailed information about **Makefile Options**.

#### Driver
Placing the drivers' source code in **application/invered_pendulum/scr/drivers** folder, C source and header file in the folder.

|       file      |                   Function                  |
|-----------------|---------------------------------------------|
| imu.c/imu.h     | get imu sensor data(int16_t raw data)       |
| spi.c/spi.h     | transfer qei data(int16_t) and pwm(int16_t) |
| timer.c/timer.h | Timer 1,2ms interrupt for motroTick         |

#### FastMath_Table
The **FastMath_Table** is used to fast atan function, ranging in -0.98rad~0.45rad(table data is 1024 times of actual angle).The table index is 
equal to tangent value enlarges 128 times and add 192.

	/*
	 * n = m << 7 + 192, y = round(x << 10) 
	 * m: tangent value
	 * n: array index
	 * x: atan(m)
	 * y: array[n]
	 */

#### Queue
The **Queue** realizes queue functions through linear table, type:char or int.

		QueueInit 
		QueueClear 
		QueueLength 
		EnQueue 
		DeQueue
Drawback: it implement queue by arrays, which will waste memory space and 
fix data type.

#### TskDbg
The **TskDbg** mainly uses for UART transmission.  

|         file        |           Function          |
|---------------------|-----------------------------|
| cmd.c/cmd.h         | interact with user          |
| dbgUart.c/dbgUart.h | uart transfer and uart Task |

### TskMotor
The **TskMotor** completes data processing and algorithm application.

#### Other
The file **algorithm.s** implements algorithms through assembly instructions.
The file **assert.c** is useful to handling exceptions.  
The file **physParmas.h** contains macro define for data factor and customize typedef struct.


[0]: ./doc/screenshot/upright.gif "upright"	
[1]: ./doc/screenshot/linear_motion.gif "linear motion"	
[2]: ./doc/screenshot/angular_rotation.gif "angular rotation"	
[3]: ./doc/screenshot/over_the_seesaw.gif "over the seesaw"	
[4]: ./doc/screenshot/system.svg "system architecture"	
[5]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[6]: http://www.electronics-lab.com/?s=HC-05 "BLE HC-05 module"
[7]: https://www.sparkfun.com/products/retired/198 "USB To TTL module"
[8]: http://playground.arduino.cc/Main/MPU-6050 "IMU MPU6050 module"
[9]: http://www.landzo.cn/forum.php?mod=viewthread&tid=10541&extra=page%3D1&page=1 "Motor Driver"
[10]: http://www.electronics-lab.com/?s=7805 "Voltage Conversion 7805 module"
[11]: ./doc/screenshot/Platform.jpg "Robot Platform"
[12]: https://www.freertos.org/a00106.html "FreeRTOS API"
[13]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html " embARC Example User Guide"
[14]: http://v.youku.com/v_show/id_XMzYzMDkzNzg3Ng==.html?spm=a2h3j.8428770.3416059.1


