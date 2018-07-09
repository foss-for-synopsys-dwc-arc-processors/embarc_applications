# ARC team 5 vending machine

## Table of Content

* [Introduction](#introduction)
* [Video](#video)
* [hardware](#hardware)
    * [EMSK Board firmware used in this application](#firmware)
    * [The peripheral devices used in this application](#peripheral)
    * [The hardware setup](#setup)
* [Software](#software)
    * [Makefile settings](#makefile)
    * [IDE settings](#ide)
* [User Manual](#usermanual)
    * [Before Running This Application](#before)
    * [Launch Server side code](#server)
    * [Launch VSCode IDE](#vscode)
    * [Wi-Fi hotspot connnection settings](#wifi)
    * [Entering Main](#main)
* [Source code directory](#sourcecode)

## Introduction <div id='introduction'/>
We use ARC as our user terminal and connect to the server, which is implemented with cloud data management and smart selling system. Peripheral devices and sensors are added to the ARC exp. WiFi, temperature sensor, OLED and many more.

Our vending machine system provides maintainance issue related to vending machine, such as auto-checking for food expired date, temperature overheat detecting. Other than that, vending machine is connected to database which users are allow to use their id for payment without physical bills or coins, and the machine will recommend what to buy depending on the purchase records of that user. The reason why our machine is different from others, the system administrator can get the information by signing in far away from the vending machine; the costomer can pay cashlessly. With e-payment, according to the purchase record of customer or other information, our machine can easily give some recommand after some operation, which can effectively decrease the time for customer making decision and interact with interest.

## Video <div id='video'/>
http://v.youku.com/v_show/id_XMzYzMDE0MTg1Mg==.html?spm=a2h0k.8191407.0.0&from=s1.8-1-1.2

## hardware connection <div id='hardware'/>
### EMSK Board firmware used in this application <div id='firmware'/>
* emsk ver2.2

### The peripheral devices used in this application <div id='peripheral'/>

| peripheral       | module     |
| :--------------: | ------:    |
| Wi-Fi            | EMW3162    |
| OLED             | SSD1306    |
| temperture       | adt7420    |
| DC motor	       | 6V 1:1000	|
| Infrared ray led | 940nm			|

### The hardware setup <div id='setup'/>

| port | interface | peripheral   |
| :--: | :-------: | :----------: |
| J1   | GPIO      | DC motor & Infrared ray |
| J2   | I2C       | temperature  |
| J3   | GPIO      | Number pad   |
| J5   | SPI       | OLED         |
| J6   | SPI       | Wi-Fi        |

## Software <div id='software'/>
sofware requirement
* Visual Studio code IDE
* ARC GNU Toolchain
* serial port terminal eg. putty
* Apache server , phpmysql

### visual Studio IDE settings <div id='ide'/>
edit `/ARC_VSCODE/StartVSCode.bat` to set your ARC toolchain path, library path, vscode program path.
example:
```
	SET ARC_ToolChain=C:/arc_gnu
	SET ARC_BSP_SRC=C:/embarc_osp-master
	start "" "C:/Program Files/Microsoft VS Code/Code.exe" %*
```

### Makefile settings <div id='makefile'/>
Target options about EMSK and toolchain:

BOARD ?= emsk
BD_VER ?= 22
CUR_CORE ?= arcem7d
TOOLCHAIN ?= gnu

## User Manual <div id='usermanual'/>
### Before Running This Application <div id='before'/>
Firstly, download source code of vending machine from GitHub.
* Source code download link: https://github.com/cstandy/ARC_team_5_vending_machine.git

### Run the server_side code <div id='server'/>
1. open the Apache HTTP server .
2. Secure your site with HTTPS(openssl etc...), you should also put the certificae file into the arc board so that it can update data via HTTPS.
    - in file `embarc_osp-master/middleware/mbedtls/library/certs.c`
    - change the content of `const char mbedtls_test_cli_crt_rsa[]`
3. import projectarc.sql into phpmysql(Data Base)
4. register a new user account from our website. 
 * register link: https://127.0.0.1/../server_side/sign_up.php

4. change the value 'Authority' in the table 'sign_data' to 1.
5. sign in to enter the main website.
* register link: https://127.0.0.1/../server_side/sign_in.php


### Launch VSCode IDE <div id='vscode'/>
```../ARC_VSCODE/StartVSCode.bat```
Launch StartVSCode.bat to run this vending machine application. After launching VScode, right click on the workspace and choose add Folder in the path ```../ARC_VSCODE/vending-machine``` to workspace. 

### Wi-Fi hotspot connnection settings <div id='wifi'/>
Modify the ssid and password of Wi-Fi AP in ``` ../embarc_osp-master/board/emsk/emsk.h``` to connect Wi-Fi module to the hotspot

```clike=137
#define WF_HOTSPOT_NAME             "EMBARC"
#define WF_HOTSPOT_PASSWD           "12345678"
```

### Entering Main <div id='main'/>
After Wi-Fi connected, FREERTOS rises all tasks including communication, oled, temperature, blinky, number pad and DC motor.

## Source code directory <div id='sourcecode'/>

* main function directory ```src\arc_osp-master\middleware\vending-machine```
	* Include
	* Source
	* Readme.md

* ```src\arc_osp-master\middleware\vending-machine\Source```

| FILE                 | description            |
| -------------------: | :--------------------- |
| vm_task.c            | main queue             |
| communication_task.c | mbedtls ssl client     |
| oled_task.c          | oled display           |
| adt7420.c            | temperature sensor     |
| dcmotor_task.c       | dc_motor controller    |
| numpad_task.c        | user input             |
| LED_task.c           | make the board blinky  |
| FreeRTOSConfig.c     | Freertos configuration |



* ```..\server_side\```

| FILE                 | description                         |
| -------------------: | :---------------------              |
| item.php             | change vending machine's data       |
| item.css             | to typeset item.php                 |
| logout.php           | logout                              |
| read.php             | updata both side server and arc     |
| sign_in.php          | sign into the item.php              |
| sign_up.php          | create an account                   |
| arc2php.txt          | exchange data from arc to server    |
| com2arc.txt          | exchange data from server to arc    |
| projectarc.sql       | initialize the data in the data base|

