#ifndef _IOS_IOP_H
#define _IOS_IOP_H



void* iosAllocIopHeap(s32 arg0);
s32 iosFreeIopHeap(void* arg0);
s32 iosLoadIopModule(char* arg0, char* arg1, s32 arg2, char* arg3);
u32 iosGetIopTotalFreeMemSize(void);
s32 iosGetIopTotalMemSize(void);
s32 iosWaitIopSema(void);
s32 iosSignalIopSema(void);
void InitIopDev(void);



#endif // _IOS_IOP_H
