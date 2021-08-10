# Smart Meter

This application is a prototype of **Smart Meter** designed on ARC IoT DK. It can measure household electricity consumption information, and then use **non-intrusive load identification technology** to identify electrical switch status. It uploads message to the **cloud platform**. The analog signal of current and Voltage is collected by **ATT7053AU** (*a power acquisition chip*).

- [Smart Meter](#smart-meter)
  - [Introduction](#introduction)
    - [Function](#function)
    - [Appearance](#appearance)
    - [Video](#video)
  - [Hardware and Software Setup](#hardware-and-software-setup)
    - [Required Hardware](#required-hardware)
    - [Required Software](#required-software)
    - [Hardware Connection](#hardware-connection)
  - [User Manual](#user-manual)
    - [Before Running This Application](#before-running-this-application)
    - [Run This Application](#run-this-application)

## Introduction
The traditional meter can only measure the user's power consumption, but in today's more and more intelligent, concepts such as smart grid and smart home are gradually popularized and realized. Non-intrusive load identification uses meter data, combined with machine learning algorithms to analyze the power consumption status of all electrical equipment in the home. It is one of the key technologies for the refined management of power energy and the realization of the intelligentization of the whole house. This application embeds non-intrusive load identification technology into the end of the electric energy measurement equipment, and performs calculations on the end side to avoid waste of resources caused by a large amount of data upload, and at the same time, it can perform real-time load pattern recognition. This application is developed based on the Synopsys IoTDK platform, uses the Tensorflow Lite for Microcontroller neural network model for inference, and realizes the connection of device networking and the mobile phone and web pages of the cloud platform. 

### Function
* Measure the electrical power.
* Real-time non-intrusive load identification.
* upload the message to the cloud platform.


### Appearance
* **System View**
<img src="./doc/pic/system architecture.png" alt="System Architecture" style="zoom:60%;" />

### Video

[Smart Meter with NILD][1] 

## Hardware and Software Setup

### Required Hardware
* Necessary Hardware:
    - ARC IoT Development Kit(IoT DK)               *1
    - ATT7053AU                                                     *1
    - ESP8266                                                          *1
    - OLED                                                               *1
* Practicality picture

### Required Software
* DesignWare ARC MetaWare Development Toolkit 2021.03
* [embARC open software platform][2]
* [Tensorflow lite for microcontrollar][3]
* Ali Cloud Platform
* Serial port terminal, such as SecureCRT or Xshell
* Python3.8
* Tensorflow 2.5

### Hardware Connection
1. Connect ATT7053AU module to IoT DK 2x18 Pin Extension Header (Using SPI0 interface and SPI0_CS1 Pin).
2. Connect OLED module to IoT DK 2x18 Pin Extension Header (Using IIC0 interface).
3. Connect ESP8266 module to IoT DK  12 Pin Pmod connector Pmod_C (Using UART0 interface).
4. Make sure your power regulator connect is correct,including the voltage and polarity.

## User Manual

### Before Running This Application
* Download source code of **Smart Meter** from github.
* Make sure all connection is correct again.
* Passing Zero or Fire Wire of Socket Wire through Current Transformer on ATT7053AU.
* Connect the zero line and live line to the Potential Transformer on ATT7053AU.
* If you want to use the cloud platform, set up your own hotspot SSID and password.


### Run This Application
1. To build this applicaiton, select the proper board version, core configuration and build with selected toolchain using this command `make BOARD=iotdk BD_VER=10 CUR_CORE=arcem9d TOOLCHAIN=mw run`.  
2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and 115200bps.  
3. Interact using IoT DK , serial port terminal and cloud platform.  

#### Makefile

- Add the path of libmli.a to application library

  ```
  # application library
  APPL_LIBS ?= third_party/arc_mli_iotdk_arcem9d/bin/libmli.a
  ```

- Selected u8glib here, then you can use [u8glib API][4] in your application:

  ```
  MID_SEL = common u8glib
  ```


- Target options about IoT DK and toolchain:

  ```
  TOOLCHAIN=mw
  BOARD=iotdk
  BD_VER=10
  CUR_CORE=arcem9d
  ```

- The relative series of the root directory, here the path of the Makefile is `./embarc_osp/application/smart_meter/src/makefile`:

  ```
  # root dir of embARC
  EMBARC_ROOT = ../../..
  ```

- Directories of source files and header files, notice that it **is not recursive**:

  ```
  # application source dirs
  
  APPL_CSRC_DIR = . \
  tensorflow/lite/c \
  tensorflow/lite/experimental/microfrontend/lib \
  ./models
  
  APPL_CXXSRC_DIR = . \
  ./models \
  tensorflow/lite/core/api \
  tensorflow/lite/experimental/microfrontend/lib \
  tensorflow/lite/kernels \
  tensorflow/lite/kernels/internal \
  tensorflow/lite/micro \
  tensorflow/lite/micro/arc_iotdk \
  tensorflow/lite/micro/kernels \
  tensorflow/lite/micro/kernels/arc_mli \
  tensorflow/lite/micro/memory_planner \
  tensorflow/lite/micro/testing \
  
  # application include dirs
  APPL_INC_DIR = . \
  ./models \
  tensorflow/lite \
  tensorflow/lite/c \
  tensorflow/lite/core/api \
  tensorflow/lite/experimental/microfrontend/lib \
  tensorflow/lite/kernels \
  tensorflow/lite/kernels/internal \
  tensorflow/lite/micro \
  tensorflow/lite/micro/kernels \
  tensorflow/lite/micro/kernels/arc_mli \
  tensorflow/lite/micro/memory_planner \
  tensorflow/lite/micro/testing \
  tensorflow/lite/schema \
  third_party/arc_mli_iotdk_arcem9d/include \
  third_party/arc_mli_iotdk_arcem9d/include/api \
  third_party/flatbuffers/include \
  third_party/gemmlowp \
  third_party/gemmlowp/fixedpoint \
  third_party/gemmlowp/internal \
  third_party/kissfft \
  third_party/ruy 
  ```

#### Program flow chart
<img src="./doc/pic/program flow chat.png" alt="Program flow chart" style="zoom:30%;" />



#### Tensorflow Lite for Microcontrollor

Placing the tflite model source code in `./models` folder.

| folder/file   | Function                                                 |
| ------------- | -------------------------------------------------------- |
| models        | include tflite models source code and header file        |
| recongnize.cc | use Tensorflow Lite for Microcontrollor to run inference |
| recongnize.h  | the header of recongnize.cc                              |

#### Driver

| folder/file    | Function                                        |
| -------------- | ----------------------------------------------- |
| spi_att7053.cc | att7053 spi drivers,include read,write,init API |
| spi_att7053.h  | the header of att7053 drivers                   |
| my_uart.cc     | uart drivers                                    |
| my_uart.h      | the head of uart driver                         |

#### Network and Cloud Platform

| folder/file        | Function                                             |
| ------------------ | ---------------------------------------------------- |
| esp8266_network.cc | connect to the hotspot by esp8266                    |
| esp8266_network.h  | the head of esp8266_network                          |
| self_mqtt.cc       | the functions of Message Queuing Telemetry Transport |
| self_mqtt.h        | the head of mqtt                                     |

#### OLED Display

| folder/file          | Function                                                 |
| -------------------- | -------------------------------------------------------- |
| display.cc           | interface function for using OLED to display information |
| display.h            | the hearder of display                                   |
| ssd1306_app_config.h | Definition of OLED Interface                             |

[1]: https://v.youku.com/v_show/id_XNTE5MTkzMjQwNA==.html
[2]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp
[3]: https://github.com/tensorflow/tflite-micro
[4]: https://github.com/olikraus/u8glib/wiki
