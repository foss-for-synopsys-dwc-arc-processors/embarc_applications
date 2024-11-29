/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"
#include "adt7420.h"
#include "vm_task.h"

static inline void _Temp_Enqueue(int target_id, float temp_receive) {
	vm_data _data = {0};
	
	_data.status    = temperature;
	_data.source_id = id_temp;
	_data.target_id = target_id;
	_data.body[0].f = temp_receive;
	enQueue(_data);
}

static ADT7420_DEF ADT7420_DEF_PTR_test = {
		.i2c_id = BOARD_TEMP_SENSOR_IIC_ID,
		.slvaddr = TEMP_I2C_SLAVE_ADDRESS
};

void temp_task(void *p_arg)
{
	EMBARC_PRINTF("entering temp task\r\n");

	int32_t ercd = 1;
	int cond = 0;
	float temp_receive = 0;
	char test[5] = {0};

	//board_init(); /* board level init */
	
	ercd = adt7420_sensor_init(&ADT7420_DEF_PTR_test);
	if (ercd != E_OK) {
		EMBARC_PRINTF("temperature sensor init failed. %d", ercd);
	}

	while(1){
		cond = adt7420_sensor_read(&ADT7420_DEF_PTR_test, &temp_receive);
		// EMBARC_PRINTF("temperature receive: %d \r\n", temp_receive);
		_Temp_Enqueue(id_temp, temp_receive);
		_Temp_Enqueue(id_oled, temp_receive);
		// board_delay_ms(500, 1);
		vTaskDelay(10000);
	}
}