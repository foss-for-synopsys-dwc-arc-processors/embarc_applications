# eCare - The Monitor device for the elderly
This applcation is designed to support Nursing Aide to **taking care of the elderly.** eCare is using embARC EM Starter Kit with BLE and ad2 pmods. You can observe thorugh our **andorid App** to monitor your **ECG(Electrocardiogram)** or **EMG(Electromyogram)** anywhere.
Let's us begin introduce eCare with you!

* [Introduction](#introduction)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## Introduction

**eCare**
- The goal is to provide users to monitor their health by the mobile. 
- We will collect body signal and convert voltage signal to digital signal and then transfer to bluetooth.
- Finally, user can see the EMG and ECG signal from the mobile

**System Architecture**
![System](https://imgur.com/ghY7syy)

**Andorid App**
- The data that collect from eCare will transmit to the andorid app. you can easily observe ECG and EMG waveform form mobile and it can also be saved as a txt file

![ECG](https://imgur.com/uQ2K9uZ)
![EMG](https://imgur.com/TpbTt8X)



## Hardware and Software Setup

* Required Hardware
  - embARC EM Starter
  - Pmod ble module
  - Pmod adc module
  - smartphone(andorid system)
  - Analog Front End(AFE)
  
![EM](https://imgur.com/TnxRAPJ)

* Required Software 
  - ARC GNU toolset
  - Serial port terminal such as putty, tera-trem
  - ExG android app
  
* Hardware Connection
  - Connect PmodAD2 to the board at J2 port
  - Connect the AFE to PmodAD2
  - Insert HM-10 to the board  and set the board version to arcem11d

![embARC](https://imgur.com/EpYaR3I)

* Demo Video

[ Demo ][6]

## User Manual

### Before run this application

- Download source code from github, and install app into your smartphone
- Connect smartphone bluetooth to ble pmod

### Run this application

- Compile the source code in the arc_gnu toolset
- Download the code into your EMSK Board. Be sure the switch 1 is off state and 2 is ON state(arcem11d configuration)
- Turn on the ExG andorid app on your smartphone
- Choose the ble UUID in the app list
- Finally, place the sensor on your body and you can see the signal on the mobile


|  file               |               Function                    |
| ------------------- | ------------------------------------------|
|  main.c             |   include ble transfer and adc function   |
                                     

[6]: https://v.youku.com/v_show/id_XMzcxMzc5ODI0NA==.html?spm=a2h3j.8428770.3416059.1 " Demo "
