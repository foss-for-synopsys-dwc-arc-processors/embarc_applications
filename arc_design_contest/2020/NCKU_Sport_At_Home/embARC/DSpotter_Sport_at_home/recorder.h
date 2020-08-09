#ifndef	__LIB_RECORDER_H
#define	__LIB_RECORDER_H
#include <stdint.h>

int Recorder_Init(); //0, success;otherwise
int Recorder_Start(); //0, success;otherwise
void Recorder_Stop();
void Recorder_Release();

int Recorder_NextBuffer();
int16_t* Recorder_GetBuffer();
int Recorder_GetBufferSize();
#endif
