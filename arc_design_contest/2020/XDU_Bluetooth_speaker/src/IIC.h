#ifndef _IIC_H_
#define _IIC_H_

#include "embARC.h"
#include "embARC_debug.h"

#define IIC_MASTER_0		        0

void Master0_slvdev_init(uint32_t slv_addr);
int32_t Master0_slvdev_write_reg(uint8_t regaddr, uint8_t regvalue);
int32_t Master0_slvdev_read_reg(uint8_t regaddr, uint8_t *data);

#endif
