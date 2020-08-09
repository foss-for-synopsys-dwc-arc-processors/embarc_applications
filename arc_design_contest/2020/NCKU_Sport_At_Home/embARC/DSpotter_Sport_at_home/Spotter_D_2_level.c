/********
Spotter API for help DSpotter  2 level recognition.
It use the first group(lppbyModel[0]) as trigger, the other group as command
In the beginning, the Spotter would recognize for trigger word and ignore the command word. It is the level 1.
After recognize trigger, it enter to level 2. 
In the Level 2, it recognize all group. If recognize command or time out(k_nLevel2TimeOut) , it back to level 1.
Default, the 2 level is disable. To enable 2 level, call Spotter_SetLevel2Enabled(1)
********/
#include <stdio.h>
#include <stdlib.h>

#include <limits.h>
#include "DSpotterSDKApi.h"

#if defined DEBUG
	#define LOG(args...) {printf(args); putchar('\n');}
#else
	#define LOG(args...)
#endif

#define k_nMaxTime			500
#define k_nMaxGroupSize 5
//interval of level 2 recognize time. After time out, it would back to level 1
#define k_nLevel2TimeOut (8*16000) 

typedef struct
{
 HANDLE hDSpotter;
 int Level2Mode;
 int level2TimeOut;
 int groupId;
 int cmdId;
 int groupList[k_nMaxGroupSize+1];
 int level2RemainTime;
}Spotter;

static Spotter gSpotter= {0, 0, k_nLevel2TimeOut, 0};
void Spotter_Release(void);
int Spotter_Init(BYTE *lpbyCYBase, BYTE *lppbyModel[], void* pMem, int memSize){
	int n;
	int nGroups, nCmds;
	int err;
	if (gSpotter.hDSpotter!= 0){
		Spotter_Release();
	}
	
	for(nGroups= 0, nCmds= 0; nGroups<= k_nMaxGroupSize; nGroups++){
		if(lppbyModel[nGroups]== 0) break;
		nCmds+= DSpotter_GetNumWord(lppbyModel[nGroups]);
		gSpotter.groupList[nGroups]= nCmds;
	}
	gSpotter.groupList[nGroups]= INT_MAX;
	// Check the memory usage
	n = DSpotter_GetMemoryUsage_Multi(lpbyCYBase , lppbyModel , nGroups, k_nMaxTime);
	if(n< 0){
		err= n;
		LOG("get mem usage fail.\r\n");
		goto Error;
	}
	if( n > memSize) {
		LOG("Spotter init fail: memory need %d\n", n);
		return -2;
	}
	
	gSpotter.hDSpotter = DSpotter_Init_Multi(lpbyCYBase , lppbyModel , nGroups, k_nMaxTime, (BYTE*)pMem , memSize , 0 , 0 , &err ) ;
	if( err != DSPOTTER_SUCCESS ) {
		goto Error;
	}
	gSpotter.level2RemainTime= 0;
	return DSPOTTER_SUCCESS;	
Error:
	LOG("DSpotter init fail:error code is %d\n", err);
	return err;
}


int Spotter_GetMemoryUsage(BYTE *lpbyCYBase, BYTE *lppbyModel[]){
	int nGroups;
	for(nGroups= 0;nGroups<= k_nMaxGroupSize; nGroups++){
		if(lppbyModel[nGroups]== 0) break;
	}	
	return DSpotter_GetMemoryUsage_Multi(lpbyCYBase , lppbyModel , nGroups, k_nMaxTime);
}

static void calResult(void){
/*
	int gmm, sg, fil;
	DSpotter_GetResultScore(gSpotter.hDSpotter, &gmm, &sg, &fil);
	printf("score: %d %d %d\n", gmm, sg, fil);
*/
	int cmd= DSpotter_GetResult(gSpotter.hDSpotter);
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

void Spotter_Reset(void);
int Spotter_AddSample(SHORT *lpsSample, INT nNumSample){
	int nRet= DSpotter_AddSample(gSpotter.hDSpotter, lpsSample, nNumSample);
	if(nRet== DSPOTTER_SUCCESS){
		calResult();
		DSpotter_Reset(gSpotter.hDSpotter);
		if(!gSpotter.Level2Mode) return nRet;
		if(gSpotter.groupId== 0){
			gSpotter.level2RemainTime= gSpotter.level2TimeOut; //goto level 1
			LOG("trigger, jump to level 2");
		}else if(gSpotter.level2RemainTime<= 0){
			nRet= DSPOTTER_ERR_NeedMoreSample; //false alarm
			LOG("ignore cmd");
		}else if(gSpotter.Level2Mode== 2){
			gSpotter.level2RemainTime= gSpotter.level2TimeOut; //detect level 2 cmd, back to level 0
			LOG("detect cmd(M)");
		}else{
			gSpotter.level2RemainTime= 0; //detect level 2 cmd, back to level 0
			LOG("detect cmd(S)");
		}		
	}else if(gSpotter.level2RemainTime> 0){
		gSpotter.level2RemainTime-= nNumSample;
		if(gSpotter.level2RemainTime<= 0){
			LOG("time out");
		}
	}
	return nRet;
}

int Spotter_SetLevel2Timeout(int millSec){
	gSpotter.level2TimeOut= millSec* 16;
	return 0;
}

int Spotter_GetLevel2Timeout(){
	return gSpotter.level2TimeOut/ 16;
}


int Spotter_SetLevel2Enabled(int isEnabled){
	gSpotter.Level2Mode= isEnabled;
	gSpotter.level2RemainTime= 0;
	return 0;
}

int Spotter_GetLevel2Enabled(){
	return gSpotter.Level2Mode;
}

int Spotter_GetLevel(){
	return gSpotter.Level2Mode&& gSpotter.level2RemainTime<= 0?0: 1;
}

int Spotter_GetResultId(void){
	return gSpotter.cmdId;
}

int Spotter_GetResultGroupId(void){
	return gSpotter.groupId;
}

void Spotter_Reset(void){
	DSpotter_Reset(gSpotter.hDSpotter);
	gSpotter.level2RemainTime= 0;
}

void Spotter_Release(void){
	if(!gSpotter.hDSpotter) return;
	DSpotter_Release(gSpotter.hDSpotter);
	gSpotter.hDSpotter= 0;		
}

const char* DSpotter_GetVersion(void);
const char* Spotter_GetVersion(void){
	return DSpotter_GetVersion();
}

int Spotter_VAD_Enable(int isEnabled){
	return -1; //not support
}

int Spotter_VAD_SetMinEnergyThreshd(int nThreshold){
	return -1; //not support
}

HANDLE Spotter_GetHandle(){
	return gSpotter.hDSpotter;
}
