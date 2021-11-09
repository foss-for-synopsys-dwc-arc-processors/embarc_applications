
// embARC
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
// Standard
#include "stdio.h"
// DEVICE-iotdk
#include "dfss_gpio_obj.h"
// 24L01-WiFi
#include "24l01.h"
// OLED
#include "oled.h"

#define NRF24L01 DFSS_GPIO_8B0_ID
#define SPI_PORT_CS_MASK	0x01		//[0]-CS	OUTPUT
#define SPI_PORT_CS_DIR		0x00	
#define SPI_PORT_MOSI_MASK	0x02		//[1]-MOSI	OUTPUT
#define SPI_PORT_MOSI_DIR	0x00	
#define SPI_PORT_MISO_MASK	0x04		//[2]-MISO	INPUT
#define SPI_PORT_MISO_DIR	0x01	
#define SPI_PORT_CLK_MASK	0x08		//[3]-CLK	OUTPUT
#define SPI_PORT_CLK_DIR	0x00	
#define CE_MASK				0x10		//[4]-CE	OUTPUT
#define CE_DIR				0x00	
#define IRQ_MASK			0x20		//[5]-IRQ	INPUT
#define IRQ_DIR				0x20	


DEV_GPIO *spi_port;
static DEV_GPIO_PTR spi_port_cs;     	//AD0  gpio8b_0[0]
static DEV_GPIO_PTR spi_port_mosi;     	//AD1  gpio8b_0[1]
static DEV_GPIO_PTR spi_port_miso;     	//AD2  gpio8b_0[2]
static DEV_GPIO_PTR spi_port_clk;     	//AD3  gpio8b_0[3]
static DEV_GPIO_PTR NRF24L01_CE;		//AD4  gpio8b_0[4]
static DEV_GPIO_PTR NRF24L01_IRQ;		//AD4  gpio8b_0[5]

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};	//transport address
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};	//receive address


//-------------------------------------------------------------------------------
//      SPI 
//-------------------------------------------------------------------------------
//SPI_CS set
void SPI_CS_INITIAL(void)
{
	spi_port_cs =  gpio_get_dev(NRF24L01);					//set AD4 port
	spi_port_cs -> gpio_open(SPI_PORT_CS_MASK);				//AD43
  	spi_port_cs -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)SPI_PORT_CS_MASK);										
															//set output
}
void SPI_CS_Clr(void)		//clear 0
{   
	spi_port_cs -> gpio_write(0x00,SPI_PORT_CS_MASK);
}
void SPI_CS_Set(void)		//set 1
{
	spi_port_cs -> gpio_write(0x01,SPI_PORT_CS_MASK);
}

//SPI_MOSI set
void SPI_MOSI_INITIAL(void)
{
   spi_port_mosi =  gpio_get_dev(NRF24L01);  				//set AD1 port
   spi_port_mosi -> gpio_open(SPI_PORT_MOSI_MASK);  		//AD1[6]
   spi_port_mosi -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)SPI_PORT_MOSI_MASK);  
   															//set output
}
void SPI_MOSI_Clr(void)		//clear 0
{
   spi_port_mosi -> gpio_write(0x00,SPI_PORT_MOSI_MASK);
}
void SPI_MOSI_Set(void)		//set 1
{
   spi_port_mosi -> gpio_write(0x02,SPI_PORT_MOSI_MASK);
}
void SPI_MOSI_WRITE(uint8_t data)
{
	if (data & 0x80)
		SPI_MOSI_Set();
	else
		SPI_MOSI_Clr();
}

//SPI_MISO set
void SPI_MISO_INITIAL(void)
{
   spi_port_miso =  gpio_get_dev(NRF24L01);  				//set AD1 port
   spi_port_miso -> gpio_open(SPI_PORT_MISO_MASK);  		//AD1[6]
   spi_port_miso -> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)SPI_PORT_MISO_MASK);  
   															//set input
}
//read data
uint8_t SPI_MISO_READ(void)		
{
	uint32_t temp=0;
	spi_port_miso -> gpio_read(&temp,SPI_PORT_MISO_MASK);
	temp = temp >> 2;
	if (temp == 0x01)
		return 0x01;
	else
		return 0x00;
}

