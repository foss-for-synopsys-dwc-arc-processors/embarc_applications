/*
 * encrypt.c
 *
 *  Created on: 2018年5月3日
 *      Author: Administrator
 */

#include "encrypt.h"
//
#include "string.h"
#include "stdlib.h"
//#include "sys.h"



//device_id加密方式
//32位key，用来加密4个字节的device id
int key = 0x0F123A67;
//64位key,用来加密最终的8字节数据
long long key64 = 0xA00FAB123AABFF78;

//a???????(???????????,?????) ;CD 47 F1 63
//b???????,????;
//c?????????,?????K???bin??,????
//d????K???CRC32??, ?????????;
//e???  FE 58 DC 56

int key_result   = 0x0E692d32;

//解密
int  decrypt(long long data)
{
	long long l = data ^ key64;
	int deviceId = (int)l;

//	printf("orignal device id: %d\n", deviceId);
	int enc = (int)(l >> 32);
	int dec =enc ^ key_result;
//	printf("解密后decrypted device id: %d\n", dec);
	if(dec == deviceId)
	{

//	printf("compare sucess\n");
		return dec;
	}
	else

	return -1;//解密失败
}


/*
long long encrypt(int deviceId)
{
	long long ret = deviceId;
	long long enc = deviceId ^ key;

	return ((ret |= (enc<<32)) ^ key64);
}
*/
void device_id_encrypt(uint32_t deviceId,uint8_t*result)
{

    long long ret = deviceId;
		long long enc = deviceId ^ key;
    long long temp;

    uint8_t i;

	temp=((ret |= (enc<<32)) ^ key64);

    for(i=0;i<8;i++)
    {
       *( result +i)=(uint8_t)(temp>>((7-i)*8));

    }

}

//end encrpyt.c

