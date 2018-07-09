/*
 * usart4task.c
 *
 *  Created on: 2018年5月16日
 *      Author: Administrator
 */

#include "usart4task.h"
#include "usart1task.h"//调试printf
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
//模拟串口不进行分帧的操作，直接用FIFO的缓冲buff来做，每次发送FIFO接收到的数据个数，应用层直接处理这些数据
//触控板串口
#define delay_ms vTaskDelay
#define CMD_WAIT_MAX 5
#define EMSK_GPIO_CHECK_EXP_NORTN(EXPR)		CHECK_EXP_NOERCD(EXPR, error_exit)
#define BTN_ACTIVE_LOW   (0)

uint8_t usart4_fifo_data[USART4_FIFO_LEN];
//uint8_t usart4_send_data[USART4_SEND_LEN];
fifo_struct  usart4_fifo;//串口4的数据存储FIFO 接收fifo
//fifo_struct  usart4_send_fifo;
suart_struct uart4_ctr;//模拟串口4的控制参数
//suart_send_struct send_ctr4;

//与触控板通信控制
uint8_t wait_cmd=0,wait_cmd_count=0;
uint16_t touch_flag=0;//记录触控板的不同档位与模式：全局存储灯光档位，wifi灯状态 台灯开关状态、wifiset状态、升级状态、

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
#define GPIO_UART4_MASK		0x00030000		//portA的16-17Pin，接J3的7-8口
#define GPIO_UART4_LOW	  (0)
#define GPIO_UART4IN_MASK	0X00010000		//portA的16Pin，接J3的7口
#define GPIO_UART4OUT_MASK	0X00020000		//portA的17Pin，接J3的8口



//传递一个byte
void EXTI1_IRQHandler(void *ptr)
{

	BaseType_t xHigherPriorityTaskWoken;

//		uart4_ctr.flag = 0;
		#if 1//延时方案接收数据
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
		uart4_ctr.bytenum++;//可能需要判断数据量过大，进行断帧




}

uint8_t senddata_by_usart4(uint8_t*str,uint8_t len)//外部调用发送函数
{
	usart_task_data USART4_send_data;//USART3send数据
	uint8_t*p;
	BaseType_t err;

	USART4_send_data.type = USART_DIRECT;

	USART4_send_data.len  = len;
	p = pvPortMalloc(USART4_send_data.len);
	memcpy(p,str,USART4_send_data.len);
	USART4_send_data.pstr = p;

	err=xQueueSend(USART4_Send_Queue,&USART4_send_data,0);
	if(err==errQUEUE_FULL)   	//发送消息帧
	{
		//发送失败需要free内存
		vPortFree(USART4_send_data.pstr);
		dprintf("队列USART4_Send_Queue已满，数据发送失败!\r\n");
		return 0;
	}
	else return 1;
}

