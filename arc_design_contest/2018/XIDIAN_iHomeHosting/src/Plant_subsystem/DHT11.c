#include "embARC.h"
#include "embARC_debug.h"

#include "dev_gpio.h"
#include "board.h"
#include "DHT11.h"


//延时函数，设y是实际延迟时间，x是设置值，则y=0.87911111*x
//如需延时50us，即y=50,则x=（y/0.87911111）取整
static void delay_us( uint32_t us) 
{
	uint32_t volatile x,y;
	for(x=2;x>0;x--)
		for(y=us;y>0;y--);
	
}

static uint8_t DHT11_Read_Byte(DEV_GPIO_PTR DHT11_port, uint32_t DHT11_bit)
{
	
	uint8_t level_cnt=0;
	uint32_t read_data=0;
	uint8_t uchartemp,ucharcomdata;

	
	uint32_t DHT11_MASK = 0x01 << DHT11_bit;	
	DHT11_port->gpio_open(GPIO_DIR_OUTPUT);

	for(uint8_t i=0;i<8;i++)
	{
		level_cnt=2;
		do{
			DHT11_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(DHT11_MASK));
			DHT11_port->gpio_read(&read_data,DHT11_MASK);
			read_data &= DHT11_MASK;			
		}while((!read_data) && level_cnt++);
		delay_us(30);
		uchartemp = 0;

		DHT11_port->gpio_read(&read_data,DHT11_MASK);
		read_data &= DHT11_MASK;
		if(read_data) uchartemp = 1;

		level_cnt=2;
		do{
			DHT11_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(DHT11_MASK));
			DHT11_port->gpio_read(&read_data,DHT11_MASK);
			read_data &= DHT11_MASK;			
		}while((read_data) && level_cnt++);
		if(level_cnt == 1) break;
		ucharcomdata <<= 1;
		ucharcomdata |= uchartemp;
	}
	return ucharcomdata;
}

void DHT11_Read_Data(DEV_GPIO_PTR DHT11_port, uint32_t DHT11_bit, DHT11_DEF_PTR data)
{	
	uint32_t read_data=0;
	uint8_t level_cnt=0;
	uint8_t RH_data_H_temp,RH_data_L_temp,T_data_H_temp,T_data_L_temp,checkdata_temp;
	uint8_t RH_data_H,RH_data_L,T_data_H,T_data_L,checkdata;
	
	uint32_t DHT11_MASK = 0x01 << DHT11_bit;	
	DHT11_port->gpio_open(GPIO_DIR_OUTPUT);
	uint32_t DHT11_WRITE_1 = 0x01 << DHT11_bit;	

	//端口配置为输出
	DHT11_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(DHT11_MASK));
	//总线拉低
	DHT11_port->gpio_write(0x00,DHT11_MASK);
	//延时18ms以上
	delay_us(20000); //delay > 18ms
	//总线拉高
	DHT11_port->gpio_write(DHT11_WRITE_1,DHT11_MASK);
	//重新配置端口为输入
	DHT11_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(DHT11_MASK));
	//延时20到40us
	delay_us(30);
	DHT11_port->gpio_read(&read_data,DHT11_MASK);
	read_data &= DHT11_MASK;
	if(!read_data)
	{
		level_cnt = 2;
		do{
			DHT11_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(DHT11_MASK));
			DHT11_port->gpio_read(&read_data,DHT11_MASK);
			read_data &= DHT11_MASK;
			// delay_us(2);
		}while((!read_data) && level_cnt++);
		level_cnt = 2;
		do{
			DHT11_port->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(DHT11_MASK));
			DHT11_port->gpio_read(&read_data,DHT11_MASK);
			read_data &= DHT11_MASK;
			// delay_us(2);
		}while((read_data) && level_cnt++);

		RH_data_H_temp=DHT11_Read_Byte(DHT11_port, DHT11_bit);
		RH_data_L_temp=DHT11_Read_Byte(DHT11_port, DHT11_bit);
		T_data_H_temp =DHT11_Read_Byte(DHT11_port, DHT11_bit);
		T_data_L_temp =DHT11_Read_Byte(DHT11_port, DHT11_bit);
		checkdata_temp = DHT11_Read_Byte(DHT11_port, DHT11_bit);
		//端口配置为输出
		DHT11_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(DHT11_MASK));
		//总线拉高
		DHT11_port->gpio_write(DHT11_WRITE_1,DHT11_MASK);
		if(checkdata_temp == (
			RH_data_H_temp + RH_data_L_temp +
			T_data_H_temp + T_data_L_temp
			))
		{
			RH_data_H = RH_data_H_temp;
			RH_data_L = RH_data_L_temp;
			T_data_H = T_data_H_temp;
			T_data_L = T_data_L_temp;
			checkdata = checkdata_temp;
		}
		data->RH_data = RH_data_H;
		data->T_data = T_data_H;
	}
	else
	{
		data->RH_data = 0;
		data->T_data = 0;
	}

	DHT11_port->gpio_close();
}

