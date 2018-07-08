#ifndef _INC_TASK_H_
#define _INC_TASK_H_

/**
 * \brief       Task Dealy For a Period
 *
 * \param[in]   ticks                  ticks Need to Delay,1 ms Every Tick
 */

inline void _Rtos_Delay(uint32_t ticks)
{
	TickType_t xLastWakeTime;
	TickType_t xFrequency;

	xLastWakeTime = xTaskGetTickCount ();
	xFrequency = ticks;
	vTaskDelayUntil( &xLastWakeTime, xFrequency );
}

inline void _Block_Delay( uint32_t ticks )
{
	uint32_t cur_time;

	cur_time = OSP_GET_CUR_MS();

	while ((OSP_GET_CUR_MS() - cur_time) < ticks);
}



extern EventGroupHandle_t evt1_cb;		//Event For Music Task
extern EventGroupHandle_t GUI_Ev;		//Event For Gui Task


extern void music_task();				//Declare Music Task
extern void net_task();					//Declare Net Task
extern void gui_task();					//Declare Gui Task


#endif /*_INC_TASK_H_*/

