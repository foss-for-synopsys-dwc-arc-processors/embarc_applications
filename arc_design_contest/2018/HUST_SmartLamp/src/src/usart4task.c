/*
 * usart4task.c
 *
 *  Created on: 2018��5��16��
 *      Author: Administrator
 */

#include "usart4task.h"
#include "usart1task.h"//����printf
#include "ESP8266.h"
//#include "bright.h"
#include "fifo.h"
#include "timer.h"
#include "task_config.h"
#include "package.h"
#include "bright.h"

//#include "timer.h"
//#include "key.h"
#include "malloc.h"
#include "string.h"


//
#include "stdlib.h"

#include "strpos.h"
//ģ�⴮�ڲ����з�֡�Ĳ�����ֱ����FIFO�Ļ���buff������ÿ�η���FIFO���յ������ݸ�����Ӧ�ò�ֱ�Ӵ�����Щ����
//���ذ崮��
#define delay_ms vTaskDelay
#define CMD_WAIT_MAX 5
#define EMSK_GPIO_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)
#define BTN_ACTIVE_LOW   (0)

uint8_t usart4_fifo_data[USART4_FIFO_LEN];
//uint8_t usart4_send_data[USART4_SEND_LEN];
fifo_struct  usart4_fifo;//����4�����ݴ洢FIFO ����fifo
//fifo_struct  usart4_send_fifo;
suart_struct uart4_ctr;//ģ�⴮��4�Ŀ��Ʋ���
//suart_send_struct send_ctr4;

//�봥�ذ�ͨ�ſ���
uint8_t wait_cmd=0,wait_cmd_count=0;
uint16_t touch_flag=0;//��¼���ذ�Ĳ�ͬ��λ��ģʽ��ȫ�ִ洢�ƹ⵵λ��wifi��״̬ ̨�ƿ���״̬��wifiset״̬������״̬��

TaskHandle_t USART4_app_Handler;//
TimerHandle_t	period_waittouch_Handle;
QueueHandle_t USART4_Queue;
QueueHandle_t USART4_Send_Queue;
DEV_GPIO * uart4;


void suart_var_init(suart_struct* re_ctr)
{
	re_ctr->byte = 0;
	re_ctr->bytenum = 0;
	re_ctr->count = 0;
//	re_ctr->flag = 0;
	re_ctr->timeout = 0;
//	sd_ctr->bit_flag = 0;
//	sd_ctr->byte_now = 0;
}


#define EMSK_GPIOUART4_PORT		DEV_GPIO_PORT_A
#define GPIO_UART4_MASK		0x00030000		//portA��16-17Pin����J3��7-8��
#define GPIO_UART4_LOW	  (0)
#define GPIO_UART4IN_MASK	0X00010000		//portA��16Pin����J3��7��
#define GPIO_UART4OUT_MASK	0X00020000		//portA��17Pin����J3��8��



//����һ��byte
void EXTI1_IRQHandler(void *ptr)
{

	BaseType_t xHigherPriorityTaskWoken;

//		uart4_ctr.flag = 0;
		#if 1//��ʱ������������
		uint32_t temp;
		delay_us(104+52);
		uart4_ctr.count=0;
		uart4_ctr.byte = 0;
		for(uart4_ctr.count=0;uart4_ctr.count<8;uart4_ctr.count++)
		{
			uart4->gpio_read(&temp, GPIO_UART4IN_MASK);
			uart4_ctr.byte |= (temp<<(uart4_ctr.count));
			delay_us(104);
		}
//		delay_us(104);
		write_fifo(&usart4_fifo,&uart4_ctr.byte,1);
		uart4_ctr.timeout = 0x80;
		uart4_ctr.bytenum++;//������Ҫ�ж����������󣬽��ж�֡




}

uint8_t senddata_by_usart4(uint8_t*str,uint8_t len)//�ⲿ���÷��ͺ���
{
	usart_task_data USART4_send_data;//USART3send����
	uint8_t*p;
	BaseType_t err;

	USART4_send_data.type = USART_DIRECT;

	USART4_send_data.len  = len;
	p = pvPortMalloc(USART4_send_data.len);
	memcpy(p,str,USART4_send_data.len);
	USART4_send_data.pstr = p;

	err=xQueueSend(USART4_Send_Queue,&USART4_send_data,0);
	if(err==errQUEUE_FULL)   	//������Ϣ֡
	{
		//����ʧ����Ҫfree�ڴ�
		vPortFree(USART4_send_data.pstr);
		dprintf("����USART4_Send_Queue���������ݷ���ʧ��!\r\n");
		return 0;
	}
	else return 1;
}

