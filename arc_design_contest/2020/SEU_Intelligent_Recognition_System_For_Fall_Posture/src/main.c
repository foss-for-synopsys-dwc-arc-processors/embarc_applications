#include "embARC.h"
#include "embARC_debug.h"
#include "mpu9250.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "NNlayer.h"
#include "train_txt_name.h"
#include "uart.h"

static DEV_GPIO *port;
static DEV_GPIO *port2;
static DEV_GPIO_BIT_ISR bit_isr;
static DEV_GPIO_INT_CFG int_cfg;
static DEV_UART *port_uart;
static DEV_UART *port2_uart;

void gpio_key_init();
void gpio_buzzer_init();
void uart_sim900a_init();
void uart_ble_init();
void sim900a_send();

void savedata(void);
void transfer_data(void);
double max_outval(double a,double b);
double sqrtii(double i);
void judge_gesture();
void clean_variable();

void key_isr(void *ptr);
void Key_Scan(void);
void Key_Process(void);

void sim900a_send_AT_copying();
void sim900a_message_copying();

void buzzer_ring();

void weight_download();

void interval_target_init();

void weight_train();

void target_jud_setting();

void converse_target_jud();

void physical_parameter_send();

void write_file();

int k=1;
double max=0;
#define N 25
uint32_t key_state=0;
uint32_t baudrate = 115200;
uint32_t flag_download_weight=0;
uint32_t flag_buzzer=0;
uint32_t flag_back_propagation=0;
uint32_t flag_upload_weight=0;
uint32_t flag_physical_parameter_send=0;
uint32_t counter=0;
uint32_t transfer_index=0;

uint32_t  KeyValue=0;
uint32_t  Value=0;
uint32_t  file_counter=28;

extern double W0[LAYER_0_OUTDIM][LAYER_0_INDIM];
extern double W1[LAYER_1_OUTDIM][LAYER_1_INDIM];
extern double B0[LAYER_0_OUTDIM];
extern double B1[LAYER_1_OUTDIM];

/*  AT_instruction  */
char in6[50]="AT\r\n";
char in7[50]="AT+CMGF=1\r\n";
char in8[50]="AT+CSMP=17,167,2,25\r\n";
char in9[50]="AT+CSCS=\"UCS2\"\r\n";
char in10[80]="AT+CMGS=\"00310037003300360031003800370037003600340038\"\r\n";
char in11[50]="80014EBA";
char in12[100];

char in13[80]="AT+CIPSTART=\"TCP\",\"33v679c292.wicp.vip\",\"23578\"\r\n";
char in14[50]="AT+CIPSEND\r\n";
char in15[2000];
char in16[2000];
char in17[2000];
char in18[2000];
char in19[2000];
char in20[2000];
char in21[2000];
char in22='s';
char in23[50]="user is applying for weight downloading W0";
char in24[50];
char in25[15000];
char delim[2]=" ";
char *token;
char in26[50]="user is applying for weight downloading W1";
char in27[50]="user is applying for bias downloading B0";
char in28[50]="user is applying for bias downloading B1";
char in29[50]="downloading W0 has finished";
char in30[50]="downloading W1 has finished";
char in31[50]="downloading B0 has finished";
char in32[50]="downloading B1 has finished";

NNLayer gMyCalculatorModel[LAYER_TOTAL];
double gLayer0OutVal[LAYER_0_OUTDIM],gLayer1OutVal[LAYER_1_OUTDIM];
double inputVal_jud[LAYER_0_INDIM];
double temp1[2],temp2[2],temp3[2],temp4[2],temp5[2],temp6[2],temp7[2],temp8[2],temp9[2],temp10[2];

#if defined(BOARD_IOTDK) || defined(BOARD_EMSDP)
#define MPU9250_IIC_ID DFSS_IIC_0_ID
#else
#define MPU9250_IIC_ID 0
#endif
static MPU9250_DEFINE(mpu9250_sensor, MPU9250_IIC_ID, MPU9250_IIC_ADDRESS);
MPU9250_DATA mpu9250_data = { 0 };

