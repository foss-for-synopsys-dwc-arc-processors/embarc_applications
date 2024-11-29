#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include "CSpotterSDK.h"
#include "conf.h"

#define DISABLE_MALLOC
#if defined DEBUG
	#define LOG(args...) {printf(args); putchar('\n');}
#else
	#define LOG(args...)
#endif


#ifdef DISABLE_MALLOC
#define k_nMemSize			(20 * 256* sizeof(int))		//Memory alloc size for CSpotter. Modify it, if need.
#endif

#define k_nStateSize		(19* sizeof(int))
#define k_nMaxTime			300
#define k_nFrameSize		160//256

#define k_nMaxGroupSize 5
#define k_nLevel2TimeOut (5*16000) //interval of level 2 recognize time. After time out, it would back to level 1

typedef struct
{
 HANDLE hCSpotter;
 int groupId;
 int cmdId;
 BYTE lpbyState[k_nStateSize] ;
 #ifndef DISABLE_MALLOC
  BYTE *lpbyMemPool;
 #else
	 #pragma message "Spotter.c: use fixed memory size"
  BYTE lpbyMemPool[k_nMemSize];
 #endif
int groupList[k_nMaxGroupSize+1];
int level2RemainTime;
}Spotter;

static Spotter gSpotter= {0};
void Spotter_Release(void);
int Spotter_Init2(BYTE *lpbyCYBase, BYTE **lppbyModel, void* lpbyRejModel){
	int n;
	int nGroups, nCmds;
	int err;
	if (gSpotter.hCSpotter!= 0){
		Spotter_Release();
	}
	
	for(nGroups= 0, nCmds= 0; nGroups<= k_nMaxGroupSize; nGroups++){
		if(lppbyModel[nGroups]== 0) break;
		nCmds+= CSpotter_GetNumWord(lppbyModel[nGroups]);
		gSpotter.groupList[nGroups]= nCmds;
	}
//	LOG("cmd num: %d\n", nCmds);
	gSpotter.groupList[nGroups]= INT_MAX;
	// Check the memory usage
	n = CSpotter_GetMemoryUsage_MultiWithExtRejModel(lpbyCYBase , lppbyModel , nGroups, k_nMaxTime, lpbyRejModel);
#ifndef DISABLE_MALLOC
 	gSpotter.lpbyMemPool= malloc(n);
	if( gSpotter.lpbyMemPool== 0 ) {
#else
	if( n > k_nMemSize ) {
#endif
		LOG("CSpotter init fail: memory need %d\n", n);
		return -2;
	}
	
	gSpotter.hCSpotter = CSpotter_Init_MultiWithExtRejModel(lpbyCYBase , lppbyModel , nGroups, k_nMaxTime, (BYTE*)lpbyRejModel, (BYTE*)gSpotter.lpbyMemPool , n , gSpotter.lpbyState , k_nStateSize , &err ) ;
	if( err != CSPOTTER_SUCCESS ) {
#ifndef DISABLE_MALLOC
 		free(gSpotter.lpbyMemPool);
		gSpotter.lpbyMemPool= 0;
#endif
		LOG("CSpotter init fail:error code is %d\n", err);
		return err;
	}
	gSpotter.level2RemainTime= 0;
	return CSPOTTER_SUCCESS;	
}

int Spotter_Init(BYTE *lpbyCYBase, BYTE *lppbyModel[]){
	return Spotter_Init2(lpbyCYBase, lppbyModel, 0);
}

static void calResult(void){
	int cmd= CSpotter_GetResult(gSpotter.hCSpotter);
	int i;
	for(i= 0; ; i++){
		if(cmd< gSpotter.groupList[i]) {
			gSpotter.groupId= i;
			if(i> 0){
				gSpotter.cmdId= cmd- gSpotter.groupList[i- 1];
			}else{
				gSpotter.cmdId= cmd;
			}
			return;
		}
	}
}

LED_MODE led;
int Spotter_AddSample(SHORT *lpsSample, INT nNumSample){
	int nRet= CSpotter_AddSample(gSpotter.hCSpotter, lpsSample, nNumSample);
	if(nRet== CSPOTTER_SUCCESS){
		calResult();
		if(gSpotter.groupId== 0){
			gSpotter.level2RemainTime= k_nLevel2TimeOut; //goto level 2
			led = LED_LIGHT;
			//printf("ledd\n",led );
			LOG("trigger, jump to level 2");
		}else if(gSpotter.level2RemainTime<= 0){
			nRet= CSPOTTER_ERR_NeedMoreSample; //false alarm
			LOG("ignore cmd");
		}else{
			gSpotter.level2RemainTime= 0; //detect level 2 cmd, back to level 1
			led = LED_DARK_TASK;
			set_work_status_EXE_CMD();
			LOG("detect cmd");
		}		
	}else if(gSpotter.level2RemainTime> 0){
		gSpotter.level2RemainTime-= nNumSample;
		if(gSpotter.level2RemainTime<= 0){
			printf("timoutttttttttttttt\n" );
			led = LED_DARK;
			LOG("time out");
		}
	}
		
	return nRet;
}


int Spotter_GetResultId(void){
	return gSpotter.cmdId;
}

int Spotter_GetResultGroupId(void){
	return gSpotter.groupId;
}

void Spotter_Reset(void){
	CSpotter_Reset(gSpotter.hCSpotter);
}

void Spotter_Release(void){
	if(!gSpotter.hCSpotter) return;
	CSpotter_Release(gSpotter.hCSpotter);
	gSpotter.hCSpotter= 0;		
#ifndef DISABLE_MALLOC
 	free(gSpotter.lpbyMemPool);
	gSpotter.lpbyMemPool= 0;
#endif
}

const char* Spotter_GetVersion(void){
	return CSpotter_GetVersion();
}

int Spotter_VAD_Enable(int isEnabled){
#if SUPPORT_VAD
	return CSpotter_VAD_Enable(gSpotter.hCSpotter , isEnabled);	
#else
	return -1;
#endif
}

int Spotter_VAD_SetMinEnergyThreshd(int nThreshold){
#if SUPPORT_VAD
	return CSpotter_VAD_SetMinEnergyThreshd(gSpotter.hCSpotter , nThreshold);	
#else
	return -1;
#endif
}

HANDLE Spotter_GetHandle(){
	return gSpotter.hCSpotter;
}
