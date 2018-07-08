#the front-end system of intelligent bus sign based on ARC EM

**the front-end system of intelligent bus sign based on ARC EM** can show the location of the bus and the degree of crowding,which can provide a lot of convenience to the waiting people。The intelligent bus stop  is based on the ARC processor,and  communicates with bus by Wifi and Gizwits. Finanlly, three color LED lights display bus position and  degree of crowding information on bus stop sign.In the later promotion process，We can add corresponding functions,such as the voice recognition system, environment monitoring system and intelligent transfer functions, to respond to the development of smart city and smart transportation and to maximize passenger convenience.

* [Introduction](#Introduction)
  * [Function](#Function)
  * [System Architecture](#System Architecture)
* [Hardware and Software Setup](#Hardware and Software Setup)
  * [Required Hardware](#Required Hardware)
  * [Required Software](#Required Software)
  * [Hardware Connection](#Hardware Connection)

## Introduction
**the front-end system of intelligent bus sign based on ARC EM**
### Function
-  show the location of the bus

- show the degree of crowding
### System Architecture 

![Alt text](http://m.qpic.cn/psb?/V13Yp3s73hVIM9/bsUX5hqh3uFm17ssr8amS0PAYebvFa1yAQ5kboilmzQ!/b/dEUBAAAAAAAA&bo=bQKbAgAAAAADF8Q!&rf=viewer_4 "optional title") 

## Hardware and Software Setup
### Required Hardware

- 3 [DesignWare ARC EM Starter Kit(EMSK)][30]

- 6 [WiFi ESP8266 (ESP-01S)][31]

- 8 [LED RGB][30]

- 2 [Infrared ejection module ][30]

- 3 [SD Card][30]

   The physical picture of wearable node is as follows:：![Alt text](http://m.qpic.cn/psb?/V13Yp3s73hVIM9/7HZ1oLIIsywXMWlxbPf60dnI8tmZpIGU9VXjsMWd.PA!/b/dDEBAAAAAAAA&bo=9gS5AwAAAAARB3g!&rf=viewer_4 "optional title") 
### Required Software

- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom
- Gizwits
### Hardware Connection

EMSK 1 Realize the bus number counting function.We can get the number of people on the vehicle by subtracting the data from the data of the vehicle infrared radiation module

- Pmod1，Pmod5 connect to WiFi

- Pmod2，Pmod3，Pmod4and Pmod5 connect to LED RGB

  EMSK 2 and EMSK 3 display the arrival position of bus stop and the degree of congestion.Through the WiFi on the station card, the rssi signal sent by the WiFi on the bus is sensed and the data sent by the smart cloud is received,We can show the arrival position and the degree of congestion by the position and color of the lights

- Pmod1 connect to Infrared ejection module  and WiFi module

- Pmod2  connect to Infrared ejection module ， and Pmod5 connect to WiFi module








