/*
 * usart3task.c
 *
 *  Created on: 2018��5��3��
 *      Author: Administrator
 */

#include "usart3task.h"
#include "usart1task.h"//����printf
#include "ESP8266.h"
#include "task_config.h"

#include "embARC.h"
#include "embARC_debug.h"
#include "dev_uart.h"


//#include "timer.h"
//#include "key.h"
#include "malloc.h"
#include "string.h"


//
#include "stdlib.h"

#include "strpos.h"

//uint8_t USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
uint8_t usart3_data_all[USART3_NUM][USART3_PER_LEN];//����1����buff
uint8_t usart3_buff_flag=1;//�����Щ���鱻ʹ���ˣ��´����ĸ������д� Ĭ��ʹ�õ�һ������
//uint8_t buff_now=0;//��ǰʹ�õĻ����������
usart_dat usart3_data;
uint8_t usart3_time_out=0;//����1���ճ�ʱ��ʱ
//����״̬ �޸�Ϊ����������������
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
uint16_t USART3_RX_STA=0;       //����״̬���
//�����ź������
SemaphoreHandle_t SendenableMutexSemaphore;	//�����ź���

TimerHandle_t OneShot_CanServer_Handle;

#if EN_USART3_RX   //���ʹ���˽���

TaskHandle_t FREE_ERRORMEM_Handler;
TaskHandle_t USART3_Handler;
TaskHandle_t USART3_send_Handler;

DEV_UART *uart3;
static uint8_t esp8266_rx_buffer;
static DEV_BUFFER esp8266_uart_int_buffer;
uint16_t testbuffer[5]={0};


//����3�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���

void occupy_buff3(void);
uint32_t uart3_init(uint32_t bound)
{
	occupy_buff3();
	uart3 = uart_get_dev(DW_UART_0_ID);

		if (uart3 == NULL) {
			EMBARC_PRINTF("Failed to get device of uart0 for blueteeth.\r\n");
			return -1;
		}

		if (uart3->uart_open(bound) == E_OPNED) {
			uart3->uart_control(UART_CMD_SET_BAUD, (void *)(bound));
			EMBARC_PRINTF("ble_uart open succeed\r\n");
		}

		DEV_BUFFER_INIT(&esp8266_uart_int_buffer, &esp8266_rx_buffer, 1);
		if (uart3->uart_control(UART_CMD_SET_RXINT_BUF, (void *) & esp8266_uart_int_buffer) != E_OK) {
			EMBARC_PRINTF("ble_isr_buffer config error\r\n");
			return -1;
		}

//		if (uart3->uart_control(UART_CMD_SET_RXCB, (void *) & USART3_IRQHandler) != E_OK) {
//			EMBARC_PRINTF("ble_isr config error\r\n");
//			return -1;
//		}

		int_handler_install(INTNO_UART0, USART3_IRQHandler);
		int_pri_set(INTNO_UART0, (-2));

		if (uart3->uart_control(UART_CMD_SET_RXINT, (void *)(1)) != E_OK) {
			EMBARC_PRINTF("Enable ble interrupt error\r\n");
			return -1;
		}

		return 0;
}


//�ҵ�δ��ʹ�õĻ���buff��
//����0-��USART3_NUM-1����ʾ��Ӧ���������ʹ��
//����USART3_NUM��ʾû�п��л���buff
//�浽 buff_now��
void occupy_buff3(void)
{
	uint8_t i,ret=USART3_NUM;
	for(i=0;i<USART3_NUM;i++)
	{
		if(!(usart3_buff_flag&(0x01<<i)))
		{
			ret  = i;
			usart3_buff_flag|=(0x01<<i);
			break;
		}
	}
	usart3_data.buff_now = ret;
}
//�����껺��buff���ݺ��ͷ�buff��ռ��״̬
void release_buff3(uint8_t buff_num)
{
	if(buff_num<USART3_NUM)
	{
		memset(usart3_data_all[buff_num],0,USART3_PER_LEN);//������ݽ��ջ�����USART_RX_BUF,������һ�����ݽ���
		usart3_buff_flag&=~(0x01<<buff_num);

	}
	if(usart3_data.buff_now==USART3_NUM) occupy_buff3();
}

