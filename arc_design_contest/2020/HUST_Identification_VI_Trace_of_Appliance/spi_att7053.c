#include "spi_att7053.h"


#include "embARC_error.h"
#include "board.h"
#include "dev_spi.h"
#include "arc_exception.h"
#include "string.h"




/**
 * \brief	att7053 spi send command to operate att7053
 * \param[in]	xfer	spi transfer that need to transfer to spi device
 * \retval	0	success
 * \retval	-1	fail
 */
Inline int32_t spi_send_cmd(ATT7053_DEF_PTR dev, DEV_SPI_TRANSFER *xfer)
{
	DEV_SPI_PTR spi_att7053;
	int32_t ercd = 0;

	spi_att7053 = spi_get_dev(dev->spi_master);

	/* select device */
	spi_att7053->spi_control(SPI_CMD_MST_SEL_DEV, CONV2VOID((uint32_t)dev->cs));
	spi_att7053->spi_control(SPI_CMD_MST_SET_FREQ, CONV2VOID(dev->spi_freq));
	spi_att7053->spi_control(SPI_CMD_SET_CLK_MODE, CONV2VOID(SPI_CLK_MODE_1));
	ercd = spi_att7053->spi_control(SPI_CMD_TRANSFER_POLLING, CONV2VOID(xfer));
	/* deselect device */
	spi_att7053->spi_control(SPI_CMD_MST_DSEL_DEV, CONV2VOID((uint32_t)dev->cs));

	return ercd;
}



/**
 * \brief	init att7053 related interface
 * \retval	0	success
 * \retval	-1	fail
 */
int32_t ATT7053_init(ATT7053_DEF_PTR dev, uint32_t freq)
{
	DEV_SPI_PTR spi_att7053;
	int32_t ercd = 0;

	dev->spi_freq = freq;
	spi_att7053 = spi_get_dev(dev->spi_master);

	ercd = spi_att7053->spi_open(DEV_MASTER_MODE, freq);

	if (ercd != E_OK && ercd != E_OPNED) {
		return ercd;
	}
  // spi_att7053->spi_control(SPI_CMD_SET_DUMMY_DATA, CONV2VOID(0xFF));

	return E_OK;
}





uint32_t Read_Reg(ATT7053_DEF_PTR dev,uint8_t cmd) {
  uint32_t data = 0;
  uint8_t tmp[3] = {0};
  DEV_SPI_TRANSFER cmd_xfer;

  DEV_SPI_XFER_SET_TXBUF(&cmd_xfer, &cmd, 0, 1);
  DEV_SPI_XFER_SET_RXBUF(&cmd_xfer, tmp, 1, 3);
  DEV_SPI_XFER_SET_NEXT(&cmd_xfer, NULL);

  spi_send_cmd(dev, &cmd_xfer);

  data |= (uint32_t)tmp[0] << 16;
  data |= (uint32_t)tmp[1] << 8;
  data |= (uint32_t)tmp[2];

  return data;
}

void Write_Reg(ATT7053_DEF_PTR dev,uint8_t cmd, uint32_t data) {
  uint8_t tmp[4];
  DEV_SPI_TRANSFER cmd_xfer;

  tmp[0] = cmd | 0x80;
  tmp[1] = (data >> 16) & 255;
  tmp[2] = (data >> 8) & 255;
  tmp[3] = (uint8_t)data;

  // spi_init(ATT7053_SPI_INDEX, SPI_WORK_MODE_1, SPI_FF_STANDARD, 8, 0);
  DEV_SPI_XFER_SET_TXBUF(&cmd_xfer, tmp, 0, 4);
  DEV_SPI_XFER_SET_RXBUF(&cmd_xfer, NULL, 0, 0);
  DEV_SPI_XFER_SET_NEXT(&cmd_xfer, NULL);

  spi_send_cmd(dev, &cmd_xfer);
}


/**
 * \brief	provide US delay function
 * \details
 * 		this function needs a 1-US timer interrupt to work.
 * 	For bare-metal, it is implemented in this file.
 * \param[in]	us		US to delay
 */
void usleep(uint32_t us)
{
	uint64_t start_us, us_delayed;
	us_delayed = ((uint64_t)us );
	start_us = board_get_cur_us();
	while ((board_get_cur_us() - start_us) < us_delayed);
}

