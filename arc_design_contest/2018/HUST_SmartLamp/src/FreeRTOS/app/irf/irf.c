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
#include "app_config.h"
#include "task_config.h"
//
#include "string.h"
#include "stdlib.h"
//#include "sys.h"
#include "timer.h"

#include "embARC.h"

TaskHandle_t IRF_app_Handler;//
TaskHandle_t MP3_TASK_Handler;//
QueueHandle_t SuperSound_Queue;//控制MP3播放

#define adc_delay() delay_us(1)

#define EMSK_IRF_PORT		DEV_GPIO_PORT_A
#define GPIO_IRF_MASK		0x00080000		//portA的19Pin，接J3的10口
#define EMSK_GPIO_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)
DEV_GPIO * irf;
DEV_GPIO * svs1;
DEV_GPIO * adc;
DEV_GPIO * mp3;
//sound
supersound_struct supersound_ctr1;

//mp3

#define EMSK_GPIOMP3_PORT		DEV_GPIO_PORT_A
#define GPIO_GPIOMP3_MASK		0X0000F000		//portA的20-21Pin，接J4的7-8口
#define GPIO_GPIOMP3_LOW	  (0)
#define GPIO_GPIOMP3_HIGH	  (1)
#define GPIO_MP3_MASK1	0X00001000		//portA的12Pin，接J2的7口
#define GPIO_MP3_MASK2	0X00002000		//portA的13Pin，接J2的8口
#define GPIO_MP3_MASK3	0X00004000		//portA的14Pin，接J2的9口
#define GPIO_MP3_MASK4	0X00008000		//portA的15Pin，接J2的10口

//irf
STUDY_STRUCT study_ctr;

unsigned long ch1_temp[11],ch2_temp[11];

void mp3_init(void)
{
	io_pmod_config(2,0,0);		//初始化PMOD2配置，使PMOD2映射到AC口
	mp3 = gpio_get_dev(EMSK_GPIOMP3_PORT);
	EMSK_GPIO_CHECK_EXP_NORTN(mp3 != NULL);
	if (adc->gpio_open(GPIO_GPIOMP3_MASK) == E_OPNED)
	{
		adc->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_MP3_MASK1));	//SCL OUT
		adc->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_MP3_MASK2));	//DI out
		adc->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_MP3_MASK3));	//DO in
		adc->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_MP3_MASK4));	//DRY IN
		adc->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_GPIOMP3_MASK));
	}
	mp3->gpio_write((GPIO_GPIOMP3_MASK),GPIO_GPIOMP3_MASK);
	error_exit:
	return;
}



void mp3_play_in(uint8_t mp3_num)
{
	switch(mp3_num)
	{
		case 1:
			mp3->gpio_write((~GPIO_MP3_MASK1),GPIO_MP3_MASK1);
			vTaskDelay(200);
			mp3->gpio_write((GPIO_MP3_MASK1),GPIO_MP3_MASK1);
			break;
		case 2:
			mp3->gpio_write((~GPIO_MP3_MASK2),GPIO_MP3_MASK2);
			vTaskDelay(200);
			mp3->gpio_write((GPIO_MP3_MASK2),GPIO_MP3_MASK2);
			break;
		case 3:
			mp3->gpio_write((~GPIO_MP3_MASK3),GPIO_MP3_MASK3);
			vTaskDelay(200);
			mp3->gpio_write((GPIO_MP3_MASK3),GPIO_MP3_MASK3);
			break;
		case 4:
			mp3->gpio_write((~GPIO_MP3_MASK4),GPIO_MP3_MASK4);
			vTaskDelay(200);
			mp3->gpio_write((GPIO_MP3_MASK4),GPIO_MP3_MASK4);
			break;
	}
}

void mp3_play(uint8_t mp3_num)
{
#if 0
	mp3_play_in(mp3_num);
#else

	xQueueSend(SuperSound_Queue,&mp3_num,10);
#endif
}

