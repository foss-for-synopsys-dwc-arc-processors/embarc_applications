#include "codec.h"

#define AUDIO_BUF_NUM			640			/* !<the length of buffer>*/

int16_t audio_buf[16*1000*2];
int16_t rx_buffer1[640*18];
DEV_BUFFER rx_buffer,tx_buffer;

unsigned long frame_energy(int16_t *buf, uint16_t len)
{
	unsigned long sum = 0;
	int16_t i, tmp;

	for(i=0;i<len;i++)
	{
		tmp =  buf[2*i];
		sum += tmp * tmp;
	}

	return sum;
}

uint16_t zc(int16_t *buf, uint16_t len)//zero cross times.
{
	uint16_t cnt=0,i;
	int16_t tmp1,tmp2;

	for(i=0;i<(len-1);i++)
	{
		tmp1 = buf[2*i];
		tmp2 = buf[2*i+2];
		if(((tmp1>0)&&(tmp2<0))||((tmp1<0)&&(tmp2>0)))cnt++;
	}

	return cnt;
}

volatile uint8_t bufcnt=0,word_state=0;
volatile int16_t state_cnt=0;

static void cs4344_rx_isr(void)
{
	uint16_t zc_cnt,i;
	unsigned long eng;

	zc_cnt = zc((rx_buffer1 + 640*bufcnt), 320);
	eng = frame_energy((rx_buffer1 + 640*bufcnt), 320);
	if((eng>200000000)&&((zc_cnt>4)&&(zc_cnt<55))&&(state_cnt<7))state_cnt++;//audio valid condition.
	else if(state_cnt > 0)state_cnt--;

	if(bufcnt<17)bufcnt++;
	else bufcnt = 0;

	if(state_cnt > 6)
	{
		rx_buffer.buf = audio_buf + 640*16;
		rx_buffer.len = 16*1000*2 - 640*16;
		cs4344_rx_isr_restart(&rx_buffer);
		for(i=0;i<16;i++)
		{
			if((bufcnt + i + 2)>17)memcpy(audio_buf + i*640,rx_buffer1 + (bufcnt + i + 2 - 18)*640,1280);
			else memcpy(audio_buf + i*640,rx_buffer1 + (bufcnt + i + 2)*640,1280);
		}
		state_cnt = 0;
		word_state = 1;
	}
	else if(word_state == 1)
	{
		word_state = 2;
		rx_buffer.len = 640;
		rx_buffer.buf = rx_buffer1 + 640*bufcnt;
	}
	else
	{
		rx_buffer.len = 640;
		rx_buffer.buf = rx_buffer1 + 640*bufcnt;
		cs4344_rx_isr_restart(&rx_buffer);
	}
}

static void cs4344_tx_isr(void)
{}

void CODEC_init()
{
	rx_buffer.buf = (int16_t *)rx_buffer1;
	rx_buffer.len = AUDIO_BUF_NUM;
	rx_buffer.ofs = 0;

	tx_buffer.buf = (int16_t *)audio_buf;
	tx_buffer.len = 16*1000*2;
	tx_buffer.ofs = 0;

	cs4344_rx_init(CS4344_SAMPLE_FREQ_16, CS4344_DATA_FORMAT_16, CS4344_MODE_ISR, &rx_buffer, cs4344_rx_isr);
	cs4344_tx_init(CS4344_SAMPLE_FREQ_16, CS4344_DATA_FORMAT_16, CS4344_MODE_ISR, &tx_buffer, cs4344_tx_isr);

	EMBARC_PRINTF("Please connet the mclk !\r\n");
	board_delay_ms(5000, OSP_DELAY_OS_COMPAT_DISABLE);
}
