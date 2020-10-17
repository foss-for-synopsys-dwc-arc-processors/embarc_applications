#ifndef TDRIVER_H
#define TDRIVER_H

#include "embARC.h"
#include "embARC_debug.h"
#include "board.h"
#include "dev_iic.h"

enum {
	TDR_READY = 0,
	TDR_IN_READ = 1,
	TDR_IN_WRITE = 2,
	TDR_IN_ERR = 3
};

typedef struct tdr {
    /* data */
    DEV_IIC *iic_obj;
    uint32_t tar_addr;
    uint8_t regaddr;
    uint8_t regval;
    uint8_t buf[2];
    uint8_t cur_state;
} TDR;

static void tdr_iic_txcb(void *ptr);
static void tdr_iic_rxcb(void *ptr);
static void tdr_iic_errcb(void *ptr);
void tdr_read_reg(TDR *tdr, uint8_t regaddr);
void tdr_write_reg(TDR *tdr, uint8_t regaddr, uint8_t regval);
int32_t tdr_chk_state(TDR *tdr);
bool tdr_state_ready(TDR *tdr);
bool tdr_state_err(TDR *tdr);
void tdr_clear_error(TDR *tdr);
void tdr_init(TDR *tdr, uint32_t iic_id, uint32_t addr);
#endif