//extern QueueHandle_t Message_Queue;	//��Ϣ���о��
QueueHandle_t USART3_Queue;

//����һ�����ڽ��յ�һ��byte���ͷ��ź�������������ݴ�����
//�����������ڽ��յ�һ��byte�����͵�������У���������ȡ�������ݲ�����
//3 ��200��ʱ��������Ҳ�ְ���
void USART3_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t Res3=0;
	BaseType_t xHigherPriorityTaskWoken;


			uart3->uart_read(&Res3, 1);	//��ȡ���յ�������


			usart3_time_out=0x80;//���������          				//���������
//		if(USART_RX_STA==0) 				//ʹ�ܶ�ʱ��7���ж�
//		{
//				TIM_Cmd(TIM7,ENABLE);//ʹ�ܶ�ʱ��7
//		}
#if 1
		if(usart3_data.buff_now<USART3_NUM)
		{
			usart3_data_all[usart3_data.buff_now][USART3_RX_STA++] = Res3;
//			testbuffer[USART3_RX_STA] = Res3;
//		USART3_RX_STA++;
		}
		else
		{
//			occupy_buff();
//			printf("USART3buff no space\r\n");
		}

		if(USART3_RX_STA>USART3_PER_LEN-1)
		{
			usart3_time_out=0x00;//�������һ�����ݣ����ٽ��г�ʱ��ʱ���´��յ������ټ�ʱ
			usart3_data.len=USART3_RX_STA&0x7fff;
			xQueueSendFromISR(USART3_Queue,&usart3_data,&xHigherPriorityTaskWoken);//������з�������
			USART3_RX_STA = 0;
			occupy_buff3();
			portYIELD_FROM_ISR();//�����Ҫ�Ļ�����һ�������л�
		}

#endif
}
#endif


//QueueHandle_t USART3_Task_Queue;//����task���������ݶ��� ���Ƿְ��õ�һ֡һ֡������
QueueHandle_t USART3_Task_server_Queue;//�洢����������
QueueHandle_t USART3_Task_rn_other_Queue;//�洢\r\n���ݺ�û�и�ʽ������
USART_TASK_PROCESS USART3_task_process=USART_DATA_IDEL;//���ݴ������̱��
uint8_t usart3_task_time_out=0;//����1 task���ճ�ʱ��ʱ
uint16_t server_data_remain3;//server data���ж���û�и��ƹ���
usart_dat USART3_temp;//���յײ㴮�ڻ������ݶ���
usart_task_data USART3_task_temp;//���͵�USART3task������

