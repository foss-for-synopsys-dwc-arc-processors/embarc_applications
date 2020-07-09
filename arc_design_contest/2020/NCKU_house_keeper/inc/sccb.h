#ifndef SCCB_H
#define SCCB_H
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"

#define OV7670_SCCB_ADDR  0x21
#define OV7670_CHECK_EXP_NORTN(EXPR)       CHECK_EXP_NOERCD(EXPR, error_exit)

static DEV_IIC_PTR ov7670_sccb = NULL;

int sccbInit(uint8_t devId);

bool writeReg(uint8_t regAddr, uint8_t data);

uint8_t readReg(uint8_t regAddr);

#endif