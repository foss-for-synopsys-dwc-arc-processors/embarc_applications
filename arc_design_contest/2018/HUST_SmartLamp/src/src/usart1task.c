/*
 * usart1task.c
 *
 *  Created on: 2018��5��4��
 *      Author: Administrator
 */

#include "usart1task.h"
#include "usart3task.h"

#include "embARC.h"
#include "embARC_debug.h"
#include "dev_uart.h"

//#include "timer.h"
//#include "key.h"
#include "malloc.h"
#include "string.h"
#include "timer.h"

//
#include "stdlib.h"

#if ASSERT_PRINTF_ENABLE
#include  "stdarg.h"
#endif

#include "strpos.h"
DEV_UART *uart1;
static uint8_t uart1_rx_buffer;
static DEV_BUFFER uart1_uart_int_buffer;


#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
//uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint8_t usart_data_all[USART1_NUM][USART1_PER_LEN];//����1����buff
uint8_t usart_buff_flag=1;//�����Щ���鱻ʹ���ˣ��´����ĸ������д� Ĭ��ʹ�õ�һ������
//uint8_t buff_now=0;//��ǰʹ�õĻ����������
usart_dat usart1_data;
uint8_t usart1_time_out=0;//����1���ճ�ʱ��ʱ
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA=0;       //����״̬���
//������
TaskHandle_t USART1_Handler;
TaskHandle_t USART1_app_Handler;
//SemaphoreHandle_t  xMutex;
void occupy_buff(void);
uint32_t uart1_init(uint32_t bound)
{
	occupy_buff();
	uart1 = uart_get_dev(DW_UART_1_ID);

	if (uart1 == NULL) {
		EMBARC_PRINTF("Failed to get device of uart0 for blueteeth.\r\n");
		return -1;
	}

	if (uart1->uart_open(bound) == E_OPNED) {
		uart1->uart_control(UART_CMD_SET_BAUD, (void *)(bound));
		EMBARC_PRINTF("ble_uart open succeed\r\n");
	}

	DEV_BUFFER_INIT(&uart1_uart_int_buffer, &uart1_rx_buffer, 1);
	if (uart1->uart_control(UART_CMD_SET_RXINT_BUF, (void *) & uart1_uart_int_buffer) != E_OK) {
		EMBARC_PRINTF("ble_isr_buffer config error\r\n");
		return -1;
	}

//	if (uart1->uart_control(UART_CMD_SET_RXCB, (void *) & USART1_IRQHandler) != E_OK) {
//		EMBARC_PRINTF("ble_isr config error\r\n");
//		return -1;
//	}

	int_handler_install(INTNO_UART1, USART1_IRQHandler);
	int_pri_set(INTNO_UART1, (-1));

	if (uart1->uart_control(UART_CMD_SET_RXINT, (void *)(1)) != E_OK) {
		EMBARC_PRINTF("Enable ble interrupt error\r\n");
		return -1;
	}

	return 0;
}


//�ҵ�δ��ʹ�õĻ���buff��
//����0-��USART1_NUM-1����ʾ��Ӧ���������ʹ��
//����USART1_NUM��ʾû�п��л���buff
//�浽 buff_now��
void occupy_buff(void)
{
	uint8_t i,ret=USART1_NUM;
	for(i=0;i<USART1_NUM;i++)
	{
		if(!(usart_buff_flag&(0x01<<i)))
		{
			ret  = i;
			usart_buff_flag|=(0x01<<i);
			break;
		}
	}
	usart1_data.buff_now = ret;
}
//�����껺��buff���ݺ��ͷ�buff��ռ��״̬
void release_buff(uint8_t buff_num)
{
	if(buff_num<USART1_NUM)
	{
		memset(usart_data_all[buff_num],0,USART1_PER_LEN);//������ݽ��ջ�����USART_RX_BUF,������һ�����ݽ���
		usart_buff_flag&=~(0x01<<buff_num);

	}
	if(usart1_data.buff_now==USART1_NUM) occupy_buff();
}