//SPI_CLK set
void SPI_CLK_INITIAL(void)
{
   spi_port_clk =  gpio_get_dev(NRF24L01);  				//set AD0 port
   spi_port_clk -> gpio_open(SPI_PORT_CLK_MASK);  			//AD0[7]
   spi_port_clk -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)SPI_PORT_CLK_MASK);  
   															// set output
}
void SPI_CLK_Clr(void)		//clear 0
{
   spi_port_clk -> gpio_write(0x00,SPI_PORT_CLK_MASK);
}
void SPI_CLK_Set(void)		//set 1
{
   spi_port_clk -> gpio_write(0x08,SPI_PORT_CLK_MASK);
}

//CE set 
void NRF24L01_CE_INITIAL(void)
{
   NRF24L01_CE =  gpio_get_dev(NRF24L01);  					//set AD0 port
   NRF24L01_CE -> gpio_open(CE_MASK);  				//AD0[7]
   NRF24L01_CE -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)CE_MASK);  
   															// set output
}
void NRF24L01_CE_Clr(void)		//clear 0
{
   NRF24L01_CE -> gpio_write(0x00,CE_MASK);
}
void NRF24L01_CE_Set(void)		//set 1
{
   NRF24L01_CE -> gpio_write(0x10,CE_MASK);
}

//IRQ set 
void NRF24L01_IRQ_INITIA(void)
{
   NRF24L01_IRQ =  gpio_get_dev(NRF24L01);  				//set AD1 port
   NRF24L01_IRQ -> gpio_open(IRQ_MASK);  		//AD1[6]
   NRF24L01_IRQ -> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)IRQ_MASK);  
   															// set input
}
//read data
uint8_t NRF24L01_IRQ_READ(void)		
{
	uint32_t temp=0;
	NRF24L01_IRQ -> gpio_read(&temp,IRQ_MASK);
	temp = temp >> 5;
	if (temp == 0x01)
		return 0x01;
	else
		return 0x00;
}

//init 24L01
void NRF24L01_Init(void)
{ 	
	SPI_CS_INITIAL();
	SPI_MOSI_INITIAL();
	SPI_MISO_INITIAL();
	SPI_CLK_INITIAL();
	NRF24L01_CE_INITIAL();
	NRF24L01_CE_Clr();			//enable 24L01
	SPI_CS_Set();				//SPI Chip Select cancel
	SPI_CLK_Clr();				//SCK is pulled low when not communicating
	EMBARC_PRINTF("NRF24L01 INITIAL  OK!\n\r");
	 		 	 
}



//------------------------------------------------------------------------
//      SPI reads and writes a byte at the same time
//------------------------------------------------------------------------
//		write_data : Data to be written
//		read_date  : Data to be read
//------------------------------------------------------------------------
uint8_t SPI_RW(uint8_t write_data)
{
	uint8_t bit_ctr;
	uint8_t read_data;
	read_data = 0;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++) 		// send 8-bits data
	{
		SPI_MOSI_WRITE(write_data & 0x80);	// write MSB into MOSI
		write_data = (write_data << 1); 	// Shift next to MSB
		SPI_CLK_Set(); 						// Pull SCK high

		read_data = read_data << 1;			// Shift the read data to the left
		// EMBARC_PRINTF("SPI_MISO_READ =%d\n",SPI_MISO_READ);
		if (SPI_MISO_READ() == 0x01)
			read_data ++;; 					// Capture current MISO online data
		SPI_CLK_Clr(); 						// Pull SCK low
	}
	return(read_data); 						// Returns the 8-bits data read
}

