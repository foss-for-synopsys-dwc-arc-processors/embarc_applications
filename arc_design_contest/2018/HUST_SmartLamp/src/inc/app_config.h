/*
 * app_config.h
 *
 *
 *      Author: Administrator
 */

#ifndef APPINC_APP_CONFIG_H_
#define APPINC_APP_CONFIG_H_

//可以在头文件中引用
#include "embARC.h"
#include "embARC_debug.h"
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*******************宏定义***************************/
//调试用宏定义
#define ASSERT_PRINTF_ENABLE 1//调试的dprintf使能

//任务宏定义


//触控板按键定义
#define Bright_0  0xEA//亮wifi灯和电源灯，档位为0
#define Bright_1  0xe0//亮度灯
#define Bright_2  0xe1
#define Bright_3  0xe2
#define Bright_4  0xe3
#define Bright_5  0xe4
#define Bright_6  0xe5
#define Bright_7  0xe6
#define Bright_8  0xe7

#define Power_On  0xe8  //触控板的电源灯
#define Power_Off 0xc0
#define Wifi_Set  0xca//长按电源灯
#define Wifi_Quit 0xc8
#define	Wifi_On	  0xde//WiFi设置灯
#define	Wifi_Off  0xce
#define	TOUCH_ON  0xc0 //触控板不响应
#define	TOUCH_OFF 0x80
#define	Boot_Start	0xcc
#define	Boot_End		0xc8
//
//receive队列的消息后，如果队列中有指针，都需要free操作
//消息队列
#define KEYMSG_Q_NUM          2  		      //按键消息队列的数量
#define USART1_Q_NUM          4 					//串口1队列长度 USART1_NUM  底层缓冲数组数量
#define USART1_TASK_Q_NUM     4  					//串口1帧数据队列长度
#define USART3_Q_NUM          8  					//串口3队列长度 USART3_NUM  底层缓冲数组数量
#define USART3_TASK_SERVER_Q_NUM       10  					//串口3帧SERVER数据队列长度
#define USART3_TASK_RN_OTHER_Q_NUM     10  					//串口3帧\R\N和没有标记的数据 数据队列长度
#define USART3_Send_Q_NUM     10						//串口3发送队列
#define pack2server_Q_NUM     5						//发送给服务器的包，进行组帧操作
#define connect_to_server_Q_NUM     1						//控制连接服务器
#define USART4_Q_NUM					10
#define USART4SEND_Q_NUM			10
#define USART5_Q_NUM					10
#define USART5SEND_Q_NUM			10

//串口缓冲数量
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define USART1_PER_LEN   50		//每个缓冲buff大小
#define USART1_NUM     USART1_Q_NUM //总共缓冲的buff数量 少于8个，因为用的u8做的标记

#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收
#define USART3_PER_LEN  200		//每个缓冲buff大小
#define USART3_NUM       USART3_Q_NUM//总共缓冲的buff数量 少于8个，因为用的u8做的标记

#define USART4_PER_LEN  50		//每个缓冲buff大小
#define USART4_NUM       USART4_Q_NUM//总共缓冲的buff数量 少于8个，因为用的u8做的标记
#define USART5_NUM       USART5_Q_NUM//总共缓冲的buff数量 少于8个，因为用的u8做的标记

//extern u8 usart3_data_all[USART3_NUM][USART3_PER_LEN];//串口3底层缓冲buff

/******************定时器相关宏定义********************/
//wifi配置使用的两个定时器
//60s定时器
//TIM60
#define WIFI_TIM60S_TICKS 60*1000/portTICK_PERIOD_MS
//TIM_ID
#define WIFI_TIM60S_ID    1

//10s定时器
//TIM10
#define WIFI_TIM10S_TICKS 10*1000/portTICK_PERIOD_MS
//TIM_ID
#define WIFI_TIM10S_ID    2

//定时发送包0002 40s或者5s
//ticks
#define PKG0002_40S_TICKS 40*1000/portTICK_PERIOD_MS
#define PKG0002_5S_TICKS  5*1000/portTICK_PERIOD_MS
//TIM_ID
#define PKG0002_TIM_ID    3



//检测触控板回执信号定时器 50MS
//ticks
#define WAIT_TOUCH_TICKS 250/portTICK_PERIOD_MS
//TIM_ID
#define WAIT_TOUCH_ID    4

#define WIFI_1MS_TICKS	1/portTICK_PERIOD_MS
#define WIFI_TIM1MS_ID 5

//定时使能发送给服务器数据5s
//ticks
#define CanServer_TICKS 5*1000/portTICK_PERIOD_MS
//TIM_ID
#define CanServer_ID    6

//定时调节灯光
//ticks
#define Bright_TICKS 200/portTICK_PERIOD_MS
//TIM_ID
#define Bright_ID    7
/******************任务相关宏定义********************/
//起始任务
//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小
#define START_STK_SIZE 		128

//按键检测任务
//任务优先级
#define TASK1_TASK_PRIO		2
//任务堆栈大小
#define TASK1_STK_SIZE 		64
//任务运行间隔 ms
#define task1_task_delay      20//

//按键处理任务
//任务优先级
#define KEYPROCESS_TASK_PRIO 3
//任务堆栈大小
#define KEYPROCESS_STK_SIZE  256
//任务运行间隔 ms
#define Keyprocess_task_delay 20//


//串口1任务 数据分帧 组帧
//任务优先级
#define USART1_TASK_PRIO 28
//任务堆栈大小
#define USART1_STK_SIZE  128
//任务运行间隔 ms
#define USART1_task_delay     10//任务运行间隔 ms

