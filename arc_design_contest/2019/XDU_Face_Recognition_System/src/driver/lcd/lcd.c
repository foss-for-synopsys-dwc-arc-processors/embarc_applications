#include "lcd.h"


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
/////////////////////////////LCD显示屏/////////////////////////
void OLED_CS_Clr(void)
{
   port_cs1 = gpio_get_dev(DFSS_GPIO_8B3_ID);	//定义AD4端口
   port_cs1 -> gpio_open(0x8); //AD4[3]
   port_cs1 -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x8);  //设置为输出
   port_cs1 -> gpio_write(0x0,0x8);
}
void OLED_CS_Set(void)
{
   port_cs1 = gpio_get_dev(DFSS_GPIO_8B3_ID);	//定义AD4端口
   port_cs1 -> gpio_open(0x8); //AD4[3]
   port_cs1 -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x8);  //设置为输出
   port_cs1 -> gpio_write(0x8,0x8);
}

void OLED_RST_Clr(void)
{
   port_rst = gpio_get_dev(DFSS_GPIO_8B3_ID); //定义AD2端口
   port_rst -> gpio_open(0x20);  //AD2[5]
   port_rst -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x20);  //设置为输出
   port_rst -> gpio_write(0x00,0x20);
}
void OLED_RST_Set(void)
{
   port_rst = gpio_get_dev(DFSS_GPIO_8B3_ID); //定义AD2端口
   port_rst -> gpio_open(0x20);  //AD2[5]
   port_rst -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x20);  //设置为输出
   port_rst -> gpio_write(0x20,0x20);
}

void OLED_DC_Clr(void)
{
   port_dc = gpio_get_dev(DFSS_GPIO_8B3_ID);  //定义AD3端口
   port_dc -> gpio_open(0x10);  //AD3[4]
   port_dc -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10);  //设置为输出
   port_dc -> gpio_write(0x00,0x10);
}
void OLED_DC_Set(void)
{
   port_dc = gpio_get_dev(DFSS_GPIO_8B3_ID);  //定义AD3端口
   port_dc -> gpio_open(0x10);  //AD3[4]
   port_dc -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10);  //设置为输出
   port_dc -> gpio_write(0x10,0x10);
}

void OLED_CLK_Clr(void)
{
   port_clk = gpio_get_dev(DFSS_GPIO_8B3_ID);  //定义AD0端口
   port_clk -> gpio_open(0x80);  //AD0[7]
   port_clk -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x80);  //设置为输出
   port_clk -> gpio_write(0x00,0x80);
}
void OLED_CLK_Set(void)
{
   port_clk = gpio_get_dev(DFSS_GPIO_8B3_ID);  //定义AD0端口
   port_clk -> gpio_open(0x80);  //AD0[7]
   port_clk -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x80);  //设置为输出
   port_clk -> gpio_write(0x80,0x80);
}

void OLED_MOSI_Clr(void)
{
   port_mosi = gpio_get_dev(DFSS_GPIO_8B3_ID);  //定义AD1端口
   port_mosi -> gpio_open(0x40);  //AD1[6]
   port_mosi -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x40);  //设置为输出
   port_mosi -> gpio_write(0x00,0x40);
}
void OLED_MOSI_Set(void)
{
   port_mosi = gpio_get_dev(DFSS_GPIO_8B3_ID);  //定义AD1端口
   port_mosi -> gpio_open(0x40);  //AD1[6]
   port_mosi -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x40);  //设置为输出
   port_mosi -> gpio_write(0x40,0x40);
}

uint16_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色
void LCD_Writ_Bus(char dat)   //串行数据写入
{	
	uint8_t i;			  
	for(i=0;i<8;i++)
	{			  
		OLED_CLK_Clr();
		if(dat&0x80)
		   OLED_MOSI_Set();
		else 
		   OLED_MOSI_Clr();
		OLED_CLK_Set();
		dat<<=1;   
	}			
}

void LCD_WR_DATA8(char da) //发送数据-8位参数
{	OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Writ_Bus(da);  
	OLED_CS_Set();
}  
 void LCD_WR_DATA(int da)
{	OLED_CS_Clr();
    OLED_DC_Set();
	LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
	OLED_CS_Set();
}	  
void LCD_WR_REG(char da)	 
{	OLED_CS_Clr();
    OLED_DC_Clr();
	LCD_Writ_Bus(da);
	OLED_CS_Set();
}
 void LCD_WR_REG_DATA(int reg,int da)
{	OLED_CS_Clr();
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
	OLED_CS_Set();
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
   LCD_WR_REG(0x2a);
  LCD_WR_DATA8(x1>>8);
   // LCD_WR_DATA8(x1/256);
  LCD_WR_DATA8(x1);
   // LCD_WR_DATA8(x1%256);
  LCD_WR_DATA8(x2>>8);
   // LCD_WR_DATA8(x2/256);
  LCD_WR_DATA8(x2);
   // LCD_WR_DATA8(x2%256);
  
   LCD_WR_REG(0x2b);
  LCD_WR_DATA8(y1>>8);
   // LCD_WR_DATA8(y1/256);
  LCD_WR_DATA8(y1);
   // LCD_WR_DATA8(y1%256);
  LCD_WR_DATA8(y2>>8);
   // LCD_WR_DATA8(y2/256);
  LCD_WR_DATA8(y2);
   // LCD_WR_DATA8(y2%256);
   

   LCD_WR_REG(0x2c);					 						 
}