//------------------------------------------------------------------------
//      Read NRF24L01 register value
//------------------------------------------------------------------------
//		reg : Register to be read
//------------------------------------------------------------------------
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;	    

	SPI_CS_Clr();                				//Enable SPI transmission		
  	SPI_RW(reg);   							//Send register number
  	reg_val=SPI_RW(0XFF);					//Read register contents
  	SPI_CS_Set();                 			//Disable SPI transmission	   	    
  	return(reg_val);           				//Return status value
}	

//------------------------------------------------------------------------
//      Write the value to the NRF24L01 register
//------------------------------------------------------------------------
//		reg   : Specify register address
//		value : Value written
//------------------------------------------------------------------------
uint8_t NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;	

   	SPI_CS_Clr();                				//Enable SPI transmission
  	status =SPI_RW(reg);					//Send register number 
  	SPI_RW(value);      					//Value written to register
  	SPI_CS_Set();                 			//Disable SPI transmission	   
  	return(status);       					//Return status value
}

//------------------------------------------------------------------------
//      Read the specified length of data at the specified location
//------------------------------------------------------------------------
//		reg    : Specify register address
//		*pBuf  : Data pointer
//		len    : Data length
//		status : Status register value read this time 
//------------------------------------------------------------------------
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
	uint8_t status,int8_t_ctr;	       
	SPI_CS_Clr();                				//Enable SPI transmission	
  	status=SPI_RW(reg); 					//Send register value (position) and read status value

 	for(int8_t_ctr=0;int8_t_ctr<len;int8_t_ctr++)
		pBuf[int8_t_ctr]=SPI_RW(0XFF);		//Read data continuously
  	SPI_CS_Set();                 			//	Disable SPI transmission
  	return status;        					//Return the status value read
}

//------------------------------------------------------------------------
//      Write data of the specified length at the specified location
//------------------------------------------------------------------------
//		reg    : Specify register address
//		*pBuf  : Data pointer
//		len    : Data length
//		status : Status register value read this time 
//------------------------------------------------------------------------
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
	uint8_t status,int8_t_ctr;	    
	SPI_CS_Clr();                				//Enable SPI transmission	
  	status = SPI_RW(reg); 					//Send register value (position) and read status value

  	for(int8_t_ctr=0; int8_t_ctr<len; int8_t_ctr++)
		SPI_RW(*pBuf++); 					//Write data continuously	 
  	SPI_CS_Set();                 			//Disable SPI transmission	
  	return status;          				//Return the status value read
}	

//------------------------------------------------------------------------
//      This function initializes NRF24L01 to RX mode
//------------------------------------------------------------------------
//		 set RX address, write RX data width, select RF channel, baud rate and LNA HCURR
//		When CE goes high, it enters RX mode and can receive data
//------------------------------------------------------------------------
void NRF24L01_RX_Mode(void)
{
	NRF24L01_CE_Clr();	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);	//0A-Write RX node address
	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    								//01-Enable automatic answer for channel 0    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);								//02-Enable the receiving address of channel 0  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     								//05- set RF communication frequency		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);						//11-Select the effective data width of channel 0 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);								//06- set TX transmission parameters, 0db gain, 2Mbps, low noise gain on 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f); 								//00-Configure the parameters of the basic working mode; PWR_UP, EN_CRC, 16BIT_CRC, receiving mode
  	NRF24L01_CE_Set(); 																//CE is high, enter the receiving mode
}		

