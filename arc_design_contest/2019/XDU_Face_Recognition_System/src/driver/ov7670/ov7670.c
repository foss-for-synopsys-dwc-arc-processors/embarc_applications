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
 	port_sda = gpio_get_dev(DFSS_GPIO_8B1_ID);//����pmod B7�ӿ�
	port_sda-> gpio_open(0x10);//B7[4]  //�˴��������������ĸ��˿�
	port_sda-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10);  //����B7[4]Ϊ��� 
}

void SCCB_SDA_IN(void)
{
	port_sda-> gpio_close();
	delay_us(50);
 	port_sda = gpio_get_dev(DFSS_GPIO_8B1_ID); 
	port_sda-> gpio_open(0x10);//B7
	port_sda-> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x10);   //����B7[4]Ϊ���� 
}
 
//��ʼ��SCCB�ӿ�
void SCCB_Init(void)
{			
 	port_sda = gpio_get_dev(DFSS_GPIO_8B1_ID);
	port_sda-> gpio_open(0x10);//B7[4]
	port_sda-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10);
	port_sda-> gpio_write(0x10,0x10);  //��ʼΪ��
	
	port_scl = gpio_get_dev(DFSS_GPIO_8B1_ID);
	port_scl-> gpio_open(0x20);//B8[5]
	port_scl-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x20);
	port_scl-> gpio_write(0x20,0x20);  //��ʼΪ��
	
 
	SCCB_SDA_OUT();
    EMBARC_PRINTF("SCCB_Init is successful\r\n");	
}			 

//SCCB��ʼ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĸߵ���,ΪSCCB��ʼ�ź�
//�ڼ���״̬��,SDA��SCL��Ϊ�͵�ƽ
void SCCB_Start(void)
{
    port_sda-> gpio_write(0x10,0x10);     //sda�����߸ߵ�ƽ
    delay_us(50); 	
    port_scl-> gpio_write(0x20,0x20);	    //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(50);  
    port_sda-> gpio_write(0x00,0x10);
    delay_us(50);	 
    port_scl-> gpio_write(0x00,0x20);	    //�����߻ָ��͵�ƽ��������������Ҫ	  
}

//SCCBֹͣ�ź�
//��ʱ��Ϊ�ߵ�ʱ��,�����ߵĵ͵���,ΪSCCBֹͣ�ź�
//����״����,SDA,SCL��Ϊ�ߵ�ƽ
void SCCB_Stop(void)
{
    port_sda-> gpio_write(0x00,0x10);
    delay_us(50);	 
    port_scl-> gpio_write(0x20,0x20);  	 //ʱ����Ϊ��
    delay_us(50); 
    port_sda-> gpio_write(0x10,0x10);	
    delay_us(50);
} 
 
//����NA�ź�
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

//SCCB,д��һ���ֽ�
//����ֵ:0,�ɹ�;1,ʧ��. 
uint8_t SCCB_WR_Byte(uint8_t dat)
{
	uint8_t j,res;
    uint32_t read_sda;	
	for(j=0;j<8;j++) //ѭ��8�η�������
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
	SCCB_SDA_IN();		//����SDAΪ���� 
	delay_us(50);
	port_scl-> gpio_write(0x20,0x20);			//���յھ�λ,���ж��Ƿ��ͳɹ�
	delay_us(50);
 	port_sda-> gpio_read(&read_sda, 0x10);
	read_sda=read_sda>>4;
//	EMBARC_PRINTF("sda= %x\n",read_sda);
	if(read_sda==0x1)
		  res=1;  //SDA=1����ʧ�ܣ�����1
	else 
		res=0;         //SDA=0���ͳɹ�������0
	port_scl-> gpio_write(0x00,0x20);		 
	SCCB_SDA_OUT();		//����SDAΪ���    
	return res;  
}
	 
