# **Contactless delivery robot**

This application is designed to show how to develop an **Contactless Delivery Robot** using embARC. The couriers first enter the customer's delivery address and mobile phone number into the chip through the keyboard, and at the same time select the robot delivery route; after the robot receives it, it will follow the prescribed route to run with obstacle avoidance and deliver the goods to the customer. After arriving at the destination, it will send a text message to the customer’s phone to inform the pickup password; the customer opens the storage box through the received pickup password and takes out the goods.

* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#system-architecture)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## **Introduction**

- **Contactless delivery robot**



### **Function**

- **Human body inducing** (It will activate the system through the induction of human body )

![sr501](C:\Users\86159\Documents\GitHub\embarc_applications\arc_design_contest\2020\XDU_Contactless-distribution-robot\doc\sr501.jpg)

- **Ultrasonic ranging and obstacle avoiding**

![untrasonic](C:\Users\86159\Documents\GitHub\embarc_applications\arc_design_contest\2020\XDU_Contactless-distribution-robot\doc\untrasonic.jpg)

- **Speech interaction** (Remind the users of the operating process)

![syn6288](C:\Users\86159\Documents\GitHub\embarc_applications\arc_design_contest\2020\XDU_Contactless-distribution-robot\doc\syn6288.jpg)

- **Communication** (This robot will send text message to the custom’s phone and call the cus![sim900a](C:\Users\86159\Documents\GitHub\embarc_applications\arc_design_contest\2020\XDU_Contactless-distribution-robot\doc\sim900a.jpg)toms, providing their pickup great convenience )

![keypad](C:\Users\86159\Documents\GitHub\embarc_applications\arc_design_contest\2020\XDU_Contactless-distribution-robot\doc\keypad.jpg)

### **System Architecture**

![Architecture](C:\Users\86159\Documents\GitHub\embarc_applications\arc_design_contest\2020\XDU_Contactless-distribution-robot\doc\Architecture.png)

![whole](C:\Users\86159\Documents\GitHub\embarc_applications\arc_design_contest\2020\XDU_Contactless-distribution-robot\doc\whole.jpg)

## **Hardware and Software Setup**

### **Required Hardware**

- 1 DesignWare ARC EM Starter Kit(EMSK)

- 1 Ultrasonic Sensor Module (US-100)

- 1 Human Infrared Sensor (SR501)

- 1 Speech Synthesis Module(SYN6288)

- 1 4*4 Keyboard

- 1 GPRS Module(SIM900A)

- 2 Motor Driver Module(L298N)

- 4 Servo Driver Module(PCA9685)

- 4 Wheel

- 4 Motor

- 1 SD Card 

  The list of hardware is shown in the picture following.

  ### **Required Software**

  - Metaware or ARC GNU Toolset

  - Serial port terminal, such as putty, tera-term or minicom
    

### **Hardware Connection**

1. The **contactless delivery robot** will process the data returned by the keyboard and ultrasonic sensor, and send instructions to motor driver module and servo driver module. It can also send data to the     communication module and send text message.

2. Connect **Servo driver module** to **PMOD2[6:3]** (Using IIC0 interface), connect **Motor driver module** to **PMOD6[4:1]** and **PMOD6[10:7]** (Using GPIO interface), connect **Ultrasonic sensor** to **PMOD5[2:1]** (Using GPIO interface).

   Connect **Human infrared sensor** to **PMOD[1:7]** (Using GPIO interface), connect **GPRS **to **PMOD1[3:2]** (Using UART0 interface ), connect **Keyboard** to **PMOD3[4:1]** and **PMOD3[10:7]** (using GPIO interface), connect **Speech Synthesis Module** to **PMOD5[9:8]** (using UART2 interface).

   | **ARC** **interface**           | **Device** | **Links of style**                         |
   | ------------------------------- | ---------- | ------------------------------------------ |
   | **PMOD1[3:2]**                  | SIM900a    | UART0                                      |
   | **PMOD1[7]**                    | SR501      | GPIO（PORTA[8]）                           |
   | **PMOD2[6:3]**                  | PCA9685    | I2C0                                       |
   | **PMOD3[4:1]**  **PMOD3[10:7]** | 4*4 keypad | GPIO（PORTC[19:16]）  GPIO（PORTA[19:16]） |
   | **PMOD5[2:1]**                  | US-100     | GPIO（PORTC[25:24]）                       |
   | **PMOD5[9:8]**                  | SYN6288    | UART2                                      |
   | **PMOD6[4:1]**  **PMOD6[10:7]** | L298N      | GPIO（PORTC[31:28]）  GPIO（PORTA[31:28]） |

3. Configure your EMSKs with proper core configuration.



## **User Manual**

### **Before Running This Application**

Download source code of **Contactless delivery robot** from github.

In order to open **UART2**，**IIC0**，**UART0**， we need to modify following files.

```
set_pmod_mux(mux_regs, PM1_UR_UART_0 | PM1_LR_GPIO_A| PM2_I2C_HRI\
				| PM3_GPIO_AC\
				| PM4_I2C_GPIO_D\
				| PM5_UR_GPIO_C | PM5_LR_SPI_M2	\
				| PM6_UR_GPIO_C | PM6_LR_GPIO_A 
				);
)
```

The functions corresponding to each module driver code are as follows.

| **file**     | **function**                                                 |
| ------------ | ------------------------------------------------------------ |
| keypad.c     | Keyboard  initialization and information transcoding storage |
| motordrive.c | Motor  initialization and status control                     |
| pwm.c        | Servo  initialization and state control                      |
| sim900.c     | SMS  sending and dialing configuration                       |
| sr501.c      | Human  sensor initialization and detection                   |
| syn6288.c    | Speech  synthesis output                                     |
| tdriver.c    | IIC  initialization and configuration                        |
| ultradrive.c | Ultrasonic  module initialization and detection              |
| main.c       | Main  entry of embARC Application                            |
| makefile     | Makefile  of embARC Application                              |

## Run This Application

###  makefile

Target options about EMSK and toolchain:

```
# Current Board And Core
##
BOARD ?= emsk
BD_VER ?= 22
CUR_CORE ?= arcem7d
TOOLCHAIN ?= gnu
##

```

Then `make run`.

## **DemoVideo**

https://v.youku.com/v_show/id_XNDkwODM0MzUyNA==.html?firsttime=65