//------------------------------------------------------------------------
//      This function initializes NRF24L01 to TX mode
//------------------------------------------------------------------------
//		set TX address, write TX data width, set RX address for automatic response, fill TX sending data, select RF channel, baud rate and LNA HCURR
//		 PWR_UP, CRC enable
//		 When CE goes high, it enters RX mode and can receive data
// 		 CE is higher than 10us, then start sending.
//------------------------------------------------------------------------	 
void NRF24L01_TX_Mode(void)
{														 
	NRF24L01_CE_Clr();	    
	
	// EMBARC_PRINTF("NRF24L01_CE_Clr OK!\n\n");	
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);	//10-Write TX node address 
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //0A- set TX node address, mainly to enable ACK	  

  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     								//01-Enable automatic answer for channel 0    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); 								//02-Enable the receiving address of channel 0  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);								//04- set Automatic retransmission interval: 500us + 86us; maximum number of automatic retransmissions: 10 times
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       								//05- set RF channel is 40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  								//06- set TX transmission parameters, 0db gain, 2Mbps, low noise gain on   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    								//00-Configure the parameters of the basic working mode; PWR_UP, EN_CRC, 16BIT_CRC, transmit mode, turn on all interrupts
	NRF24L01_CE_Set();																//CE is high, start sending after 10us
	EMBARC_PRINTF("NRF24L01_TX_Mode OK!\n\n");	
}

//Check if 24L01 exists
//Return value: 0, success; 1, failure	
// uint8_t NRF24L01_Check(void)
// {
// 	uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
// 	uint8_t i;
// 	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); //The spi speed is 9Mhz (the maximum SPI clock of 24L01 is 10Mhz) 
// 	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//Write the address of 5 bytes.	
// 	NRF24L01_Read_Buf(TX_ADDR,buf,5); //Read the written address  
// 	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;	 							   
// 	if(i!=5)return 1;//Detect 24L01 error	
// 	return 0;		 //24L01 detected
// }	 	 

//------------------------------------------------------------------------
//      Start NRF24L01 to send data once
//------------------------------------------------------------------------
//		txbuf  : The first address of the data to be sent
//		Return value: Sending completion status
//------------------------------------------------------------------------
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
	uint8_t sta;
 	// SPI2_SetSpeed(SPI_BaudRatePrescaler_8);//The spi speed is 9Mhz (the maximum SPI clock of 24L01 is 10Mhz)   
	NRF24L01_CE_Clr();
  	NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);	//Write data to TX BUF 32 bytes
 	NRF24L01_CE_Set();										//Start sending	   
	while(NRF24L01_IRQ!=0);									//Wait for sending to complete
	sta=NRF24L01_Read_Reg(STATUS);  						//Read the value of the status register	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta);			//Clear TX_DS or MAX_RT interrupt flag
	if(sta&MAX_TX)											//Maximum number of retransmissions reached
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);					//Clear TX FIFO register 
		EMBARC_PRINTF("attach max time\n\n");
		return MAX_TX; 
	}
	if(sta&TX_OK)											//Send complete
	{
		return TX_OK;
		EMBARC_PRINTF("send ok!\n\n");
	}
	return 0xff;											//Failed to send for other reasons
}

//------------------------------------------------------------------------
//      Start NRF24L01 to receive data once
//------------------------------------------------------------------------
//		rxbuf  : The first address of the data to be sent
//		Return value: 0, reception is complete; others, error code
//------------------------------------------------------------------------
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
	uint8_t sta;		    							   
	// SPI2_SetSpeed(SPI_BaudRatePrescaler_8); //The spi speed is 9Mhz (the maximum SPI clock of 24L01 is 10Mhz)   
	sta=NRF24L01_Read_Reg(STATUS);  						//Read the value of the status register    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 			//Clear TX_DS or MAX_RT interrupt flag
	if(sta&RX_OK)											//Data received
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//Read data
		NRF24L01_Write_Reg(FLUSH_RX,0xff);					//Clear RX FIFO register
		return 0; 
	}	   
	return 1;//Did not receive any data
}					    



// void main()
// {	
// 	cpu_lock();
// 	board_init(); 
// 	cpu_unlock();

// 	// OLED_Configuration();//set oled
// 	// OLED_Clear();
// 	// OLED_ShowString(10, 2, "TEST BEGINING!", 16);
//     uint8_t     key,mode;
//     uint16_t    t;
//     uint8_t     tmp_buf[33];

