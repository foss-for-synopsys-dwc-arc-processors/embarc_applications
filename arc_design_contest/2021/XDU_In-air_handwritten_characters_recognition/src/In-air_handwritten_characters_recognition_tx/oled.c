#include "string.h"
#include "stdio.h"
#include "embARC.h"
#include "embARC_debug.h"
#include "embARC_error.h"
#include "board.h"
#include "dev_iic.h"
#include "dfss_iic_obj.h"
#include "oled.h"
#include "oledfont.h"
#include "dev_gpio.h"
#include "dfss_gpio_obj.h"
#include "oled.h"
#include "dev_gpio.h"
#include "bmp.h"


#define I2C_SLAVE_OLED_ADDR 0x3c
#define I2C_SLAVE_OLED_ID 0
DEV_IIC *dev_iic_oled = NULL;     //set a pointer of oled_iic
DEV_GPIO *oled_reset_port = NULL; //set a pointer of oled_reset

//define oled reset port gpoi define
#define OLED_RESET_OFFSET (0)
#define OLED_RESET_MASK ((0x04) << OLED_RESET_OFFSET) //bit2
#define OLED_RESET_PORT DFSS_GPIO_4B2_ID              //io_gpio_4b2
#define OLED_RESET_DIR ((0x1) << OLED_RESET_OFFSET)   //input


/*****************************************************************************************
 * \brief  iot oled reset port configuration
 *
 * \param[in] void, interrupt number
 * \return 0 ok, -1 error
 *****************************************************************************************/
// void iot_oled_reset(void)
// {
//     oled_reset_port = gpio_get_dev(OLED_RESET_PORT);     //get gpio port

//     if (oled_reset_port->gpio_open(OLED_RESET_DIR) == E_OPNED)//open gpio port
//         {
//             oled_reset_port->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(OLED_RESET_MASK));
//             oled_reset_port->gpio_control(GPIO_CMD_DIS_BIT_INT, (void *)(OLED_RESET_MASK));
//         }

//     error_exit:
//         return;        
// }

//-----------------------------------------------------------------------------------
//      Delay function
//-----------------------------------------------------------------------------------
static void delay_ms(volatile int z) //delay for 1ms
{
    volatile uint32_t x, y;
    for (x = 1400; x > 0; x--)
        for (y = z; y > 0; y--)
            ;
}

//-----------------------------------------------------------------------------------
//      OLED display function
//-----------------------------------------------------------------------------------
uint8_t t = ' '; 
void OLED_Display()
{
        delay_ms(500);
        OLED_Clear();
    

        t++;
        if (t > '~')
            t = ' ';
        OLED_ShowNum(103, 6, t, 3, 16);
        delay_ms(5000);
        OLED_Clear();
}

//-----------------------------------------------------------------------------------
//      OLED IIC port configuration and initialization
//-----------------------------------------------------------------------------------
void OLED_Configuration(void)
{
    
    dev_iic_oled = iic_get_dev(I2C_SLAVE_OLED_ID);
    // EMBARC_PRINTF("iic open status = %d\n",dev_iic_oled->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD));
    // EMBARC_PRINTF("iic control status = %d\n",dev_iic_oled->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_OLED_ADDR)));    

    OLED_Init();         //Initialize OLED
    OLED_ColorTurn(0);   //0 normal display, 1 reverse color display
    OLED_DisplayTurn(0); //0 Normal display 1 Reversed display    
    		EMBARC_PRINTF("OLED INITIAL  OK!\n\r");

}

