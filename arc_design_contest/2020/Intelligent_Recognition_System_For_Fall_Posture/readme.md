# Intelligent Recognition System for Fall Posture 

------

This application is a prototype of  **intelligent recognition system for fall posture** designed on ARC Iot DK, which can identify whether users  are in normal state or fall state according to the intelligent system, and transfer the current data to the **cloud server upper machine** which is designed by **Visual Studio 2019**. The original data is collected by **IMU sensor MPU9250** which is embedded in **ARC Iot DK** board. The data is transferred to cloud server. 

**The cloud server upper machine is composed of three parts**: Data-monitor Zone, Weight-management Zone and Weight-transfer Zone. **Data-monitor Zone mainly collects original data and draws inertial figure**. **Original data is transferred to server by Bluetooth Module HC-06** and server draws inertial figure at the same time.  **Weight-management Zone controls the interaction between ARC Iot DK and cloud server** including downloading initial weight for users from cloud server and uploading current weight to cloud server for big data acquisition. **The process of uploading and downloading weight can be watched in Weight-transfer Zone** and be established by **wireless communication module Sim900a**. 

**BP Neural network** is deployed in ARC Iot DK Board and be used to **train weight and make intelligent recognition**. When neural network miscalculates the posture, users uploads the correct tag by the key in **matrix keyboard** and neural network retrains weight according the correct tag and keep optimizing by itself. To solve the forgetting problem of neural network, **knowledge distillation algorithm** is introduced to retain the memory of the old data set while learning the new data, so that the network can achieve the effect of continuous upgrading, improve the accuracy of attitude judgment, and use the most suitable network weight data for different users. 

Intelligent recognition system for fall posture provides various alarm ways. Basically, when system judges fall gesture of users, it will make the **buzzer** ring intermittently to remind people around to help the elderly. The system also makes full use of Sim900a Module and **send information for help** or **make a phone call** to the guardian of user if necessary.

------

## Catalog

