# iRhythm Internet MP3 Radio

This appilcation is a prototype of **Internet Radio** designed on ARC EM Starter Kit,which could find music from online audition websites,like *douban FM*,*Baidu FM*,*KuWo*,Download mp3 file among the music,and decode them by using **ARC DSP Instruction** and **optimized code** for mp3 decode.By adding a I2S peripheral with a **FPGA**,song can be sent out and outputted from two **speaker**.

-----------------------------------------

* [Introduction](#Introduction)
    - [Function](#function)
    - [Appearance](#appearance)
    - [Video](#video)
* [Hardware and Software Setup](#hardware-and-software-setup)
    - [Required Hardware](#required-hardware)
    - [Required Software](#required-software)
    - [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
    - [Before Running This Application](#before-running-this-application)
    - [Run This Application](#run-this-application)


-------------------------------

## Intruduction
iRhythm is a Internet Radio,which can download music from online audition websites,decompress the file and play,while the music file in SD card can also be decoded and played.
With the help of a FPGA, the digital music data in SPI format can be converted into I2S format,which can be recognize by audio D/A convertor.
Through an amplifier outside,the online music can be play out from two big speaker.And it's a really good feeling!

### Function
* Online Music Crawling and Download
* Decode Mp3 file 
* Protocol transit form SPI to I2S 
* Power Amplification 
* Performance data display

### Appearance
* **System Top View -- Control Part**
![Control Part][2]

* **System Middle View -- Signal Processing Part**
![Signal Processing Part][3]
![Signal Processing Part][4]

* **System Bottom View -- Output Part**
![Output Part][5]

* **System View**
![iRhythm][6]

### Video
[ iRhythm demo video ][7]

## Hardware and Software Setup

-------------------------------
### Required Hardware
* Necessary Hardware:
    - DesignWare ARC EM Starter Kit(EMSK)        *1
    - SD card                                    *1
    - ESP8266                                    *1
    - Cmod A7 XC7A35T FPGA develop board         *1
    - Pmod I2S                                   *1
    - OLED(12864)                                *1
    - Headphone                                  *1

![Necessary Hardware][8]
--------------------------------

* Additional Hardware:
    - Lipo Battery                               *1
    - Buck power regulator                       *2
    - Audio Amplifiers                           *1
    - Speaker                                    *1

![Additional Hardware][9]

---------------------------------

### Required Software
* embarc_ops **newest** commit:976b8ed3b24b061df91f78bcc71a726dd2a63fae
* Metaware or ARC GNU Toolset
* Serial port terminal, such as putty, tera-term or minicom
* Any bitstream file can config your FPGA as a SPI to I2S protocol interface
* More than one MP3 file in SD card to avoid waiting while downloading

### Hardware Connection
1. Insert your SD card with mp3 file in it.
2. Make sure all your module has been mounted on the board.
3. Make sure your power regulator connect is correct,including the voltage and polarity, or the prototype may be damaged
4. Connect your 3.5 inch Audio cable.
5. After all connector has been connected correctly,connect the Li-po battery to power up the design.

## User Manual

### Before Running This Application
* Download source code of iRhythm from github.
* Download ./doc/spi2i2s.bit,which is the bitstram file of Cmod A7 to extend a i2s peripherals
* Make sure all connection is correct again.
* Make sure iRhythm is in the Wifi environment,which is matching with the setting in the code,including wifi name and wifi password.
* Check the switch 1 and 2 of EMSK are in off state to boot as ARCem_7d core.

### Run This Application
* Download with USB-JTAG or use bootloader to boot the program.
* After iRhythm connect the wifi,initialization will complete very fast and OLED may light to show the song reading from SD card.
* After that song can be heard from the speaker and OLED shows download speed and decode speed.If not,try to rotate the volume bar to increase the volume.
* the user can control with the key any time.

#### Makefile

- Selected FreeRTOS here, then you can use [FreeRTOS API][10] in your application:

>>     # Selected OS
>>     OS_SEL ?= freertos
    

- Target options about EMSK and toolchain:

>>      TOOLCHAIN=gnu
>>      BOARD=emsk
>>      BD_VER=23
>>      CUR_CORE=arcem7d

- The relative series of the root directory, here the path of the Makefile is `./embarc_osp/application/iRhythm/makefile`:

>>        #
>>        # root dir of embARC
>>        #
>>        EMBARC_ROOT = ../..

- Directories of source files and header files, notice that it **is not recursive**:

>>        # application source dirs
>>        APPL_CSRC_DIR = ./src/GUI ./src/MUSIC ./src/MUSIC/mp3_dec ./src/MUSIC/mp3_dec/mp3_Ori ./src/MUSIC/mp3_dec/mp3_Dsp ./src/HW_ASSI ./src/NET ./src/NET/esp8266_wifi ./src/TASK
>>        APPL_ASMSRC_DIR = .

>>        # application include dirs
>>        APPL_INC_DIR = ./inc ./inc/mp3_dec
>>        APPL_DEFINES =

- Directories of Linker Script File,to use CCM

>>        # linker script file
>>        LINKER_SCRIPT_FILE ?= linker_file.ldf
>>

See [ embARC Example User Guide][11], **"Options to Hard-Code in the Application Makefile"** for more detailed information about **Makefile Options**.

#### GUI

Placing the GUI source code in `src/GUI` folder.
Placing the C source file.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  gui.c              |      gui control reflash      |
|  key.c              |     key interrupt callback    |

#### HW_ASSI

Placing the HW_ASSI source code in `src/HW_ASSI` folder.
Placing the C source file.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  dma2spi.c          |    dma configuration for spi  |
|  iosignal.c         |        IO initialization      |
|  mem.c              |        SD file read           |

#### MUSIC

Placing the MUSIC source code in `src/MUSIC` folder.
Placing the C source file.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  mp3_dec/           |walkgeek library for mp3 decode|
|  filelist.c         |   api for link list control   |
|  mp3api.c           |  top lever Api of mp3 decode  |
|  music.c            |    music task top lever api   |

#### NET

Placing the NET source code in `src/NET` folder.
Placing the C source file.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  esp8266_wifi/      |   control api for ESP8266     |
|  net.c              |      api for net task         |

#### TASK

Placing the TASK source code in `src/TASK` folder.
Placing the C source file.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  GUI_task.c         |  main function of gui task    |
|  MUSIC_task.c       |  main function of mussic task |
|  NET_task.c         |  main function of net task    |
|  main.c             |    start application          |

#### INCLUDE

Placing the INCLUDE source code in `inc` folder.
Placing the HEADER source file.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  mp3_dec            |walkgeek library for mp3 decode|
|at_parser.h/esp8266.h|    control api for ESP8266    |
|  include.h          |  all declaration of c file    |
| perform.h/inc_task.h|   api for debuging            |
| FreeRTOSConfig.h    |   configuration of ROTS       |
| ssd1306_app_config.h|   configuration of u8glib     |

#### DSP
Placing the DSP source code in `inc` folder.
Placing the HEADER source file.

|  folder/file        |            Function           |
| ------------------- | ------------------------------|
|  Dsp_assembly.h     |   optimize code for Arc Dsp   |


[2]: ./doc/pic/top.jpg
[3]: ./doc/pic/mid.jpg
[4]: ./doc/pic/mid2.jpg
[5]: ./doc/pic/bottom.jpg
[6]: ./doc/pic/main3.jpg
[7]: http://v.youku.com/v_show/id_XMzYzMTg2MDA3Ng==.html?spm=a2h3j.8428770.3416059.1   "iRhythm demo video"
[8]: ./doc/pic/necessaryhw.png
[9]: ./doc/pic/addhw.png
[10]: http://www.freertos.org/a00106.html   "FreeRTOS API"
[11]: http://embarc.org/embarc_osp/doc/embARC_Document/html/page_example.html   " embARC Example User Guide"

