#include "embARC.h"
#include "embARC_debug.h"

#include "seg.h"

#define ov7670_IIC_ID 	DFSS_IIC_0_ID
#define PIN_VSYNC 0
#define PIN_PCLK 1
#define PIN_DATA 0xff

DEV_GPIO_PTR clk_ptr = NULL;
DEV_GPIO_PTR data_ptr = NULL;
DEV_GPIO_PTR read_ptr = NULL;
DEV_UART *console_uart;

static uint8_t image[2][32][32];
static uint8_t images[10][32][32];
static uint8_t* out;

static void captureImg(int i,uint8_t* p0,uint8_t* p1);
static void captureImgs(int n);

/* image global data */

static uint8_t img[2];

int main(void) {
    console_uart = uart_get_dev(CONSOLE_UART_ID);

    clk_ptr = gpio_get_dev(DFSS_GPIO_4B2_ID);
    //io_arduino_config(ARDUINO_PIN_0, ARDUINO_GPIO, IO_PINMUX_ENABLE); // vsync
    io_arduino_config(ARDUINO_PIN_1, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // oe
    io_arduino_config(ARDUINO_PIN_2, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // wen
    io_arduino_config(ARDUINO_PIN_3, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // wrst
	/*if (clk_ptr->gpio_open((1 << 0)) == E_OPNED) {
		clk_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
								(void *)(1 << PIN_VSYNC));
	}*/
    if (clk_ptr->gpio_open(1<<1) == E_OPNED ){
		clk_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(1<<1));
	}
    if (clk_ptr->gpio_open(1<<2) == E_OPNED ){
    	clk_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(1<<2));
    }
    if (clk_ptr->gpio_open(1<<3) == E_OPNED ){
        clk_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(1<<3));
        }

    data_ptr = gpio_get_dev(DFSS_GPIO_8B2_ID);
    io_arduino_config(ARDUINO_PIN_4, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d0
    io_arduino_config(ARDUINO_PIN_5, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d1
    io_arduino_config(ARDUINO_PIN_6, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d2
    io_arduino_config(ARDUINO_PIN_7, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d3
    io_arduino_config(ARDUINO_PIN_8, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d4
    io_arduino_config(ARDUINO_PIN_9, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d5
    io_arduino_config(ARDUINO_PIN_10, ARDUINO_GPIO, IO_PINMUX_ENABLE); // d6
    io_arduino_config(ARDUINO_PIN_11, ARDUINO_GPIO, IO_PINMUX_ENABLE); // d7
	if (data_ptr->gpio_open(1<<0) == E_OPNED ){
		data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(1<<0));
	}
	if (data_ptr->gpio_open(1<<1) == E_OPNED ){
		data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(1<<1));
		}
	if (data_ptr->gpio_open(1<<2) == E_OPNED ){
		data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(1<<2));
		}
	if (data_ptr->gpio_open(1<<3) == E_OPNED ){
		data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(1<<3));
		}
	if (data_ptr->gpio_open(1<<4) == E_OPNED ){
		data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(1<<4));
		}
	if (data_ptr->gpio_open(1<<5) == E_OPNED ){
		data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(1<<5));
		}
	if (data_ptr->gpio_open(1<<6) == E_OPNED ){
		data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(1<<6));
		}
	if (data_ptr->gpio_open(1<<7) == E_OPNED ){
		data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,(void *)(1<<7));
		}

    read_ptr = gpio_get_dev(DFSS_GPIO_4B1_ID);//DFSS_GPIO_8B3_ID  // rck  // rrst
    if (read_ptr->gpio_open(1<<0) == E_OPNED ){
		read_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(1<<0));
	}
    if (read_ptr->gpio_open(1<<1) == E_OPNED ){
    	read_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,(void *)(1<<1));
    	}
/*    ov7670_sensor_init();
    board_delay_ms(1500, 1);*/
    clk_ptr->gpio_write(0<< 1 , 1 << 1);

	while(1){
		captureImgs(10);
		process(images);
	}
    return E_SYS;
}

