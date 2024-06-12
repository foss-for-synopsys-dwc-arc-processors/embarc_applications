/*
 * app_config.h
 *
 *  Created on: 2018��5��3��
 *      Author: Administrator
 */

#ifndef APPINC_APP_CONFIG_H_
#define APPINC_APP_CONFIG_H_

//������ͷ�ļ�������
#include "embARC.h"
#include "embARC_debug.h"
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*******************�궨��***************************/
//�����ú궨��
#define ASSERT_PRINTF_ENABLE 1//���Ե�dprintfʹ��

//����궨��
#define RX_LEN_DATA 1024*2//1024//�ӷ��������չ̼���һ�εĹ̼�����

#define SYS_CONFIG_START_ADDR 518144 //ϵͳ����sys_config �洢�ڷ�flash�е���ʼ��ַ
#define MCU_BIN_LENGTH_ADDR	522240	//MCU�̼����ȴ洢��flash�е���ʼ��ַλ ����4
#define POWER_ON_VOICE_ADDR	MCU_BIN_LENGTH_ADDR+4	//MP3��mcu�������flag ����1 ��sys_config �������˱�־

//���ذ尴������
#define Bright_0  0xEA//��wifi�ƺ͵�Դ�ƣ���λΪ0
#define Bright_1  0xe0//���ȵ�
#define Bright_2  0xe1
#define Bright_3  0xe2
#define Bright_4  0xe3
#define Bright_5  0xe4
#define Bright_6  0xe5
#define Bright_7  0xe6
#define Bright_8  0xe7

#define Power_On  0xe8  //���ذ�ĵ�Դ��
#define Power_Off 0xc0
#define Wifi_Set  0xca//������Դ��
#define Wifi_Quit 0xc8
#define	Wifi_On	  0xde//WiFi���õ�
#define	Wifi_Off  0xce
#define	TOUCH_ON  0xc0 //���ذ岻��Ӧ
#define	TOUCH_OFF 0x80
#define	Boot_Start	0xcc
#define	Boot_End		0xc8
//
//receive���е���Ϣ�������������ָ�룬����Ҫfree����
//��Ϣ����
#define KEYMSG_Q_NUM          2  		      //������Ϣ���е�����
#define USART1_Q_NUM          4 					//����1���г��� USART1_NUM  �ײ㻺����������
#define USART1_TASK_Q_NUM     4  					//����1֡���ݶ��г���
#define USART3_Q_NUM          8  					//����3���г��� USART3_NUM  �ײ㻺����������
#define USART3_TASK_SERVER_Q_NUM       10  					//����3֡SERVER���ݶ��г���
#define USART3_TASK_RN_OTHER_Q_NUM     10  					//����3֡\R\N��û�б�ǵ����� ���ݶ��г���
#define USART3_Send_Q_NUM     10						//����3���Ͷ���
#define pack2server_Q_NUM     5						//���͸��������İ���������֡����
#define connect_to_server_Q_NUM     1						//�������ӷ�����
#define USART4_Q_NUM					10
#define USART4SEND_Q_NUM			10
#define USART5_Q_NUM					10
#define USART5SEND_Q_NUM			10
#define SuperSound_Q_NUM			4
#define Dprintf_Q_NUM				10

//���ڻ�������
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define USART1_PER_LEN   50		//ÿ������buff��С
#define USART1_NUM     USART1_Q_NUM //�ܹ������buff���� ����8������Ϊ�õ�u8���ı��

#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define USART3_PER_LEN  200		//ÿ������buff��С
#define USART3_NUM       USART3_Q_NUM//�ܹ������buff���� ����8������Ϊ�õ�u8���ı��

#define USART4_PER_LEN  50		//ÿ������buff��С
#define USART4_NUM       USART4_Q_NUM//�ܹ������buff���� ����8������Ϊ�õ�u8���ı��
#define USART5_NUM       USART5_Q_NUM//�ܹ������buff���� ����8������Ϊ�õ�u8���ı��

//extern u8 usart3_data_all[USART3_NUM][USART3_PER_LEN];//����3�ײ㻺��buff

