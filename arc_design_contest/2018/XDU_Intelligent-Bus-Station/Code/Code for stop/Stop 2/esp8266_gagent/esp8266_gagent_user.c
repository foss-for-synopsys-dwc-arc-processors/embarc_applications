uint8_t SN = 1; 	
uint8_t CR = 2; 	
uint8_t buf = 0;
uint8_t crow = 0;	
uint16_t temp = 0; 	
#define DEV_STATUS_BYTES (2)


#define ATTR_FLAGS_BYTES  (1)
#define ATTR_VALS_BYTES   (2)
void mcu2wifi_wifi_ctrl_dev(uint8_t *receive_data)
{
	uint8_t checksum=0;
	uint8_t send_data[0x09] = {
		0xFF,0xFF,  
		0x00,0x00,     
		0x04, 
		0x01,    
		0x00,0x00,      //flags		
		0x14    
	};
	send_data[2] = (0x09-0x04)>>8; 
	send_data[3] = (0x09-0x04)%256;
	send_data[5] = sn;	
	for(uint8_t i=2;i<0x09-1;i++) checksum = checksum + send_data[i];
	send_data[0x09-1] =checksum;

    esp8266_uart->uart_write(send_data,0x09);
}

void mcu2wifi_wifi_read_dev(void)
{
	uint8_t checksum=0;
	uint8_t send_data[0x0A + DEV_STATUS_BYTES] = {
		0xFF,0xFF,   
		0x00,0x05,    
		0x04,   
		0x00,     
		0x00,0x00,      //flags
		0x03,         //action;
		0x00,0x00, 
		0x14   
	};
	send_data[2] = (0x0A + DEV_STATUS_BYTES-0x04)>>8;  
	send_data[3] = (0x0A + DEV_STATUS_BYTES-0x04)%256;  
	send_data[5] = sn; 

	// SN
	send_data[9] = SN;
	// CR
	send_data[10] = CR;

	//send_data[11] = temp >> 8;

	send_data[11] = temp % 256;
	for(uint8_t i=2;i<0x0A + DEV_STATUS_BYTES-1;i++) checksum = checksum + send_data[i];
	send_data[0x0A + DEV_STATUS_BYTES-1] =checksum;

    esp8266_uart->uart_write(send_data,0x0A + DEV_STATUS_BYTES);
}



void mcu2wifi_dev_report_status(void)
{
	uint8_t checksum=0;
	uint8_t send_data[0x0A + DEV_STATUS_BYTES] = {
		0xFF,0xFF,    
		0x00,0x08,    
		0x05,   
		0x00,     
		0x00,0x00,      //flagsflags
		0x04,         //action;
		0x11,0x11, 
		0x00      
	};
	send_data[2] = (0x0A + DEV_STATUS_BYTES-0x04)>>8;  
	send_data[3] = (0x0A + DEV_STATUS_BYTES-0x04)%256;  
	send_data[5] = ++sn;
	// SN
	send_data[9] = SN;
	// CR
	send_data[10] = CR;
	send_data[11] = temp % 256;
	for(uint8_t i=2;i<0x0A + DEV_STATUS_BYTES-1;i++) checksum = checksum + send_data[i];
	send_data[0x0A + DEV_STATUS_BYTES-1] =checksum;
    esp8266_uart->uart_write(send_data,0x0A + DEV_STATUS_BYTES);
}
	
	

void mcu2wifi_dev_write(void)
{
	uint8_t checksum=0;
	uint8_t send_data[0x0A + DEV_STATUS_BYTES] = {
		0xFF,0xFF,     
		0x00,0x0A,   
		0x05,     
		0x00,    
		0x00,0x00,      //flags
		0x04,         //action;
		0x11,0x11,
		0x00    
	};
	send_data[2] = (0x0A + DEV_STATUS_BYTES-0x04)>>8; 
	send_data[3] = (0x0A + DEV_STATUS_BYTES-0x04)%256;  
	send_data[5] = ++sn; 
	// SN
	/*send_data[9] = SN;
	// CR
	send_data[10] = CR;
	send_data[11] = temp >> 8;*/
	send_data[11] = temp % 256;
	for(uint8_t i=2;i<0x0A + DEV_STATUS_BYTES-1;i++) checksum = checksum + send_data[i];
	send_data[0x0A + DEV_STATUS_BYTES-1] =checksum;
    esp8266_uart->uart_write(send_data,0x0A + DEV_STATUS_BYTES);
}
