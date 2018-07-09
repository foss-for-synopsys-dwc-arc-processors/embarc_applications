#include "dbgUart.h"

void vApplicationAssert(const char *file, unsigned int line)
{
	putStr("fail in %s, line %d\r\n", file, line);
	vTaskDelay(100);
    while(1);
}