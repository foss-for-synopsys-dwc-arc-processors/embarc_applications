#include "string.h"
#include "stdio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "dev_uart.h"
#include "dfss_uart_obj.h"

int audio_synth_init(uint32_t baudrate);
void audio_synth(uint8_t *text);
int AUDIO_SYNTH(uint8_t *speak);