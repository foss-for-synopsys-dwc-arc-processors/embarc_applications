#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"  

    
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01 register operation command
#define NRF_READ_REG    0x00  //Read the configuration register, the lower 5 bits are the register address
#define NRF_WRITE_REG   0x20  //Write the configuration register, the lower 5 bits are the register address
#define RD_RX_PLOAD     0x61  //Read RX valid data, 1~32 bytes
#define WR_TX_PLOAD     0xA0  //Write TX valid data, 1~32 bytes
#define FLUSH_TX        0xE1  //Clear TX FIFO register. Used in transmit mode
#define FLUSH_RX        0xE2  //Clear the RX FIFO register. Used in receive mode
#define REUSE_TX_PL     0xE3  //The last packet of data is reused, CE is high, and the data packet is continuously sent.
#define NOP             0xFF  //No operation, can be used to read the status register	 
//SPI(NRF24L01)Register address
#define CONFIG          0x00  //Configuration register address; bit0: 1 receiving mode, 0 transmitting mode; bit1: electrical selection; bit2: CRC mode; bit3: CRC enable;
                              //bit4: Interrupt MAX_RT (Maximum Retransmission Interrupt) is enabled; bit5: Interrupt TX_DS is enabled; bit6: Interrupt RX_DR is enabled
#define EN_AA           0x01  //Enable auto answer function bit0~5, corresponding to channel 0~5
#define EN_RXADDR       0x02  //The receiving address is allowed, bit0~5, corresponding to channel 0~5
#define SETUP_AW        0x03  //Set the address width (all data channels): bit1,0:00, 3 bytes; 01, 4 bytes; 02, 5 bytes;
#define SETUP_RETR      0x04  //Establish automatic retransmission; bit3:0, automatic retransmission counter; bit7:4, automatic retransmission delay 250*x+86us
#define RF_CH           0x05  //RF channel, bit6:0, working channel frequency;
#define RF_SETUP        0x06  //RF register; bit3: transmission rate (0:1Mbps, 1:2Mbps); bit2:1, transmit power; bit0: low noise amplifier gain
#define STATUS          0x07  //Status register; bit0: TX FIFO full flag; bit3:1, receiving data channel number (maximum: 6); bit4, reaching the maximum number of retransmissions
                              //bit5: data transmission complete interrupt; bit6: receive data interrupt;
#define MAX_TX  		0x10  //Interrupt when the maximum number of transmissions is reached
#define TX_OK   		0x20  //TX transmission complete interrupt
#define RX_OK   		0x40  //Received data interrupt

#define OBSERVE_TX      0x08  //Transmission detection register, bit7:4, data packet loss counter; bit3:0, retransmission counter
#define CD              0x09  //Carrier detection register, bit0, carrier detection;
#define RX_ADDR_P0      0x0A  //Data channel 0 receiving address, maximum length 5 bytes, low byte first
#define RX_ADDR_P1      0x0B  //Data channel 1 receiving address, maximum length is 5 bytes, low byte first
#define RX_ADDR_P2      0x0C  //Data channel 2 receiving address, the lowest byte can be set, and the high byte must be equal to RX_ADDR_P1[39:8];
#define RX_ADDR_P3      0x0D  //Data channel 3 receiving address, the lowest byte can be set, and the high byte must be equal to RX_ADDR_P1[39:8];
#define RX_ADDR_P4      0x0E  //Data channel 4 receiving address, the lowest byte can be set, and the high byte must be equal to RX_ADDR_P1[39:8];
#define RX_ADDR_P5      0x0F  //Data channel 5 receiving address, the lowest byte can be set, and the high byte must be equal to RX_ADDR_P1[39:8];
#define TX_ADDR         0x10  //Send address (low byte first), in ShockBurstTM mode, RX_ADDR_P0 is equal to this address
#define RX_PW_P0        0x11  //Receive data channel 0 effective data width (1~32 bytes), set to 0 is illegal
#define RX_PW_P1        0x12  //Receive data channel 1 effective data width (1~32 bytes), set to 0 is illegal
#define RX_PW_P2        0x13  //Receive data channel 2 effective data width (1~32 bytes), set to 0 is illegal
#define RX_PW_P3        0x14  //Receive data channel 3 effective data width (1~32 bytes), set to 0 is illegal
#define RX_PW_P4        0x15  //Receive data channel 4 effective data width (1~32 bytes), set to 0 is illegal
#define RX_PW_P5        0x16  //Receive data channel 5 effective data width (1~32 bytes), set to 0 is illegal
#define NRF_FIFO_STATUS 0x17  //FIFO status register; bit0, RX FIFO register empty flag; bit1, RX FIFO full flag; bit2, 3, reserved
                               //bit4, TX FIFO empty flag; bit5, TX FIFO full flag; bit6, 1, cyclically send the last packet. 0, not cyclic;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//24L01 operation line
// #define NRF24L01_CE   PGout(8) //24L01 chip select signal
// #define NRF24L01_CSN  PGout(7) //SPI chip select signal	   
// #define NRF24L01_IRQ  PGin(6)  //IRQ host data input
//24L01 send and receive data width definition
#define TX_ADR_WIDTH    5   	//5-byte address width
#define RX_ADR_WIDTH    5   	//5-byte address width
#define TX_PLOAD_WIDTH  32  	//32-byte user data width
#define RX_PLOAD_WIDTH  32  	//32-byte user data width
									   	   

void NRF24L01_Init(void);						//nitialization
void NRF24L01_RX_Mode(void);					//Configure to receive mode
void NRF24L01_TX_Mode(void);					//Configure to send mode
void NRF24L01_CE_INITIAL(void);

void SPI_CS_INITIAL(void);
void SPI_CS_Clr(void);
void SPI_CS_Set(void);

void SPI_MOSI_INITIAL(void);
void SPI_MOSI_Clr(void);
void SPI_MOSI_Set(void);
void SPI_MOSI_WRITE(uint8_t data);

void SPI_MISO_INITIAL(void);
uint8_t SPI_MISO_READ(void);

void SPI_CLK_INITIAL(void);
void SPI_CLK_Clr(void);
void SPI_CLK_Set(void);

void NRF24L01_CE_INITIAL(void);
void NRF24L01_CE_Clr(void);
void NRF24L01_CE_Set(void);

void NRF24L01_IRQ_INITIA(void);
uint8_t NRF24L01_IRQ_READ(void);

uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t u8s);//Write data area
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t u8s);	//Read data area		  
uint8_t NRF24L01_Read_Reg(uint8_t reg);					//Read register
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);		//Write register
uint8_t NRF24L01_Check(void);						//Check if 24L01 exists
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);				//Send a packet of data
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);				//Receive a packet of data












