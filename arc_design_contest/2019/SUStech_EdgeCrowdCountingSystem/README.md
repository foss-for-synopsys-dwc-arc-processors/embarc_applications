# Edge Crowd Counting System

## Introduction

It's a demo of Edge-to-cloud collaboration to solve the crowd counting problem in SMART City. At present,in the crowd analysis prog of cameras on the market, the analysis of the crowd characteristics needs to upload images from the edge to the central server for unified analysis and processing,with poor real-time performance, but requiring high power consumption, big storage, and wide network bandwidth. So, This project proposes a real-time crowd density detection model based on deep learning, and deploys the model to the ARC MCU, analyzes the video in real time on the edge computing platform, and uploads the structured population information to the cloud center. 
The crowd information analysis technology at the edge end greatly reduces the computing, network bandwidth and storage requirements for the cloud center. The video image is directly processed and analyzed at the edge, which improves the response speed, compensates for the shortcomings of cloud computing response, and high power consumption. It can meet the needs of the security industry in real-time business, urban security, etc., It has broad application prospects.

### Function

* **Crowd Count**

  Counting the number of people from pictures or video. 

* **Congestion Warning**

  When the total population of the region exceeds the threshold set by the system, the node will return the alarm information to the server.

* **Data processing on the edge**

  To handle the data explosion problem in IoT era, preliminary data processing work is transferred from cloud server to edge node. In our system, edge node can handle simple data preprocessing works like data gathering, data cleaning etc.

* **Analysis of Flow Trend**

  If multi-nodes is set in a large region, edge processing node can forecasting the flow of people.

* **Data Transmission**

  Upload processed data to the cloud.

### Architecture

![屏幕快照 2019-07-24 下午6.28.14](http://ww3.sinaimg.cn/large/006tNc79ly1g5b3ukesxcj315c0m6k4a.jpg)



Due to limited development time, we use raspberry pi 3b+ with picam as video processing nodes and ARC IoTDK as edge data processing node. Actually we believe that IoTDK can replace raspberry pi as video processing node in theoretically with MLI lib but more works have to be done. In our future work, we will try to move this part to IoTDK.

## Hardware and Software Setup

### Hardware Required List

* ARC IoTDK or EMSK
* SD Card
* ESP01/ESP01S WiFi Module
* Raspberry pi 3b+ with picam

### Set Up Software environment

#### a. Raspberry Part

* python2.7
* tensorflow lite
* opencv
* socket

#### b. ARC IoTDK or EMSK

##### For EMSK

- ESP01/ESP01S WiFi Module  connectes to J5.
- Configure your EMSK with proper core configuration(EM9D).
- Insert SD Card

##### For IoTDK

- ESP01/ESP01S WiFi Module connects to PMOD B.
- Insert SD Card

## User Manual

### Crowd Counting Part

This part is implement on Raspberry pi.

* Prepare training data：[BaiduDisk](http://pan.baidu.com/s/1nuAYslz)

* Train The Model

  ~~~python
  #For tensorflow
  #run from prompt
  python3 train.py A(or B) 
  #Model is saved to modelA/ or modelB/
  ~~~

* Prepared the model

  ~~~bash
  python3 train.py
  ~~~

* Run this project

  ~~~bash
  python3 main.py
  ~~~

### Data Processing Node

This part can implement on both ARC IoTDK  and EMSK .

#### Before Running

- In the `main.c`, you need to replace WIFI_SSID and WIFI_PWD with your own SSID and PWD.

  ```C
  #define WIFI_SSID   "\"embarc\""
  #define WIFI_PWD    "\"12344321\""
  ```

  ##### For EMSK

  - In the `main.c`, you need to set the SD card header file.

  ```
  #include "emsk_sdcard.h"
  #include "emsk_sdcard_spi.h"
  ```

  - In the `Makefile`, you need to set up SD card environment.

  ```
  MID_SEL = common fatfs
  APPL_CSRC_DIR = . ./wifi ./emsk_sdcard
  APPL_INC_DIR = ./inc ./inc/emsk_sdcard
  ```
  
  ##### For IoTDK
  
  - In the `main.c`, you need to set the SD card header file.
  
  ```
  #include "emsk_sdcard.h"
  #include "emsk_sdcard_spi.h"
  ```
  
  - In the `Makefile`, you need to set up SD card environment.
  
  ```
  MID_SEL = common fatfs
  APPL_CSRC_DIR = . ./wifi ./iotdk_sdcard
  APPL_INC_DIR = ./inc ./inc/iotdk_sdcard
  ```
#### Run This Application

This example outputs to the console. It is supported by IoTDK and EMSK. The commands to run this example are as follows:

	$ cd <embarc_root>/embarc_applications-master/arc_design_contest/2019/   SUStech_EdgeCrowdCountingSystem/src
	$ make BOARD=emsk BD_VER=22 CUR_CORE=arcem9d TOOLCHAIN=mw run
##### Sample Output

```
-----------------------------------------------------------
 ____                                _ ____
|  _ \ _____      _____ _ __ ___  __| | __ ) _   _
| |_) / _ \ \ /\ / / _ \ '__/ _ \/ _` |  _ \| | | |
|  __/ (_) \ V  V /  __/ | |  __/ (_| | |_) | |_| |
|_|   \___/ \_/\_/ \___|_|  \___|\__,_|____/ \__, |
                                             |___/
                     _       _    ____   ____
       ___ _ __ ___ | |__   / \  |  _ \ / ___|
      / _ \ '_ ` _ \| '_ \ / _ \ | |_) | |
     |  __/ | | | | | |_) / ___ \|  _ <| |___
      \___|_| |_| |_|_.__/_/   \_\_| \_\\____|
