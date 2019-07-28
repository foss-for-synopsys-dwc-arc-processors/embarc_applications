#include "ov7670.h"


static void delay_us(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=11;x>0;x--)
		for(y=z;y>0;y--);
}

static void delay_ms(volatile int z) //1ms
{
	volatile uint32_t x,y;
	for(x=11000;x>0;x--)
		for(y=z;y>0;y--);
}
void SCCB_SDA_OUT(void)
{
	port_sda-> gpio_close();
	delay_us(50);
 	port_sda = gpio_get_dev(DFSS_GPIO_8B1_ID);//定义pmod B7接口
	port_sda-> gpio_open(0x10);//B7[4]  //此处决定具体连接哪个端口
	port_sda-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10);  //设置B7[4]为输出 
}

void SCCB_SDA_IN(void)
{
	port_sda-> gpio_close();
	delay_us(50);
 	port_sda = gpio_get_dev(DFSS_GPIO_8B1_ID); 
	port_sda-> gpio_open(0x10);//B7
	port_sda-> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x10);   //设置B7[4]为输入 
}
 
//初始化SCCB接口
void SCCB_Init(void)
{			
 	port_sda = gpio_get_dev(DFSS_GPIO_8B1_ID);
	port_sda-> gpio_open(0x10);//B7[4]
	port_sda-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10);
	port_sda-> gpio_write(0x10,0x10);  //初始为高
	
	port_scl = gpio_get_dev(DFSS_GPIO_8B1_ID);
	port_scl-> gpio_open(0x20);//B8[5]
	port_scl-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x20);
	port_scl-> gpio_write(0x20,0x20);  //初始为高
	
 
	SCCB_SDA_OUT();
    EMBARC_PRINTF("SCCB_Init is successful\r\n");	
}			 

//SCCB起始信号
//当时钟为高的时候,数据线的高到低,为SCCB起始信号
//在激活状态下,SDA和SCL均为低电平
void SCCB_Start(void)
{
    port_sda-> gpio_write(0x10,0x10);     //sda数据线高电平
    delay_us(50); 	
    port_scl-> gpio_write(0x20,0x20);	    //在时钟线高的时候数据线由高至低
    delay_us(50);  
    port_sda-> gpio_write(0x00,0x10);
    delay_us(50);	 
    port_scl-> gpio_write(0x00,0x20);	    //数据线恢复低电平，单操作函数必要	  
}

//SCCB停止信号
//当时钟为高的时候,数据线的低到高,为SCCB停止信号
//空闲状况下,SDA,SCL均为高电平
void SCCB_Stop(void)
{
    port_sda-> gpio_write(0x00,0x10);
    delay_us(50);	 
    port_scl-> gpio_write(0x20,0x20);  	 //时钟线为高
    delay_us(50); 
    port_sda-> gpio_write(0x10,0x10);	
    delay_us(50);
} 
 
//产生NA信号
void SCCB_No_Ack(void)
{
	delay_us(50);
	port_sda-> gpio_write(0x10,0x10);	
	port_scl-> gpio_write(0x20,0x20);	
	delay_us(50);
	port_scl-> gpio_write(0x00,0x20);	
	delay_us(50);
	port_sda-> gpio_write(0x00,0x10);	
	delay_us(50);
}

//SCCB,写入一个字节
//返回值:0,成功;1,失败. 
uint8_t SCCB_WR_Byte(uint8_t dat)
{
	uint8_t j,res;
    uint32_t read_sda;	
	for(j=0;j<8;j++) //循环8次发送数据
	{
		if(dat&0x80)
			   port_sda-> gpio_write(0x10,0x10);	
		else port_sda-> gpio_write(0x00,0x10);
		dat<<=1;
		delay_us(50);
		port_scl-> gpio_write(0x20,0x20);	
		delay_us(50);
		port_scl-> gpio_write(0x00,0x20);		   
	}			 
	SCCB_SDA_IN();		//设置SDA为输入 
	delay_us(50);
	port_scl-> gpio_write(0x20,0x20);			//接收第九位,以判断是否发送成功
	delay_us(50);
 	port_sda-> gpio_read(&read_sda, 0x10);
	read_sda=read_sda>>4;
//	EMBARC_PRINTF("sda= %x\n",read_sda);
	if(read_sda==0x1)
		  res=1;  //SDA=1发送失败，返回1
	else 
		res=0;         //SDA=0发送成功，返回0
	port_scl-> gpio_write(0x00,0x20);		 
	SCCB_SDA_OUT();		//设置SDA为输出    
	return res;  
}
	 
