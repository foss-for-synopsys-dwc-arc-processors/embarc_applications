#include "embARC.h"
#include "embARC_debug.h"
#include <stdio.h>
#include "board.h"
#include "dev_iic.h"
#include "dev_gpio.h"

#include "../../../board/emsk/drivers/mux/mux.h"
#include "../../../board/emsk/drivers/mux/mux_hal.h"

#include "ad7991.h"
// #include "step_motor.h"
#include "esp8266_gagent_common.h"
#include "esp8266_gagent_user.h"

#include "DHT11.h"
#include "GP2Y1051.h"


//
DEV_UART_PTR esp8266_uart;
#define ESP8266_UART_ID DW_UART_0_ID
#define ESP8266_UART_BAUDRATE UART_BAUDRATE_9600

void esp8266_init(void)
{
	// set pmod mux and uart map
	set_uart_map(0xe4);	
	
	esp8266_uart = uart_get_dev(ESP8266_UART_ID);
	esp8266_uart->uart_close();
	if(esp8266_uart->uart_open(ESP8266_UART_BAUDRATE) == E_OPNED)
		{
			esp8266_uart->uart_control(UART_CMD_SET_BAUD, (void *)(ESP8266_UART_BAUDRATE));
		}		
}
uint32_t esp8266_read(uint8_t *buf, uint32_t cnt)
{
	if (esp8266_uart == NULL) return 0;

	uint32_t rd_avail;
	esp8266_uart->uart_control(UART_CMD_GET_RXAVAIL, (void *)(&rd_avail));
	cnt = (cnt > rd_avail)?rd_avail:cnt;
	if (cnt > 0) {
		return esp8266_uart->uart_read(buf, cnt);
	}
	return cnt;
}
uint32_t esp8266_write(uint8_t *buf, uint32_t cnt)
{
	if (esp8266_uart == NULL) return 0;

	return esp8266_uart->uart_write(buf, cnt);
}

DEV_GPIO_PTR  dht11;
DHT11_DEF_PTR dht11_data;
#define DHT11_PORT DW_GPIO_PORT_C
#define DHT11_BIT  (19)
void DHT11_init(void)
{
	dht11 = gpio_get_dev(DHT11_PORT);
	dht11->gpio_open(GPIO_DIR_OUTPUT);
}

AD7991_DEF_PTR ad7991;
float ad7991_data[4];
#define AD7991_ID  (0)
void ad7991_init(void)
{
	ad7991->dev_id = AD7991_ID;
	ad7991->slvaddr = ADC_I2C_SLAVE_ADDRESS;
	ad7991_adc_init(ad7991);
}

DEV_GPIO_PTR button;
#define BUTTON_MASK 0x07
void gpio_keyL_isr(void *ptr)
{
	EMBARC_PRINTF("key 1: L\r\n");
	mcu2wifi_request_into_binding(esp8266_uart);
}
void gpio_keyR_isr(void *ptr)
{
	EMBARC_PRINTF("key 2: R\r\n");
	mcu2wifi_reset_wifi(esp8266_uart);
}
void gpio_keyA_isr(void *ptr)
{
	EMBARC_PRINTF("key 3: A\r\n");
}
void button_init(void)
{
	DEV_GPIO_BIT_ISR bit_isr;
	DEV_GPIO_INT_CFG int_cfg;
	button = gpio_get_dev(DW_GPIO_PORT_A);
	button ->gpio_open(BUTTON_MASK);
	button ->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)BUTTON_MASK);
	int_cfg.int_bit_mask = BUTTON_MASK;
	int_cfg.int_bit_type = GPIO_INT_BITS_EDGE_TRIG(BUTTON_MASK);
	int_cfg.int_bit_polarity = GPIO_INT_BITS_POL_FALL_EDGE(BUTTON_MASK);
	int_cfg.int_bit_debounce = GPIO_INT_BITS_ENA_DEBOUNCE(BUTTON_MASK);
	button->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);
	bit_isr.int_bit_ofs = 0;
	bit_isr.int_bit_handler = gpio_keyL_isr;
	button->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&bit_isr));
	bit_isr.int_bit_ofs = 1;
	bit_isr.int_bit_handler = gpio_keyR_isr;
	button->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&bit_isr));
	bit_isr.int_bit_ofs = 2;
	bit_isr.int_bit_handler = gpio_keyA_isr;
	button->gpio_control(GPIO_CMD_SET_BIT_ISR, (void *)(&bit_isr));

	button->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)(0x7));
}