------------------------------------------------------------

embARC Build Time: Jun 13 2019, 06:45:36
Compiler Version: Metaware, 4.2.1 Compatible Clang 6.0.1 (branches/release_60)
FatFS initialized successfully!
============================ Init ============================
[at_parser_init]57: obj->psio 0x8000af18 -> 0x80001868
[at_send_cmd]131: at_out: "AT
" (4)
[at_get_reply]154: "AT

OK" (9)
============================ Set Mode ============================
[at_send_cmd]131: at_out: "AT+CWMODE_CUR?
" (16)
[at_get_reply]154: "
AT+CWMODE_CUR?
+CWMODE_CUR:1

OK" (38)
CWMODE_CUR = 1
[at_send_cmd]131: at_out: "AT+CWMODE_CUR=3
" (17)
[at_get_reply]15▒: "
AT+CWMODE_CUR=3

OK" (24)
============================ Connect WiFi ============================
[at_send_cmd]131: at_out: "AT+CWLAP="embarc"
" (19)
[at_get_reply]154: "
AT+CWLAP="embarc"
+CWLAP:(3,"embarc",-20,"2e:ea:1d:bd:4f:8b",9,32767,0)

OK" (81)
Searching for WIFI "embarc" ......
WIFI "embarc" found! Try to connect
[at_send_cmd]131: at_out: "AT+CWMODE_CUR=1
" (17)
[at_get_reply]154: "
AT+CWMODE_CUR=1

OK" (24)
[at_send_cmd]131: at_out: "AT+CWJAP_CUR="embarc","12344321"
" (34)
[at_get_reply]154: "
AT+CWJAP_CUR="embarc","12344321"
0,CONNECT FAIL
WIFI DISCONNECT
WIFI CONNECTED
WIFI GOT IP

OK" (103)
WIFI "embarc" connect succeed
============================ Connect Server ============================
[at_send_cmd]131: at_out: "AT+CIPMUX=1
" (13)
[at_get_reply]154: "
AT+CIPMUX=1

OK" (20)
[at_send_cmd]131: at_out: "AT+CIPSERVER=1,80
" (19)
[at_get_reply]154: "
AT+CIPSERVER=1,80
no change

OK" (37)
============================ Show IP ============================
[at_send_cmd]131: at_out: "AT+CIFSR
" (10)
[at_get_reply]154: "
AT+CIFSR
+CIFSR:STAIP,"192.168.137.254"
+CIFSR:STAMAC,"3c:71:bf:36:e4:f4"

OK" (84)

============================== Receive Data =================================


+IPD,0,19:A061020190725123625

============================ Receive Finished ===============================

========================== Start Data Analysis ==============================

============================= Number of people ================================

Number of people:  The number of people in area A is:0610.
Number of people:  The number of people in area B is:0000.
Number of people:  The number of people in area C is:0000.

============================== Crowd status  ================================

Crowd status: The number of people in area A  has increased.
A0610201907251A:0610

============================= Stores the data =================================

 Data successfully saved!

Number of people:  The total number of people in the current region is: 0610 ,  ,at 25,Jul,2019, 12:36:25,

======================== Data Analysis Finished =============================
There are 610 people in the area.
========================= Data sent to the cloud ===========================

[at_send_cmd]131: at_out: "AT+CIPSEND=0,50
" (17)
[at_get_reply]154: "AT+CIPSEND=0,50

OK" (22)

============================= Send finished ================================


============================= Receive Data ================================


+IPD,0,19:B054020190725123630

=========================== Receive Finished ==============================

========================= Start Data Analysis =============================

========================== Number of people =============================

Number of people:  The number of people in area A is:0610.
Number of people:  The number of people in area B is:0540.
Number of people:  The number of people in area C is:0000.

========================== Crowd status  =============================

Crowd status: The number of people in area B  has increased.
B0540201907251B:0540

============================= Stores the data =================================

 Data successfully saved!

Number of people:  The total number of people in the current region is: 1150 ,  ,at 25,Jul,2019, 12:36:30,

======================== Data Analysis Finished =============================
There are 1150 people in the area.
========================= Data sent to the cloud ===========================

[at_send_cmd]131: at_out: "AT+CIPSEND=0,50
" (17)
[at_get_reply]154: "AT+CIPSEND=0,50

OK" (22)

============================= Send finished ================================


============================== Receive Data =================================


+IPD,0,19:C025020190725123635

============================ Receive Finished ===============================

