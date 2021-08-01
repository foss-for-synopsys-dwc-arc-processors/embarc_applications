// embARC
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
// DEVICE-iotdk
#include "board.h"
#include "dev_iic.h"
#include "dev_gpio.h"
#include "dev_uart.h"
#include "dfss_iic_obj.h"
#include "dfss_gpio_obj.h"
// dht11
#include "dht11.h"
// 24L01-WiFi
#include "24l01.h"
// dwin
#include "dwin.h"
// ds1302
#include "ds1302.h"
// syn6288
#include "audio_synth.h"
#include "paj7620.h"

//-----------------------------------------------------------------------------------
//      main function
//-----------------------------------------------------------------------------------
int main()
{
	/* Initialization */
	cpu_lock();                                 // board initial
	board_init(); 
	cpu_unlock();
    		EMBARC_PRINTF("BOARD INITIAL  OK!\n\r");

    rtc_init();                                 // rtc initial
    dht11_init();
    Master1_slvdev_init(I2C_SLAVE_UART_ADDR);   // dwin i2c to uart initial
    NRF24L01_Init();                            //nrf24l01 initial
    NRF24L01_RX_Mode();
	audio_synth_init(UART_BAUDRATE_9600);
	dwin_display_init();
	paj7620_config();
			EMBARC_PRINTF("DEVICE ALL INITIAL  OK!\n\r");
	

	// /* Modify the time only it is incorrect*/
	// rtc_write_byte(DS1302_CONTROL_ADD,   0x00);			// turn off write protection
	// rtc_write_byte(DS1302_SEC_ADD,       0x80);			// pause the clock
	// // rtc_write_byte(DS1302_CHARGER_ADD,   0xa9);         // trickle charge
	// rtc_write_byte(DS1302_YEAR_ADD,      0x21);			// year (20xx)
	// rtc_write_byte(DS1302_MONTH_ADD,     0x06);			// month
	// rtc_write_byte(DS1302_DATE_ADD,      0x29);			// day
	// rtc_write_byte(DS1302_HR_ADD,        0x11);			// hour
	// rtc_write_byte(DS1302_MIN_ADD,       0x00);			// minute
	// rtc_write_byte(DS1302_SEC_ADD,       0x00);			// second
	// rtc_write_byte(DS1302_DAY_ADD,       0x6);			// day of week
	// rtc_write_byte(DS1302_CONTROL_ADD,   0x80);			// turn on write protection
	
	while(1)
	{	
		dwin_display();
		// board_delay_ms(100, 1);		
	}

    return E_SYS;
}