void msleep(uint32_t ms)
{
	uint64_t start_us, us_delayed;
	us_delayed = ((uint64_t)ms*1000 );
	start_us = board_get_cur_us();
	while ((board_get_cur_us() - start_us) < us_delayed);
}



void EMU_Init(ATT7053_DEF_PTR dev) {
  Write_Reg(dev,Soft_Reset_Register, 0x55);  //软件复位
  msleep(50);
  //printf("软件复位SRSTREG = 0x%x\r\n", Read_Reg(Soft_Reset_Register));
  EMBARC_PRINTF(" SRSTREG = 0x%x\r\n", Read_Reg(dev,Soft_Reset_Register));

  // 0xA6: 写保护打开，只能操作 50H 到 71H 的校表参数寄存器，
  // 不可操作 40H 到 45H的校表参数寄存器
  Write_Reg(dev,Written_Protect_Register, 0xA6);
  usleep(50);
  //printf("写保护寄存器WPREG = 0x%x\r\n", Read_Reg(Written_Protect_Register));

  // ADC 通道增益 bit0~bit5表示模拟通道增益
  // bit6~bit11表示数字增益
  Write_Reg(dev,ADC_Channel_Gain_Register, 0x00);
  usleep(50);
  //printf("ADC通道增益ADCCON:0x%x\r\n", Read_Reg(ADC_Channel_Gain_Register));

  //脉冲频率 默认0x0040
  //Write_Reg(dev,High_Frequency_Impulse_Const_Register, 0x61);
  usleep(50);
  //printf("脉冲频率设置 HFConst:0x%x\r\n",
  Read_Reg(dev,High_Frequency_Impulse_Const_Register);


  // Write_Reg(dev,Active_Power_Gain1_Register, 0x0);
  // usleep(50);

  // Write_Reg(dev,Power_Offset1, 0xF5);
  // usleep(50);

  //电流通道1有效值校正寄存器
  Write_Reg(dev,I1_RMS_Offset, 0x00);
  usleep(50);
  //printf("电流1校正I1RMSOFFSET:0x%x\r\n", Read_Reg(I1_RMS_Offset));

  //电流通道2有效值校正寄存器
  Write_Reg(dev,I2_RMS_Offset, 0x00);
  usleep(50);
  //printf("电流1校正I1RMSOFFSET:0x%x\r\n", Read_Reg(I2_RMS_Offset));

  // 0xBC: 写保护打开，只能操作 40H 到 45H 的校表参数寄存器，
  // 不可操作 50H 到 71H的校表参数寄存器
  Write_Reg(dev,Written_Protect_Register, 0xBC);
  usleep(50);
  //printf("写保护寄存器WPREG = 0x%x\r\n", Read_Reg(Written_Protect_Register));

  // EMU配置,视在电能输出
  Write_Reg(dev,EMUCFG, 0x100);
  usleep(50);
  //printf("EMUCFG = 0x%x\r\n", Read_Reg(EMUCFG));

  //配置时钟
  Write_Reg(dev,Frequency_Configure_Register, 0x8B);
  usleep(50);
  //printf("时钟配置FreqCFG = 0x%x\r\n", Read_Reg(Frequency_Configure_Register));

  //打开高通滤波器，脉冲计数开关打开,默认0x7E
  Write_Reg(dev,ModuleEn, 0x7E);
  usleep(50);
  //printf("模式控制ModuleEN = 0x%x\r\n", Read_Reg(ModuleEn));  // 最低位写不进去

  //打开ADC
  Write_Reg(dev,Analog_Enable_Register, 0x03);
  usleep(50);
  //printf("ADC开关ANAEN = 0x%x\r\n", Read_Reg(Analog_Enable_Register));

  //中断输出低电平有效
  Write_Reg(dev,IO_Configuration_Register, 0x00);
  usleep(50);
  //printf("输出引脚配置IOCFG = 0x%x\r\n", Read_Reg(IO_Configuration_Register));

  // 0x40表示波形寄存器的更新中断使能
  Write_Reg(dev,EMU_Interrupt_Enable_Register, 0x04);
  usleep(50);
  //printf("中断使能EMUIE = 0x%x\r\n", Read_Reg(EMU_Interrupt_Enable_Register));

  //关闭写保护寄存器
  Write_Reg(dev,Written_Protect_Register, 0x00);
  usleep(50);
  //printf("写保护寄存器WPREG = 0x%x\r\n", Read_Reg(Written_Protect_Register));
}