void USART3_task(void *pvParameters)
{

		uint8_t temp_pos3;//�ݴ��־λ��λ����Ϣ
		uint8_t data_num_remain3=0;//��ǰ����������.ʣ�����ݳ��� Ҫ�ѽ��յ���USART3_temp.len�������ݴ�����

	//�洢��һ�����ݵĳ��� �� �洢�ĵ�ַ
//	  uint16_t data_num_last=0;
		uint8_t* data_p3;//�洢���ݵĵ�ַ �����ڴ�ʱ�����ݰ�����Ϣ�����Ӧ�Ĵ�С�ڴ�
		uint8_t* data_temp3;//�ײ㴮������ָ��
		BaseType_t err;
		dprintf("USART3_task success\r\n");

	//������֡��־λ����
//		printf("start:%d\r\n", findpos(server_start,4,test_data,11));
//		printf("posrn:%d\r\n", findpos(rn_stop,2,test_data,11));
//		printf("stop:%d\r\n", findpos(server_stop,4,test_data,15));
//		vTaskSuspend(NULL);
    while(1)
    {
//			if(xQueueReceive(USART3_Queue,&USART3_temp,0))//���յ��ײ㴮������
       if(xQueueReceive(USART3_Queue,&USART3_temp,portMAX_DELAY))//���յ��ײ㴮������
			 {

//    	   	   printf("hhhhhhhhhhhhh");

				 data_num_remain3 = USART3_temp.len;
				 data_temp3 = usart3_data_all[USART3_temp.buff_now];

					switch(USART3_task_process)
					{
						case USART_DATA_IDEL:
						{
//							dprintf("u3re\r\n");

//							data_num_last=0;//���¼������ݶ���
							CHECK_BUFF:while(data_num_remain3)
							{
								temp_pos3 = findpos(server_start,4,data_temp3,data_num_remain3);
								if(temp_pos3==0xff)//û�з��������ݵ���ʼ��־ ��Ϊ����ͨ����
								{
									temp_pos3=findpos(rn_stop,2,data_temp3,data_num_remain3);
//									printf("USART3task test:%d\r\n",temp_pos);
									if(temp_pos3==0xFF)//û��\r\n�������
									{

										/*
										if(USART3_temp.len==USART3_PER_LEN)
										//ȫ�������ޱ�ǵ����ݣ����Һ��滹���ޱ�����ݣ��ȴ���\R\N���֡������������ȴ���һ֡���ݵ�\R\N
										//��Ҳ������Ϊ��Ч���ݹ���ֱ�Ӷ����� Ĭ��buff����Ϊ200 ��\r\n���ݲ��ᳬ��200 ������ֱ�Ӷ��������ޱ������ ��ʼ�µĽ��ܴ���
										//��������Ƿ���server_stop ,?
										{

										}
										else//�����ݳ���С��buff���� ��Ϊ���ݽ�����ֱ�ӷ��͵���������֡������
										{

										}
										*/
										//û��server_start��\r\n ֱ�ӷ��ͳ�ȥ
//										if(data_num_remain3<50)//�����������ʱ�����ֱ�ӷ���
										{
//											if(data_num_remain3>0)
											{
												data_p3=pvPortMalloc(data_num_remain3);//������\r\n��ȥ��ֻ����ǰ�������
												memcpy(data_p3,data_temp3,data_num_remain3);
												USART3_task_temp.len = data_num_remain3;
												USART3_task_temp.pstr=data_p3;
												USART3_task_temp.type=USART_OTHER;
												err=xQueueSend(USART3_Task_rn_other_Queue,&USART3_task_temp,10);
												if(err==errQUEUE_FULL)   	//������Ϣ֡
												{
													//����ʧ����Ҫfree�ڴ�
													vPortFree(USART3_task_temp.pstr);
														dprintf("����USART3_Task_rn_other_Queue���������ݷ���OTHERʧ��!\r\n");
												}
											}
										}

										data_num_remain3 = 0;

										break;
									}
									else//Ѱ�ҵ�\r\n ���͵�ǰ֡���жϻ���buffʣ�������Ƿ�����ϣ�û�д�����Ͼͼ��� USART_DATA_IDEL �Ĵ���
									{
											if(temp_pos3>0)
											{
												data_p3=pvPortMalloc(temp_pos3);//������\r\n��ȥ��ֻ����ǰ�������
												memcpy(data_p3,data_temp3,temp_pos3);
												USART3_task_temp.len = temp_pos3;
												USART3_task_temp.pstr=data_p3;
												USART3_task_temp.type=USART_RN;
												err=xQueueSend(USART3_Task_rn_other_Queue,&USART3_task_temp,10);
												if(err==errQUEUE_FULL)   	//������Ϣ֡
												{
														//����ʧ����Ҫfree�ڴ�
														vPortFree(USART3_task_temp.pstr);
														dprintf("����USART3_Task_rn_other_Queue���������ݷ���RNʧ��!\r\n");
												}
											}
											data_num_remain3-=temp_pos3+2;
											data_temp3+=temp_pos3+2;

									}
								}
								else if(temp_pos3==0x00)//��ʼλ�þ�����ʼ��־
								{
			                        temp_pos3 = findpos(server_stop,2,data_temp3,data_num_remain3);
			                        if(0xff!=temp_pos3)
			                        {
			                            data_p3=pvPortMalloc(temp_pos3+3);//
			                            memcpy(data_p3,data_temp3,temp_pos3+3);
			                            data_p3[temp_pos3+2] = 0;
			                            USART3_task_temp.len = temp_pos3+3;
			                            USART3_task_temp.pstr=data_p3;
			                            USART3_task_temp.type=USART_SERVER;
			                            err=xQueueSend(USART3_Task_server_Queue,&USART3_task_temp,10);
			                            if(err==errQUEUE_FULL)   	//������Ϣ֡
			                            {
			                                //����ʧ����Ҫfree�ڴ�
			                                vPortFree(USART3_task_temp.pstr);
			                                dprintf("����USART3_Task_rn_other_Queue���������ݷ���serverʧ��!\r\n");
			                            }
			                            data_num_remain3-=temp_pos3+2;
			                            data_temp3+=temp_pos3+2;

			                        }
			                        else
			                        {
			                            data_p3=pvPortMalloc(data_num_remain3);
			                            memcpy(data_p3,data_temp3,data_num_remain3);
			                            USART3_task_temp.len = data_num_remain3;
			                            USART3_task_temp.pstr=data_p3;
			                            USART3_task_temp.type=USART_OTHER;
			                            err=xQueueSend(USART3_Task_rn_other_Queue,&USART3_task_temp,10);
			                            data_num_remain3 = 0;

			                            if(err==errQUEUE_FULL)   	//������Ϣ֡
			                            {
			                                //����ʧ����Ҫfree�ڴ�
			                                vPortFree(USART3_task_temp.pstr);
			                                dprintf("����USART3_Task_rn_other_Queue���������ݷ���OTHERʧ��!\r\n");
			                            }
			                            break;
			                        }


								}
								else//��ʼλ�����м� Ҳ���Ƿ���������֮ǰ���������� ��������ʼλ�þ�����ʼ��־
								{
			                        {
			                            data_p3=pvPortMalloc(temp_pos3);
			                            memcpy(data_p3,data_temp3,temp_pos3);
			                            USART3_task_temp.len = temp_pos3;
			                            USART3_task_temp.pstr=data_p3;
			                            USART3_task_temp.type=USART_OTHER;
			                            err=xQueueSend(USART3_Task_rn_other_Queue,&USART3_task_temp,10);
			                            if(err==errQUEUE_FULL)   	//������Ϣ֡
			                            {
			                                //����ʧ����Ҫfree�ڴ�
			                                vPortFree(USART3_task_temp.pstr);
			                                dprintf("����USART3_Task_rn_other_Queue���������ݷ���OTHERʧ��!\r\n");
			                            }
			                        }

			                        data_num_remain3-=temp_pos3;
			                        data_temp3+=temp_pos3;
								}
						}
						//���������ݣ��ͷ�buff��ռ��
						release_buff3(USART3_temp.buff_now);//�ͷ�buff��ռ��״̬
					}
							break;


					}
			 }

			vTaskDelay(50);      //��ʱ10ms��Ҳ����10��ʱ�ӽ���USART3_task_delay
    }
}

