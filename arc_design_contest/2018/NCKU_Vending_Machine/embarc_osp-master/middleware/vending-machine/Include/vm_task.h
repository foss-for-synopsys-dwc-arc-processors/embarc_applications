#ifndef VM_TASK_H
#define VM_TASK_H

// define id
#define id_main      1
#define id_wifi      2
#define id_temp      3
#define id_numpad    4
#define id_dcmotor   5
#define id_oled      6

// define target_item
#define no_item      0
#define item_1       1
#define item_2       2
#define item_3       3
#define item_4       4

// define status
#define server_do_nothing 0
#define balance_check     1
#define sell_request      2
#define sell_recommand    3
#define reject_balance    4
#define reject_overdue    5
#define reject_enough     6
#define purchase_check    7
#define temperature       8
#define purchase_confirm  9
#define user_input_1      10
#define user_input_2      11
#define user_input_3      12
#define user_input_4      13

#include "stdint.h" 
extern QueueHandle_t xVMQueue;
extern QueueHandle_t xMainQueue;
extern QueueHandle_t xOledQueue;
extern QueueHandle_t xCommunicationQueue;
extern QueueHandle_t xTempQueue;
extern QueueHandle_t xNumPadQueue;
extern QueueHandle_t xDCmotorQueue;

typedef union u_data{
    int i;
    float f;
} data_union;

typedef struct _vm_data
{
    int source_id;
    int target_id;

    int status;
    int target_item;

    char user[4];
    char type[7];
    char name[7];

    union u_data body[4];
    //             number       name       type
    // item 1:    body[0].i    name[0]    type[0]
    //
    // temp: body[0].f
    // other integer data: body[0].i
    
} vm_data;


void vm_task(void *p_arg);

void enQueue(vm_data);

#endif
