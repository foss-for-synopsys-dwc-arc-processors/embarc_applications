#include "Spotter.h"
#include "embARC.h"
#include "recorder.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Models.h"

// use external memory to avoid heap too small. External memory had poor
// performace. Don't use it, if possible
#define USE_FIXED_MEM
#define FIXED_MEM ((void *)0x30000000)
#define FIXED_MEM_SIZE (0x00020000)

static int status;

static int doRecognize() {
  int nRet;
  nRet = Recorder_Init();
  int16_t *pData;
  if (nRet != 0) {
    printf("record init fail: %d\r\n", nRet);
    return 1;
  }
  Recorder_Start();
  status = 0;
  while (1) {
    pData = Recorder_GetBuffer();
    if (pData == 0)
      continue;
    nRet = Spotter_AddSample(pData, Recorder_GetBufferSize());
    if (nRet == SPOTTER_SUCCESS) {
      /*switch(status)
      {

              // set pitch
              case 1 :
                      if(Spotter_GetResultGroupId()== 1){
                              status = 0;
                              switch(Spotter_GetResultId())
                              {
                                      case 0 :
                                              printf("設定直球\n");break;
                                      case 1 :
                                              printf("取消設定\n");break;
                              }
                              printf("Detect Trigger gpID=%d cmdID=%d\n",
      Spotter_GetResultGroupId(), Spotter_GetResultId()); }else{ printf("Wrong !
      This is status 1\n");
                      }
                      break;
              // set velocity
              case 2 :
                      if(Spotter_GetResultGroupId()== 2){
                              status = 0;
                              switch(Spotter_GetResultId())
                              {
                                      case 0 :
                                              printf("一百公里\n");break;
                                      case 1 :
                                              printf("一百一十\n");break;
                                      case 2 :
                                              printf("一百二十\n");break;
                                      case 3 :
                                              printf("一百三十\n");break;
                                      case 4 :
                                              printf("一百四十\n");break;
                                      case 5 :
                                              printf("一百五十\n");break;
                                      case 6 :
                                              printf("一百六十\n");break;
                                      case 7 :
                                              printf("一百七十\n");break;
                                      case 8 :
                                              printf("一百八十\n");break;
                                      case 9 :
                                              printf("一百九十\n");break;
                                      case 10 :
                                              printf("隨機設定\n");break;
                              }
                              printf("Detect Trigger gpID=%d cmdID=%d\n",
      Spotter_GetResultGroupId(), Spotter_GetResultId()); }else
      if(Spotter_GetResultGroupId()== 1 && Spotter_GetResultId() == 1){ status =
      0; printf("取消設定\n"); printf("Detect Trigger gpID=%d cmdID=%d\n",
      Spotter_GetResultGroupId(), Spotter_GetResultId()); }else{ printf("Wrong !
      This is status 2\n");
                      }
                      break;
              // set position
              case 3 :
                      if(Spotter_GetResultGroupId()== 3){
                              status = 0;
                              switch(Spotter_GetResultId())
                              {
                                      case 0 :
                                              printf("一號位置\n");break;
                                      case 1 :
                                              printf("二號位置\n");break;
                                      case 2 :
                                              printf("三號位置\n");break;
                                      case 3 :
                                              printf("四號位置\n");break;
                                      case 4 :
                                              printf("五號位置\n");break;
                                      case 5 :
                                              printf("六號位置\n");break;
                                      case 6 :
                                              printf("七號位置\n");break;
                                      case 7 :
                                              printf("八號位置\n");break;
                                      case 8 :
                                              printf("九號位置\n");break;
                              }
                              printf("Detect Trigger gpID=%d cmdID=%d\n",
      Spotter_GetResultGroupId(), Spotter_GetResultId()); }else
      if(Spotter_GetResultGroupId()== 1 && Spotter_GetResultId() == 1){ status =
      0; printf("取消設定\n"); printf("Detect Trigger gpID=%d cmdID=%d\n",
      Spotter_GetResultGroupId(), Spotter_GetResultId()); }else
      if(Spotter_GetResultGroupId()== 2 && Spotter_GetResultId() == 10){ status
      = 0; printf("隨機設定\n"); printf("Detect Trigger gpID=%d cmdID=%d\n",
      Spotter_GetResultGroupId(), Spotter_GetResultId()); }else{ printf("Wrong !
      This is status 3\n");
                      }
                      break;
              // no set
              case 0 :
                      if(Spotter_GetResultGroupId()== 0){
                              switch(Spotter_GetResultId())
                              {
                                      case 0 :
                                              printf("球種設定\n");status =
      1;break; case 1 : printf("球速設定\n");status = 2;break; case 2 :
                                              printf("位置設定\n");status =
      3;break; case 3 : printf("開始投球\n");break;
                              }
                              printf("Detect Trigger gpID=%d cmdID=%d\n",
      Spotter_GetResultGroupId(), Spotter_GetResultId()); }else{ printf("Wrong !
      This is status 0\n");
                      }
                      break;
      }*/
      /*if(Spotter_GetResultGroupId()== 0){
              printf("Detect Trigger gpID=%d cmdID=%d\n",
      Spotter_GetResultGroupId(), Spotter_GetResultId()); }else{ printf("Detect
      Command gpID=%d cmdID=%d\n", Spotter_GetResultGroupId(),
      Spotter_GetResultId());
      }*/
      if (Spotter_GetResultGroupId() == 0) {
        switch (Spotter_GetResultId()) {
        case 0:
          printf("0\n");
          break;
        case 1:
          printf("1\n");
          break;
        case 2:
          printf("2\n");
          break;
        case 3:
          printf("3\n");
          break;
        }
      } else if (Spotter_GetResultGroupId() == 1) {
        switch (Spotter_GetResultId()) {
        case 0:
          printf("4\n");
          break;
        case 1:
          printf("5\n");
          break;
        }
      } else if (Spotter_GetResultGroupId() == 2) {
        switch (Spotter_GetResultId()) {
        case 0:
          printf("6\n");
          break;
        case 1:
          printf("7\n");
          break;
        case 2:
          printf("8\n");
          break;
        case 3:
          printf("9\n");
          break;
        case 4:
          printf("10\n");
          break;
        case 5:
          printf("11\n");
          break;
        case 6:
          printf("12\n");
          break;
        case 7:
          printf("13\n");
          break;
        case 8:
          printf("14\n");
          break;
        case 9:
          printf("15\n");
          break;
        case 10:
          printf("16\n");
          break;
        }
      } else if (Spotter_GetResultGroupId() == 3) {
        switch (Spotter_GetResultId()) {
        case 0:
          printf("17\n");
          break;
        case 1:
          printf("18\n");
          break;
        case 2:
          printf("19\n");
          break;
        case 3:
          printf("20\n");
          break;
        case 4:
          printf("21\n");
          break;
        case 5:
          printf("22\n");
          break;
        case 6:
          printf("23\n");
          break;
        case 7:
          printf("24\n");
          break;
        case 8:
          printf("25\n");
          break;
        }
      }
    }
    /*else if(nRet!= SPOTTER_ERR_NeedMoreSample){
            printf("recognize fail: %d\n" , nRet);
            break;
    }*/
    Recorder_NextBuffer();
  }
  Recorder_Stop();
  Recorder_Release();
  return nRet;
}

