/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#define OBJECT_NOT_DROP 0
#define OBJECT_DROP     1

static DEV_GPIO *dc_motor;
static DEV_GPIO *infrared_ray;


void dcmotor_task(void *p_arg)
{
    vm_data dcmotor_data = {0};
    EMBARC_PRINTF("entering dc motor task\r\n");

    dc_motor = gpio_get_dev(DW_GPIO_PORT_C);
    //dc_motor->gpio_close(); /* opened before now close it */
    dc_motor->gpio_open(0x0f00); //only open pin1~4:motor1 portc[8]
    dc_motor->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(0x0f00));
    dc_motor->gpio_write(0x00, 0x0f00);

    infrared_ray = gpio_get_dev(DW_GPIO_PORT_A);	
	infrared_ray->gpio_open(0x0f00);
	infrared_ray->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(0x0f00));

    int motor = 0;
    bool product = OBJECT_NOT_DROP; 
	uint32_t motor_spining;
	while(1){
        if( xDCmotorQueue != 0 )	
        {
            if( xQueueReceive( xDCmotorQueue, &dcmotor_data, portMAX_DELAY ) )
            {
                motor = dcmotor_data.body[0].i;
                // EMBARC_PRINTF("=========================> motor receive from numpad: %d\r\n", motor);
            }
        }

        
            if(motor == 1)      dc_motor->gpio_write(0x0100, 0x0f00);
            else if(motor == 2) dc_motor->gpio_write(0x0200, 0x0f00);
            //else if(motor == 3) dc_motor->gpio_write(0x0400, 0x0f00);
            //else if(motor == 4) dc_motor->gpio_write(0x0800, 0x0f00);
            else                dc_motor->gpio_write(0x0000, 0x0f00);


        while(1){
            vTaskDelay(1);
            infrared_ray->gpio_read(&motor_spining, 0x0f00);
            // EMBARC_PRINTF("object %d\r\n", motor_spining && 0x0100);
                if(motor_spining && 0x0100) product = OBJECT_NOT_DROP; // no object drop
                else                        product = OBJECT_DROP;// object drop
            if (product == OBJECT_DROP) { 
                dc_motor->gpio_write(0x0000, 0x0f00);
                motor = 5;
                break;
            } 
        }

        vTaskDelay(1000);
 	}
}
