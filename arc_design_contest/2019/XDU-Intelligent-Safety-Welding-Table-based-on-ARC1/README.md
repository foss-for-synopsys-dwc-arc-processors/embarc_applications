# Intelligent Safety Welding Table based on ARC
The project is an intelligent safety welding platform based on Synopsys ARC EM processor. When the laboratory personnel are operating the welding table, the instrument works normally. Judging by human infrared sensor, when the operator leaves, the voice module reminds to close the welding platform. If the soldering table is not closed after the operator has been away for a period of time, the power is automatically cut off through the relay module. This project also includes flame sensor, smoke sensor, and GSM module. When the flame sensor and the smoke sensor sense a certain amount of smoke and fire light in the laboratory, the processor immediately determines that there may be a fire, giving priority to the welding table through the relay There are hidden dangers in voice broadcasting through power outage and voice module, and text messages are sent to laboratory administrators through GSM module to inform them to check the hidden dangers in time.
* [Introduction](#introduction)
	* [Project context](#project-context)
	* [System Architecture](#system-architecture)
       * [Function](#function)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)
	* [DemoVideo](#demovideo)
## Introduction
### Project context
With the progress and development of science and technology, more and more colleges and universities are committed to cultivating college students' scientific literacy and strengthening practical skills and entrepreneurial ability. The establishment of innovation laboratory for college students has become the main place and platform for college students to impart knowledge, develop intelligence, cultivate college students' scientific quality and skills, cultivate college students' innovative spirit, entrepreneurial consciousness, innovative entrepreneurial ability and so on. It has an irreplaceable and unique role, stimulates students' innovative thinking and innovative consciousness, but also constructs innovative educational culture, and comprehensively improves students' innovative experimental ability. However, with the increase of laboratory students and the difficulty of management, it is also gradually becoming more and more difficult. There are some hidden dangers, such as irregular use of sockets and sockets, electric soldering iron, hot air gun, and other laboratory instruments and equipment have not been turned off from time to time, and it is easy to cause fire accidents.
![0](https://github.com/pfli07/Intelligent-protection-system-for-forgotten-children-in-vehicle/blob/master/screenshots/Project%20context.png)
Based on an analysis of 95 laboratory accidents in colleges and universities at home and abroad from 2010 to 2015 (66 in mainland China), a similar conclusion was drawn: 68% of explosions and fires, 12% of leakage, 11% of biosafety, 2% of poisoning and 7% of others. The main reason that triggered the safety accident was also the violation of the experimental operation rules or careless experimental operation, accounting for 52% of the total number of accidents, resulting in 40% of the deaths and 65% of the injuries or poisoning.
### System Architecture
![00](https://github.com/pfli07/Intelligent-protection-system-for-forgotten-children-in-vehicle/blob/master/screenshots/yingwen.png)
### Function
  * Intelligent real-time monitoring
  * Voice real-time broadcast
  * Security hazard alarm
  * send information in real time
  * Automatic control circuit
  ## Hardware and Software Setup
### Required Hardware
* 1 DesignWare ARC EM Starter Kit(EMSK)
* 1 Diffuse photoelectric switch.
* 1 SIM900A SMS module
* 1 SIM card
* 1 All the way optical coupling isolation relay
* 1 SDCard
* 1 TTS voice module
* 1 Smoke sensor module
* 1 Flame sensor module
* 1 buzzer
* **The physical picture shown below**
![00](https://github.com/pfli07/Intelligent-protection-system-for-forgotten-children-in-vehicle/blob/master/screenshots/waiguan.jpg)![00](https://github.com/pfli07/Intelligent-protection-system-for-forgotten-children-in-vehicle/blob/master/screenshots/neibu.jpg)
### Required Software
* **The physical picture shown below**
![00](https://github.com/pfli07/Intelligent-protection-system-for-forgotten-children-in-vehicle/blob/master/screenshots/waiguan.jpg)![00](https://github.com/pfli07/Intelligent-protection-system-for-forgotten-children-in-vehicle/blob/master/screenshots/neibu.jpg)
### Required Software
* Metaware or ARC GNU Toolset
* Serial port terminal, such as putty
### Hardware Connection
|         link module                               |         attended mode            |   
| --------------------------------------------------|----------------------------------|
|   All the way optical coupling isolation relay    |                GPIO              |
|             SIM900A SMS module                    |                UART              |
|              TTS voice module                     |                UART              |
|          Diffuse photoelectric switch             |                GPIO              |
|             Smoke sensor module                   |                GPIO              |
|             Flame sensor module                   |                GPIO              |
|                   buzzer                          |                GPIO              |
## User Manual
### Before Running This Application
The hardware resources are allocated as following table.

|          Hardware Resource                 |                          Function                           |   
| -------------------------------------------| ------------------------------------------------------------|
|All the way optical coupling isolation relay|    used to realize the shutdown of the circuit              |
|            SIM900A SMS module              |   For sending text messages remotely                        |
|             TTS voice module               |           For voice broadcast prompts                       |
|        Diffuse photoelectric switch        |            Used to detect the human body.                   |
|            Smoke sensor module             |             Used to detect smoke                            |
|            Flame sensor module             |              Used to detect open fire                       |
|                  buzzer                    |                   Beehive prompt                            |
### Run This Application


#### Makefile
- Target options about EMSK and toolchain:

                BOARD ?= emsk
                BD_VER ?= 22
                CUR_CORE ?= arcem7d
                TOOLCHAIN ?= gnu
                
- The relative series of the root directory, here the path of the Makefile is<br/> 
`./Intelligent Safety Welding Table based on ARC/src/makefile:`:
# DemoVideo
[Link](http://v.youku.com/v_show/id_XMzYyNjcwOTY4OA==.html?spm=a2hzp.8244740.0.0)