FILE *fp;
char *filename;
FILE *write_fp;
char *write_filename;

float  a, b;
int i = 0, j = 0, count_1 = 0,name_index = 0;
int epoch = 0;
double train_loss = 0.0;
double inputVal_train[50][LAYER_0_INDIM] = { 0.0 };
double target[50][LAYER_1_OUTDIM] = { 0.0 };
double target_judgement[LAYER_1_OUTDIM]={ 0.0 };
double dp_w1[LAYER_1_OUTDIM][LAYER_1_INDIM] = { 0.0 }, dp_b1[LAYER_1_OUTDIM] = {0.0};
double dp_w0[LAYER_0_OUTDIM][LAYER_0_INDIM] = { 0.0 },dp_b0[LAYER_0_OUTDIM] = { 0.0 };

int main(void)
{
	key_state=~key_state;

	gpio_key_init();
	gpio_buzzer_init();
	uart_sim900a_init();
	uart_ble_init();
	interval_target_init();
	mpu9250_sensor_init(mpu9250_sensor);

	sim900a_send_AT_copying();
	sim900a_message_copying();

	board_delay_ms(20000, 1);

	while(1)
	{
		if(flag_download_weight==1)
		{
			weight_download();
			flag_download_weight=0;
		}
		if(counter>5)
		{
			buzzer_ring();
			counter++;
		}
		if(flag_upload_weight==1)
		{
			sim900a_send();
			flag_upload_weight=0;
		}
		if(flag_physical_parameter_send==1)
		{
			physical_parameter_send();
			flag_physical_parameter_send=0;
		}
		mpu9250_sensor_read(mpu9250_sensor, &mpu9250_data);
		char datap[20];
		sprintf(datap, "%.2f  %.2f\r\n ", mpu9250_data.pitch,mpu9250_data.roll);
		board_delay_ms(100, 1);
		port2_uart->uart_write(datap, 20);

		savedata();
		transfer_data();

		NNLayerInit();

		/*     gesture judgement    */
		NNLayerPredict(gMyCalculatorModel,inputVal_jud);
		max=max_outval(gMyCalculatorModel[1].outVal[0],gMyCalculatorModel[1].outVal[1]);
		target_jud_setting();
		judge_gesture();

	   /*wait for the flag of back propagation*/
		if(flag_back_propagation==1)
		{
			file_counter++;
			converse_target_jud();
			write_file();
			weight_train();
			flag_back_propagation=0;
		}

		clean_variable();
	}

	return E_SYS;
}

void key_isr(void *ptr)
{
	Key_Scan();
	Key_Process();
	port->gpio_write(0x00,0xFF);
}

void Key_Scan(void)
{
   uint32_t KeyValue1;//KeyValue1:judge whether key pression
   uint32_t KeyValue2;//KeyValue2:judge which key was pressed

   port->gpio_write(0xF0,0xFF);
   KeyValue1 = _arc_aux_read(0x80017a50);
  if(KeyValue1!=0xF0)
  {
	board_delay_ms(10, 1); //debounce
	KeyValue1 = _arc_aux_read(0x80017a50);
	if(KeyValue1!=0xF0)
	{                     //Start the scan column by column, starting with the first column
	  port->gpio_write(0xF7,0xFF);    //first column
	  KeyValue2 = _arc_aux_read(0x80017a50);
	  switch (KeyValue2)
	   {
		case 0xe7:{KeyValue=13;break;}
		case 0xd7:{KeyValue=9;break;}
		case 0xb7:{KeyValue=5;break;}
		case 0x77:{KeyValue=1;break;}
	   }

	  port->gpio_write(0xFB,0xFF);   //second column
	  KeyValue2 = _arc_aux_read(0x80017a50);
		switch (KeyValue2)
		{
		 case 0xeb:{KeyValue=14;break;}
		 case 0xdb:{KeyValue=10;break;}
		 case 0xbb:{KeyValue=6;break;}
		 case 0x7b:{KeyValue=2;break;}
		}

		port->gpio_write(0xFD,0xFF); //third column
		KeyValue2 = _arc_aux_read(0x80017a50);
	   switch (KeyValue2)
		 {
		 case 0xed:KeyValue=15;break;
		 case 0xdd:KeyValue=11;break;
		 case 0xbd:KeyValue=7;break;
		 case 0x7d:KeyValue=3;break;
		 }

	   port->gpio_write(0xFE,0xFF);   //fourth column
	   KeyValue2 = _arc_aux_read(0x80017a50);
	  switch (KeyValue2)
	  {
		case 0xee:KeyValue=16;break;
		case 0xde:KeyValue=12;break;
		case 0xbe:KeyValue=8;break;
		case 0x7e:KeyValue=4;break;
	  }
	 }
   }
   else
   {
	 KeyValue=0;
   }
}

