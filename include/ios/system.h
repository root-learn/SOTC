#ifndef _SYSTEM_H
#define _SYSTEM_H


extern s32 iosSystemSema;

void iosInit();
s32 iosCreateSema(s32 arg0, s32 arg1, s32 arg2);
void iosDeleteSema(s32 arg0);
void iosInitIntrSystem();
s64 iosGetTCount();
s64 iosGetTCountExtension();
f32 iosGetSecondFromTCount(u64 arg0);
void iosResetCpuRapCounter(u32 arg0);
f32 iosGetCpuRapCountPar1Int();
s32 iosDI();
s32 iosEI();
void iosEE2IopRequest(s32 data, s32 addr, s32 size);
s32 iosEE2IopEndChk();
void iosCreateIopMgr(void);







#endif // _SYSTEM_H