static void captureImgs(int n) {
    //captureImg(void);
	out = images;
	uint8_t flag=0;
	uint8_t* image0 = image+flag;
	flag = 1-flag;
	uint8_t* image1 = image+flag;
	captureImg(0,image0,image1);
	for(int i = 0; i < n; ++i){
		image0 = image+flag;
		flag = 1-flag;
		image1 = image+flag;
		captureImg(1,image0,image1);
	}


}



static void captureImg(int i,uint8_t* p0,uint8_t* p1) {
	uint8_t temp;
    while(!(_arc_aux_read(0x80017e50) & (1 << PIN_VSYNC)));
    while((_arc_aux_read(0x80017e50) & (1 << PIN_VSYNC)));
    clk_ptr->gpio_write(0<< 3 , 1 << 3);
    clk_ptr->gpio_write(1<< 3 , 1 << 3);

    clk_ptr->gpio_write(1<< 2 , 1 << 2); //wr
    if(i){
    	for(int cnt = 0; cnt < 1024; ++cnt){

    		temp = p1[cnt]-p0[cnt]+40;
    		if(temp<80){
    			*out++ = 0;
    		}
    		else{
    			*out++ = temp;
    		}


    	}

    }
    while(!(_arc_aux_read(0x80017e50) & (1 << PIN_VSYNC)));
    while((_arc_aux_read(0x80017e50) & (1 << PIN_VSYNC)));
    clk_ptr->gpio_write(0<< 2 , 1 << 2);


    read_ptr->gpio_write(0<< 1 , 1 << 1);  //FIFO_RRST, LOW
    read_ptr->gpio_write(0<< 0 , 1 << 0);  //FIFO_RCK, LOW
    read_ptr->gpio_write(1<< 0 , 1 << 0);  //FIFO_RCK, HIGH
    read_ptr->gpio_write(0<< 0 , 1 << 0);  //FIFO_RCK, LOW
    read_ptr->gpio_write(1<< 1 , 1 << 1);  //FIFO_RRST, HIGH
    read_ptr->gpio_write(1<< 0 , 1 << 0);  //FIFO_RCK, HIGH

    uint8_t w[]={0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155};
    uint8_t h[]={0,4,8,11,15,19,23,26,30,34,38,41,45,49,53,56,60,64,68,71,75,79,83,86,90,94,98,101,105,109,113,116};
    uint8_t* hPtr = h;
    for(int y = 0; y < 120; ++y) {

    	if(y == *hPtr){
    		hPtr++;
    		uint8_t* wPtr = w;
    		for(int x = 0; x < 160; ++x) {

    		    if(x == *wPtr){
    		    	wPtr++;
    		    	read_ptr->gpio_write(0<< 0 , 1 << 0);
    		    	*p1++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);
    		    	//EMBARC_PRINTF("%x",(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA));
    		    	read_ptr->gpio_write(1<< 0 , 1 << 0);
    		    	read_ptr->gpio_write(0<< 0 , 1 << 0);
    		    	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);
    		    	read_ptr->gpio_write(1<< 0 , 1 << 0);

    		    	//console_uart->uart_write((void*) p1++, 1);
    		    }
    		    else{
    		    	read_ptr->gpio_write(0<< 0 , 1 << 0);
    		    	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);
    		    	read_ptr->gpio_write(1<< 0 , 1 << 0);
    		    	read_ptr->gpio_write(0<< 0 , 1 << 0);
    		    	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);
    		    	read_ptr->gpio_write(1<< 0 , 1 << 0);
    		    }
    		}
    	}
    	else{
    		for(int x = 0; x < 160; ++x) {
    		read_ptr->gpio_write(0<< 0 , 1 << 0);
    		(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);
    		read_ptr->gpio_write(1<< 0 , 1 << 0);
    		read_ptr->gpio_write(0<< 0 , 1 << 0);
    		(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);
    		read_ptr->gpio_write(1<< 0 , 1 << 0);
    		}
    	}
    }
    /*uint8_t he[]={0x01,0xFE};
    uint8_t ta[]={0xFE,0x01};
    console_uart->uart_write((void*) he, 2);
    console_uart->uart_write((void*) (p0), 1024);
    console_uart->uart_write((void*) ta, 2);*/
}