void Key_Process(void)
{
  switch(KeyValue)
  {
  case 1:
  {
	Value=Value*10+1;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 2:
  {
	Value=Value*10+2;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 3:
  {
	Value=Value*10+3;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 4:
  {
	port2->gpio_write(0x0F,0x0F);     //key4: stop ring of buzzer
	counter=0;
	break;
  }
  case 5:
  {
	Value=Value*10+4;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 6:
  {
	Value=Value*10+5;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 7:
  {
	Value=Value*10+6;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 8:
  {
	flag_back_propagation=1;           //key8:Back_propagation key
	break;
  }
  case 9:
  {
	Value=Value*10+7;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 10:
  {
	Value=Value*10+8;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 11:
  {
	Value=Value*10+9;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 12:
  {
	flag_download_weight=1;           //key12:download weight
	break;
  }
  case 13:
  {
	flag_upload_weight=1;             //key13:upload weight
	break;
  }
  case 14:
  {
	Value=Value*10+0;
	if(Value > 256)
	{ Value = 0x00;}
	break;
  }
  case 15:
  {
	break;
  }
  case 16:
  {
	flag_physical_parameter_send=1;   //key16:send physical parameter
	break;
  }

  }
  KeyValue = 0;
}

void savedata(void)
{
	if(k<=10)
	{
		switch(k)
		{
			case 1:
			{
				temp1[0]= mpu9250_data.pitch;
				temp1[1]= mpu9250_data.roll;
				break;
			}
			case 2:
			{
				temp2[0]= mpu9250_data.pitch;
				temp2[1]= mpu9250_data.roll;
				break;
			}
			case 3:
			{
				temp3[0]= mpu9250_data.pitch;
				temp3[1]= mpu9250_data.roll;
				break;
			}
			case 4:
			{
				temp4[0]= mpu9250_data.pitch;
				temp4[1]= mpu9250_data.roll;
				break;
			}
			case 5:
			{
				temp5[0]= mpu9250_data.pitch;
				temp5[1]= mpu9250_data.roll;
				break;
			}
			case 6:
			{
				temp6[0]= mpu9250_data.pitch;
				temp6[1]= mpu9250_data.roll;
				break;
			}
			case 7:
			{
				temp7[0]= mpu9250_data.pitch;
				temp7[1]= mpu9250_data.roll;
				break;
			}
			case 8:
			{
				temp8[0]= mpu9250_data.pitch;
				temp8[1]= mpu9250_data.roll;
				break;
			}
			case 9:
			{
				temp9[0]= mpu9250_data.pitch;
				temp9[1]= mpu9250_data.roll;
				break;
			}
			case 10:
			{
				temp10[0]= mpu9250_data.pitch;
				temp10[1]= mpu9250_data.roll;
				break;
			}
		}
	}
	else
	{
		temp1[0]=temp2[0];
		temp1[1]=temp2[1];
		temp2[0]=temp3[0];
		temp2[1]=temp3[1];
		temp3[0]=temp4[0];
		temp3[1]=temp4[1];
		temp4[0]=temp5[0];
		temp4[1]=temp5[1];
		temp5[0]=temp6[0];
		temp5[1]=temp6[1];
		temp6[0]=temp7[0];
		temp6[1]=temp7[1];
		temp7[0]=temp8[0];
		temp7[1]=temp8[1];
		temp8[0]=temp9[0];
		temp8[1]=temp9[1];
		temp9[0]=temp10[0];
		temp9[1]=temp10[1];
		temp10[0]= mpu9250_data.pitch;
		temp10[1]= mpu9250_data.roll;
	}
}

void transfer_data()
{
	inputVal_jud[0]=temp1[0];
	inputVal_jud[1]=temp1[1];
	inputVal_jud[2]=temp2[0];
	inputVal_jud[3]=temp2[1];
	inputVal_jud[4]=temp3[0];
	inputVal_jud[5]=temp3[1];
	inputVal_jud[6]=temp4[0];
	inputVal_jud[7]=temp4[1];
	inputVal_jud[8]=temp5[0];
	inputVal_jud[9]=temp5[1];
	inputVal_jud[10]=temp6[0];
	inputVal_jud[11]=temp6[1];
	inputVal_jud[12]=temp7[0];
	inputVal_jud[13]=temp7[1];
	inputVal_jud[14]=temp8[0];
	inputVal_jud[15]=temp8[1];
	inputVal_jud[16]=temp9[0];
	inputVal_jud[17]=temp9[1];
	inputVal_jud[18]=temp10[0];
	inputVal_jud[19]=temp10[1];
}

double max_outval(double a,double b)
{
	double max=a;
	if(b>max)
		max=b;
	return max;
}

double sqrtii(double i)
{
	double re;
	if(i<0)
		re=-i;
	else
		re=i;
	return re;
}

void judge_gesture(void)
{
	if(k>10)
	{
		if(counter<=5)
		{
		   if(sqrtii(inputVal_jud[0])>N||sqrtii(inputVal_jud[1])>N||sqrtii(inputVal_jud[2])>N||sqrtii(inputVal_jud[3])>N||sqrtii(inputVal_jud[4])>N||sqrtii(inputVal_jud[5])>N||sqrtii(inputVal_jud[6])>N||sqrtii(inputVal_jud[7])>N||sqrtii(inputVal_jud[8])>N||sqrtii(inputVal_jud[9])>N||sqrtii(inputVal_jud[10])>N||sqrtii(inputVal_jud[11])>N||sqrtii(inputVal_jud[12])>N||sqrtii(inputVal_jud[13])>N||sqrtii(inputVal_jud[14])>N||sqrtii(inputVal_jud[15])>N||sqrtii(inputVal_jud[16])>N||sqrtii(inputVal_jud[17])>N||sqrtii(inputVal_jud[18])>N||sqrtii(inputVal_jud[19])>N)
		   {
			   if(max==gMyCalculatorModel[1].outVal[1])
			   {
				   buzzer_ring();
				   counter++;
			   }
			   else counter=0;
		   }
		}
		if(counter==20)
		{
			port_uart->uart_write(in10, 80);
			board_delay_ms(100, 1);
			port_uart->uart_write(in12, 100);
			board_delay_ms(100, 1);
			counter=0;
		}
	}
}

void gpio_key_init()
{
	port = gpio_get_dev(DFSS_GPIO_8B2_ID);
	port->gpio_open(0x0F);
	port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x0F);

	int_cfg.int_bit_mask = 1<<4;
	int_cfg.int_bit_type = GPIO_INT_BIT_EDGE_TRIG(4);  //edge trigger
	int_cfg.int_bit_polarity = GPIO_INT_BIT_POL_FALL_EDGE(4);  //falling sensitive
	int_cfg.int_bit_debounce = GPIO_INT_BIT_ENA_DEBOUNCE(4);  //with debounce
	port->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);

	bit_isr.int_bit_ofs = 4;
	bit_isr.int_bit_handler = key_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	port->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)(1<<4));

	int_cfg.int_bit_mask = 1<<5;
	int_cfg.int_bit_type = GPIO_INT_BIT_EDGE_TRIG(5);
	int_cfg.int_bit_polarity = GPIO_INT_BIT_POL_FALL_EDGE(5);
	int_cfg.int_bit_debounce = GPIO_INT_BIT_ENA_DEBOUNCE(5);
	port->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);

	bit_isr.int_bit_ofs = 5;
	bit_isr.int_bit_handler = key_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	port->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)(1<<5));

	int_cfg.int_bit_mask = 1<<6;
	int_cfg.int_bit_type = GPIO_INT_BIT_EDGE_TRIG(6);
	int_cfg.int_bit_polarity = GPIO_INT_BIT_POL_FALL_EDGE(6);
	int_cfg.int_bit_debounce = GPIO_INT_BIT_ENA_DEBOUNCE(6);
	port->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);

	bit_isr.int_bit_ofs = 6;
	bit_isr.int_bit_handler = key_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	port->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)(1<<6));

	int_cfg.int_bit_mask = 1<<7;
	int_cfg.int_bit_type = GPIO_INT_BIT_EDGE_TRIG(7);
	int_cfg.int_bit_polarity = GPIO_INT_BIT_POL_FALL_EDGE(7);
	int_cfg.int_bit_debounce = GPIO_INT_BIT_ENA_DEBOUNCE(7);
	port->gpio_control(GPIO_CMD_SET_BIT_INT_CFG, &int_cfg);

	bit_isr.int_bit_ofs = 7;
	bit_isr.int_bit_handler = key_isr;
	port->gpio_control(GPIO_CMD_SET_BIT_ISR, &bit_isr);

	port->gpio_control(GPIO_CMD_ENA_BIT_INT, (void *)(1<<7));

	port->gpio_write(0xF0,0xFF);
}

