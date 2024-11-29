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
		0x00,      //
		0x00,0x00,      //flags		
		0x00      //
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
    
}

void mcu2wifi_wifi_read_dev(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[0x0A + DEV_STATUS_BYTES] = {
		0xFF,0xFF,      //
		0x00,0x05,      //
		0x04,      //
		0x00,      //
		0x00,0x00,      //flags
		0x03,         //action;
		0x00, //
		0x14      //
	};
	send_data[2] = (0x0A + DEV_STATUS_BYTES-0x04)>>8;  //
	send_data[3] = (0x0A + DEV_STATUS_BYTES-0x04)%256;  //
	send_data[5] = sn; //
	//
	// Aquarius_Oxy_Freq and Aquarius_Feed_Freq and Aquarius_Light
	send_data[9] = Lamp_switch * 8 + Fan_switch * 4 + LeaveModel_switch * 2 + AquariusLight_switch;	
	// Aquarius_Feed_Amount
	//send_data[10] = Aquarius_Feed_Amount;
	// Aquarius_Temp
	//send_data[11] = Aquarius_Temp;	
		
	//
	for(uint8_t i=2;i<0x0A + DEV_STATUS_BYTES-1;i++) checksum = checksum + send_data[i];
	send_data[0x0A + DEV_STATUS_BYTES-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,0x0A + DEV_STATUS_BYTES);
}


void mcu2wifi_dev_report_status(DEV_UART_PTR esp8266_uart)
{
		uint8_t checksum=0;
	uint8_t send_data[0x0A + DEV_STATUS_BYTES] = {
		0xFF,0xFF,      //
		0x00,0x05,      //
		0x05,      //
		0x00,      //
		0x00,0x00,      //flags
		0x04,         //action;
		0x00, //
		0x14      //
	};
	send_data[2] = (0x0A + DEV_STATUS_BYTES-0x04)>>8;  //
	send_data[3] = (0x0A + DEV_STATUS_BYTES-0x04)%256;  //
	send_data[5] = ++sn; //
	//
	// Aquarius_Oxy_Freq and Aquarius_Feed_Freq and Aquarius_Light
	send_data[9] = AquariusLight_switch * 8 + LeaveModel_switch * 4 + Fan_switch * 2 + Lamp_switch;
	// Aquarius_Feed_Amount
	//send_data[10] = Aquarius_Feed_Amount;
	// Aquarius_Temp
	//send_data[11] = Aquarius_Temp;	
		
	//
	for(uint8_t i=2;i<0x0A + DEV_STATUS_BYTES-1;i++) checksum = checksum + send_data[i];
	send_data[0x0A + DEV_STATUS_BYTES-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,0x0A + DEV_STATUS_BYTES);
}