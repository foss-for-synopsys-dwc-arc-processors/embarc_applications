#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define CORE_FREQ_MHZ   50
#define GYRO_DPS        1000    // +/-1000dps
#define ACCEL_G         4       // +/-4g

#define FRAME_SIZE              128

#define CHUNK_SIZE                  (9)                             // "chunk" is a 9 numbers set: filtered (ACC_body_x, ACC_body_y, ACC_body_z) + raw (ACCx, ACCy, ACCz, GYRx, GYRy, GYRz) 
#define CHUNK_SIZE_BYTES            (CHUNK_SIZE * sizeof(float))    // "chunk" size in bytes 
#define OVERLAP_SIZE                (FRAME_SIZE / 2)                // overlap size in chunks
#define BUFFERING_DEPTH             (8)
#define INPUT_BUFFER_SIZE           (FRAME_SIZE * BUFFERING_DEPTH)
#define INPUT_BUFFER_THRESHOLD      (INPUT_BUFFER_SIZE - (FRAME_SIZE / 2))

#define ENABLE_DUMP     0

#define  FC1_IN_SIZE 9
#define  lSTM_H_SIZE 32
#define  lSTM_X_SIZE 32
#define  lSTM_SEQEN 128
#define  CLASS_NUM 4


#define TIMER           TIMER_0
#define TIMER_IRQ       16
#define TIMER_RATE_HZ   25
#endif //_SETTINGS_H_