#ifndef _THREAD_H
#define _THREAD_H

#include "sdk/ee/eekernel.h"


typedef struct iosThread{
    s32 unk0;
    s32 unk4;
    struct ThreadParam* unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    struct iosThread* unk1C;
    struct iosThread* unk20;
    s32 unk24;
    struct iosThread* unk28;
} iosThread;



void iosCallBackThreadMgr(void*, iosThread*);
iosThread* iosGetThread();
void iosSleepThread();
s32 iosWakeupThread(iosThread*);



#endif // _THREAD_H