- [Introduction](#Introduction)
- [Appearance](#Appearance)
  - [System View](#System-View)
- [Hardware and Software Setup](#Hardware-and-Software-Setup)
  - [Required Hardware](#Required-Hardware)
  - [Required Software](#Required-Software)
  - [Hardware Connection](#Hardware-Connection)
- [User Manual](#User-Manual)
  - [Before Running This Application](#Before-Running-This-Application)
  - [Run This Application](#Run-This-Application)
  - [Specific instructions for use](#Specific-instructions-for-use)
- [Makefile](#Makefile)
- [The system block diagram](#The-system-block-diagram)
- [The cloud server upper machine](#The-cloud-server-upper-machine)
- [File Structure](#File-Structure)

------

## Introduction

This application is designed basing on BP network and knowledge distillation algorithm. First, users uploads their own physical parameters by matrix keyboard and downloads multilayer perceptron MLP initial weights from cloud server. ARC Iot DK Board uses inertial measurement unit IMU to collect user's real-time Euler Angle of posture,  Euler Angle is imported to MLP network and neural network completes the training of weight data and gesture recognition. The user wears this application and gradually inform the system of retraining weight data of neural network by pressing the key of correct tag when neural network miscalculates the posture. If a fall gesture is detected, an alarm will be sent through the buzzer and the system will send information for help if necessary, so that the elderly can be rescued in the first place.

------

## Appearance

### System View

<img src=".\picture\ARC IOTDK.png" style="zoom:80%;" />

<img src=".\picture\System composition diagram.jpg" style="zoom: 33%;" />





------

## Hardware and Software Setup

### Required Hardware

Necessary Hardware:

1. ARC IoT Development Kit(IoT DK)                                                    *1

2. HC-06(Bluetooth Module)                                                                 *1

3. Matrix Keyboard                                                                                 *1

4. Sim900a (Wireless Communication Module)                                 *1 

5. Buzzer                                                                                                   *1

<img src=".\picture\ARC IOTDK.png" style="zoom:80%;" />

​                                                                                                  Hardware1: ARC IOTDK

<img src=".\picture\HC-06.png" style="zoom:130%;" />

​                                                                                                Hardware2: Bluetooth HC-06

<img src=".\picture\Matrix%20Keyboard.png" style="zoom:80%;" />

​                                                                                                  Hardware3: Matrix Keyboard

<img src=".\picture\sim900a.png" style="zoom: 50%;" />

​                                                                              Hardware4: Wireless Communication Module Sim900a

<img src=".\picture\buzzer.png" style="zoom: 50%;" />

​                                                                                                             Hardware5: Buzzer

### Required Software

1. ARC GNU Toolchain 2020.03
2. embarc_osp(Branch:embarc_mli): https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp.git
3. Visual Studio 2019
4. Python3.7
5. Matlab R2020a

### Hardware Connection

1. Connect HC-06 to Arduino Shield Interface (1x) (Uart2)

2. Connect Sim900a to 12-pin PMOD connectors PMOD_C(Uart1)

3. Connect matrix keyboard to Arduino Shield Interface (1x) (IO4-IO7) and Power Board(including pull-up resistors)

4. Connect buzzer to 2x18 Pin Extension Header(GPIO4b_1[0])

5. Connect Power Board to Arduino Shield Interface (1x) (IO8-IO11)

------

## User Manual

### Before Running This Application

1. Download source code from Github.

2. Make sure all connection is correct again.

3. Start cloud server upper machine and set correct communication protocol(TCP) and port.

### Run This Application

1. To build this application, select the proper board version, core configuration and build with selected toolchain using this command `make BOARD=iotdk BD_VER=10 CUR_CORE=arcem9d TOOLCHAIN=gnu run`.  

2. Open the serial terminal on cloud server upper machine.

3. When the application starts, it will connect with cloud server via Bluetooth Module and  Wireless Communication Module.

### Specific instructions for use

1. After running the application, user inputs their own physical parameters including height, weight and waist circumference via matrix keyboard and press "Download" key. The cloud server upper machine will transfer appropriate weight data to ARC IOTDK Board and user can get initial weight from cloud server.
2. After getting initial weight data, user begins to wear this application to train better weight data. The application has integrated BP neural network in IOTDK, When neural network miscalculates the posture, users uploads the correct tag by the "Upload" key in matrix keyboard and neural network retrains weight according the correct tag and keep optimizing by itself. 
3. After a period of training, the application's fall posture recognition success rate can be close to 100 %.
4. When the application judges that the user is in fall state, it will make buzzer ring first. If user thinks the situation of falling is not very dangerous, he/she can press the "Stop" key on matrix keyboard to make buzzer stop to ring. If user  doesn't press "Stop" key within the allotted time, the application will judge that user is in serious injured state, it will automatically send alarm information to user's guardian.

------

## Makefile

- Select correct board  

```
BOARD = iotdk
```

- Select middleware

```
MID_SEL = common
```

- Select devices

```
EXT_DEV_LIST += sensor/imu/mpu9250
```

- Select root direction of embARC

```
EMBARC_ROOT = ../../..
```

- Select application source directions

```
APPL_CSRC_DIR = .
APPL_ASMSRC_DIR = .
```

- Select application include directions

```
APPL_INC_DIR = .
```

- Include current project makefile

```
COMMON_COMPILE_PREREQUISITES += makefile
```

------

## The system block diagram

<img src=".\picture\system block diagram.png" style="zoom:150%;" />

------

## The cloud server upper machine

The cloud server upper machine is designed by Visual Studio 2019. Here are running pictures of three parts:

<img src=".\picture\data-monitor zone.png" style="zoom: 80%;" />

​                                                                                                   Data-Monitor Zone

<img src=".\picture\weight-transfer zone.png" style="zoom:80%;" />

​                                                                                                    Weight-Transfer Zone

<img src=".\picture\weight-management zone.png" style="zoom:80%;" />

​                                                                                                Weight-Management Zone

------

## File Structure

| File             | Function                                      |
| ---------------- | --------------------------------------------- |
| main.c           | Initialization, Hardware Driver and main loop |
| B0.h             | including Bias0 data of Neural Network        |
| B1.h             | including Bias1 data of Neural Network        |
| W0.h             | including Weight0 data of Neural Network      |
| W1.h             | including Weight1 data of Neural Network      |
| NNLayer.c        | Algorithm of Neural Network Layer             |
| NNLayer.h        | Head File of Neural Network Layer             |
| train_txt_name.h | Head file of  txt files' name list            |
| train            | collected-data folder                         |

