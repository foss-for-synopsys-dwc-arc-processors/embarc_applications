/*
 * initsys.c
 *
 *  Created on: 2018年5月4日
 *      Author: Administrator
 */

#include "initsys.h"
//
#include "string.h"
#include "stdlib.h"

#include "malloc.h"
#include "timer.h"

#include "usart3task.h"
#include "usart1task.h"//调试printf
#include "encrypt.h"

#include "task_config.h"

//函数相关定义
uint8_t DEVICE_ID[8];
SYS_CONFIG sys_config;


void init_system(void)
{
		sys_config_init();
		default_sys_config();//调试时使用，实际从flash读取
		ALL_TIM_init();
		set_rtc_sec(0);//初始化rtc时间
		device_id_encrypt(sys_config.device_id,DEVICE_ID);
}

void default_sys_config(void)
{
	uint8_t i;

	sys_config.pkt_version = 0x00010000;

    //台灯升级标志位
    sys_config.boot_flag = 0x66;//需要boot升级MCU主控时，修改标志位，重启MCU
     //是否初始化了flash
    sys_config .flash_init = 0x55;

    //魔豆相关的数据
    //MP3的更新数据
    //台灯版本号 ID 等
    sys_config .device_id =1000000+8;		//默认id 在flash中不能修改				+5 MP3测试

    sys_config.LAMP_MODEL[0]=0X00;      //台灯型号
    sys_config.LAMP_MODEL[1]=0X00;
    sys_config.LAMP_MODEL[2]=0X00;
    sys_config.LAMP_MODEL[3]=0X01;

    sys_config.LAMP_HV[0] =0x00;        //台灯硬件版本号
    sys_config.LAMP_HV[1] =0x00;
    sys_config.LAMP_HV[2] =0x01;
    sys_config.LAMP_HV[3] =0x00;

    sys_config. LAMP_SV[0]=0x08;        //台灯软件版本号
    sys_config. LAMP_SV[1]=0x00;
    sys_config. LAMP_SV[2]=0x01;
    sys_config. LAMP_SV[3]=0x00;

		sys_config. M_SERIAL[0]=0X00;       //魔豆序列号
		sys_config. M_SERIAL[1]=0X00;
		sys_config. M_SERIAL[2]=0X00;
		sys_config. M_SERIAL[3]=0X00;
		sys_config. M_SERIAL[4]=0X03;
		sys_config. M_SERIAL[5]=0X00;
		sys_config. M_SERIAL[6]=0X00;
		sys_config. M_SERIAL[7]=0X00;

    sys_config. S_HW[0]=0x00;           //魔豆硬件版本号
    sys_config. S_HW[1]=0x00;
    sys_config. S_HW[2]=0x01;
    sys_config. S_HW[3]=0x00;

    sys_config. S_SW[0]=0x00;           //魔豆软件版本号
    sys_config. S_SW[1]=0x08;
    sys_config. S_SW[2]=0x00;
    sys_config. S_SW[3]=0x00;

    sys_config. S_SERIAL[0]=0X87;       //魔豆序列号
    sys_config. S_SERIAL[1]=0X65;
    sys_config. S_SERIAL[2]=0X43;
    sys_config. S_SERIAL[3]=0X21;
    sys_config. S_SERIAL[4]=0X0F;
    sys_config. S_SERIAL[5]=0XED;
    sys_config. S_SERIAL[6]=0XCB;
    sys_config. S_SERIAL[7]=0XA9;

		sys_config.WIFI_SW [0]=0x08;//wifi版本号
    sys_config.WIFI_SW [1]=0x00;
    sys_config.WIFI_SW [2]=0x01;
		sys_config.WIFI_SW [3]=0x00;

    //台灯的报警角度时间等数据
    sys_config .ANGLE_ALARM1 =20; //前倾 默认20度
    sys_config .ANGLE_ALARM2 =30; //后倾 默认30度
    sys_config .ALERT_TIME1  =5 ; //首次报警时间，默认5s
    sys_config .ALERT_TIME2  =180;//重复报警间隔
    sys_config .MUTE_START   =0;  //默认静音开始时间0:00
    sys_config .MUTE_END     =420;//默认静音结束时间7:00
		sys_config .SLEEP_START  =0;  //默认静音开始时间0:00
    sys_config .SLEEP_END    =1440;//默认静音结束时间24:00

		//MP3 ID初始化
		for(i=0;i<80;i++)
		{
			sys_config.MP3_ID[i]=0;
		}
    sys_config.MP3_STA=1;

//    write_sys_config();
}

void sys_config_init(void)
{
	#if 1
	default_sys_config();//调试时使用，实际从flash读取
	#else
	//实际使用从flash读取
	#endif
}
//end initsys.c

