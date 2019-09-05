#include "sccb.h"
int sccbInit(uint8_t devId) {
    int ercd = E_SYS;
    ov7670_sccb = iic_get_dev(devId);
    OV7670_CHECK_EXP_NORTN(ov7670_sccb != NULL);

    ercd = ov7670_sccb->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);
    ercd = ov7670_sccb->iic_control(IIC_CMD_MST_SET_TAR_ADDR, (void*)(OV7670_SCCB_ADDR));

error_exit:
    return ercd;
};

bool writeReg(uint8_t regAddr, uint8_t data) {
    // static int cnt = 0;
    int ercd = 0;
    uint8_t seq[2] = {regAddr, data};
    ov7670_sccb->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
    ercd = ov7670_sccb->iic_write(seq, 2);
    // EMBARC_PRINTF("%d\n", ++cnt);
    board_delay_ms(1, 0);
    return (ercd == 2)? true: false;
};

uint8_t readReg(uint8_t regAddr) {
    int ercd = 0;
    uint8_t data;
    ov7670_sccb->iic_control(IIC_CMD_MST_SET_NEXT_COND, (void*)(IIC_MODE_STOP));
    ercd = ov7670_sccb->iic_write(&regAddr, 1);
    ercd = ov7670_sccb->iic_read(&data, 1);
    board_delay_ms(1, 0);
    return data;
};