//SCCB 读取一个字节
//在SCL的上升沿,数据锁存
//返回值:读到的数据
uint8_t SCCB_RD_Byte(void)
{
	uint8_t temp=0,j;
    uint32_t read_sda;    
	SCCB_SDA_IN();		//设置SDA为输入  
	for(j=8;j>0;j--) 	//循环8次接收数据
	{		     	  
		delay_us(50);
		port_scl-> gpio_write(0x20,0x20);
		temp=temp<<1;
		port_sda-> gpio_read(&read_sda, 0x10);
	    read_sda=read_sda>>4;
		if(read_sda==0x1)
			      temp++;   
		delay_us(50);
		port_scl-> gpio_write(0x00,0x20);
	}	
	SCCB_SDA_OUT();		//设置SDA为输出    
	return temp;
} 							    
//写寄存器
//返回值:0,成功;1,失败. //2019612
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data)
{
	uint8_t res=0;
	SCCB_Start(); 					//启动SCCB传输
	if(SCCB_WR_Byte(SCCB_ID))       //写器件ID
	res=1;
         //发送SCCB总线停止传输命令		
	delay_us(100);
  	if(SCCB_WR_Byte(reg))       //写寄存器地址
	    res=1;		
	delay_us(100);
  	if(SCCB_WR_Byte(data))      //写数据
       res=1;	   
  	SCCB_Stop();	  
  	return	res;
}
		  					    
//读寄存器
//返回值:读到的寄存器值
uint8_t SCCB_RD_Reg(uint8_t reg)
{
	uint8_t val=0;
	//通过写操作设置寄存器地址
	SCCB_Start(); 				//启动SCCB传输
	SCCB_WR_Byte(SCCB_ID);		//写器件ID	  
	delay_us(100);	 
  	SCCB_WR_Byte(reg);			//写寄存器地址	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	//设置寄存器地址后，才是读
	SCCB_Start();
	SCCB_WR_Byte(0x43);	//发送读命令	0x43=SCCB_ID|0x01  
	delay_us(100);
  	val=SCCB_RD_Byte();		 	//读取数据
  	SCCB_No_Ack();
  	SCCB_Stop();
  	return val;
}
void Rclk_h()
{
	port_RCLK-> gpio_write(0x20,0x20);
};
void Rclk_l()
{
	port_RCLK-> gpio_write(0x00,0x20);
};

