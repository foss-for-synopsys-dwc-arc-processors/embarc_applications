# Identification VI Trace of Appliance

This application is a prototype of **Electrical Load Identification** designed on ARC IoT DK. It can generate a VI trace chart according to the current and voltage waveforms of electrical appliances, and then identify the types of electrical appliances according to the VI trace diagram. The analog signal of **current** and Voltage is collected by **Current Transformer(CT)** and **Potential Transformer(PT)**, then converted into digital signal by **ATT7053** (*power acquisition chip*).

-----------------------------------------

- [Electrical Load Identification](#electrical-load-identification)
  - [Introduction](#introduction)
    - [Function](#function)
    - [Appearance](#appearance)
    - [Video](#video)
  - [Hardware and Software Setup](#hardware-and-software-setup)
    - [Required Hardware](#required-hardware)
	- [Necessary Hardware](#necessary-hardware3)
    - [Required Software](#required-software)
    - [Hardware Connection](#hardware-connection)
  - [User Manual](#user-manual)
    - [Before Running This Application](#before-running-this-application)
    - [Run This Application](#run-this-application)
      - [Makefile](#makefile)
      - [Program flow chart](#program-flow-chart)
      - [Model](#model)
      - [Data Processing](#data-processing)
      - [OLED Display](#oled-display)


-------------------------------

## Introduction
The detailed monitoring of power user's electric load (load monitoring) is the first step to realize power demand side management (DSM). Load monitoring technology monitors the detailed operation status of each kind of electrical appliances within the user by sampling and analyzing the total load data of users, so as to obtain the data information of power consumption and electricity consumption behavior of each kind of electrical appliances of power users Power companies and other parties are of great significance. In terms of users, such as the smart socket with load identification, it can count the power consumption of household appliances and control the appliances in real time; in the aspect of power grid, such as smart meter, the accuracy of load modeling can be improved by load monitoring, such as considering the time-varying and distribution of load to achieve higher precision load modeling.

We used Synopsys IOT DK to design the system,which can generate VI trace map through current and voltage data measured by sensors. Each kind of electrical appliance corresponds to different VI trace diagram. Through the neural network API provided by embarc MLI library, CNN algorithm can be run on Embedded MCU to identify VI trace map, which avoids uploading data to the cloud for identification, reduces the delay of the system, and makes it better adaptive quotient Business environment requirements.

### Function
* Identifying Electrical Appliances. *At present, it can identify five kinds of electrical appliances by their VI trace(Fan, Compact Fluorescent Lamp, Laptop charger, Hairdryer, Incandescent Light Bulb)*
* Display the VI trace of the current appliance and show the appliance`s name on OLED


### Appearance
* **System View**
![System Overall][1]


### Video
[ Electrical Load Identification demo video ][2]

## Hardware and Software Setup

-------------------------------
### Required Hardware
* Necessary Hardware:
    - ARC IoT Development Kit(IoT DK)            *1
    - ATT7053                                                          *1
    - ZMCT103                                                         *1
    - ZMPT107                                                         *1
    - OLED(12864)                                                  *1
* Practicality picture

![](.\doc\practicality picture.JPG)

---------------------------------

### Required Software
* ARC GNU Toolchain 2019.06
* embarc_osp(Branch:embarc_mli): https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp.git
* Serial port terminal, such as SecureCRT or Xshell
* Python3.6
* Pycharm
* Tensorflow 1.13.1

### Hardware Connection
1. Connect ATT7053 module to IoT DK 2x18 Pin Extension Header(Using SPI0 interface and SPI0_CS1 Pin).
3. Connect OLED module to IoT DK 2x18 Pin Extension Header(Using IIC0 interface).
4. Make sure your power regulator connect is correct,including the voltage and polarity.
5. After all connector has been connected correctly,connect the Li-po battery to power up the design.

## User Manual

### Before Running This Application
* Download source code of Identification VI Trace of Appliance from github.
* Make sure all connection is correct again.
* Passing Zero or Fire Wire of Socket Wire through Current Transformer.
* Connect the zero line and live line to the Potential Transformer.


### Run This Application
Here take IoT DK, CUR_CORE = arcem9d with ARC GNU Toolchain 2019.06 for example to show how to run this application.  
1. To build this applicaiton, select the proper board version, core configuration and build with selected toolchain using this command `make BOARD=iotdk BD_VER=10 CUR_CORE=arcem9d TOOLCHAIN=gnu run`.  
2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and 115200bps.  
3. Interact using IoT DK and serial port terminal.  

#### Makefile

- Selected embarc_mli here, then you can use [embarc_mli API][4] in your application:

>>     # Library
>>     LIB_SEL = embarc_mli

- Selected u8glib here, then you can use [u8glib API][5] in your application:

>>     # Middleware
>>     MID_SEL = common	u8glib

- Other Configure:

>>      # use -Hpurge option to optimize the code size
>>      ifeq ($(TOOLCHAIN), gnu)
>>      ADT_COPT += -ffunction-sections -fdata-sections
>>      ADT_LOPT += -Wl,--gc-sections
>>      else
>>      ADT_COPT += -Hpurge
>>      ADT_LOPT += -Hpurge
>>      endif


- Target options about EMSK and toolchain:

>>      TOOLCHAIN=gnu
>>      BOARD=iotdk
>>      BD_VER=10
>>      CUR_CORE=arcem9d

- The relative series of the root directory, here the path of the Makefile is `./embarc_osp/application/electrical_load_identification/makefile`:

>>        #
>>
>>        # root dir of embARC
>>        #
>>        EMBARC_ROOT = ../..

- Directories of source files and header files, notice that it **is not recursive**:

>>        # application source dirs
>>        APPL_CSRC_DIR = . model
>>        APPL_ASMSRC_DIR = .

>>        # application include dirs
>>        APPL_INC_DIR = . model
>>        APPL_DEFINES = APPL_DEFINES = -DUSE_APPL_MEM_CONFIG -DV2DSP_XY -DMODEL_BIT_DEPTH=16 -g


See [ embARC Example User Guide][6], **"Options to Hard-Code in the Application Makefile"** for more detailed information about **Makefile Options**.

#### Program flow chart
![Program flow chart][7]



#### Model

Placing the model source code in `./model` folder.
Placing the C source and header file.

| folder/file        | Function                           |
| ------------------ | ---------------------------------- |
| coefficients_chw.c | Define weights and biases of LeNet |
| constants.h        | Define Constants of LeNet          |
| model_chw.c        | Define the structure of LeNet      |
| model.h            | the header of LeNet model          |
![Structural Chart of LeNet Model][8]

#### ATT7053 Driver

Placing the device ATT7053 Driver source code in `./` folder.
Placing the C source and hearder file.

| folder/file   | Function                                        |
| ------------- | ----------------------------------------------- |
| spi_att7053.c | att7053 spi drivers,include read,write,init API |
| spi_att7053.h | the header of att7053 drivers                   |

#### VI Image  Generation Algorithm

Placing the Algorithm of  VI Image Generation source code in `./` folder.
Placing the C source and hearder file.

| folder/file           | Function                                       |
| --------------------- | ---------------------------------------------- |
| VI_image.c            | generate VI image algorithm                    |
| VI_image.h            | the header of generate VI image algorithm      |
| hsv2rgb.c             | convert VI image from HSV coding to RGB coding |
| hsv2rgb.h             | the header of hsv2rgb.c                        |
| recognize_algorithm.c | VI image recognize algorithm                   |
| recognize_algorithm.h | the header of VI image recognize algorithm     |

#### OLED Display

Placing the INCLUDE source code in `./` folder.
Placing the C source and header file.

| folder/file          | Function                                                     |
| -------------------- | ------------------------------------------------------------ |
| VI_image_display.c   | Display interface function and convert RGB images to Binary images for display |
| VI_image_display.h   | the hearder of oled_display                                  |
| ssd1306_app_config.h | Definition of OLED Interface                                 |


[1]: ./doc/pic/system blocks.jpg
[2]: https://v.youku.com/v_show/id_XNDI5MTM3MjQ4OA==.html?spm=a2h3j.8428770.3416059.1  "Electrical Load Identification"
[3]: ./doc/pic/practicality picture.jpg
[4]: https://embarc.org/embarc_mli/doc/build/html/index.html
[5]: https://github.com/olikraus/u8glib/wiki
[6]: https://embarc.org/embarc_osp/doc/build/html/index.html
[7]: ./doc/pic/Program flow chart.jpg
[8]: ./doc/pic/LeNet Model structure.jpg

