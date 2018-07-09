/* embARC HAL */

#include "embARC.h"
#include "embARC_debug.h"

#include "vm_task.h"
#include "communication_task.h"

extern SemaphoreHandle_t xSemaphore;

enum main_status_type
{
    main_init_1,
	main_init_2,
	main_init_3,
	main_init_4,
	main_user_input_1,
	main_user_input_2,
	main_user_input_3,
	main_user_input_4,
	main_balance_check,
	main_recommand,
	main_purchase_choice,
	main_purchase_confirm
};

void _Main_Enqueue (int target_id, vm_data _data)
{
    _data.source_id = id_main;
    _data.target_id = target_id;
    enQueue(_data);
}


// 有問題，待改 沒有這個型態的 queue
void _Wifi_Main_Enqueue (WIFI_data _wifi_data)
{
	vm_data _data;
	_data.source_id   = id_main;
	_data.target_id   = id_wifi;
	strncpy(_data.user, _wifi_data.user, 4);
	_data.target_item = _wifi_data.target_item;

	for ( int i = 0; i < 4; i++ )
		_data.body[i].i = _wifi_data.body[i].i;

	for ( int i = 0; i < 4; i++ )
	{
		strncpy(_data.type, _wifi_data.type[i], 6);
		strncpy(_data.name, _wifi_data.name[i], 6);
		_Main_Enqueue(id_wifi, _data);
	}
}

