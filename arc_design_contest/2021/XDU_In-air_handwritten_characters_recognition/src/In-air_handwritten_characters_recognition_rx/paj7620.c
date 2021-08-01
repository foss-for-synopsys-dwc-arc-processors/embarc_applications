#include "paj7620.h"
#include "delay.h"
#include "dwin.h"
DEV_IIC * iic_master_2 = NULL;

/* Registers' initialization data */
unsigned char initRegisterArray[][2] = {	// Initial Gesture
    {0xEF,0x00},
	{0x32,0x29},
	{0x33,0x01},
	{0x34,0x00},
	{0x35,0x01},
	{0x36,0x00},
	{0x37,0x07},
	{0x38,0x17},
	{0x39,0x06},
	{0x3A,0x12},
	{0x3F,0x00},
	{0x40,0x02},
	{0x41,0xFF},
	{0x42,0x01},
	{0x46,0x2D},
	{0x47,0x0F},
	{0x48,0x3C},
	{0x49,0x00},
	{0x4A,0x1E},
	{0x4B,0x00},
	{0x4C,0x20},
	{0x4D,0x00},
	{0x4E,0x1A},
	{0x4F,0x14},
	{0x50,0x00},
	{0x51,0x10},
	{0x52,0x00},
	{0x5C,0x02},
	{0x5D,0x00},
	{0x5E,0x10},
	{0x5F,0x3F},
	{0x60,0x27},
	{0x61,0x28},
	{0x62,0x00},
	{0x63,0x03},
	{0x64,0xF7},
	{0x65,0x03},
	{0x66,0xD9},
	{0x67,0x03},
	{0x68,0x01},
	{0x69,0xC8},
	{0x6A,0x40},
	{0x6D,0x04},
	{0x6E,0x00},
	{0x6F,0x00},
	{0x70,0x80},
	{0x71,0x00},
	{0x72,0x00},
	{0x73,0x00},
	{0x74,0xF0},
	{0x75,0x00},
	{0x80,0x42},
	{0x81,0x44},
	{0x82,0x04},
	{0x83,0x20},
	{0x84,0x20},
	{0x85,0x00},
	{0x86,0x10},
	{0x87,0x00},
	{0x88,0x05},
	{0x89,0x18},
	{0x8A,0x10},
	{0x8B,0x01},
	{0x8C,0x37},
	{0x8D,0x00},
	{0x8E,0xF0},
	{0x8F,0x81},
	{0x90,0x06},
	{0x91,0x06},
	{0x92,0x1E},
	{0x93,0x0D},
	{0x94,0x0A},
	{0x95,0x0A},
	{0x96,0x0C},
	{0x97,0x05},
	{0x98,0x0A},
	{0x99,0x41},
	{0x9A,0x14},
	{0x9B,0x0A},
	{0x9C,0x3F},
	{0x9D,0x33},
	{0x9E,0xAE},
	{0x9F,0xF9},
	{0xA0,0x48},
	{0xA1,0x13},
	{0xA2,0x10},
	{0xA3,0x08},
	{0xA4,0x30},
	{0xA5,0x19},
	{0xA6,0x10},
	{0xA7,0x08},
	{0xA8,0x24},
	{0xA9,0x04},
	{0xAA,0x1E},
	{0xAB,0x1E},
	{0xCC,0x19},
	{0xCD,0x0B},
	{0xCE,0x13},
	{0xCF,0x64},
	{0xD0,0x21},
	{0xD1,0x0F},
	{0xD2,0x88},
	{0xE0,0x01},
	{0xE1,0x04},
	{0xE2,0x41},
	{0xE3,0xD6},
	{0xE4,0x00},
	{0xE5,0x0C},
	{0xE6,0x0A},
	{0xE7,0x00},
	{0xE8,0x00},
	{0xE9,0x00},
	{0xEE,0x07},
	{0xEF,0x01},
	{0x00,0x1E},
	{0x01,0x1E},
	{0x02,0x0F},
	{0x03,0x10},
	{0x04,0x02},
	{0x05,0x00},
	{0x06,0xB0},
	{0x07,0x04},
	{0x08,0x0D},
	{0x09,0x0E},
	{0x0A,0x9C},
	{0x0B,0x04},
	{0x0C,0x05},
	{0x0D,0x0F},
	{0x0E,0x02},
	{0x0F,0x12},
	{0x10,0x02},
	{0x11,0x02},
	{0x12,0x00},
	{0x13,0x01},
	{0x14,0x05},
	{0x15,0x07},
	{0x16,0x05},
	{0x17,0x07},
	{0x18,0x01},
	{0x19,0x04},
	{0x1A,0x05},
	{0x1B,0x0C},
	{0x1C,0x2A},
	{0x1D,0x01},
	{0x1E,0x00},
	{0x21,0x00},
	{0x22,0x00},
	{0x23,0x00},
	{0x25,0x01},
	{0x26,0x00},
	{0x27,0x39},
	{0x28,0x7F},
	{0x29,0x08},
	{0x30,0x03},
	{0x31,0x00},
	{0x32,0x1A},
	{0x33,0x1A},
	{0x34,0x07},
	{0x35,0x07},
	{0x36,0x01},
	{0x37,0xFF},
	{0x38,0x36},
	{0x39,0x07},
	{0x3A,0x00},
	{0x3E,0xFF},
	{0x3F,0x00},
	{0x40,0x77},
	{0x41,0x40},
	{0x42,0x00},
	{0x43,0x30},
	{0x44,0xA0},
	{0x45,0x5C},
	{0x46,0x00},
	{0x47,0x00},
	{0x48,0x58},
	{0x4A,0x1E},
	{0x4B,0x1E},
	{0x4C,0x00},
	{0x4D,0x00},
	{0x4E,0xA0},
	{0x4F,0x80},
	{0x50,0x00},
	{0x51,0x00},
	{0x52,0x00},
	{0x53,0x00},
	{0x54,0x00},
	{0x57,0x80},
	{0x59,0x10},
	{0x5A,0x08},
	{0x5B,0x94},
	{0x5C,0xE8},
	{0x5D,0x08},
	{0x5E,0x3D},
	{0x5F,0x99},
	{0x60,0x45},
	{0x61,0x40},
	{0x63,0x2D},
	{0x64,0x02},
	{0x65,0x96},
	{0x66,0x00},
	{0x67,0x97},
	{0x68,0x01},
	{0x69,0xCD},
	{0x6A,0x01},
	{0x6B,0xB0},
	{0x6C,0x04},
	{0x6D,0x2C},
	{0x6E,0x01},
	{0x6F,0x32},
	{0x71,0x00},
	{0x72,0x01},
	{0x73,0x35},
	{0x74,0x00},
	{0x75,0x33},
	{0x76,0x31},
	{0x77,0x01},
	{0x7C,0x84},
	{0x7D,0x03},
	{0x7E,0x01},
};