QueueHandle_t USART1_Queue;


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t Res;
	BaseType_t xHigherPriorityTaskWoken;


		uart1->uart_read(&Res, 1);	//��ȡ���յ�������


			usart1_time_out=0x80;//���������
		if(usart1_data.buff_now<USART1_NUM)
			usart_data_all[usart1_data.buff_now][USART_RX_STA++] = Res;
		else
		{
			dprintf("usart1buff no space\r\n");
		}

		if(USART_RX_STA>USART1_PER_LEN-1)
		{
			usart1_time_out=0x00;//�������һ�����ݣ����ٽ��г�ʱ��ʱ���´��յ������ټ�ʱ
			usart1_data.len=USART_RX_STA&0x7fff;
			xQueueSendFromISR(USART1_Queue,&usart1_data,&xHigherPriorityTaskWoken);//������з�������
			USART_RX_STA = 0;
			occupy_buff();
			portYIELD_FROM_ISR();//�����Ҫ�Ļ�����һ�������л�
		}


}
#endif


QueueHandle_t USART1_Task_Queue;//����task���������ݶ��� ���Ƿְ��õ�һ֡һ֡������
USART_TASK_PROCESS usart1_task_process=USART_DATA_IDEL;//���ݴ������̱��
uint8_t usart1_task_time_out=0;//����1 task���ճ�ʱ��ʱ
uint16_t server_data_remain;//server data���ж���û�и��ƹ���
usart_dat usart1_temp;//���յײ㴮�ڻ������ݶ���
usart_task_data usart1_task_temp;//���͵�usart1task������

