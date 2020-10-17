#include "keypad.h"
#include "stdio.h"



void keypad_init(void)
{
  PMOD_1 = gpio_get_dev(DW_GPIO_PORT_A);
  PMOD_1->gpio_open(PA_MASK);
  PMOD_1->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)PA_MASK);

  PMOD_2 = gpio_get_dev(DW_GPIO_PORT_C);
  PMOD_2->gpio_open(PC_MASK);
  PMOD_2->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)PC_MASK);
} 

bool key_down(void)
{
bool flag;
uint32_t key;
PMOD_2 -> gpio_read(&key,PC_MASK);
if(~key & PC_MASK){
flag = true;
}else{
flag = false;
}
return flag;
}


uint8_t key_perpare(void)
{
uint8_t local;
uint32_t key;
PMOD_1 -> gpio_write(0x00000,PA_MASK);
do{
   while(!key_down());
   board_delay_ms(10,1);
}
   while(!key_down());
  PMOD_1 -> gpio_write(0x70000,PA_MASK);
  PMOD_2 -> gpio_read(&key,PC_MASK);
   if(~key & PC_MASK){
    local = (key>>16) | 0x70;
	return local;
   }
  PMOD_1 -> gpio_write(0xB0000,PA_MASK);
  PMOD_2 -> gpio_read(&key,PC_MASK);
	if(~key & PC_MASK){
	 local = (key>>16) | 0xB0;
	 return local;
	}
  PMOD_1 -> gpio_write(0xD0000,PA_MASK);
  PMOD_2 -> gpio_read(&key,PC_MASK);
   if(~key & PC_MASK){
    local = (key>>16) | 0xD0;
	return local;
   }
  PMOD_1 -> gpio_write(0xE0000,PA_MASK);
  PMOD_2 -> gpio_read(&key,PC_MASK);
   if(~key & PC_MASK){
    local = (key>>16) | 0xE0;
	return local;
   }
   return 0;
}
bool skey = false;
uint8_t keypad_read (void){
	uint8_t local,b; 
	if(skey==false)
		{
	local = key_perpare();
	skey=true;
		}
	else
		local=0;
	
		switch(local){
			case 0xee: b = Enter; break;
			case 0xed: b = 14; break;
			case 0xeb: b = 0; break;
			case 0xe7: b = 15; break;
			case 0xde: b = Esc; break;
			case 0xdd: b = 9; break;
			case 0xdb: b = 8; break;
			case 0xd7: b = 7; break;
			case 0xbe: b = Delete; break;
			case 0xbd: b = 6; break;
			case 0xbb: b = 5; break;
			case 0xb7: b = 4; break;
			case 0x7e: b = F1; break;
			case 0x7d: b = 3; break;
			case 0x7b: b = 2; break;
			case 0x77: b = 1; break;
			default: b = 20; break;
		}

		if(skey==true && !key_down())
		{
   			board_delay_ms(10,1);
			if(!key_down())
				skey=false;
				
		}

return b;
} 

uint8_t *VerifyString(uint16_t len)
{
    uint16_t maxlen=0;  
    uint8_t keynum=20;  
    uint8_t *vstring;
	uint8_t mset=4*len+1; 
    vstring=(uint8_t *)malloc(mset); 
	uint8_t *string;

    while(maxlen<len)
    {
        keynum=keypad_read();
		if((keynum!=20)) 
        {
            vstring[maxlen*4]  =0x30;
			vstring[maxlen*4+1]=0x30;
			vstring[maxlen*4+2]=0x33;
			vstring[maxlen*4+3]=keynum+0x30; 
			maxlen++;
        }
		
    }
    vstring[4*len]=0;	
	return (uint8_t *)vstring;
}


uint8_t *passwordstring(void)
{
    uint16_t maxlen=0;  
    uint8_t *vstring,num;
	uint16_t k=1000;
    vstring=(uint8_t *)malloc(17); 
	uint16_t a=rand()%9000+1000;
	
	
	for(maxlen=0;maxlen<4;maxlen++){
		num=a/k;
		a=a%k;
            vstring[maxlen*4]  =0x30;
			vstring[maxlen*4+1]=0x30;
			vstring[maxlen*4+2]=0x33;
			vstring[maxlen*4+3]=num+0x30; 
		k=k/10;
			
        }	
    vstring[16]=0;
    return (uint8_t *)vstring;
}
	