void Lcd_Init(void)
{
    port_cs1 = gpio_get_dev(DFSS_GPIO_8B3_ID);	//定义AD4端口
    port_cs1 -> gpio_open(0x8); //AD4[3]
    port_cs1 -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x8);  //设置为输出	

    port_rst = gpio_get_dev(DFSS_GPIO_8B3_ID); //定义AD2端口
    port_rst -> gpio_open(0x20);  //AD2[5]
    port_rst -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x20);  //设置为输出
	
	port_dc = gpio_get_dev(DFSS_GPIO_8B3_ID);  //定义AD3端口
    port_dc -> gpio_open(0x10);  //AD3[4]
    port_dc -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x10);  //设置为输出
   
    port_clk = gpio_get_dev(DFSS_GPIO_8B3_ID);  //定义AD0端口
    port_clk -> gpio_open(0x80);  //AD0[7]
    port_clk -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x80);  //设置为输出
	
	port_mosi = gpio_get_dev(DFSS_GPIO_8B3_ID);  //定义AD1端口
    port_mosi -> gpio_open(0x40);  //AD1[6]
    port_mosi -> gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)0x40);  //设置为输出
	
	OLED_CS_Clr();  //打开片选使能
	OLED_RST_Clr();
	delay_ms(20);
	OLED_RST_Set();
	delay_ms(20);
	
//************* Start Initial Sequence **********// 
    LCD_WR_REG(0xCF);  
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x81); /////a2
    LCD_WR_DATA8(0X30); /////f0
 
    LCD_WR_REG(0xED);  
    LCD_WR_DATA8(0x64); /////55
    LCD_WR_DATA8(0x03); /////01
    LCD_WR_DATA8(0X12); /////23
    LCD_WR_DATA8(0X81); /////01
 
    LCD_WR_REG(0xE8);  
    LCD_WR_DATA8(0x85); 
    LCD_WR_DATA8(0x10); 
    LCD_WR_DATA8(0x78); 
 
    LCD_WR_REG(0xCB);  
    LCD_WR_DATA8(0x39); 
    LCD_WR_DATA8(0x2C); 
    LCD_WR_DATA8(0x00); 
    LCD_WR_DATA8(0x34); 
    LCD_WR_DATA8(0x02); 
 
    LCD_WR_REG(0xF7);  
    LCD_WR_DATA8(0x20); 
 
    LCD_WR_REG(0xEA);  
    LCD_WR_DATA8(0x00); /////66
    LCD_WR_DATA8(0x00); 
    LCD_WR_REG(0xB1);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x1B);
    LCD_WR_REG(0xB6);
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0xA2);
    LCD_WR_REG(0xC0);    //Power control 
    LCD_WR_DATA8(0x35);   //VRH[5:0] 
 
    LCD_WR_REG(0xC1);    //Power control 
    LCD_WR_DATA8(0x11);   //SAP[2:0];BT[3:0] 
 
    LCD_WR_REG(0xC5);    //VCM control 
    LCD_WR_DATA8(0x45); 
    LCD_WR_DATA8(0x45); 
 
    LCD_WR_REG(0xC7);    //VCM control2 
    LCD_WR_DATA8(0XA2); 
 
        LCD_WR_REG(0xF2);
        LCD_WR_DATA8(0x00);
        LCD_WR_REG(0x26);
        LCD_WR_DATA8(0x01);
        LCD_WR_REG(0xE0);
        LCD_WR_DATA8(0x0F);
        LCD_WR_DATA8(0x26);
        LCD_WR_DATA8(0x24);
        LCD_WR_DATA8(0x0B);
        LCD_WR_DATA8(0x0E);
        LCD_WR_DATA8(0x09);
        LCD_WR_DATA8(0x54);
        LCD_WR_DATA8(0xA8);
        LCD_WR_DATA8(0x46);
        LCD_WR_DATA8(0x0C);
        LCD_WR_DATA8(0x17);
        LCD_WR_DATA8(0x09);
        LCD_WR_DATA8(0x0F);
        LCD_WR_DATA8(0x07);
        LCD_WR_DATA8(0x00);
        LCD_WR_REG(0xE1);
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x19);
        LCD_WR_DATA8(0x1B);
        LCD_WR_DATA8(0x04);
        LCD_WR_DATA8(0x10);
        LCD_WR_DATA8(0x07);
        LCD_WR_DATA8(0x2A);
        LCD_WR_DATA8(0x47);
        LCD_WR_DATA8(0x39);
        LCD_WR_DATA8(0x03);
        LCD_WR_DATA8(0x06);
        LCD_WR_DATA8(0x06);
        LCD_WR_DATA8(0x30);
        LCD_WR_DATA8(0x38);
        LCD_WR_DATA8(0x0F);
        LCD_WR_REG(0x36);     //set the model of scanning 
        LCD_WR_DATA8((1<<5)|(1<<6));
	    LCD_WR_REG(0x2B);     //set the page address 横屏幕设置
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0xEF);
        LCD_WR_REG(0x2A);    //set the column address
		LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x00);
        LCD_WR_DATA8(0x01);
        LCD_WR_DATA8(0x3F);
		LCD_WR_REG(0x3A);
        LCD_WR_DATA8(0x55); //16pixel
        LCD_WR_REG(0x11);
 
    LCD_WR_REG(0x11);    //Exit Sleep 
    delay_ms(120); 
    LCD_WR_REG(0x29);    //Display on 
}

//清屏函数
//Color:要清屏的填充色
void LCD_Clear(uint16_t Color)
{
	uint16_t i,j;  	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	LCD_WR_DATA(Color);	 			 
	    }

	  }
}

//画点
//POINT_COLOR:此点的颜色     注意此处的point_color
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	Address_set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(POINT_COLOR); 	    
}
