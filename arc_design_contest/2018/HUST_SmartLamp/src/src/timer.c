/*
 * timer.c
 *
 *  Created on: 2018��5��9��
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
#define GPIO_PWM_MASK	0X00040000		//portA��19Pin����J3��9��

volatile static int t0 = 0;
volatile static int second = 0;

volatile unsigned long long FreeRTOSRunTimeTicks;
uint32_t RTC_SEC=0;//������ͬ��ʱ��
uint16_t rtc_1000ms=0;
uint16_t rtc_1ms=0;
uint32_t RTC_SEC2=0;
uint16_t compare_pwm=0;
uint16_t compare_now=0;
DEV_GPIO * gpio_pwm;
//��ʼ��TIM3ʹ��ΪFreeRTOS��ʱ��ͳ���ṩʱ��
//void ConfigureTimeForRunTimeStats(void)
//{
//	//��ʱ��3��ʼ������ʱ��ʱ��Ϊ72M����Ƶϵ��Ϊ72-1�����Զ�ʱ��3��Ƶ��
//	//Ϊ72M/72=1M���Զ���װ��Ϊ50-1����ô��ʱ�����ھ���50us
//	FreeRTOSRunTimeTicks=0;
//	TIM3_Int_Init(50-1,72-1);	//��ʼ��TIM3
//}

void wifi_tim7(void)
{
		TIM7_Int_Init();//��ʱ1ms
}


//��ʼ�����ж�ʱ��
void ALL_TIM_init(void)
{
	TIM7_Int_Init();
#if 0
	ConfigureTimeForRunTimeStats();//ϵͳFreeRTOS��ʱ��ͳ���ṩʱ��
	TIM7_Int_Init(750-1,480-1);//��ʱ1ms ���ڽ������ݳ�ʱ��� �ײ���պ�task��֡����ʱ�ж�
	TIM2_Int_Init(5000-1,7200-1);			//��ʼ����ʱ��2������500ms  Ԥ��RTCʱ��
#endif
}

//����ϵͳRTCʱ��
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
////ͨ�ö�ʱ��3�жϳ�ʼ��
////����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
////arr���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////����ʹ�õ��Ƕ�ʱ��3!
//void TIM3_Int_Init(uint16_t arr,uint16_t psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
//
//	//��ʱ��TIM3��ʼ��
//	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
//
//	//�ж����ȼ�NVIC����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�1��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
//
//	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
//}
//
////ͨ�ö�ʱ��2�жϳ�ʼ��
////����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
////arr���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////����ʹ�õ��Ƕ�ʱ��2!
//void TIM2_Int_Init(uint16_t arr,uint16_t psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
//
//	//��ʱ��TIM2��ʼ��
//	TIM_TimeBaseStructure.TIM_Period = arr; 			//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 			//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		//����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 			//ʹ��ָ����TIM2�ж�,��������ж�
//
//	//�ж����ȼ�NVIC����
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  	//TIM2�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 9;  //��ռ���ȼ�9��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 	//IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  					//��ʼ��NVIC�Ĵ���
//
//	TIM_Cmd(TIM2, ENABLE);  							//ʹ��TIM2
//}
//
void TIM7_Int_Init(void)
{
	//��ʼ��Ӳ����ʱ��
#if 0
	int_disable(INTNO_TIMER0);
	timer_stop(TIMER_0);
	int_handler_install(INTNO_TIMER0, timer0_isr);
	int_pri_set(INTNO_TIMER0, 8);
	int_enable(INTNO_TIMER0);
	timer_start(TIMER_0, TIMER_CTRL_IE | TIMER_CTRL_NH, COUNT);
#endif
	//��ʼ�������ʱ��

}
//
////��ʱ��3�жϷ�����
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
//	{
//		FreeRTOSRunTimeTicks++;
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
//}
//
////extern QueueHandle_t Message_Queue;	//��Ϣ���о��
//
//
////��ʱ��2�жϷ�����
//void TIM2_IRQHandler(void)
//{
//	#if 0
//	uint8_t *buffer;
//	BaseType_t xTaskWokenByReceive=pdFALSE;
//	BaseType_t err;
//
//	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //����ж�
//	{
//		buffer=mymalloc(USART_REC_LEN+15);
//        if(Message_Queue!=NULL)
//        {
//			memset(buffer,0,USART_REC_LEN);	//���������
//			err=xQueueReceiveFromISR(Message_Queue,buffer,&xTaskWokenByReceive);//������ϢMessage_Queue
//            if(err==pdTRUE)			//���յ���Ϣ
//            {
////				disp_str(buffer);	//��LCD����ʾ���յ�����Ϣ
//                printf("tim2process:%s\r\n",buffer);
//            }
//        }
//		myfree(buffer);		//�ͷ��ڴ�
//
//		portYIELD_FROM_ISR(xTaskWokenByReceive);//�����Ҫ�Ļ�����һ�������л�
//	}
//	#endif
//	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����жϱ�־λ
//
//}

//��ʱ��7�жϷ������
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
		//�ײ�������ݳ�ʱ
		if(usart1_time_out&0x80)
		{
			usart1_time_out++;
			if((usart1_time_out&0x7f)>9)
			{
				usart1_time_out = 0;
				usart1_data.len=USART_RX_STA&0x7fff;
				if(usart1_data.len)
				{
					xQueueSend(USART1_Queue,&usart1_data,0);//������з�������
					USART_RX_STA = 0;
					occupy_buff();
//					portYIELD_FROM_ISR();//�����Ҫ�Ļ�����һ�������л�
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
					xQueueSend(USART3_Queue,&usart3_data,0);//������з�������
					USART3_RX_STA = 0;
					occupy_buff3();
//					portYIELD_FROM_ISR();//�����Ҫ�Ļ�����һ�������л�
				}
			}
		}
#if 1
		//usart3task�����������ݳ�ʱ
		if(usart3_task_time_out&0x80)
		{
			usart3_task_time_out++;
			if((usart3_task_time_out&0x7f)>50)//�����֡ʧ�ܣ��������
			{
				usart3_task_time_out = 0;

			//�ͷŶ�ֵ�ź��� ��������ͬ��
			if(BinarySemaphore_free!=NULL)//���յ����ݣ����Ҷ�ֵ�ź�����Ч
			{
				xSemaphoreGive(BinarySemaphore_free);	//�ͷŶ�ֵ�ź���
//				portYIELD_FROM_ISR();//�����Ҫ�Ļ�����һ�������л�
			}
//				usart1_task_process = USART_DATA_IDEL;
//				vPortFree(usart1_task_temp.pstr);//���ж���free�������Ҫ���ⲿtask�н��в���
//				usart1_task_temp.type = USART_OTHER;
				//�������ݣ��ѵ�ǰ���ݴ��Ϊһ֡OTHER���ݷ��ͳ�ȥ��

			}
		}

//		USART3_RX_STA|=1<<15;	//��ǽ������
//		if(USART3_RX_BUF[20]==0x80&&USART3_RX_BUF[21]==0x03&&USART3_RX_BUF[22]==0x24)
//		{
//			light4_flag=1;
//			USART3_RX_STA=0;
//		}
#endif
//		timer_int_clear(TIMER_0);  //���TIM0�����жϱ�־
//		TIM_Cmd(TIM7, DISABLE);  //�ر�TIM7

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
		timer_int_clear(TIMER_1);  //���TIM0�����жϱ�־
}
#endif