========================== Start Data Analysis ==============================

============================= Number of people ================================

Number of people:  The number of people in area A is:0610.
Number of people:  The number of people in area B is:0540.
Number of people:  The number of people in area C is:0250.

================================ Crowd status  ==================================


============================== Crowd status  ================================

Crowd status: The number of people in area C  has increased.
C0250201907251C:0250

============================= Stores the data =================================

 Data successfully saved!

=============================== Flow of crowd =================================

Flow of crowd: The crowd is gathering.
* * * * * * * * * * * * * *            * * * * * * * * * * * * * *
*                         *            *                         *
*         *          *    *            *   * * * * * *      *    *
*        * *        * *   *            *   *           *   * *   *
*       *   *      *****  *  ********  *   *          *   *****  *
*      *     *       *    *  ********  *   * * * * * *      *    *
*     * ***** *      *    *  ********  *   *          *     *    *
*    *         *          *            *   *           *         *
*   *           *         *            *   * * * * * *           *
*                         *            *                         *
* * * * * * * * * * * * * *            * * * * * * * * * * * * * *









* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                               *
*                     * * * * * *    *                          *
*                    *              * *                         *
*                    *             *****                        *
*                    *               *                          *
*                    *               *                          *
*                    *               *                          *
*                     * * * * * *                               *
*                                                               *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Number of people:  The total number of people in the current region is: 1400 ,  ,at 25,Jul,2019, 12:36:35,

======================== Data Analysis Finished =============================
There are 1400 people in the area.
========================= Data sent to the cloud ===========================

[at_send_cmd]131: at_out: "AT+CIPSEND=0,50
" (17)
[at_get_reply]154: "AT+CIPSEND=0,50

OK" (22)

============================= Send finished ================================


============================== Receive Data =================================


+IPD,0,19:A059020190725123640

============================ Receive Finished ===============================

========================== Start Data Analysis ==============================

============================= Number of people ================================

Number of people:  The number of people in area A is:0590.
Number of people:  The number of people in area B is:0540.
Number of people:  The number of people in area C is:0250.

=============================== Crowd status  =================================

Crowd status: The number of people in area A has decreased.
A0590201907251A:0590

============================= Stores the data =================================

 Data successfully saved!

=============================== Flow of crowd =================================

Flow of crowd: Crowds move from area A to area B and C.
* * * * * * * * * * * * * *            * * * * * * * * * * * * * *
*                         *      *     *                         *
*         *          *    *      **    *   * * * * * *      *    *
*        * *         *    *      ***   *   *           *   * *   *
*       *   *      *****  *  ********  *   *          *   *****  *
*      *     *      * *   *  ********* *   * * * * * *      *    *
*     * ***** *      *    *  ********  *   *          *     *    *
*    *         *          *      ***   *   *           *         *
*   *           *         *      **    *   * * * * * *           *
*                         *      *     *                         *
* * * * * * * * * * * * * *            * * * * * * * * * * * * * *

          * * *
          * * *
          * * *
       * ** * ** *
        * * * * *
          * * *
            *

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                               *
*                     * * * * * *    *                          *
*                    *              * *                         *
*                    *             *****                        *
*                    *               *                          *
*                    *               *                          *
*                    *               *                          *
*                     * * * * * *                               *
*                                                               *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

Number of people:  The total number of people in the current region is: 1380 ,  ,at 25,Jul,2019, 12:36:40,

======================== Data Analysis Finished =============================
There are 1380 people in the area.
========================= Data sent to the cloud ===========================

[at_send_cmd]131: at_out: "AT+CIPSEND=0,50
" (17)
[at_get_reply]154: "AT+CIPSEND=0,50

OK" (22)

============================= Send finished ================================

```

#### Function Module

* Data aggregation 

  ~~~C
   int data_process(char *buff)
  ~~~

  This program is divided into two parts: data aggregation and data analysis.

  The data aggregation part receives data sent by each node through wifi, extracts data such as region and time, classifies data, aggregates multi-node data.

  The data analysis part is based on the current data and previous data of the nodes, reports the changes of the number of nodes, analyses the trend of crowd flow, and predicts the trend of the crowd. 

* Abnormal Data Process

  ~~~C
  int abnormal_data_processing(float var, float ave)
  ~~~

  The program is based on the previous number information for data processing, according to the calculation results to determine whether the current data is abnormal, and eliminate abnormal data to ensure the authenticity of the data.

* Data storage

  ~~~C
  int data_storage(char *buff)
  ~~~

  The program saves the data information of the processed node in the local SD card to backup the data .

* Cloud-side Collaborative Processing and Continuous transmission of breakpoints

  This program sends the aggregated information to the cloud server for the distribution and scheduling of the cloud, and when the server fails to send the information to the cloud server due to network outage, data will be backed up on the local SD card to prevent data loss.

## Demo

[DEMO]( https://v.youku.com/v_show/id_XNDI4ODA4MTY0NA==.html?spm=a2h4v.8841035.uerCenter.5!3~5~5!2~DL~DD!2~A&firsttime=437.67)

