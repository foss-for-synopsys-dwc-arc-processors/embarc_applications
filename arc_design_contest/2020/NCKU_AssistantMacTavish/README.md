Assistant Mactavish
===
This application is for people who need a offline inteligent home assistant which can prevent from being recorded by authorities, and still have the function of command recognition. With command recognition, Assistant Mactavish can controll subdevices through BLE and reach to air conditioner and switches. Moreover, these subdevices can be controlled through Android app as well, through wifi connection the Arc IoTDK have and communicating to cellphone through HTTP and MQTT.

- [Introduction](#introduction)
  - [Function](#function)
  - [Architecture](#architecture)
  - [App UI](#appui)
  - [Voice UI](#voiceui)
- [HW/SW Setup](#hwswsetup)
  - [Required Hardware](#requiredhw)
  - [Required Software](#requiredsw)
  - [Hardware Connection](#hwconnection)
- [User Manual](#usermanual)
  - [Prerequisite](#prerequisite)
  - [Run the System](#run)
  - [Demo Video](#demovid)


## Introduction <a name="introduction"></a>

### Function <a name="function"></a>
1. Voice recognition on edge with DSpotter model.
2. The central control can only be woken by "麥克塔維奇", and shoul be followed by command containing the combinations of "開/關", "客廳/書房/大門/玄關/廚房/餐廳", "燈/冷氣". E.g.: 麥克塔維奇, 開客廳的燈.
3. Command would be decoded and sent to specific subsystem on BLE with HM10.
4. Command can also be sent from smart phone to server and reach IoTDK to do corresponding actions.

### Architecture <a name="architecture"></a>
![architecture.png](https://imgur.com/sgek2ye.png)
1. Arc IoTDK is the central controll in this system and connected to peripherals and cellphone through BLE and Wifi.
2. Through Wifi and MQTT protocol, IoTDK can communicate with server, and the same for smartphone. Thus, smartphone can pass command to IoTDK through server.
3. Through BLE, IoTDK can send command to subdevices, and therefore controll switches connected to lights and IR encoders.

### APP UI <a name="appui"></a>
![appmain.png](https://i.imgur.com/eozl1c0_d.webp)
**Starting Page**
![appregiotdk](https://i.imgur.com/YQyoilF_d.webp)
**Search for Central Controll**
By clicking the "IOTDK連線" button, this page would show up.
![appregiotdk](https://i.imgur.com/fylZYq5_d.webp)
**Send Wifi SSID and PSWD to Central Controll**
This is a demo page of connect the central control to wifi, which would connect to central control with BLE first, and send SSID/PSWD to central control to allow central control have connection to Wifi AP.
![appregdevicemain](https://i.imgur.com/85wCH8Z_d.webp)
**Register Subdevices**
By clicking the "新裝置" button, this page would show up.
![appfilloupdevice](https://i.imgur.com/1dMGMzo_d.webp)
**Fill up Required Infomation about the Subdevice**
We use a dummy mac address here.
![appconfirmregdevice](https://i.imgur.com/f6bAe9l_d.webp)
**Confirm Register Subdevice**
![appcompleteadddevice](https://i.imgur.com/hcIItmP_d.webp)
**Complete Adding Subdevice**
![appregdevice2](https://i.imgur.com/l9EKYqM_d.webp)
**Register Another Subdevice as AC**
We choose AC controll this time different from the light controll last time.
![appconfirmregdevice2](https://i.imgur.com/0sQbucy_d.webp)
**Confirm Register Subdevice**
![appcompleteadddevice](https://i.imgur.com/bxTf0ih_d.webp)
**Complete Adding Subdevice**
![appactionAC](https://i.imgur.com/BW4bqkb_d.webp)
**Actions on Air Conditioner Subdevices**
By long pressing the registered devices, this menu would show up for user to choose which action they want to do to the AC subdevice. We can setup IR code by pressing the first and second button, and delete this registered device by third button.
![appactionLight](https://i.imgur.com/iZiBlNy_d.webp)
**Actions on Light Subdevices**
By long pressing the registered devices, user can choose to delete this light controlling subdevice.

### Voice UI <a name="voiceui"></a>
Due to the voice controll feature of this system, user interface needs to handle the situation that user controll subdevices by voice. Thus, the central controll would respond users' command with DFPlayer Mini by playing recorded MP3 files. The responding logic is as below flowgraph.
![VoiceUI](https://i.imgur.com/F9Pzd47.png)

## HW/SW Setup <a name="hwswsetup"></a>
### Required Hardware <a name="requiredhw"></a>
1. HM10 BLE Module![](https://i.imgur.com/us9o9eg.png)
2. Wemos D1 Mini![](https://i.imgur.com/2wLLV3v.png)
3. DFPlayer Mini![](https://i.imgur.com/jeAXYT0.png)
4. INMP441 MEM Microphone![](https://i.imgur.com/I8RqHEK.png)
5. Arc IoTDK![](https://i.imgur.com/TD2maMZ.png)
6. (Optional for Light Subdevice) Arduino UNO R3
7. (Optional for Light Subdevice) SG90 Servo
8. (Optional for AC Subdevice) Arduino Mega 2560 Rev3
9. (Optional for ACSubdevice) IR LED/Receiver

### Required Software <a name="requriedsw"></a>
1. ARC Metaware
2. Arduino IDE

### Hardware Connection <a name="hwconnection"></a>
1. HM10 BLE Module Connected to UART2 on IoTDK
  This HM10 module has been modified to connect the reset pin through hard wire.
  - HM10.VCC - IoTDK.Arduino.+5V      //VCC
  - HM10.GND - IoTDK.Arduino.GND      //GND
  - HM10.TX  - IoTDK.Arduino.IO0      //TX-RX
  - HM10.RX  - IoTDK.Arduino.IO1      //RX-TX
  - HM10.RST - IoTDK.GPIO8b_1[4]      //RST

2. Wemos D1 Mini Connected to UART1 on IoTDK
  - WemosD1.TX  - IoTDK.PMODC3        //TX-RX
  - WemosD1.RX  - IoTDK.PMODC2        //RX-TX
  - WemosD1.3V3 - IoTDK.PMODC12       //VCC
  - WemosD1.GND - IoTDK.PMODC5        //GND

3. DFPlayer Mini Connceted to Software Serial on Wemo D1 Mini
  - DFPlayer.VCC - IoTDK.Arduino.+5V  //5V
  - DFPlayer.GND - IoTDK.Arduino.GND  //GND
  - DFPlayer.RX  - WemosD1.D6         //RX-TX
  - DFPlayer.TX  - WemosD1.D5         //TX-RX
  - DFPlayer.SPK1- Speaker1           
  - DFPlayer.SPK2- Speaker2

4. INMP441 Connected to I2S on IoTDK
  - INMP441.SCK - IoTDK.I2S_RX_SLCK   //SLCK
  - INMP441.WS  - IoTDK.I2S_RX_WS     //WS
  - INMP441.SD  - IoTDK.I2S_RX_SDI    //SD
  - INMP441.LR  - IoTDK.GND           //LR
  - INMP441.GND - IoTDK.GND           //GND
  - INMP441.VCC - IoTDK.3V3           //VCC

## User Manual <a name="usermanual"></a>

### Prerequisite <a name="prerequisite"></a>
1. Download source code(this repo) of Assistant Mactavish for Arc IoTDK.
2. Download [source code for Wemos D1 Mini](https://github.com/ParkerMactavish/WemosD1withMQTT) and burn it into Wemos D1 mini or ESP8266 with pinout listed above.
3. Download [source code for Android APP](https://github.com/ParkerMactavish/SwitchBot_AndroidApp) and install it into Android phone or emulator.
4. Download [source code for subdevice](https://github.com/ParkerMactavish/Subdevices) and install `/IRRemote` to the subdevices controlling AC and install `/switchBot` to the subdevices controlling light with servo.

### Run the System <a name="run"></a>
1. Type `gmake run` and the central controll will boot.
2. Follow the userflow below and you have a built-up smart home assistant.
![](https://i.imgur.com/6U7cBgo_d.webp)

## Demo Video Link <a name="demovid"></a>
Link is still on audit