void gpio_buzzer_init()
{
	port2 = gpio_get_dev(DFSS_GPIO_4B1_ID);
	port2->gpio_open(0x0F);
	port2->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x0F);
	port2->gpio_write(key_state,0x0F);
}

void uart_sim900a_init()
{
	port_uart = uart_get_dev(DFSS_UART_1_ID);
	port_uart->uart_open(baudrate);
	port_uart->uart_control(UART_CMD_SET_BAUD, (void *)(baudrate));
}

void uart_ble_init()
{
	io_arduino_config_uart(IO_PINMUX_ENABLE);
	port2_uart = uart_get_dev(2);
	port2_uart->uart_open(baudrate);
	port2_uart->uart_control(UART_CMD_SET_BAUD, (void *)(baudrate));
}

void sim900a_send_AT_copying()
{
	for(int i=0; i<5; i++)
	{
		for(int j=0; j<20; j++)
		{
			sprintf(in15, "%s %f",in15,W0[i][j]);
		}
	}
	sprintf(in15, "%s%c",in15,0x1A);
	for(int i=5; i<10; i++)
	{
		for(int j=0; j<20; j++)
		{
			sprintf(in16, "%s %f",in16,W0[i][j]);
		}
	}
	sprintf(in16, "%s%c",in16,0x1A);
	for(int i=10; i<15; i++)
	{
		for(int j=0; j<20; j++)
		{
			sprintf(in17, "%s %f",in17,W0[i][j]);
		}
	}
	sprintf(in17, "%s%c",in17,0x1A);
	for(int i=15; i<20; i++)
	{
		for(int j=0; j<20; j++)
		{
			sprintf(in18, "%s %f",in18,W0[i][j]);
		}
	}
	sprintf(in18, "%s%c",in18,0x1A);
	for(int i=20; i<25; i++)
	{
		for(int j=0; j<20; j++)
		{
			sprintf(in19, "%s %f",in19,W0[i][j]);
		}
	}
	sprintf(in19, "%s%c",in19,0x1A);
	for(int i=25; i<30; i++)
	{
		for(int j=0; j<20; j++)
		{
			sprintf(in20, "%s %f",in20,W0[i][j]);
		}
	}
	sprintf(in20, "%s%c",in20,0x1A);
	for(int i=30; i<LAYER_0_OUTDIM; i++)
	{
		for(int j=0; j<20; j++)
		{
			sprintf(in21, "%s %f",in21,W0[i][j]);
		}
	}
	sprintf(in21, "%s%c%c",in21,in22,0x1A);
}