void Master2_slvdev_init(uint32_t slv_addr)
{
	uint8_t sta;
					EMBARC_PRINTF("test int_int  1\n\r");
	iic_master_2 = iic_get_dev(IIC_MASTER_0);/* get IIC Master_2 device */
	// EMBARC_PRINTF("iic get = %d   ",sta);
					EMBARC_PRINTF("test int_int  2\n\r");
	// sta = iic_master_2->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);/* open IIC device in master mode */
					EMBARC_PRINTF("test int_int  3\n\r");
	EMBARC_PRINTF("iic open = %d   ",sta);
	sta = iic_master_2->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));/* Set target slave device address */
					EMBARC_PRINTF("test int_int  4\n\r");
	EMBARC_PRINTF("iic control = %d   ",sta);
}




/**************************************************************** 
 * Function Name: paj7620WriteReg
 * Description:  PAJ7620 Write reg cmd
 * Parameters: addr:reg address; cmd:function data
 * Return: error code; success: return 0
****************************************************************/ 
uint8_t paj7620WriteReg(uint8_t addr, uint8_t cmd)
{
//	char i = 1;
    uint8_t buf[2]; /* reg buffer */
	int32_t ercd;
//	Wire.beginTransmission(PAJ7620_ID);		// start transmission to device 
    iic_master_2->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	//write cmd
	buf[0] = addr;
	buf[1] = cmd;
	ercd = iic_master_2->iic_write(buf, 2);
	if(ercd == E_OBJ||ercd == E_PAR||ercd == E_CTX||ercd == E_SYS)
	{
		EMBARC_PRINTF("ERROR~~~`\n\r\n\n");
	}

}