void MP3_task(void *pvParameters)
{
	uint8_t mp3_temp;
	while(1)
	{
		if(xQueueReceive(SuperSound_Queue,&mp3_temp,0))
		{
			mp3_play_in(mp3_temp);
//			dprintf("mp3 task %d\r\n",mp3_temp);
			vTaskDelay(3000);
		}
		else
		{
			vTaskDelay(MP3_TASK_delay);
		}
	}
}


//adc
void supersound_ctr_init(supersound_struct*ctr)
{
	ctr->tim_start = 0;
	ctr->tim_stop = 0;
	ctr->flag = SOUND_FLAG_IDEL;
	ctr->count = 0;
	ctr->tim_result = 0;
	ctr->tim_ctr_start = 0;
	ctr->tim_ctr_stop = 0;
}
void study_ctr_init(void)
{
	study_ctr.up  = 3100;
	study_ctr.forward  = 2600;
	study_ctr.noperson = 3800;
//	study_ctr.nearby   = 0;
	study_ctr.ch1	   = 0;//up
	study_ctr.ch2	   = 0;//forward
	study_ctr.startstudy=0;
	study_ctr.time = 0;
	study_ctr.flag = 0;
	study_ctr.timeflag = 0;
}
//检测人体靠近 PB5
void irf_init(void)
{
#if 0
	irf = gpio_get_dev(EMSK_IRF_PORT);
	if (irf->gpio_open(GPIO_IRF_MASK) == E_OPNED)
	{
		irf->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(GPIO_IRF_MASK));
		irf->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_IRF_MASK));
	}
	irf->gpio_write(GPIO_IRF_MASK,GPIO_IRF_MASK);
#endif

#if 0
	//超声参数初始化
	supersound_ctr_init(&supersound_ctr1);
	//io初始化
	supersound_IOinit_1();
#endif
#if 1//adc方案
	study_ctr_init();
	adc_io_init();
	//MP3 IO 初始化
	mp3_init();
#endif

}

//**关灯一会儿后，才能再次触发IRF
#define IRF_TRI_LEVEL 2*5//2S