//-----------------------------------------------------------------------------------
//      OLED initialization
//-----------------------------------------------------------------------------------
void OLED_Init(void)
{

    // OLED_RES_Clr(); //Reset oled, first pull it down for 200ms and then pull it up
    // delay_ms(200);
    // OLED_RES_Set();

    OLED_WR_Byte(0xFD, OLED_CMD); // 2B data -0xFD set OLED
    OLED_WR_Byte(0x12, OLED_CMD); // Unlock oled driver ic
    OLED_WR_Byte(0xAE, OLED_CMD); // --turn off oled pane
    OLED_WR_Byte(0xd5, OLED_CMD); // --set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0xA0, OLED_CMD); // Clock, division factor
    OLED_WR_Byte(0xA8, OLED_CMD); // --set multiplex ratio(1 to 64)(The number of lines at the end of the screen display)
    OLED_WR_Byte(0x3f, OLED_CMD); // --1/64 duty .Full screen multiplexing
    OLED_WR_Byte(0xD3, OLED_CMD); // -set display offset	Shift Mapping RAM Counter (0x00~0x3F)(The screen moves up the number of lines
    OLED_WR_Byte(0x00, OLED_CMD); // -not offset
    OLED_WR_Byte(0x40, OLED_CMD); // --set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0xA1, OLED_CMD); // --Set SEG/Column Mapping     									0xa0 left and right reversed 0xa1 normal
    OLED_WR_Byte(0xC8, OLED_CMD); // Set COM/Row Scan Direction   									0xc0 up and down reversed 0xc8 normal
    OLED_WR_Byte(0xDA, OLED_CMD); // --set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD); //
    OLED_WR_Byte(0x81, OLED_CMD); // --set contrast control register									
    OLED_WR_Byte(0x7F, OLED_CMD); // Set SEG Output Current Brightness								Medium contrast
    OLED_WR_Byte(0xD9, OLED_CMD); // --set pre-charge period											
    OLED_WR_Byte(0x82, OLED_CMD); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDB, OLED_CMD); // --set vcomh														
    OLED_WR_Byte(0x34, OLED_CMD); // Set VCOM Deselect Level
    OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)/All bright
    OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)/Inverted color
    OLED_Clear();                 // Clear screen
    OLED_WR_Byte(0xAF, OLED_CMD); // open oled
}


//-----------------------------------------------------------------------------------
//      Write a byte to SSD1306.
//-----------------------------------------------------------------------------------
//mode:Data/command flag    0, means command; 1, means data;
//-----------------------------------------------------------------------------------
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
    // EMBARC_PRINTF("iic open status = %d\n",dev_iic_oled->iic_open(DEV_MASTER_MODE, IIC_SPEED_STANDARD));
    // EMBARC_PRINTF("iic control status = %d\n",dev_iic_oled->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_OLED_ADDR)));  
    dev_iic_oled->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(I2C_SLAVE_OLED_ADDR));
    char text[2];
    if (mode)
    {
        text[0] = 0x40;
    }
    else
    {
        text[0] = 0x00;
    }
    text[1] = dat;
    dev_iic_oled->iic_write(text, 2);
    // EMBARC_PRINTF("iic write status = %d\n",dev_iic_oled->iic_write(text, 2));
    // I2C_Start();     //Start signal
    // Send_Byte(0x78); //Send 1B slave address
    // I2C_WaitAck();   //Waiting for response
    // if (mode)
    // {
    //     Send_Byte(0x40); //Representative will send data
    // }
    // else
    // {
    //     Send_Byte(0x00); //The representative will send instructions
    // }
    // I2C_WaitAck();
    // Send_Byte(dat);
    // I2C_WaitAck();
    // I2C_Stop();
}

//-----------------------------------------------------------------------------------
//      Clear screen
//-----------------------------------------------------------------------------------
void OLED_Clear(void)
{   
    // OLED_Configuration();
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //Set page address (0~7£©
        OLED_WR_Byte(0x00, OLED_CMD);     //Set display position-column low address
        OLED_WR_Byte(0x10, OLED_CMD);     //Set display position-column high address
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(0, OLED_DATA);
    } //Update display
}

//-----------------------------------------------------------------------------------
//      Reverse display
//-----------------------------------------------------------------------------------
void OLED_ColorTurn(uint8_t i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xA6, OLED_CMD); //normal display
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xA7, OLED_CMD); //Inverted display
    }
}

//-----------------------------------------------------------------------------------
//      Rotate the screen 180 degrees
//-----------------------------------------------------------------------------------
void OLED_DisplayTurn(uint8_t i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD); //normal display
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xC0, OLED_CMD); //Inverted display
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

