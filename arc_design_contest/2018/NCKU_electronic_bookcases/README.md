arc-2018-Team4 承佑賴床是我不隊
==================

* [Introduction](#overview)
* [Video](#video)
* [Configuration](#struct)
  * [Hardware](#hardware)
  * [Software](#software)
  * [Makefile](#Makefile)
* [source code](#code)

<h2 id="overview">Introduction</h2>

The project name is “electronic bookcases”, which is designed to digitize the information inside the library.

With the server that stores the data, administrators can easily manage and directly grasp the first-hand information at any time. 

As for clients, procedures such as borrowing or returning books will be facilitated.  Clients will be able to handle several formalities by interacting with such system instead of librarian. The system will then collect related data so that administrators can do their work with comfort.

Moreover, we hope to promote the system for in-depth analysis. Throughout further operation which provides convenient capability of management, administrators can be aware of popularity of certain books or have other thorough information in hand.


<h2 id="video">Video</h2>
https://v.youku.com/v_show/id_XMzY3OTI0MDg3Ng==.html?spm=a2h3j.8428770.3416059.1

<h2 id="struct">Configuration</h2>
<h3 id="hardware">Hardware</h3>

* **DesignWare ARC EM Starter Kit(EMSK)** 

* **Linkit 7688 DUO** 

* **LCD**

* **Keyboard** 
* **A lot of LEDS (depend on how big your shelf is)**

The version of EM Starter Kit is currently EMSK22 em7d.

Linkit 7688 DUO is meant to build internet connection between client and server side.

#### Hardware Connection
1. The linkit 7688 DUO is connected with ARC EMSK via uart on **J1**
2. Lcd is connected with ARC EMSK via I2C on **J4**
3. Keyboard is connected with ARC EMSK via gpio on **J3**
4. LEDS are connected with ARC EMSK via gpio on **J5 and J6**
5. Configure your EMSKs with proper core configuration.

<h3 id="software">Software</h3>

* **Metaware or ARC GNU Toolset** 
* **Arduino IDE (for linkit 7688 DUO mcu)**
* **Filezilla or other ftp software**
* **Serial port terminal, such as putty, tera-term or minicom (for linkit 7688 DUO mpu)**

In this project, we use **embarc osp 2017.12** and **arc gnu tool chain 2017.09**

#### Run This Application

Modify the settings for connecting to the App, as shown below:

Here take **EMSK2.2 - ARC EM7D** with GNU Toolset and **Linkit 7688 DUO** with Arduino IDE and filezilla for example to show how to run this application.

1. We need to use embARC GNU Toolset to load application for EMSK and run.

2. Set linkit to client mode and connect to wifi router.

3. Open Arduino IDE to load application for linkit 7688 DUO MCU and run.

4. Open putty and start a ssh connect with linkit MPU.

5. Open Filezilla to upload application for linkit 7688 DUO MPU and use putty to run.
-Before running application on linkit 7688 DUO MPU , modify following code first:

		url="http://192.168.1.103//ARC2DB.php"

**URL depends on where your mySQL server is.**

<h5 id="Makefile">Get ipaddress</h5>

- Take Windows as example:

		command line: ipconfig

<h3 id="Makefile">Makefile</h3>

- Target options about EMSK and toolchain:

		BOARD ?= emsk
		BD_VER ?= 22
		CUR_CORE ?= arcem7d
		TOOLCHAIN ?= gnu

- The relative series of the root directory, depends on your structure. 

		#
		# root dir of embARC
		#
		EMBARC_ROOT = ../..
 
<h2 id="code">source code</h2>

* **src** 

|  folder/file             |            Function                |
| ------------------------ | -----------------------------------|
|  src/arc                 | source code for EMBARC	        |
|  src/linkit7688          | source code for linkit7688DUO      |
|  src/database            | source code for database (mySQL)   |

* **src/arc/** 

|  folder/file             |            Function                |
| ------------------------ | -----------------------------------|
|  team4/                  | project's source code folder       |
|  team4/main.h & main.c   | project's main function            |
|  team4/LCD.h  & LCD.c    | LCD drivers                        |
|  team4/makefile          | project's configuration            |

* **src/linkit7688** 

|  folder/file               |            Function                                      |
| -------------------------- | -----------------------------------------------------    |
|  linkit-mpu/               | linkit 7688 DUO mpu source code folder                   |
|  linkit-mpu/arc2018.py     | deal with network in this project                        |
|  linkit_mcu/               | linkit 7688 DUO arduino-compatible mcu source code folder|
|  linkit_mcu/linkit_mcu.ino | bridge between **arc emsk** and **linkit MPU**           |

* **src/database/Library** 

|  folder/file                     |            Function                        |
| -------------------------------- | -------------------------------------------|
|  ARC2DB.php                      | project source code for database operation |
|  ARC-Hive Library Information.php| display all information                    |
|  Book.php                        | display books' status                      |
|  connect database.php            | project source code for database connection|
|  Location.php                    | display available location                 |
|  ReservationRecoed.php           | display reservation history of each books  |
|  WaitingQueue.php                | display current reservation status         |