int main(int argc, char **argv) {
  int nRet;
  int nSize;
  void *pMem;
  printf("\r\nSpotter version: %s\r\n", Spotter_GetVersion());

#ifndef USE_FIXED_MEM
  //	nSize= Spotter_GetMemoryUsage((void*)pCYBase_mod, (const
  //void*[]){(void*)pGroup_1_mod, 0});
  nSize = Spotter_GetMemoryUsage(
      (void *)pCYBase_mod,
      (const void *[]){(void *)pGroup_1_mod, (void *)pGroup_2_mod,
                       (void *)pGroup_3_mod, (void *)pGroup_4_mod, 0});
  if (nSize <= 0) {
    printf("wrong model: %d\n", nSize);
    goto Exit;
  }
  pMem = malloc(nSize);
  if (nSize <= 0) {
    printf("malloc size %d fail\n", nSize);
    goto Exit;
  }
#else
  nSize = FIXED_MEM_SIZE;
  pMem = FIXED_MEM;
#endif

  printf("\r\nModel info: %s %s\r\n", CYBase_mod_DIRNAME,
         CYBase_mod_DATE); // model info in CYBase.mod.h
                           //	nRet= Spotter_Init((void*)pCYBase_mod, (const
                           //void*[]){(void*)pGroup_1_mod, 0}, pMem, nSize);
  nRet = Spotter_Init(
      (void *)pCYBase_mod,
      (const void *[]){(void *)pGroup_1_mod, (void *)pGroup_2_mod,
                       (void *)pGroup_3_mod, (void *)pGroup_4_mod, 0},
      pMem, nSize);
  if (nRet != 0) {
    printf("init fail: %d\n", nRet);
    goto Exit;
  }
  printf("Begin recognize\r\n");
  doRecognize();
Exit:
  Spotter_Release();
#ifndef USE_FIXED_MEM
  free(pMem);
#endif
  return 0;
}
