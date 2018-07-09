#include "embARC.h"
#include "embARC_debug.h"

#include "dev_gpio.h"
#include "dev_uart.h"
#include "dev_iic.h"
#include "dev_spi.h"
#include "board.h"

#include "esp8266_gagent_common.h"

/*
函数功能：MCU向WiFi模组回复设备信息。
包括：通用串口协议版本号
	  业务协议版本号
	  硬件版本号
	  软件版本号
	  产品标识码（在机智云创建产品时分配的全网唯一标识码，从机智云获取）
	  设备属性
	  产品密匙（与产品标识码对应的产品密匙，从机智云获取）

|---------------|----------------|---------|----------------------------------------------------------------------|
|参数名         |类型            |方向     |说明                                                                  |
|---------------|----------------|---------|----------------------------------------------------------------------|
|esp8266_uart   |DEV_UART_PTR    |in       |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值        |
|---------------|----------------|---------|----------------------------------------------------------------------|
|product_key    |uint8_t         |in       |字符指针，传入机智云产品标识码                                         |
|---------------|----------------|---------|----------------------------------------------------------------------|
|product_secret |uint8_t         |in       |字符指针，传入机智云产品密钥                                          |
|---------------|----------------|---------|----------------------------------------------------------------------|
*/
void mcu2wifi_product_info(DEV_UART_PTR esp8266_uart, uint8_t *product_key, uint8_t *product_secret)
{
	uint8_t checksum=0;
	int i=0; //循环计数变量
	uint8_t send_data[] = {
		0xFF,0xFF, // 包头
		0x00,0x00, //包长度
		0x02,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags
		'0','0','0','0','0','0','0','4',      //通用串口协议版本号
		'0','0','0','0','0','0','0','2',      //业务协议版本号
		'0','0','0','0','0','0','0','0',      //硬件版本号
		'0','0','0','0','0','0','0','0',      //软件版本号


		'0','0','0','0','0','0','0','0',      //产品标识码
		'0','0','0','0','0','0','0','0',      
		'0','0','0','0','0','0','0','0',      
		'0','0','0','0','0','0','0','0',                  

		0x01,0x2C,      //可绑定失效时间,以秒为单位
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      //设备属性

		'0','0','0','0','0','0','0','0',      //产品密钥
		'0','0','0','0','0','0','0','0',      
		'0','0','0','0','0','0','0','0',      
		'0','0','0','0','0','0','0','0',
		0xEE      //校验和
	};	
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256; //计算包长度（命令至校验和），低位
	send_data[5] = sn;  //获取包序号
	for(i=0; i<32; i++)
	{
		send_data[40+i] = product_key[i];
		send_data[82+i] = product_secret[i];
	}
	for(i=0; i<32; i++) EMBARC_PRINTF("%c",send_data[40+i]); EMBARC_PRINTF("\r\n");
	for(i=0; i<32; i++) EMBARC_PRINTF("%c",send_data[82+i]); EMBARC_PRINTF("\r\n");	
	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i< sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//发送数据包
	esp8266_uart->uart_write(send_data,sizeof(send_data));
}


/*
函数功能：MCU向WiFi发送心跳包。
当WiFi模组超过55秒没有收到MCU的数据包，应向MCU发送心跳包。MCU收到心跳包后马上回复。
当WiFi模组连续3次没有收到MCU的心跳回复，进行报警。

|---------------|----------------|---------|----------------------------------------------------------------------|
|参数名         |类型            |方向     |说明                                                                  |
|---------------|----------------|---------|----------------------------------------------------------------------|
|esp8266_uart   |DEV_UART_PTR    |in       |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值        |
|---------------|----------------|---------|----------------------------------------------------------------------|
*/
void mcu2wifi_heartbeat(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x00,      //包长度
		0x08,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags		
		0x00      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;  //计算包长度（命令至校验和），低位
	send_data[5] = sn;	//获取包序号
	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}

