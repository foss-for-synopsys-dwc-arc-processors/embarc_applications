/*
 * strpos.c
 *
 *  Created on: 2018��5��3��
 *      Author: Administrator
 */

#include "strpos.h"
//
#include "string.h"
#include "stdlib.h"
//#include "sys.h"




//��������ʼλ�ͽ���λ
uint8_t server_start[4] = {'{','"','M','"'};//{"M"
uint8_t server_stop [2] = {'"','}'};//"}
uint8_t rn_stop[2]      = {0x0d,0x0a};
uint8_t test_data[]={0x00,0x01,0x02,0xff,0xaa,0x55,0x01,0x07,0x08,0x0d,0x0a,0x32,0x97,0x25,0xEE};

//Ѱ��֡��ʼ ������־
//���ضԱ�λ�õ���ʼ��ַ  00 01 02 ff aa 55 00 07 08 ����3
//�ַ����ܳ���ҪС��255
uint8_t findpos(uint8_t*pstr1,uint8_t len1,uint8_t *pstr2,uint8_t len2)//1�Ǳ�־λ��2��ԭʼ��������
{
	uint8_t i,j;
	if(!(len2<len1))
	{
		for(i=0;i<len2-len1+1;i++)
		{
			if(pstr1[0]==pstr2[i])
			{
//				printf("0 find\r\n");
				for(j=1;j<len1;j++)
				{
					if(pstr1[j]!=pstr2[++i])
					{
						break;
					}
				}
//				printf("fianl j=%d\r\n",j);
				if(j==len1)
				{
					return i-len1+1;
				}
				else
				{
					i-=j;
				}
			}
		}
	}
	return 0xff;
}

//�Ա��ַ��Ƿ�һ�� һ�·���1 ��һ������0
uint8_t strsame(uint8_t*str1,uint8_t*str2,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		if(str1[i]!=str2[i])
		{
			return 0;
		}

	}
	return 1;
}


//end strpos.c