void u4app_send(uint8_t*str,uint8_t len)//�ڲ����ú���
{
	#define DELAY_1bit 90
	#define	DELAY_1bit2	92

	uint8_t i,j,data;
//	taskENTER_CRITICAL();           //�����ٽ���
	for(i=0;i<len;i++)
	{
		//PBout(0) = 0;
		uart4->gpio_write((~GPIO_UART4OUT_MASK), GPIO_UART4OUT_MASK);
		data = str[i];
//		printf("1\r\n");
		delay_us(DELAY_1bit2);
//		printf("1\r\n");
		for(j=0;j<8;j++)
		{
			//PBout(0) =(data&0x01);
			uart4->gpio_write(((data&0x01)<<17), GPIO_UART4OUT_MASK);
//			uart4->gpio_write(GPIO_UART4OUT_MASK, GPIO_UART4OUT_MASK);
			data=data>>1;
			delay_us(DELAY_1bit);
		}
		//PBout(0) = 1;
		uart4->gpio_write(GPIO_UART4OUT_MASK, GPIO_UART4OUT_MASK);
		delay_us(DELAY_1bit2);
	}
//	taskEXIT_CRITICAL();            //�˳��ٽ���
	#endif
}

void usart4_init(void)
{
	DEV_GPIO_BIT_ISR bit_isr;
	DEV_GPIO_INT_CFG int_cfg;

	uart4 = gpio_get_dev(EMSK_GPIOUART4_PORT);

	EMSK_GPIO_CHECK_EXP_NORTN(uart4 != NULL);
	if (uart4->gpio_open(GPIO_UART4_MASK) == E_OPNED) {
		uart4->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(GPIO_UART4OUT_MASK));
		uart4->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(GPIO_UART4IN_MASK));
		uart4->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(GPIO_UART4_MASK));
	}
	int_cfg.int_bit_mask = GPIO_UART4IN_MASK;
	int_cfg.int_bit_type = GPIO_UART4IN_MASK;
	int_cfg.int_bit_polarity = BTN_ACTIVE_LOW;
	int_cfg.int_bit_debounce = GPIO_UART4IN_MASK;
	uart4->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);
	bit_isr.int_bit_ofs = 16;
	bit_isr.int_bit_handler = EXTI1_IRQHandler;
	uart4->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);
	uart4->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)GPIO_UART4IN_MASK);
	uart4->gpio_write(GPIO_UART4OUT_MASK,GPIO_UART4OUT_MASK);
error_exit:
	return;
}




void USART4_app_task(void *pvParameters)
{
	uint8_t nlen=0;
	uint8_t* p;
	uint8_t* buffer;
	usart_task_data usart_send4;
	BaseType_t err;

	dprintf("usart4_app_task start!\r\n");
	while(1)
	{
		if(xQueueReceive(USART4_Queue,&nlen,0))//��������
		{
			uint8_t i;
			p = pvPortMalloc(nlen);
			read_fifo(&usart4_fifo,p,nlen);

			#if 1//���Դ���
			buffer =  pvPortMalloc(nlen+1);
			memcpy(buffer , p , nlen);
			buffer[nlen] = 0;
			dprintf("u4re %d:%s\r\n",nlen,buffer);
			vPortFree(buffer);

			#endif
			//�����ذ尴������

//				for(i=0;i<nlen;i++)
			if(nlen==1)
				{
					touch_process(p[0]);
				}
			vPortFree(p);
		}
		if(xQueueReceive(USART4_Send_Queue,&usart_send4,0))//�������� ���ͺ���Ҫ�ȴ���ִ��Ϣ
		{
			#if 1//���Դ���
			buffer =  pvPortMalloc(usart_send4.len+1);
			memcpy(buffer , usart_send4.pstr ,usart_send4.len);
			buffer[usart_send4.len] = 0;
			dprintf("u4sd %d:%s\r\n",usart_send4.len,buffer);
			vPortFree(buffer);
			#endif
			//������ʱ������⼸�λ�ִ�źţ�û���յ���ִ�źžͶ෢�ͼ�������
			if(usart_send4.len==1)//���ͳ���Ϊ1 �Ƿ��͸����ذ�Ŀ�������
			{
				if(usart_send4.pstr[0]&0x80)//��32���п��ƣ�����32�ظ����ذ����
				{
					if(wait_cmd_count&&(usart_send4.pstr[0]==wait_cmd))//�ǵ�ǰ���ڵȴ���ָ�� �����в���
					{

					}
					else//û��ָ���ڵȴ������߳����µ���Ҫ�ȴ���ָ��������ڶ�ʱ������ָ����Ͷ���
					{
						//�رն�ʱ��
						xTimerStop(period_waittouch_Handle,0);
						wait_cmd = usart_send4.pstr[0];
						wait_cmd_count = CMD_WAIT_MAX;//cmd���ȴ�����

						//������ʱ��
						xTimerStart(period_waittouch_Handle,0);
					}
				}
			}
			u4app_send(usart_send4.pstr,usart_send4.len);

			vPortFree(usart_send4.pstr);
		}

		vTaskDelay(USART4_APP_delay);
	}
}