void USART1_task(void *pvParameters)
{

	uint8_t temp_pos;//�ݴ��־λ��λ����Ϣ
	uint8_t data_num_remain=0;//��ǰ����������.ʣ�����ݳ��� Ҫ�ѽ��յ���usart1_temp.len�������ݴ�����
	uint8_t* data_p;//�洢���ݵĵ�ַ �����ڴ�ʱ�����ݰ�����Ϣ�����Ӧ�Ĵ�С�ڴ�
	uint8_t* data_temp;//�ײ㴮������ָ��
	BaseType_t err;
	printf("USART1_task success\r\n");
    while(1)
    {
		if(xQueueReceive(USART1_Queue,&usart1_temp,0))//���յ��ײ㴮������
			{
				data_num_remain = usart1_temp.len;
				data_temp = usart_data_all[usart1_temp.buff_now];
				switch(usart1_task_process)
				{
					case USART_DATA_IDEL:
					{
						CHECK_BUFF:while(data_num_remain)
						{
							temp_pos = 0xff;//findpos(server_start,4,data_temp,data_num_remain);//����1Ĭ�ϲ������������ݣ�ֻ����\r\n��β����
							if(temp_pos==0xff)//û�з��������ݵ���ʼ��־ ��Ϊ����ͨ����
							{
								temp_pos=findpos(rn_stop,2,data_temp,data_num_remain);
								if(temp_pos==0xFF)//û��\r\n�������
								{
									data_p=pvPortMalloc(data_num_remain);//������\r\n��ȥ��ֻ����ǰ�������
									memcpy(data_p,data_temp,data_num_remain);
									usart1_task_temp.len = data_num_remain;
									usart1_task_temp.pstr=data_p;
									usart1_task_temp.type=USART_OTHER;
									err=xQueueSend(USART1_Task_Queue,&usart1_task_temp,10);
									if(err==errQUEUE_FULL)   	//������Ϣ֡
									{
										vPortFree(usart1_task_temp.pstr);
										printf("����USART1_Task_Queue���������ݷ���OTHERʧ��!\r\n");
									}
									data_num_remain = 0;
									break;
									}
									else//Ѱ�ҵ�\r\n ���͵�ǰ֡���жϻ���buffʣ�������Ƿ�����ϣ�û�д�����Ͼͼ��� USART_DATA_IDEL �Ĵ���
									{
										if(temp_pos)
										{
											data_p=pvPortMalloc(temp_pos);//������\r\n��ȥ��ֻ����ǰ�������
											memcpy(data_p,data_temp,temp_pos);
											usart1_task_temp.len = temp_pos;
											usart1_task_temp.pstr=data_p;
											usart1_task_temp.type=USART_RN;
											err=xQueueSend(USART1_Task_Queue,&usart1_task_temp,10);
											if(err==errQUEUE_FULL)   	//������Ϣ֡
											{
												vPortFree(usart1_task_temp.pstr);
												dprintf("����USART1_Task_Queue���������ݷ���RNʧ��!\r\n");
											}
										}
										data_num_remain-=temp_pos+2;
										data_temp+=temp_pos+2;
									}
								}
								else if(temp_pos==0x00)//��ʼλ�þ�����ʼ��־
								{
									server_data_remain = ((data_temp[4])<<8)+data_temp[5];//��һ֡���ݳ���
									if(data_num_remain>5)//���ٰ���������Ϣ����Ȼ�Ͳ��ܼ���
									{
									if(server_data_remain>data_num_remain)//һ֡��Խ����������Ҫ������������֡
									{
										usart1_task_time_out=0x80;
										data_p=pvPortMalloc(server_data_remain);
										usart1_task_temp.len = server_data_remain;
										usart1_task_temp.pstr=data_p;
										memcpy(data_p,data_temp,data_num_remain);
										data_p+=data_num_remain;
										server_data_remain-=data_num_remain;//֡����Ҫ�ĳ���
										usart1_task_process = USART_DATA_SERVER_START;//SERVER��֡�Ľ���
										data_num_remain = 0;
										break;
									}
									else//һ֡server ��һ���������������
									{
										//���жϽ���λ�Ƿ���ȷ����ִ�в���
										if(strsame(server_stop,data_temp+server_data_remain-4,4))//��֤ʧ�ܣ��Ƿ�����ݵ���other���ݷ���������Ĵ����Ƕ���
										{
											data_p=pvPortMalloc(server_data_remain);
											usart1_task_temp.len = server_data_remain;
											usart1_task_temp.pstr=data_p;
											usart1_task_temp.type=USART_SERVER;
											memcpy(data_p,data_temp,server_data_remain);
											err=xQueueSend(USART1_Task_Queue,&usart1_task_temp,10);
											if(err==errQUEUE_FULL)   	//������Ϣ֡
											{
													//����ʧ����Ҫfree�ڴ�
												  vPortFree(usart1_task_temp.pstr);
													dprintf("����USART1_Task_Queue���������ݷ��͵�����һ֡server����ʧ��!\r\n");
											}
										}
									}
										{
											data_num_remain-=server_data_remain;
											data_temp+=server_data_remain;
										}
									}
									else//��������ʼ��6λ ֱ�Ӷ���
									{
										data_num_remain = 0;
										break;
									}
								}
								else//��ʼλ�����м� Ҳ���Ƿ���������֮ǰ���������� ��������ʼλ�þ�����ʼ��־
								{
										data_num_remain-=temp_pos;
										data_temp+=temp_pos;
								}
						}
						//���������ݣ��ͷ�buff��ռ��
						release_buff(usart1_temp.buff_now);//�ͷ�buff��ռ��״̬
					}
							break;
						case USART_DATA_SERVER_START:
						{
							{
								if(server_data_remain>data_num_remain)//�����֡
								{
									usart1_task_time_out=0x80;
									memcpy(data_p,data_temp,data_num_remain);
									data_p+=data_num_remain;
									server_data_remain-=data_num_remain;//֡����Ҫ�ĳ���
									usart1_task_process = USART_DATA_SERVER_START;//SERVER��֡�Ľ���
									data_num_remain = 0;
									break;
								}
								else//һ���ڽ�����֡
								{
									usart1_task_time_out=0x00;
									usart1_task_process = USART_DATA_IDEL;
									memcpy(data_p,data_temp,server_data_remain);
									if(strsame(server_stop,usart1_task_temp.pstr+usart1_task_temp.len-4,4))
									{
										usart1_task_temp.type=USART_SERVER;
										err=xQueueSend(USART1_Task_Queue,&usart1_task_temp,10);
										if(err==errQUEUE_FULL)   	//������Ϣ֡
										{
												//����ʧ����Ҫfree�ڴ�
										  vPortFree(usart1_task_temp.pstr);
										  dprintf("����USART1_Task_Queue���������ݷ��Ͷ����һ֡server����ʧ��!\r\n");
										}
									}
									else//���ݲ��ԣ���Ҫfree�ڴ�
									{
										vPortFree(usart1_task_temp.pstr);
									}
									data_num_remain-=server_data_remain;
									data_temp+=server_data_remain;

									{//��֡��ϻ�������û�д���
										if(data_num_remain)
										{//�ٴ��������ݼ��
											goto CHECK_BUFF;//��һ����Ч���������buff����
										}
									}
								}
							}
						//���������ݣ��ͷ�buff��ռ��
						release_buff(usart1_temp.buff_now);//�ͷ�buff��ռ��״̬
						}
						break;
					}
			 }

			vTaskDelay(USART1_task_delay);      //��ʱ10ms��Ҳ����10��ʱ�ӽ���
    }
}


