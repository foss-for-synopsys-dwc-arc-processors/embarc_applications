# OpenThread Smarthome Application
This application is designed to show how to set up **OpenThread-CoAP-Based Smarthome Application** using embARC, the application layer of the application is built on top of the **CoAP protocol**.

All the Thread nodes interact with **Gateway** through the OpenThread **Network Co-Processor(NCP)**. The NCP can serve as a low-power wireless Network Co-Processor (NCP) to communicate with Userspace WPAN Network Daemon (wpantund) on Unix-like operating systems. It can be used along with OpenThread wpantund to provide Internet connectivity to the Thread network. See OpenThread NCP example's README.md for more information.

The Gateway contains wpantund, running on the Linux Host with Unix-like operating systems, like the [Raspberry Pi 3][33] with [Raspbian Stretch Lite][38]. So the Gateway has similar functionality as the **Border Router** available in the official OpenThread repository, but it is a more flexible solution.

There are only two nodes available now, but you can add more nodes easily to OpenThread Smarthome by taking these two examples as a reference.

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

There are two Thread Nodes in this application:

- **FrontDoor** Node. It provides one resource:

	- lock status (Use **LED0** to simulate the FrontDoor's Lock status). 

- **LivingRoom** Node. It provides two resources:

	- light status (Use **LED0** to simulate the Light in the LivingRoom).
	- temperature value.

### Function

- **FrontDoor** Node:

	- Control **lock status** via Gateway and UI based on CoAP.
	- Control **lock status**  by pressing the **button L** on emsk, and send it to Gateway.

- **LivingRoom** Node:

	- Control **light status** via Gateway and UI based on CoAP.
	- Control **light status** by pressing the **button L** on emsk, and send it to Gateway.
	- Enable/Disable the function that send **temperature value** to Gateway every 5s by pressing the **button R**.

### System Architecture

![openthread_smarthome_application][1]

### Freeboard UI

- Current data is accessed on the Freeboard UI.
- Control lock status and light status by clicking on the light components on Freeboard UI.

![freeboard_ui][2]

## Hardware and Software Setup
### Required Hardware

- 3 x [DesignWare ARC EM Starter Kit(EMSK)][30]
- 3 x [Digilent PMOD RF2 (MRF24J40)][31]
- 1 x [Digilent PMOD TMP2][32]
- 3 x SD card
- 1 x microUSB-USB cable
- 1 x [Raspberry Pi 3][33] or other Linux Host for running [OpenThread Smarthome Gateway and UI][34] and [OpenThread wpantund][35]

### Required Software

- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom
- [OpenThread Smarthome Gateway and UI][34]
- [OpenThread wpantund][35]

### Hardware Connection

1. EMSK 1 implement **FrontDoor** node.
	- Connect **PMOD RF2 (MRF24J40)** to **J6**.

2. EMSK 2 implement **LivingRoom** node.
	- Connect **PMOD RF2 (MRF24J40)** to **J6**.
	- Connect **PMOD TMP2** to **J2**.

3. EMSK 3 implement **NCP**.
	- Connect **PMOD RF2 (MRF24J40)** to **J6**.
	- Connect **microUSB** interface to the **USB** interface on the Raspberry Pi 3 (Gateway).

4. Insert SD Card to EMSKs, make sure Bit 4 of the onboard DIP switch is ON to enable the secondary bootloader.

5. Configure your EMSKs with proper core configuration.

## User Manual
### Before Running This Application

Install [OpenThread Smarthome Gateway and UI][34] and [OpenThread wpantund][35] on the Raspberry Pi 3.
   See [OpenThread Smarthome Gateway and UI' README][34] for reference and [Simulation Codelab][39] for more information.

### Run This Application

Here take **EMSK2.3 - ARC EM7D** with GNU Toolset for application to show how to run this application.

- Program the secondary bootloader application into onboard SPI flash of EMSK.
- Generate boot.bin of the Openthread CoAP application using "make bin".
- Insert SD Card back to the EMSKs. Press the reset button to reboot it. Wait for loading boot.bin from SD card.
- See **Bootloader** example in the [embARC OSP][37] for detailed information.

1. Start **FrontDoor** Node, wait a few seconds for completing Thread configuration and create a Thread network.
   Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.
   Enter **ipaddr** to show the IP address of the Thread node. Such as *fdde:ad00:beef:0:63a8:7376:c6ad:828c*.

2. Start **LivingRoom** Node, wait 2 minutes for completing Thread configuration and **join** the Thread network
   created by **FrontDoor** Node earlier. After that it will be a **router**, not the **child**.
   Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.
   Enter **ipaddr** to show the IP address of the Thread node. Such as *fdde:ad00:beef:0:4f6e:7e53:67c8:f5b0*.
   It really doesn't matter the FrontDoor Node or the LivingRoom Node to be the leader. It just depends on which node started first.

3. Start **NCP**. Connect it to the Raspi (Gateway) via USB cable.
   Start the **wpantund** on the Raspi (Gateway), then start the **wpanctl** and enter the Command Line.
   Enter the following commands in the wpanctl to **join** the Thread network existed:

		wpanctl:utun6> scan
		wpanctl:utun6> set Network:Key --data masterkey
		wpanctl:utun6> join 1

   Find out and note the utun6's (virtual IPv6 network interface) IPv6 address. It is just the **Gateway's IPv6 address** also. Here it is *fdde:ad00:beef:0:3b33:8a2f:8e4:67d3*.

   See `\embARC\example\baremetal\openthread\ncp\README.md` for reference.

Modify the two Nodes' and Gateway's IPv6 address in the [`ot_smarthome_gw/gateway/config.js`][36]. Then, Run the Gateway. After that, you can visit **Freeboard UI** in the browser.

See [OpenThread Smarthome Gateway and UI][34] for more infomation about how to interact using EMSK and Freeboard UI.

[1]: ./doc/screenshots/openthread_smarthome_application.png "openthread_smarthome_application"
[2]: ./doc/screenshots/freeboard_ui.png "freeboard_ui"

[30]: https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit    "DesignWare ARC EM Starter Kit(EMSK)"
[31]: http://store.digilentinc.com/pmod-rf2-ieee-802-15-rf-transceiver/    "Digilent PMOD RF2 (MRF24J40)"
[32]: http://store.digilentinc.com/pmod-tmp2-temperature-sensor/    "Digilent PMOD TMP2"
[33]: https://www.raspberrypi.org/products/raspberry-pi-3-model-b/    "Raspberry Pi 3"
[34]: https://github.com/XiangcaiHuang/ot_smarthome_gw    "OpenThread Smarthome Gateway and UI"
[35]: https://github.com/openthread/wpantund    "OpenThread wpantund"
[36]: https://github.com/XiangcaiHuang/ot_smarthome_gw/blob/master/gateway/config.js    "`ot_smarthome_gw/gateway/config.js`"
[37]: https://github.com/foss-for-synopsys-dwc-arc-processors/embarc_osp    "embARC OSP"
[38]: https://www.raspberrypi.org/downloads/raspbian/ "Raspbian Stretch Lite"
[39]: https://codelabs.developers.google.com/codelabs/openthread-simulation/index.html "Simulation Codelab"