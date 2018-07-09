#include "dev_gpio.h"
/* DHT11 sensor object type*/
typedef struct {
	uint8_t RH_data;
	uint8_t T_data;
} DHT11_DEF, * DHT11_DEF_PTR;

extern void DHT11_Read_Data(DEV_GPIO_PTR DHT11_port, uint32_t DHT11_bit, DHT11_DEF_PTR data);

