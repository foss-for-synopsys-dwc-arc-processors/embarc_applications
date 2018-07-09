/*
 * usart1task.c
 *
 *  Created on: 2018年5月4日
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


#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误
//uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
uint8_t usart_data_all[USART1_NUM][USART1_PER_LEN];//串口1缓冲buff
uint8_t usart_buff_flag=1;//标记哪些数组被使用了，下次往哪个数组中存 默认使用第一个数组
//uint8_t buff_now=0;//当前使用的缓冲数组序号
usart_dat usart1_data;
uint8_t usart1_time_out=0;//串口1接收超时计时
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记
//任务句柄
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


//找到未被使用的缓冲buff，
//返回0-（USART1_NUM-1）表示相应的数组可以使用
//返回USART1_NUM表示没有空闲缓冲buff
//存到 buff_now中
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
//处理完缓冲buff数据后，释放buff的占用状态
void release_buff(uint8_t buff_num)
{
	if(buff_num<USART1_NUM)
	{
		memset(usart_data_all[buff_num],0,USART1_PER_LEN);//清除数据接收缓冲区USART_RX_BUF,用于下一次数据接收
		usart_buff_flag&=~(0x01<<buff_num);

	}
	if(usart1_data.buff_now==USART1_NUM) occupy_buff();
}

QueueHandle_t USART1_Queue;


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t Res;
	BaseType_t xHigherPriorityTaskWoken;


		uart1->uart_read(&Res, 1);	//读取接收到的数据


			usart1_time_out=0x80;//计数器清空
		if(usart1_data.buff_now<USART1_NUM)
			usart_data_all[usart1_data.buff_now][USART_RX_STA++] = Res;
		else
		{
			dprintf("usart1buff no space\r\n");
		}

		if(USART_RX_STA>USART1_PER_LEN-1)
		{
			usart1_time_out=0x00;//接收完成一次数据，不再进行超时计时。下次收到数据再计时
			usart1_data.len=USART_RX_STA&0x7fff;
			xQueueSendFromISR(USART1_Queue,&usart1_data,&xHigherPriorityTaskWoken);//向队列中发送数据
			USART_RX_STA = 0;
			occupy_buff();
			portYIELD_FROM_ISR();//如果需要的话进行一次任务切换
		}


}
#endif


QueueHandle_t USART1_Task_Queue;//串口task处理后的数据队列 都是分包好的一帧一帧的数据
USART_TASK_PROCESS usart1_task_process=USART_DATA_IDEL;//数据处理流程标记
uint8_t usart1_task_time_out=0;//串口1 task接收超时计时
uint16_t server_data_remain;//server data还有多少没有复制过来
usart_dat usart1_temp;//接收底层串口缓冲数据队列
usart_task_data usart1_task_temp;//发送到usart1task队列中

void USART1_task(void *pvParameters)
{

	uint8_t temp_pos;//暂存标志位的位置信息
	uint8_t data_num_remain=0;//当前处理了数据.剩余数据长度 要把接收到的usart1_temp.len长度数据处理完
	uint8_t* data_p;//存储数据的地址 申请内存时，根据包内信息申请对应的大小内存
	uint8_t* data_temp;//底层串口数据指针
	BaseType_t err;
	printf("USART1_task success\r\n");
    while(1)
    {
		if(xQueueReceive(USART1_Queue,&usart1_temp,0))//接收到底层串口数据
			{
				data_num_remain = usart1_temp.len;
				data_temp = usart_data_all[usart1_temp.buff_now];
				switch(usart1_task_process)
				{
					case USART_DATA_IDEL:
					{
						CHECK_BUFF:while(data_num_remain)
						{
							temp_pos = 0xff;//findpos(server_start,4,data_temp,data_num_remain);//串口1默认不检查服务器数据，只处理\r\n结尾数据
							if(temp_pos==0xff)//没有服务器数据的起始标志 认为是普通数据
							{
								temp_pos=findpos(rn_stop,2,data_temp,data_num_remain);
								if(temp_pos==0xFF)//没有\r\n结束标记
								{
									data_p=pvPortMalloc(data_num_remain);//不传递\r\n出去，只传递前面的数据
									memcpy(data_p,data_temp,data_num_remain);
									usart1_task_temp.len = data_num_remain;
									usart1_task_temp.pstr=data_p;
									usart1_task_temp.type=USART_OTHER;
									err=xQueueSend(USART1_Task_Queue,&usart1_task_temp,10);
									if(err==errQUEUE_FULL)   	//发送消息帧
									{
										vPortFree(usart1_task_temp.pstr);
										printf("队列USART1_Task_Queue已满，数据发送OTHER失败!\r\n");
									}
									data_num_remain = 0;
									break;
									}
									else//寻找到\r\n 发送当前帧，判断缓冲buff剩余数据是否处理完毕，没有处理完毕就继续 USART_DATA_IDEL 的处理
									{
										if(temp_pos)
										{
											data_p=pvPortMalloc(temp_pos);//不传递\r\n出去，只传递前面的数据
											memcpy(data_p,data_temp,temp_pos);
											usart1_task_temp.len = temp_pos;
											usart1_task_temp.pstr=data_p;
											usart1_task_temp.type=USART_RN;
											err=xQueueSend(USART1_Task_Queue,&usart1_task_temp,10);
											if(err==errQUEUE_FULL)   	//发送消息帧
											{
												vPortFree(usart1_task_temp.pstr);
												dprintf("队列USART1_Task_Queue已满，数据发送RN失败!\r\n");
											}
										}
										data_num_remain-=temp_pos+2;
										data_temp+=temp_pos+2;
									}
								}
								else if(temp_pos==0x00)//起始位置就是起始标志
								{
									server_data_remain = ((data_temp[4])<<8)+data_temp[5];//这一帧数据长度
									if(data_num_remain>5)//至少包含长度信息，不然就不能继续
									{
									if(server_data_remain>data_num_remain)//一帧跨越几个包，需要几个包进行组帧
									{
										usart1_task_time_out=0x80;
										data_p=pvPortMalloc(server_data_remain);
										usart1_task_temp.len = server_data_remain;
										usart1_task_temp.pstr=data_p;
										memcpy(data_p,data_temp,data_num_remain);
										data_p+=data_num_remain;
										server_data_remain-=data_num_remain;//帧还需要的长度
										usart1_task_process = USART_DATA_SERVER_START;//SERVER组帧的进程
										data_num_remain = 0;
										break;
									}
									else//一帧server 在一个包里面就能组完
									{
										//先判断结束位是否正确，再执行操作
										if(strsame(server_stop,data_temp+server_data_remain-4,4))//验证失败，是否把数据当做other数据发出？这里的处理是丢弃
										{
											data_p=pvPortMalloc(server_data_remain);
											usart1_task_temp.len = server_data_remain;
											usart1_task_temp.pstr=data_p;
											usart1_task_temp.type=USART_SERVER;
											memcpy(data_p,data_temp,server_data_remain);
											err=xQueueSend(USART1_Task_Queue,&usart1_task_temp,10);
											if(err==errQUEUE_FULL)   	//发送消息帧
											{
													//发送失败需要free内存
												  vPortFree(usart1_task_temp.pstr);
													dprintf("队列USART1_Task_Queue已满，数据发送单包内一帧server数据失败!\r\n");
											}
										}
									}
										{
											data_num_remain-=server_data_remain;
											data_temp+=server_data_remain;
										}
									}
									else//不包括起始的6位 直接丢弃
									{
										data_num_remain = 0;
										break;
									}
								}
								else//起始位置在中间 也就是服务器数据之前有其他数据 调整到起始位置就是起始标志
								{
										data_num_remain-=temp_pos;
										data_temp+=temp_pos;
								}
						}
						//处理完数据，释放buff的占用
						release_buff(usart1_temp.buff_now);//释放buff的占用状态
					}
							break;
						case USART_DATA_SERVER_START:
						{
							{
								if(server_data_remain>data_num_remain)//跨包组帧
								{
									usart1_task_time_out=0x80;
									memcpy(data_p,data_temp,data_num_remain);
									data_p+=data_num_remain;
									server_data_remain-=data_num_remain;//帧还需要的长度
									usart1_task_process = USART_DATA_SERVER_START;//SERVER组帧的进程
									data_num_remain = 0;
									break;
								}
								else//一包内结束组帧
								{
									usart1_task_time_out=0x00;
									usart1_task_process = USART_DATA_IDEL;
									memcpy(data_p,data_temp,server_data_remain);
									if(strsame(server_stop,usart1_task_temp.pstr+usart1_task_temp.len-4,4))
									{
										usart1_task_temp.type=USART_SERVER;
										err=xQueueSend(USART1_Task_Queue,&usart1_task_temp,10);
										if(err==errQUEUE_FULL)   	//发送消息帧
										{
												//发送失败需要free内存
										  vPortFree(usart1_task_temp.pstr);
										  dprintf("队列USART1_Task_Queue已满，数据发送多包内一帧server数据失败!\r\n");
										}
									}
									else//数据不对，需要free内存
									{
										vPortFree(usart1_task_temp.pstr);
									}
									data_num_remain-=server_data_remain;
									data_temp+=server_data_remain;

									{//组帧完毕还有数据没有处理
										if(data_num_remain)
										{//再次启动数据检查
											goto CHECK_BUFF;//不一定有效，继续检查buff数据
										}
									}
								}
							}
						//处理完数据，释放buff的占用
						release_buff(usart1_temp.buff_now);//释放buff的占用状态
						}
						break;
					}
			 }

			vTaskDelay(USART1_task_delay);      //延时10ms，也就是10个时钟节拍
    }
}


#if 0 //串口1不需要释放多余内存 只接收处理\R\N数据 不进行跨包组帧操作
//二值信号量句柄
SemaphoreHandle_t BinarySemaphore_free;	//二值信号量句柄


//USART1_task中会存在多包组帧数据出现错误的情况，需要进行free 错误内存的操作
void FREE_ERRORMEM_task(void *pvParameters)
{
	BaseType_t err;
	printf("FREE_ERRORMEM_task success\r\n");
	while(1)
	{
		if(BinarySemaphore_free!=NULL)
		{
			err=xSemaphoreTake(BinarySemaphore_free,portMAX_DELAY);	//获取信号量
//			err=xSemaphoreTake(BinarySemaphore_free,0);	//获取信号量
			if(err==pdTRUE)										//获取信号量成功
			{
				usart1_task_process = USART_DATA_IDEL;
				vPortFree(usart1_task_temp.pstr);//在中断中free会出错，需要到外部task中进行操作
				printf("free_mem sucess!\r\n");
			}
			else if(err==pdFALSE)
			{
				vTaskDelay(FREE_ERRORMEM_delay);     //延时10ms，也就是10个时钟节拍
			}
		}
		      //延时10ms，也就是10个时钟节拍
	}
}




#endif

//串口1 APP层处理任务 ***测试功能，回传
void usart1_app_task(void *pvParameters)
{
	usart_task_data usart1_app_temp;//接收USART3_task 发出的服务器分帧数据
	uint8_t* buffer;
	BaseType_t err;
	dprintf("usart1_app_task start!\r\n");
	while(1)
	{
	if(xQueueReceive(USART1_Task_Queue,&usart1_app_temp,0))//接收到服务器数据
	{
		buffer=pvPortMalloc(usart1_app_temp.len+1);
    memcpy(buffer,usart1_app_temp.pstr,usart1_app_temp.len);
		buffer[usart1_app_temp.len]=0;
		printf("U1:%s\r\n",buffer);//正常使用printf 不是调试
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
			if(err==errQUEUE_FULL)   	//发送消息帧
				{
					printf("队列connect_to_server_Queue已满，1数据发送失败!\r\n");
				}
			vTaskResume(connect_to_server_Handler);
		}
#endif

#if 0//使用串口3，把板子模拟成串口使用
			err=xQueueSend(USART3_Send_Queue,&usart1_app_temp,10);
			if(err==errQUEUE_FULL)   	//发送消息帧
			{
				//发送失败需要free内存
				vPortFree(usart1_app_temp.pstr);
//				pkt_serial--;
				dprintf("队列USART3_Send_Queue已满，rn发送失败!\r\n");
			}
#else
		vPortFree(usart1_app_temp.pstr);//如果发送给uart3，就会被清除
#endif
		vPortFree(buffer);
	}
	vTaskDelay(USART1_APP_delay/10);
	}
}

//不执行任何操作
void printf_nosense(char *format, ...)
{
	//
}


//end usart1task.c