void u4app_send(uint8_t*str,uint8_t len)//内部调用函数
{
	#define DELAY_1bit 90
	#define	DELAY_1bit2	92

	uint8_t i,j,data;
//	taskENTER_CRITICAL();           //进入临界区
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
//	taskEXIT_CRITICAL();            //退出临界区
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
		if(xQueueReceive(USART4_Queue,&nlen,0))//接收数据
		{
			uint8_t i;
			p = pvPortMalloc(nlen);
			read_fifo(&usart4_fifo,p,nlen);

			#if 1//测试处理
			buffer =  pvPortMalloc(nlen+1);
			memcpy(buffer , p , nlen);
			buffer[nlen] = 0;
			dprintf("u4re %d:%s\r\n",nlen,buffer);
			vPortFree(buffer);

			#endif
			//处理触控板按键数据

//				for(i=0;i<nlen;i++)
			if(nlen==1)
				{
					touch_process(p[0]);
				}
			vPortFree(p);
		}
		if(xQueueReceive(USART4_Send_Queue,&usart_send4,0))//发送数据 发送后需要等待回执信息
		{
			#if 1//测试处理
			buffer =  pvPortMalloc(usart_send4.len+1);
			memcpy(buffer , usart_send4.pstr ,usart_send4.len);
			buffer[usart_send4.len] = 0;
			dprintf("u4sd %d:%s\r\n",usart_send4.len,buffer);
			vPortFree(buffer);
			#endif
			//开启定时器，检测几次回执信号，没有收到回执信号就多发送几次命令
			if(usart_send4.len==1)//发送长度为1 是发送给触控板的控制命令
			{
				if(usart_send4.pstr[0]&0x80)//是32进行控制，不是32回复触控板控制
				{
					if(wait_cmd_count&&(usart_send4.pstr[0]==wait_cmd))//是当前正在等待的指令 不进行操作
					{

					}
					else//没有指令在等待，或者出现新的需要等待的指令：开启周期定时器发送指令到发送队列
					{
						//关闭定时器
						xTimerStop(period_waittouch_Handle,0);
						wait_cmd = usart_send4.pstr[0];
						wait_cmd_count = CMD_WAIT_MAX;//cmd最大等待次数

						//开启定时器
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

//各个按键对应的功能
void touch_process(uint8_t touch_cmd)
{
	uint8_t back_cmd;
	if(touch_cmd&0x80)//控制信号
	{
		//回执给触控板
		back_cmd = touch_cmd&0x7f;
		senddata_by_usart4(&back_cmd,1);
		if(touch_cmd&(0x01<<6))//enable
		{
		if(touch_cmd&0x08)//sp =1
		{
			if(touch_cmd&(0x01<<5))//sp  = 1  power=1    0档(只有下行模式有)和四挡开灯
			{
				if(!(touch_cmd&0x07))//四挡开灯 bit2-0=0
				{
					#if DEBUG_TOUCH
					dprintf("touch re:power on\r\n");
					#endif
					touch_process_poweron();
				}
			}
			else//sp  = 1  power=0    wifiset   boot(只有下行控制有boot模式控制)
			{
				if(touch_cmd&(0x01<<1))//wifiset 进入wifi配置模式
				{
					#if DEBUG_TOUCH
					dprintf("touch re:wifiset start\r\n");
					#endif
					touch_flag|=(0x0001<<5);
					touch_flag&=(~(0x0001<<4));//记录wifi灯关闭

					//恢复wifi_config的任务
					//wifi配置任务
					start_config_task();

				}
				else//退出wifi配置模式
				{
					#if DEBUG_TOUCH
					dprintf("touch re:wifiset quit\r\n");
					#endif
					touch_flag&=(~(0x0001<<5));
					touch_flag&=(~(0x0001<<4));//记录wifi灯关闭
					//挂起wifi_config的任务
					stop_config_task(0);
				}
			}
		}
		else//sp = 0
		{
				if(touch_cmd&(0x01<<5))//sp  = 0  power=1    档位控制 1-8
				{
					#if DEBUG_TOUCH
					dprintf("touch re:light lv=%d\r\n",((touch_cmd&0x07)+1));
					#endif

					touch_process_lightswitch(((touch_cmd&0x07)+1),touch_cmd&(0x01<<4));

				}
				else//sp  = 0  power=0  关灯
				{
					if(!(touch_cmd&0x07))//关灯 bit2-0=0
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


	else //回执信号
	{
		if(wait_cmd_count)
		{
			if(touch_cmd==(wait_cmd&0x7f))
			{
				//关闭定时器
				xTimerStop(period_waittouch_Handle,0);
				wait_cmd_count = 0;
			}
		}
	}
}

void touch_process_poweroff(void)//poweroff的处理
{
		touch_flag|=(0x0001<<10);//触控板进行了操作标记
		touch_flag&=(~(0x0001<<7));//标记为关灯状态
		touch_flag&=0xfff0;//记录灯光档位为0档
		bright_mode(LIGHT_SWITCH,touch_flag&0x000F);

		touch_flag&=(~(0x0001<<9));
		touch_flag&=(~(0x0001<<8));//退出自动调光
		//记录wifi灯状态 关闭
		touch_flag&=(~(0x0001<<4));
		//发送0002
		sendpack2server(0x0002);
}

void touch_process_poweron(void)//poweron的处理 进入自动调光
{
	touch_flag|=(0x0001<<10);//触控板进行了操作标记
	touch_flag|=(0x0001<<7);//标记为开灯状态
	touch_flag&=0xfff0;
	touch_flag|=0x0004;//记录灯光档位为4档
	bright_mode(LIGHT_SWITCH,touch_flag&0x000F);//直接跳档
	delay_ms(300);

	//开启自动调光
	touch_flag|=(0x0001<<8);//标记为自动调光模式
	bright_mode(LIGHT_AUTO,touch_flag&0x000F);//直接跳档

	//发送包0002
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

void touch_process_lightswitch(uint8_t light,uint8_t wifi)//控制直接调到某个档位
{
	uint8_t temp;
	touch_flag|=(0x0001<<10);//触控板进行了操作标记
	if((touch_flag&0x000f)!=light)//是不同档位
	{
		//标记开灯
		touch_flag|=(0x0001<<7);
		//调节灯光到对应档位
		touch_flag&=0xfff0;
		touch_flag|=light;//记录灯光档位
		bright_mode(LIGHT_SWITCH,touch_flag&0x000F);
		#if 0//触控板按键功能
		if(light==1){
												temp = 1;
												xQueueSend(connect_to_server_Queue,&temp,10);
												vTaskResume(connect_to_server_Handler);
		}
		#endif

		//退出自动调光
		touch_flag&=(~(0x0001<<8));

		//判断是否进入睡眠模式
		if(light<4&&light>0)//可以进入睡眠模式 但是要判断魔豆是否连接且处于佩戴状态
		{

		}
		else//读书档，退出睡眠模式
		{
			touch_flag&=(~(0x0001<<9));

		}

		//记录wifi灯状态
		if(wifi)
		{
			touch_flag|=(0x0001<<4);
		}
		else
		{
			touch_flag&=(~(0x0001<<4));
		}

		//发送0002
		sendpack2server(0x0002);
			//控制wifi灯
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

//定时器周期发送按键控制，等触控板响应
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
	//关闭定时器
		wait_cmd_count = 0;
		xTimerStop(period_waittouch_Handle,0);
	}
}

//给触控板发送命令
void mcu_sendcmd2touch(uint8_t cmd)
{
	senddata_by_usart4(&cmd,1);
}

//32控制灯光和触控板，进入不同的模式 8003[22]  的响应
void server_control_light(uint8_t mode)
{
	if(mode)//
	{
		if(mode&0x10)//自动调光
		{
			mcu_sendcmd2touch(Power_On|0x10);//开机并且，打开wifi灯
			touch_process_poweron();

		}
		else if(mode&0x20)//调档位 这里单独写，服务器控制的不能退出自动调光模式
		{
			//控制触控板到几档
			mcu_sendcmd2touch(0xf0|((mode&0x0f)-1));

			//调节灯光到对应档位
			touch_flag&=0xfff0;
			touch_flag|=(mode&0x0f);//记录灯光档位
			bright_mode(LIGHT_SWITCH,touch_flag&0x000F);

		//判断是否进入睡眠模式
		if((mode&0x0f)<4)//可以进入睡眠模式 但是要判断魔豆是否连接且处于佩戴状态
		{

		}
		else//读书档，退出睡眠模式
		{
			touch_flag&=(~(0x0001<<9));

		}

		//记录wifi灯状态
		touch_flag|=(0x0001<<4);

		//发送0002
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