void sim900a_message_copying()
{
	sprintf(in12, "80014EBA53D1751F8DCC5012FF0C8BF753CA65F6655152A9%c", 0x1A);
}

void clean_variable()
{
	for (int i = 0; i < LAYER_0_OUTDIM; i++)
	{
		gLayer0OutVal[i] = 0;
	}
	for (int j = 0; j < LAYER_1_OUTDIM; j++)
	{
		gLayer1OutVal[j] = 0;
	}
	max=0;
	k=k+1;
}

void sim900a_send()               //Send weight data sequentially
{
	port_uart->uart_write(in13, 80);
  	board_delay_ms(3000, 1);
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in15, 2000);
  	board_delay_ms(5000, 1);
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in16, 2000);
  	board_delay_ms(5000, 1);
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in17, 2000);
  	board_delay_ms(5000, 1);
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in18, 2000);
  	board_delay_ms(5000, 1);
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in19, 2000);
  	board_delay_ms(5000, 1);
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in20, 2000);
  	board_delay_ms(5000, 1);
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in21, 2000);
  	board_delay_ms(5000, 1);
}

void buzzer_ring()
{
	key_state=~key_state;
	port2->gpio_write(key_state,0xFF);
	board_delay_ms(10, 1);
	key_state=~key_state;
	port2->gpio_write(key_state,0xFF);
	board_delay_ms(10, 1);
	key_state=~key_state;
	port2->gpio_write(key_state,0xFF);
	board_delay_ms(10, 1);
	key_state=~key_state;
	port2->gpio_write(key_state,0xFF);
	board_delay_ms(10, 1);
}

