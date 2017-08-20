#include "blu_int.h"

/***********    blueteeth isr operation     ********/
static DEV_UART *blu_uart;
static uint8_t blu_rx_buffer;
static DEV_BUFFER blu_uart_int_buffer;

static void blu_rx_state_second(mode_info *blu_mode_info_ptr, uint8_t *blu_rx_handle_ptr, uint8_t data)
{
	if (data == BLU_RX_MUSIC_MODE) {
		blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
		blu_mode_info_ptr->mode = LIGHT_MODE_MUSIC;
		xQueueOverwrite(mode_info_queue, blu_mode_info_ptr);
		xTaskResumeFromISR( pattern_change_task_handle );
	}
	else if (data == BLU_RX_COMFOR_MODE) {
		blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
		blu_mode_info_ptr->mode = LIGHT_MODE_COMFOR;
		xQueueOverwrite(mode_info_queue, blu_mode_info_ptr);
		xTaskResumeFromISR( pattern_change_task_handle );
	}
	else {
		blu_rx_handle_ptr[BLU_STATE_BYTE] = THIRD_STATE;
		blu_rx_handle_ptr[BLU_MODE_TAG_BYTE] = data;
	}
}

static void blu_rx_state_third(mode_info *blu_mode_info_ptr, uint8_t *blu_rx_handle_ptr, uint8_t data)
{
	switch (blu_rx_handle_ptr[BLU_MODE_TAG_BYTE]) {
		case BLU_RX_WEATHER_MODE:
			if( data == '0') {
				blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
				blu_mode_info_ptr->mode = LIGHT_MODE_WEATHER;
				xQueueOverwrite(mode_info_queue, blu_mode_info_ptr);
				xTaskResumeFromISR( pattern_change_task_handle );				
			}
			else {
				blu_rx_handle_ptr[BLU_STATE_BYTE] = FOURTH_STATE;
				blu_mode_info_ptr->weather = data - '0';				
			}
			
			break;
		case BLU_RX_RUNNING_MODE:
		case BLU_RX_ALARM_MODE:
		case BLU_RX_RIDING_MODE:
		case BLU_RX_LIGHTING_MODE:
			blu_rx_handle_ptr[BLU_STATE_BYTE] = FOURTH_STATE;
			blu_rx_handle_ptr[BLU_MODE_AUX_TAG_BYTE] = data;
			break;
		case BLU_RX_CLOCK_MODE:
		case BLU_RX_TIME_MODE:
			blu_rx_handle_ptr[ blu_rx_handle_ptr[BLU_DATA_CNT_BYTE]++ ] = data - '0';

			if (blu_rx_handle_ptr[BLU_DATA_CNT_BYTE] == BLU_MODE_TIME_RX_LEN) {
				blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
				blu_rx_handle_ptr[BLU_DATA_CNT_BYTE] = 0;

				if (blu_rx_handle_ptr[BLU_MODE_TAG_BYTE] == BLU_RX_CLOCK_MODE)
					blu_mode_info_ptr->mode = LIGHT_MODE_CLOCK;
				else
					blu_mode_info_ptr->mode = LIGHT_MODE_TIMING_LAND;
				blu_mode_info_ptr->time = blu_rx_handle_ptr[0] * 600 + blu_rx_handle_ptr[1] * 60 + blu_rx_handle_ptr[2] * 10 + blu_rx_handle_ptr[3];
				xQueueOverwrite(mode_info_queue, blu_mode_info_ptr);
				xTaskResumeFromISR( pattern_change_task_handle );
			}
			break;
		case BLU_RX_FANSE_MODE:
			blu_rx_handle_ptr[BLU_STATE_BYTE] = FOURTH_STATE;
			blu_rx_handle_ptr[BLU_DATA_LEN_BYTE] = data;
			break;
		default:
			blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
			EMBARC_PRINTF("Wrong input %d in state 3.\r\n", blu_rx_handle_ptr[BLU_MODE_TAG_BYTE]);
			break;
	}
}


