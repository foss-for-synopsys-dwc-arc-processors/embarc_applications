
uint8_t 				sn = 0;

/*
函数功能：MCU向WiFi模组回复设备信息。
包括：通用串口协议版本号
		业务协议版本号
		硬件版本号
		软件版本号
		产品标识码（在机智云创建产品时分配的全网唯一标识码，从机智云获取）
		设备属性
		产品密匙（与产品标识码对应的产品密匙，从机智云获取）

*/
void mcu2wifi_product_info (void)
{
	uint8_t 				checksum = 0;
	uint8_t 				send_data[0x73] =
		{
			0xFF, 0xFF, 																	// 包头
			0x00, 0x6F, 																	//包长度
			0x02, 																				//命令
			0x00, 																				//包序号
			0x00, 0x00, 																	//flags
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, //通用串口协议版本号
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, //业务协议版本号
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x36, //硬件版本号
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x35, //软件版本号
			0x61, 0x31, 0x30, 0x36, 0x37, 0x38, 0x35, 0x66, //产品标识码
			0x62, 0x61, 0x39, 0x65, 0x34, 0x35, 0x39, 0x30, 
			0x62, 0x39, 0x36, 0x63, 0x63, 0x65, 0x35, 0x31, 
			0x62, 0x37, 0x63, 0x65, 0x35, 0x36, 0x63, 0x39, 
			0x00, 0x00, 																	//可绑定失效时间
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, //设备属性
			0x31, 0x37, 0x62, 0x62, 0x32, 0x39, 0x66, 0x63, //产品密匙
			0x31, 0x39, 0x32, 0x65, 0x34, 0x32, 0x30, 0x36, 
			0x61, 0x34, 0x63, 0x61, 0x38, 0x64, 0x38, 0x36, 
			0x32, 0x66, 0x65, 0x62, 0x36, 0x61, 0x33, 0x31, 
			0xEE																					//校验和
		};
	send_data[2]				= (0x73 - 0x04) >> 8; 				//计算包长度（命令至校验和），高位
	send_data[3]				= (0x73 - 0x04) % 256;				//计算包长度（命令至校验和），低位
	send_data[5]				= sn; 												//获取包序号

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for (uint8_t i = 2; i < 0x73 - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x73 - 1] = checksum;

	//发送数据包
	esp8266_uart->uart_write (send_data, 0x73);
	EMBARC_PRINTF ("product info is ok \r\n");

}


/*
函数功能：MCU向WiFi发送心跳包。
当WiFi模组超过55秒没有收到MCU的数据包，应向MCU发送心跳包。MCU收到心跳包后马上回复。
当WiFi模组连续3次没有收到MCU的心跳回复，进行报警。

*/
void mcu2wifi_heartbeat (void)
{
	uint8_t 				checksum = 0;
	uint8_t 				send_data[0x09] =
		{
			0xFF, 0xFF, 																	//包头
			0x00, 0x05, 																	//包长度
			0x08, 																				//命令
			0x00, 																				//包序号
			0x00, 0x00, 																	//flags		
			0x00																					//校验和
		};
	send_data[2]				= (0x09 - 0x04) >> 8; 				//计算包长度（命令至校验和），高位
	send_data[3]				= (0x09 - 0x04) % 256;				//计算包长度（命令至校验和），低位
	send_data[5]				= sn; 												//获取包序号

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for (uint8_t i = 2; i < 0x09 - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x09 - 1] = checksum;

	//发送数据包
	//printf("%s",send_data);
	esp8266_uart->uart_write (send_data, 0x09);
}


