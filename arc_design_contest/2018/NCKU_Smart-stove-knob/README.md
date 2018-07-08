# Smart Stove Konb Application
We designed this product for **aging society**. Most of the old in Taiwan live alone because their children work in the city. some of them forget that they are cooking while watching TV, chating, etc. So we designed this smart stove knob to turn off the stove when detecting smoke or the user leave more than three minutes. We aim to reduce the harm from the stove.  
* [Introduction](#introduction)
  * [Function](#function)
  * [Algorithm](#algorithm)
  * [System Architecture](#system-architecture)
  * [Demo Video](https://v.youku.com/v_show/id_XMzYxMzEzODU1Ng==.html?spm=a2h3j.8428770.3416059.1)
* [Hardware and Software Setup](#hardware-and-software-setup)
  * [Required Hardware](#required-hardware)
  * [Required Software](#required-software)
  * [Hardware Connection](#hardware-connection)
  
## Introduction

**Smart Stove Konb** 
 
 Smart Stove Konb reduces the possibilty of the house firing induced by the stove.

### Function

- Turn off the stove when
  - Detecting smoke
  - The user leave more than three minutes
  - Times up when the user set when to turn off the stove(developing)
- Linking to the user's cellphone
  - Set when to turn off the stove in the App(developing) 
  - Check if the stove is on when leaving home(developing)
  ![app][1]

### Algorithm 
  ![algorithm][2]

### System Architecture
  ![architecture][3]
  
## Hardware and Software Setup
### Required Hardware
- 1 [DesignWare ARC EM Starter Kit(EMSK)][4]
- 2 [HM10 module][5] 
- 1 [Arduino UNO board][6]
- 1 [Digilent PMOD TMP2][7]
- 1 [MQ2 gas sensor][8]
- 1 [PIR motion sensor module:DYP-ME003][9]
- 1 [servo motor SG90][10]

The list of haraware is shown in the picture following.

### Required Software
- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom
- Arduino

### Hardware Connection
1. EMSK acts as **hub** in this system. It receives stove condition from node, and sends control signal to turn off the stove.
- Connect **HM10** to **J1**
2. Arduino acts as **node** in this system. It gets the condition around the stove with temperature sensor, motion sensor, and smoke sensor. The Arduino board concludes received data, and send the judgement to hub.
- Connect **HM10** to Arduino board as the table below

| HM10 | Arduino |
|------|---------|
| RXD  | D9      |
| TXD  | D8      |
| GND  | GND     |
| VDD  | 5V      |
- Connect **PMOD TMP2** to Arduino board as the table below

| TMP2 | Arduino |
|------|---------|
| SCL  | A5      |
| SDA  | A4      |
| GND  | GND     |
| VDD  | 5V      |
- Connect **MQ2 gas sensor** to Arduino board as the table below
- Connect **motion sensor** to Arduino board as the table below

| motion sensor | Arduino |
|---------------|---------|
| OUT           | D5      |
| GND           | GND     |
| VDD           | 5V      |
- Connect **servo motor** to Arduino board as the table below

| servo motor            | Arduino |
|------------------------|---------|
| PWM input(orange line) | D3      |
| GND(brown line)        | GND     |
| VDD(red line)          | 5V      |



[1]:./doc/pic/app.png "app"
[2]:./doc/pic/algorithm.png "algorithm"
[3]:./doc/pic/system_architecture.png "system_architecture"

[4]:https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[5]:https://goods.ruten.com.tw/item/show?21726620422161 "Ble HM10 module"
[6]:https://goods.ruten.com.tw/item/show?21404239751648
[7]:https://store.digilentinc.com/pmod-tmp2-temperature-sensor/
[8]:https://goods.ruten.com.tw/item/show?21209142783548
[9]:https://goods.ruten.com.tw/item/show?21210114921071
[10]:https://goods.ruten.com.tw/item/show?21211166622336

