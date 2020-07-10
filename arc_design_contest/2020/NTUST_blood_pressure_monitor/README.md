# Personal identification of blood pressure monitor without pressure belt

* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#system-architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Make This Application](#Make-this-application)

## Introduction
With the advancement of science and technology, people's living habits have changed, and health management is gradually valued. In health management, measuring blood pressure regularly is an inestimable part.
Usually, it is a common situation for a family to buy a sphygmomanometer, and most of the commercially available sphygmomanometers do not have the function of identification, so it is more inconvenient for multiple people to share the recording process of the sphygmomanometer.
We want to achieve a system that can automatically identify the identity through the camera and ECG signal characteristics and automatically record the measurement results to the cloud. Through the convenience of automatic identification, the operation of the sphygmomanometer is simplified, and the public's expectations for using the sphygmomanometer for a long time are raised. The competitive strength of blood pressure plan products will help to establish a sound health management mechanism.
In addition, most of the current sphygmomanometers use a cuff measurement method, and the process is slightly uncomfortable. Therefore, we also hope to establish a new measurement method of pressureless vascular belt through the PTT time difference between ECG and PPG.

### Function
- Using ECG for body discrimination can increase the accuracy of identification and reduce the possibility of using photos to deceive.
- Using ECG and PPG to measure blood pressure can improve the comfort during measurement.
- Significantly reduce blood pressure measurement time
- Reduce operation complexity by automatically identifying users


### System Architecture
#### Hardware architecture
<div align="center"><img width="330" height="200" src="images/hardware.PNG"></div>

#### Software architecture
<div align="center"><img width="330" height="200" src="images/Software.PNG"></div>

#### Project overview
<div align="center"><img width="330" height="200" src="images/project_photo (1).jpg"></div>

## Hardware and Software Setup
### Required Hardware
- 1 embarc iot develop kit
- 1 ecg ppg sensor(sample rate:100hz,baudrate:115200)
- 1 PC with com port

### Required Software
- Metaware or ARC GNU Toolset
- python
- python module:PyQt5,numpy,matplotlib,random,Serial,struct,time

### Hardware Connection
1.
   - Connect **4 electrode** to **sensor board LL RL RA LA**.
   - Connect **PPG SENSOR** to **sensor board**.
   - Connect **sensor board** to **PC**(use UART).
2. Connect your **iotdk** to computer.

## User Manual
### Make This Application

Here take **iotdk** with METAWARE Toolset for example to show how to run this application.

#### Makefile

- Target options about EMSK and toolchain:

		BOARD ?= iotdk
		BD_VER ?= 10
		CUR_CORE ?= arcem9d
		TOOLCHAIN ?= mw

- The relative series of the root directory, here the path of the Makefile is 

		#
		# root dir of embARC
		#
		EMBARC_ROOT = ../../../../..
		MID_SEL = common 
 #### Command
 
 - Use Command gmake BOARD=iotdk BD_VER=10 CUR_CORE=arcem9d TOOLCHAIN=mw V=1 ADT_COPT="-Hdsplib" ADT_LOPT="-Hdsplib -Xdsp1 -          Hlib=../my_dsp"

 #### python setting
 Please make sure you iotdk port and sensor port is COM6 and COM3 or you can change the com port in gui.py line332,337,665