#define DEBUG_TOUCH 1

//����������Ӧ�Ĺ���
void touch_process(uint8_t touch_cmd)
{
	uint8_t back_cmd;
	if(touch_cmd&0x80)//�����ź�
	{
		//��ִ�����ذ�
		back_cmd = touch_cmd&0x7f;
		senddata_by_usart4(&back_cmd,1);
		if(touch_cmd&(0x01<<6))//enable
		{
		if(touch_cmd&0x08)//sp =1
		{
			if(touch_cmd&(0x01<<5))//sp  = 1  power=1    0��(ֻ������ģʽ��)���ĵ�����
			{
				if(!(touch_cmd&0x07))//�ĵ����� bit2-0=0
				{
					#if DEBUG_TOUCH
					dprintf("touch re:power on\r\n");
					#endif
					touch_process_poweron();
				}
			}
			else//sp  = 1  power=0    wifiset   boot(ֻ�����п�����bootģʽ����)
			{
				if(touch_cmd&(0x01<<1))//wifiset ����wifi����ģʽ
				{
					#if DEBUG_TOUCH
					dprintf("touch re:wifiset start\r\n");
					#endif
					touch_flag|=(0x0001<<5);
					touch_flag&=(~(0x0001<<4));//��¼wifi�ƹر�

					//�ָ�wifi_config������
					//wifi��������
					start_config_task();

				}
				else//�˳�wifi����ģʽ
				{
					#if DEBUG_TOUCH
					dprintf("touch re:wifiset quit\r\n");
					#endif
					touch_flag&=(~(0x0001<<5));
					touch_flag&=(~(0x0001<<4));//��¼wifi�ƹر�
					//����wifi_config������
					stop_config_task(0);
				}
			}
		}
		else//sp = 0
		{
				if(touch_cmd&(0x01<<5))//sp  = 0  power=1    ��λ���� 1-8
				{
					#if DEBUG_TOUCH
					dprintf("touch re:light lv=%d\r\n",((touch_cmd&0x07)+1));
					#endif

					touch_process_lightswitch(((touch_cmd&0x07)+1),touch_cmd&(0x01<<4));

				}
				else//sp  = 0  power=0  �ص�
				{
					if(!(touch_cmd&0x07))//�ص� bit2-0=0
					{
					#if DEBUG_TOUCH
					dprintf("touch re:power off\r\n");
					#endif
						touch_process_poweroff();
					}
				}
		}
	}

}


	else //��ִ�ź�
	{
		if(wait_cmd_count)
		{
			if(touch_cmd==(wait_cmd&0x7f))
			{
				//�رն�ʱ��
				xTimerStop(period_waittouch_Handle,0);
				wait_cmd_count = 0;
			}
		}
	}
}

void touch_process_poweroff(void)//poweroff�Ĵ���
{
		touch_flag|=(0x0001<<10);//���ذ�����˲������
		touch_flag&=(~(0x0001<<7));//���Ϊ�ص�״̬
		touch_flag&=0xfff0;//��¼�ƹ⵵λΪ0��
		bright_mode(LIGHT_SWITCH,touch_flag&0x000F);

		touch_flag&=(~(0x0001<<9));
		touch_flag&=(~(0x0001<<8));//�˳��Զ�����
		//��¼wifi��״̬ �ر�
		touch_flag&=(~(0x0001<<4));
		//����0002
		sendpack2server(0x0002);
}