/**************************************************************** 
 * Function Name: paj7620ReadReg
 * Description:  PAJ7620 read reg data
 * Parameters: addr:reg address;
 *			   qty:number of data to read, addr continuously increase;
 *			   data[]:storage memory start address
 * Return: error code; success: return 0
****************************************************************/ 
uint8_t paj7620ReadReg(uint8_t addr, uint8_t *data)
{
//	uint8_t error;
    uint8_t buf[1];
	uint8_t result[1];
	int32_t ercd=0;

//	EMBARC_PRINTF("paj7620ReadReg1 %x\n\r",addr);
	iic_master_2->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(paj7620ADDR));
	iic_master_2->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_RESTART));
//	buf[0] = 0x73;
//	iic_master_2->iic_write(buf, 1);
	buf[0] = addr;
	iic_master_2->iic_write(buf, 1); /* Master write IIC_slv addr to IIC wire */
	iic_master_2->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	ercd = iic_master_2->iic_read(result, 1); /* Read data from slvdev */
//	iic_master_2->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));
	*data = result[0];
	// EMBARC_PRINTF("Ercd = %d",ercd);
	return ercd;
}

/**************************************************************** 
 * Function Name: paj7620SelectBank
 * Description:  PAJ7620 select register bank
 * Parameters: BANK0, BANK1
 * Return: none
****************************************************************/ 
void paj7620SelectBank(bank_e bank)
{
    switch(bank){
		case BANK0:
			paj7620WriteReg(PAJ7620_REGITER_BANK_SEL, PAJ7620_BANK0);
			EMBARC_PRINTF("PAJ7620_BANK0Select\n\r");
			break;
		case BANK1:
			paj7620WriteReg(PAJ7620_REGITER_BANK_SEL, PAJ7620_BANK1);
			break;
		default:
			break;
	}
}

/**************************************************************** 
 * Function Name: paj7620Init
 * Description:  PAJ7620 REG INIT
 * Parameters: none
 * Return: error code; success: return 0
****************************************************************/ 
uint8_t paj7620Init(void) 
{
	//Near_normal_mode_V5_6.15mm_121017 for 940nm
	int i = 0;
//	uint8_t error;
	uint8_t data0 = 0, data1 = 0;
	uint8_t buff[1];
	buff[0] = 0x73 << 1;
	
				// EMBARC_PRINTF("test int  1\n\r");
	//wakeup the sensor

    // I2C initialization
	Master2_slvdev_init(paj7620ADDR);
	iic_change_address_paj();
				// EMBARC_PRINTF("test int  2\n\r");
	

	iic_write_paj(buff, 1);
	// iic_master_2->iic_write(buff, 1);
    delay_us(700);  //Wait 700us for PAJ7620U2 to stabilize	 
	
    
				// EMBARC_PRINTF("test int  3\n\r");
    EMBARC_PRINTF("INIT SENSOR...\n\r");

//	Wire.begin();
	
	paj7620SelectBank(BANK0);
	paj7620SelectBank(BANK0);
    
				// EMBARC_PRINTF("test int  4\n\r");
//	EMBARC_PRINTF("DATA0 = %d\n\r",data0);
//	data0 = paj7620ReadReg(0x00);
    paj7620ReadReg(0x00,&data0);
	EMBARC_PRINTF("data0 = %x\n\r",data0);
//	data0 = paj7620ReadReg(0x01);
    paj7620ReadReg(0x01,&data1);
				// EMBARC_PRINTF("test int  5\n\r");
	EMBARC_PRINTF("data1 = %x\n\r",data1);
	

	if ( (data0 != 0x20 ) || (data1 != 0x76) )
	{
		return 0xff;
	}
	if ( data0 == 0x20 )
	{
		EMBARC_PRINTF("wake-up finish.\n\r");
	}
	
				// EMBARC_PRINTF("test int  6\n\r");
	for (i = 0; i < INIT_REG_ARRAY_SIZE; i++) 
	{
		// EMBARC_PRINTF("%d : ",i+1);
		paj7620WriteReg(initRegisterArray[i][0], initRegisterArray[i][1]);
	}
	
//	EMBARC_PRINTF("Set up gaming mode.");
//    paj7620SelectBank(BANK1);  //gesture flage reg in Bank1
//    // paj7620WriteReg(0x65, 0xB7); // far mode 120 fps
//    paj7620WriteReg(0x65, 0x12);  // near mode 240 fps

	paj7620SelectBank(BANK0);  //gesture flage reg in Bank0
	
    EMBARC_PRINTF("Paj7620 initialize register finished.\n\r");
	return 0;
}