/******************��ʱ����غ궨��********************/
//wifi����ʹ�õ�������ʱ��
//60s��ʱ��
//TIM60
#define WIFI_TIM60S_TICKS 60*1000/portTICK_PERIOD_MS
//TIM_ID
#define WIFI_TIM60S_ID    1

//10s��ʱ��
//TIM10
#define WIFI_TIM10S_TICKS 10*1000/portTICK_PERIOD_MS
//TIM_ID
#define WIFI_TIM10S_ID    2

//��ʱ���Ͱ�0002 40s����5s
//ticks
#define PKG0002_40S_TICKS 40*1000/portTICK_PERIOD_MS
#define PKG0002_5S_TICKS  5*1000/portTICK_PERIOD_MS
//TIM_ID
#define PKG0002_TIM_ID    3



//��ⴥ�ذ��ִ�źŶ�ʱ�� 50MS
//ticks
#define WAIT_TOUCH_TICKS 250/portTICK_PERIOD_MS
//TIM_ID
#define WAIT_TOUCH_ID    4

#define WIFI_1MS_TICKS	1/portTICK_PERIOD_MS
#define WIFI_TIM1MS_ID 5

//��ʱʹ�ܷ��͸�����������5s
//ticks
#define CanServer_TICKS 5*1000/portTICK_PERIOD_MS
//TIM_ID
#define CanServer_ID    6

//��ʱ���ڵƹ�
//ticks
#define Bright_TICKS 10/portTICK_PERIOD_MS//200 -10��  66-30��
//TIM_ID
#define Bright_ID    7
#define Bright_bei 3
/******************������غ궨��********************/
//��ʼ����
//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С
#define START_STK_SIZE 		128

//�����������
//�������ȼ�
#define TASK1_TASK_PRIO		2//2
//�����ջ��С
#define TASK1_STK_SIZE 		128
//�������м�� ms
#define task1_task_delay      20//

//������������
//�������ȼ�
#define KEYPROCESS_TASK_PRIO 3//3
//�����ջ��С
#define KEYPROCESS_STK_SIZE  256
//�������м�� ms
#define Keyprocess_task_delay 20//


//����1���� ���ݷ�֡ ��֡
//�������ȼ�
#define USART1_TASK_PRIO 28
//�����ջ��С
#define USART1_STK_SIZE  128
//�������м�� ms
#define USART1_task_delay     50//�������м�� ms

//����1��֡���ݴ���
//�������ȼ�
#define USART1_APP_PRIO  13//�Ƚ����������ȼ��� 13
//�����ջ��С
#define USART1_APP_SIZE  256
//�������м�� ms
#define USART1_APP_delay     100//�������м�� ms



//8266 smartconfig ����
//�������ȼ�
#define SMARTCONFIG_TASK_PRIO  19//���ڴ���3�ʹ��ذ������ȼ����Ϳ��ԣ�����ʱֻ���⼸��������ִ��19
//�����ջ��С
#define SMARTCONFIG_TASK_SIZE  256
//�������м�� ms
#define SMARTCONFIG_TASK_delay 50//�������м�� ms

//8266 connect t0 server ����
//�������ȼ�
#define connect_to_server_PRIO  18//����smart18
//�����ջ��С
#define connect_to_server_SIZE  256
//�������м�� ms
#define connect_to_server_delay 50//�������м�� ms


//����3���� ���ݷ�֡ ��֡
//�������ȼ�
#define USART3_TASK_PRIO 29
//�����ջ��С
#define USART3_STK_SIZE  128
//�������м�� ms
#define USART3_task_delay     50//�������м�� ms

//����3���� ���պ������ݳ�ʱ����Ϊ���ݴ��� free�ڴ棬��ʼidel����ģʽ
//�������ȼ�
#define FREE_ERRORMEM_PRIO 30
//�����ջ��С
#define FREE_ERRORMEM_SIZE  96
//�������м�� ms
#define FREE_ERRORMEM_delay     50//�������м�� ms

