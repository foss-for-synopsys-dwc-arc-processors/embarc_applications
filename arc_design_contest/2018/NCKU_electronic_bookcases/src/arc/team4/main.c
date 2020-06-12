#include "main.h"

static DEV_UART *Console;
static DEV_UART *linkit;
static DEV_GPIO *led;
static DEV_IIC *LCD;

static button_status keyboard_table[4][4] = { 0 };
static DEV_GPIO * keyboard_row;
static DEV_GPIO * keyboard_col;
static DEV_GPIO * port_C_led;
static DEV_GPIO * port_A_led;
static char *msg;

/** main entry */
int main(void) {
	//set up device
	device_setup();
	board_delay_ms(2000, 1);
	Lcd_ClearScreen();
	Lcd_DisplayString(0, 0, "A(Search)");
	Lcd_DisplayString(1, 0, "B(Borrow)");
	board_delay_ms(2000, 1);
	Lcd_ClearScreen();
	Lcd_DisplayString(0, 0, "C(Return)");
	Lcd_DisplayString(1, 0, "D(Reservation)");

	while (1) {
		board_delay_ms(2000, 1);
		Lcd_ClearScreen();
		port_C_led->gpio_write(0x0, J5_PIN1to4_MASK_C);
		port_A_led->gpio_write(0x0, J56_MASK_A);
		Lcd_DisplayString(0, 0, "Select Function\r\n");
		Lcd_DisplayString(1, 0, "A,B,C,D   *:help");

		/*we will  use the string below later*/
		char cmd[CMD_Length] = { 0 };
		for (int base = 0; base < CMD_Length; base++) {
			cmd[base] = ' ';
		} //Your ID:________
		cmd[CMD_Length - 1] = 'Z';

		char book[16] = "BOOK ID:";
		for (int base = 0; base < Book_ID_Length; base++) {
			book[8 + base] = '_';
		} //BOOK ID:________

		char user[16] = "Your ID:";
		for (int base = 0; base < User_ID_Length; base++) {
			user[8 + base] = '_';
		} //Your ID:________

		char location[16] = "Location:";
		for (int base = 0; base < location_length; base++) {
			location[9 + base] = '_';
		} //Location:________

		/*GET WHAT USER WANT TO DO*/
		int function = btn();
		switch (function) {
		case Search: {
			Lcd_DisplayString(0, 0, "Search \n\r");
			Lcd_DisplayString(1, 0, book);

			/*Enter Book ID*/
			for (int count = 0; count < Book_ID_Length; count++) {
				int input = btn();
				cmd[count + 1] = keyboard_Convert[input];
				book[8 + count] = keyboard_Convert[input];
				Lcd_DisplayString(1, 0, book);
			}
			sendcmd(cmd, function);
			/*get response*/
			rev(function);
			break;
		}
		case Borrow: {
			Lcd_DisplayString(0, 0, "Borrow \n\r");
			Lcd_DisplayString(1, 0, book);
			/*Enter Book ID*/
			for (int count = 0; count < Book_ID_Length; count++) {
				int input = btn();
				cmd[count + 1] = keyboard_Convert[input];
				book[8 + count] = keyboard_Convert[input];
				Lcd_DisplayString(1, 0, book);
			}

			/*Enter User ID*/
			Lcd_DisplayString(0, 0, user);
			for (int count = 0; count < User_ID_Length; count++) {
				int input = btn();
				cmd[count + Book_ID_Length + 1] = keyboard_Convert[input];
				user[8 + count] = keyboard_Convert[input];
				Lcd_DisplayString(0, 0, user);
			}
			sendcmd(cmd, function);
			/*get response*/
			rev(function);
			break;
		}
		case Giveback: {
			Lcd_DisplayString(0, 0, "Giveback \n\r");
			Lcd_DisplayString(1, 0, book);
			/*Enter Book ID*/
			for (int count = 0; count < Book_ID_Length; count++) {
				int input = btn();
				cmd[count + 1] = keyboard_Convert[input];
				book[8 + count] = keyboard_Convert[input];
				Lcd_DisplayString(1, 0, book);
			}
			/*Enter Location*/
			Lcd_DisplayString(0, 0, location);
			for (int count = 0; count < location_length; count++) {
				if (count == 1) {
					cmd[count + Book_ID_Length + 1] = '-';
					location[10] = '-';
					Lcd_DisplayString(0, 0, location);
				} else {
					int input = btn();
					if (count == 0) {
						location[9] = keyboard_Convert[input];
					} else {
						location[11] = keyboard_Convert[input];
					}
					cmd[count + Book_ID_Length + 1] = keyboard_Convert[input];
					Lcd_DisplayString(0, 0, location);
				}
			}
			sendcmd(cmd, function);
			/*get response*/
			rev(function);
			break;
		}
		case help: {
			Lcd_ClearScreen();
			Lcd_DisplayString(0, 0, "A(Search)");
			Lcd_DisplayString(1, 0, "B(Borrow)");
			board_delay_ms(2000, 1);
			Lcd_ClearScreen();
			Lcd_DisplayString(0, 0, "C(Return)");
			Lcd_DisplayString(1, 0, "D(Reservation)");
			break;
		}
		case Reservation: {
			Lcd_DisplayString(0, 0, "A:New");
			Lcd_DisplayString(1, 0, "B:Cancel");
			do {
				int res_function = btn();
				if (res_function == New_Reservation) {
					function = NewReservation;
					Lcd_ClearScreen();
					Lcd_DisplayString(0, 0, "A:New");
					msg = "New_Reservation \n\r";
					Console->uart_write(msg, strlen(msg));
					break;
				} else if (res_function == Cancel_Reservation) {
					function = CancelReservation;
					Lcd_ClearScreen();
					Lcd_DisplayString(0, 0, "B:Cancel");
					msg = "Cancel_Reservation \n\r";
					Console->uart_write(msg, strlen(msg));
					break;
				} else {
					Lcd_DisplayString(0, 0, "A:New");
					Lcd_DisplayString(1, 0, "B:Cancel");
				}
			} while (true);
			Lcd_DisplayString(1, 0, book);
			/*Enter Book ID*/
			for (int count = 0; count < Book_ID_Length; count++) {
				int input = btn();
				cmd[count + 1] = keyboard_Convert[input];
				book[8 + count] = keyboard_Convert[input];
				Lcd_DisplayString(1, 0, book);
			}

			/*Enter User ID*/
			Lcd_DisplayString(0, 0, user);
			for (int count = 0; count < User_ID_Length; count++) {
				int input = btn();
				cmd[count + Book_ID_Length + 1] = keyboard_Convert[input];
				user[8 + count] = keyboard_Convert[input];
				Lcd_DisplayString(0, 0, user);
			}

			sendcmd(cmd, function);
			/*get response*/
			rev(function);
			break;
		}
		default:
			break;
		}
	}
}
int btn(void) {
	uint32_t data;
	uint32_t buffer;
	int i, j;
	while (1) {
		for (i = 0; i < 4; i++) {
			//scan 4 rows
			keyboard_row->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,
					(void *) (0x10000 << i));
			keyboard_row->gpio_write(0x00000000, J3_PIN1to4_MASK_C);
			board_delay_ms(1, 1);
			keyboard_col->gpio_read(&data, J3_PIN7to10_MASK_A);

			for (j = 0; j < 4; j++) {
				//scan 4 columns
				if (data & (0x10000 << j)) {
					if (keyboard_table[i][j].ispress == true) {
						keyboard_table[i][j].release_time = board_get_cur_us();
						keyboard_table[i][j].ispress = debounce_start_off;
					} else if (keyboard_table[i][j].ispress == debounce_start_on) {
						keyboard_table[i][j].ispress = false;
					} else if (keyboard_table[i][j].ispress
							== debounce_start_off) {
						if (board_get_cur_us()
								- keyboard_table[i][j].release_time
								>= 100000ul) {
							keyboard_table[i][j].ispress = false;
						}
					} else {
						keyboard_table[i][j].ispress = false;
					}
				} else {
					if (keyboard_table[i][j].ispress == false) {
						keyboard_table[i][j].press_time = board_get_cur_us();
						keyboard_table[i][j].ispress = debounce_start_on;
					} else if (keyboard_table[i][j].ispress == debounce_start_on) {
						if (board_get_cur_us() - keyboard_table[i][j].press_time
								>= 10000ul) {
							keyboard_table[i][j].ispress = true;
							//printf("PRESS %c\n", keyboard_Convert[i * 4 + j]);
							return (i * 4 + j);
						}
					} else if (keyboard_table[i][j].ispress
							== debounce_start_off) {
						keyboard_table[i][j].ispress = true;
					} else {
						keyboard_table[i][j].ispress = true;
					}
				}
			}
			keyboard_row->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
					(void *) (J3_PIN1to4_MASK_C));
		}
	}
}
void device_setup(void) {
	board_init();
	mux_setup();
	/*get device*/
	linkit = uart_get_dev(LINKIT_UART_ID);		//get linkit  pointer
	Console = uart_get_dev(PC_UART_ID);	//get console pointer
	led = gpio_get_dev(EMSK_GPIO_PORT_B_ID);	//get on-board led pointer
	LCD = iic_get_dev(LCD_IIC_PORT_PM4);
	keyboard_row = gpio_get_dev(keyboard_row_ID); //get keyboard_row pointer
	keyboard_col = gpio_get_dev(keyboard_col_ID); //get keyboard_col pointer
	port_C_led = keyboard_row;
	port_A_led = keyboard_col;

	/*LCD setup*/
	LCD->iic_open(DEV_MASTER_MODE, IIC_SPEED_HIGH);
	LCD->iic_control(IIC_CMD_MST_SET_TAR_ADDR, CONV2VOID(LCD_Slave_Address));
	Lcd_Init(LCD);
	Lcd_DisplayOnOff(1);  //display on
	Lcd_DisplayMode(1);   //wrap 16 chars
	Lcd_ClearScreen();       //clear screen
	Lcd_CursorOff();
	Lcd_DisplayString(0, 0, "Initialize...");
	Lcd_DisplayString(1, 0, "Hello My Friend");
	//open Console Device
	led->gpio_open(0x1FF);
	Console->uart_open(UART_BAUDRATE_115200);
	linkit->uart_open(UART_BAUDRATE_9600);
	led->gpio_write(0x055, 0x1FF);

	/*set up for 4*4 keyboard*/
	keyboard_row->gpio_close(); /* opened before now close it */
	keyboard_col->gpio_close(); /* opened before now close it */
	keyboard_row->gpio_open(J3_PIN1to4_MASK_C | J5_PIN1to4_MASK_C);
	keyboard_col->gpio_open(J3_PIN7to10_MASK_A | J56_MASK_A);
	keyboard_row->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
			(void *) (J3_PIN1to4_MASK_C));
	keyboard_col->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT,
			(void *) (J3_PIN7to10_MASK_A));
	keyboard_row->gpio_write(0x00000, J3_PIN1to4_MASK_C);

	/*LED SETUP*/
	//port_C_led->gpio_open(J5_PIN1to4_MASK_C);
	//port_A_led->gpio_open(J56_MASK_A);
	port_C_led->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,
			(void *) (J5_PIN1to4_MASK_C));
	port_A_led->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT,
			(void *) (J56_MASK_A));
	board_delay_ms(1, 1);
}
void mux_setup(void) {
	set_pmod_mux( PM1_UR_UART_0 | //PM1 for linkit7688DUO
			PM3_GPIO_AC |   //PM3 for LCD
			PM4_I2C_GPIO_D |   //PM4 for keyboard
			PM5_UR_GPIO_C | //PM5 for LEDS
			PM5_LR_GPIO_A | //PM5 for LEDS
			PM6_LR_GPIO_A); //PM6 for LEDS

	set_uart_map(0x9c);//old version board
}
void sendcmd(char cmd[], int function) {
	cmd[0] = keyboard_Convert[function];
	Console->uart_write("\r\n", 2);
	Console->uart_write(cmd, CMD_Length);
	/*send data to linkit 7688 MPU*/
	linkit->uart_write(cmd, CMD_Length);
}
void rev(int operation) {
	uint64_t start_us;
	start_us = board_get_cur_us();
	char rev[16] = { 0 };
	int count = 0;
	while (board_get_cur_us() - start_us < (uint64_t) 10000000) { //give 10 seconds for response
		int available;
		linkit->uart_control(UART_CMD_GET_RXAVAIL, &available);
		if (available >= 4) {
			board_delay_ms(10, 1);
			linkit->uart_read(&rev[count], available);
			count += available;
			if (rev[0] == 'M') { // Error Message is started with character 'M'
				sprintf(msg, "Error number:%c\n\r", rev[1]);
				Lcd_DisplayString(0, 0, msg);
				linkit->uart_control(UART_CMD_GET_RXAVAIL, &available);
				while (available != 0) {/*flush input buffer*/
					linkit->uart_read(rev, 1);
					linkit->uart_control(UART_CMD_GET_RXAVAIL, &available);
				}
				break;
			} else {
				int tem = 0;
				char strng[16] = { 0 };
				/*get string*/
				while (tem < 10) {
					if (rev[tem] == 'Z')
						break;
					strng[tem] = rev[tem];
					tem++;
				}

				board_delay_ms(1, 1);
				/*classify response*/
				if (rev[0] == 'E') {
					switch (operation) {
					case Search: {
						switch (rev[2]) {
						case '0': {
							Lcd_DisplayString(0, 0, "No Such ID");
							break;
						}
						case '1': {
							Lcd_DisplayString(0, 0, "Not in shelf");
							break;
						}
						}
						break;
					}
					case Borrow: {
						switch (rev[2]) {
						case '0': {
							Lcd_DisplayString(0, 0, "No Such ID");
							break;
						}
						case '1': {
							Lcd_DisplayString(0, 0, "Borrowed");
							break;
						}
						case '2': {
							Lcd_DisplayString(0, 0, "Already Reserved");
							break;
						}
						}
						break;
					}
					case Giveback: {
						switch (rev[2]) {
						case '0': {
							Lcd_DisplayString(0, 0, "No Such ID");
							break;
						}
						case '1': {
							Lcd_DisplayString(0, 0, "Already returned");
							break;
						}
						case '2': {
							Lcd_DisplayString(0, 0, "No such location");
							break;
						}
						}
						break;
					}
					case NewReservation: {
						switch (rev[2]) {
						case '0': {
							Lcd_DisplayString(0, 0, "No Matched Data");
							break;
						}
						}
						break;
					}
					case CancelReservation: {
						switch (rev[2]) {
						case '0': {
							Lcd_DisplayString(0, 0, "No Matched Data");
							break;
						}
						}
						break;
					}
					}
				} else if (rev[0] == 'O') {
					Lcd_DisplayString(0, 0, "Success!!");
				} else {
					if (operation == Search) {

						int locate = rev[2] - '1';
						if (rev[0] == '1') {
							port_A_led->gpio_write(0x1000000 << locate,
							J56_MASK_A);
						} else {
							port_C_led->gpio_write(0x1000000 << locate,
							J5_PIN1to4_MASK_C);
							;
						}
					}
					Lcd_DisplayString(0, 0, strng);
				}

				/*flush input buffer*/
				linkit->uart_control(UART_CMD_GET_RXAVAIL, &available);
				while (available != 0) {
					linkit->uart_read(rev, 1);
					linkit->uart_control(UART_CMD_GET_RXAVAIL, &available);
				}
				break;
			}
		}
	}
}
