
uint8_t 				sn = 0;


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
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34,
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x34, 
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x36,
			0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x35,
			0x61, 0x31, 0x30, 0x36, 0x37, 0x38, 0x35, 0x66, 
			0x62, 0x61, 0x39, 0x65, 0x34, 0x35, 0x39, 0x30, 
			0x62, 0x39, 0x36, 0x63, 0x63, 0x65, 0x35, 0x31, 
			0x62, 0x37, 0x63, 0x65, 0x35, 0x36, 0x63, 0x39, 
			0x00, 0x00, 																	//可绑定失效时间
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
			0x31, 0x37, 0x62, 0x62, 0x32, 0x39, 0x66, 0x63, 
			0x31, 0x39, 0x32, 0x65, 0x34, 0x32, 0x30, 0x36, 
			0x61, 0x34, 0x63, 0x61, 0x38, 0x64, 0x38, 0x36, 
			0x32, 0x66, 0x65, 0x62, 0x36, 0x61, 0x33, 0x31, 
			0xEE																					//校验和
		};
	send_data[2]				= (0x73 - 0x04) >> 8; 				
	send_data[3]				= (0x73 - 0x04) % 256;			
	send_data[5]				= sn; 												//获取包序号

	for (uint8_t i = 2; i < 0x73 - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x73 - 1] = checksum;

	esp8266_uart->uart_write (send_data, 0x73);
	EMBARC_PRINTF ("product info is ok \r\n");

}



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
	send_data[2]				= (0x09 - 0x04) >> 8; 				
	send_data[3]				= (0x09 - 0x04) % 256;				
	send_data[5]				= sn; 												//获取包序号

	for (uint8_t i = 2; i < 0x09 - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x09 - 1] = checksum;


	//printf("%s",send_data);
	esp8266_uart->uart_write (send_data, 0x09);
}



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
	send_data[2]				= (0x0a - 0x04) >> 8; 				
	send_data[3]				= (0x0a - 0x04) % 256;				
	send_data[5]				= ++sn; 											//获取包序号


	send_data[8]				= mode;

	for (uint8_t i = 2; i < 0x0a - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x0a - 1] = checksum;

	esp8266_uart->uart_write (send_data, 0x0a);
}



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
	send_data[2]				= (0x09 - 0x04) >> 8; 			
	send_data[3]				= (0x09 - 0x04) % 256;			
	send_data[5]				= ++sn; 											//获取包序号	


	for (uint8_t i = 2; i < 0x09 - 1; i++)
		checksum = checksum + send_data[i];

	send_data[0x09 - 1] = checksum;


	esp8266_uart->uart_write (send_data, 0x09);
}



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