/*
函数功能：MCU向WiFi发送指令，通知WiFi模组进入配置模式。
当WiFi模组收到进入配置模式的指令后，让设备进入对应的SoftAP或AirLink等OnBoarding配置方式。

|---------------|----------------|---------|----------------------------------------------------------------------|
|参数名         |类型            |方向     |说明                                                                  |
|---------------|----------------|---------|----------------------------------------------------------------------|
|esp8266_uart   |DEV_UART_PTR    |in       |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值        |
|---------------|----------------|---------|----------------------------------------------------------------------|
|mode           |uint8_t         |in       |配置模式，1为SoftAP方式，2为AirLink方式;                              |
|               |                |         |配置方式不合法时，默认进入AirLink配置方式，超时时间1分钟。            |
|               |                |         |softAp配置，超时时间5分钟                                             |
|---------------|----------------|---------|----------------------------------------------------------------------|
*/
void mcu2wifi_set_cfg_mode(DEV_UART_PTR esp8266_uart, uint8_t mode)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x05,      //包长度
		0x09,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags	
		0x00,     //
		0x00      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;  //计算包长度（命令至校验和），低位
	send_data[5] = ++sn;	//获取包序号
	/*设置配置方式，1为SoftAP方式，2为AirLink方式;
	配置方式不合法时，默认进入AirLink配置方式，超时时间1分钟。softAp配置，超时时间5分钟*/
	send_data[8] = mode;
	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}

/*
函数功能：MCU向WiFi发送指令，重置WiFi模组。
重置的内容包括WiFi模组保存的局域网WiFi SSID和密码,DID,Passcode等信息。
重置后模组重启进入AirLink配置模式,超时时间5分钟。

|---------------|----------------|---------|----------------------------------------------------------------------|
|参数名         |类型            |方向     |说明                                                                  |
|---------------|----------------|---------|----------------------------------------------------------------------|
|esp8266_uart   |DEV_UART_PTR    |in       |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值        |
|---------------|----------------|---------|----------------------------------------------------------------------|
*/
void mcu2wifi_reset_wifi(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x00,      //包长度
		0x0b,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags			
		0x00      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;  //计算包长度（命令至校验和），低位
	send_data[5] = ++sn;	//获取包序号	
	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}


/*
函数功能：WiFi模组向MCU推送WiFi状态后，MCU向WiFi回复信息。
同时打印WiFi状态，通过LED灯显示相应的状态。
|---------------|----------------|---------|----------------------------------------------------------------------|
|参数名         |类型            |方向     |说明                                                                  |
|---------------|----------------|---------|----------------------------------------------------------------------|
|esp8266_uart   |DEV_UART_PTR    |in       |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值        |
|---------------|----------------|---------|----------------------------------------------------------------------|
|receive_data   |uint8_t         |in       |uint8_t类型的指针，用来传入WiFi发送给MCU的数据                        |
|---------------|----------------|---------|----------------------------------------------------------------------|
*/
void mcu2wifi_wifi_statu(DEV_UART_PTR esp8266_uart,uint8_t *receive_data)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x05,      //包长度
		0x0E,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags
		0x00      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;
	send_data[3] = (sizeof(send_data)-0x04)%256;
	send_data[5] = sn;
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum+send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	esp8266_uart->uart_write(send_data,sizeof(send_data));

	uint8_t statu_bit0_7 = receive_data[9];
	uint8_t statu_bit8_15 = receive_data[8];

	if((statu_bit0_7)   %2 ) EMBARC_PRINTF("SoftAP mode is open\r\n");	
	if((statu_bit0_7>>1 )%2 ) EMBARC_PRINTF("Station mode is open\r\n");	
	if((statu_bit0_7>>2 )%2 ) EMBARC_PRINTF("OnBoarding mode is open\r\n");	
	//LED 5 显示WiFi模组绑定模式开启，设备与手机APP可以绑定；
	if((statu_bit0_7>>3 )%2 ) 	
	{
		EMBARC_PRINTF("Binding mode is open\r\n");
		led = gpio_get_dev(LED_PORT);
		led-> gpio_open(0x20);
		led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x20));
		led-> gpio_write(0x00,0x20);
	}
	else 
	{
		led = gpio_get_dev(LED_PORT);
		led-> gpio_open(0x20);
		led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x20));
		led-> gpio_write(0x20,0x20);
	}
	//LED 0~2 显示WiFi信号强度，3位2进制表示0~7，全灭表示WiFi信号强度最弱为0，全亮表示WiFi信号最强为7；
	if((statu_bit0_7>>4 )%2 ) 
	{
		EMBARC_PRINTF("The WiFi module is connected to the wireless router,the RSSI = %d\r\n",(statu_bit8_15)%8);	
		led = gpio_get_dev(LED_PORT);
		led-> gpio_open(0x07);
		led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x07));
		led-> gpio_write(~((statu_bit8_15)%8),0x07);
	}
	else 
	{
		led = gpio_get_dev(LED_PORT);
		led-> gpio_open(0x07);
		led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x07));
		led-> gpio_write(0x7,0x07);
	}
	//LED 3 显示WiFi模组是否连接到M2M服务器，亮表示连接到服务器，灭表示未连接至服务器；
	if((statu_bit0_7>>5 )%2 ) 
	{
		EMBARC_PRINTF("The WiFi module is connected to the M2M server\r\n");
		led = gpio_get_dev(LED_PORT);
		led-> gpio_open(0x08);
		led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x08));
		led-> gpio_write(0x00,0x08);

	}
	else
	{
		led = gpio_get_dev(LED_PORT);
		led-> gpio_open(0x08);
		led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x08));
		led-> gpio_write(0x08,0x08);
	}
	//LED 4 显示是否有APP在线；
	if((statu_bit8_15>>3)%2 ) 
	{
		EMBARC_PRINTF("APP online\r\n");
		led = gpio_get_dev(LED_PORT);
		led-> gpio_open(0x10);
		led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x10));
		led-> gpio_write(0x00,0x10);

	}
	else
	{
		led = gpio_get_dev(LED_PORT);
		led-> gpio_open(0x10);
		led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x10));
		led-> gpio_write(0x10,0x10);
	}

}