int main(int argc, char **argv)
{
	EMBARC_PRINTF("entering main function\r\n");

    vm_data data = {0}; // data for receiveing
	WIFI_data local_data = {0}; // local database
	// int main_status = main_init_1;
	int main_status = main_init_1;
	char output = 'Z';

	while (1)
	{
		if (main_status == main_init_1)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						strncpy(local_data.type[data.target_item - 1], data.type, 6);
						strncpy(local_data.name[data.target_item - 1], data.name, 6);
						local_data.body[data.target_item - 1].i = data.body[data.target_item - 1].i;
						local_data.source_id = id_main;
						main_status = main_init_2;
					}
				}
			}
		}
		if (main_status == main_init_2)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						strncpy(local_data.type[data.target_item - 1], data.type, 6);
						strncpy(local_data.name[data.target_item - 1], data.name, 6);
						local_data.body[data.target_item - 1].i = data.body[data.target_item - 1].i;
						local_data.source_id = id_main;
						main_status = main_init_3;
					}
				}
			}
		}
		if (main_status == main_init_3)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						strncpy(local_data.type[data.target_item - 1], data.type, 6);
						strncpy(local_data.name[data.target_item - 1], data.name, 6);
						local_data.body[data.target_item - 1].i = data.body[data.target_item - 1].i;
						local_data.source_id = id_main;
						main_status = main_init_4;
					}
				}
			}
		}
		if (main_status == main_init_4)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						strncpy(local_data.type[data.target_item - 1], data.type, 6);
						strncpy(local_data.name[data.target_item - 1], data.name, 6);
						local_data.body[data.target_item - 1].i = data.body[data.target_item - 1].i;
						local_data.source_id = id_main;
						main_status = main_user_input_1;

						EMBARC_PRINTF("\r\n\r\n####### initial value stored in main function #######\r\n\r\n");
						EMBARC_PRINTF("\t target item: %d\r\n", local_data.target_item);
						EMBARC_PRINTF("\t status     : %d\r\n", local_data.status);
						EMBARC_PRINTF("\t user       : %s\r\n", local_data.user);
						EMBARC_PRINTF("\t name 1: %s, type 1: %s, #: %d\r\n", local_data.name[0], local_data.type[0], local_data.body[0].i);
						EMBARC_PRINTF("\t name 2: %s, type 2: %s, #: %d\r\n", local_data.name[1], local_data.type[1], local_data.body[1].i);
						EMBARC_PRINTF("\t name 3: %s, type 3: %s, #: %d\r\n", local_data.name[2], local_data.type[2], local_data.body[2].i);
						EMBARC_PRINTF("\t name 4: %s, type 4: %s, #: %d\r\n", local_data.name[3], local_data.type[3], local_data.body[3].i);
						EMBARC_PRINTF("\r\n#####################################################\r\n\r\n");
					}
					EMBARC_PRINTF("  -> main initial success\r\n");
				}
			}
		}
		if (main_status == main_user_input_1)
		{
			// EMBARC_PRINTF("Reading from numpad in main function started. \r\n");
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						data.user[0] = (char) data.body[0].i;
						data.status  = user_input_1;
						data.target_id = no_item;
						_Main_Enqueue(id_oled, data);
						local_data.user[0] = (char) data.body[0].i;
						main_status = main_user_input_2;
					}
				}
			}
		}
		if (main_status == main_user_input_2)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						data.user[1] = (char) data.body[0].i;
						data.status  = user_input_2;
						data.target_id = no_item;
						_Main_Enqueue(id_oled, data);
						local_data.user[1] = (char) data.body[0].i;
						main_status = main_user_input_3;
					}
				}
			}
		}
		if (main_status == main_user_input_3)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						data.user[2] = (char) data.body[0].i;
						data.status  = user_input_3;
						data.target_id = no_item;
						_Main_Enqueue(id_oled, data);
						local_data.user[2] = (char) data.body[0].i;
						main_status = main_user_input_4;
					}
				}
			}
		}
		if (main_status == main_user_input_4)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						EMBARC_PRINTF("receive char: %c\n", (char) data.body[0].i);
						data.user[3]   = (char) data.body[0].i;
						data.status    = user_input_4;
						data.target_id = no_item;
						_Main_Enqueue(id_oled, data);


						data.status    = balance_check;
						local_data.user[3] = (char) data.body[0].i;

						for (int i = 0; i < 4; i++)
							data.user[i] = local_data.user[i];
						for (int i = 0; i < 4; i++)
							data.body[i].i = local_data.body[i].i;
						_Main_Enqueue(id_wifi, data);

						local_data.target_item = no_item;
						// local_data.status      = balance_check;
						// _Wifi_Main_Enqueue(local_data);

						// check balance just once
						local_data.status = server_do_nothing;
						// _Wifi_Main_Enqueue(local_data);

						EMBARC_PRINTF("\r\n\r\n  -> user id recognize as: %s\r\n\r\n", data.user);

						EMBARC_PRINTF("\r\n\r\n####### value after user inputted #######\r\n\r\n");
						EMBARC_PRINTF("\t target item: %d\r\n", local_data.target_item);
						EMBARC_PRINTF("\t status     : %d\r\n", local_data.status);
						EMBARC_PRINTF("\t user       : %s\r\n", local_data.user);
						EMBARC_PRINTF("\t name 1: %s, type 1: %s, #: %d\r\n", local_data.name[0], local_data.type[0], local_data.body[0].i);
						EMBARC_PRINTF("\t name 2: %s, type 2: %s, #: %d\r\n", local_data.name[1], local_data.type[1], local_data.body[1].i);
						EMBARC_PRINTF("\t name 3: %s, type 3: %s, #: %d\r\n", local_data.name[2], local_data.type[2], local_data.body[2].i);
						EMBARC_PRINTF("\t name 4: %s, type 4: %s, #: %d\r\n", local_data.name[3], local_data.type[3], local_data.body[3].i);
						EMBARC_PRINTF("\r\n#####################################################\r\n\r\n");

						main_status = main_balance_check;
					}
					EMBARC_PRINTF("  -> main user input 4 success\r\n");
				}
			}
		}
		if (main_status == main_balance_check)
		{

			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("  -> main balance check wwith Wi-Fi\r\n");
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						EMBARC_PRINTF("  -> server balance check is %s.\r\n", (data.status == sell_recommand) ? "PASSED" : (data.status = reject_balance) ? "REJECTED" : "UNDEFINED");
						EMBARC_PRINTF("     => target_item is %d\r\n", data.target_item);
						EMBARC_PRINTF("     => balance check is %d\r\n", data.status);

						if (data.status == sell_recommand)
						{
							main_status = main_purchase_choice;
							_Main_Enqueue(id_oled, data);
						}
						else if (data.status == reject_balance)
						{
							data.target_item = 5; // judgement in oled_task()
							data.status = purchase_confirm;
							main_status = main_user_input_1;
							_Main_Enqueue(id_oled, data);
						}
					}
				}
			}
		}
		/*
		if (main_status == main_recommand)
		{
			EMBARC_PRINTF("\r\n  -> enter recommand state.\r\n");

			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_wifi)
					{
						data.status = sell_recommand;
						_Main_Enqueue(id_oled, data);
						main_status = main_purchase_choice;
					}
				}
			}
		}
		*/
		if (main_status == main_purchase_choice)
		{
			EMBARC_PRINTF("  -> enter choice state.\r\n");

			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					EMBARC_PRINTF("  -> numpad input: %c\r\n", (char)data.body[0].i);
					if (data.source_id == id_numpad)
					{
						if (data.body[0].i == '1' || data.body[0].i == '2')
						{
							data.status            = purchase_check;
							data.target_item       = (int)data.body[0].i - 48;
							local_data.target_item = (int)data.body[0].i - 48;
							strncpy(data.name, local_data.name[data.target_item - 1], 6);
							strncpy(data.type, local_data.type[data.target_item - 1], 6);
							_Main_Enqueue(id_oled, data);
							main_status = main_purchase_confirm;
						}	
					}
				}
			}
		}
		if (main_status == main_purchase_confirm)
		{
			if( xMainQueue != NULL )	
			{
				if( xQueueReceive( xMainQueue, &data, portMAX_DELAY ) )
				{
					EMBARC_PRINTF("  -> main purchase_confirm\r\n");
					// EMBARC_PRINTF("receive data, source_id and target_id: %d, %d\r\n", data.source_id, data.target_id);
					if (data.source_id == id_numpad)
					{
						if ((char) data.body[0].i == '#')
						{
							data.target_item = local_data.target_item;

							for ( int i = 0; i < 4; i++ )
								data.body[i].i = 0;
							
							data.body[0].i = local_data.target_item;

							data.status       = purchase_confirm;
							local_data.status = purchase_confirm;
							local_data.body[local_data.target_item - 1].i--;
							
							_Main_Enqueue(id_dcmotor, data);

							for ( int i = 0; i < 4; i++ )
								data.body[i].i = local_data.body[i].i;
							
							strncpy(data.user, local_data.user, 4);

							_Main_Enqueue(id_oled, data);
							_Main_Enqueue(id_wifi, data);

							// _Wifi_Main_Enqueue(local_data);
						}

						local_data.user[0] = 0;
						local_data.user[1] = 0;
						local_data.user[2] = 0;
						local_data.user[3] = 0;
						main_status = main_user_input_1;
					}
				}
			}
		}
		vTaskDelay(20);
	}

	return E_SYS;
}