//串口1分帧数据处理
//任务优先级
#define USART1_APP_PRIO  13//比接收任务优先级低
//任务堆栈大小
#define USART1_APP_SIZE  256
//任务运行间隔 ms
#define USART1_APP_delay     100//任务运行间隔 ms



//8266 smartconfig 任务
//任务优先级
#define SMARTCONFIG_TASK_PRIO  19//低于串口3和触控板检测优先级，就可以，配置时只有这几个任务在执行
//任务堆栈大小
#define SMARTCONFIG_TASK_SIZE  256
//任务运行间隔 ms
#define SMARTCONFIG_TASK_delay 10//任务运行间隔 ms

//8266 connect t0 server 任务
//任务优先级
#define connect_to_server_PRIO  18//低于smart
//任务堆栈大小
#define connect_to_server_SIZE  256
//任务运行间隔 ms
#define connect_to_server_delay 10//任务运行间隔 ms


//串口3任务 数据分帧 组帧
//任务优先级
#define USART3_TASK_PRIO 29
//任务堆栈大小
#define USART3_STK_SIZE  128
//任务运行间隔 ms
#define USART3_task_delay     10//任务运行间隔 ms

//串口3任务 接收后续数据超时，认为数据错误 free内存，开始idel接收模式
//任务优先级
#define FREE_ERRORMEM_PRIO 30
//任务堆栈大小
#define FREE_ERRORMEM_SIZE  64
//任务运行间隔 ms
#define FREE_ERRORMEM_delay     10//任务运行间隔 ms

//串口3数据发送任务
//任务优先级
#define USART3_SEND_PRIO  20//比接收任务优先级低
//任务堆栈大小
#define USART3_SEND_SIZE  256+256
//任务运行间隔 ms
#define USART3_SEND_delay     20//任务运行间隔 ms

//串口3服务器数据处理
//任务优先级
#define USART3_APP_PRIO  14//比接收任务优先级低
//任务堆栈大小
#define USART3_APP_SIZE  256
//任务运行间隔 ms
#define USART3_APP_delay     20//任务运行间隔 ms


//组包数据处理
//任务优先级
#define pack2server_PRIO  13//比接收任务优先级低
//任务堆栈大小
#define pack2server_SIZE  256
//任务运行间隔 ms
#define pack2server_delay 30

//串口4任务
//任务优先级
#define USART4_APP_PRIO  15//
//任务堆栈大小
#define USART4_APP_SIZE  256
//任务运行间隔 ms
#define USART4_APP_delay     20//任务运行间隔 ms

//串口5任务
//任务优先级
#define USART5_APP_PRIO  15//
//任务堆栈大小
#define USART5_APP_SIZE  256
//任务运行间隔 ms
#define USART5_APP_delay     20//任务运行间隔 ms

//人体近距离检测
//任务优先级
#define IRF_APP_PRIO  10//
//任务堆栈大小
#define IRF_APP_SIZE  128
//任务运行间隔 ms
#define IRF_APP_delay     100//任务运行间隔 ms
/**************状态枚举*****************************/
//串口分帧后，数据的类型
typedef enum
{
	USART_RN,			//接收到\r\n结束的数据
	USART_SERVER,	//接收到服务器下发的协议格式的数据
	USART_DIRECT, //直接发送的数据，用在退出透传发送+
	USART_OTHER,	//格式不对的数据

}USART_DATA_TYPE;
//串口task分帧组帧的流程标记
typedef enum
{
	USART_DATA_NORMAL_START,//普通帧开始
	USART_DATA_NORMAL_STOP, //普通帧结束
	USART_DATA_RN_STOP,     //\r\n帧结束
	USART_DATA_SERVER_START,//接收到服务器协议的起始数据
	USART_DATA_SERVER_STOP, //接收到服务器协议的结束数据
	USART_DATA_WAIT,//等待下一个缓冲buff的数据进行组帧
	USART_DATA_IDEL,//空闲没有收到过数据
}USART_TASK_PROCESS;
/**************结构体*******************************/
//task对串口数据分帧处理的结果
typedef struct
{
  uint8_t* pstr;//数据存储位置
  uint32_t len;//数据总长度
	USART_DATA_TYPE type;//分帧得到数据的类型
}usart_task_data;

//串口数据传递类型 串口队列
typedef struct
{
	uint8_t buff_now;//数据存储数组
	uint8_t len;//数据长度
}usart_dat;

//FIFO结构体
typedef struct
{
	uint8_t* pstr;//存储数组
	uint16_t total_len;//FIFO总大小
	uint16_t now_len;//当前FIFO数据个数
	uint16_t read_pos;
	uint16_t write_pos;
}fifo_struct;

//模拟串口用定时器控制接收一个byte的控制变量
typedef struct
{
//	uint8_t flag;
	uint8_t count;
	uint8_t byte;
	uint8_t timeout;
	uint8_t bytenum;
}suart_struct;
//模拟串口用定时器控制发送一个byte的控制变量
typedef struct
{
	uint8_t bit_flag;//byte的第几位
	uint8_t byte_now;//当前发送的数据缓存

}suart_send_struct;



//系统参数结构体

#pragma pack (4)
typedef struct
{
	uint32_t pkt_version;//
    uint32_t device_id;//台灯id
}SYS_CONFIG;
extern SYS_CONFIG sys_config;






#endif /* APPINC_APP_CONFIG_H_ */
