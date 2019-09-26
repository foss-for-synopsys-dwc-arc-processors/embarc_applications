/**
 * \brief	the snake game and led matrix.
 */
#include "snake.h"

DEV_GPIO_PTR gpio_led1,gpio_led2;

uint8_t led_matrix_ram[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

snake_body snake[256],food;
int snake_head, snake_end, snake_towards;

void led_init(void)
{
	gpio_led1 = gpio_get_dev(DFSS_GPIO_8B0_ID);
	gpio_led1->gpio_open(0x30);
	gpio_led1->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(0x30));
	gpio_led2 = gpio_get_dev(DFSS_GPIO_8B1_ID);
	gpio_led2->gpio_open(0x10);
	gpio_led2->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(0x10));
}

void max7219_write(uint8_t dat)
{
	uint8_t i,tmp=dat;

	for (i=0;i<8;i++)
	{
		if(tmp&0x80)DOUT_1;
		else DOUT_0;
		tmp = tmp << 1;
		CLK_0;
		CLK_1;       
	}
}

void max7219_send(uint8_t addr1,uint8_t dat1,uint8_t addr2,uint8_t dat2,uint8_t addr3,uint8_t dat3,uint8_t addr4,uint8_t dat4)
{
        CS_0;
        max7219_write(addr4);
        max7219_write(dat4);
        max7219_write(addr3);
        max7219_write(dat3);
        max7219_write(addr2);
        max7219_write(dat2);
        max7219_write(addr1);
        max7219_write(dat1);
        CS_1;
}

void led_matrix_init()
{
	led_init();
  	max7219_send (0x0c,0x01,0x0c,0x01,0x0c,0x01,0x0c,0x01);     /*  设置电源工作模式      */   
    max7219_send (0x0a,0x09,0x0a,0x09,0x0a,0x09,0x0a,0x09);     /*   设置亮度  19/32      */  
    max7219_send (0x0b,0x0f,0x0b,0x0f,0x0b,0x0f,0x0b,0x0f);     /*  设置扫描界限          */
    max7219_send (0x09,0x00,0x09,0x00,0x09,0x00,0x09,0x00);     /*   设置译码模式         */

    led_matrix_ram[0] = 0;
    led_matrix_ram[1] = 0;
    led_matrix_ram[2] = 0;
    led_matrix_ram[3] = 0;
}

#define msk(x)	((led_matrix_ram[(x)]<<0)|(led_matrix_ram[(x)+1]<<1)|(led_matrix_ram[(x)+2]<<2)|(led_matrix_ram[(x)+3]<<3)|(led_matrix_ram[(x)+4]<<4)|(led_matrix_ram[(x)+5]<<5)|(led_matrix_ram[(x)+6]<<6)|(led_matrix_ram[(x)+7]<<7))
void led_matrix_refresh()
{
	max7219_send (0x01,msk(112),0x01,msk(120),0x01,msk(240),0x01,msk(248)); 
	max7219_send (0x02,msk(96),0x02,msk(104),0x02,msk(224),0x02,msk(232));
	max7219_send (0x03,msk(80),0x03,msk(88),0x03,msk(208),0x03,msk(216)); 
	max7219_send (0x04,msk(64),0x04,msk(72),0x04,msk(192),0x04,msk(200));
	max7219_send (0x05,msk(48),0x05,msk(56),0x05,msk(176),0x05,msk(184)); 
	max7219_send (0x06,msk(32),0x06,msk(40),0x06,msk(160),0x06,msk(168));
	max7219_send (0x07,msk(16),0x07,msk(24),0x07,msk(144),0x07,msk(152));
	max7219_send (0x08,msk(0),0x08,msk(8),0x08,msk(128),0x08,msk(136)); 
}

