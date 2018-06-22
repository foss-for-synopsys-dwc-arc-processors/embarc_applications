# AWS IoT Smarthome Application
This application is designed to show how to connect 3 EMSKs and [AWS IoT Cloud](https://aws.amazon.com/iot/?nc1=h_ls) using embARC. The connection between EMSK and AWS IoT Cloud is secured by TLS.

## Hardware and Software Setup
### Required Hardware
- 3 [DesignWare ARC EM Starter Kit(EMSK)](https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit)
- 3 [Digilent PMOD WiFi(MRF24WG0MA)](http://store.digilentinc.com/pmodwifi-wifi-interface-802-11g/)
- 1 [Digilent PMOD TMP2](http://store.digilentinc.com/pmod-tmp2-temperature-sensor/)
- 3 SD Card
- WiFi Hotspot(SSID:**embARC**, Password:**qazwsxedc**, WPA/WPA2 encypted)

### Required Software
- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom

### Hardware Connection
1. EMSK 1 simulate **frontdoor** node, it will publish frontdoor lock status and temperature sensor value to AWS IoT Cloud.
   - Connect PMOD WiFi to J5, connect PMOD TMP2 to J2.
2. EMSK 2 simulate **livingroom** node, it will publish livingroom lights status to AWS IoT Cloud.
   - Connect PMOD WiFi to J5.
2. EMSK 2 simulate **kitchen** node, it will publish kitchen lights status to AWS IoT Cloud.
   - Connect PMOD WiFi to J5.
3. Configure your EMSKs with proper core configuration.

## User Manual
### Before running this application
Similiar to the [AWS IoT Smarthome Application - Single Node](aws_iot_smarthome/README.md), you need to create 3 different things for different EMSK nodes and generate the certs for different nodes, and modify specific `aws_iot_config.h` for different nodes.

|  EMSK Simulated Node      |    AWS IoT Thing      |     Source Folder      |          Cert Folder                |
| ------------------------- | --------------------- | ---------------------- | ----------------------------------- |
|      frontdoor            |   frontdoor           | src/frontDoor          | src/cert/frontdoor                  |
|      kitchen              |   kitchen             | src/kitchen            | src/cert/kitchen                    |
|      livingroom           |   livingRoom          | src/livingRoom         | src/cert/livingRoom                 |

Copy `src/cert/` folder to root folder of the EMSK SD card, in SD card, the cert folder should be `cert/`.

The hardware resources are allocated as following table.

|  Hardware Resource  |            Represent                                          |
| ------------------- | ------------------------------------------------------------- |
|  BUTTON R           | Livingroom Lights Control                                     |
|  LED 0-1            | Livingroom Lights Status(On or Off)                           |
|  BUTTON L           | Kitchen Lights Control                                        |
|  LED 2-3            | Kitchen Lights Status(On or Off)                              |
|  BUTTON X           | Front Door Lock Control                                       |
|  LED 4-5            | Front Door Lock Status(On or Off)                             |
|  LED 7              | WiFi connection status(On for connected, Off for not)         |
|  LED X              | Node working status(toggling in 2s period if working well)    |
|  PMOD TMP2          | Temperature Sensor                                            |
|  PMOD WiFi          | Provide WiFi Connection                                       |

EMSK can send the status of the Room Temperature, FrontDoor Lock status, LivingRoom Light status and Kitchen Light status to the AWS IoT and interact with AWS IoT Cloud with using embARC.

Open [dashboard website](http://foss-for-synopsys-dwc-arc-processors.github.io/freeboard/), and load `dashboard-smarthome-multithings.json` dashboard configuration file in current folder, and then you can control and monitor this multiple nodes.

After loading this configuration file, you also need to click at the setting icon, and then click on the aws datasource, and then change the **AWS IOT ENDPOINT**, **REGION**, **ACCESS KEY**, **SECRET KEY** to your own aws ones.
![embARC AWS Smart Home Dashboard Setting - Step1](doc/screenshots/webapp_setting1.jpg)
![embARC AWS Smart Home Dashboard Setting - Step2](doc/screenshots/webapp_setting2.jpg)


### Run this application
Here take **EMSK2.2 - ARC EM7D** with Metaware Toolset for example to show how to run this application.

1. We need to use embARC 2nd bootloader to automatically load application binary for different EMSK and run.

2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.

3. Interact using EMSK and Dashboard.
  - You can press the **button L/R/X** on specific board to see the led changes on board and also on dashboard web app.
  - You can also click the lights of **DESIRED STATUS** pane on the dashboard app, and see the led changes on board and dashboard web app.
  ![AWS IoT Smarthome Multiple Nodes Web App](doc/screenshots/webapp.jpg)
