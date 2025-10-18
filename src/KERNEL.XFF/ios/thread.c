#include "common.h"
#include "ios/thread.h"
#include "ios/system.h"
#include "ios/memory.h"


// .rodata
extern char D_40046D10;
extern char D_40046D48;
extern char D_40046D70;



// .bss
extern struct ThreadParam D_4007E1E0[32];
extern iosThread D_4007E7E0[32];
extern iosThread* D_4007ED60[256];
extern iosThread* D_4007F160;
extern iosThread* D_4007F164;
extern char D_4007F170[0x2000];
extern iosThread* D_40081170;
extern iosThread* D_40081174;
extern void (*D_40081178)(s32);
extern s32 D_4008117C;
extern s32 D_40081180;
extern s32 D_40081184;





INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosCreateThread);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", func_40018530);
void func_40018530(void)
{
    void (*temp_v0)(iosThread*);
    s32 temp_s1;
    iosThread* temp_a0;
    iosThread* temp_s0;
    iosThread* temp_v1;
    s32* tmp;
    iosThread** tmp2;
    
    tmp = &D_40081184;
    *tmp = iosCreateSema(1, 1, 0);
    while(1)
    {
        SleepThread();
        switch (D_40081180)
        {
            case 0:
                temp_s0 = D_40081174;
                temp_v0 = temp_s0->unkC;
                if (temp_v0 != NULL)
                    temp_v0(temp_s0);
                
                temp_s1 = temp_s0->unk4;
                temp_v1 = temp_s0->unk20;
                temp_a0 = temp_s0->unk1C;
                if (temp_s0 == D_4007F164)
                    D_4007F164 = temp_v1;
                
                if (temp_a0 != NULL)
                    temp_a0->unk20 = temp_v1;
                
                if (temp_v1 != NULL)
                    temp_v1->unk1C = temp_a0;
                
                temp_s0->unk1C = NULL;
                tmp2 = &D_4007F160;
                temp_s0->unk20 = *tmp2;
                
                if (*tmp2 != NULL)
                    (*tmp2)->unk1C = temp_s0;
                
                *tmp2 = temp_s0;
                TerminateThread(temp_s1);
                DeleteThread(temp_s1);
                LoaderSysDeleteExternalThreadList(temp_s1);
                if (temp_s0->unk18 & 1)
                    iosFree(temp_s0->unk8->stack);
                
                temp_s0->unk0 = 0;
                D_4007ED60[temp_s1] = NULL;
                break;
            case 1:
                D_40081178(D_4008117C);
                break;
            default:
                iosJumpRecoverPoint(&D_40046D70);
                break;
        }
        SignalSema(D_40081184);
    }
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosInitThreadSystem);
void iosInitThreadSystem(void)
{
    iosThread* temp_v0_2;
    s32 i;

    memset(D_4007ED60, 0, sizeof(iosThread*)*256);
    memset(D_4007E7E0, 0, sizeof(iosThread)*32);
    
    for(i=0; i<32; i++)
        D_4007E7E0[i].unk8 = D_4007E1E0 + i;
    
    D_4007F160 = D_4007E7E0;
    D_4007F164 = NULL;
    
    for (i=0; i<31; i++)
        D_4007E7E0[i].unk20 = D_4007E7E0 + i + 1;

    D_4007E7E0[31].unk20 = NULL;
    D_4007E7E0[0].unk1C = NULL;
    
    for (i=1; i<32; i++)
        D_4007E7E0[i].unk1C = D_4007E7E0 + i - 1;
    
    temp_v0_2 = iosCreateThread(func_40018530, D_4007F170, 0x2000, 2, 0);
    D_40081170 = temp_v0_2;
    
    StartThread((temp_v0_2 == NULL ? D_4007ED60[GetThreadId()] : temp_v0_2)->unk4, NULL);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosGetThread);
iosThread* iosGetThread(void)
{
    return D_4007ED60[GetThreadId()];
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosCreateThreadS);
iosThread* iosCreateThreadS(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    iosThread* var_s0;
    void* temp_v0;

    temp_v0 = iosMallocAlign(arg2, arg3, 0x10);
    var_s0 = NULL;
    if (temp_v0 == NULL)
        iosJumpRecoverPoint(&D_40046D10);
    
    else
    {
        var_s0 = iosCreateThread(arg0, temp_v0, arg3, arg4, arg5);
        if (var_s0 != NULL)
            var_s0->unk18 |= 1;
        else
            iosFree(temp_v0);
    }
    return var_s0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosStartThread);
void iosStartThread(iosThread* thread, void *arg1)
{

    if (thread == NULL)
        thread = D_4007ED60[GetThreadId()];
    
    StartThread(thread->unk4,  arg1);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosSleepThread);
void iosSleepThread(void)
{
    SleepThread();
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosWakeupThread);
void iosWakeupThread(iosThread* arg0)
{
    if (arg0 == NULL)
        arg0 = D_4007ED60[GetThreadId()];
    
    if (WakeupThread(arg0->unk4) < 0)
        iosJumpRecoverPoint(&D_40046D48);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosiWakeupThread);
void iosiWakeupThread(iosThread* arg0)
{
    iWakeupThread(arg0->unk4);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosChangeThreadPriority);
void iosChangeThreadPriority(iosThread* arg0, s32 arg1)
{
    if (arg0 == NULL)
        arg0 = D_4007ED60[GetThreadId()];
    
    arg0->unk14 = arg1;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosGetThreadPriority);
s32 iosGetThreadPriority(iosThread* thread)
{
    if (thread == NULL)
        thread = D_4007ED60[GetThreadId()];
    
    thread->unk14 = 0;
    return 0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosDeleteThread);
void iosDeleteThread(iosThread* arg0)
{
    WaitSema(D_40081184);
    if (arg0 == 0)
        arg0 = D_4007ED60[GetThreadId()];

    D_40081180 = 0;
    D_40081174 = arg0;
    
    WakeupThread(D_40081170->unk4);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/thread", iosCallBackThreadMgr);
void iosCallBackThreadMgr(void* arg0, s32 arg1)
{
    WaitSema(D_40081184);
    D_40081180 = 1;
    D_40081178 = arg0;
    D_4008117C = arg1;
    WakeupThread(D_40081170->unk4);
}