static void blu_rx_state_forth(mode_info *blu_mode_info_ptr, uint8_t *blu_rx_handle_ptr, uint8_t data)
{
	switch (blu_rx_handle_ptr[BLU_MODE_TAG_BYTE]) {
		case BLU_RX_WEATHER_MODE:
			blu_rx_handle_ptr[ blu_rx_handle_ptr[BLU_DATA_CNT_BYTE]++ ] = data - '0';

			if (blu_rx_handle_ptr[BLU_DATA_CNT_BYTE] == BLU_MODE_WEATHER_RX_LEN) {
				blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
				blu_rx_handle_ptr[BLU_DATA_CNT_BYTE] = 0;

				blu_mode_info_ptr->temperture = blu_rx_handle_ptr[0] * 10 + blu_rx_handle_ptr[1];
				blu_mode_info_ptr->humidity = blu_rx_handle_ptr[2] * 10 + blu_rx_handle_ptr[3];
				xQueueOverwrite(mode_info_queue, blu_mode_info_ptr);
			}
			break;
		case BLU_RX_RUNNING_MODE:
		case BLU_RX_ALARM_MODE:
		case BLU_RX_RIDING_MODE:
			blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
			if (blu_rx_handle_ptr[BLU_MODE_AUX_TAG_BYTE] == BLU_TAG_BRIGHT)
				blu_mode_info_ptr->bright = data;
			else if (blu_rx_handle_ptr[BLU_MODE_AUX_TAG_BYTE] == BLU_TAG_FREQUENCE) {
				switch (data) {
					case BLU_FREQUENCY_SLOW      : blu_mode_info_ptr->frequence = FREQUENCY_SLOW; 		break;
					case BLU_FREQUENCY_NORMAL    : blu_mode_info_ptr->frequence = FREQUENCY_NORMAL; 	break;
					case BLU_FREQUENCY_FAST      : blu_mode_info_ptr->frequence = FREQUENCY_FAST; 		break;
					case BLU_FREQUENCY_VERY_FAST : blu_mode_info_ptr->frequence = FREQUENCY_VERY_FAST; 	break;
					default  					 : blu_mode_info_ptr->frequence = 0; 					break;
				}
			}

			if (blu_rx_handle_ptr[BLU_MODE_TAG_BYTE] == BLU_RX_RUNNING_MODE) {

				if (blu_mode_info_ptr->mode != LIGHT_MODE_RUNNING)
					blu_mode_info_ptr->frequence = 0;
				blu_mode_info_ptr->mode = LIGHT_MODE_RUNNING;
			}
			else if (blu_rx_handle_ptr[BLU_MODE_TAG_BYTE] == BLU_RX_ALARM_MODE) {

				if (blu_mode_info_ptr->mode != LIGHT_MODE_ALARM)
					blu_mode_info_ptr->frequence = 0;
				blu_mode_info_ptr->mode = LIGHT_MODE_ALARM;
			}
			else if (blu_rx_handle_ptr[BLU_MODE_TAG_BYTE] == BLU_RX_RIDING_MODE) {

				if (blu_mode_info_ptr->mode != LIGHT_MODE_RIDING)
					blu_mode_info_ptr->frequence = 0;
				blu_mode_info_ptr->mode = LIGHT_MODE_RIDING;
			}

			xQueueOverwrite(mode_info_queue, blu_mode_info_ptr);
			xTaskResumeFromISR( pattern_change_task_handle );
			break;
		case BLU_RX_LIGHTING_MODE:
			blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
			if (blu_rx_handle_ptr[BLU_MODE_AUX_TAG_BYTE] == BLU_TAG_BRIGHT)
				blu_mode_info_ptr->bright = data;
			else if (blu_rx_handle_ptr[BLU_MODE_AUX_TAG_BYTE] == BLU_TAG_COOL_OR_WARM) {

				if (data == BLU_TAG_LIGHT_WARM)
					blu_rx_handle_ptr[BLU_LIGHT_TAG_BYTE] = BLU_TAG_LIGHT_WARM;
				else
					blu_rx_handle_ptr[BLU_LIGHT_TAG_BYTE] = BLU_TAG_LIGHT_COOL;
			}
			else if (blu_rx_handle_ptr[BLU_MODE_AUX_TAG_BYTE] == BLU_TAG_ANIMATION) {

				if (data == BLU_TAG_LIGHT_ANIM)
					blu_rx_handle_ptr[BLU_COLOR_TAG_BYTE] = BLU_TAG_LIGHT_ANIM;
				else
					blu_rx_handle_ptr[BLU_COLOR_TAG_BYTE] = BLU_TAG_LIGHT_NOT_ANIM;
			}

			if (blu_rx_handle_ptr[BLU_COLOR_TAG_BYTE] == BLU_TAG_LIGHT_ANIM)
				blu_mode_info_ptr->mode = LIGHT_MODE_ANIMATION;
			else {
				if (blu_rx_handle_ptr[BLU_LIGHT_TAG_BYTE] == BLU_TAG_LIGHT_WARM)
					blu_mode_info_ptr->mode = LIGHT_MODE_WARM_LIGHTING;
				else
					blu_mode_info_ptr->mode = LIGHT_MODE_CO_LIGHTING;
			}

			xQueueOverwrite(mode_info_queue, blu_mode_info_ptr);
			xTaskResumeFromISR( pattern_change_task_handle );
			break;
		case BLU_RX_FANSE_MODE:
			blu_mode_info_ptr->fance_txt[ blu_rx_handle_ptr[BLU_DATA_CNT_BYTE]++ ] = data;
			if (blu_rx_handle_ptr[BLU_DATA_CNT_BYTE] == blu_rx_handle_ptr[BLU_DATA_LEN_BYTE]) {
				blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
				blu_mode_info_ptr->fance_txt[ blu_rx_handle_ptr[BLU_DATA_CNT_BYTE] ] = '\0';
				blu_rx_handle_ptr[BLU_DATA_CNT_BYTE] = 0;
				
				blu_mode_info_ptr->mode = LIGHT_MODE_SHAKING;
				xQueueOverwrite(mode_info_queue, blu_mode_info_ptr);
				xTaskResumeFromISR( pattern_change_task_handle );
			}
			break;
		default:
			blu_rx_handle_ptr[BLU_STATE_BYTE] = INITIAL_STATE;
			EMBARC_PRINTF("Wrong mode input %d in state 4.\r\n", blu_rx_handle_ptr[BLU_MODE_TAG_BYTE]);
			break;
		}
}

