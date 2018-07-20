#include "embARC.h"
#include "embARC_debug.h"

#include "dbgUart.h"
#include "cmd.h"
#include "TskMotor.h"

const int topTskPrio = 2;
const int topTskStkSize = 128;

void topTask(void *pvParameters)
{
    /** first initialize debug Task, print system information */
    dbgInit();
    /** then initialize motor Task */
    motorInit();
    vTaskDelay(50);

    putStr("TOP TIME:%s, %s\r\n",  __DATE__, __TIME__);
    vTaskDelay(100);

    while(1)
    {
        /** processing control command */
        paramCorr();
    }

}

void topInit()
{
	BaseType_t rtn;

    // Create tasks
    rtn=xTaskCreate(topTask, "topTask", topTskStkSize, NULL, topTskPrio, NULL);
    configASSERT(rtn==pdPASS);

}