/*
函数功能：MCU向WiFi发送指令，通知WiFi模组进入配置模式。
当WiFi模组收到进入配置模式的指令后，让设备进入对应的SoftAP或AirLink等OnBoarding配置方式�
	��
参数：mode 配置模式，1为SoftAP方式，2为AirLink方式;
			配置方式不合法时，默认进入AirLink配置方式，
			超时时间1分钟。softAp配置，超时时间5分钟

*/
void mcu2wifi_set_cfg_mode (uint8_t mode)
{
	uint8_t 				checksum = 0;
	uint8_t 				send_data[0x0a] =
		{
			0xFF, 0xFF, 																	//包头
			0x00, 0x05, 																	//包长度
			0x09, 																				//命令
			0x00, 																				//包序号
			0x00, 0x00, 																	//flags	
			0x00, 																				//
			0x00																					//校验和
		};
	send_data[2]				= (0x0a - 0x04) >> 8; 				//计算包长度（命令至校验和），高位
	send_data[3]				= (0x0a - 0x04) % 256;				//计算包长度（命令至校验和），低位
	send_data[5]				= ++sn; 											//获取包序号

	/*设置配置方式，1为SoftAP方式，2为AirLink方式;
	配置方式不合法时，默认进入AirLink配置方式，超时时间1分钟。softAp配置，超时时间5分�
		�*/
	send_data[8]				= mode;

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for (uint8_t i = 2; i < 0x0a - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x0a - 1] = checksum;

	//发送数据包
	esp8266_uart->uart_write (send_data, 0x0a);
}


/*
函数功能：MCU向WiFi发送指令，重置WiFi模组。
重置的内容包括WiFi模组保存的局域网WiFi SSID和密码,DID,Passcode等信息。
重置后模组重启进入AirLink配置模式,超时时间5分钟。

*/
void mcu2wifi_reset_wifi (void)
{
	uint8_t 				checksum = 0;
	uint8_t 				send_data[0x09] =
		{
			0xFF, 0xFF, 																	//包头
			0x00, 0x00, 																	//包长度
			0x0b, 																				//命令
			0x00, 																				//包序号
			0x00, 0x00, 																	//flags			
			0x00																					//校验和
		};
	send_data[2]				= (0x09 - 0x04) >> 8; 				//计算包长度（命令至校验和），高位
	send_data[3]				= (0x09 - 0x04) % 256;				//计算包长度（命令至校验和），低位
	send_data[5]				= ++sn; 											//获取包序号	

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for (uint8_t i = 2; i < 0x09 - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x09 - 1] = checksum;

	//发送数据包
	esp8266_uart->uart_write (send_data, 0x09);
}


/*
函数功能：WiFi模组向MCU推送WiFi状态后，MCU向WiFi回复信息。
同时打印WiFi状态。
*/
void mcu2wifi_wifi_statu (uint8_t * receive_data)
{
	uint8_t 				checksum = 0;
	uint8_t 				send_data[0x09] =
		{
			0xFF, 0xFF, 																	//包头
			0x00, 0x05, 																	//包长度
			0x0E, 																				//命令
			0x00, 																				//包序号
			0x00, 0x00, 																	//flags
			0x00																					//校验和
		};
	send_data[2]				= (0x09 - 0x04) >> 8;
	send_data[3]				= (0x09 - 0x04) % 256;
	send_data[5]				= sn;

	for (uint8_t i = 2; i < 0x09 - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x09 - 1] = checksum;
	esp8266_uart->uart_write (send_data, 0x09);

	uint16_t				statu = receive_data[8] *256 + receive_data[9];

	if ((statu) % 2 == 1)
		EMBARC_PRINTF ("SoftAP mode is open\r\n");

	if ((statu >> 1) % 2 == 1)
		EMBARC_PRINTF ("Station mode is open\r\n");

	if ((statu >> 2) % 2 == 1)
		EMBARC_PRINTF ("OnBoarding mode is open\r\n");

	if ((statu >> 3) % 2 == 1)
		EMBARC_PRINTF ("Binding mode is open\r\n");

	if ((statu >> 4) % 2 == 1)
		EMBARC_PRINTF ("The WiFi module is connected to the wireless router,the RSSI = %d\r\n", (statu >> 8) % 8);

	if ((statu >> 5) % 2 == 1)
		EMBARC_PRINTF ("The WiFi module is connected to the M2M server\r\n");
}


