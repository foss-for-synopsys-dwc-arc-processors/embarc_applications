#ifndef _SPI_ATT7053_H_
#define _SPI_ATT7053_H_


#include "embARC_toolchain.h"
#include "embARC_error.h"
#include "embARC_debug.h"


#define WAVE_SIZE 1024



/**
 * \name ATT7053 SPI Commands
 * @{
 */
// define ATT7053 internal addresses
#define Current1_Waveform 0x00
#define Current2_Waveform 0x01
#define Votage_Waveform 0x02
#define Active_Power_Waveform 0x03
#define Reactive_Power_Waveform 0x04
#define Apparent_Power_Waveform 0x05
#define Current1_Rms_Register 0x06
#define Current2_Rms_Register 0x07
#define Voltage_Rms_Register 0x08
#define Voltage_Frequency_Register 0x09
#define Active_Power_Register 0x0A
#define Reactive_Power_Register 0x0B
#define Apparent_Power_Register 0x0C
#define Active_Energy_Register 0x0D
#define Reactive_Energy_Register 0x0E
#define Apparent_Energy_Register 0x0F
#define PowerP2_Register 0x10
#define PowerQ2_Register 0x11
#define I1_Angle_Register 0x12
#define I2_Angle_Register 0x13
#define TempData_Register 0x14
#define BackupData_Register 0x16
#define ComChecksum_Register 0x17
#define SumChecksum_Register 0x18
#define EMU_Status_Register 0x19

//List of calibration parameter registers
#define EMU_Interrupt_Enable_Register 0x30
#define EMU_Interrupt_Flag_Register 0x31
#define Written_Protect_Register 0x32
#define Soft_Reset_Register 0x33

#define EMUCFG 0x40
#define Frequency_Configure_Register 0x41
#define ModuleEn 0x42
#define Analog_Enable_Register 0x43
#define IO_Configuration_Register 0x45

#define Active_Power_Gain1_Register 0x50
#define Reactive_Power_Gain1_Register 0x51
#define Apparent_Power_Gain1_Register 0x52
#define Phase_Calibration1_Register 0x53
#define Active_Power_Gain2_Register 0x54
#define Reactive_Power_Gain2_Register 0x55
#define Apparent_Power_Gain2_Register 0x56
#define Phase_Calibration2_Register 0x57
#define Reactive_Power_Phase_Calibration_Register 0x58
#define ADC_Channel_Gain_Register 0x59
#define All_Channel_Gain_Register 0x5A
#define Current2_Gain_Register 0x5B
#define Current1_Offset_Register 0x5C
#define Current2_Offset_Register 0x5D
#define Voltage_Offset_Register 0x5E
#define Start_Power_Threshold_Setup_Register 0x5F
#define Start_RMS_Threshold_Setup_Register 0x60
#define High_Frequency_Impulse_Const_Register 0x61
#define Check_Register 0x62
#define Tamper_Current_Power_Register 0x63
#define UCONST 0x64
#define Power_Offset1 0x65
#define Power_Offset2 0x66
#define Reactive_Power_Offset1 0x67
#define Reactive_Power_Offset2 0x68
#define I1_RMS_Offset 0x69
#define I2_RMS_Offset 0x6A
#define URMS_Offset 0x6B
#define ZeroCross_Current_Configuration_Register 0x6C
#define Phase_Calibration_Gain1_Register 0x6D
#define Phase_Calibration_Gain2_Register 0x6E
#define Active_Energy_Counter_Register 0x6F
#define Reactive_Energy_Counter_Register 0x70
#define Apparent_Energy_Counter_Register 0x71
/** @} end of name */


/** att7053 data type */
typedef struct {
	uint32_t Current_buf[WAVE_SIZE];
	uint32_t Votage_buf[WAVE_SIZE];
	uint32_t Active_Power;
} data_t;

/** att7053 object type */
typedef struct att7053_t {
	uint8_t spi_master;
	uint8_t cs;
	uint32_t spi_freq;
} ATT7053_DEF, * ATT7053_DEF_PTR;

#define ATT7053_DEF(name, spi_master_id, cs_line) \
	ATT7053_DEF __ ## name = { \
		.spi_master = spi_master_id, \
		.cs = cs_line \
	}; \
	ATT7053_DEF_PTR name = &__ ## name \

#ifdef __cplusplus
extern "C" {
#endif


extern void EMU_Init(ATT7053_DEF_PTR dev);
extern void Write_Reg(ATT7053_DEF_PTR dev,uint8_t cmd, uint32_t data);
extern uint32_t Read_Reg(ATT7053_DEF_PTR dev,uint8_t cmd);
extern int32_t ATT7053_init(ATT7053_DEF_PTR dev, uint32_t freq);
extern void msleep(uint32_t ms);
extern void usleep(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif
