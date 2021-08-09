#include "dht11.h"

DEV_GPIO * port_dht11;
uint8_t * dht11_pre;
uint8_t dht11_read_pre(uint8_t *dht11_data){
	dht11_data[0]=dht11_pre[0]; 
	dht11_data[1]=dht11_pre[1]; 
	dht11_data[2]=dht11_pre[2]; 
	dht11_data[3]=dht11_pre[3]; 
}

void dht11_init(void)
{
	port_dht11 = gpio_get_dev(DHT11_PORT_ID);
	port_dht11->gpio_open(0x01 << DHT11_PIN_DAT);     // default direction: output
}

uint32_t dht11_bit_out(uint32_t num, uint32_t bit)
{
	port_dht11->gpio_open(0x01 << num);
	port_dht11->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, CONV2VOID(0x01 << num));
	port_dht11->gpio_write(bit << num, 0x01 << num);

	return bit;
}

uint32_t dht11_bit_in(uint32_t num)
{
	uint32_t bit;

	port_dht11->gpio_open(0x01 << num);
	port_dht11->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, CONV2VOID(0x01 << num));
	port_dht11->gpio_read(&bit, 0x01 << num);

	return ((bit & (0x01 << num)) >> num);
}

uint8_t  dht11_read_byte(void)
{
	uint8_t i;
	uint8_t timeout_cnt;
	uint8_t rcv_temp;
	uint8_t rcv_data;

	for (i = 0; i < 8; i++) {
		timeout_cnt = 0;

		while (dht11_bit_in(DHT11_PIN_DAT) && timeout_cnt<100)
		{
			dht11_bit_in(DHT11_PIN_DAT);
			timeout_cnt++;	
			delay_us(1);
		}
		// chcek time out
		if (timeout_cnt>50)
		{
			printf("DHT11 ERROR0: read byte timeout !");
			return 0;
		}
		else timeout_cnt=0;

		/*
		 * if DHT11_PIN_DAT is been asserted for 26~28us
		 * means that DHT11 send a '0' to master
		 */
		while ((!(dht11_bit_in(DHT11_PIN_DAT))) && timeout_cnt<100) {
			dht11_bit_in(DHT11_PIN_DAT);
			timeout_cnt++;	
			delay_us(1);
		}

		// chcek time out
		if (timeout_cnt>100)
		{
			printf("DHT11 ERROR1: read byte timeout !");
			return 0;
		}
		else timeout_cnt=0;

		delay_us(40);

		rcv_temp = dht11_bit_in(DHT11_PIN_DAT);

		rcv_data <<= 1;
		rcv_data |= rcv_temp;
	}

	return rcv_data;
}

/*
 * input format: {humidity(integer), humidit(decimal),
 * 		temperature(integer), temperature(decimal), parity}
 * temperature range: 0~50°C
 * humidity range   : 20%~90%
 */
uint8_t dht11_read_data(uint8_t *dht11_data)
{
	uint8_t timeout_cnt;
	uint8_t parity;         // parity calculation
	uint8_t rcv_tmp_h;      // received integer data of temperature
	uint8_t rcv_tmp_l;      // received decimal data of temperature
	uint8_t rcv_hmd_h;      // received integer data of relative humidity
	uint8_t rcv_hmd_l;      // received decimal data of relative humidity
	uint8_t rcv_parity;     // received parity data
	uint16_t dht11_tmp;
	uint16_t dht11_hmd;
	uint8_t i;

	/*
	 * start transfer
	 * master must set DHT11_PIN_DAT to '0' for at least 18ms
	 * and set DHT11_PIN_DAT to '1' for 20~40us
	 * then, DHT11_PIN_DAT can be input port and master can receive data
	 */
	dht11_bit_out(DHT11_PIN_DAT,0);
	delay_ms(25);
	dht11_bit_out(DHT11_PIN_DAT,1);
	delay_us(30);


	// continue only when dht11 send respond '0'
	if (!dht11_bit_in(DHT11_PIN_DAT)) {
		// printf("DHT11 start!");
		timeout_cnt = 0;

		/*
		 * dht11 must set DHT11_PIN_DAT to '0' for 80us
		 * using timeout_cnt to check if delay time is more than 80us
		 */
		while (!dht11_bit_in(DHT11_PIN_DAT) && timeout_cnt<100) {
			dht11_bit_in(DHT11_PIN_DAT);
			timeout_cnt++;
			delay_us(1);
		}

		// chcek time out
		if (timeout_cnt>100)
		{
			printf("DHT11 ERROR0: read timeout !");
			return 0;
		}
		else timeout_cnt=0;

		/*
		 * dht11 must set DHT11_PIN_DAT to '1' for 80us
		 * using timeout_cnt to check if delay time is more than 80us
		 */
		while (dht11_bit_in(DHT11_PIN_DAT) && timeout_cnt<100) {
			dht11_bit_in(DHT11_PIN_DAT);
			timeout_cnt++;
			delay_us(1);
		}

		// chcek time out
		if (timeout_cnt>100)
		{
			printf("DHT11 ERROR1: read timeout!");
			return 0;
		}
		else timeout_cnt=0;


		// receive data from DHT11
		rcv_hmd_h  = dht11_read_byte();
		rcv_hmd_l  = dht11_read_byte();
		rcv_tmp_h  = dht11_read_byte();
		rcv_tmp_l  = dht11_read_byte();
		rcv_parity = dht11_read_byte();

		// enter idle state by setting DHT11_PIN_DAT to '1'
		dht11_bit_out(DHT11_PIN_DAT, 1);

		// output only when parity is correct
		parity = (rcv_tmp_h + rcv_tmp_l + rcv_hmd_h + rcv_hmd_l);
		if (parity == rcv_parity) {
			dht11_data[0] = rcv_hmd_h;
			dht11_data[1] = rcv_hmd_l;
			dht11_data[2] = rcv_tmp_h;
			dht11_data[3] = rcv_tmp_l;

			dht11_pre[0]=dht11_data[0]; 
			dht11_pre[1]=dht11_data[1]; 
			dht11_pre[2]=dht11_data[2]; 
			dht11_pre[3]=dht11_data[3]; 
		}
	}
	else return 0;

	return 1;
}
