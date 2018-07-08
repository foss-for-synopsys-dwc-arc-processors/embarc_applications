/**
 * API to Get Performance Data
 */

#include "embARC.h"
#include "embARC_debug.h"

#include "include.h"



uint32_t cost_cyc;
static volatile uint32_t start = 0;

/** performance timer start */
static inline void perf_start(void)
{
	start = xTaskGetTickCount();
}

/** performance timer end, and return the time passed */
static inline uint32_t perf_end(void)
{
	return xTaskGetTickCount() - start;
}