DEV_GPIO_PTR  body_ir;
#define BODY_PORT DW_GPIO_PORT_A
#define BODY_BIT  (19)
#define BODY_MASK (0x01<<BODY_BIT)
void body_ir_init(void)
{
	body_ir = gpio_get_dev(BODY_PORT);
	body_ir-> gpio_open(GPIO_DIR_INPUT);
	body_ir-> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(BODY_MASK));
}
uint32_t body_ir_read(void)
{
	uint32_t read_data;
	body_ir-> gpio_read(&read_data,BODY_MASK);
	return read_data;
}

DEV_GPIO_PTR fan;
#define FAN_PORT DW_GPIO_PORT_C
#define FAN_BIT  (31)
#define FAN_MASK (0x01 << FAN_BIT)

void fan_open(void)
{
	fan = gpio_get_dev(FAN_PORT);
	fan->gpio_open(FAN_MASK);
	fan->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(FAN_MASK));
	fan->gpio_write(0x01<<FAN_BIT, FAN_MASK);
}
void fan_close(void)
{
	fan = gpio_get_dev(FAN_PORT);
	fan->gpio_open(FAN_MASK);
	fan->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(FAN_MASK));
	fan->gpio_write(0x00, FAN_MASK);
}

DEV_GPIO_PTR buzzer;
#define BUZZER_PORT DW_GPIO_PORT_A
#define BUZZER_BIT  (31)
#define BUZZER_MASK (0x01 << FAN_BIT)

void buzzer_open(void)
{
	buzzer = gpio_get_dev(BUZZER_PORT);
	buzzer->gpio_open(BUZZER_MASK);
	buzzer->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(BUZZER_MASK));
	buzzer->gpio_write(0x01<<BUZZER_BIT, BUZZER_MASK);
}
void buzzer_close(void)
{
	buzzer = gpio_get_dev(BUZZER_PORT);
	buzzer->gpio_open(BUZZER_MASK);
	buzzer->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(BUZZER_MASK));
	buzzer->gpio_write(0x00, BUZZER_MASK);
}

DEV_GPIO_PTR roomLamp;
#define roomLamp_PORT DW_GPIO_PORT_C
#define roomLamp_BIT (23)
#define roomLamp_MASK (0x01<<roomLamp_BIT)
void roomLamp_open(void)
{
	roomLamp = gpio_get_dev(roomLamp_PORT);
	roomLamp->gpio_open(0x01 << roomLamp_BIT);
	roomLamp->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(roomLamp_MASK));
	roomLamp->gpio_write(0x01 << roomLamp_BIT, roomLamp_MASK);
}
void roomLamp_close(void)
{
	roomLamp = gpio_get_dev(roomLamp_PORT);
	roomLamp->gpio_open(0x01 << roomLamp_BIT);
	roomLamp->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(roomLamp_MASK));
	roomLamp->gpio_write(0x00 << roomLamp_BIT, roomLamp_MASK);	
}




void timer0_isr(void *ptr)
{
	
	timer_int_clear(TIMER_0);

	
	mcu2wifi_request_time(esp8266_uart);
	mcu2wifi_dev_report_status(esp8266_uart);
	
}
void timer1_isr(void *ptr)
{	
	timer_int_clear(TIMER_1);

		
	
	DHT11_Read_Data(dht11, DHT11_BIT, dht11_data);
	Room_Env_Tepm = dht11_data->T_data;
	Room_Env_Humidity =dht11_data->RH_data;
	EMBARC_PRINTF("Temperature is %d, Humidity is %d%%\r\n", dht11_data->T_data,dht11_data->RH_data);

	Room_PM25 = GP2Y1051_read();
	EMBARC_PRINTF("The PM2.5 is %d ug/m3.\r\n",Room_PM25);
	
	
}

uint8_t *product_key = "ea7f60661bb543dd9da050869fc611e7";
uint8_t *product_secret = "f13add71b9d749b496cba0d84c46f0b4";