uint32_t read_data()
{
	uint32_t data;
	port_D0_D7-> gpio_read(&data,0xff);
	return data;
};
//初始化OV7670
//返回0成功，返回其他值失败
uint8_t OV7670_init(void)
{
    uint8_t temp;
    uint16_t i=0;
	
  port_VSYNC = gpio_get_dev(DFSS_GPIO_8B0_ID);//定义pmod A1接口
	port_VSYNC->gpio_open(0x1);//A1[0]  //此处决定具体连接哪个端口
	port_VSYNC->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x1);
	
	port_WEN = gpio_get_dev(DFSS_GPIO_8B0_ID);//定义pmod A2接口
	port_WEN-> gpio_open(0x2);//A2[1]  //此处决定具体连接哪个端口
	port_WEN-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x2);
	port_WEN-> gpio_write(0x0,0x2);

  port_RCLK = gpio_get_dev(DFSS_GPIO_8B0_ID);//定义pmod A8接口
	port_RCLK-> gpio_open(0x20);//A8[5]  //此处决定具体连接哪个端口
	port_RCLK-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x20);
	port_RCLK-> gpio_write(0x20,0x20);
	 
	port_D0_D7 = gpio_get_dev(DFSS_GPIO_8B2_ID);//定义IO4~IO11接口
	port_D0_D7-> gpio_open(0xff);//open [0:7]  //此处决定具体连接哪个端口
	port_D0_D7-> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0xff);
	
	port_WRST = gpio_get_dev(DFSS_GPIO_8B0_ID);//定义pmod A4接口
	port_WRST-> gpio_open(0x8);//A4[3]  //此处决定具体连接哪个端口
	port_WRST-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x8);
	port_WRST-> gpio_write(0x8,0x8);

  port_RRST = gpio_get_dev(DFSS_GPIO_8B0_ID);//定义pmod A3接口
	port_RRST-> gpio_open(0x4);//A3[2]  //此处决定具体连接哪个端口
	port_RRST-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x4);
	port_RRST-> gpio_write(0x4,0x4);

	port_OE = gpio_get_dev(DFSS_GPIO_8B0_ID);//定义pmod A7接口
	port_OE-> gpio_open(0x10);//A7[4]  //此处决定具体连接哪个端口
	port_OE-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10);
	port_OE-> gpio_write(0x10,0x10);
	

    
    SCCB_Init();            //初始化SCCB

    if(SCCB_WR_Reg(0x12,0x80))  return 1;   //复位SCCB
    delay_ms(10);	  
	for(i=0;i<(sizeof(OV7670_reg)/sizeof(OV7670_reg[0]));i++)
	{
	   	SCCB_WR_Reg(OV7670_reg[i][0],OV7670_reg[i][1]);
  	}
  	port_OE-> gpio_write(0x00,0x10);
    EMBARC_PRINTF("OV7670_init is successful\r\n");
   	   return 0x00;   //ok
}