//��ֵ�ź������
SemaphoreHandle_t BinarySemaphore_free;	//��ֵ�ź������


//USART3_task�л���ڶ����֡���ݳ��ִ�����������Ҫ����free �����ڴ�Ĳ���
void FREE_ERRORMEM_task(void *pvParameters)
{
	BaseType_t err;
	dprintf("FREE_ERRORMEM_task success\r\n");
	while(1)
	{
		if(BinarySemaphore_free!=NULL)
		{
			err=xSemaphoreTake(BinarySemaphore_free,portMAX_DELAY);	//��ȡ�ź���
//			err=xSemaphoreTake(BinarySemaphore_free,0);	//��ȡ�ź���
			if(err==pdTRUE)										//��ȡ�ź����ɹ�
			{
				USART3_task_process = USART_DATA_IDEL;
				vPortFree(USART3_task_temp.pstr);//���ж���free�������Ҫ���ⲿtask�н��в���
				dprintf("free_mem sucess!\r\n");
			}
			else if(err==pdFALSE)
			{
				vTaskDelay(FREE_ERRORMEM_delay);     //��ʱ10ms��Ҳ����10��ʱ�ӽ���
			}
		}
		      //��ʱ10ms��Ҳ����10��ʱ�ӽ���
	}
}

//���Ͷ��У������ݷŵ����Ͷ����У����з���


