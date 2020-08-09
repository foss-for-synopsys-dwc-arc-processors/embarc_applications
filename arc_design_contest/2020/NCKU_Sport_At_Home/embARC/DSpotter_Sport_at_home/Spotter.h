#ifndef	__SPOTTER_IC_H
#define	__SPOTTER_IC_H

#include "DSpotterSDKApi_Const.h"

#define SPOTTER_SUCCESS DSPOTTER_SUCCESS
#define SPOTTER_ERR_NeedMoreSample DSPOTTER_ERR_NeedMoreSample

int Spotter_GetMemoryUsage(const void *lpbyCYBase, const void *lppbyModel[]);
// lpbyCYBase(IN): The background model, contents of CYBase.mod.
// lppbyModel(IN): The  null-terminal array of command group.
int Spotter_Init(const void *lpbyCYBase, const void *lppbyModel[], void* pMem, int memSize);
void Spotter_Reset(void);
int Spotter_AddSample(short *lpsSample, int nNumSample);
int Spotter_GetResultId(void);
//command group index of result
int Spotter_GetResultGroupId(void);
void Spotter_Release(void);

int Spotter_VAD_Enable(int isEnabled);
int Spotter_VAD_SetMinEnergyThreshd(int nThreshold); // The default is 500. It is equal to the RMS amplitude and the wave amplitude is more than 4 times of that amount.
const char* Spotter_GetVersion(void); //get version info 

//------------------- 2-level recognize--------------
//When Enabled, lppbyModel[0] would be triggle group and others are command group. 
//Initailly the state is at level 0(triggle mode), only triggle group would be active, others are ignored. 
//After recognized triggle, the state enter level 1 and all group is active.
//In the level 1, when recognized command group or time out, the state would be back to level 0.

int Spotter_SetLevel2Enabled(int mode); //enable or disable 2-Level recognize. 0: disable, 2: multi-command, otherwise: single-command, default: 0
int Spotter_GetLevel2Enabled();

//2-level recognize level state. 
//0: only triggle group active(lppbyModel[0]),; 
//1: all groups are active.
int Spotter_GetLevel(); 

int Spotter_SetLevel2Timeout(int millSec); //level 1 time out time in million sec. default 8000 ms
int Spotter_GetLevel2Timeout();

#endif
