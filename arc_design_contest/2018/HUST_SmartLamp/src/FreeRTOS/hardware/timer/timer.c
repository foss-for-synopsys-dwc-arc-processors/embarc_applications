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
#include "usart4task.h"
#include "bright.h"
#include "fifo.h"

#include "app_config.h"

#include "embARC.h"
#include "embARC_debug.h"


#define COUNT2 (BOARD_CPU_CLOCK/1000)
#define COUNT (BOARD_CPU_CLOCK/(1000*Bright_bei))

#define EMSK_PWM_PORT		DEV_GPIO_PORT_A
#define GPIO_PWM_MASK	0X00040000		//portA��19Pin����J3��9��

#define HARD_TIME1 1

volatile static int t0 = 0;
volatile static int second = 0;
uint32_t temp_re =0;

volatile unsigned long long FreeRTOSRunTimeTicks;
uint32_t RTC_SEC=0;//������ͬ��ʱ��
uint16_t rtc_1000ms=0;
uint16_t rtc_1ms=0;
uint32_t RTC_SEC2=0;
uint16_t compare_pwm=0;
uint16_t compare_now=0;
DEV_GPIO * gpio_pwm;
uint32_t secsoft = 0;
uint16_t cnt_sec = 0;
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
	uint8_t i;
	TIM7_Int_Init();
	//��ʼ����λ��Ϣ
//	for(i=0;i<9;i++)
//	{
//		compare_set[i] = Bright_bei*compare_set[i];
//	}
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

//
void TIM7_Int_Init(void)
{
	//��ʼ��Ӳ����ʱ��
#if 0
	int_disable(INTNO_TIMER1);
	timer_stop(TIMER_1);
	int_handler_install(INTNO_TIMER1, timer0_isr);
	int_pri_set(INTNO_TIMER1, 32);
	int_enable(INTNO_TIMER1);
	timer_start(TIMER_1, TIMER_CTRL_IE | TIMER_CTRL_NH, COUNT);
#endif
	//��ʼ�������ʱ��

}

void timer0_isr(void *ptr)
{
#if 0
	secsoft++;
	if(secsoft==10000)
	{
		cnt_sec++;
		printf("%d\r\n",cnt_sec);
		secsoft=0;
	}
#endif
#if HARD_TIME1


	compare_now++;
		if(compare_now<compare_set[8])
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
//send
#if 1
		//suart4 send fifo
		if(usart4_send_fifo.now_len&&(!send_ctr4.send_flag))
		{
			//read send fifo get 1 byte
			read_fifo(&usart4_send_fifo,&(send_ctr4.byte_now),1);
			send_ctr4.send_flag=1;
			send_ctr4.bit_flag=0;
		}
		//suart4 send
		if(send_ctr4.send_flag&&(!(send_ctr4.tim_count%(6))))
		{
			if(send_ctr4.bit_flag==0)
			{
				uart4->gpio_write((~GPIO_UART4OUT_MASK), GPIO_UART4OUT_MASK);
			}
			else if(send_ctr4.bit_flag<9)
			{
				uart4->gpio_write((((send_ctr4.byte_now>>(send_ctr4.bit_flag-1))&0x01)<<17), GPIO_UART4OUT_MASK);
			}
			else //if(send_ctr4.bit_flag==9)
			{
				uart4->gpio_write(GPIO_UART4OUT_MASK, GPIO_UART4OUT_MASK);
//				send_ctr4.bit_flag=0;
				send_ctr4.send_flag=0;
			}
			send_ctr4.bit_flag++;

		}
		send_ctr4.tim_count++;
		if(send_ctr4.tim_count>59) send_ctr4.tim_count = 0;
#endif
#if 1//receive

		if(uart4_ctr.flag)
		{
			if(uart4_ctr.count>(8)&&uart4_ctr.count<(52))//9 15 21 27 33 39 45 51
			{
				if(!(((uart4_ctr.count)%(6))-3))
//				if(uart4_ctr.count==9||uart4_ctr.count==15)
				{
//					uint32_t
					temp_re =0;
					uart4->gpio_read(&temp_re, GPIO_UART4IN_MASK);
					temp_re = temp_re & GPIO_UART4IN_MASK;
					temp_re = temp_re >> 16;
					uart4_ctr.byte |= (temp_re<<((uart4_ctr.count-(9))/(6)));
				}
			}
			uart4_ctr.count++;
			if(uart4_ctr.count>51)
			{
				write_fifo(&usart4_fifo,&uart4_ctr.byte,1);
				uart4_ctr.timeout = 0x80;
				uart4_ctr.bytenum++;//
				uart4_ctr.flag = 0;
				uart4_ctr.count = 0;
				uart4->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)GPIO_UART4IN_MASK);
			}
		}
