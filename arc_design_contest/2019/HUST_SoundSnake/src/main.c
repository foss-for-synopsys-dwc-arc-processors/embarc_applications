#include "embARC.h"
#include "embARC_debug.h"
#include "wm8978.h"
#include "wm8978i2s.h"
#include "kws.h"
#include "dev_gpio.h"
#include "codec.h"
#include "snake.h"


void ShowWord(uint8_t i)
{
	switch(i)
	{
		case 0:
			EMBARC_PRINTF("Silence\r\n");break;
		case 1:
			EMBARC_PRINTF("Unknown\r\n");break;
		case 2:
			EMBARC_PRINTF("yes\r\n");break;
		case 3:
			EMBARC_PRINTF("no\r\n");break;
		case 4:
			EMBARC_PRINTF("up\r\n");break;
		case 5:
			EMBARC_PRINTF("down\r\n");break;
		case 6:
			EMBARC_PRINTF("left\r\n");break;
		case 7:
			EMBARC_PRINTF("right\r\n");break;
		case 8:
			EMBARC_PRINTF("on\r\n");break;
		case 9:
			EMBARC_PRINTF("off\r\n");break;
		case 10:
			EMBARC_PRINTF("stop\r\n");break;
		case 11:
			EMBARC_PRINTF("go\r\n");break;
		default :
			break;
	}
}

extern DEV_BUFFER rx_buffer;
extern int16_t audio_buf[16*1000*2],rx_buffer1[640*18];
extern volatile uint8_t word_state;
extern int snake_towards;

int main(void)
{
	uint8_t keyword;

	ds_cnn_init();
	wm8978_init_func();//Config the WM8978 through I2C.
	led_matrix_init();
	snake_init();
	CODEC_init();//Start I2S transfer and interrupts.

	while (1) {
		if(word_state == 1)//word_state = 1 means recording is in process.
		{
			while(word_state != 2);//word_state = 2 means recording has just been finished.
			word_state = 0;
			keyword = kws_using_ds_cnn(audio_buf);//figure out the keyword from the sound sequence with cnn.
			ShowWord(keyword);//print the keyword.
			switch(keyword)
			{
				case 4: snake_towards = 0;break;//up
				case 5: snake_towards = 1;break;//down
				case 6: snake_towards = 2;break;//left
				case 7: snake_towards = 3;break;//right
				default : break;
			}
			snake_move((int8_t)rx_buffer1[0]);//move the snake by one step.
			cs4344_rx_isr_restart(&rx_buffer);//restart the transfer.
			board_delay_ms(400, 1);
		}
		else
		{
			snake_move((int8_t)rx_buffer1[0]);//move the snake by one step.
			board_delay_ms(400, 1);
		}
	}

	return E_SYS;
}
