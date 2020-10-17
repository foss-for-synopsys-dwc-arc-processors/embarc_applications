#include "ds1302.h"

DEV_GPIO * port_rtc;

uint32_t rtc_bit_out(uint32_t num, uint32_t bit)
{
	port_rtc->gpio_open(0x01 << num);
	port_rtc->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, CONV2VOID(0x01 << num));
	port_rtc->gpio_write(bit << num, 0x01 << num);

	return bit;
}

uint32_t rtc_bit_in(uint32_t num)
{
	uint32_t bit;

	port_rtc->gpio_open(0x00);
	port_rtc->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, CONV2VOID(0x01 << num));
	port_rtc->gpio_read(&bit, 0x01 << num);

	// return (bit >> num);
	return ((bit & (0x01 << num)) >> num);
}

void rtc_init(void)
{
	port_rtc = gpio_get_dev(RTC_PORT_ID);
	port_rtc->gpio_open(0xe0000);   // default direction of 3 pins are output
	rtc_bit_out(RTC_PIN_RST, 0);
	rtc_bit_out(RTC_PIN_CLK, 0);
}

void rtc_write_byte(uint8_t addr, uint8_t data)
{
	uint8_t i;

	addr = addr & 0xfe;     // set lsb to 0 for write mode

	// rtc_bit_out(RTC_PIN_RST, 0);
	// rtc_bit_out(RTC_PIN_CLK, 0);
	rtc_bit_out(RTC_PIN_RST, 1);

	/* write addr */
	for (i = 0; i < 8; i++) {
		if (addr & 0x01) {
			rtc_bit_out(RTC_PIN_DAT, 1);
		} else{
			rtc_bit_out(RTC_PIN_DAT, 0);
		}
		rtc_bit_out(RTC_PIN_CLK, 1);
		rtc_bit_out(RTC_PIN_CLK, 0);
		addr = addr >> 1;
	}

	/* write data */
	for (i = 0; i < 8; i++) {
		if (data & 0x01) {
			rtc_bit_out(RTC_PIN_DAT, 1);
		} else{
			rtc_bit_out(RTC_PIN_DAT, 0);
		}
		rtc_bit_out(RTC_PIN_CLK, 1);
		rtc_bit_out(RTC_PIN_CLK, 0);
		data = data >> 1;
	}

	rtc_bit_out(RTC_PIN_RST, 0);
}

uint8_t rtc_read_byte(uint8_t addr)
{
	uint8_t i;
	uint8_t data = 0;

	addr = addr | 0x01;     // set lsb to 1 for read mode

	// rtc_bit_out(RTC_PIN_RST, 0);
	// rtc_bit_out(RTC_PIN_CLK, 0);
	rtc_bit_out(RTC_PIN_RST, 1);

	/* write addr */
	for (i = 0; i < 8; i++) {
		if (addr & 0x01) {
			rtc_bit_out(RTC_PIN_DAT, 1);
		} else{
			rtc_bit_out(RTC_PIN_DAT, 0);
		}
		rtc_bit_out(RTC_PIN_CLK, 1);
		rtc_bit_out(RTC_PIN_CLK, 0);
		addr = addr >> 1;
	}

	/* read data */
	for (i = 0; i < 8; i++) {
		data = data >> 1;
		if (rtc_bit_in(RTC_PIN_DAT) == 1) {            //  how to read bit?
			data |= 0x80;
		} else if (rtc_bit_in(RTC_PIN_DAT) == 0) {
			data &= 0x7f;
		}
		rtc_bit_out(RTC_PIN_CLK, 1);
		rtc_bit_out(RTC_PIN_CLK, 0);
	}

	rtc_bit_out(RTC_PIN_RST, 0);


	return data;
}

void rtc_write_time(uint8_t *time_init)
{
	rtc_write_byte(DS1302_CONTROL_ADD,   0x00);			// turn off write protection
	rtc_write_byte(DS1302_SEC_ADD,       0x80);			// pause the clock
	// rtc_write_byte(DS1302_CHARGER_ADD,   0xa9);         // trickle charge
	rtc_write_byte(DS1302_YEAR_ADD,      time_init[1]);	// year (20xx)
	rtc_write_byte(DS1302_MONTH_ADD,     time_init[2]);	// month
	rtc_write_byte(DS1302_DATE_ADD,      time_init[3]);	// day
	rtc_write_byte(DS1302_HR_ADD,        time_init[4]);	// hour
	rtc_write_byte(DS1302_MIN_ADD,       time_init[5]);	// minute
	rtc_write_byte(DS1302_SEC_ADD,       time_init[6]);	// second
	rtc_write_byte(DS1302_DAY_ADD,       time_init[7]);	// day of week
	rtc_write_byte(DS1302_CONTROL_ADD,   0x80);			// turn on write protection
}

