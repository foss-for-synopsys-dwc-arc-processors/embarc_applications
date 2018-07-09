/*
 * irf.c
 *
 *  Created on: 2018��5��27��
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
#define GPIO_IRF_MASK		0x00080000		//portA��19Pin����J3��10��
DEV_GPIO * irf;
//������忿�� PB5
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

//**�ص�һ����󣬲����ٴδ���IRF
#define IRF_TRI_LEVEL 3*10//3S
uint8_t irf_flag = 0;
//80������Ƿ�ʼ����˴��ڽ�����
void IRF_app_task(void *pvParameters)
{
	uint32_t io_temp;
	TickType_t xLastWakeTime;
	/* ��ȡ��ǰ��ϵͳʱ�� */
  xLastWakeTime = xTaskGetTickCount();
	dprintf("IRF start\r\n");
	while(1)
	{
		irf->gpio_read(&io_temp,GPIO_IRF_MASK);
		if(!(touch_flag&(0x0001<<10)))//û����Ϊ����
		{
			if(touch_flag&(0x0001<<7))//��ǰ�ǿ���
			{
				if(io_temp)//���뿪
				{
					irf_flag++;
					if((irf_flag&0x7f)>IRF_TRI_LEVEL)
					{
						irf_flag = 0;
						mcu_sendcmd2touch(Power_Off);
						touch_process_poweroff();//�ص�
						touch_flag&=(~(0x0001<<10));//���������¼
					}
				}
				else
				{
					irf_flag = 0;
				}
			}
			else//��ǰ�ǹص�
			{
					if(!io_temp)//�˿���
					{
						irf_flag++;
						if((irf_flag&0x7f)>IRF_TRI_LEVEL)
						{
							irf_flag = 0;
							mcu_sendcmd2touch( Power_On);
							touch_process_poweron();//����
							touch_flag&=(~(0x0001<<10));//���������¼
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
			if(!(touch_flag&(0x0001<<7)))//��ǰ�ǹص�
			{
				if(io_temp)//�뿪��
				{
						touch_flag&=(~(0x0001<<10));//���������¼
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

