#include "word.h"

uint64_t word[26][8]={  /*A*/{0x0000,0x0700,0x3800,0xc800,0x3800,0x0700,0x0000},/*B*/{0x0000,0xff00,0x9100,0x9100,0x9100,0x6e00,0x0000},
						/*C*/{0x0000,0x7e00,0x8100,0x8100,0x8100,0x4200,0x0000},/*D*/{0x0000,0xff00,0x8100,0x8100,0x8100,0x4200,0x3c00},
						/*E*/{0x0000,0xff00,0x9100,0x9100,0x9100,0x9100,0x0000},/*F*/{0x0000,0xff00,0x900000,0x900000,0x900000,0x9000,0x000000},
						/*G*/{0x0000,0x3c00,0x4200,0x8100,0x8900,0x8900,0x4e00},/*H*/{0x0000,0x7f00,0x080000,0x080000,0x080000,0x7f0000,0x000000},
						/*I*/{0x0000,0x8100,0x8100,0xff00,0x8100,0x8100,0x0000},/*J*/{0x0000,0x0400,0x820000,0x820000,0xfc0000,0x800000,0x000000},
						/*K*/{0x0000,0x7f00,0x0800,0x1400,0x2200,0x0000,0x0000},/*L*//*{0x0000,0xff00,0x010000,0x010000,0x010000,0x010000,0x000000}*/
										/*love*/	{0x3000,0x7800,0x7c00,0x3e00,0x7c00,0x7800,0x3000},
						/*M*/{0X3f00,0x4000,0x3800,0x0600,0x3800,0x4000,0X3f00},/*N*/{0x0000,0x7f00,0x6000,0x3800,0x0c00,0x0600,0xef00},
						/*O*/{0X3c00,0x4200,0x8100,0x8100,0x8100,0x4200,0X3c00},/*P*/{0x0000,0xff00,0x8800,0x8800,0x8800,0x7000,0X0000},
						/*Q*/{0x0000,0X3c00,0x4200,0x8100,0x8500,0x4200,0X3d00},/*R*/{0x0000,0xff00,0x9800,0x9c00,0x9600,0x9300,0x6100},
						/*S*/{0x0000,0x6200,0x9100,0x9100,0x8900,0x8600,0X0000},/*T*/{0x0000,0x8000,0x8000,0xff00,0x8000,0x8000,0X0000},
						/*U*/{0x0000,0xfc00,0x0200,0x0100,0x0100,0x0200,0xfc00},/*V*/{0xe000,0x1c00,0x0200,0x0100,0x0200,0x1c00,0xe000},
						/*W*/{0xe000,0x1c00,0x0300,0x1c00,0x0300,0x1c00,0xe000},/*X*/{0x8100,0x4200,0x2400,0x1800,0x2400,0x4200,0x8100},
						/*Y*/{0x8000,0x4000,0x3000,0x0f00,0x3000,0x4000,0x8000},/*Z*/{0x8100,0x8300,0x8d00,0x9900,0xb100,0xc100,0x8100}
					   			};
void word_show_A(uint8_t num)
{
	int i;
	EMBARC_PRINTF("WORD SHOW!!%d  \r\n",num);
	for(i=0;i<7;i++)
	{
		light_ctr_rgb(0xff00,LIGHT_ROW_MIDDLE,0x00,0x0f,0x00);
		vTaskDelay(1);
		if(num==11)
		{
			light_ctr_rgb(word[num][i],LIGHT_ROW_MIDDLE,0xff,0x00,0x00);
		} else
		light_ctr_rgb(word[num][i],LIGHT_ROW_MIDDLE,0xaf,0x2f,0xff);
	}
}