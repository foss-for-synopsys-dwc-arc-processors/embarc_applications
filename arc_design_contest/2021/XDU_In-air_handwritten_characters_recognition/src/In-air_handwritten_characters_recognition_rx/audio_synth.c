#include "string.h"
#include "stdio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "dev_uart.h"
#include "dfss_uart_obj.h"

DEV_UART *dev_uart1 = NULL; //set a pointer of uart

//-----------------------------------------------------------------------------------
//      Delay function
//-----------------------------------------------------------------------------------
// static void delay_ms(volatile int z) //delay for 1ms
// {
//     volatile uint32_t x, y;
//     for (x = 1400; x > 0; x--)
//         for (y = z; y > 0; y--)
//             ;
// }

int audio_synth_init(uint32_t baudrate)
{

  dev_uart1 = uart_get_dev(DFSS_UART_1_ID); //set uart port as iot uart 1

  if (dev_uart1 == NULL)
    return -1;

  if (dev_uart1->uart_open(baudrate) == E_OPNED) //if uart was opened before this time,and the baudrate was different
  {
    dev_uart1->uart_control(UART_CMD_SET_BAUD, (void *)(baudrate)); //forc modify the baudrate
  }
	EMBARC_PRINTF("audio_synth INITIAL  OK!\n");

  return 0;
}

void audio_synth(uint8_t *text)
{
  char frame_info[50];
  uint8_t text_len;
  uint8_t frame_len;
  uint8_t ecc = 0;
  uint32_t i;

  text_len = strlen(text);      //data length
  frame_len = 5 + text_len + 1; //fram length

  frame_info[0] = 0xFD;         //frame header
  frame_info[1] = 0x00;         //low  bit of data area length
  frame_info[2] = text_len + 3; //high bit of data area length
  frame_info[3] = 0x01;         //command word
  frame_info[4] = 0x00;         //command parameter

  // memcpy(&frame_info[5], text, text_len);
  // write data from text to frame
  for (i = 0; i < text_len; i++)
  {
    frame_info[i + 5] = text[i];
  }

  //produce the crc
  for (i = 0; i < frame_len - 1; i++)
  {
    ecc = ecc ^ frame_info[i];
  }

  frame_info[frame_len - 1] = ecc;

  //print the frame
  // for (i = 0; i < frame_len; i++)
  // {
  //   EMBARC_PRINTF("frame_info[%2d] = 0x%02X\n", i, frame_info[i]);
  // }
  int state;
  //write frame through uart
  state=dev_uart1->uart_write(frame_info, (uint32_t)(frame_len));
  // EMBARC_PRINTF("uart_write state = \n\r",state);
  // delay_ms(1500);
  
	// board_delay_ms(1500, 1);
  // EMBARC_PRINTF("play end, next...\n\r");
}

// int main(uint8_t *speak)
// {

//   cpu_lock();   /* lock cpu to do initializations */
//   board_init(); /* board level init */
//   cpu_unlock(); /* unlock cpu to let interrupt work */

//   audio_synth_init(UART_BAUDRATE_9600); // set master baudrate
//   board_delay_ms(1000, 1);

  
//   // this file must be encoded as GB2312 if you want to use chinese character directly
//   audio_synth(speak);
  

//   return E_SYS;
// }

int AUDIO_SYNTH(uint8_t *speak)
{
  // audio_synth_init(UART_BAUDRATE_9600); // set master baudrate
  board_delay_ms(1000, 1);
  
  // this file must be encoded as GB2312 if you want to use chinese character directly
  audio_synth(speak);
  return E_SYS;
}