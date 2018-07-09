----
**Smart-Lamp**
====
----
# **Content**
* [Introduction](#introduction)
  * [Function](#function)
  * [System Architecture](#architecture)
* [System Config](#config)
  * [Hardware](#hardware)
  * [Software](#software)
  * [Hardware Connection](#connection)
* [User Manual](#usermanual)
* [Video](#video)
----
# **Introduction** <div id='introduction'/>
This project designed a smart lamp which was based on ARC EM Starter Kit. The lamp can detect whether the user is using to turn on or off, automatically connect the server to report the real-time status, and manually and automatically adjust the brightness, remotely be controled by the APP console, record the user's usage time. The follow-up will add the functions of detecting the unhealthy work schedule of the user and correcting the user's incorrect sitting habits, which can help the user to effectively get rid of the health hazards brought by the high pressure of modern life, develop a good working habit,  and keep your body and eyes healthy.<br>
## **Function** <div id='function'/>
* **automatic switch**<br>
When the human body is located at a distance of about 15cm from the infrared transceiver port and keeps the occlusion time for more than 3 seconds, the lamp can normally illuminate and enter the auto dimming mode by default. Similarly, the lamp will automatically turn off 3 seconds after the person leaves the lamp, avoiding the occasional behavioral instability of the user and causing the desk lamp to be frequently turned off.
* **automatic sign in**<br>
After restarting the lamp and the network, the lamp can automatically re-launch within 5 seconds.
* **automatic adjust brightness**<br>
The lamp can realize stepless natural dimming in 8 different light intensity ranges, and there is no phenomenon that the user feels uncomfortable due to the light intensity jump and stroboscopic. In the automatic dimming mode, the lamp can adapt to the change of light intensity in the environment, and adjust the brightness in time, with better sensitivity.
* **remote control**<br>
The APP can support the frequent operation of the user, and there is no problem of wrong operation or invalid operation under the high-intensity round-trip conversion test.
## **System Architecture** <div id='architecture'/>
<img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/system.jpg" width = "500" alt="图片名称" align=center /> <br>

----
# **System Config** <div id='config'/>
## **Hardware** <div id='hardware'/>
* EMSK V2.2 ARCEM7D Starter Kit
* ESP8266 WIFI
* Infrared Ray Sensor
* Touchpad
* CM3232 Ambient Light Sensor <br>
<img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/esp8266.jpg" height = "150" alt="图片名称" align=center /><img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/ir.jpg" height = "150" alt="图片名称" align=center /><img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/touchpad.jpg" height = "150" alt="图片名称" align=center /><img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/cm3232.jpg" height = "150" alt="图片名称" align=center />

## **Software** <div id='software'/>
* ARC GNU Toolchain
* embARC Open Software Platform (OSP) 201709
* Serial Port Terminal eg. XCOM
* APP for Android
* Mini Program of Wechat 
## **Hardware Connection** <div id='connection'/>
<img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/all.jpg" height = "500" alt="图片名称" align=center /><br>
* ESP8266 connected to UART1 through PMOD1 of ARC EMSK
* CM3232 connected to I2C through PMOD3 of ARC EMSK
* Touchpad connected to UART emulator through PMOD3 of ARC EMSK
* Infrared Ray Sensor connected to GPIO through PMOD2 of ARC EMSK

----
# **User Manual** <div id='usermanual'/>
* The user configures the current network for the lamp through the APP and connects to the server.
* In the automatic control mode, the lamp can enter the automatic dimming while the user is nearby, which will automatically adjust the brightness according to the ambient light intensity.
* In manual control mode, the user can manually adjust the brightness of the lamp through the touchpad button and exit the automatic mode.
* The user can remotely control via mobile client

----
# **[Video](https://v.youku.com/v_show/id_XMzcxMzE0NzA0NA==.html?spm=a2h3j.8428770.3416059.1) <div id='video'/>**
----
