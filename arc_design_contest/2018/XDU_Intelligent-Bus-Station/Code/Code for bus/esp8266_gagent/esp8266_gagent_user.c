
uint8_t 				buf = 0; //站点初始化
uint8_t 				SN = 0;
uint8_t 				CR = 0;
uint8_t 				crow = 0; //拥挤程度初始化
uint16_t				temp = 0; //只读

#define DEV_STATUS_BYTES				(2)

/*
函数功能：MCU向WiFi模组回复信息,并做相应控制。
控制部分应依照数据点定义以及机智云WiFi类设备接入协议文档做相应处理

*/
// 定义attr_flags字段的字节长度
#define ATTR_FLAGS_BYTES				(1)

// 定义attr_vals字段的字节长度
#define ATTR_VALS_BYTES 				(2)


void mcu2wifi_wifi_ctrl_dev (uint8_t * receive_data)
{
	//先回复WiFi模组
	uint8_t 				checksum = 0;
	uint8_t 				send_data[0x09] =
		{
			0xFF, 0xFF, 																	//包头
			0x00, 0x00, 																	//包长度
			0x04, 																				//命令
			0x01, 																				//包序号
			0x00, 0x00, 																	//flags		
			0x14																					//校验和
		};
	send_data[2]				= (0x09 - 0x04) >> 8; 				//计算包长度（命令至校验和），高位
	send_data[3]				= (0x09 - 0x04) % 256;				//计算包长度（命令至校验和），低位
	send_data[5]				= sn; 												//获取包序号

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for (uint8_t i = 2; i < 0x09 - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x09 - 1] = checksum;

	//向WiFi模组发送数据包
	esp8266_uart->uart_write (send_data, 0x09);
}


/*
函数功能：MCU向WiFi模组回复设备当前状态。

*/
//定义dev_status字段字节长度，数值可见机智云接入协议
void mcu2wifi_wifi_read_dev (void)
{
	uint8_t 				checksum = 0;
	uint8_t 				send_data[0x0A + DEV_STATUS_BYTES] =
		{
			0xFF, 0xFF, 																	//包头
			0x00, 0x05, 																	//包长度
			0x04, 																				//命令
			0x00, 																				//包序号
			0x00, 0x00, 																	//flags
			0x03, 																				//action;
			0x00, 0x00, 																	//设备状态，具体字节长度由DEV_STATUS_BYTES决定
			0x14																					//校验和
		};
	send_data[2]				= (0x0A + DEV_STATUS_BYTES - 0x04) >> 8; //计算包长度（命令至校验和），高位
	send_data[3]				= (0x0A + DEV_STATUS_BYTES - 0x04) % 256; //计算包长度（命令至校验和），低位
	send_data[5]				= sn; 												//获取包序号

	//设备状态,具体字节长度由DEV_STATUS_BYTES决定
	// SN
	send_data[9]				= SN;

	// CR
	send_data[10] 			= CR;

	// temp 高位
	//send_data[11] = temp >> 8;
	// temp 低位
	send_data[11] 			= temp % 256;

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for (uint8_t i = 2; i < 0x0A + DEV_STATUS_BYTES - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x0A + DEV_STATUS_BYTES - 1] = checksum;

	//向WiFi模组发送数据包
	esp8266_uart->uart_write (send_data, 0x0A + DEV_STATUS_BYTES);
}


/*
函数功能：MCU向WiFi模组主动设备当前状态。

注意：关于发送频率。当设备MCU收到WiFi模组控制产生的状态变化,设备MCU应立刻主动上报
当前状态,发送频率不受限制。
但如设备的状态的变化是由于用户触发或环境变化所产生的,其发送的频率不能快于6秒每条。
建议按需上报，有特殊上报需求请。建议按需上报，有特殊上报需求请联系机智云。

注意：设备MCU需要每隔10分钟定期主动上报当前状态。

*/
void mcu2wifi_dev_report_status (uint8_t SN1, uint8_t CR1)
{
	uint8_t 				checksum = 0;
	uint8_t 				send_data[0x0A + DEV_STATUS_BYTES] =
		{
			0xFF, 0xFF, 																	//包头
			0x00, 0x08, 																	//包长度
			0x05, 																				//命令
			0x00, 																				//包序号
			0x00, 0x00, 																	//flagsflags
			0x04, 																				//action;
			0x11, 0x11, 																	//设备状态，具体字节长度由DEV_STATUS_BYTES决定
			0x00																					//校验和
		};
	send_data[2]				= (0x0A + DEV_STATUS_BYTES - 0x04) >> 8; //计算包长度（命令至校验和），高位
	send_data[3]				= (0x0A + DEV_STATUS_BYTES - 0x04) % 256; //计算包长度（命令至校验和），低位
	send_data[5]				= ++sn; 											//获取包序号

	//设备状态,具体字节长度由DEV_STATUS_BYTES决定
	// SN
	send_data[9]				= SN1;

	// CR
	send_data[10] 			= CR1;

	// temp 高位
	send_data[11] 			= temp % 256;

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for (uint8_t i = 2; i < 0x0A + DEV_STATUS_BYTES - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x0A + DEV_STATUS_BYTES - 1] = checksum;

	//向WiFi模组发送数据包
	esp8266_uart->uart_write (send_data, 0x0A + DEV_STATUS_BYTES);
}



/*写函数，将设备状态的数据写入WiFi模组中*/
void mcu2wifi_dev_write (void)
{
	uint8_t 				checksum = 0;
	uint8_t 				send_data[0x0A + DEV_STATUS_BYTES] =
		{
			0xFF, 0xFF, 																	//包头
			0x00, 0x0A, 																	//包长度
			0x05, 																				//命令
			0x00, 																				//包序号
			0x00, 0x00, 																	//flags
			0x04, 																				//action;
			0x11, 0x11, 																	//设备状态，具体字节长度由DEV_STATUS_BYTES决定
			0x00																					//校验和
		};
	send_data[2]				= (0x0A + DEV_STATUS_BYTES - 0x04) >> 8; //计算包长度（命令至校验和），高位
	send_data[3]				= (0x0A + DEV_STATUS_BYTES - 0x04) % 256; //计算包长度（命令至校验和），低位
	send_data[5]				= ++sn; 											//获取包序号

	//设备状态,具体字节长度由DEV_STATUS_BYTES决定
	// SN

	/*send_data[9] = SN;
	// CR
	send_data[10] = CR;
	// temp 高位
	send_data[11] = temp >> 8;*/
	// temp 低位
	send_data[11] 			= temp % 256;

	//计算校验和（包长度至校验和前一byte所有数据之和对256取余）
	for (uint8_t i = 2; i < 0x0A + DEV_STATUS_BYTES - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x0A + DEV_STATUS_BYTES - 1] = checksum;

	//向WiFi模组发送数据包
	esp8266_uart->uart_write (send_data, 0x0A + DEV_STATUS_BYTES);
}