//80：标记是否开始检测人处于近距离
void IRF_app_task(void *pvParameters)
{
//	uint32_t io_temp;//
	TickType_t xLastWakeTime;
	uint8_t adc_count=0;
	uint8_t irf_flag = 0;
	uint8_t irf_noflag = 0;
	uint8_t study_error = 0;
	/* 获取当前的系统时间 */
  xLastWakeTime = xTaskGetTickCount();
	dprintf("IRF start\r\n");
//	vTaskDelay(2000);
	while(1)
	{
#if 0
		irf->gpio_read(&io_temp,GPIO_IRF_MASK);
//		io_temp = IRF_IN1;

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
//				irf_flag++;
//				if((irf_flag&0x7f)>IRF_TRI_LEVEL*2)//人为操作后6s,且关灯后，可以再次触发IRF
//				{
//					touch_flag&=(~(0x0001<<10));//清除操作记录
//				}
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


#endif
		//sound
#if 0
		switch(supersound_ctr1.flag)
		{
		case SOUND_FLAG_IDEL://开启测量
			if((supersound_ctr1.count++)%10==0)//10*100ms测试一次
			{
				supersound_tri_start_process_1();
			}
			break;

		case SOUND_FLAG_TRI_START:
			supersound_measure_falied_1();
//			printf("SOUND_FLAG_TRI_START\r\n");
			break;
		case SOUND_FLAG_PEDG_START://没有收到结束
			supersound_measure_falied_1();
//			printf("SOUND_FLAG_PEDG_START\r\n");
			break;

		case SOUND_FLAG_NEDG_STOP://完成测量
			supersound_measure_done_1();
			break;
		default:break;
		}
#endif
#if 0//sound
		if((supersound_ctr1.count++)%5==0)//10*100ms测试一次
		{
			supersound_ctr1.count = 1;
			supersound_tri_start_process_1();
			vTaskSuspendAll();
//			taskENTER_CRITICAL();
			while(1)
			{
				if(supersound_ctr1.flag==SOUND_FLAG_TRI_START)
				{
					if(supersound_echo_read_1())
					{
						supersound_ctr1.flag=SOUND_FLAG_PEDG_START;
						supersound_ctr1.tim_start = board_get_cur_us();
					}
				}
				else if(supersound_ctr1.flag==SOUND_FLAG_PEDG_START)
				{
					if(!supersound_echo_read_1())
					{
						supersound_ctr1.flag=SOUND_FLAG_NEDG_STOP;
						supersound_ctr1.tim_stop = board_get_cur_us();

						//done
						supersound_measure_done_1();
						break;

					}
				}

				if(supersound_ctr1.flag!=SOUND_FLAG_NEDG_STOP)
				{
					supersound_ctr1.tim_ctr_stop = board_get_cur_us();
					if(
							((supersound_ctr1.tim_ctr_stop-supersound_ctr1.tim_ctr_start)>8000))
					{
						supersound_measure_falied_1();
						break;
					}
				}



			}
			xTaskResumeAll();
//			taskEXIT_CRITICAL();
//			printf("sound: %d\r\n",supersound_ctr1.tim_result);
#if 1
			if(supersound_ctr1.flag==SOUND_FLAG_NEDG_STOP)
			{
				/*
				char say_str_temp[30];
				sprintf(say_str_temp,"ss=%d",supersound_ctr1.tim_result);
				say2server(say_did,say_str_temp);
				*/

				xQueueSend(SuperSound_Queue,&(supersound_ctr1.tim_result),10);
//				printf("sound %d\r\n",supersound_ctr1.tim_result);
			}

#endif

		}
#endif
#if 0//adc 测试
//		adc_count++;
//		delay_us(300);
//		dprintf("%d",adc_count);
//		vTaskSuspendAll();
		if(++adc_count%10==0)
		{
#if 0
			if(adc_count%20==0)
			{
				adc_count=0;
				dprintf("ch2=%d\r\n",ReadData2_7705());
			}
			else if(adc_count==10)
			{
				dprintf("ch1=%d\r\n",ReadData1_7705());
			}
#else
			adc_count=0;
//			vTaskDelay(400);
			study_ctr.ch1 = ReadData1_7705();
			vTaskDelay(100);
			study_ctr.ch2 = ReadData2_7705();
			printf("ch1=%d  ",study_ctr.ch1);
			printf("ch2=%d\r\n",study_ctr.ch2);
#endif

		}
//		xTaskResumeAll();


#endif
		if(!study_ctr.startstudy)
		{
			study_ctr.timeflag=0;
			study_ctr.time = 0;
		}

		if(study_ctr.startstudy||!(touch_flag&(0x0001<<7)))
		{
#if 1//study

			study_ctr.ch1 = ReadData1_7705();
			vTaskDelay(100);
			study_ctr.ch2 = ReadData2_7705();


		ch1_temp[adc_count] = study_ctr.ch1;
		ch2_temp[adc_count] = study_ctr.ch2;
		if(++adc_count>10)
		{
#if 1
			dprintf("ch1:");
			for(adc_count = 0;adc_count<11;adc_count++)
			{
				dprintf("%d\t",ch1_temp[adc_count]);
			}
			dprintf("\r\n");
			dprintf("ch2:");
			for(adc_count = 0;adc_count<11;adc_count++)
			{
				dprintf("%d\t",ch2_temp[adc_count]);
			}
			dprintf("\r\n");
#else
			printf("ch1=%d\tch2=%d\r\n",study_ctr.ch1 ,study_ctr.ch2);
#endif
			adc_count=0;
		}
		if(study_ctr.startstudy&&(touch_flag&(0x0001<<7)))
		{
//			dprintf("test 01\r\n");
//			touch_flag|=(0x0001<<10);
			if(study_check(1)==STUDY_FLAG_FORWARD)//错误坐姿
			{
//				dprintf("test 02\r\n");
				study_error++;
				irf_noflag=0;
//				dprintf("irf_flag %d\r\n",study_error);
				if(study_error>10)//IRF_TRI_LEVEL)
				{
//					dprintf("test 03\r\n");
					study_error = 0;
					study_ctr.flag = 1;
					study_ctr.time_pre = get_rtc_sec();
					dprintf("study error\r\n");
					mp3_play(2);

				}
			}
			else
			{
				if(study_ctr.timeflag==0)
				{
					study_ctr.time = get_rtc_sec();
					study_ctr.timeflag = 1;
				}

				if(get_rtc_sec()-study_ctr.time_pre==6) study_ctr.flag = 0;


//				dprintf("test 04\r\n");
				irf_noflag++;
				if(irf_noflag>3)
				{
					irf_noflag = 0;
					study_error = 0;
				}
			}

			if((study_ctr.timeflag)&&((get_rtc_sec()-study_ctr.time)>50)&&!((get_rtc_sec()-study_ctr.time)%60))
			{
				mp3_play(3);//休息
				dprintf("study time 60\r\n");
				vTaskDelay(1000);
			}

		}

#endif

#if 1//红外开灯

		//检测adc 判断是否在台灯附近  进行靠近开灯和远离关灯


		if(!(touch_flag&(0x0001<<10)))//没有人为操作
		{
			if(touch_flag&(0x0001<<7))//当前是开灯
			{
				if(study_check(0)==STUDY_FLAG_NOPERSON)//人离开
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
					if(study_check(0)==STUDY_FLAG_PERSON)//人靠近
					{
						irf_flag++;
						if((irf_flag&0x7f)>IRF_TRI_LEVEL)
						{
							irf_flag = 0;
							server_control_light(0x28);
							mp3_play(1);//欢迎使用
							study_ctr.startstudy = 1;
//							mcu_sendcmd2touch( Power_On);
//							touch_process_poweron();//开灯
							touch_flag&=(~(0x0001<<10));//清除操作记录
						}
					}
					else
					{
						irf_flag = 0;
					}
			}
		}
		else//人为操作了，开始学习模式
		{
			if(!(touch_flag&(0x0001<<7)))//当前是关灯
			{
//				irf_flag++;
//				if((irf_flag&0x7f)>IRF_TRI_LEVEL*2)//人为操作后6s,且关灯后，可以再次触发IRF
//				{
//					touch_flag&=(~(0x0001<<10));//清除操作记录
//				}
				if(study_check(0)==STUDY_FLAG_NOPERSON)//离开过
				{
					touch_flag&=(~(0x0001<<10));//清除操作记录
				}
			}
			else
			{
				irf_flag = 0;
			}
		}
#endif
		}
		else
		{
			study_ctr.time = 0;
			study_ctr.timeflag=0;

		}

//		printf("100ms = %d\r\n",board_get_cur_us());
		 vTaskDelayUntil(&xLastWakeTime, 200);
//		vTaskDelay(200);//100ms检测一次，不用太频繁
	}

}

