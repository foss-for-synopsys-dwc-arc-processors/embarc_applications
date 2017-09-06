# OpenThread Smarthome Application
This application is designed to show how to set up **OpenThread-CoAP-Based Smarthome Application** using embARC, the application layer of the application is built on top of the **CoAP protocol**.

All the Thread nodes interact with **Gateway** through the OpenThread **Network Co-Processor(NCP)**. The NCP can serve as a low-power wireless Network Co-Processor (NCP) to communicate with Userspace WPAN Network Daemon (wpantund) on Unix-like operating systems. It can be used along with OpenThread wpantund to provide Internet connectivity to the Thread network. See [embARC OpenThread NCP example][1] for more information.

The Gateway contains wpantund and Web UI, running on the Host with Unix-like operating systems, like the [Raspberry Pi 3][2] running recent [Raspbian Stretch Lite][3]. So the Gateway together with NCP have similar functionality as the **Border Router** available in the official OpenThread repository.

There are only two nodes available now, but you can add more nodes to OpenThread Smarthome easily by taking these two examples as a reference.

* [Introduction](#introduction)
	* [Function](#function)
	* [System Architecture](#system-architecture)
	* [Freeboard UI](#freeboard-ui)
* [Hardware and Software Setup](#hardware-and-software-setup)
	* [Required Hardware](#required-hardware)
	* [Required Software](#required-software)
	* [Hardware Connection](#hardware-connection)
* [User Manual](#user-manual)
	* [Before Running This Application](#before-running-this-application)
	* [Run This Application](#run-this-application)

## Introduction

There are two Thread Nodes in this application's Thread network:

- **FrontDoor** Node. It provides one resource:

	- Lock status (Use **LED0** to simulate the FrontDoor's Lock status). 

- **LivingRoom** Node. It provides two resources:

	- Light status (Use **LED0** to simulate the Light in the LivingRoom).
	- Temperature value.

### Function

- **FrontDoor** Node:

	- Control **lock status** via Gateway and UI.
	- Control **lock status**  by pressing the **button L** on the emsk, then it will send the value to Gateway.

- **LivingRoom** Node:

	- Control **light status** via Gateway and UI.
	- Control **light status** by pressing the **button L** on the emsk, then it will send the value to Gateway.
	- Turn ON/OFF the function of sending **temperature value** to Gateway every 5s by pressing the **button R**.

### System Architecture

![openthread_smarthome_application][30]

### Freeboard UI

- Current data can be accessed on the Freeboard UI.
- Control lock status and light status by clicking on the according components on the Freeboard UI.

![freeboard_ui][31]

## Hardware and Software Setup

### Required Hardware

- 3 x [DesignWare ARC EM Starter Kit(EMSK)][4]
- 3 x [Digilent PMOD RF2 (MRF24J40)][5]
- 1 x [Digilent PMOD TMP2][6]
- 3 x SD card
- 1 x microUSB-USB cable
- 1 x [Raspberry Pi 3][2] (running recent [Raspbian Stretch Lite][3])

### Required Software

- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom
- [OpenThread Smarthome Gateway][7]

### Hardware Connection

![hardware_connection][32]

1. EMSK 1 implement **FrontDoor** node.
	- Connect **PMOD RF2 (MRF24J40)** to **J6**.

2. EMSK 2 implement **LivingRoom** node.
	- Connect **PMOD RF2 (MRF24J40)** to **J6**.
	- Connect **PMOD TMP2** to **J2**.

3. EMSK 3 implement **NCP**.
	- Connect **PMOD RF2 (MRF24J40)** to **J6**.
	- Connect with the Raspberry Pi 3 (Gateway) via microUSB-USB cable.

4. Insert SD Card to EMSKs, make sure Bit 4 of the onboard DIP switch is ON to enable the secondary bootloader.

5. Configure your EMSKs with proper core configuration.

## User Manual
### Before Running This Application

Install [OpenThread Smarthome Gateway][7] on the Raspberry Pi 3. See [OpenThread Smarthome Gateway's README][7] for reference.

### Run This Application

Here take **EMSK2.3 - ARC EM7D** with GNU Toolset for application to show how to run this application.

- Program the secondary bootloader application into onboard SPI flash of EMSK.
- Generate boot.bin of the **frontDoor** Node, **livingRoom** Node and **NCP** using "make bin".
- Insert SD Card back to the EMSKs. Press the reset button to reboot it. Wait for loading boot.bin from SD card.
- See [embARC OSP **Bootloader** example][8] for more information.

> Notice
> 
> **Before making boot.bin for NCP**, it is necessary to modify the source file in [embARC OpenThread NCP example' main.c][9] as the following shows:
> 
> 		149    PlatformInit(argc, argv, NUM_NCP);
>
> It is for generating pseudo random number (by given a constant number: NUM_NCP here) for OpenThred automatically. Then, you don't need to input it manually.

1. Start **FrontDoor** Node, wait a few seconds for completing Thread configuration and create a Thread network.
   Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.
   Enter **ipaddr** to show the IP address of the Thread node. Here it is *fdde:ad00:beef:0:63a8:7376:c6ad:828c*.

2. Start **LivingRoom** Node, wait 2 minutes for completing Thread configuration and **join** the Thread network
   created by **FrontDoor** Node earlier. After that it will be a **router**.
   Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.
   Enter **ipaddr** to show the IP address of the Thread node. Here it is *fdde:ad00:beef:0:4f6e:7e53:67c8:f5b0*.
   
	It really doesn't matter which Thread Node to be the leader. It just depends on which node starts first.

	![start_thread_nodes][33]

3. Start **Gateway**, Connect the **NCP** to the Raspi via USB cable. Check the two Nodes' and Gateway's IPv6 address in the [`ot_smarthome_gw/gateway/config.js`][10]. Then, power the Raspi, the Gateway and NCP will be working automatically. See [OpenThread Smarthome Gateway's README][7] for more information and learn how to use the Gateway.

	> Notice
	> 
	> For checking the IPv6 address, find out and note the IPv6 address of virtual IPv6 network interface (named utun6 here) on the Raspi. It is **Gateway's IPv6 address** also. In general, this value is constant and don't need to change it, the same as here: *fdde:ad00:beef:0:3b33:8a2f:8e4:67d3*. But if not, modify the following macro definition in `./src/frontDoor/main.c` and `./src/livingRoom/main.c`:
	>
	>		#define GATEWAY_ADDR_USER ("INPUT_YOUR_GATEWAY_IPV6_ADDRESS")
	> Then, make boot.bin of the frontDoor and livingRoom Nodes again.

4. Start Freeboard UI, open the browser and enter the Raspi's IPv4 address and port to visit UI, like *192.168.137.116:8080*. Enjoy the interaction using EMSK and Freeboard UI !

	- On the frontDoor Node
	  - Press **Button L** to control the Lock and send its status to UI. LED0 shows the Lock status.
	- On the livingRoom Node
	  - Press **Button L** to control the Light and send its status to UI. LED0 shows the Light status.
	  - Press **Button R** to start/stop sending the **Temperature value** to UI every 5s. LED1 blinking when it reports data.
	- On the Freeboard UI
	  - Control **Lock status** and **Light status** by clicking on the according components.

> Notice

> Before restart Gateway, it is better to restart all Thread Nodes.

[1]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/tree/master/example/baremetal/openthread/ncp "embARC OpenThread NCP example"
[2]: https://www.raspberrypi.org/products/raspberry-pi-3-model-b/    "Raspberry Pi 3"
[3]: https://www.raspberrypi.org/downloads/raspbian/ "Raspbian Stretch Lite"
[4]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[5]: http://store.digilentinc.com/pmod-rf2-ieee-802-15-rf-transceiver/    "Digilent PMOD RF2 (MRF24J40)"
[6]: http://store.digilentinc.com/pmod-tmp2-temperature-sensor/    "Digilent PMOD TMP2"
[7]: https://github.com/XiangcaiHuang/ot_smarthome_gw    "OpenThread Smarthome Gateway"
[8]: http://embarc.org/embarc_osp/doc/embARC_Document/html/group___e_m_b_a_r_c___a_p_p___b_a_r_e_m_e_t_a_l___b_o_o_t_l_o_a_d_e_r.html    "embARC OSP **Bootloader** example"
[9]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp/blob/master/example/baremetal/openthread/ncp/main.c "embARC OpenThread NCP example' main.c"
[10]: https://github.com/XiangcaiHuang/ot_smarthome_gw/blob/master/gateway/config.js    "`ot_smarthome_gw/gateway/config.js`"

[30]: ./doc/screenshots/openthread_smarthome_application.png "openthread_smarthome_application"
[31]: ./doc/screenshots/freeboard_ui.png "freeboard_ui"
[32]: ./doc/screenshots/hardware_connection.jpg "hardware_connection"
[33]: ./doc/screenshots/start_thread_nodes.PNG "start_thread_nodes"
