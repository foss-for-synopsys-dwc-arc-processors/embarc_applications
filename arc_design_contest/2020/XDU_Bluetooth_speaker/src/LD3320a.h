#ifndef _LD3320_H_
#define _LD3320_H_

#include "embARC.h"
#include "embARC_debug.h"
#include <string.h>
#include "IIC.h"
#include "delay.h"

//define the TEXT char
#define ASR_ADDR 0x79
#define ASR_RESULT_ADDR 100
#define ASR_WORDS_ERASE_ADDR 101

//识别模式设置，值范围1~3
//1：循环识别模式
//2：口令模式
//3：按键模式
#define ASR_MODE_ADDR 102

#define ASR_ADD_WORDS_ADDR 160

#define AUDIO_SW_CTRL_MODE      0x01
#define AUDIO_SW_PLAY_MODE      0x02
#define AUDIO_NEXT_SONG         0x03
#define AUDIO_PREV_SONG         0x04
#define AUDIO_PLAY_SONG         0x05
#define AUDIO_PAUSE_SONG        0x06
#define AUDIO_VOL_UP            0x07
#define AUDIO_VOL_DOWN          0x08

void    AsrErase(void);
void    AsrAddWords(uint8_t idNum, uint8_t words[], uint8_t len);
void    AsrSetMode(uint8_t mode);
uint8_t Ars_Result(void);

void    LD3320A_init(void);
uint8_t get_audio_cmd(void);

#endif
