#ifndef _LD3320_H_
#define _LD3320_H_

#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "delay.h"

#include <string.h>
#include <stdio.h>
#include "dht11.h"
#include "SYN6288.h"
#include "PluseSensor.h"

#define CMD_START            0x00
#define CMD_TEMPERATURE      0x01
#define CMD_HEARTRATE        0x02
#define CMD_MOTIONSTATE      0x03
#define CMD_MOVEMENTMDOE     0x04
#define CMD_END              0x05


uint8_t Ars_Result(void);
void    LD3320A_init(void);
uint8_t get_audio_cmd(void);

#endif
