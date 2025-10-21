#ifndef _THREAD_H
#define _THREAD_H

#include "sdk/ee/eekernel.h"


typedef struct iosThread{
    s32 unk0;
    s32 unk4;
    struct ThreadParam* unk8;
    void* unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    struct iosThread* unk1C;
    struct iosThread* unk20;
    s32 unk24;
    struct iosThread* unk28;
} iosThread;



iosThread* iosCreateThread(void*, void*, s32, s32, s32);
void iosInitThreadSystem(void);
iosThread* iosGetThread();
iosThread* iosCreateThreadS(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void iosStartThread(iosThread* thread, void *arg1);
void iosSleepThread();
void iosiWakeupThread(iosThread* arg0);
void iosChangeThreadPriority(iosThread* arg0, s32 arg1);
void iosDeleteThread(iosThread* arg0);
void iosCallBackThreadMgr(void* arg0, s32 arg1);




#endif // _THREAD_H