#if 0 //����1����Ҫ�ͷŶ����ڴ� ֻ���մ���\R\N���� �����п����֡����
//��ֵ�ź������
SemaphoreHandle_t BinarySemaphore_free;	//��ֵ�ź������


//USART1_task�л���ڶ����֡���ݳ��ִ�����������Ҫ����free �����ڴ�Ĳ���
void FREE_ERRORMEM_task(void *pvParameters)
{
	BaseType_t err;
	printf("FREE_ERRORMEM_task success\r\n");
	while(1)
	{
		if(BinarySemaphore_free!=NULL)
		{
			err=xSemaphoreTake(BinarySemaphore_free,portMAX_DELAY);	//��ȡ�ź���
//			err=xSemaphoreTake(BinarySemaphore_free,0);	//��ȡ�ź���
			if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{
				usart1_task_process = USART_DATA_IDEL;
				vPortFree(usart1_task_temp.pstr);//���ж���free�������Ҫ���ⲿtask�н��в���
				printf("free_mem sucess!\r\n");
			}
			else if(err==pdFALSE)
			{
				vTaskDelay(FREE_ERRORMEM_delay);     //��ʱ10ms��Ҳ����10��ʱ�ӽ���
			}
		}
		      //��ʱ10ms��Ҳ����10��ʱ�ӽ���
	}
}




#endif

//����1 APP�㴦������ ***���Թ��ܣ��ش�
void usart1_app_task(void *pvParameters)
{
	usart_task_data usart1_app_temp;//����USART3_task �����ķ�������֡����
	uint8_t* buffer;
	BaseType_t err;
	dprintf("usart1_app_task start!\r\n");
	while(1)
	{
	if(xQueueReceive(USART1_Task_Queue,&usart1_app_temp,0))//���յ�����������
	{
		buffer=pvPortMalloc(usart1_app_temp.len+1);
    memcpy(buffer,usart1_app_temp.pstr,usart1_app_temp.len);
		buffer[usart1_app_temp.len]=0;
		printf("U1:%s\r\n",buffer);//����ʹ��printf ���ǵ���
#if 0
		if(findpos("AT+L",4,usart1_app_temp.pstr,usart1_app_temp.len)!=0XFF)
		{
			switch(usart1_app_temp.pstr[4])
			{
			case '0':
				compare_pwm = 0;
				break;
			case '1':
				compare_pwm = 3;
//				senddata_by_usart4((uint8_t*)"a\r\n",3);
				break;
			case '2':
				compare_pwm = 6;
//				senddata_by_usart4((uint8_t*)"5\r\n",3);
				break;
			case '3':
				compare_pwm = 10;
//				senddata_by_usart4((uint8_t*)"9\r\n",3);
				break;
			}
//			senddata_by_usart4((uint8_t*)"hello usart5\r\n",14);
		}
		else if(findpos("AT+GMR",6,usart1_app_temp.pstr,usart1_app_temp.len)!=0XFF)
		{
			uint8_t	temp = 1;
			err=xQueueSend(connect_to_server_Queue,&temp,10);
			if(err==errQUEUE_FULL)   	//������Ϣ֡
				{
					printf("����connect_to_server_Queue������1���ݷ���ʧ��!\r\n");
				}
			vTaskResume(connect_to_server_Handler);
		}
#endif

#if 0//ʹ�ô���3���Ѱ���ģ��ɴ���ʹ��
			err=xQueueSend(USART3_Send_Queue,&usart1_app_temp,10);
			if(err==errQUEUE_FULL)   	//������Ϣ֡
			{
				//����ʧ����Ҫfree�ڴ�
				vPortFree(usart1_app_temp.pstr);
//				pkt_serial--;
				dprintf("����USART3_Send_Queue������rn����ʧ��!\r\n");
			}
#else
		vPortFree(usart1_app_temp.pstr);//������͸�uart3���ͻᱻ���
#endif
		vPortFree(buffer);
	}
	vTaskDelay(USART1_APP_delay/10);
	}
}

//��ִ���κβ���
void printf_nosense(char *format, ...)
{
	//
}


//end usart1task.c

