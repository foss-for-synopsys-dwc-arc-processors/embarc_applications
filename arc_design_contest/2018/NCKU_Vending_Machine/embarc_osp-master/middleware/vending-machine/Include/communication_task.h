#ifndef COMMUNICATION_TASK_H
#define COMMUNICATION_TASK_H

#include "stdint.h"

void communication_task(void *p_arg);

typedef struct _wifi_data
{
    int source_id;
    int target_id;

    int status;
    int target_item;

    char user[5];
    char type[4][7];
    char name[4][7];

    data_union body[4];
    //             number       name       type
    // item 1:    body[0].i    name[0]    type[0]
    // item 2:    body[1].i    name[1]    type[1]
    // item 3:    body[2].i    name[2]    type[2]
    // item 4:    body[3].i    name[3]    type[3]
    //
    // temp: body[0].f
    // other integer data: body[0].i
    
} WIFI_data;

#endif
