/*
 * encrypt.c
 *
 *  Created on: 2018��5��3��
 *      Author: Administrator
 */

#include "encrypt.h"
#include "string.h"
#include "stdlib.h"

//device_id���ܷ�ʽ

void device_id_encrypt(uint32_t deviceId,uint8_t*result)
{
    uint8_t i;
    for(i=0;i<8;i++)
    {
       *( result +i)=(deviceId>>(i*4))&0x0f;
    }
}

//end encrpyt.c