int main(void)
{
	cpu_lock();	
	board_init();	

	timer_stop(TIMER_0); /* Stop it first since it might be enabled before */
	int_handler_install(INTNO_TIMER0, timer0_isr);
	int_enable(INTNO_TIMER0);
	timer_start(TIMER_0, TIMER_CTRL_IE, 10*BOARD_CPU_CLOCK);

	timer_stop(TIMER_1); /* Stop it first since it might be enabled before */
	int_handler_install(INTNO_TIMER1, timer1_isr);
	int_enable(INTNO_TIMER1);
	timer_start(TIMER_1, TIMER_CTRL_IE, 2*BOARD_CPU_CLOCK);

	set_pmod_mux(
		PM1_UR_UART_0|PM1_LR_GPIO_A | 
		PM2_I2C_HRI | 
		PM3_GPIO_AC | 
		PM4_GPIO_AC |
		PM5_UR_GPIO_C | PM5_LR_SPI_M2 |
		PM6_UR_GPIO_C | PM6_LR_GPIO_A
		);

	button_init();
	cpu_unlock();		

	

	//step_motor_init(STEP_MOTOR_PORT, STEP_MOTOR_MASK);
	DHT11_init();  
	ad7991_init();  
	esp8266_init(); 
	body_ir_init();

	roomLamp_close();

	uint32_t rd_cnt = 0;
	uint8_t read_data[MAX_READ_CNT];	

	led = gpio_get_dev(LED_PORT);
	led-> gpio_open(0x80);
	led-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(0x80));
	uint8_t led_count = 0;

	while(1)
	{
		if((led_count)%10 == 0 ) led->gpio_write(0x80,0x80);
		if((led_count)%20 == 0 ) led->gpio_write(0x00,0x80);
		led_count++;

		board_delay_ms(100,0);
		rd_cnt = esp8266_read(read_data,sizeof(read_data));
		if(rd_cnt)
		{
			for(uint8_t i=0;i<rd_cnt;i++) EMBARC_PRINTF("%2x ",read_data[i]);
			EMBARC_PRINTF("\r\n");
			
			if(read_data[0]==0xFF && read_data[1]==0xFF) 
			{			
				sn = read_data[5];
				switch(read_data[4]) 
				{
					case 0x01: mcu2wifi_product_info(esp8266_uart,product_key,product_secret);break;					
					case 0x03: 
						switch(read_data[8])
						{
							case 0x01:mcu2wifi_wifi_ctrl_dev(esp8266_uart,read_data);break;
							case 0x02:mcu2wifi_wifi_read_dev(esp8266_uart);break;
							default : ;
						}
					case 0x06: break;
					case 0x07: mcu2wifi_heartbeat(esp8266_uart);break;
					case 0x0a: break;
					case 0x0c: break;
					case 0x0d: mcu2wifi_wifi_statu(esp8266_uart,read_data);break;
					case 0x11: EMBARC_PRINTF("Data package is not valid!!!\r\n");break;
					case 0x18: wifi2mcu_get_time(read_data);
					case 0x2a: break;
					default : EMBARC_PRINTF("Undefine commond or error!!!\r\n");break;
				}
			}
			else
			{
				mcu2wifi_receive_error(esp8266_uart);
			}
		}
		uint64_t start_us,end_us;

		start_us = board_get_cur_us();
		//
		ad7991_init();
		ad7991_data[0] = 0;ad7991_data[1] = 0;ad7991_data[2] = 0;ad7991_data[3] = 0;
		ad7991_adc_read(ad7991, ad7991_data);	
		Room_Smoke = (uint8_t)(ad7991_data[0]/50);  
		//printf("MQ-2 Analog voltage is %f, send adta is %d\r\n",ad7991_data[0],  Room_Smoke);
		Room_Light = (uint8_t)(100.49058-0.03045 * ad7991_data[1]); 
		//printf("Photoresistance Analog voltage is %f, send adta is %d\r\n",ad7991_data[1],  Room_Light);
		//		
		if(Room_Lamp == 0) 
		{
			if(Room_Light < 50)	roomLamp_open();
			else if(Room_Light > 90) roomLamp_close();
		}
		else if(Room_Lamp == 1) roomLamp_open();
		else roomLamp_close();
		
		//
		if(Fan_switch == 0)
		{
			if((Room_PM25 > 135) | (Room_Smoke > 30) | (Room_Env_Tepm > 35)) fan_open();
			if((Room_PM25 < 110) & (Room_Smoke < 20) & (Room_Env_Tepm < 30) ) fan_close();
		}
		else if(Fan_switch == 1) fan_open();
		else  fan_close();
		//
		if(Room_Smoke > 30) buzzer_open();
		else buzzer_close();
		//
		if (Leave_home)
		{
			Body_infrared = (body_ir_read())?0x01:0x00;	
			if(Body_infrared) buzzer_open();
			else buzzer_close();
		}
		else Body_infrared = 0;


		end_us = board_get_cur_us();
		EMBARC_PRINTF("use %d us time \r\n",end_us - start_us);
	}
	return E_SYS;
}