#endif
#endif
		timer_int_clear(TIMER_1);  //���TIM0�����жϱ�־
}

//��ʱ��7�жϷ������
void softtimer(TimerHandle_t xTimer)
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
#if HARD_TIME1
		if((rtc_1000ms==0)&&(RTC_SEC==2))
		{
			int_disable(INTNO_TIMER1);
			timer_stop(TIMER_1);
			int_handler_install(INTNO_TIMER1, timer0_isr);
			int_pri_set(INTNO_TIMER1, (-2));
			int_enable(INTNO_TIMER1);
			timer_start(TIMER_1, TIMER_CTRL_IE | TIMER_CTRL_NH, COUNT);
		}
#else

		compare_now++;
			if(compare_now<compare_set[8])
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

		if(uart4_ctr.timeout&0x80)
		{
			uart4_ctr.timeout++;
			if((uart4_ctr.timeout&0x7f)>20)
			{
				uart4_ctr.timeout = 0;
				//���͵�����
				if(uart4_ctr.bytenum)
				{
					xQueueSend(USART4_Queue,&uart4_ctr.bytenum,0);//������з�������
					uart4_ctr.bytenum = 0;
//					portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//�����Ҫ�Ļ�����һ�������л�
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
#if !HARD_TIME1
#if 1//send
		//suart4 send fifo
		if(usart4_send_fifo.now_len&&(!send_ctr4.send_flag))
		{
			//read send fifo get 1 byte
			read_fifo(&usart4_send_fifo,&(send_ctr4.byte_now),1);
			send_ctr4.send_flag=1;
			send_ctr4.bit_flag=0;
		}
		//suart4 send
		if(send_ctr4.send_flag&&(rtc_1000ms%2==0))
		{
			if(send_ctr4.bit_flag==0)
			{
				uart4->gpio_write((~GPIO_UART4OUT_MASK), GPIO_UART4OUT_MASK);
			}
			else if(send_ctr4.bit_flag<9)
			{
				uart4->gpio_write((((send_ctr4.byte_now>>(send_ctr4.bit_flag-1))&0x01)<<17), GPIO_UART4OUT_MASK);
			}
			else //if(send_ctr4.bit_flag==9)
			{
				uart4->gpio_write(GPIO_UART4OUT_MASK, GPIO_UART4OUT_MASK);
//				send_ctr4.bit_flag=0;
				send_ctr4.send_flag=0;
			}
			send_ctr4.bit_flag++;
		}
#endif
#if 1//receive
//		uart4_ctr.count=0;
//		uart4_ctr.byte = 0;
//		uart4_ctr.flag = 1;
		if(uart4_ctr.flag)
		{
			if(uart4_ctr.count>2&&uart4_ctr.count<18)//3 5 7 9 11 13 15 17
			{
				if(uart4_ctr.count%2)
				{
					uint32_t temp =0;
					uart4->gpio_read(&temp, GPIO_UART4IN_MASK);
					temp = temp & GPIO_UART4IN_MASK;
					temp = temp >> 16;
					uart4_ctr.byte |= (temp<<((uart4_ctr.count-3)/2));
				}
			}
			uart4_ctr.count++;
			if(uart4_ctr.count>17)
			{
				write_fifo(&usart4_fifo,&uart4_ctr.byte,1);
				uart4_ctr.timeout = 0x80;
				uart4_ctr.bytenum++;//
				uart4_ctr.flag = 0;
				uart4_ctr.count = 0;
				uart4->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)GPIO_UART4IN_MASK);
			}
		}
#endif
#endif

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
