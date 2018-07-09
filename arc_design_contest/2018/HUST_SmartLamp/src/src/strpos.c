/*
 * strpos.c
 *
 *  Created on: 2018年5月3日
 *      Author: Administrator
 */

#include "strpos.h"
//
#include "string.h"
#include "stdlib.h"
//#include "sys.h"




//服务器起始位和结束位
uint8_t server_start[4] = {0xFF,0xFF,0xFF,0xFF};
uint8_t server_stop [4] = {0x00,0x00,0x00,0x00};
uint8_t rn_stop[2]      = {0x0d,0x0a};

uint8_t findpos(uint8_t*pstr1,uint8_t len1,uint8_t *pstr2,uint8_t len2)//1是标志位，2是原始数据数组
{
	uint8_t i,j;
	if(!(len2<len1))
	{
		for(i=0;i<len2-len1+1;i++)
		{
			if(pstr1[0]==pstr2[i])
			{
				for(j=1;j<len1;j++)
				{
					if(pstr1[j]!=pstr2[++i])
					{
						break;
					}
				}
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

//对比字符是否一致 一致返回1 不一样返回0
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

