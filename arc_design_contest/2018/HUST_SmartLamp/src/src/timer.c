/*
 * timer.c
 *
 *  Created on: 2018年5月9日
 *      Author: Administrator
 */

#include "timer.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "usart1task.h"
#include "usart3task.h"

#include "embARC.h"
#include "embARC_debug.h"


#define COUNT2 (BOARD_CPU_CLOCK/1000)
#define EMSK_PWM_PORT		DEV_GPIO_PORT_A
#define GPIO_PWM_MASK	0X00040000		//portA的19Pin，接J3的9口

volatile static int t0 = 0;
volatile static int second = 0;

volatile unsigned long long FreeRTOSRunTimeTicks;
uint32_t RTC_SEC=0;//服务器同步时间
uint16_t rtc_1000ms=0;
uint16_t rtc_1ms=0;
uint32_t RTC_SEC2=0;
uint16_t compare_pwm=0;
uint16_t compare_now=0;
DEV_GPIO * gpio_pwm;
//初始化TIM3使其为FreeRTOS的时间统计提供时基
//void ConfigureTimeForRunTimeStats(void)
//{
//	//定时器3初始化，定时器时钟为72M，分频系数为72-1，所以定时器3的频率
//	//为72M/72=1M，自动重装载为50-1，那么定时器周期就是50us
//	FreeRTOSRunTimeTicks=0;
//	TIM3_Int_Init(50-1,72-1);	//初始化TIM3
//}

void wifi_tim7(void)
{
		TIM7_Int_Init();//定时1ms
}


//初始化所有定时器
void ALL_TIM_init(void)
{
	TIM7_Int_Init();
#if 0
	ConfigureTimeForRunTimeStats();//系统FreeRTOS的时间统计提供时基
	TIM7_Int_Init(750-1,480-1);//定时1ms 串口接收数据超时检测 底层接收和task分帧处理超时判断
	TIM2_Int_Init(5000-1,7200-1);			//初始化定时器2，周期500ms  预计RTC时钟
#endif
}

//设置系统RTC时间
void set_rtc_sec(uint32_t rtc_time)
{
	RTC_SEC = rtc_time;
	rtc_1000ms=0;
}
uint32_t get_rtc_sec(void)
{
	return RTC_SEC;
}

void delay_us(uint32_t us)
{

	uint64_t start_us, us_delayed;
	us_delayed = (uint64_t)us;
	start_us = board_get_cur_us();
//	taskENTER_CRITICAL();
	while ((board_get_cur_us() - start_us) < us_delayed);
//	taskEXIT_CRITICAL();
}
//
////通用定时器3中断初始化
////这里时钟选择为APB1的2倍，而APB1为36M
////arr：自动重装值。
////psc：时钟预分频数
////这里使用的是定时器3!
//void TIM3_Int_Init(uint16_t arr,uint16_t psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
//
//	//定时器TIM3初始化
//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
//
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
//
//	//中断优先级NVIC设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级1级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
//
//	TIM_Cmd(TIM3, ENABLE);  //使能TIMx
//}
//
////通用定时器2中断初始化
////这里时钟选择为APB1的2倍，而APB1为36M
////arr：自动重装值。
////psc：时钟预分频数
////这里使用的是定时器2!
//void TIM2_Int_Init(uint16_t arr,uint16_t psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
//
//	//定时器TIM2初始化
//	TIM_TimeBaseStructure.TIM_Period = arr; 			//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			//设置用来作为TIMx时钟频率除数的预分频值
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		//根据指定的参数初始化TIMx的时间基数单位
//
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 			//使能指定的TIM2中断,允许更新中断
//
//	//中断优先级NVIC设置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  	//TIM2中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;  //先占优先级9级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	//IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  					//初始化NVIC寄存器
//
//	TIM_Cmd(TIM2, ENABLE);  							//使能TIM2
//}
//
void TIM7_Int_Init(void)
{
	//初始化硬件定时器
#if 0
	int_disable(INTNO_TIMER0);
	timer_stop(TIMER_0);
	int_handler_install(INTNO_TIMER0, timer0_isr);
	int_pri_set(INTNO_TIMER0, 8);
	int_enable(INTNO_TIMER0);
	timer_start(TIMER_0, TIMER_CTRL_IE | TIMER_CTRL_NH, COUNT);
#endif
	//初始化软件定时器

}
//
////定时器3中断服务函数
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
//	{
//		FreeRTOSRunTimeTicks++;
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
//}
//
////extern QueueHandle_t Message_Queue;	//信息队列句柄
//
//
////定时器2中断服务函数
//void TIM2_IRQHandler(void)
//{
//	#if 0
//	uint8_t *buffer;
//	BaseType_t xTaskWokenByReceive=pdFALSE;
//	BaseType_t err;
//
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
//	{
//		buffer=mymalloc(USART_REC_LEN+15);
//        if(Message_Queue!=NULL)
//        {
//			memset(buffer,0,USART_REC_LEN);	//清除缓冲区
//			err=xQueueReceiveFromISR(Message_Queue,buffer,&xTaskWokenByReceive);//请求消息Message_Queue
//            if(err==pdTRUE)			//接收到消息
//            {
////				disp_str(buffer);	//在LCD上显示接收到的消息
//                printf("tim2process:%s\r\n",buffer);
//            }
//        }
//		myfree(buffer);		//释放内存
//
//		portYIELD_FROM_ISR(xTaskWokenByReceive);//如果需要的话进行一次任务切换
//	}
//	#endif
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
//
//}

