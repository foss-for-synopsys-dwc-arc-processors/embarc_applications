
uint8_t 				buf = 0;
uint8_t 				SN = 0;
uint8_t 				CR = 0;
uint8_t 				crow = 0; 
uint16_t				temp = 0;

#define DEV_STATUS_BYTES				(2)


#define ATTR_FLAGS_BYTES				(1)


#define ATTR_VALS_BYTES 				(2)


void mcu2wifi_wifi_ctrl_dev (uint8_t * receive_data)
{

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
	send_data[2]				= (0x09 - 0x04) >> 8; 				
	send_data[3]				= (0x09 - 0x04) % 256;				
	send_data[5]				= sn; 												//获取包序号


	for (uint8_t i = 2; i < 0x09 - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x09 - 1] = checksum;


	esp8266_uart->uart_write (send_data, 0x09);
}



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
	send_data[2]				= (0x0A + DEV_STATUS_BYTES - 0x04) >> 8; 
	send_data[3]				= (0x0A + DEV_STATUS_BYTES - 0x04) % 256; 
	send_data[5]				= sn; 												//获取包序号

	// SN
	send_data[9]				= SN;

	// CR
	send_data[10] 			= CR;

	// temp 
	//send_data[11] = temp >> 8;
	// temp
	send_data[11] 			= temp % 256;


	for (uint8_t i = 2; i < 0x0A + DEV_STATUS_BYTES - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x0A + DEV_STATUS_BYTES - 1] = checksum;


	esp8266_uart->uart_write (send_data, 0x0A + DEV_STATUS_BYTES);
}



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
	send_data[2]				= (0x0A + DEV_STATUS_BYTES - 0x04) >> 8; 
	send_data[3]				= (0x0A + DEV_STATUS_BYTES - 0x04) % 256; 
	send_data[5]				= ++sn; 											//获取包序号

	// SN
	send_data[9]				= SN1;

	// CR
	send_data[10] 			= CR1;


	send_data[11] 			= temp % 256;


	for (uint8_t i = 2; i < 0x0A + DEV_STATUS_BYTES - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x0A + DEV_STATUS_BYTES - 1] = checksum;


	esp8266_uart->uart_write (send_data, 0x0A + DEV_STATUS_BYTES);
}




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
	send_data[2]				= (0x0A + DEV_STATUS_BYTES - 0x04) >> 8;
	send_data[3]				= (0x0A + DEV_STATUS_BYTES - 0x04) % 256; 
	send_data[5]				= ++sn; 											//获取包序号


	// SN

	/*send_data[9] = SN;
	// CR
	send_data[10] = CR;

	send_data[11] = temp >> 8;*/

	send_data[11] 			= temp % 256;

	for (uint8_t i = 2; i < 0x0A + DEV_STATUS_BYTES - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x0A + DEV_STATUS_BYTES - 1] = checksum;


	esp8266_uart->uart_write (send_data, 0x0A + DEV_STATUS_BYTES);
}

