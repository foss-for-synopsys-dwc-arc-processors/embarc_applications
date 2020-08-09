
#ifndef __DSPOTTERSDK_API_H
#define __DSPOTTERSDK_API_H

#if defined(_WIN32)
	#ifdef DSpotterDll_EXPORTS
		#define DSPDLL_API __declspec(dllexport)
	#endif
#endif

#ifndef DSPDLL_API
#define DSPDLL_API
#endif

#include "base_types.h"
#include "DSpotterSDKApi_Const.h"

#ifdef __cplusplus
extern "C"{
#endif

// lpbyCYBase(IN): The background model, contents of CYBase.mod.
// lppbyModel(IN): The command model.
// nMaxTime(IN): The maximum buffer length in number of frames for keeping the status of commands.
// lpbyMemPool(IN/OUT): Memory buffer for the recognizer.
// nMemSize(IN): Size in bytes of the memory buffer lpbyMemPool.
// lpbyPreserve(IN/OUT): Preserve param.
// nPreserve(IN): Preserve param.
// pnErr(OUT): DSPOTTER_SUCCESS indicates success, else error code. It can be NULL.
// Return: a recognizer handle or NULL
DSPDLL_API HANDLE DSpotter_Init_Multi(BYTE *lpbyCYBase, BYTE *lppbyModel[], INT nNumModel, INT nMaxTime, BYTE *lpbyMemPool, INT nMemSize, BYTE *lpbyPreserve, INT nPreserve, INT *pnErr);

// Purpose: Destroy a recognizer (free resources)
// hDSpotter(IN): a handle of the recognizer
// Return: Success or error code
DSPDLL_API INT DSpotter_Release(HANDLE hDSpotter);

// Purpose: Reset recognizer
// hDSpotter(IN): a handle of the recognizer
// Return: Success or error code
DSPDLL_API INT DSpotter_Reset(HANDLE hDSpotter);

// Purpose: Transfer voice samples to the recognizer for recognizing.
// hDSpotter(IN): a handle of the recognizer
// lpsSample(IN): the pointer of voice data buffer
// nNumSample(IN): the number of voice data (a unit is a short, we prefer to add 480 samples per call)
// Return: "DSPOTTER_ERR_NeedMoreSample" indicates call this function again, else call DSpotter_GetResult(...)
DSPDLL_API INT DSpotter_AddSample(HANDLE hDSpotter, SHORT *lpsSample, INT nNumSample);

// Purpose: Get recognition results.
// hDSpotter(IN): a handle of the recognizer
// Return: the command ID. It is 0 based
DSPDLL_API INT DSpotter_GetResult(HANDLE hDSpotter);
DSPDLL_API INT DSpotter_GetResultEPD(HANDLE hDSpotter, INT *pnWordDura, INT *pnEndDelay);
DSPDLL_API INT DSpotter_GetResultScore(HANDLE hDSpotter, INT *pnGMM, INT *pnSG, INT *pnFIL);

// Purpose: Get Cmd energy(RMS)
// hDSpotter(IN): a handle of the recognizer
// Return: Value or error code
DSPDLL_API INT DSpotter_GetCmdEnergy(HANDLE hDSpotter);

//Model Param
DSPDLL_API INT DSpotter_GetNumWord(BYTE *lpbyModel);

DSPDLL_API INT DSpotter_GetMemoryUsage_Multi(BYTE *lpbyCYBase, BYTE *lppbyModel[], INT nNumModel, INT nMaxTime);

DSPDLL_API const char *DSpotter_VerInfo(void);


/************************************************************************/
//  Threshold Adjust API                                                                   
/************************************************************************/
// Purpose: Set model rejection level
// hDSpotter(IN): a handle of the recognizer
// nRejectionLevel(IN): rejection level
// Return: Success or error code
DSPDLL_API INT DSpotter_SetRejectionLevel(HANDLE hDSpotter, INT nRejectionLevel);

// Purpose: Set model SG rejection level
// hDSpotter(IN): a handle of the recognizer
// nRejectionLevel(IN): SG rejection level
// Return: Success or error code
DSPDLL_API INT DSpotter_SetSgLevel(HANDLE hDSpotter, INT nRejectionLevel);

// Purpose: Set model FIL rejection level
// hDSpotter(IN): a handle of the recognizer
// nRejectionLevel(IN): FIL rejection level
// Return: Success or error code
DSPDLL_API INT DSpotter_SetFilLevel(HANDLE hDSpotter, INT nRejectionLevel);

// Purpose: Set engine response time
// hDSpotter(IN): a handle of the recognizer
// nResponseTime(IN): response time
// Return: Success or error code
DSPDLL_API INT DSpotter_SetResponseTime(HANDLE hDSpotter, INT nResponseTime);

// Purpose: Set engine energy threshold
// hDSpotter(IN): a handle of the recognizer
// nEnergyTH(IN): energy threshold(RMS)
// Return: Success or error code
DSPDLL_API INT DSpotter_SetEnergyTH(HANDLE hDSpotter, INT nEnergyTH);

// Purpose: Get engine energy threshold
// hDSpotter(IN): a handle of the recognizer
// Return: energy threshold(RMS) or error code
DSPDLL_API INT DSpotter_GetEnergyTH(HANDLE hDSpotter);

// Purpose: Set Cmd response time
// hDSpotter(IN): a handle of the recognizer
// nCmdIdx(IN): the command ID. It is 0 based
// nResponseTime(IN): response time
// Return: Success or error code
DSPDLL_API INT DSpotter_SetCmdResponseTime(HANDLE hDSpotter, INT nCmdIdx, INT nResponseTime);

// Purpose: Set Cmd reward
// hDSpotter(IN): a handle of the recognizer
// nCmdIdx(IN): the command ID. It is 0 based
// nReward(IN): reward
// Return: Success or error code
DSPDLL_API INT DSpotter_SetCmdReward(HANDLE hDSpotter, INT nCmdIdx, INT nReward);

// Purpose: Get Cmd response time
// hDSpotter(IN): a handle of the recognizer
// nCmdIdx(IN): the command ID. It is 0 based
// Return: response time or error code
DSPDLL_API INT DSpotter_GetCmdResponseTime(HANDLE hDSpotter, INT nCmdIdx);

// Purpose: Get Cmd reward
// hDSpotter(IN): a handle of the recognizer
// nCmdIdx(IN): the command ID. It is 0 based
// *pnErr(IN/Out): error code
// Return: reward
DSPDLL_API INT DSpotter_GetCmdReward(HANDLE hDSpotter, INT nCmdIdx, INT *pnErr);

#ifdef __cplusplus
}
#endif

#endif // __DSPOTTERSDK_API_H