/**
 * \brief  Blueteeth data receive
 * \details Transfer the data to the useful format.
 * \param[in] data: the data received   
 */
Inline void blu_data_recesive(uint8_t data)
{
	static mode_info blu_mode_info = INIT_MODE_INFO;
	static uint8_t blu_rx_handle[BLU_RX_HANDLE_BUF_MAX];

	if (blu_rx_handle[BLU_STATE_BYTE] == INITIAL_STATE && data == BLU_RX_HEAD_FIRST)
		blu_rx_handle[BLU_STATE_BYTE] = FIRST_STATE;
	else if (blu_rx_handle[BLU_STATE_BYTE] == FIRST_STATE && data == BLU_RX_HEAD_SECOND)
		blu_rx_handle[BLU_STATE_BYTE] = SECOND_STATE;
	else if (blu_rx_handle[BLU_STATE_BYTE] == SECOND_STATE && data <= (BLU_RX_MIN_MODE + MAX_MODE) && data >= BLU_RX_MIN_MODE)
		blu_rx_state_second(&blu_mode_info, blu_rx_handle, data);
	else if (blu_rx_handle[BLU_STATE_BYTE] == THIRD_STATE)
		blu_rx_state_third(&blu_mode_info, blu_rx_handle, data);
	else if (blu_rx_handle[BLU_STATE_BYTE] == FOURTH_STATE)
		blu_rx_state_forth(&blu_mode_info, blu_rx_handle, data);
	else
		blu_rx_handle[BLU_STATE_BYTE] = INITIAL_STATE;
}
/**
 * \brief  Blueteeth isr
 * \details The callback function of blueteeth isr.
 * \param[in] ptr: nonsense   
 */
static void blu_irs(void *ptr)
{
	static uint8_t i;

 	if(i % 2 == 0) {	
 		blu_data_recesive(blu_rx_buffer);
		EMBARC_PRINTF("blu uart input %d.\r\n", blu_rx_buffer);
 	}
	i++;

	if (blu_uart->uart_control(UART_CMD_SET_RXINT_BUF, (void *)&blu_uart_int_buffer) != E_OK){
		EMBARC_PRINTF("blu_isr_buffer config error\r\n");
	}
 	if (blu_uart->uart_control(UART_CMD_SET_RXINT, (void *)(1)) != E_OK){
		EMBARC_PRINTF("Enable blu interrupt error\r\n");
	}
}
/**
 * \brief  Initial the blueteeth drivers
 * \details Initial the uart0 driver for blueteeth.
 * \param[in] baudrate: the baudrate of uart0
 * \param[out] -1 for error, 0 for succeed  
 */
uint32_t blu_uart_init(uint32_t baudrate)
{
	blu_uart = uart_get_dev(DW_UART_0_ID);

	if (blu_uart == NULL) {
 		EMBARC_PRINTF("Failed to get device of uart0 for blueteeth.\r\n");
 		return -1;
 	}

	if (blu_uart->uart_open(baudrate) == E_OPNED) {
		blu_uart->uart_control(UART_CMD_SET_BAUD, (void *)(baudrate));
		EMBARC_PRINTF("blu_uart open ok\r\n");
  	}

  	DEV_BUFFER_INIT(&blu_uart_int_buffer, &blu_rx_buffer, 1);
  	if (blu_uart->uart_control(UART_CMD_SET_RXINT_BUF, (void *)&blu_uart_int_buffer) != E_OK){
		EMBARC_PRINTF("blu_isr_buffer config error\r\n");
		return -1;
	}
  	if (blu_uart->uart_control(UART_CMD_SET_RXCB, (void *)&blu_irs) != E_OK){
		EMBARC_PRINTF("blu_isr config error\r\n");
		return -1;
	}
	if (blu_uart->uart_control(UART_CMD_SET_RXINT, (void *)(1)) != E_OK){
		EMBARC_PRINTF("Enable blu interrupt error\r\n");
		return -1;
	}
	return 0;
}
/**
 * \brief  Send n bytes
 * \details Send n bytes  using blueteeth in uart0.
 * \param[in] psend: the buffer of date sended during uart0,
              cnt:   the number of bytes to send
 * \param[out] -1 for error, 0 for succeed    
 */
uint32_t blu_send(uint8_t *psend, uint8_t cnt)
{
	return blu_uart->uart_write(psend, cnt);
}

