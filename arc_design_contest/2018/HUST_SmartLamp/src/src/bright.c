/*
 * bright.c
 *
 *  Created on: 2018年5月24日
 *      Author: Administrator
 */

#include "bright.h"
#include "cm3232.h"
#include "package.h"
#include "usart4task.h"

#include "string.h"
#include "stdlib.h"
//#include "sys.h"
#include "timer.h"


uint16_t compare_pre,compare_target;
bright_mode_enum bright_change_mode = LIGHT_SWITCH;
uint16_t auto_cm3232_count;//cm3232积分需要时间
uint8_t autobright_direction;
TimerHandle_t period_Bright_Handle;

//三档180
uint16_t compare_set[9] = {0,1,2,3,4,5,6,8,10};//存储调光的数据

//控制灯光亮度
//mode:0 直接调光到指定档位
//1 进行自动调光
//2 睡眠调光
void bright_mode(bright_mode_enum mode,uint8_t light)
{
	switch(mode)
	{
		case LIGHT_SWITCH:bright_switch_process(light);
			break;
		case LIGHT_DIRECT:bright_direct_process(light);
			break;
		case LIGHT_AUTO:bright_auto_process(light);
			break;
		case LIGHT_SLEEP:
			break;
		default:break;
	}
}

//1ms调节一次
#define TIME_PER Bright_TICKS
//每个周期调节占空比 1
#define COMPARE_PER 1
void bright_switch_process(uint8_t light)
{
	xTimerStop(period_Bright_Handle,10);
	bright_change_mode  = LIGHT_SWITCH;
	compare_pre = compare_pwm;
	compare_target = compare_set[light];
	xTimerStart(period_Bright_Handle,10);
}

//直接调节到对应档位，不渐变
void bright_direct_process(uint8_t light)
{
	xTimerStop(period_Bright_Handle,10);
	bright_change_mode  = LIGHT_DIRECT;
	compare_pwm = compare_set[light];

}

//自动调光
#define AUTO_COMPARE_MAX 0x9500
#define AUTO_COMPARE_MIN 0x6000
void bright_auto_process(uint8_t light)
{
	(void)light;
	xTimerStop(period_Bright_Handle,10);
	bright_change_mode  = LIGHT_AUTO;
	auto_cm3232_count = 0;
	autobright_direction=0;
	xTimerStart(period_Bright_Handle,10);
}

void period_Bright_Callback(TimerHandle_t xTimer)
{
	switch (bright_change_mode)
	{
		case LIGHT_SWITCH:
		{
			compare_pre = compare_pwm;
			if(compare_target<compare_pre)
			{
				compare_pre-=COMPARE_PER;
				compare_pwm = compare_pre;
			}
			else if(compare_target>compare_pre)
			{
				compare_pre+=COMPARE_PER;
				compare_pwm = compare_pre;
			}
			else//调节到目标占空比
			{
				xTimerStop(period_Bright_Handle,0);
			}
		}
			break;

		case LIGHT_DIRECT:
			break;
		case LIGHT_AUTO :
		{
			if(auto_cm3232_count%2)
			{
//			if(!(auto_cm3232_count%cm3232_time[1][CM3232_TIME]))
			{
				uint32_t cm3232_light = CM3232_Read(0x50);
				auto_cm3232_count = 0;
//				dprintf("tim cm3232 %4x\r\n",cm3232_light);
				if(cm3232_light>AUTO_COMPARE_MAX)
				{
					autobright_direction=2;
				}
				else if(cm3232_light<AUTO_COMPARE_MIN)
				{
					autobright_direction=1;
				}
				else
				{
					autobright_direction=0;
				}
			}
//			if(!(auto_cm3232_count%10))//调光
			{
				compare_pre=compare_pwm;

				if(autobright_direction==1)//增大
				{
					if(++compare_pre>compare_set[8])	compare_pre=compare_set[8];
					brightchange_send2server_touch(compare_pre);
				}
				else if(autobright_direction==2)//调小
				{
					if(compare_pre)
					{
						if((--compare_pre)==0) compare_pre=0;
					}
					brightchange_send2server_touch(compare_pre);
				}
				else//不调
				{

				}
			}
			}
			auto_cm3232_count++;
		}
		break;
		case LIGHT_SLEEP:
		{

		}
		break;
		default:break;
	}

}

//检测占空比变化是否要进行档位切换,发给服务器和触控板
void brightchange_send2server_touch(uint32_t compare_temp)
{
	uint8_t i,touch_temp = 0xff;

	compare_pwm = compare_temp;
	for(i=0;i<9;i++)
	{
		if(compare_temp==compare_set[i])
		{
			touch_temp = i;
			break;
		}
	}
	if(touch_temp!=0xff)
	{
			if(touch_temp!=(touch_flag&0x000f))
			{
//				dprintf("touch_temp = %d\r\n",touch_temp);
					if(touch_temp)
					{
						if(touch_flag&(0x0001<<14))//联网状态
						{
						//控制触控板到几档
						mcu_sendcmd2touch(0xf0|((touch_temp&0x0f)-1));
						}
						else
						{
						//控制触控板到几档
						mcu_sendcmd2touch(0xe0|((touch_temp&0x0f)-1));
						}
					}
					else//0档 不控制wifi灯
					{
						mcu_sendcmd2touch(0xea);
					}
				touch_flag&=0xfff0;
				touch_flag|=(touch_temp&0x0f);//记录灯光档位
				//发给服务器
				//发送0002
				sendpack2server(0x0002);
//					dprintf("timer is complete %d\r\n",i);
			}
	}
}

//end bright.c

