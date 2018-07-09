----
**Smart-Lamp**
====
----
# **目录**
* [作品介绍](#作品介绍)
  * [功能](#功能)
  * [系统结构](#系统结构)
* [系统配置](#系统配置)
  * [硬件需求](#硬件需求)
  * [软件需求](#软件需求)
  * [硬件连接](#硬件连接)
* [用户手册](#用户手册)
* [注释](#注释)  
* [视频链接](#视频链接)
----
# **作品介绍**
本项目设计了一种基于ARC EM Starter Kit开发板的智能台灯，能够检测用户是否在使用而开关台灯，自动连接服务器报告台灯的实时状态，可以手动和自动调节台灯的亮度，通过APP远程控制台灯，记录用户的使用时间。后续会加入检测用户不健康工作作息规律、矫正用户不正确坐姿习惯的功能，帮助用户有效避免现代生活高压下带来的健康隐患，在有一个舒适的工作环境的同时，养成一种良好的工作作息习惯，保持身体与眼部的健康。<br>
## **功能**
* **自动开关台灯**<br>
在人体位于红外收发口15cm左右距离，并保持3秒以上的遮挡时间时，台灯能正常亮起并默认进入自动调光模式。同样的，在人离开灯附近3秒后，台灯才会自动关闭，避免用户偶尔的行为不稳定导致台灯频繁亮灭。
* **自动连接服务器上线**<br>
在重启无线网络、重启后台服务器后，台灯均能在5s内重新自动上线。
* **自动调节台灯亮度**<br>
台灯能在8档不同光照强度范围内实现无级自然调光，没有光强跳变、频闪等令用户感到不舒服的现象发生。自动调光模式下，台灯能自适应环境中的光强变化，及时进行亮度调节，灵敏度较好。
* **远程APP控制**<br>
APP控制能够支持用户的频繁操作，在高强度的来回调档测试下也不会出现错误操作、无效操作等问题。
## **系统结构**
<img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/system.jpg" width = "500" alt="图片名称" align=center /> <br>

----
# **系统配置**
## **硬件需求**
* EMSK V2.2 ARCEM7D开发板
* ESP8266 WIFI模块
* 红外对管模块
* 触摸按键模块
* CM3232环境光检测模块<br>
<img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/esp8266.jpg" height = "150" alt="图片名称" align=center /><img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/ir.jpg" height = "150" alt="图片名称" align=center /><img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/touchpad.jpg" height = "150" alt="图片名称" align=center /><img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/cm3232.jpg" height = "150" alt="图片名称" align=center />

## **软件需求**
* ARC GNU Toolchain
* embARC Open Software Platform (OSP) 201709
* 串口调试助手
* 移动端应用程序
## **硬件连接**
<img src="https://github.com/tyhucosiii/smart-lamp/blob/master/pictures/all.jpg" height = "500" alt="图片名称" align=center /><br>
* ESP8266模块通过开发板的Pmod1接口与UART1连接
* CM3232感光模块通过开发板的Pmod3接口与I²C接口连接
* 触控板模块通过开发板的Pmod3接口与模拟UART连接
* 红外模块通过开发板的Pmod2接口与GPIO连接

----
# **用户手册**
* 用户通过移动客户端为台灯配置当前网络，连入服务器
* 自动控制模式下，用户靠近台灯就能进入自动调光，台灯会根据环境光强自动进行亮度调节
* 手动控制模式下，用户可通过触控板按键手动调节台灯亮度，并退出自动模式
* 可通过移动客户端进行远程控制
----

## **注释**
GitHub中的src和inc目录下的文件只用来预览代码，由于makefile的目录不一致不能直接下载运行。
如果想运行该工程，请下载[压缩包](https://github.com/tyhucosiii/smart-lamp/blob/master/src/FreeRTOS.rar),并解压到embarc_osp-master\example\freertos文件夹下载入工程。

----
# **[视频链接](http://v.youku.com/v_show/id_XMzYzMDk1MzMyOA==.html?spm=a2h3j.8428770.3416059.1)**
----