void touch_process_poweron(void)//poweron�Ĵ��� �����Զ�����
{
	touch_flag|=(0x0001<<10);//���ذ�����˲������
	touch_flag|=(0x0001<<7);//���Ϊ����״̬
	touch_flag&=0xfff0;
	touch_flag|=0x0004;//��¼�ƹ⵵λΪ4��
	bright_mode(LIGHT_SWITCH,touch_flag&0x000F);//ֱ������
	delay_ms(300);

	//�����Զ�����
	touch_flag|=(0x0001<<8);//���Ϊ�Զ�����ģʽ
	bright_mode(LIGHT_AUTO,touch_flag&0x000F);//ֱ������

	//���Ͱ�0002
	sendpack2server(0x0002);

	if(touch_flag&(0x0001<<14))
	{
	mcu_sendcmd2touch(Wifi_On);
	}
			else
			{
				mcu_sendcmd2touch(Wifi_Off);
			}
}

void touch_process_lightswitch(uint8_t light,uint8_t wifi)//����ֱ�ӵ���ĳ����λ
{
	uint8_t temp;
	touch_flag|=(0x0001<<10);//���ذ�����˲������
	if((touch_flag&0x000f)!=light)//�ǲ�ͬ��λ
	{
		//��ǿ���
		touch_flag|=(0x0001<<7);
		//���ڵƹ⵽��Ӧ��λ
		touch_flag&=0xfff0;
		touch_flag|=light;//��¼�ƹ⵵λ
		bright_mode(LIGHT_SWITCH,touch_flag&0x000F);
		#if 0//���ذ尴������
		if(light==1){
												temp = 1;
												xQueueSend(connect_to_server_Queue,&temp,10);
												vTaskResume(connect_to_server_Handler);
		}
		#endif

		//�˳��Զ�����
		touch_flag&=(~(0x0001<<8));

		//�ж��Ƿ����˯��ģʽ
		if(light<4&&light>0)//���Խ���˯��ģʽ ����Ҫ�ж�ħ���Ƿ������Ҵ������״̬
		{

		}
		else//���鵵���˳�˯��ģʽ
		{
			touch_flag&=(~(0x0001<<9));

		}

		//��¼wifi��״̬
		if(wifi)
		{
			touch_flag|=(0x0001<<4);
		}
		else
		{
			touch_flag&=(~(0x0001<<4));
		}

		//����0002
		sendpack2server(0x0002);
			//����wifi��
		if(touch_flag&(0x0001<<14))
		{
			mcu_sendcmd2touch(Wifi_On);
		}
			else
			{
				mcu_sendcmd2touch(Wifi_Off);
			}
	}
}

//��ʱ�����ڷ��Ͱ������ƣ��ȴ��ذ���Ӧ
void period_waittouch_Callback(TimerHandle_t xTimer)		//
{
	dprintf("wait_touch run:%d\r\n",wait_cmd_count);
	if(wait_cmd_count>1)
	{
		wait_cmd_count--;
		senddata_by_usart4(&wait_cmd,1);
	}
	else
	{
	//�رն�ʱ��
		wait_cmd_count = 0;
		xTimerStop(period_waittouch_Handle,0);
	}
}

//�����ذ巢������
void mcu_sendcmd2touch(uint8_t cmd)
{
	senddata_by_usart4(&cmd,1);
}

//32���Ƶƹ�ʹ��ذ壬���벻ͬ��ģʽ 8003[22]  ����Ӧ
void server_control_light(uint8_t mode)
{
	if(mode)//
	{
		if(mode&0x10)//�Զ�����
		{
			mcu_sendcmd2touch(Power_On|0x10);//�������ң���wifi��
			touch_process_poweron();

		}
		else if(mode&0x20)//����λ ���ﵥ��д�����������ƵĲ����˳��Զ�����ģʽ
		{
			//���ƴ��ذ嵽����
			mcu_sendcmd2touch(0xf0|((mode&0x0f)-1));

			//���ڵƹ⵽��Ӧ��λ
			touch_flag&=0xfff0;
			touch_flag|=(mode&0x0f);//��¼�ƹ⵵λ
			bright_mode(LIGHT_SWITCH,touch_flag&0x000F);

		//�ж��Ƿ����˯��ģʽ
		if((mode&0x0f)<4)//���Խ���˯��ģʽ ����Ҫ�ж�ħ���Ƿ������Ҵ������״̬
		{

		}
		else//���鵵���˳�˯��ģʽ
		{
			touch_flag&=(~(0x0001<<9));

		}

		//��¼wifi��״̬
		touch_flag|=(0x0001<<4);

		//����0002
		sendpack2server(0x0002);
		}
	}
	else//power off 0x00
	{
		mcu_sendcmd2touch(Power_Off);
		touch_process_poweroff();
	}
}

//end USART4task.c