// 	NRF24L01_Init();
// 	EMBARC_PRINTF("NRF24L01 INITIAL OK!\n\n");
// // for(t=0;t<32;t++)
// // 		    	{
// // 		    		key++;
// // 		    		if(key>('~'))key=' ';
// // 		    		tmp_buf[t]=key;	
// // 		    	}
//     // LCD_ShowString(30,150,200,16,16,"NRF24L01 TX_Mode");	
// 	NRF24L01_RX_Mode();
	
// 	EMBARC_PRINTF("NRF24L01_TX_Mode OK!\n\n");
// 	mode=' ';//Start with the space bar  
// 	while(1)
// 	{	
// 		// OLED_Clear();		
// 		// EMBARC_PRINTF("NRF24L01_RxPacket == %x\n\n",NRF24L01_RxPacket(tmp_buf));		 
// 		if(NRF24L01_RxPacket(tmp_buf)==0)
// 			{
// 		    	tmp_buf[32]=0;// Add end of string
// 				// OLED_ShowString(10, 2,tmp_buf, 16);
// 				EMBARC_PRINTF("RECEIVE DATA = %s\n\n",tmp_buf);
// 			}
// 		else board_delay_ms(1000, 1);			    
// 	};
// }
// void NRF24L01_RX_Mode_config()
// {

// }

uint8_t NRF24L01_TX(uint8_t *tmp_buf)
{ 	
	uint8_t state;
	// EMBARC_PRINTF("wifi test 1\r\n",tmp_buf);
	// OLED_Clear();	
	OLED_ShowString(1, 6, "Sending data...", 8);
	while (NRF24L01_TxPacket(tmp_buf) != TX_OK)
	{
		EMBARC_PRINTF("wifi test 2\r\n",tmp_buf);						   	
		
	}
	// OLED_Clear();
	OLED_ShowString(1, 6, "Sending data done!", 8);   
	EMBARC_PRINTF("SEND DATA = %s\r\n",tmp_buf);
	return(0);

	// if(NRF24L01_TxPacket(tmp_buf)==TX_OK)
	// 	{	
	// 		OLED_Clear();
	// 		OLED_ShowString(0, 1, "You haved written :", 8);    
	// 		OLED_ShowString(0, 2, tmp_buf, 8);    
	// 		EMBARC_PRINTF("SEND DATA = %s\r\n",tmp_buf);

	// 		// strcpy(tmp_buf, "hello");
	// 		return(0);

	// 	}
	// else
	// 	{			
	// 		OLED_Clear();							   	
	// 		OLED_ShowString(10, 5, "NRF24L01 ERROR!", 16);
	// 		return(-1);
	// 	};

	// board_delay_ms(1000, 1);
}

void Write_line(int line,uint8_t *tmp_buf)
{
	uint8_t str[23];

	sprintf(str,"w%d%s",line,tmp_buf);
	NRF24L01_TX(str);
}

void Clean_char(int line,uint8_t *tmp_buf)
{
	uint8_t str[23];

	sprintf(str,"e%d%s",line,tmp_buf);
	NRF24L01_TX(str);
}

void Change_mode(int mode)
{
	if(mode)
		NRF24L01_TX("c1");
	else
		NRF24L01_TX("c0");
}

void Up_line(int line)
{

	uint8_t str[23];
	sprintf(str,"u%d",line);
	NRF24L01_TX(str);
}

void Down_line(int line)
{
	uint8_t str[23];
	sprintf(str,"d%d",line);
	NRF24L01_TX(str);
}

void Cannot_clean()
{
	uint8_t str[23];
	sprintf(str,"ne");
	NRF24L01_TX(str);
}

void Cannot_write()
{
	uint8_t str[23];
	sprintf(str,"nw");
	NRF24L01_TX(str);
}