# Electrical Load Identification

This application is a prototype of **Electrical Load Identification** designed on ARC Iot DK, which can identify the type of appliance according to the current waveform, and display it through OLED. The analog signal of **current** is collected by **Current Transformer(CT)**, then converted into digital signal by **A/D(AD7991)**. The data is input to **ARC IoT DK** for data preprocessing, then the processed data is input to **Convolutional Neural Network(CNN)** for classification. Finally, the current type of electrical appliances is displayed on **OLED**.  

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
Non-intrusive load monitoring (NILM) is a conceptual framework for the development of power systems. Compared with traditional load monitoring methods, non-intrusive load monitoring is more intelligent. </br>NILM collects current and voltage signals during load operation by installing signal sensors at the entrance of sockets, then transfers the data to Iot DK development board, and carries out load identification through CNN built by ARC MLI library.</br>Users can make statistics and analysis of household energy consumption data, which can help users make rational use of electricity and reduce unnecessary losses.

### Function
* Identifying Electrical Appliances. *At present, it can identify three kinds of electrical appliances and their two previous combinations (hot water kettle, electric fan, mobile phone charger)*
* Display the current types of electrical appliances 


### Appearance
* **System View**</br>
![System Overall][1]


### Video
[ Electrical Load Identification demo video ][2]

## Hardware and Software Setup

-------------------------------
### Required Hardware
* Necessary Hardware:
    - ARC IoT Development Kit(IoT DK)            *1
    - AD7991                                     *1
    - ZMCT103C                                   *1
    - OLED(12864)                                *1

![Necessary Hardware][3]
--------------------------------

---------------------------------

### Required Software
* ARC GNU Toolchain 2019.03
* embarc_osp(Branch:embarc_mli): https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp.git
* Serial port terminal, such as SecureCRT or Xshell
* Python3.6
* Pycharm
* Tensorflow 1.13.1

### Hardware Connection
1. Connect AD7991 module to IoT DK 2x18 Pin Extension Header(Using IIC0 interface).
2. Connect analog output of current sensor to AD7991 CH1 channel.
3. Connect OLED module to IoT DK 2x18 Pin Extension Header(Using IIC1 interface).
4. Make sure your power regulator connect is correct,including the voltage and polarity.
5. After all connector has been connected correctly,connect the Li-po battery to power up the design.

## User Manual

### Before Running This Application
* Download source code of Electrical Load Identification from github.
* Make sure all connection is correct again.
* Passing Zero or Fire Wire of Socket Wire through Current Transformer.


### Run This Application
Here take IoT DK, CUR_CORE = arcem9d with ARC GNU Toolchain 2019.03 for example to show how to run this application.  
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

- Selected AD7991 here:

>>     # Device
>>     EXT_DEV_LIST += adc/ad7991

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

| folder/file                        | Function                         |
| ---------------------------------- | -------------------------------- |
| load_identification_coefficients.c | Define weights and biases of CNN |
| load_identification_constants.h    | Define Constants of CNN          |
| load_identification_model.c        | Define the structure of CNN      |
| load_identification_model.h        | the header of CNN model          |
![Structural Chart of CNN Model][8]

#### Data Processing

Placing the data processing source code in `./` folder.
Placing the C source and hearder file.

| folder/file    | Function                                           |
| -------------- | -------------------------------------------------- |
| process.c      | Processing the results input to CNN and CNN output |
| process.h      | the header of process                              |
| input_buffer.c | Save collected current data                        |
| input_buffer.h | the hearder of input_buffer                        |


#### OLED Display

Placing the INCLUDE source code in `./` folder.
Placing the C source and header file.

| folder/file          | Function                        |
| -------------------- | ------------------------------- |
| oled_display.c       | OLED display interface function |
| oled_display.h       | the hearder of oled_display     |
| ssd1306_app_config.h | Definition of OLED Interface    |


[1]: ./doc/pic/system.jpg
[2]: http://www.baidu.com   "Electrical Load Identification"
[3]: ./doc/pic/hardware.jpg
[4]: https://embarc.org/embarc_mli/doc/build/html/index.html
[5]: https://github.com/olikraus/u8glib/wiki
[6]: https://embarc.org/embarc_osp/doc/build/html/index.html
[7]: ./doc/pic/prog_chart.jpg
[8]: ./doc/pic/CNN_graph.jpg