#if 0
//
void supersound_exit_IRQHandler_1(void *ptr)
{
//	printf("i'm coming in\r\n");
	if(supersound_ctr1.flag==SOUND_FLAG_TRI_START)
	{
		supersound_ctr1.flag=SOUND_FLAG_PEDG_START;
		supersound_ctr1.tim_start = board_get_cur_us();
		supersound_exit_config_1(0);
	}
	else if(supersound_ctr1.flag==SOUND_FLAG_PEDG_START)
	{
		supersound_ctr1.flag=SOUND_FLAG_NEDG_STOP;
		supersound_ctr1.tim_stop = board_get_cur_us();
		supersound_exit_config_1(1);
	}
}

void supersound_tri_start_process_1(void)
{

	supersound_ctr1.tim_start = 0;
	supersound_ctr1.tim_stop  = 0;
	supersound_ctr1.flag=SOUND_FLAG_TRI_START;
	supersound_tri_config_1(1);
	vTaskDelay(1);
	supersound_tri_config_1(0);
#if 0
	supersound_exit_config_1(1);
#else
	supersound_ctr1.tim_ctr_start = board_get_cur_us();//控制测量时间
#endif

}



void supersound_measure_done_1(void)
{
//	supersound_measure_falied_1();
	if(supersound_ctr1.tim_stop>supersound_ctr1.tim_start)
	{
		supersound_ctr1.tim_result=supersound_ctr1.tim_stop-supersound_ctr1.tim_start;
//		printf("sound %d\r\n",supersound_ctr1.tim_result);
	}
//	else
//	{
//		supersound_ctr1.tim_result=supersound_ctr1.tim_stop+(0xffffffffffffffff-supersound_ctr1.tim_start);
//		printf("sound %d\r\n",supersound_ctr1.tim_result);
//	}
}

