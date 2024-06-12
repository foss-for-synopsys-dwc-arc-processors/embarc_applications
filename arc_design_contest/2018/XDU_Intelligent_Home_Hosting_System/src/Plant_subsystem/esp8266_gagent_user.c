#include "embARC.h"
#include "embARC_debug.h"

#include "dev_gpio.h"
#include "dev_uart.h"
#include "dev_iic.h"
#include "dev_spi.h"

#include "board.h"
#include "esp8266_gagent_user.h"


void mcu2wifi_wifi_ctrl_dev(DEV_UART_PTR esp8266_uart,uint8_t *receive_data)
{
	//
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x00,      //
		0x04,      //
		0x01,      //
		0x00,0x00,      //flags		
		0x14      //
	};
	uint8_t i;
	send_data[2] = (sizeof(send_data)-0x04)>>8; //
	send_data[3] = (sizeof(send_data)-0x04)%256;//
	send_data[5] = sn;	//
	//
	for(i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));

    //
    i=0; //
	//
	uint8_t attr_flags[ ATTR_FLAGS_BYTES ];
	for(i=0;i<ATTR_FLAGS_BYTES;i++) attr_flags[i] = receive_data[9 + i];
	//
	uint8_t attr_vals[ATTR_VALS_BYTES];
	for(i=0;i<ATTR_VALS_BYTES;i++)  attr_vals[i]  = receive_data[9+ATTR_FLAGS_BYTES+i];

	//
	if(attr_flags[0] & 0x01) //Plant_Watering 
	{
		Plant_Watering = attr_vals[0];
	}
	if(attr_flags[0] & 0x02) //
	{
		WaterThreshold = attr_vals[1];		
	}
	
}


void mcu2wifi_wifi_read_dev(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x05,      //
		0x04,      //
		0x00,      //
		0x00,0x00,      //flags
		0x03,         //action;
		0x00,0x00,0x00,0x00,0x00, 0x00,//
		0x14      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = sn; //
	//
	// Plant_Watering
	send_data[9] = Plant_Watering;	
	// WaterThreshold
	send_data[10] = WaterThreshold;	
	// Plant_Soil_Moisture
	send_data[11] = Plant_Soil_Moisture;
	// Plant_Env_Temp
	send_data[12] = Plant_Env_Temp;	
	// Plant_Env_Humidity
	send_data[13] = Plant_Env_Humidity;
	// Add_water
	send_data[14] = 10 - day % 10;
			
	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}


void mcu2wifi_dev_report_status(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[10 + DEV_STATUS_BYTES] = {
		0xFF,0xFF,      //
		0x00,0x05,      //
		0x05,      //
		0x00,      //
		0x00,0x00,      //flags
		0x04,         //action;
		0x00,0x00,0x00,0x00,0x00, 0x00,//
		0x14      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = ++sn; //
	//
	// Plant_Watering
	send_data[9] = Plant_Watering;	
	// WaterThreshold
	send_data[10] = WaterThreshold;	
	// Plant_Soil_Moisture
	send_data[11] = Plant_Soil_Moisture;
	// Plant_Env_Temp
	send_data[12] = Plant_Env_Temp;	
	// Plant_Env_Humidity
	send_data[13] = Plant_Env_Humidity;
	// Add_water
	send_data[14] = day % 10;
			
	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}