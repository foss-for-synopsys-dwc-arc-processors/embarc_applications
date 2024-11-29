#include "embARC.h"
#include "embARC_debug.h"

#include "dev_gpio.h"
#include "dev_uart.h"
#include "dev_iic.h"
#include "dev_spi.h"

#include "board.h"
#include "step_motor.h"
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
	
	if (attr_flags[0] & 0x01)
	{
		Leave_home = attr_vals[0] & 0x01;		
	}
	if(attr_flags[0] & 0x02) //
	{
		Room_Lamp = (attr_vals[0] & 0x06) / 2;
	}
	if (attr_flags[0] & 0x04)
	{
		Fan_switch = (attr_vals[0] & 0x18) / 8;
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
		0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,//
		0x14      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = sn; //
	//
	// Leave_home	
	if(Leave_home) send_data[9] = send_data[9] | 0x01;
	else send_data[9] = send_data[9] & 0xFE;
	//Room_Lamp
	if(Room_Lamp == 0) send_data[9] = send_data[9] & 0xF9;
	if(Room_Lamp == 1) {send_data[9] = send_data[9] & 0xFB; send_data[9] = send_data[9] | 0x02;}
	if(Room_Lamp == 2) {send_data[9] = send_data[9] | 0x04; send_data[9] = send_data[9] & 0xFD;}
	//Fan_switch
	if(Fan_switch == 0) send_data[9] = send_data[9] & 0xE7;
	if(Fan_switch == 1) {send_data[9] = send_data[9] & 0xEF; send_data[9] = send_data[9] | 0x08;}
	if(Fan_switch == 2) {send_data[9] = send_data[9] | 0x10; send_data[9] = send_data[9] & 0xF7;}

	
	// Room_Env_Tepm
	send_data[10] = Room_Env_Tepm;
	// Room_Env_Humidity
	send_data[11] = Room_Env_Humidity;	
	// Room_Smoke
	send_data[12] = Room_Smoke;
	// Room_Light
	send_data[13] = Room_Light;	
	// Room_PM25
	send_data[14] = (uint8_t)(Room_PM25 / 256);
	send_data[15] = (uint8_t)(Room_PM25 % 256);

	// Body_infrared
	if(Body_infrared) send_data[16] = send_data[16] | 0x01;
	else send_data[16] = send_data[16] & 0xFE;

	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}


void mcu2wifi_dev_report_status(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x05,      //
		0x05,      //
		0x00,      //
		0x00,0x00,      //flags
		0x04,         //action;
		0x00,0x00,0x00,0x00,0x00,0x00, 0x00,0x00,//
		0x14      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = ++sn; //
	//
	// Leave_home	
	if(Leave_home) send_data[9] = send_data[9] | 0x01;
	else send_data[9] = send_data[9] & 0xFE;
	//Room_Lamp
	if(Room_Lamp == 0) send_data[9] = send_data[9] & 0xF9;
	if(Room_Lamp == 1) {send_data[9] = send_data[9] & 0xFB; send_data[9] = send_data[9] | 0x02;}
	if(Room_Lamp == 2) {send_data[9] = send_data[9] | 0x04; send_data[9] = send_data[9] & 0xFD;}
	//Fan_switch
	if(Fan_switch == 0) send_data[9] = send_data[9] & 0xE7;
	if(Fan_switch == 1) {send_data[9] = send_data[9] & 0xEF; send_data[9] = send_data[9] | 0x08;}
	if(Fan_switch == 2) {send_data[9] = send_data[9] | 0x10; send_data[9] = send_data[9] & 0xF7;}
	
	// Room_Env_Tepm
	send_data[10] = Room_Env_Tepm;
	// Room_Env_Humidity
	send_data[11] = Room_Env_Humidity;	
	// Room_Smoke
	send_data[12] = Room_Smoke;
	// Room_Light
	send_data[13] = Room_Light;	
	// Room_PM25
	send_data[14] = (uint8_t)(Room_PM25 / 256);
	send_data[15] = (uint8_t)(Room_PM25 % 256);

	// Body_infrared
	if(Body_infrared) send_data[16] = send_data[16] | 0x01;
	else send_data[16] = send_data[16] & 0xFE;

	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}