void supersound_measure_falied_1(void)
{
	supersound_ctr1.flag=SOUND_FLAG_IDEL;
	supersound_ctr1.count = 1;

	//关闭外部中断

}
#endif

#define EMSK_GPIOADC_PORT		DEV_GPIO_PORT_A
#define GPIO_GPIOADC_MASK		0X00F00000		//portA的20-21Pin，接J4的7-8口
#define GPIO_GPIOADC_LOW	  (0)
#define GPIO_GPIOADC_HIGH	  (1)
#define GPIO_SCL_MASK	0X00100000		//portA的20Pin，接J4的7口
#define GPIO_DI_MASK	0X00200000		//portA的21Pin，接J4的8口
#define GPIO_DO_MASK	0X00400000		//portA的22Pin，接J4的9口
#define GPIO_DRY_MASK	0X00800000		//portA的23Pin，接J4的10口


//DEV_GPIO_BIT_ISR bit_svs1_isr;
//DEV_GPIO_INT_CFG int_svs1_cfg;

//IO init
void adc_io_init(void)
{
	io_pmod_config(4,0,0);		//初始化PMOD4配置，使PMOD4映射到AC口
	adc = gpio_get_dev(EMSK_GPIOADC_PORT);
	EMSK_GPIO_CHECK_EXP_NORTN(adc != NULL);
	if (adc->gpio_open(GPIO_GPIOADC_MASK) == E_OPNED)
	{
		adc->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_SCL_MASK));	//SCL OUT
		adc->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_DI_MASK));	//DI out
		adc->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(GPIO_DO_MASK));	//DO in
		adc->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(GPIO_DRY_MASK));	//DRY IN
		adc->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_GPIOADC_MASK));
	}
	error_exit:
	return;


}

#if 0
void supersound_IOinit_1(void)
{
	io_pmod_config(4,0,0);

	svs1 = gpio_get_dev(EMSK_GPIOSVS1_PORT);

EMSK_GPIO_CHECK_EXP_NORTN(svs1 != NULL);
if (svs1->gpio_open(GPIO_SVS1_MASK) == E_OPNED) {
	svs1->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_SVS1OUT_MASK));
	svs1->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(GPIO_SVS1IN_MASK));
	svs1->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_SVS1_MASK));
}
int_svs1_cfg.int_bit_mask = GPIO_SVS1IN_MASK;
int_svs1_cfg.int_bit_type = GPIO_SVS1IN_MASK;
int_svs1_cfg.int_bit_polarity = GPIO_SVS1IN_MASK;//上升沿
int_svs1_cfg.int_bit_debounce = (~GPIO_SVS1IN_MASK);
svs1->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_svs1_cfg);
bit_svs1_isr.int_bit_ofs = 20;
bit_svs1_isr.int_bit_handler = supersound_exit_IRQHandler_1;
svs1->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_svs1_isr);
//svs1->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)GPIO_SVS1IN_MASK);	//不使能
svs1->gpio_write((~GPIO_SVS1OUT_MASK),GPIO_SVS1OUT_MASK);//默认低电平输出
int_pri_set(INTNO_GPIO, 30);
error_exit:
return;
	//tri 配置为输出IO 默认低电平

	//exit 配置为外部中断，先配置为上升沿触发 不使能

}

