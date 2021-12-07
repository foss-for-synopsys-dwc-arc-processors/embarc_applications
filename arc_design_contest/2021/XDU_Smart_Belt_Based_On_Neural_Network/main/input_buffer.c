#include "embARC.h"
#include "embARC_debug.h"
#include "input_buffer.h"
#include "memory.h"
#include "settings.h"
#include <stdio.h>

/* 
 * Input buffer declaration and settings 
 */

static float input_buffer[INPUT_BUFFER_SIZE * CHUNK_SIZE];
static uint32_t read_pos;
static uint32_t write_pos;
static volatile uint32_t s_samples = 0;

#define SCALE_ACC_TO_FLOAT(val) ((float)val * ACCEL_G / 0x7FFF)
#define DPS_TO_RPS(val)         (((float)val * GYRO_DPS / 0x7FFF) * 3.14159265 / 180)

/* DC removal */

static float filter_state[3][4] = {0};

#define B0  (float)(0.977945136)
#define B1  (float)(-1.914934054)
#define B2  (float)(0.936989906)
#define A1  (float)(-1.963137795)
#define A2  (float)(0.963991778)

#define STATE_X     (0)
#define STATE_Y     (1)
#define STATE_Z     (2)

static float dc_removal(float x0, uint8_t n)     //dc滤波？
{
    float x1,x2,y0,y1,y2;
    x1 = filter_state[n][0];
    x2 = filter_state[n][1];
    y1 = filter_state[n][2];
    y2 = filter_state[n][3];
    /* y0 = b0*x0 + b1*x1 + b2*x2 - a1*y1 - a2*y2 */
    y0 = B0 * (float)x0 + B1 * (float)x1 + B2 * (float)x2 - A1 * (float)y1 - A2 * (float)y2;
    filter_state[n][0] = x0;
    filter_state[n][1] = x1;
    filter_state[n][2] = y0;
    filter_state[n][3] = y1;
    return y0;
}

void input_buffer_init(void)
{
    read_pos = 0;
    write_pos = 0;
    memset((void*)input_buffer, 0, INPUT_BUFFER_SIZE * CHUNK_SIZE_BYTES);
}

void input_buffer_push_data(MPU9250_DATA_PTR mp_data)
{
    float *value;
    s_samples++;
    /* Check if the threshold has been reached so 
       it is needed to shift the data back to the beginning */
    if(write_pos == INPUT_BUFFER_THRESHOLD) {
        memmove((void*)input_buffer, (void*)&input_buffer[read_pos * CHUNK_SIZE], (write_pos - read_pos) * CHUNK_SIZE_BYTES);
        write_pos -= read_pos;
        read_pos = 0;
    }
    /* Copy data to the buffer */
    value = (float*)&input_buffer[write_pos * CHUNK_SIZE];
    /* Converting accelerometer values to float (in units of g=9.8m^2/s) */
    value[ACCL_TOTAL_X] = SCALE_ACC_TO_FLOAT(mp_data->accel_x);
    value[ACCL_TOTAL_Y] = SCALE_ACC_TO_FLOAT(mp_data->accel_y);
    value[ACCL_TOTAL_Z] = SCALE_ACC_TO_FLOAT(mp_data->accel_z);
    /* Converting gyroscope values to float (deg/sec -> rad/sec) */
    value[GYRO_X] = DPS_TO_RPS(mp_data->gyro_x);
    value[GYRO_Y] = DPS_TO_RPS(mp_data->gyro_y);
    value[GYRO_Z] = DPS_TO_RPS(mp_data->gyro_z);
    value[ACCL_BODY_X] = dc_removal(value[ACCL_TOTAL_X], STATE_X);
    value[ACCL_BODY_Y] = dc_removal(value[ACCL_TOTAL_Y], STATE_Y);
    value[ACCL_BODY_Z] = dc_removal(value[ACCL_TOTAL_Z], STATE_Z);
    write_pos++;
}
static void input_buffer_print(float *value,int16_t pos)
{
    int i;
    static int num;
    EMBARC_PRINTF( "-------------------[%d]-------------------\r\n",  pos);
    for(i=0; i<FRAME_SIZE; i++)
    {
        EMBARC_PRINTF( "[%d],",   num);
        printf( "%f,%f,%f,",     value[ACCL_BODY_X], value[ACCL_BODY_Y], value[ACCL_BODY_Z]);
        printf( "%f,%f,%f,",     value[GYRO_X], value[GYRO_Y], value[GYRO_Z]);
        printf( "%f,%f,%f,\r\n",  value[ACCL_TOTAL_X], value[ACCL_TOTAL_Y], value[ACCL_TOTAL_Z]);
        value += CHUNK_SIZE;
        num += 1;
    }
}
float* input_buffer_get_next_buffer(void)
{
    float* ret = &input_buffer[read_pos * CHUNK_SIZE];

    /* Frame is ready? Return NULL if not */
    if((write_pos - read_pos) < FRAME_SIZE) {
        return (float*)NULL;  
    }
#if ENABLE_DUMP
    input_buffer_print(ret,read_pos);
#endif  
    read_pos += OVERLAP_SIZE;    
    return ret; 
}



