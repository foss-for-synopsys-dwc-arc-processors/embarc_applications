# iBaby Smarthome Application
This application is designed to show how to connect 1 or more EMSKs and iBaby Smarthome Gateway using embARC. The connection between EMSK and Gateway is based on LwM2M protocol. All iBaby nodes indirectly interact with AWS IoT through the Gateway.

## Hardware and Software Setup
### Required Hardware
- 1 [DesignWare ARC EM Starter Kit(EMSK)](https://www.synopsys.com/dw/ipdir.php?ds=arc_em_starter_kit)
- 1 [Digilent PMOD WiFi(MRF24WG0MA)](http://store.digilentinc.com/pmodwifi-wifi-interface-802-11g/)
- 1 [Acceleration sensor MPU6050](http://store.digilentinc.com/pmod-tmp2-temperature-sensor/)
- 1 [Heartrate sensor MAX30102](http://store.digilentinc.com/pmod-tmp2-temperature-sensor/)
- 1 [Temperature sensor MLX90614](http://store.digilentinc.com/pmod-tmp2-temperature-sensor/)
- 1 SD Card
- WiFi Hotspot(SSID:**embARC**, Password:**qazwsxedc**, WPA/WPA2 encypted)

### Required Software
- Metaware or ARC GNU Toolset
- Serial port terminal, such as putty, tera-term or minicom
- iBaby Smarthome Gateway
- iBaby Freeboard UI

### Hardware Connection
1. EMSK 1 implement **wearable** node, as an intelligent foot ring for baby, it will publish baby's status to AWS IoT Cloud via the Gateway, including body temperature, heartrate, sleep-wake state, motion intensity and some warning information. We can view all data on the Freeboard UI.
   - Connect PMOD WiFi to J5, connect 3 sensor modules to J4.
2. Configure your EMSKs with proper core configuration.

## User Manual
### Before running this application
Firstly, you need a AWS account, and create several things for ibaby nodes, generate and save the certs for different nodes, and modify specific `config.js` for different nodes.

Secondly, run the iBaby Smarthome Gateway, open the browser and type IP address of the Gateway to access user interface.

|  EMSK Implemented Node   |    AWS IoT Thing      |     Source Folder      |
| ------------------------ | --------------------- | ---------------------- |
|      wearable node       |        ibaby          |   src/wearable_node    |

The hardware resources are allocated as following table.

|  Hardware Resource  |            Function                                           |
| ------------------- | ------------------------------------------------------------- |
|  MPU6050 module     | Acceleration sensor                                           |
|  MAX30102 module    | Heartrate sensor                                              |
|  MLX90614 module    | Body temperature sensopr                                      |
|  PMOD WiFi          | Provide WiFi Connection                                       |

### Run this application
Here take **EMSK2.2 - ARC EM7D** with Metaware Toolset for example to show how to run this application.

1. We need to use embARC 2nd bootloader to automatically load application binary for different EMSK and run.

2. Open your serial terminal such as Tera-Term on PC, and configure it to right COM port and *115200bps*.

3. Interact using EMSK and Freeboard.
