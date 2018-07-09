/*
 * initsys.c
 *
 *  Created on: 2018��5��4��
 *      Author: Administrator
 */

#include "initsys.h"
//
#include "string.h"
#include "stdlib.h"

#include "malloc.h"
#include "timer.h"

#include "usart3task.h"
#include "usart1task.h"//����printf
#include "encrypt.h"

#include "task_config.h"

//������ض���
uint8_t DEVICE_ID[8];
SYS_CONFIG sys_config;


void init_system(void)
{
		sys_config_init();
		default_sys_config();//����ʱʹ�ã�ʵ�ʴ�flash��ȡ
		ALL_TIM_init();
		set_rtc_sec(0);//��ʼ��rtcʱ��
		device_id_encrypt(sys_config.device_id,DEVICE_ID);
}

void default_sys_config(void)
{
	sys_config.pkt_version = 0x00;
    sys_config .device_id =0x44;
}

void sys_config_init(void)
{
	default_sys_config();
}
//end initsys.c

