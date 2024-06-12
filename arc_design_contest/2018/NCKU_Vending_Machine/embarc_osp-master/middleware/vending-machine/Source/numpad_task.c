/* embARC HAL */
#include "embARC.h"
#include "embARC_debug.h"

#include "vm_task.h"

#define J3_PIN7to10_MASK_A 0xf0000
#define J3_PIN1to4_MASK_C  0xf0000
#define false			   0
#define true  			   1
#define debounce_start_on  2
#define debounce_start_off 3

static inline void _Npad_Enqueue(int target_id, int user_input) {
	vm_data _data = {0};

	_data.source_id = id_numpad;
	_data.target_id = target_id;
	_data.body[0].i = user_input;
	enQueue(_data);
}

// 要設計讓使用者輸入 帳號 和 要買的物品

typedef struct _button_status{
	int ispress;
	uint64_t press_time;
	uint64_t release_time;
} button_status;

const static char Numpad_Convert[] = {
	'1', '2', '3', 'A',
	'4', '5', '6', 'B',
	'7', '8', '9', 'C',
	'*', '0', '#', 'D'
};

static button_status Numpad_table[4][4] = {0};
static DEV_GPIO_PTR numpad_row;
static DEV_GPIO_PTR numpad_col;

void numpad_task(void *p_arg)
{
	EMBARC_PRINTF("enter num_pad task\r\n");

	numpad_row = gpio_get_dev(DW_GPIO_PORT_C);
	numpad_col = gpio_get_dev(DW_GPIO_PORT_A);
	numpad_row->gpio_close(); /* opened before now close it */
	numpad_col->gpio_close(); /* opened before now close it */	
	numpad_row->gpio_open(J3_PIN1to4_MASK_C);
	numpad_col->gpio_open(J3_PIN7to10_MASK_A);

	numpad_row->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(J3_PIN1to4_MASK_C));
	numpad_col->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(J3_PIN7to10_MASK_A));

	numpad_row->gpio_write(0x00000000, J3_PIN1to4_MASK_C);
	numpad_col->gpio_write(0xf0000, J3_PIN7to10_MASK_A);

	uint32_t data;
	uint32_t buffer;

	int i, j;
	
	while(1){
		for(i= 0; i < 4; i ++){
			//scan 4 rows
			
			numpad_row->gpio_control(GPIO_CMD_SET_BIT_DIR_OUTPUT, (void *)(0x10000 << i));
			numpad_row->gpio_write(0x00000000, J3_PIN1to4_MASK_C);
			vTaskDelay(1);
			numpad_col->gpio_read(&data, J3_PIN7to10_MASK_A);

			for(j= 0; j < 4; j ++){
				//scan 4 rows
				if(data & (0x10000 << j)) {//Not press

					if(Numpad_table[i][j].ispress == true){
						Numpad_table[i][j].release_time = board_get_cur_us();
						//EMBARC_PRINTF("%d,%d\r\n",i,j);
						if((Numpad_table[i][j].release_time - Numpad_table[i][j].press_time) >= 100000ul) {
							// EMBARC_PRINTF("%c\n", Numpad_Convert[i*4+j]);
							_Npad_Enqueue(id_main,    (int) Numpad_Convert[i*4+j]);
							// _Npad_Enqueue(id_oled,    (int) Numpad_Convert[i*4+j]);
							// _Npad_Enqueue(id_dcmotor, (int) Numpad_Convert[i*4+j] - 48);
						}
						Numpad_table[i][j].ispress = false;
					} 
				} else {//Press
					if(Numpad_table[i][j].ispress == false) {
						Numpad_table[i][j].press_time = board_get_cur_us();	
						Numpad_table[i][j].ispress = true;
					}
				}

			}
			numpad_row->gpio_control(GPIO_CMD_SET_BIT_DIR_INPUT, (void *)(J3_PIN1to4_MASK_C));
		}
		vTaskDelay(0);
	}
}