//-----------------------------------------------------------------------------------
//      display image
//-----------------------------------------------------------------------------------
//x-starting column coordinates, y starting page coordinates, sizex, sizey, picture length and width, BMP: picture to be displayed
//-----------------------------------------------------------------------------------
void OLED_DrawBMP(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[])
{
    uint16_t j = 0;
    uint8_t i, m;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0); //Determine how many pages are needed
    for (i = 0; i < sizey; i++)                //
    {                                          //
        OLED_Set_Pos(x, i + y);                //Set the starting page, starting column high and low address
        for (m = 0; m < sizex; m++)
        {
            OLED_WR_Byte(BMP[j++], OLED_DATA); //Write picture
        }
    }
}
//-----------------------------------------------------------------------------------
//      Display Chinese characters
//-----------------------------------------------------------------------------------
//x-starting column coordinate, y-starting page coordinate, no-number in font, sizey-longitudinal length of character (font size)
//-----------------------------------------------------------------------------------
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no, uint8_t sizey)
{
    uint16_t i, size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    for (i = 0; i < size1; i++)
    {
        if (i % sizey == 0)
            OLED_Set_Pos(x, y++);
        if (sizey == 16)
            OLED_WR_Byte(Hzk[no][i], OLED_DATA); //16x16 font size
                                                 //		else if(sizey==xx) OLED_WR_Byte(xxx[c][i],OLED_DATA);//User add font size
        else
            return;
    }
}

//-----------------------------------------------------------------------------------
//      Display a character string
//-----------------------------------------------------------------------------------
//x-starting column coordinates, y starting page coordinates, chr-character pointer, sizey-longitudinal length
//-----------------------------------------------------------------------------------
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey)
{   
    // OLED_Configuration();
    uint8_t j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j++], sizey);
        if (sizey == 8)
            x += 6;
        else
            x += sizey / 2;
        
	// EMBARC_PRINTF("OLED_ShowString = %s\r\n",chr);    
    }
}

//-----------------------------------------------------------------------------------
//      Display a character at the specified position, including some characters
//-----------------------------------------------------------------------------------
//x:0~127£¬y:0~63£¬sizey:Choose font 6x8  8x16
//-----------------------------------------------------------------------------------
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t sizey)
{
    uint8_t c = 0, sizex = sizey / 2;
    uint16_t i = 0, size1;
    if (sizey == 8)
        size1 = 6;
    else
        size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * (sizey / 2);
    c = chr - ' '; //Get the offset value
    OLED_Set_Pos(x, y);
    for (i = 0; i < size1; i++)
    {
        if (i % sizex == 0 && sizey != 8)
            OLED_Set_Pos(x, y++);
        if (sizey == 8)
            OLED_WR_Byte(asc2_0806[c][i], OLED_DATA); //6X8 font size
        else if (sizey == 16)
            OLED_WR_Byte(asc2_1608[c][i], OLED_DATA); //8x16 font size
                                                      //		else if(sizey==xx) OLED_WR_Byte(asc2_xxxx[c][i],OLED_DATA);//User add font size
        else
            return;
    }
}
//-----------------------------------------------------------------------------------
//      Calculate m^n
//-----------------------------------------------------------------------------------
uint32_t oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}

//-----------------------------------------------------------------------------------
//      Display number
//-----------------------------------------------------------------------------------
//x,y: starting point coordinates, num: number to be displayed, len: number of digits, sizey: font size
//-----------------------------------------------------------------------------------
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey)
{
    uint8_t t, temp, m = 0;
    uint8_t enshow = 0;
    if (sizey == 8)
        m = 2;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10; //Take out the tens and hundreds of numbers
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (sizey / 2 + m) * t, y, ' ', sizey);
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (sizey / 2 + m) * t, y, temp + '0', sizey);
    }
}

//-----------------------------------------------------------------------------------
//      Coordinate settings
//-----------------------------------------------------------------------------------
//x-starting column, y-starting page
//-----------------------------------------------------------------------------------
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);				  //Set write start page
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD); //Set the write start column high address (extract the high 4 bits, then shift 4 bits to the right, and set the high four bits to 1)
	OLED_WR_Byte((x & 0x0f), OLED_CMD);				  //Set the write start column low address
}