void paj7620_config(void)
{
	uint8_t error = 0;
    uint8_t result = 0, result1 = 0;
	int32_t ercd;
				// EMBARC_PRINTF("test 1\n\r");
	error = paj7620Init();			// initialize Paj7620 registers
	if (error) 
	{
		EMBARC_PRINTF("INIT ERROR,CODE:\n\r");
	}
	else
	{
		EMBARC_PRINTF("paj7620 INIT OK\n\r");
	}
				// EMBARC_PRINTF("test 2\n\r");
	EMBARC_PRINTF("Please input your gestures:\n\r");
	delay_ms(1000);
	ercd = paj7620ReadReg(0x43,&result);
				// EMBARC_PRINTF("test 3\n\r");
	if(ercd != 0)
	{
		EMBARC_PRINTF("ercd = %x\n\r",ercd);				// Read Bank_0_Reg_0x43/0x44 for gesture result.
		EMBARC_PRINTF("results = %x\n\r",&result);
	}
	else
	{
		EMBARC_PRINTF("ercd error!!!!\n\r");
	}
				// EMBARC_PRINTF("test 4\n\r");
}

uint8_t paj7620_detect(void)
{
	uint8_t result = 0, result1 = 0;
	delay_ms(GES_ENTRY_TIME);
	paj7620ReadReg(0x43,&result);
	switch (result) 						// When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
	{

		case GES_RIGHT_FLAG:
			EMBARC_PRINTF("results default = %x\n\r",result);
			EMBARC_PRINTF("Right\n\r");
			delay_ms(GES_QUIT_TIME);
			break;
		case GES_LEFT_FLAG: 
			EMBARC_PRINTF("results default = %x\n\r",result);
			EMBARC_PRINTF("Left\n\r");
			delay_ms(GES_QUIT_TIME);
			break;
		case GES_UP_FLAG:
			EMBARC_PRINTF("results default = %x\n\r",result);
			EMBARC_PRINTF("Up\n\r");
			delay_ms(GES_QUIT_TIME);
			break;
		case GES_DOWN_FLAG:
			EMBARC_PRINTF("results default = %x\n\r",result);
			EMBARC_PRINTF("Down\n\r");
			delay_ms(GES_QUIT_TIME);
			break;
		case GES_FORWARD_FLAG:
			EMBARC_PRINTF("results default = %x\n\r",result);
			EMBARC_PRINTF("Forward\n\r");
			delay_ms(GES_QUIT_TIME);
			break;
		case GES_BACKWARD_FLAG:
			EMBARC_PRINTF("results default = %x\n\r",result);		  
			EMBARC_PRINTF("Backward\n\r");
			delay_ms(GES_QUIT_TIME);
			break;
		case GES_CLOCKWISE_FLAG:
			EMBARC_PRINTF("results default = %x\n\r",result);
			EMBARC_PRINTF("Clockwise\n\r");
			break;
		case GES_COUNT_CLOCKWISE_FLAG:
			EMBARC_PRINTF("results default = %x\n\r",result);
			EMBARC_PRINTF("anti-clockwise\n\r");
			break;  
		default:
			delay_ms(GES_ENTRY_TIME);
//				result = paj7620ReadReg(0x43);
			paj7620ReadReg(0x43,&result);
			if (result != 0)
				EMBARC_PRINTF("results default = %x\n\r",result);
//				result1 = paj7620ReadReg(0x44);
			paj7620ReadReg(0x44,&result1);
			if (result1 == GES_WAVE_FLAG) 
			{
				EMBARC_PRINTF("wave\n\r");
			}
			break;
	}
	return result;
}