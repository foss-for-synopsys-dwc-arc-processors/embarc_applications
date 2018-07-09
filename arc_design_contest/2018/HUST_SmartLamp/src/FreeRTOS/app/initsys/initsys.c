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
	uint8_t i;

	sys_config.pkt_version = 0x00010000;

    //̨��������־λ
    sys_config.boot_flag = 0x66;//��Ҫboot����MCU����ʱ���޸ı�־λ������MCU
     //�Ƿ��ʼ����flash
    sys_config .flash_init = 0x55;

    //ħ����ص�����
    //MP3�ĸ�������
    //̨�ư汾�� ID ��
    sys_config .device_id =1000000+8;		//Ĭ��id ��flash�в����޸�				+5 MP3����

    sys_config.LAMP_MODEL[0]=0X00;      //̨���ͺ�
    sys_config.LAMP_MODEL[1]=0X00;
    sys_config.LAMP_MODEL[2]=0X00;
    sys_config.LAMP_MODEL[3]=0X01;

    sys_config.LAMP_HV[0] =0x00;        //̨��Ӳ���汾��
    sys_config.LAMP_HV[1] =0x00;
    sys_config.LAMP_HV[2] =0x01;
    sys_config.LAMP_HV[3] =0x00;

    sys_config. LAMP_SV[0]=0x08;        //̨������汾��
    sys_config. LAMP_SV[1]=0x00;
    sys_config. LAMP_SV[2]=0x01;
    sys_config. LAMP_SV[3]=0x00;

		sys_config. M_SERIAL[0]=0X00;       //ħ�����к�
		sys_config. M_SERIAL[1]=0X00;
		sys_config. M_SERIAL[2]=0X00;
		sys_config. M_SERIAL[3]=0X00;
		sys_config. M_SERIAL[4]=0X03;
		sys_config. M_SERIAL[5]=0X00;
		sys_config. M_SERIAL[6]=0X00;
		sys_config. M_SERIAL[7]=0X00;

    sys_config. S_HW[0]=0x00;           //ħ��Ӳ���汾��
    sys_config. S_HW[1]=0x00;
    sys_config. S_HW[2]=0x01;
    sys_config. S_HW[3]=0x00;

    sys_config. S_SW[0]=0x00;           //ħ������汾��
    sys_config. S_SW[1]=0x08;
    sys_config. S_SW[2]=0x00;
    sys_config. S_SW[3]=0x00;

    sys_config. S_SERIAL[0]=0X87;       //ħ�����к�
    sys_config. S_SERIAL[1]=0X65;
    sys_config. S_SERIAL[2]=0X43;
    sys_config. S_SERIAL[3]=0X21;
    sys_config. S_SERIAL[4]=0X0F;
    sys_config. S_SERIAL[5]=0XED;
    sys_config. S_SERIAL[6]=0XCB;
    sys_config. S_SERIAL[7]=0XA9;

		sys_config.WIFI_SW [0]=0x08;//wifi�汾��
    sys_config.WIFI_SW [1]=0x00;
    sys_config.WIFI_SW [2]=0x01;
		sys_config.WIFI_SW [3]=0x00;

    //̨�Ƶı����Ƕ�ʱ�������
    sys_config .ANGLE_ALARM1 =20; //ǰ�� Ĭ��20��
    sys_config .ANGLE_ALARM2 =30; //���� Ĭ��30��
    sys_config .ALERT_TIME1  =5 ; //�״α���ʱ�䣬Ĭ��5s
    sys_config .ALERT_TIME2  =180;//�ظ��������
    sys_config .MUTE_START   =0;  //Ĭ�Ͼ�����ʼʱ��0:00
    sys_config .MUTE_END     =420;//Ĭ�Ͼ�������ʱ��7:00
		sys_config .SLEEP_START  =0;  //Ĭ�Ͼ�����ʼʱ��0:00
    sys_config .SLEEP_END    =1440;//Ĭ�Ͼ�������ʱ��24:00

		//MP3 ID��ʼ��
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
	default_sys_config();//����ʱʹ�ã�ʵ�ʴ�flash��ȡ
	#else
	//ʵ��ʹ�ô�flash��ȡ
	#endif
}
//end initsys.c