//读取echo的值，高为1 低为0
uint8_t supersound_echo_read_1(void)
{
	uint32_t temp_svs1;
	svs1->gpio_read(&temp_svs1, GPIO_SVS1IN_MASK);
	temp_svs1 = temp_svs1 & GPIO_SVS1IN_MASK;
	temp_svs1 = temp_svs1 >> 20;
	return (uint8_t)temp_svs1;
}

void supersound_exit_config_1(uint8_t edg)
{
#if 1
	svs1->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)GPIO_SVS1IN_MASK);//失能

	if(edg)//上升沿
	{
		int_svs1_cfg.int_bit_mask = GPIO_SVS1IN_MASK;
		int_svs1_cfg.int_bit_type = GPIO_SVS1IN_MASK;
		int_svs1_cfg.int_bit_polarity = GPIO_SVS1IN_MASK;//上升沿
		int_svs1_cfg.int_bit_debounce = (~GPIO_SVS1IN_MASK);
		svs1->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_svs1_cfg);
	}
	else//下降沿
	{
		int_svs1_cfg.int_bit_mask = GPIO_SVS1IN_MASK;
		int_svs1_cfg.int_bit_type = GPIO_SVS1IN_MASK;
		int_svs1_cfg.int_bit_polarity = 0;//下降沿
		int_svs1_cfg.int_bit_debounce = (~GPIO_SVS1IN_MASK);
		svs1->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_svs1_cfg);
	}

	svs1->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)GPIO_SVS1IN_MASK);//使能
#endif
}

void supersound_tri_config_1(uint8_t level)
{
#if 1
	if(level)//高电平
	{
		svs1->gpio_write(GPIO_SVS1OUT_MASK,GPIO_SVS1OUT_MASK);
	}
	else//低电平
	{
		svs1->gpio_write((~GPIO_SVS1OUT_MASK),GPIO_SVS1OUT_MASK);
	}
#endif
}
#endif
//adc驱动

/******向AD7705写入一个字节******/
void WriteByte7705(uint8_t dat)
{
	uint8_t i;
	for (i=0; i<8; i++)
	{
		adc_io_process(ADC_IO_SCL,0);
		adc_delay();
		if(dat & 0x80)
			{adc_io_process(ADC_IO_DI,1);}
		else
			adc_io_process(ADC_IO_DI,0);
		adc_delay();
		adc_io_process(ADC_IO_SCL,1);
		adc_delay();
		dat=dat<<1;
	}
}
/****** 从AD7705读一个字节 ******/
unsigned long ReadWord7705()
{
	unsigned long read_dat=0;
	unsigned char i;
	for(i=0;i<16;i++)
	{
		 read_dat=read_dat<<1;
		 adc_io_process(ADC_IO_SCL,0);
		 adc_delay();
		 if(adc_io_process(ADC_IO_DO,0)==1)
		 {read_dat++;}
		 adc_delay();
		 adc_io_process(ADC_IO_SCL,1);
		 adc_delay();
//		 vTaskDelay(5);
	 }
	 return read_dat;
}

/******ad7705通信端口复位******/
void reset7705()
{
	unsigned char i;
	adc_io_process(ADC_IO_DI,1);
	for(i=0;i<36;i++)
	{
		adc_io_process(ADC_IO_SCL,0);
		adc_delay();
		adc_io_process(ADC_IO_SCL,1);
		adc_delay();
	}
}

/******AD7705通道1初始化******/
void ad7705_init1()
{
	 reset7705();
	 WriteByte7705(0x20);//写通信寄存器,选择通道1,将下一次操作设为写时钟寄存器
	 WriteByte7705(0x01);//写时钟寄存器,不分频,更新频率25Hz
	 WriteByte7705(0x10);//写通信寄存器,选择通道1
	 WriteByte7705(0x44);//写设置寄存器4,单极性,非缓冲模式,清除滤波器同步,启动对1通道的自校准
}
/******AD7705通道2初始化******/
void ad7705_init2()
{
	 reset7705();
	 WriteByte7705(0x21);//写通信寄存器,选择通道2,将下一次操作设为写时钟寄存器
	 WriteByte7705(0x01);//写时钟寄存器,不分频,更新频率25Hz
	 WriteByte7705(0x11);//写通信寄存器,选择通道2
	 WriteByte7705(0x44);//写设置寄存器4,单极性,非缓冲模式,清除滤波器同步,启动对2通道的自校准
}