//SCCB ��ȡһ���ֽ�
//��SCL��������,��������
//����ֵ:����������
uint8_t SCCB_RD_Byte(void)
{
	uint8_t temp=0,j;
    uint32_t read_sda;    
	SCCB_SDA_IN();		//����SDAΪ����  
	for(j=8;j>0;j--) 	//ѭ��8�ν�������
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
	SCCB_SDA_OUT();		//����SDAΪ���    
	return temp;
} 							    
//д�Ĵ���
//����ֵ:0,�ɹ�;1,ʧ��. //2019612
uint8_t SCCB_WR_Reg(uint8_t reg,uint8_t data)
{
	uint8_t res=0;
	SCCB_Start(); 					//����SCCB����
	if(SCCB_WR_Byte(SCCB_ID))       //д����ID
	res=1;
         //����SCCB����ֹͣ��������		
	delay_us(100);
  	if(SCCB_WR_Byte(reg))       //д�Ĵ�����ַ
	    res=1;		
	delay_us(100);
  	if(SCCB_WR_Byte(data))      //д����
       res=1;	   
  	SCCB_Stop();	  
  	return	res;
}
		  					    
//���Ĵ���
//����ֵ:�����ļĴ���ֵ
uint8_t SCCB_RD_Reg(uint8_t reg)
{
	uint8_t val=0;
	//ͨ��д�������üĴ�����ַ
	SCCB_Start(); 				//����SCCB����
	SCCB_WR_Byte(SCCB_ID);		//д����ID	  
	delay_us(100);	 
  	SCCB_WR_Byte(reg);			//д�Ĵ�����ַ	  
	delay_us(100);	  
	SCCB_Stop();   
	delay_us(100);	   
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	SCCB_WR_Byte(0x43);	//���Ͷ�����	0x43=SCCB_ID|0x01  
	delay_us(100);
  	val=SCCB_RD_Byte();		 	//��ȡ����
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
//��ʼ��OV7670
//����0�ɹ�����������ֵʧ��
uint8_t OV7670_init(void)
{
    uint8_t temp;
    uint16_t i=0;
	
  port_VSYNC = gpio_get_dev(DFSS_GPIO_8B0_ID);//����pmod A1�ӿ�
	port_VSYNC->gpio_open(0x1);//A1[0]  //�˴��������������ĸ��˿�
	port_VSYNC->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0x1);
	
	port_WEN = gpio_get_dev(DFSS_GPIO_8B0_ID);//����pmod A2�ӿ�
	port_WEN-> gpio_open(0x2);//A2[1]  //�˴��������������ĸ��˿�
	port_WEN-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x2);
	port_WEN-> gpio_write(0x0,0x2);

  port_RCLK = gpio_get_dev(DFSS_GPIO_8B0_ID);//����pmod A8�ӿ�
	port_RCLK-> gpio_open(0x20);//A8[5]  //�˴��������������ĸ��˿�
	port_RCLK-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x20);
	port_RCLK-> gpio_write(0x20,0x20);
	 
	port_D0_D7 = gpio_get_dev(DFSS_GPIO_8B2_ID);//����IO4~IO11�ӿ�
	port_D0_D7-> gpio_open(0xff);//open [0:7]  //�˴��������������ĸ��˿�
	port_D0_D7-> gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)0xff);
	
	port_WRST = gpio_get_dev(DFSS_GPIO_8B0_ID);//����pmod A4�ӿ�
	port_WRST-> gpio_open(0x8);//A4[3]  //�˴��������������ĸ��˿�
	port_WRST-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x8);
	port_WRST-> gpio_write(0x8,0x8);

  port_RRST = gpio_get_dev(DFSS_GPIO_8B0_ID);//����pmod A3�ӿ�
	port_RRST-> gpio_open(0x4);//A3[2]  //�˴��������������ĸ��˿�
	port_RRST-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x4);
	port_RRST-> gpio_write(0x4,0x4);

	port_OE = gpio_get_dev(DFSS_GPIO_8B0_ID);//����pmod A7�ӿ�
	port_OE-> gpio_open(0x10);//A7[4]  //�˴��������������ĸ��˿�
	port_OE-> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10);
	port_OE-> gpio_write(0x10,0x10);
	

    
    SCCB_Init();            //��ʼ��SCCB

    if(SCCB_WR_Reg(0x12,0x80))  return 1;   //��λSCCB
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
//OV7670��������
//��ƽ������
//0:�Զ�
//1:̫��sunny
//2,����cloudy
//3,�칫��office
//4,����home
void OV7670_Light_Mode(uint8_t mode)
{
	uint8_t reg13val=0XE7;//Ĭ�Ͼ�������Ϊ�Զ���ƽ��
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
	SCCB_WR_Reg(0X13,reg13val);//COM8���� 
	SCCB_WR_Reg(0X01,reg01val);//AWB��ɫͨ������ 
	SCCB_WR_Reg(0X02,reg02val);//AWB��ɫͨ������ 
}				  
//ɫ������
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(uint8_t sat)
{
	uint8_t reg4f5054val=0X80;//Ĭ�Ͼ���sat=2,��������ɫ�ȵ�����
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
	SCCB_WR_Reg(0X4F,reg4f5054val);	//ɫ�ʾ���ϵ��1
	SCCB_WR_Reg(0X50,reg4f5054val);	//ɫ�ʾ���ϵ��2 
	SCCB_WR_Reg(0X51,0X00);			//ɫ�ʾ���ϵ��3  
	SCCB_WR_Reg(0X52,reg52val);		//ɫ�ʾ���ϵ��4 
	SCCB_WR_Reg(0X53,reg53val);		//ɫ�ʾ���ϵ��5 
	SCCB_WR_Reg(0X54,reg4f5054val);	//ɫ�ʾ���ϵ��6  
	SCCB_WR_Reg(0X58,0X9E);			//MTXS 
}
//��������
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(uint8_t bright)
{
	uint8_t reg55val=0X00;//Ĭ�Ͼ���bright=2
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
	SCCB_WR_Reg(0X55,reg55val);	//���ȵ��� 
}
//�Աȶ�����
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(uint8_t contrast)
{
	uint8_t reg56val=0X40;//Ĭ�Ͼ���contrast=2
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
	SCCB_WR_Reg(0X56,reg56val);	//�Աȶȵ��� 
}
//��Ч����
//0:��ͨģʽ    
//1,��Ƭ
//2,�ڰ�   
//3,ƫ��ɫ
//4,ƫ��ɫ
//5,ƫ��ɫ
//6,����	    
void OV7670_Special_Effects(uint8_t eft)
{
	uint8_t reg3aval=0X04;//Ĭ��Ϊ��ͨģʽ
	uint8_t reg67val=0XC0;
	uint8_t reg68val=0X80;
	switch(eft)
	{
		case 1://��Ƭ
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://�ڰ�
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://ƫ��ɫ
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://����
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	SCCB_WR_Reg(0X3A,reg3aval);//TSLB���� 
	SCCB_WR_Reg(0X68,reg67val);//MANU,�ֶ�Uֵ 
	SCCB_WR_Reg(0X67,reg68val);//MANV,�ֶ�Vֵ 
}	
//����ͼ���������
//��QVGA���á�
void OV7670_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t endx;
	uint16_t endy;
	uint8_t temp; 
	endx=sx+width*2;	//V*2
 	endy=sy+height*2;
	if(endy>784)endy-=784;
	temp=SCCB_RD_Reg(0X03);				//��ȡVref֮ǰ��ֵ
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	SCCB_WR_Reg(0X03,temp);				//����Vref��start��end�����2λ
	SCCB_WR_Reg(0X19,sx>>2);			//����Vref��start��8λ
	SCCB_WR_Reg(0X1A,endx>>2);			//����Vref��end�ĸ�8λ

	temp=SCCB_RD_Reg(0X32);				//��ȡHref֮ǰ��ֵ
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	SCCB_WR_Reg(0X32,temp);             //2019718
	SCCB_WR_Reg(0X17,sy>>3);			//����Href��start��8λ
	SCCB_WR_Reg(0X18,endy>>3);			//����Href��end�ĸ�8λ
}