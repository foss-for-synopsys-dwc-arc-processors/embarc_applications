# iLight SmartDevice Application
This application is designed to show how to develop a **Smart Device** using embARC. iLight can be controlled by gestures and iOS App. The connection between EMSK and the SmartPhone is based on bluetooth. Let's start a interesting trip with iLight!

* [Introduction](#introduction)
	* [Function](#function)
	* [Use Case](#use-case)
	* [iOS App](#ios-app)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## Introduction
**iLight**

### Function

- 

### Use Case

- 

### iOS App

- 

## Hardware and Software Setup
### Required Hardware
- 1 [DesignWare ARC EM Starter Kit(EMSK)][30]
- 1 [BLE HM-10 module][31]
- 1 [iLight bar(Homemade)][32]
- 1 SD Card
- 1 iOS SmartPhone

The iLight bar is a integrated module made by ourselves. The physical picture of it is as follows:

![ilight_bar][3]

### Required Software
- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom
- [iLight iOS App][35]

### Hardware Connection
1. The EMSK implement **iLight** SmartDevice, it will publish ... to the App via bluetooth, including .... We can view all data on the App UI. And the SmartPhone will send ... to it.
   - Connect **BLE HM-10 module** to **J?**, connect **iLight bar** to **J?**(Using UART interface).
2. Configure your EMSKs with proper core configuration.

## User Manual
### Before Running This Application
Firstly, download source code of **iLight SmartDevice** from github, and install **iLight iOS App** in your iOS smartphone.

Secondly, ....

The hardware resources are allocated as following table.

|  Hardware Resource  |            Function                                           |
| ------------------- | ------------------------------------------------------------- |
|  iLight bar         |        integrated module                                      |
|  BLE HM-10 module   |        Provide Bluetooth Connection                           |

### Run This Application

Modify the settings for connecting to the App, as shown below:

...

Here take **EMSK2.2 - ARC EM11D** with GNU Toolset for example to show how to run this application.

1. We need to use embARC 2nd bootloader to automatically load application binary for different EMSK and run. See *embARC Secondary Bootloader Example* for reference.

2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.

3. Interact using EMSK and App.


[0]: ./doc/screenshots/ibaby_function.PNG         "ibaby_function"

[30]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[40]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"
