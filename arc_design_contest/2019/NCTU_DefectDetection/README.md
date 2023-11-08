# Eletronic Component Defect Detection on ARC IOT Development Kit
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
In this contest, we aim to develop a convolutional neural network model to detect the defect on eletronic component, such as capacitor and resistor. To run on the ARC Development Kit (IOTDK), we need to create a model with small amount of paramter.

### System Architecture
![](https://imgur.com/VJ883mq.png=650x)

We get a dataset with three kinds of data, defect, false-alarm, and golden. False-alarm and golden are kinds of good product, but defect is a kind of defective product. Then, we have a 2-class classifier convolutional neural network which is trained with the training images. Finally, we get a result which is good or defect.

## Hardware and Software Setup
### Required Hardware
[DesignWare ARC IoT Development Kit](https://github.com/foss-for-synopsys-dwc-arc-processors/ARC-Development-Systems-Forum/wiki/ARC-Development-Systems-Forum-Wiki-Home#arc-iot-dev-kit-1)
![](https://imgur.com/kGNpoki.png=500x)

| Training Platform     | Device                                               |
| --------------------- | ---------------------------------------------------- |
| CPU                   | Intel(R) Xeon(R) CPU E5-2620 v4 @ 2.10GHz (32 Cores) |
| GPU                   | GeForce GTX 1080                                     |
| RAM                   | 188GB                                                |
| OS                    | Linux Ubuntu 14.04.5                                 |

### Required Software
* EMSK Environment 
	* [embARC OSP branch-embARC MLI](https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/tree/embarc_mli)
	* [Teraterm](https://ttssh2.osdn.jp/index.html.en)
	* [Environment Build Up](https://embarc.org/embarc_osp/doc/build/html/getting_started/getting_started.html)
* Python 3.7
* Deep Learning related Package
    * Jupyter (optional)
    * Caffe
	* Numpy
	* Skimage
	* Matplot

### Hardware Connection
* IOTDK connection

![](https://imgur.com/Z9IYyTH.png=400x)

## User Manual
### Before Running This Application
* Dataset

Due to ownership, we could not provide dataset. You should prepare it as a char array by yourselves.

* makefile
```
BOARD ?= iotdk
CUR_CORE ?= arcem9d
APPL_DEFINES = -DUSE_APPL_MEM_CONFIG -DV2DSP_XY -DMODEL_BIT_DEPTH=8
EMBARC_ROOT = setting your root path of your embARC-osp without " "
```

### Run This Application
Run the "make run" command on cmd in directory where your makefile is
![](https://imgur.com/ybhg4RR.png)

Result:
![](https://imgur.com/cL7dEJy.png)
