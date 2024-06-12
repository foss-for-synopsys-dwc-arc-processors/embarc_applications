#include "embARC.h"
#include "embARC_debug.h"

#include "dev_gpio.h"
#include "dev_uart.h"
#include "dev_iic.h"
#include "dev_spi.h"
#include "board.h"

#include "esp8266_gagent_common.h"


void mcu2wifi_product_info(DEV_UART_PTR esp8266_uart, uint8_t *product_key, uint8_t *product_secret)
{
	uint8_t checksum=0;
	int i=0; //
	uint8_t send_data[] = {
		0xFF,0xFF, // 
		0x00,0x00, //
		0x02,      //
		0x00,      //
		0x00,0x00,      //flags
		'0','0','0','0','0','0','0','4',      //
		'0','0','0','0','0','0','0','2',      //
		'0','0','0','0','0','0','0','0',      //
		'0','0','0','0','0','0','0','0',      //


		'0','0','0','0','0','0','0','0',      //
		'0','0','0','0','0','0','0','0',      
		'0','0','0','0','0','0','0','0',      
		'0','0','0','0','0','0','0','0',                  

		0x01,0x2C,      //
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,      //

		'0','0','0','0','0','0','0','0',      //
		'0','0','0','0','0','0','0','0',      
		'0','0','0','0','0','0','0','0',      
		'0','0','0','0','0','0','0','0',
		0xEE      //
	};	
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256; //
	send_data[5] = sn;  //
	for(i=0; i<32; i++)
	{
		send_data[40+i] = product_key[i];
		send_data[82+i] = product_secret[i];
	}
	for(i=0; i<32; i++) EMBARC_PRINTF("%c",send_data[40+i]); EMBARC_PRINTF("\r\n");
	for(i=0; i<32; i++) EMBARC_PRINTF("%c",send_data[82+i]); EMBARC_PRINTF("\r\n");	
	//
	for(uint8_t i=2;i< sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
	esp8266_uart->uart_write(send_data,sizeof(send_data));
}


void mcu2wifi_heartbeat(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x00,      //
		0x08,      //
		0x00,      //
		0x00,0x00,      //flags		
		0x00      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = sn;	//
	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}


void mcu2wifi_set_cfg_mode(DEV_UART_PTR esp8266_uart, uint8_t mode)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x05,      //
		0x09,      //
		0x00,      //
		0x00,0x00,      //flags	
		0x00,     //
		0x00      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = ++sn;	//
	
	send_data[8] = mode;
	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}


void mcu2wifi_reset_wifi(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x00,      //
		0x0b,      //
		0x00,      //
		0x00,0x00,      //flags			
		0x00      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = ++sn;	//	
	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}



void mcu2wifi_wifi_statu(DEV_UART_PTR esp8266_uart,uint8_t *receive_data)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x05,      //
		0x0E,      //
		0x00,      //
		0x00,0x00,      //flags
		0x00      //
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
	//LED 5 
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
	//LED 0~2 
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
	//LED 3 
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


void mcu2wifi_receive_error(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x00,      //
		0x12,      //
		0x00,      //
		0x00,0x00,      //flags	
		0x00, //	
		0x00      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = sn;	//
	send_data[8] = 2;
	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}


void mcu2wifi_request_time(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x05,      //
		0x17,      //
		0x00,      //
		0x00,0x00,      //flags			
		0x00      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = ++sn;	//	
	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}


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


void mcu2wifi_restart_wifi(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x00,      //
		0x29,      //
		0x00,      //
		0x00,0x00,      //flags			
		0x00      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = ++sn;	//	
	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}


void mcu2wifi_request_into_binding(DEV_UART_PTR esp8266_uart)
{
	uint8_t checksum=0;
	uint8_t send_data[] = {
		0xFF,0xFF,      //
		0x00,0x00,      //
		0x15,      //
		0x00,      //
		0x00,0x00,      //flags			
		0x00      //
	};
	send_data[2] = (sizeof(send_data)-0x04)>>8;  //
	send_data[3] = (sizeof(send_data)-0x04)%256;  //
	send_data[5] = ++sn;	//	
	//
	for(uint8_t i=2;i<sizeof(send_data)-1;i++) checksum = checksum + send_data[i];
	send_data[sizeof(send_data)-1] =checksum;
	//
    esp8266_uart->uart_write(send_data,sizeof(send_data));
}
