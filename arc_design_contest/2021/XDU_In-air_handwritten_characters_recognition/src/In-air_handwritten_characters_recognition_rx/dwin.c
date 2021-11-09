//*Use GB2312 encoding  !!!!!!!!!!!!!!!!!!!

#include "dwin.h"
#include "dfss_iic_obj.h"
#include "ds1302.h"
#include "dht11.h"
#include "24l01.h"
#include "audio_synth.h"
#include "paj7620.h"
// #include "global.h"

DEV_IIC * iic_master_1;

uint8_t dwin_cmd[MAX_DWIN_CMD_DATA_LEN] = {0};



    uint8_t write_data;
	uint8_t write_data_str[1];	

	/* Store characters in each line */
	uint8_t write_line_1[23]="L1:";
	uint8_t write_line_2[23]="L2:";
	uint8_t write_line_3[23]="L3:";
	uint8_t write_line_4[23]="L4:";
	uint8_t write_line_5[23]="L5:";
	uint8_t write_line_6[23]="L6:";
	uint8_t write_line_7[23]="L7:";
	uint8_t data_copy[23]="";

	uint8_t LINE = 3;			//Number of rows added

//-----------------------------------------------------------------------------------
//      OLED I2C config
//-----------------------------------------------------------------------------------
void Master1_slvdev_init(uint32_t slv_addr)
{
	iic_master_1 = iic_get_dev(IIC_MASTER_1);/* get IIC Master_0 device */
	
	iic_master_1->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD);/* open IIC device in master mode */

	iic_master_1->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(0x73));/* Set target slave device address */
	iic_master_1->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(slv_addr));/* Set target slave device address */
	EMBARC_PRINTF("dwin INITIAL  OK!\n\r");
}


/* Must use global variable to aviod cpu crash */
void dwin_write_cmd(uint16_t addr, uint8_t *data, uint32_t len)
{
	uint32_t i;

	iic_master_1->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_UART_ADDR));
	/* Set next condition for following transmit operation */
	iic_master_1->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));

	dwin_cmd[0] = 0x5a;		// head high
	dwin_cmd[1] = 0xa5;		// head low
	dwin_cmd[2] = len + 3;	// data length
	dwin_cmd[3] = 0x82;		// write: 0x82;  read: 0x83
	dwin_cmd[4] = (uint8_t)(addr >> 8);			// addr(high)
	dwin_cmd[5] = (uint8_t)(addr & 0x00ff);		// addr(low)

	for (i = 0; i < len; i++) {
		dwin_cmd[6+i] = data[i];
	}

	iic_master_1->iic_write(dwin_cmd, len+6);
}



uint8_t dwin_read_btn(uint16_t addr)
{
	uint32_t i;
	uint8_t  read_btn_cmd[10];

	uint8_t addr_h = (uint8_t)(addr >> 8);
	uint8_t addr_l = (uint8_t)(addr & 0x00ff);

	iic_master_1->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_UART_ADDR));
	/* Set next condition for following transmit operation */
	iic_master_1->iic_control(IIC_CMD_MST_SET_NEXT_COND, CONV2VOID(IIC_MODE_STOP));

	dwin_cmd[0] = 0x5a;					// head high
	dwin_cmd[1] = 0xa5;					// head low
	dwin_cmd[2] = 0x04;					// data length
	dwin_cmd[3] = 0x83;					// write: 0x82;  read: 0x83
	dwin_cmd[4] = addr_h;				// addr(high)
	dwin_cmd[5] = addr_l;				// addr(low)
	dwin_cmd[6] = 0x01;					// length of returned key value

	iic_master_1->iic_write(dwin_cmd, 7);


	uint8_t read_buf[16] = {0};

	iic_master_1->iic_read(read_buf, 8);

	while (!((read_buf[3] == addr_h) && (read_buf[4] == addr_l))) {
		iic_master_1->iic_write(dwin_cmd, 7);
		iic_master_1->iic_read(read_buf, 8);
	}


	dwin_cmd[2] = 0x05;							// data length
	dwin_cmd[3] = 0x82;							// write: 0x82;  read: 0x83
	dwin_cmd[6] = 0x00;							// length of returned key value
	dwin_cmd[7] = 0x00;							// length of returned key value

	iic_master_1->iic_write(dwin_cmd, 8);

	return read_buf[7];
}