void weight_download()
{
	sprintf(in23, "%s%c",in23,0x1A);
	port_uart->uart_write(in13, 80);
  	board_delay_ms(3000, 1);
	port_uart->uart_write(in14, 50);
	board_delay_ms(1000, 1);
 	/* apply for downloading W0 */
  	port_uart->uart_write(in23, 50);
  	board_delay_ms(3000, 1);
  	port_uart->uart_read(in25, 15000);
  	board_delay_ms(10000, 1);
	token=strtok(in25,delim);
	while(token!=NULL)
	{
		int i=transfer_index/LAYER_0_INDIM;
		int j=transfer_index%LAYER_0_INDIM;
		W0[i][j]=atof(token);
		transfer_index++;
	}
	transfer_index=0;
	/*downloading W0 has finished */
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in29, 50);
  	board_delay_ms(3000, 1);
 	/* apply for downloading W1 */
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in26, 50);
  	board_delay_ms(3000, 1);
  	port_uart->uart_read(in25, 15000);
  	board_delay_ms(10000, 1);
	token=strtok(in25,delim);
	while(token!=NULL)
	{
		int i=transfer_index/LAYER_1_INDIM;
		int j=transfer_index%LAYER_1_INDIM;
		W1[i][j]=atof(token);
		transfer_index++;
	}
	transfer_index=0;
	/*downloading W1 has finished */
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in30, 50);
  	board_delay_ms(3000, 1);
 	/* apply for downloading B0 */
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in27, 50);
  	board_delay_ms(3000, 1);
  	port_uart->uart_read(in25, 15000);
  	board_delay_ms(10000, 1);
	token=strtok(in25,delim);
	while(token!=NULL)
	{
		B0[transfer_index]=atof(token);
		transfer_index++;
	}
	transfer_index=0;
	/*downloading B0 has finished */
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in31, 50);
  	board_delay_ms(3000, 1);
 	/* apply for downloading B1 */
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in28, 50);
  	board_delay_ms(3000, 1);
  	port_uart->uart_read(in25, 15000);
  	board_delay_ms(10000, 1);
	token=strtok(in25,delim);
	while(token!=NULL)
	{
		B1[transfer_index]=atof(token);
		transfer_index++;
	}
	transfer_index=0;
	/*downloading B1 has finished */
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in32, 50);
  	board_delay_ms(3000, 1);
}

