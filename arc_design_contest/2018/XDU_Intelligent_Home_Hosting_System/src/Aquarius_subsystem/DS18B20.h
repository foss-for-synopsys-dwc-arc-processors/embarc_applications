#ifndef _DS18B20_H_
#define _DS18B20_H_

/* DS18B20 sensor object type*/
typedef struct {
	uint8_t temperature_int;
	uint8_t temperature_decimal;
	uint8_t temperature_sign;
} DS18B20_DEF, * DS18B20_DEF_PTR;

// extern void delay_us( uint32_t us);

extern uint8_t DS18b20_init(DEV_GPIO_PTR ds18b20_port,uint8_t ds18b20_bit);

extern uint8_t DS18b20_ReadOneChar(DEV_GPIO_PTR ds18b20_port,uint8_t ds18b20_bit);

extern void DS18b20_WriteOneChar(DEV_GPIO_PTR ds18b20_port,uint8_t ds18b20_bit, uint8_t data);

extern void DS18b20_ReadTemperature(DEV_GPIO_PTR ds18b20_port, uint8_t ds18b20_bit, DS18B20_DEF_PTR obj);

#endif