void judge_vsync()
{
	uint32_t vsync_data;
	port_VSYNC-> gpio_read(&vsync_data,0x1);
	while(vsync_data!=0x1){
		port_VSYNC-> gpio_read(&vsync_data,0x1);
	}
	if(vsync_data=0x1)
	{
	  port_WRST-> gpio_write(0x8,0x8);
	  delay_us(100);
	  port_WRST-> gpio_write(0x0,0x8);
	  delay_us(100);
      port_WRST-> gpio_write(0x8,0x8);
	  delay_us(100);
      port_WEN-> gpio_write(0x2,0x2);
	  port_VSYNC-> gpio_read(&vsync_data,0x1);
	  while(vsync_data!=0x1){
		  port_VSYNC-> gpio_read(&vsync_data,0x1);
	  }
      port_WEN-> gpio_write(0x0,0x2); 
	}
		port_RRST-> gpio_write(0x0,0x4);
		delay_us(1);
		port_RCLK-> gpio_write(0x00,0x20);
		delay_us(1);
		port_RCLK-> gpio_write(0x20,0x20);
		delay_us(1);
    port_RCLK-> gpio_write(0x00,0x20);
		delay_us(1);
		port_RRST-> gpio_write(0x4,0x4);
		delay_us(1);
		port_RCLK-> gpio_write(0x20,0x20);
	
}
//OV7670功能设置
//白平衡设置
//0:自动
//1:太阳sunny
//2,阴天cloudy
//3,办公室office
//4,家里home
void OV7670_Light_Mode(uint8_t mode)
{
	uint8_t reg13val=0XE7;//默认就是设置为自动白平衡
	uint8_t reg01val=0;
	uint8_t reg02val=0;
	switch(mode)
	{
		case 1://sunny
			reg13val=0XE5;
			reg01val=0X5A;
			reg02val=0X5C;
			break;	
		case 2://cloudy
			reg13val=0XE5;
			reg01val=0X58;
			reg02val=0X60;
			break;	
		case 3://office
			reg13val=0XE5;
			reg01val=0X84;
			reg02val=0X4c;
			break;	
		case 4://home
			reg13val=0XE5;
			reg01val=0X96;
			reg02val=0X40;
			break;	
	}
	SCCB_WR_Reg(0X13,reg13val);//COM8设置 
	SCCB_WR_Reg(0X01,reg01val);//AWB蓝色通道增益 
	SCCB_WR_Reg(0X02,reg02val);//AWB红色通道增益 
}				  
//色度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(uint8_t sat)
{
	uint8_t reg4f5054val=0X80;//默认就是sat=2,即不调节色度的设置
 	uint8_t reg52val=0X22;
	uint8_t reg53val=0X5E;
 	switch(sat)
	{
		case 0://-2
			reg4f5054val=0X40;  	 
			reg52val=0X11;
			reg53val=0X2F;	 	 
			break;	
		case 1://-1
			reg4f5054val=0X66;	    
			reg52val=0X1B;
			reg53val=0X4B;	  
			break;	
		case 3://1
			reg4f5054val=0X99;	   
			reg52val=0X28;
			reg53val=0X71;	   
			break;	
		case 4://2
			reg4f5054val=0XC0;	   
			reg52val=0X33;
			reg53val=0X8D;	   
			break;	
	}
	SCCB_WR_Reg(0X4F,reg4f5054val);	//色彩矩阵系数1
	SCCB_WR_Reg(0X50,reg4f5054val);	//色彩矩阵系数2 
	SCCB_WR_Reg(0X51,0X00);			//色彩矩阵系数3  
	SCCB_WR_Reg(0X52,reg52val);		//色彩矩阵系数4 
	SCCB_WR_Reg(0X53,reg53val);		//色彩矩阵系数5 
	SCCB_WR_Reg(0X54,reg4f5054val);	//色彩矩阵系数6  
	SCCB_WR_Reg(0X58,0X9E);			//MTXS 
}
//亮度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(uint8_t bright)
{
	uint8_t reg55val=0X00;//默认就是bright=2
  	switch(bright)
	{
		case 0://-2
			reg55val=0XB0;	 	 
			break;	
		case 1://-1
			reg55val=0X98;	 	 
			break;	
		case 3://1
			reg55val=0X18;	 	 
			break;	
		case 4://2
			reg55val=0X30;	 	 
			break;	
	}
	SCCB_WR_Reg(0X55,reg55val);	//亮度调节 
}
//对比度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(uint8_t contrast)
{
	uint8_t reg56val=0X40;//默认就是contrast=2
  	switch(contrast)
	{
		case 0://-2
			reg56val=0X30;	 	 
			break;	
		case 1://-1
			reg56val=0X38;	 	 
			break;	
		case 3://1
			reg56val=0X50;	 	 
			break;	
		case 4://2
			reg56val=0X60;	 	 
			break;	
	}
	SCCB_WR_Reg(0X56,reg56val);	//对比度调节 
}
//特效设置
//0:普通模式    
//1,负片
//2,黑白   
//3,偏红色
//4,偏绿色
//5,偏蓝色
//6,复古	    
void OV7670_Special_Effects(uint8_t eft)
{
	uint8_t reg3aval=0X04;//默认为普通模式
	uint8_t reg67val=0XC0;
	uint8_t reg68val=0X80;
	switch(eft)
	{
		case 1://负片
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://黑白
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://偏红色
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://偏绿色
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://偏蓝色
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://复古
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	SCCB_WR_Reg(0X3A,reg3aval);//TSLB设置 
	SCCB_WR_Reg(0X68,reg67val);//MANU,手动U值 
	SCCB_WR_Reg(0X67,reg68val);//MANV,手动V值 
}	
//设置图像输出窗口
//对QVGA设置。
void OV7670_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t endx;
	uint16_t endy;
	uint8_t temp; 
	endx=sx+width*2;	//V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	temp=SCCB_RD_Reg(0X03);				//读取Vref之前的值
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(0X03,temp);				//设置Vref的start和end的最低2位
	SCCB_WR_Reg(0X19,sx>>2);			//设置Vref的start高8位
	SCCB_WR_Reg(0X1A,endx>>2);			//设置Vref的end的高8位

	temp=SCCB_RD_Reg(0X32);				//读取Href之前的值
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(0X32,temp);             //2019718
	SCCB_WR_Reg(0X17,sy>>3);			//设置Href的start高8位
	SCCB_WR_Reg(0X18,endy>>3);			//设置Href的end的高8位
}