/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#include "vm_task.h"

extern SemaphoreHandle_t vm_queue_mutex;
extern TaskHandle_t task_handle_communication;

void enQueue(vm_data data)
{
    BaseType_t ret;
    do{
        xSemaphoreTake(vm_queue_mutex, portMAX_DELAY);
        ret = xQueueSendToBack(xVMQueue, &data, 20);
        xSemaphoreGive(vm_queue_mutex);
    } while(ret != pdTRUE);
}


void vm_task(void *p_arg)
{
    BaseType_t ret;
    EMBARC_PRINTF("entering vm task\r\n");
    vm_data data = {0};

    // xVMQueue, xOledQueue, xCommunicationQueue, xTempQueue, xNumPadQueue, xDCmotorQueue
    // all tasks push the data to xVMQueue

    while (1)
    {
        //xSemaphoreTake(vm_queue_mutex, portMAX_DELAY);
        ret = xQueueReceive( xVMQueue, &data, portMAX_DELAY );
        //xSemaphoreGive(vm_queue_mutex);
        if (ret == pdTRUE)
        {
            switch (data.target_id)
            {
                case id_main:
                    xQueueSend(xMainQueue, &data, portMAX_DELAY);
                    break;
                case id_wifi:
                    xQueueSend(xCommunicationQueue, &data, portMAX_DELAY);
                    if (data.source_id == id_main) vTaskResume(task_handle_communication);
                    break;
                case id_temp:
                    xQueueSend(xTempQueue, &data, portMAX_DELAY);
                    // for wifi accessing to temperature
                    break;
                case id_dcmotor:
                    xQueueSend(xDCmotorQueue, &data, portMAX_DELAY);
                    break;
                case id_oled:
                    xQueueSend(xOledQueue, &data, portMAX_DELAY);
                    break;
            }
        }
    }
}