/*
函数功能：MCU回应WiFi模组对应包序号的数据包非法。
请求WiFi模组重新发送。

|---------------|----------------|---------|----------------------------------------------------------------------|
|参数名         |类型            |方向     |说明                                                                  |
|---------------|----------------|---------|----------------------------------------------------------------------|
|esp8266_uart   |DEV_UART_PTR    |in       |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值        |
|---------------|----------------|---------|----------------------------------------------------------------------|
*/
void mcu2wifi_receive_error(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x00,      //包长度
		0x12,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags	
		0x00, //错误码	
		0x00      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;  //计算包长度（命令至校验和），低位
	send_data[5] = sn;	//获取包序号
	send_data[8] = 2;
	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}

/* 
函数功能：MCU请求网络时间

|--------------|------------|---------|--------------------------------------------------------------|
|参数名        |类型        |方向     |说明                                                          |
|--------------|------------|---------|--------------------------------------------------------------|
|esp8266_uart  |DEV_UART_PTR|input    |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值|
|--------------|------------|---------|--------------------------------------------------------------|
*/
void mcu2wifi_request_time(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x05,      //包长度
		0x17,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags			
		0x00      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;  //计算包长度（命令至校验和），低位
	send_data[5] = ++sn;	//获取包序号	
	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}

/* 
函数功能：WiFi模组回复网络时间

|--------------|------------|---------|--------------------------------------------------------------|
|参数名        |类型        |方向     |说明                                                          |
|--------------|------------|---------|--------------------------------------------------------------|
|receive_data  |uint8_t     |in       |uint8_t类型的指针，用来传入WiFi发送给MCU的数据                |
|--------------|------------|---------|--------------------------------------------------------------|
*/
void wifi2mcu_get_time(uint8_t *receive_data)
{
	year = receive_data[8]*256+receive_data[9];
	month = receive_data[10];
	day  = receive_data[11];
	hour  = receive_data[12];
	minute  = receive_data[13];
	second  = receive_data[14];

	EMBARC_PRINTF("Now is %d/%d/%d  %d:%d:%d \r\n",year,month,day,hour,minute,second);
}

/* 
函数功能：mcu重启wifi模组

|--------------|-------------|---------|--------------------------------------------------------------|
|参数名        |类型         |方向     |说明                                                          |
|--------------|-------------|---------|--------------------------------------------------------------|
|esp8266_uart  |DEV_UART_PTR |input    |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值|
|--------------|-------------|---------|--------------------------------------------------------------|
*/
void mcu2wifi_restart_wifi(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x00,      //包长度
		0x29,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags			
		0x00      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;  //计算包长度（命令至校验和），低位
	send_data[5] = ++sn;	//获取包序号	
	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}

/* 
函数功能：MCU请求wifi模组进入可绑定模式，可绑定时间由获取设备信息 函数中的”可绑定状态失效时间“确定。

|--------------|------------|---------|--------------------------------------------------------------|
|参数名        |类型        |方向     |说明                                                          |
|--------------|------------|---------|--------------------------------------------------------------|
|esp8266_uart  |DEV_UART_PTR|input    |DEV_UART类型的指针，传入前必须先使用uart_get_dev()函数进行赋值|
|--------------|------------|---------|--------------------------------------------------------------|
*/
void mcu2wifi_request_into_binding(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //包头
		0x00,0x00,      //包长度
		0x15,      //命令
		0x00,      //包序号
		0x00,0x00,      //flags			
		0x00      //校验和
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //计算包长度（命令至校验和），高位
	send_data[3] = (sizeof(send_data)-0x04)%256;  //计算包长度（命令至校验和），低位
	send_data[5] = ++sn;	//获取包序号	
	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//发送数据包
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}