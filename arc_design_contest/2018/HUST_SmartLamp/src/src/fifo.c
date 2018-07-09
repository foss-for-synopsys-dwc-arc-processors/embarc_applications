/*
 * fifo.c
 *
 *  Created on: 2018��5��17��
 *      Author: Administrator
 */

#include "fifo.h"

//fifo��ʼ��
void init_fifo(fifo_struct*fifo2init,uint16_t totallen,uint8_t*fifo_data)
{
	fifo2init->total_len = totallen;
	fifo2init->pstr			= fifo_data;

	reset_fifo(fifo2init);
}

//д�����ݵ�FIFO�У�����ģʽ�������ݻḲ��δ����ľ�����
uint8_t write_fifo(fifo_struct*fifo2write,uint8_t*byte,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		fifo2write->pstr[fifo2write->write_pos] = *(byte+i);
		if(++fifo2write->write_pos>=fifo2write->total_len)
		{
			fifo2write->write_pos = 0;
		}
		if(fifo2write->now_len < fifo2write->total_len)
		{
			fifo2write->now_len++;
		}
	}
	if(fifo2write->now_len == fifo2write->total_len) return 0;//��������
	else return 1;

}

//��ȡFIFO������
uint8_t read_fifo(fifo_struct*fifo2read,uint8_t*byte,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		if(fifo2read->now_len)
		{
			*(byte+i) = fifo2read->pstr[fifo2read->read_pos];
			if(++fifo2read->read_pos>=fifo2read->total_len)
			{
				fifo2read->read_pos = 0;
			}
			fifo2read->now_len--;
		}
		else
		{
			break;
		}
	}
	if(i>=len) return 1;
	else return 0;//��ȡ���ݳ���FIFO�����ݸ���

}

//����fifo����
void reset_fifo(fifo_struct*fifo2clear)
{
	fifo2clear->now_len = 0;
	fifo2clear->read_pos= 0;
	fifo2clear->write_pos=0;

}

//end fifo.c