//����3���ݷ�������
//�������ȼ�
#define USART3_SEND_PRIO  20//�Ƚ����������ȼ���20
//�����ջ��С
#define USART3_SEND_SIZE  256+256
//�������м�� ms
#define USART3_SEND_delay     50//�������м�� ms

//����3���������ݴ���
//�������ȼ�
#define USART3_APP_PRIO  14//�Ƚ����������ȼ���14
//�����ջ��С
#define USART3_APP_SIZE  256
//�������м�� ms
#define USART3_APP_delay     50//�������м�� ms


//������ݴ���
//�������ȼ�
#define pack2server_PRIO  13//�Ƚ����������ȼ���13
//�����ջ��С
#define pack2server_SIZE  256
//�������м�� ms
#define pack2server_delay 50

//����4����
//�������ȼ�
#define USART4_APP_PRIO  27//13
//�����ջ��С
#define USART4_APP_SIZE  256
//�������м�� ms
#define USART4_APP_delay     30//�������м�� ms

//����5����
//�������ȼ�
#define USART5_APP_PRIO  15//15
//�����ջ��С
#define USART5_APP_SIZE  256
//�������м�� ms
#define USART5_APP_delay     50//�������м�� ms

//�����������
//�������ȼ�
#define IRF_APP_PRIO  15//10 4
//�����ջ��С
#define IRF_APP_SIZE  256
//�������м�� ms
#define IRF_APP_delay     100//�������м�� ms

//MP3����
//�������ȼ�
#define MP3_TASK_PRIO  10//10 4
//�����ջ��С
#define MP3_TASK_SIZE  128
//�������м�� ms
#define MP3_TASK_delay     100//�������м�� ms
/**************״̬ö��*****************************/
//���ڷ�֡�����ݵ�����
typedef enum
{
	USART_RN,			//���յ�\r\n����������
	USART_SERVER,	//���յ��������·���Э���ʽ������
	USART_DIRECT, //ֱ�ӷ��͵����ݣ������˳�͸������+
	USART_OTHER,	//��ʽ���Ե�����

}USART_DATA_TYPE;
//����task��֡��֡�����̱��
typedef enum
{
	USART_DATA_NORMAL_START,//��ͨ֡��ʼ
	USART_DATA_NORMAL_STOP, //��ͨ֡����
	USART_DATA_RN_STOP,     //\r\n֡����
	USART_DATA_SERVER_START,//���յ�������Э�����ʼ����
	USART_DATA_SERVER_STOP, //���յ�������Э��Ľ�������
	USART_DATA_WAIT,//�ȴ���һ������buff�����ݽ�����֡
	USART_DATA_IDEL,//����û���յ�������
}USART_TASK_PROCESS;
/**************�ṹ��*******************************/
//task�Դ������ݷ�֡����Ľ��
typedef struct
{
  uint8_t* pstr;//���ݴ洢λ��
  uint32_t len;//�����ܳ���
	USART_DATA_TYPE type;//��֡�õ����ݵ�����
}usart_task_data;

//�������ݴ������� ���ڶ���
typedef struct
{
	uint8_t buff_now;//���ݴ洢����
	uint8_t len;//���ݳ���
}usart_dat;

//FIFO�ṹ��
typedef struct
{
	uint8_t* pstr;//�洢����
	uint16_t total_len;//FIFO�ܴ�С
	uint16_t now_len;//��ǰFIFO���ݸ���
	uint16_t read_pos;
	uint16_t write_pos;
}fifo_struct;

//ģ�⴮���ö�ʱ�����ƽ���һ��byte�Ŀ��Ʊ���
typedef struct
{
	uint8_t flag;
	uint8_t count;
	uint8_t byte;
	uint8_t timeout;
	uint8_t bytenum;
}suart_struct;
//ģ�⴮���ö�ʱ�����Ʒ���һ��byte�Ŀ��Ʊ���
typedef struct
{
	uint8_t bit_flag;//byte�ĵڼ�λ
	uint8_t byte_now;//��ǰ���͵����ݻ���
	uint8_t send_flag;
	uint8_t tim_count;
}suart_send_struct;

