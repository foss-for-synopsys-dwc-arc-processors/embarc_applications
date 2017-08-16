# OpenThread Smarthome Application
This application is designed to show how to set up **OpenThread-CoAP-Based Smarthome Application** using embARC, the application layer of the application is built on top of the **CoAP protocol**.

All the Thread nodes interact with **Gateway** through the OpenThread **Network Co-Processor(NCP)**. The Gateway has similar functionality as the **Border Router** available in the official OpenThread repository, but it is a more flexible solution. There are only 2 nodes available now, but you can add more nodes easily to OpenThread Smarthome by taking these 2 examples as a reference.

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

There are 2 Thread Nodes in this application:

- **FrontDoor** Node. It provides one resource:

	- lock status (Use LED0 to simulate the FrontDoor's Lock status). 

- **LivingRoom** Node. It provides two resources:

	- light status (Use LED0 to simulate the Light in the LivingRoom).
	- temperature sensor value.

### Function

- **FrontDoor** Node:

	- Receive CoAP PUT request (lock_sta).
	- Send CoAP PUT request (lock_sta) when the button L on EMSK is pressed.

- **LivingRoom** Node:

	- Receive CoAP PUT request (light_sta).
	- Send CoAP PUT request (lock_sta) when the button L on EMSK is pressed.
	- Send CoAP PUT request (temp) when the button R on EMSK is pressed.

### System Architecture

![openthread_smarthome_application][1]

### Freeboard UI

- Current data accessed via Freeboard UI.
- Control lock status and light status via clicking the light components on Freeboard UI.

![freeboard_ui][2]

## Hardware and Software Setup
### Required Hardware

- 3 x [DesignWare ARC EM Starter Kit(EMSK)][30]
- 3 x [Digilent PMOD RF2 (MRF24J40)][31]
- 1 x [Digilent PMOD TMP2][32]
- 3 x SD card
- 1 x microUSB-USB cable
- 1 x [Raspberry Pi 3][33] or PC for running [OpenThread Smarthome Gateway and UI][34] and [OpenThread wpantund][35]

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
	- Connect **microUSB** to the Raspberry Pi 3 through **USB**.

4. Insert SD Card to EMSKs, make sure Bit 4 of the onboard DIP switch is ON to enable the secondary bootloader.

5. Configure your EMSKs with proper core configuration.

## User Manual
### Before Running This Application

Install [OpenThread Smarthome Gateway and UI][34] and [OpenThread wpantund][35] on the Raspberry Pi 3.
   See [OpenThread Smarthome Gateway and UI' README][34] for reference.

### Run This Application

Here take **EMSK2.2 - ARC EM11D** with GNU Toolset for application to show how to run this application.

- Program the secondary bootloader application into onboard SPI flash of EMSK.
- Generate boot.bin of the Openthread CoAP application using "make bin".
- Insert SD Card back to the EMSKs. Press the reset button to reboot it. Wait for loading boot.bin from SD card.
- See **Bootloader** example in the [embARC OSP][37] for detailed information.

1. Start **FrontDoor** Node, wait 20 seconds for completing Thread configuration and create a Thread network.
   Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.
   Enter "**ipaddr**" to show the IP address of the Thread node. Such as *fdde:ad00:beef:0:da5:79f6:2a33:5c9c*.

2. Start **LivingRoom** Node, wait 20 seconds for completing Thread configuration and **join** the primary Thread network
   created by **FrontDoor** Node automatically.
   Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.
   Enter "**ipaddr**" to show the IP address of the Thread node. Such as *fdde:ad00:beef:0:69be:72ea:57d4:e9c2*.

3. Start **NCP**. Connect it to the Gateway through USB.

Modify the Thread Nodes' and Gateway's IPv6 address in the [`ot_smarthome_gw/gateway/config.js`][36]. Then, Run the Gateway.
**Join** the Thread network via **NCP wpanctl**. After that, you can visit **Freeboard UI**.

See [OpenThread Smarthome Gateway and UI][35] for more infomation about how to interact using EMSK and Freeboard.

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