//定时器7中断服务程序
void timer0_isr(TimerHandle_t xTimer)
{
	BaseType_t xHigherPriorityTaskWoken;


		//rtc_1000ms
		rtc_1000ms++;

		if(rtc_1000ms>999)
		{
			rtc_1000ms = 0;
			RTC_SEC++;
//			printf("%d\r\n",RTC_SEC);
		}
#if 1

		compare_now++;
			if(compare_now<10)
			{
				if(compare_pwm==compare_now)
				{
					gpio_pwm->gpio_write((~GPIO_PWM_MASK),GPIO_PWM_MASK);//			gpio low;
				}
			}
			else
			{
				compare_now = 0;
				if(compare_pwm)
				{
					gpio_pwm->gpio_write(GPIO_PWM_MASK,GPIO_PWM_MASK);//			gpio high;
				}
				else
				{
					gpio_pwm->gpio_write((~GPIO_PWM_MASK),GPIO_PWM_MASK);//			gpio low;
				}

			}


#endif
#if 1
		//底层接收数据超时
		if(usart1_time_out&0x80)
		{
			usart1_time_out++;
			if((usart1_time_out&0x7f)>9)
			{
				usart1_time_out = 0;
				usart1_data.len=USART_RX_STA&0x7fff;
				if(usart1_data.len)
				{
					xQueueSend(USART1_Queue,&usart1_data,0);//向队列中发送数据
					USART_RX_STA = 0;
					occupy_buff();
//					portYIELD_FROM_ISR();//如果需要的话进行一次任务切换
				}
			}
		}
#endif
		if(usart3_time_out&0x80)
		{
			usart3_time_out++;
			if((usart3_time_out&0x7f)>9)
			{
				usart3_time_out = 0;
				usart3_data.len=(uint8_t)(USART3_RX_STA&0x7fff);
				if(usart3_data.len)
				{
					xQueueSend(USART3_Queue,&usart3_data,0);//向队列中发送数据
					USART3_RX_STA = 0;
					occupy_buff3();
//					portYIELD_FROM_ISR();//如果需要的话进行一次任务切换
				}
			}
		}
#if 1
		//usart3task接收连续数据超时
		if(usart3_task_time_out&0x80)
		{
			usart3_task_time_out++;
			if((usart3_task_time_out&0x7f)>50)//跨包组帧失败，清除数据
			{
				usart3_task_time_out = 0;

			//释放二值信号量 进行任务同步
			if(BinarySemaphore_free!=NULL)//接收到数据，并且二值信号量有效
			{
				xSemaphoreGive(BinarySemaphore_free);	//释放二值信号量
//				portYIELD_FROM_ISR();//如果需要的话进行一次任务切换
			}
//				usart1_task_process = USART_DATA_IDEL;
//				vPortFree(usart1_task_temp.pstr);//在中断中free会出错，需要到外部task中进行操作
//				usart1_task_temp.type = USART_OTHER;
				//丢弃数据（把当前数据打包为一帧OTHER数据发送出去）

			}
		}

//		USART3_RX_STA|=1<<15;	//标记接收完成
//		if(USART3_RX_BUF[20]==0x80&&USART3_RX_BUF[21]==0x03&&USART3_RX_BUF[22]==0x24)
//		{
//			light4_flag=1;
//			USART3_RX_STA=0;
//		}
#endif
//		timer_int_clear(TIMER_0);  //清除TIM0更新中断标志
//		TIM_Cmd(TIM7, DISABLE);  //关闭TIM7

}

void pwm_gpio_int(void)
{
	gpio_pwm = gpio_get_dev(EMSK_PWM_PORT);
	if (gpio_pwm->gpio_open(GPIO_PWM_MASK) == E_OPNED) {
		gpio_pwm->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_PWM_MASK));
		gpio_pwm->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_PWM_MASK));
	}
	gpio_pwm->gpio_write((~GPIO_PWM_MASK),GPIO_PWM_MASK);
}
#if 0
void timertest_int(void)
{
	gpio_pwm = gpio_get_dev(EMSK_PWM_PORT);

//	EMSK_GPIO_CHECK_EXP_NORTN(gpio_pwm != NULL);
//printf("11\r\n");
	if (gpio_pwm->gpio_open(GPIO_PWM_MASK) == E_OPNED) {
		gpio_pwm->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_PWM_MASK));
		gpio_pwm->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_PWM_MASK));
	}
	gpio_pwm->gpio_write((~GPIO_PWM_MASK),GPIO_PWM_MASK);
	int_disable(INTNO_TIMER1);
	timer_stop(TIMER_1);
	int_handler_install(INTNO_TIMER1, timertest_isr);
	int_pri_set(INTNO_TIMER1, 8);
	int_enable(INTNO_TIMER1);
	timer_start(TIMER_1, TIMER_CTRL_IE | TIMER_CTRL_NH, COUNT2);
}


void timertest_isr(void)
{
//	BaseType_t xHigherPriorityTaskWoken;


	compare_now++;
	if(compare_now<10)
	{
		if(compare_pwm==compare_now)
		{
			gpio_pwm->gpio_write((~GPIO_PWM_MASK),GPIO_PWM_MASK);//			gpio low;
		}
	}
	else
	{
		compare_now = 0;
		if(compare_pwm)
		{
			gpio_pwm->gpio_write(GPIO_PWM_MASK,GPIO_PWM_MASK);//			gpio high;
		}
		else
		{
			gpio_pwm->gpio_write((~GPIO_PWM_MASK),GPIO_PWM_MASK);//			gpio low;
		}

	}



		//rtc_1000ms
	rtc_1ms++;

		if(rtc_1ms>999)
		{
			rtc_1ms = 0;
			RTC_SEC2++;
//			if(RTC_SEC2>999)
//			{
//			printf("%d\r\n",RTC_SEC2);
//			RTC_SEC2=0;
//			}
		}
		timer_int_clear(TIMER_1);  //清除TIM0更新中断标志
}
#endif
