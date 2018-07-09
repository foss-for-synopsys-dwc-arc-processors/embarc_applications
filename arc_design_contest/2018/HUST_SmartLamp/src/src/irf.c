/*
 * irf.c
 *
 *  Created on: 2018年5月27日
 *      Author: Administrator
 */

#include "bright.h"
#include "irf.h"
#include "package.h"
#include "usart4task.h"

#include "string.h"
#include "stdlib.h"
#include "timer.h"


TaskHandle_t IRF_app_Handler;//

#define EMSK_IRF_PORT		DEV_GPIO_PORT_A
#define GPIO_IRF_MASK		0x00080000		//portA的19Pin，接J3的10口
DEV_GPIO * irf;
//检测人体靠近 PB5
void irf_init(void)
{
	irf = gpio_get_dev(EMSK_IRF_PORT);
	if (irf->gpio_open(GPIO_IRF_MASK) == E_OPNED)
	{
		irf->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(GPIO_IRF_MASK));
		irf->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_IRF_MASK));
	}
	irf->gpio_write(GPIO_IRF_MASK,GPIO_IRF_MASK);
}

//**关灯一会儿后，才能再次触发IRF
#define IRF_TRI_LEVEL 3*10//3S
uint8_t irf_flag = 0;
//80：标记是否开始检测人处于近距离
void IRF_app_task(void *pvParameters)
{
	uint32_t io_temp;
	TickType_t xLastWakeTime;
	/* 获取当前的系统时间 */
  xLastWakeTime = xTaskGetTickCount();
	dprintf("IRF start\r\n");
	while(1)
	{
		irf->gpio_read(&io_temp,GPIO_IRF_MASK);
		if(!(touch_flag&(0x0001<<10)))//没有人为操作
		{
			if(touch_flag&(0x0001<<7))//当前是开灯
			{
				if(io_temp)//人离开
				{
					irf_flag++;
					if((irf_flag&0x7f)>IRF_TRI_LEVEL)
					{
						irf_flag = 0;
						mcu_sendcmd2touch(Power_Off);
						touch_process_poweroff();//关灯
						touch_flag&=(~(0x0001<<10));//清除操作记录
					}
				}
				else
				{
					irf_flag = 0;
				}
			}
			else//当前是关灯
			{
					if(!io_temp)//人靠近
					{
						irf_flag++;
						if((irf_flag&0x7f)>IRF_TRI_LEVEL)
						{
							irf_flag = 0;
							mcu_sendcmd2touch( Power_On);
							touch_process_poweron();//开灯
							touch_flag&=(~(0x0001<<10));//清除操作记录
						}
					}
					else
					{
						irf_flag = 0;
					}
			}
		}
		else
		{
			if(!(touch_flag&(0x0001<<7)))//当前是关灯
			{
				if(io_temp)//离开过
				{
						touch_flag&=(~(0x0001<<10));//清除操作记录
				}
			}
			else
			{
				irf_flag = 0;
			}
		}
		 vTaskDelayUntil(&xLastWakeTime, 100);
	}
}
//end irf.c