/******读取AD7705通道1转换数据函数******/
unsigned int ReadData1_7705()
{
	  unsigned int value;
	  uint8_t adc_while=0;
	  ad7705_init1();
	  reset7705();
		 for(adc_while=0;adc_while<30;adc_while++)
		 {
			 if(!adc_io_process(ADC_IO_DRY,0)) break;
	//		 vTaskDelay(10);
		 }
//	  while(adc_io_process(ADC_IO_DRY,0));        //等待转换结束
#if 0
	  {
//		  vTaskDelay(20);
		  adc_while++;
		  if(adc_while>250) break;
	  }
#endif
	  WriteByte7705(0x38);    //写通信寄存器,下一次操作为读数据寄存器
	  value=5000*ReadWord7705()/65535;
	  return value;
}
/******读取AD7705通道2转换数据函数******/
unsigned int ReadData2_7705()
{
	 unsigned int value;
	 uint8_t adc_while;
	 ad7705_init2();
	 reset7705();

	 for(adc_while=0;adc_while<30;adc_while++)
	 {
		 if(!adc_io_process(ADC_IO_DRY,0)) break;
//		 vTaskDelay(10);
	 }
//	 while(adc_io_process(ADC_IO_DRY,0));
		 //等待转换结束
#if 0
	  {
//		  vTaskDelay(20);
		  adc_while++;
		  if(adc_while>250) break;
	  }
#endif
	 WriteByte7705(0x39);//写通信寄存器,下一次操作为读数据寄存器
	 value=5000*ReadWord7705()/65535;
	 return value;
}

//IO set
uint8_t adc_io_process(ADC_IO_ENUM io,uint8_t highorlow)
{
	uint32_t temp_adc;


	switch(io)
	{
		case ADC_IO_SCL://out
			if(highorlow)
			{
				adc->gpio_write((GPIO_SCL_MASK), GPIO_SCL_MASK);
			}
			else
			{
				adc->gpio_write((~GPIO_SCL_MASK), GPIO_SCL_MASK);
			}
			break;
		case ADC_IO_DI://out
			if(highorlow)
			{
				adc->gpio_write((GPIO_DI_MASK), GPIO_DI_MASK);
			}
			else
			{
				adc->gpio_write((~GPIO_DI_MASK), GPIO_DI_MASK);
			}
			break;
		case ADC_IO_DO://in
			adc->gpio_read(&temp_adc, GPIO_DO_MASK);
			temp_adc = temp_adc & GPIO_DO_MASK;
			temp_adc = temp_adc >> 22;
			return (uint8_t)temp_adc;
			//get io level
			break;

		case ADC_IO_DRY://in
			adc->gpio_read(&temp_adc, GPIO_DRY_MASK);
			temp_adc = temp_adc & GPIO_DRY_MASK;
			temp_adc = temp_adc >> 23;
			return (uint8_t)temp_adc;
			//get io level
			break;

	}
	return 0;
}


#define AUTO_SWITCH_ADC 3810
STUDY_FLAG_ENUM study_check(uint8_t study)
{
	if(study_ctr.ch2<4200&&study_ctr.ch1<4200)
	{
		if(study)
		{
			if(/*study_ctr.ch2<study_ctr.forward||*/study_ctr.ch1<study_ctr.up)
				return STUDY_FLAG_FORWARD;
			else
				return STUDY_FLAG_RIGHT;
		}
		else//人来开 人走关
		{
			if(study_ctr.ch2<study_ctr.noperson)
				return STUDY_FLAG_PERSON;
			else
				return STUDY_FLAG_NOPERSON;
		}
		}
	else
	{
		return STUDY_FLAG_NOSENSE;
	}
}



//end irf.c

