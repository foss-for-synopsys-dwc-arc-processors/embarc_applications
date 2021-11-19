
Please use UTF-8 open this file
 
use ARC IoTDK develpment kit


设备名称	            通信协议	    ARC IoT接口	                       实现功能            头文件
HC-05蓝牙模块	        UART	    DFSS_UART_2_ID(uart2)	            蓝牙数据传输        "HC_05.h"
DHT11温湿度传感器	    GPIO	    DFSS_GPIO_8B0_ID(B7:gpio8b_0[4])	环境温湿度信息采集  "dht11.h"
LD3320A语音识别模块	    UART	    DFSS_UART_1_ID (B3:uart1_rxd)	    语音控制信息识别    "LD3320a.h"
SYN6288语音合成模块	    UART	    DFSS_UART_1_ID   (B2:uart1_txd)	    语音信息播报        "SYN6288.h"
MPU9250九轴传感器	    IIC	        On-board	                        运动九轴信息采集    "mpu9250.h"
PulseSenso心率传感器	ADC	        J1:ADC0/gpio8b_3[7]	                运动心率采集        "PluseSensor.h"

注意：
1 ARC IoTDK的UART2端口的使用，需要在编译前的板子设置文件“mux.c” 的文件中设置 sysconf_reg_ptr->ARDUINO_MUX = 1
2 请使用GBK编码打开 "main.c"