QueueHandle_t USART3_Send_Queue;//����3���Ͷ���
//����3������Ϣ����
void USART3_send_task(void *pvParameters)
{
	usart_task_data usart3_send_temp;
	char at_cipsend[30];

	dprintf("usart3_send start\r\n");
	    while(1)
    {
			if(xQueueReceive(USART3_Send_Queue,&usart3_send_temp, 0))
//       if(xQueueReceive(USART3_Send_Queue,&usart3_send_temp, portMAX_DELAY))//���Ͷ�����������Ҫ����
			 {
				 if(usart3_send_temp.type ==USART_SERVER)
				 {
					if(touch_flag&(0x0001<<14))
					 {
						if(xSemaphoreTake(SendenableMutexSemaphore,portMAX_DELAY))//һ����һ��û���յ���ִ���ͻ���Ҳ������  ����һ����ʱ������ʱ�Զ��ͷţ��������ܷ���
						{
							 xTimerStart(OneShot_CanServer_Handle,10);
						}
					 }

					 {
						//dprintf("send server\r\n");
					    sprintf(at_cipsend,"AT+CIPSEND=%d\r\n",usart3_send_temp.len);
					    u3_senddata((uint8_t*)at_cipsend,strlen(at_cipsend));
					    vTaskDelay(50); //����busy���� 150
					    free_usart3_rn_other_queue();//��������
						u3_senddata(usart3_send_temp.pstr,usart3_send_temp.len);
					 }
				 }
				else if(usart3_send_temp.type ==USART_RN)
				{
					 u3_senddata(usart3_send_temp.pstr,usart3_send_temp.len);
					if(usart3_send_temp.type ==USART_RN) u3_senddata((uint8_t*)"\r\n",2);

				}
				vPortFree(usart3_send_temp.pstr);
				#if 0


				 //�ж��������ͣ��Ƿ�����ݷ��͵�������
				 //������������������Ҫ�ӷ����������ģ���ָ����Ҫ�ں��油��\R\N
				 if(usart3_send_temp.type ==USART_SERVER)
				 {
					 //�ȴ��ź���
					 if(touch_flag&(0x0001<<14))
					 {
//						 while(!u3cansend);
					 if(xSemaphoreTake(SendenableMutexSemaphore,portMAX_DELAY))	//��ȡ�����ź���
						 {

						 }
					 }

					 dprintf("send server\r\n");
					 sprintf(at_cipsend,"AT+CIPSEND=%d\r\n",usart3_send_temp.len);
					 u3_senddata((uint8_t*)at_cipsend,strlen(at_cipsend));
					 vTaskDelay(150);
					 free_usart3_rn_other_queue();//��������

				 }


//				 taskENTER_CRITICAL();           //�����ٽ���
				 u3_senddata(usart3_send_temp.pstr,usart3_send_temp.len);

//				 taskEXIT_CRITICAL();            //�˳��ٽ���
				 if(usart3_send_temp.type ==USART_RN) u3_senddata((uint8_t*)"\r\n",2);
				 vPortFree(usart3_send_temp.pstr);
//				  if(usart3_send_temp.type ==USART_SERVER)  //vTaskDelay(100); //���������ݷ�����Ҫʱ��ϳ�
//				check_msg_queue();
				 u3cansend=0;
				 #endif
			 }
			 vTaskDelay(USART3_SEND_delay );
		}

}

void CanServer_Callback(TimerHandle_t xTimer)
{
	xSemaphoreGive(SendenableMutexSemaphore);					//�ͷ��ź���
}

#define U3toU1 0

 //����3��senddata
void u3_senddata(uint8_t *data,uint16_t length)
{
		uint16_t j;
//	i=length;		//�˴η������ݵĳ���
//	PTK_SERIAL++;
//	RTC_ONLINE=RTC_SEC;
		uart3 ->uart_write(data,length);
}






//end USART3task.c

