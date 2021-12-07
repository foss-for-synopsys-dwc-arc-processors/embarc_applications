#ifndef _INPUT_BUFFER_H_
#define _INPUT_BUFFER_H_

#include "mpu9250.h"

/* Values position inside the input buffer */
#define ACCL_BODY_X     0
#define ACCL_BODY_Y     1
#define ACCL_BODY_Z     2
#define GYRO_X          3
#define GYRO_Y          4 
#define GYRO_Z          5
#define ACCL_TOTAL_X    6
#define ACCL_TOTAL_Y    7
#define ACCL_TOTAL_Z    8

extern void 	input_buffer_init(void);
extern void 	input_buffer_push_data(MPU9250_DATA_PTR mp_data);
extern float*	input_buffer_get_next_buffer(void);

#endif /* _INPUT_BUFFER_H_ */