/*
 * original transfered data
 * each data in time_init is in BCD format
 * hex order: year(high 8-bit), year(low 8-bit), month, day, hour, min, sec, weekday
 */
void rtc_set_time(uint8_t *time_rcv)
{
	uint8_t i;
	uint8_t time_init[8] = {0x20, 0x20, 0x09, 0x11, 0x18, 0x27, 0x40, 0x06};    // BCD code

	rtc_write_time(time_init);
	time_rcv[0] = time_init[0];

	EMBARC_PRINTF("Initial time: ");
	for (i = 0; i < 8; i++) {
		EMBARC_PRINTF("%02X", time_init[i]);
		// EMBARC_PRINTF("%d", time_init[i]);
		if ((i > 0) && (i < 7)) {
			EMBARC_PRINTF(" - ");
		}
	}
}

void rtc_read_time(uint8_t *time_rcv)
{
	time_rcv[1] =  rtc_read_byte(DS1302_YEAR_ADD);		     // year
	time_rcv[2] =  rtc_read_byte(DS1302_MONTH_ADD);		     // month
	time_rcv[3] =  rtc_read_byte(DS1302_DATE_ADD);		     // day
	time_rcv[4] =  rtc_read_byte(DS1302_HR_ADD);		     // hour
	time_rcv[5] =  rtc_read_byte(DS1302_MIN_ADD);		     // minute
	time_rcv[6] = (rtc_read_byte(DS1302_SEC_ADD)) & 0x7f;  // second, disable msb in order not to exceed 59
	time_rcv[7] =  rtc_read_byte(DS1302_DAY_ADD);		     // day of week
}

void rtc_time_extract(uint8_t *time_rcv, uint8_t *time_extr)
{
	time_extr[0]  = (time_rcv[0] >> 4);       //分离出年千位
	time_extr[1]  = (time_rcv[0] &  0x0F);    //分离出年百位
	time_extr[2]  = (time_rcv[1] >> 4);       //分离出年十位
	time_extr[3]  = (time_rcv[1] &  0x0F);    //分离出年个位

	time_extr[4]  = (time_rcv[2] >> 4);       //分离出月十位
	time_extr[5]  = (time_rcv[2] &  0x0F);    //分离出月个位

	time_extr[6]  = (time_rcv[3] >> 4);       //分离出日十位
	time_extr[7]  = (time_rcv[3] &  0x0F);    //分离出日个位

	time_extr[8]  = (time_rcv[4] >> 4);       //分离出小时十位
	time_extr[9]  = (time_rcv[4] &  0x0F);    //分离出小时个位

	time_extr[10] = (time_rcv[5] >> 4);       //分离出分钟十位
	time_extr[11] = (time_rcv[5] &  0x0F);    //分离出分钟个位

	time_extr[12] = (time_rcv[6] >> 4);       //分离出秒钟十位
	time_extr[13] = (time_rcv[6] &  0x0F);    //分离出秒钟个位
	/* weekday don't needs to be extracted*/
}

/* dwin time hex format: {year(low), month, day, day_of_week, hour, minute, second, 0x00} */
void rtc_time_extract_hex(uint8_t *time_rcv, uint8_t *time_hex)
{
	time_hex[0] = 10 * (time_rcv[1] >> 4) + (time_rcv[1] & 0x0F);       // year(low)
	time_hex[1] = 10 * (time_rcv[2] >> 4) + (time_rcv[2] & 0x0F);       // month
	time_hex[2] = 10 * (time_rcv[3] >> 4) + (time_rcv[3] & 0x0F);       // day
	time_hex[3] = time_rcv[7] - 1;                                      // day of week, SUN  = 0 in dwin
	time_hex[4] = 10 * (time_rcv[4] >> 4) + (time_rcv[4] & 0x0F);       // hour
	time_hex[5] = 10 * (time_rcv[5] >> 4) + (time_rcv[5] & 0x0F);       // minute
	time_hex[6] = 10 * (time_rcv[6] >> 4) + (time_rcv[6] & 0x0F);       // second
	time_hex[7] = 0x00;
}

char *day_of_week(uint8_t day_num, char *day_string)
{
	switch (day_num) {
	case 1:
			day_string = "SUN";
			break;
	case 2:
			day_string = "MON";
			break;
	case 3:
			day_string = "TUR";
			break;
	case 4:
			day_string = "WED";
			break;
	case 5:
			day_string = "THU";
			break;
	case 6:
			day_string = "FRI";
			break;
	case 7:
			day_string = "SAT";
			break;
	}

	return day_string;
}