//�������
typedef enum
{
	SOUND_FLAG_IDEL,
	SOUND_FLAG_TRI_START,
	SOUND_FLAG_PEDG_START,
	SOUND_FLAG_NEDG_STOP
}SOUND_FLAG_ENUM;

typedef struct
{
	uint64_t tim_start;
	uint64_t tim_stop;
	uint64_t tim_result;
	uint64_t tim_ctr_start;
	uint64_t tim_ctr_stop;

	SOUND_FLAG_ENUM flag;
	uint8_t count;


}supersound_struct;

//ϵͳ�����ṹ��
//���ӽṹ���Աʱ������ռ�ÿռ��С������
//flash���� ��Ҫ��ȡflash�ڽṹ��Ĵ��������ȷ�����������
#pragma pack (4)
typedef struct
{
    //
    uint8_t boot_flag;//0x66 ������Ҫ��������Ϊ0x00 ������boot��Ҫ����

    //�Ƿ��ʼ����flash
    uint8_t flash_init;//������ ��ʼ����ɴ洢һ��0x55

    //ħ����ص�����
		uint8_t modou_update;//���ڹ���ģʽ�����ڽ���ģʽ 0 ��/r/n ģʽ || 1: ħ������ģʽ
    uint8_t angle_correct_plus;//���ҽǶ�
//		uint8_t last_LR_angle;		//��һ�ε�У׼����
		uint8_t power_on_add_voice;//�����Ƿ񲥷ţ�����MP3��ɣ������̼����

    //MP3�ĸ�������
    uint8_t MP3_STA;					//MP3�ĸ���״̬
    uint8_t MP3_NUM;					//���ڸ��µ�MP3��Ŀ
		uint8_t NEW_MP3_ID[4];		//Ҫ���µ�������
		uint8_t MP3_ID[80];			//MP3������
		uint8_t MP3_LEN[4];			//��ǰMP3�ļ�����

    //̨�ư汾�� ID ��  ���µ����°汾���Լ��̼���С
    uint8_t bin_receive_flag;//����bin�ļ���״̬��0δ���ܣ�1 ���ڽ��ܣ�2 �������

    uint8_t S_HW[4];         //ID3�����ݰ�
    uint8_t S_SW[4];         //
    uint8_t S_SERIAL[8];     //
    uint8_t angle_correct;   //
    uint8_t LAMP_MODEL[4];   //̨���ͺ�
    uint8_t NEW_S_SW[4];     //ħ�����µ�����汾��
    uint8_t NEW_M_SW[4];     //̨�Ƶ��µ�����汾��
    uint8_t FW_LEN[4];       //�µĹ̼��ĳ���

    uint8_t LAMP_HV[4];      //̨��Ӳ���汾��
    uint8_t LAMP_SV[4];      //̨������汾��
		uint8_t M_SERIAL[8];			//̨�����к�
    //uint8_t MODOU_HV[4];     //ħ��Ӳ���汾��
    //uint8_t MODOU_SV[4];     //ħ������汾��

		uint8_t WIFI_SW[4];//wifi�汾��

    //̨�Ƶı����Ƕ�ʱ�������
    uint8_t ANGLE_ALARM1 ; //ǰ�� Ĭ��20��
    uint8_t ANGLE_ALARM2 ; //���� Ĭ��30��
    uint8_t ALERT_TIME1  ; //�״α���ʱ�䣬Ĭ��5s
    uint8_t ALERT_TIME2  ;//�ظ��������180

    uint16_t MUTE_START  ;//Ĭ�Ͼ�����ʼʱ��0:00
    uint16_t MUTE_END    ;//Ĭ�Ͼ�������ʱ��7:00  420


		uint32_t pkt_version;//������Э��汾

    uint32_t mcu_bin_len; //mcu�̼��ĳ���
    uint32_t modo_bin_len;//ħ���̼�����

    uint32_t device_id;//��flash��ȡ̨��id

		uint32_t SLEEP_START;
		uint32_t SLEEP_END;


}SYS_CONFIG;
extern SYS_CONFIG sys_config;






#endif /* APPINC_APP_CONFIG_H_ */