void snake_init()
{
	memset(led_matrix_ram, 0, 256);
	snake_head = 0;
	snake_end = 2;
	snake_towards = 3;
	snake[0].x = 2;
	snake[0].y = 10;
	led_matrix_ram[snake[0].y * 16 + snake[0].x] = 1;
	snake[1].x = 1;
	snake[1].y = 10;
	led_matrix_ram[snake[1].y * 16 + snake[1].x] = 1;
	snake[2].x = 0;
	snake[2].y = 10;
	led_matrix_ram[snake[2].y * 16 + snake[2].x] = 1;
	food.x = 2;
	food.y = 13;
	led_matrix_ram[food.y * 16 + food.x] = 1;
	led_matrix_refresh();
}

int8_t compare_with_snake(uint8_t head, snake_body pos)
{
	uint8_t i,tmp=head;

	while(tmp!=snake_end)
	{
		if((pos.x==snake[tmp].x)&&(pos.y==snake[tmp].y))return 1;
		if(tmp==255)tmp=0;
		else tmp++;
	}
	if((pos.x==snake[snake_end].x)&&(pos.y==snake[snake_end].y))return 1;
	else return 0;
}

void snake_move(uint8_t seed)//input: seed of the position of the food.
{
	if(snake_head > 0)
	{
		switch(snake_towards)
		{
			case 0:
				snake[snake_head - 1].x = snake[snake_head].x;
				snake[snake_head - 1].y = snake[snake_head].y - 1;
				snake_head--;
				if(snake[snake_head].y < 0)snake[snake_head].y+=16;
				break;
			case 1:
				snake[snake_head - 1].x = snake[snake_head].x;
				snake[snake_head - 1].y = snake[snake_head].y + 1;
				snake_head--;
				if(snake[snake_head].y > 15)snake[snake_head].y-=16;
				break;
			case 2:
				snake[snake_head - 1].x = snake[snake_head].x - 1;
				snake[snake_head - 1].y = snake[snake_head].y;
				snake_head--;
				if(snake[snake_head].x < 0)snake[snake_head].x+=16;
				break;
			case 3:
				snake[snake_head - 1].x = snake[snake_head].x + 1;
				snake[snake_head - 1].y = snake[snake_head].y;
				snake_head--;
				if(snake[snake_head].x > 15)snake[snake_head].x-=16;
				break;
			default :
				break;
		}
	}
	else
	{
		switch(snake_towards)
		{
			case 0:
				snake[255].x = snake[0].x;
				snake[255].y = snake[0].y - 1;
				snake_head = 255;
				if(snake[255].y < 0)snake[255].y+=16;
				break;
			case 1:
				snake[255].x = snake[0].x;
				snake[255].y = snake[0].y + 1;
				snake_head = 255;
				if(snake[255].y > 15)snake[255].y-=16;
				break;
			case 2:
				snake[255].x = snake[0].x - 1;
				snake[255].y = snake[0].y;
				snake_head = 255;
				if(snake[255].x < 0)snake[255].x+=16;
				break;
			case 3:
				snake[255].x = snake[0].x + 1;
				snake[255].y = snake[0].y;
				snake_head = 255;
				if(snake[255].x > 15)snake[255].x-=16;
				break;
			default :
				break;
		}
	}
	led_matrix_ram[snake[snake_head].y * 16 + snake[snake_head].x] = 1;

	if((snake[snake_head].x == food.x)&&(snake[snake_head].y == food.y))
	{
		food.x = seed&0x0f;
		food.y = seed>>4;
		led_matrix_ram[food.y * 16 + food.x] = 1;
	}
	else
	{
		led_matrix_ram[snake[snake_end].y * 16 + snake[snake_end].x] = 0;
		if(snake_end > 0)snake_end--;
		else snake_end = 255;
	}

	led_matrix_refresh();

	if(snake_head == 255)
	{
		if(compare_with_snake(0, snake[255]))snake_init();
	}
	else
	{
		if(compare_with_snake(snake_head+1, snake[snake_head]))snake_init();
	}
}
