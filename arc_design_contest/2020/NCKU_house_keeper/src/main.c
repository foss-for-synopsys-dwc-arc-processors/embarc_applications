/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */


#include "embARC.h"
#include "embARC_debug.h"
#include "sccb.h"
#include "ov7670.h"
#include <stdlib.h>
#include "detect.h"

#include "NCKU_facedetection.h"

#define PIN_VSYNC 0
#define PIN_PCLK 1
#define PIN_DATA 0xff

#define READ_5  while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);

#define READ_6  while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	*readPtr++ = (uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);

#define NOREAD_5  while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);

#define NOREAD_6  while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);\
	while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));\
	while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK))); \
	(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);





DEV_GPIO_PTR clk_ptr = NULL;
DEV_GPIO_PTR data_ptr = NULL;
//static int cnt = 0;



static void captureImg(uint16_t width, uint16_t height);

/* image global data */
static uint8_t image[64 * 64 * 3];
//static uint8_t image2[32*32*3];

DEV_UART *console_uart;
int main(void)
{
    // config SCCB
    sccbInit(0);
    console_uart = uart_get_dev(CONSOLE_UART_ID);
    console_uart->uart_open(1500000);

    // config PWM
    io_arduino_config(ARDUINO_PIN_3, ARDUINO_PWM, IO_PINMUX_ENABLE);//pwm timer ch0
    DEV_PWM_TIMER_CFG ch0_pwm_cfg;
    ch0_pwm_cfg.mode = DEV_PWM_TIMER_MODE_PWM;
    ch0_pwm_cfg.count_high = 2;
    ch0_pwm_cfg.count_low = 2;
    ch0_pwm_cfg.isr_hander = NULL;
    DEV_PWM_TIMER_PTR pwm_timer_test = pwm_timer_get_dev(DW_PWM_TIMER_0_ID);
    pwm_timer_test->pwm_timer_open();
    pwm_timer_test->pwm_timer_control(0, PWM_TIMER_CMD_SET_CFG, (void *)(&ch0_pwm_cfg));

    board_delay_ms(3000, 1);

    // other pins
    clk_ptr = gpio_get_dev(DFSS_GPIO_4B2_ID);

    io_arduino_config(ARDUINO_PIN_AD0, ARDUINO_GPIO, IO_PINMUX_ENABLE); // vsync
    if(clk_ptr->gpio_open((1 << PIN_VSYNC)) == E_OPNED) {
        clk_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                              (void *)(1 << PIN_VSYNC));
    }

    io_arduino_config(ARDUINO_PIN_AD1, ARDUINO_GPIO, IO_PINMUX_ENABLE); // pclk
    if(clk_ptr->gpio_open((1 << PIN_PCLK)) == E_OPNED) {
        clk_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                              (void *)(1 << PIN_PCLK));
    }

    io_arduino_config(ARDUINO_PIN_4, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d0
    io_arduino_config(ARDUINO_PIN_5, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d1
    io_arduino_config(ARDUINO_PIN_6, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d2
    io_arduino_config(ARDUINO_PIN_7, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d3
    io_arduino_config(ARDUINO_PIN_8, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d4
    io_arduino_config(ARDUINO_PIN_9, ARDUINO_GPIO, IO_PINMUX_ENABLE);  // d5
    io_arduino_config(ARDUINO_PIN_10, ARDUINO_GPIO, IO_PINMUX_ENABLE); // d6
    io_arduino_config(ARDUINO_PIN_11, ARDUINO_GPIO, IO_PINMUX_ENABLE); // d7
    data_ptr = gpio_get_dev(DFSS_GPIO_8B2_ID);
    if(data_ptr->gpio_open(1 << 0) == E_OPNED &&
       data_ptr->gpio_open(1 << 1) == E_OPNED &&
       data_ptr->gpio_open(1 << 2) == E_OPNED &&
       data_ptr->gpio_open(1 << 3) == E_OPNED &&
       data_ptr->gpio_open(1 << 4) == E_OPNED &&
       data_ptr->gpio_open(1 << 5) == E_OPNED &&
       data_ptr->gpio_open(1 << 6) == E_OPNED &&
       data_ptr->gpio_open(1 << 7) == E_OPNED) {
        data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                               (void *)(1 << 0));
        data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                               (void *)(1 << 1));
        data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                               (void *)(1 << 2));
        data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                               (void *)(1 << 3));
        data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                               (void *)(1 << 4));
        data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                               (void *)(1 << 5));
        data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                               (void *)(1 << 6));
        data_ptr->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
                               (void *)(1 << 7));
    }

    camInit();
    setRes(QQVGA);
    setColorSpace(RGB565);

    writeReg(0x11, 9);

    while(1) {
        captureImg(160 * 2, 120);

        uint8_t ans;
        ans = (uint8_t)mli_face_detection(image);
        console_uart->uart_write((void*)ans, 1);
        //printf("\npixel:%d\n",image[500]);
    }
}


static void captureImg(uint16_t width, uint16_t height)
{

    EMBARC_PRINTF("RDY");
    uint8_t buf[128];

    while(!(_arc_aux_read(0x80017e50) & (1 << PIN_VSYNC)));
    while((_arc_aux_read(0x80017e50) & (1 << PIN_VSYNC)));

    uint8_t* imagePtr = image;
    for(int h = 0; h < 28; ++h) {
        for(int w = 0; w < 320; ++w) {
            while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));
            while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));
            //(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);
        }
    }


    for(int h = 28; h < 92; ++h) {
        uint8_t* readPtr = buf;
        uint8_t* writePtr = buf;
        for(int w = 0; w < 96; ++w) {
            while(!(_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));
            while((_arc_aux_read(0x80017e50) & (1 << PIN_PCLK)));
            //(uint8_t)(_arc_aux_read(0x80017a50) & PIN_DATA);
        }
        for(int w = 0; w < 8; ++w) {
            READ_5;

            *imagePtr = ((*writePtr) >> 3);
            console_uart->uart_write((void*) imagePtr++, 1);

            READ_5;

            *imagePtr = (((*writePtr++) & 7) << 3);
            *imagePtr = (*imagePtr)  + ((*writePtr) >> 5);
            console_uart->uart_write((void*) imagePtr++, 1);

            READ_6;

            *imagePtr = (*writePtr++) & 31;
            console_uart->uart_write((void*) imagePtr++, 1);
        }
        for(int w = 0; w < 3; ++w) {
            NOREAD_5;

            *imagePtr = ((*writePtr) >> 3);
            console_uart->uart_write((void*) imagePtr++, 1);

            NOREAD_5;

            *imagePtr = (((*writePtr++) & 7) << 3);
            *imagePtr = (*imagePtr)  + ((*writePtr) >> 5);
            console_uart->uart_write((void*) imagePtr++, 1);

            NOREAD_6;

            *imagePtr = (*writePtr++) & 31;
            console_uart->uart_write((void*) imagePtr++, 1);
        }
        for(int i = 0; i < 53; ++i) {
            *imagePtr = ((*writePtr) >> 3);
            console_uart->uart_write((void*) imagePtr++, 1);

            *imagePtr = (((*writePtr++) & 7) << 3);
            *imagePtr = (*imagePtr)  + ((*writePtr) >> 5);
            console_uart->uart_write((void*) imagePtr++, 1);

            *imagePtr = (*writePtr++) & 31;
            console_uart->uart_write((void*) imagePtr++, 1);
        }
    }

}