void dwin_display(void)
{
    /* DS1302 parameter */
	uint8_t time_rcv[8];			// time data receive from ds1302
	uint8_t time_extr[14];			// extracted time data in bcd format
	uint8_t time_hex[TIME_HEX_LEN]; // extracted time data in hex format
	char *day_string;				// day of week string
	uint8_t sec_buf = 0;
	uint8_t sec_flag = 0;

    /* DHT11 paramete */
	uint8_t dht11_rcv[5]; 			// temp/humi data recived from dht11
	uint8_t dht11_hex[4];

	/* Text paramete */
	uint8_t     tmp_buf[33];	
	uint8_t     write_buf[20];	
	uint8_t     i; 
	int8_t      len_tmp_buf; 
	uint8_t		audio_str[2];
	uint8_t		detect_result;
	// WiFi receive data
	if(NRF24L01_RxPacket(tmp_buf)==0)					
	{		
		len_tmp_buf = strlen(tmp_buf);
		mode_record();		
					EMBARC_PRINTF("RECEIVE DATA = %s\r\n",tmp_buf);

		switch(LINE)
		{
			
			case 1  : 	if (strlen(write_line_1) <20)
						{
							strcat(write_line_1,tmp_buf);
							for (int i = strlen(write_line_1);i<20;i++)
								write_line_1[i+1] = '\0';
							// Write_line(1,write_line_1); 
							dwin_write_cmd(DWIN_LINE_1_ADDR,write_line_1, 20);
							audio_synth(tmp_buf);
							break;
						}
						else
						{
							Cannot_write();
							strcpy(write_line_2,Next_line(LINE,write_line_2,tmp_buf));
							dwin_write_cmd(DWIN_LINE_2_ADDR,write_line_2, 20);
							break;
						}
			case 2  : 	if (strlen(write_line_2) <20)
						{
							strcat(write_line_2,tmp_buf);
							for (int i = strlen(write_line_2);i<20;i++)
								write_line_2[i+1] = '\0';
							// Write_line(2,write_line_2); 
							dwin_write_cmd(DWIN_LINE_2_ADDR,write_line_2,20);
							audio_synth(tmp_buf);
							break;
						}
						else
						{
							Cannot_write();
							strcpy(write_line_3,Next_line(LINE,write_line_3,tmp_buf));
							dwin_write_cmd(DWIN_LINE_3_ADDR,write_line_3, 20);
							break;
						}
			case 3  : 	if (strlen(write_line_3) <20)
						{
							strcat(write_line_3,tmp_buf);
							for (int i = strlen(write_line_3);i<20;i++)
								write_line_3[i+1] = '\0';
							// Write_line(3,write_line_3); 
							dwin_write_cmd(DWIN_LINE_3_ADDR,write_line_3, 20);
							audio_synth(tmp_buf);
							break;
						}
						else
						{
							Cannot_write();
							strcpy(write_line_4,Next_line(LINE,write_line_4,tmp_buf));
							dwin_write_cmd(DWIN_LINE_4_ADDR,write_line_4, 20);
							break;
						}
			case 4  : 	if (strlen(write_line_4) <20)
						{
							strcat(write_line_4,tmp_buf);
							for (int i = strlen(write_line_4);i<20;i++)
								write_line_4[i+1] = '\0';
							// Write_line(4,write_line_4); 
							dwin_write_cmd(DWIN_LINE_4_ADDR,write_line_4, 20);
							audio_synth(tmp_buf);
							break;
						}
						else
						{
							Cannot_write();
							strcpy(write_line_5,Next_line(LINE,write_line_5,tmp_buf));
							dwin_write_cmd(DWIN_LINE_5_ADDR,write_line_5, 20);
							break;
						}
			case 5  : 	if (strlen(write_line_5) <20)
						{
							strcat(write_line_5,tmp_buf);
							for (int i = strlen(write_line_5);i<20;i++)
								write_line_5[i+1] = '\0';
							// Write_line(5,write_line_5); 
							dwin_write_cmd(DWIN_LINE_5_ADDR,write_line_5, 20);
							audio_synth(tmp_buf);
							break;
						}
						else
						{
							Cannot_write();
							strcpy(write_line_6,Next_line(LINE,write_line_6,tmp_buf));
							dwin_write_cmd(DWIN_LINE_6_ADDR,write_line_6, 20);
							break;
						}
			case 6  : 	if (strlen(write_line_6) <20)
						{
							strcat(write_line_6,tmp_buf);
							for (int i = strlen(write_line_6);i<20;i++)
								write_line_6[i+1] = '\0';
							// Write_line(6,write_line_6); 
							dwin_write_cmd(DWIN_LINE_6_ADDR,write_line_6, 20);
							audio_synth(tmp_buf);
							break;
						}
						else
						{
							Cannot_write();
							strcpy(write_line_7,Next_line(LINE,write_line_7,tmp_buf));
							dwin_write_cmd(DWIN_LINE_7_ADDR,write_line_7, 20);
							break;
						}
			case 7  : 	if (strlen(write_line_7) <20)
						{
							strcat(write_line_7,tmp_buf);
							for (int i = strlen(write_line_7);i<20;i++)
								write_line_7[i+1] = '\0';
							// Write_line(7,write_line_7); 
							dwin_write_cmd(DWIN_LINE_7_ADDR,write_line_7, 20);
							audio_synth(tmp_buf);
							break;
						}
						else
						{
							Cannot_write();
							strcpy(write_line_1,Next_line(LINE,write_line_1,tmp_buf));					
							dwin_write_cmd(DWIN_LINE_1_ADDR,write_line_1, 20);
							break;
						}
			default : 	if (strlen(write_line_1) <20)
						{
							strcat(write_line_1,tmp_buf);
							for (int i = strlen(write_line_1);i<20;i++)
								write_line_1[i+1] = '\0';
							// Write_line(1,write_line_1);
							dwin_write_cmd(DWIN_LINE_1_ADDR,write_line_1, 20); 
							audio_synth(tmp_buf);
							break;
						}
						else
						{
							Cannot_write();
							strcpy(write_line_2,Next_line(LINE,write_line_2,tmp_buf));
							dwin_write_cmd(DWIN_LINE_2_ADDR,write_line_2, 20);
							break;
						}
		}
			
	}
	else 
	{
		// EMBARC_PRINTF("RF24L01 DON'T RECEIVE DATA !\r\n");
	}		    

	/* Detect the gesture */
	detect_result = paj7620_detect();

	if (detect_result == ERASE_DATA)		// Erase characters
	{
		EMBARC_PRINTF("line = %d",LINE);
		switch(LINE)
		{
			case  1 : if(strlen(write_line_1) > 3) {write_line_1[strlen(write_line_1)-1] = '\0';dwin_write_cmd(DWIN_LINE_1_ADDR,write_line_1, 20);audio_synth("擦除");mode_erase_data(); break;}	else{Cannot_erase();break;}
			case  2 : if(strlen(write_line_2) > 3) {write_line_2[strlen(write_line_2)-1] = '\0';dwin_write_cmd(DWIN_LINE_2_ADDR,write_line_2, 20);audio_synth("擦除");mode_erase_data(); break;}	else{Cannot_erase();break;}
			case  3 : if(strlen(write_line_3) > 3) {write_line_3[strlen(write_line_3)-1] = '\0';dwin_write_cmd(DWIN_LINE_3_ADDR,write_line_3, 20);audio_synth("擦除");mode_erase_data(); break;}	else{Cannot_erase();break;}
			case  4 : if(strlen(write_line_4) > 3) {write_line_4[strlen(write_line_4)-1] = '\0';dwin_write_cmd(DWIN_LINE_4_ADDR,write_line_4, 20);audio_synth("擦除");mode_erase_data(); break;}	else{Cannot_erase();break;}
			case  5 : if(strlen(write_line_5) > 3) {write_line_5[strlen(write_line_5)-1] = '\0';dwin_write_cmd(DWIN_LINE_5_ADDR,write_line_5, 20);audio_synth("擦除");mode_erase_data(); break;}	else{Cannot_erase();break;}
			case  6 : if(strlen(write_line_6) > 3) {write_line_6[strlen(write_line_6)-1] = '\0';dwin_write_cmd(DWIN_LINE_6_ADDR,write_line_6, 20);audio_synth("擦除");mode_erase_data(); break;}	else{Cannot_erase();break;}
			case  7 : if(strlen(write_line_7) > 3) {write_line_7[strlen(write_line_7)-1] = '\0';dwin_write_cmd(DWIN_LINE_7_ADDR,write_line_7, 20);audio_synth("擦除");mode_erase_data(); break;}	else{Cannot_erase();break;}
			default : if(strlen(write_line_1) > 3) {write_line_1[strlen(write_line_1)-1] = '\0';dwin_write_cmd(DWIN_LINE_1_ADDR,write_line_1, 20);audio_synth("擦除");mode_erase_data(); break;}	else{Cannot_erase();break;}
		}
		
	}
	else if (detect_result == ERASE_LINE)	// Clear this bank
	{
		EMBARC_PRINTF("line = %d\r\n",LINE);
		EMBARC_PRINTF("long = %d\r\n",strlen(write_line_1));
		int length = 3;
		switch(LINE)
		{
			case  1 : if(strlen(write_line_1) > 3) { while(length <20){write_line_1[length] = '\0';length++;}dwin_write_cmd(DWIN_LINE_1_ADDR,write_line_1, 20); audio_synth("清除本行");mode_erase_line();break;}		else{Cannot_erase();break;} 
	 	 	case  2 : if(strlen(write_line_2) > 3) { while(length <20){write_line_2[length] = '\0';length++;}dwin_write_cmd(DWIN_LINE_2_ADDR,write_line_2, 20); audio_synth("清除本行");mode_erase_line();break;}		else{Cannot_erase();break;} 
	 	 	case  3 : if(strlen(write_line_3) > 3) { while(length <20){write_line_3[length] = '\0';length++;}dwin_write_cmd(DWIN_LINE_3_ADDR,write_line_3, 20); audio_synth("清除本行");mode_erase_line();break;}		else{Cannot_erase();break;} 
	 	 	case  4 : if(strlen(write_line_4) > 3) { while(length <20){write_line_4[length] = '\0';length++;}dwin_write_cmd(DWIN_LINE_4_ADDR,write_line_4, 20); audio_synth("清除本行");mode_erase_line();break;}		else{Cannot_erase();break;} 
	 	 	case  5 : if(strlen(write_line_5) > 3) { while(length <20){write_line_5[length] = '\0';length++;}dwin_write_cmd(DWIN_LINE_5_ADDR,write_line_5, 20); audio_synth("清除本行");mode_erase_line();break;}		else{Cannot_erase();break;} 
	 	 	case  6 : if(strlen(write_line_6) > 3) { while(length <20){write_line_6[length] = '\0';length++;}dwin_write_cmd(DWIN_LINE_6_ADDR,write_line_6, 20); audio_synth("清除本行");mode_erase_line();break;}		else{Cannot_erase();break;} 
	 	 	case  7 : if(strlen(write_line_7) > 3) { while(length <20){write_line_7[length] = '\0';length++;}dwin_write_cmd(DWIN_LINE_7_ADDR,write_line_7, 20); audio_synth("清除本行");mode_erase_line();break;}		else{Cannot_erase();break;} 
	 	 	default : if(strlen(write_line_1) > 3) { while(length <20){write_line_1[length] = '\0';length++;}dwin_write_cmd(DWIN_LINE_1_ADDR,write_line_1, 20); audio_synth("清除本行");mode_erase_line();break;}		else{Cannot_erase();break;}
		}
		EMBARC_PRINTF("string = %s",write_line_1);
		
		
	}
	else if (detect_result == UP_LINE)			// previous row
	{
		EMBARC_PRINTF("line = %d",LINE);
		audio_synth("上一杭");					// The speech synthesis module does not recognize polyphonic characters = = ！
		mode_up();								//*Use GB2312 encoding  previous row
		if(LINE == 1)
			LINE = 7;
		else	
			LINE--;

		for(int i=1; i<8; i++)
		{
			if (i  == LINE)
			{
				dwin_write_cmd(0x2100 + i*16," *", 20);
			}						
			else
				dwin_write_cmd(0x2100 + i*16," ", 20);	
		}
	}
	else if (detect_result == DOWN_LINE)		// Next line
	{
		EMBARC_PRINTF("line = %d",LINE);
		audio_synth("下一杭");//*Use GB2312 encoding  Next line
		mode_down();
		if(LINE == 7)
			LINE = 1;
		else	
			LINE++;
		for(int i=1; i<8; i++)
		{
			if (i  == LINE)
				dwin_write_cmd(0x2100 + i*16," *", 20);
			else
				dwin_write_cmd(0x2100 + i*16," ", 20);	
		}
	}
	else if (detect_result == COPY_LINE)		// Copy this line
	{
		EMBARC_PRINTF("line = %d",LINE);
		audio_synth("复制本杭");//*Use GB2312 encoding   Copy this line
		switch(LINE)
		{
			case  1 : /*strcpy(data_copy,write_line_1);*/for(int i =3;i<strlen(write_line_1);i++)data_copy[i-3] = write_line_1[i];mode_copy();break; 
			case  2 : /*strcpy(data_copy,write_line_2);*/for(int i =3;i<strlen(write_line_2);i++)data_copy[i-3] = write_line_2[i];mode_copy();break; 
			case  3 : /*strcpy(data_copy,write_line_3);*/for(int i =3;i<strlen(write_line_3);i++)data_copy[i-3] = write_line_3[i];mode_copy();break; 
			case  4 : /*strcpy(data_copy,write_line_4);*/for(int i =3;i<strlen(write_line_4);i++)data_copy[i-3] = write_line_4[i];mode_copy();break; 
			case  5 : /*strcpy(data_copy,write_line_5);*/for(int i =3;i<strlen(write_line_5);i++)data_copy[i-3] = write_line_5[i];mode_copy();break; 
			case  6 : /*strcpy(data_copy,write_line_6);*/for(int i =3;i<strlen(write_line_6);i++)data_copy[i-3] = write_line_6[i];mode_copy();break; 
			case  7 : /*strcpy(data_copy,write_line_7);*/for(int i =3;i<strlen(write_line_7);i++)data_copy[i-3] = write_line_7[i];mode_copy();break; 
			default : /*strcpy(data_copy,write_line_1);*/for(int i =3;i<strlen(write_line_1);i++)data_copy[i-3] = write_line_1[i];mode_copy();break; 
		}		
		delay_ms(1000);
	}
	else if (detect_result == PASTE_LINE)		// Paste our bank
	{
		EMBARC_PRINTF("line = %d",LINE); 
		audio_synth("黏贴本杭");//*Use GB2312 encoding   Paste our bank
		switch(LINE)   
		{
			case  1 : strcat(write_line_1,data_copy);dwin_write_cmd(DWIN_LINE_1_ADDR,write_line_1, 20);mode_paste();break; 
			case  2 : strcat(write_line_2,data_copy);dwin_write_cmd(DWIN_LINE_2_ADDR,write_line_2, 20);mode_paste();break; 
			case  3 : strcat(write_line_3,data_copy);dwin_write_cmd(DWIN_LINE_3_ADDR,write_line_3, 20);mode_paste();break; 
			case  4 : strcat(write_line_4,data_copy);dwin_write_cmd(DWIN_LINE_4_ADDR,write_line_4, 20);mode_paste();break; 
			case  5 : strcat(write_line_5,data_copy);dwin_write_cmd(DWIN_LINE_5_ADDR,write_line_5, 20);mode_paste();break; 
			case  6 : strcat(write_line_6,data_copy);dwin_write_cmd(DWIN_LINE_6_ADDR,write_line_6, 20);mode_paste();break; 
			case  7 : strcat(write_line_7,data_copy);dwin_write_cmd(DWIN_LINE_7_ADDR,write_line_7, 20);mode_paste();break; 
			default : strcat(write_line_1,data_copy);dwin_write_cmd(DWIN_LINE_1_ADDR,write_line_1, 20);mode_paste();break; 
		}	
		delay_ms(500);	
	}



	/* Display the time */
	rtc_read_time(time_rcv);
	time_rcv[0] = 0x20;
	rtc_time_extract(time_rcv, time_extr);
	rtc_time_extract_hex(time_rcv, time_hex);	

	if (time_extr[13] != sec_buf)
	{
		sec_flag = 0;
		sec_buf = time_extr[13];

		dwin_write_cmd(DWIN_RTC_ADDR, time_hex, TIME_HEX_LEN);
	}

	/* Display the temprature */
	dht11_read_data(dht11_rcv, dht11_hex);
	dwin_write_cmd(DWIN_TEMP_ADDR, &dht11_hex[2], 2);
	// dwin_write_cmd(DWIN_HUMI_ADDR, &dht11_hex[0], 2);
	// AUDIO_SYNTH("你好");//*Use GB2312 encoding

	
    
}

