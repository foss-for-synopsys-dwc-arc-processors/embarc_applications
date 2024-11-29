/***************************GPIO Libraries*******************************/
#include "embARC.h"
#include "embARC_debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LCD.h"

/***************************FOR Console UART1****************************/
#define PC_UART_ID			DW_UART_1_ID	//Console ID
/***************************FOR Pmod1 Linkit 7688 DUO UART0**************/
#define LINKIT_UART_ID			DW_UART_0_ID	//Linkit 7688 DUO ID
/***************************FOR GPIO*************************************/
#define EMSK_GPIO_PORT_B_ID		DW_GPIO_PORT_B  //GPIO-B ID
#define BOARD_LED_MASK			(0x1ff)
/***************************FOR LCD*************************************/
#define LCD_IIC_PORT_PM4		DW_IIC_1_ID
#define LCD_Slave_Address        0x48
#define MAX_LCD_COL  16
/***************************For mux**************************************/
#include "../../board/emsk/drivers/mux/mux.h"
/***************************FOR Pmod3 4*4 keyboard***********************/
#define J3_PIN7to10_MASK_A      0xf0000
#define J3_PIN1to4_MASK_C       0xf0000
#define J56_MASK_A            	0x1f000000
#define J5_PIN1to4_MASK_C     	0xf000000
#define keyboard_row_ID			DW_GPIO_PORT_C	//keyboard_row_ID
#define keyboard_col_ID			DW_GPIO_PORT_A	//keyboard_col_ID
/***************************FOR Debounce*********************************/
#define false			   0
#define true  			   1
#define debounce_start_on  2
#define debounce_start_off 3
typedef struct _button_status{
	int ispress;
	uint64_t press_time;
	uint64_t release_time;
} button_status;
const static char keyboard_Convert[] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
	'*', '0', '#', 'D',
	'E','F'
};
/***************************User Function********************************/
#define Book_ID_Length  2
#define location_length 3
#define Func_ID_Length  2
#define User_ID_Length  5
#define CMD_Length 9//(FUNC)+(BOOKID)+(USERID)+Z
#define Search 		3  //A
#define Borrow 		7  //B
#define Giveback 	11 //C
#define help		12
#define Reservation 15 //D
#define NewReservation 16
#define CancelReservation 17
#define New_Reservation 3    //A
#define Cancel_Reservation 7 //B

/***************************Fun Declaration******************************/
int btn(void);
void device_setup(void);
void mux_setup(void);
void sendcmd(char cmd[], int function);
void rev(int operation);