void interval_target_init()
{
	while (name_index < 28)
		{
			filename = train_txt_name[name_index];
			fp = fopen(filename, "r");
			fscanf(fp, "%f %f", &a, &b);
			fscanf(fp, "%f %f", &a, &b);
			fscanf(fp, "%f %f", &a, &b);
			fscanf(fp, "%f %f", &a, &b);
			for (; !feof(fp); i++)
			{
				for (j = 0; j < LAYER_0_INDIM && (!feof(fp)); j += 2)
				{
					for (count_1 = 0; count_1 < 10 && (!feof(fp)); count_1++)
					{
						fscanf(fp, "%f %f", &a, &b);
					}
					inputVal_train[name_index][j] = a;
					inputVal_train[name_index][j + 1] = b;
				}
				if(name_index >=12 && name_index <=16)
				{
					target[name_index][0] = 0;
					target[name_index][1] = 1;
				}
				else
				{
					target[name_index][0] = 1;
					target[name_index][1] = 0;
				}
			}
			fclose(fp);
			name_index++ ;
		}
}

void weight_train()
{
	for (epoch = 0; epoch < EPOCH; epoch++)
		{
			for (i = 0; i < file_counter; i++)
			{
				NNLayerPredict(gMyCalculatorModel, inputVal_train[i]);
				train_loss = NNLayerloss(gMyCalculatorModel, target[i]);
				cal_dp_outlayer(gMyCalculatorModel, target[i], dp_w1, dp_b1);
				cal_dp_hidlayer(gMyCalculatorModel, inputVal_train[i], target[i], dp_w1, dp_b1, dp_w0, dp_b0);
				w_b_upgrate(gMyCalculatorModel, dp_w1, dp_b1, dp_w0, dp_b0);
				for (j = 0; j < LAYER_0_OUTDIM; j++)
				{
					gMyCalculatorModel[0].outVal[j] = 0;
				}
				gMyCalculatorModel[1].outVal[0] = 0; gMyCalculatorModel[1].outVal[1] = 0;
				//memset(gMyCalculatorModel[0].outVal, 0, sizeof(gMyCalculatorModel[0].outVal)); memset(gMyCalculatorModel[1].outVal, 0, sizeof(gMyCalculatorModel[1].outVal));
				//gMyCalculatorModel[0].outVal = gLayer0OutVal;
				//gMyCalculatorModel[1].outVal = gLayer1OutVal;
				//memset(dp_w1, 0, sizeof(dp_w1)); memset(dp_w0, 0, sizeof(dp_w0));
				//memset(dp_b1, 0, sizeof(dp_b1)); memset(dp_b0, 0, sizeof(dp_b0));

			}
		}
}

void target_jud_setting()
{
	if(max==gMyCalculatorModel[1].outVal[0])
	{
		target_judgement[0]=1;
		target_judgement[1]=0;
	}
	if((max==gMyCalculatorModel[1].outVal[1]))
	{
		target_judgement[0]=0;
		target_judgement[1]=1;
	}
}

void converse_target_jud()
{
	if(max==gMyCalculatorModel[1].outVal[0])
	{
		target_judgement[0]=0;
		target_judgement[1]=1;
	}
	if((max==gMyCalculatorModel[1].outVal[1]))
	{
		target_judgement[0]=1;
		target_judgement[1]=0;
	}
}

void physical_parameter_send()
{
	sprintf(in24,"physic:%d%c",Value,0x1A);
	port_uart->uart_write(in13, 80);
	board_delay_ms(3000, 1);
  	port_uart->uart_write(in14, 50);
  	board_delay_ms(1000, 1);
  	port_uart->uart_write(in24, 50);
  	board_delay_ms(5000, 1);
  	Value=0;
}

void write_file()
{
	sprintf(write_filename,".\\train\\%d.txt",file_counter);
	write_fp=fopen(write_filename,"w");
	fwrite(inputVal_jud, sizeof(inputVal_jud) , 1, write_fp );
	target[file_counter][0]=target_judgement[0];
	target[file_counter][1]=target_judgement[1];
	fclose(write_fp);
}