void mode_erase_data(void)
{
	const uint8_t test[8] = {0x64, 0xE6,0x96,0x64,0x00,0x20,0x00,0x20};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}
void mode_erase_line(void)
{
	const uint8_t test[8] = {0x6E, 0x05,0x96,0x64,0x67,0x2C,0x88,0x4C};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}
void mode_number(void)
{
	const uint8_t test[8] = {0x65, 0x70,0x5B,0x57,0x6A,0x21,0x5F,0x0F};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_alpha(void)
{
	const uint8_t test[8] = {0x5B, 0x57,0x6B,0xCD,0x6A,0x21,0x5F,0x0F};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_down(void)
{
	const uint8_t test[8] = {0x4E, 0x0B,0x4E,0x00,0x88,0x4C,0x00,0x20};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_up(void)
{
	const uint8_t test[8] = {0x4E, 0x0A,0x4E,0x00,0x88,0x4C,0x00,0x20};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_full(void)
{
	const uint8_t test[8] = {0x6B,0x64, 0x88,0x4C,0x5D,0xF2,0x6E,0xE1};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_empty(void)
{
	const uint8_t test[8] = {0x6B,0x64, 0x88,0x4C,0x5D,0xF2,0x7A,0x7A};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_record(void)
{
	const uint8_t test[8] = {0x8B,0xB0,0x5F,0x55,0x4E,0x2D,0x00,0x20};	
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_init(void)
{
	const uint8_t test[8] = {0x8B,0xF7,0x4E,0x66,0x51,0x99,0x00,0x20};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_copy(void)
{
	const uint8_t test[8] = {0x59,0x0D,0x52,0x36,0x67,0x2C,0x88,0x4C};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_paste(void)
{
	const uint8_t test[8] = {0x9E,0xCF,0x8D,0x34,0x67,0x2C,0x88,0x4C};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}

void mode_next_line(void)
{
	const uint8_t test[8] = {0x79,0xFB,0x81,0xF3,0x4E,0x0B,0x88,0x4C};
	dwin_write_cmd(DWIN_MODE_ADDR, (uint8_t *)test, 8);
}
void dwin_display_init(void)
{
	uint8_t str [4];
	for(int i=1; i<8; i++)
	{
		if (i == 1)
			dwin_write_cmd(DWIN_LINE_POINT_1_ADDR + (i-1)*16," *", 20);
		else
			dwin_write_cmd(DWIN_LINE_POINT_1_ADDR + (i-1)*16," ", 20);	

		sprintf(str,"L%d:",i);
		str[3] = '\0';
		printf("str = %s\r\n",str);
		dwin_write_cmd(DWIN_LINE_1_ADDR + (i-1)*16,str, 20);		
		// mode_init();
		mode_record();
	}
	mode_init();


}
void iic_change_address_paj(void)
{
	iic_master_1->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(0x73));
}

void iic_write_paj(const void *data, uint32_t len)
{
	iic_master_1->iic_write(data, len);
}
void Cannot_write(void)
{
	audio_synth("此杭已满移至下杭");//*Use GB2312 encoding   This line is full and move down
	mode_next_line();
}

void Cannot_erase(void)
{
	audio_synth("此杭已空");//*Use GB2312 encoding   This line is empty
	mode_empty();
}


uint8_t * Next_line(uint8_t line,uint8_t *write_line_next,uint8_t *tmp_buf)
{
	if(LINE == 7)
		LINE = 1;
	else	
		LINE++;
	for(int i=1; i<8; i++)
	{
		if (i  == LINE)
			dwin_write_cmd(0x2100 + i*16," *", 20);
		else
			dwin_write_cmd(0x2100 + i*16," ", 20);	
	}
	strcat(write_line_next,tmp_buf);
	dwin_write_cmd(DWIN_LINE_1_ADDR + (LINE-1)*16,write_line_next, 20);
	return write_line_next;
}

