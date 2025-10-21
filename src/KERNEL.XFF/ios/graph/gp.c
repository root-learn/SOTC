#include "common.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/libgraph.h"
#include "sdk/ee/eeregs.h"
#include "ios/thread.h"
#include "ios/system.h"


extern void iosGsInitDC();
extern void iosCopyFrameBuffer();
extern void iosGsResetCRTC();
extern void iosDlSwap();
extern void iosDlTrans();
extern void iosSyncV(s32);
extern void iosSyncPath(s32, s32);

extern void InitDL(s32, s32);
extern void iosSetScreenEnv(s32, s32, s32, s32);
extern void iosAddIntcHandler(s32, void*);

extern volatile s32 gpMainCallCounter;




// .data
extern s32 D_40045AB8;
extern s32 D_40045ABC;
extern volatile s32 D_40045AC0;
extern volatile s32 D_40045AC4;
extern volatile s64 D_40045AC8;
extern volatile s32 D_40045AD0;
extern void (*D_40045AD4)(); // i start function
extern void (*D_40045AD8)(); // i finish function
extern void (*D_40045ADC)(); // gp finish function
extern void (*D_40045AE0)(); // dl trans function
extern void (*D_40045AE4)(); // v scheduler function
extern void (*D_40045AE8)();
extern s32 D_40045AEC;
extern struct unkstrc {
    struct unkstrc *unk0;
    s64 *unk4, *unk8;
    volatile s64* unkC;
    s32 unk10, unk14, unk18, unk1C;
} volatile *D_40045AF4;
extern s32 D_40045AF8;


// .bss
extern volatile s64 D_40092C90;
extern iosThread* D_40092C98;
extern s32 D_40092CA0;



// prototypes
s32 func_40028600(void);
void func_400286E8();









INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", InitGp);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosWaitDMA);
s32 iosWaitDMA(void)
{
    asm(".L400281D0: bc0f .L400281D0; nop");
    return 0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosWaitVU1);
s32 iosWaitVU1(void)
{
    asm(".L400281F8: bc2t .L400281F8; nop;");
    return 0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", getCRTCoffsetU);
s32* getCRTCoffsetU(void)
{
    return &D_40045AB8;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", getCRTCoffsetV);
s32* getCRTCoffsetV(void)
{
    return &D_40045ABC;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosGsChangeScreenOffset);
void iosGsChangeScreenOffset(void)
{
    func_400268A8();
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosSyncGP);
void iosSyncGP(void)
{
    FlushCache(0);
    iosGsInitDC();
    iosCopyFrameBuffer();
    if (D_40045AE0 != NULL)
        D_40045AE0();
    iosGsResetCRTC();
    D_40045AD0 = 1;
    sceGsPutIMR(sceGsGetIMR() & ~0x200);
    iosDlSwap();
    iosDlTrans();
    FlushCache(0);
    if (D_40045AE0 != NULL)
        D_40045AE0();
    iosGsResetCRTC();
    D_40045AD0 = 1;
    sceGsPutIMR(sceGsGetIMR() & ~0x200);
    iosDlSwap();
    iosDlTrans();
    iosSyncPath(0, 0);
    iosSyncV(0);
    iosSyncV(0);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", bootGPThread);
void bootGPThread(void* arg0)
{
    InitDL(0x7C80, 0x20);
    iosSetScreenEnv(0x200, 0x1C0, 0x200, 0x1C0);
    D_40092C90 = iosGetTCount();
    *GS_CSR |= 2;
    sceGsPutIMR(sceGsGetIMR() & ~0x200);
    iosAddIntcHandler(0, func_40028600);
    D_40045AE8 = arg0;
    D_40092C98 = iosCreateThread(func_400286E8, &D_40092CA0, 0x8000, 0x2A, 0);
    iosStartThread(D_40092C98, NULL);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosTransGP);
void iosTransGP(void)
{
    if (D_40045AE0 != NULL)
        D_40045AE0();
    iosGsResetCRTC();
    D_40045AD0 = 1;
    sceGsPutIMR(sceGsGetIMR() & ~0x200);
    iosDlSwap();
    iosDlTrans();
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosChkGpDlTransExec);
s32 iosChkGpDlTransExec(void)
{
    s32 var_a0;

    var_a0 = 0;
    if ((*D1_CHCR & 0x100) || (*D2_CHCR & 0x100))
        var_a0 = 1;
    return var_a0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosGPFrameInit);
void iosGPFrameInit(void)
{
    iosGsInitDC();
    iosCopyFrameBuffer();
    if (D_40045AE0 != NULL)
        D_40045AE0();
    iosGsResetCRTC();
    D_40045AD0 = 1;
    sceGsPutIMR(sceGsGetIMR() & ~0x200);
    iosDlSwap();
    iosDlTrans();
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosSetIStartFunc);
s32 iosSetIStartFunc(void* arg0)
{
    D_40045AD4 = arg0;
    return 1;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosSetGpFinishFunc);
s32 iosSetGpFinishFunc(void* arg0)
{
    D_40045ADC = arg0;
    return 1;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosSetIFinishFunc);
s32 iosSetIFinishFunc(void* arg0)
{
    D_40045AD8 = arg0;
    return 1;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", getIntrGpCount);
s64 getIntrGpCount()
{
    return D_40045AC8;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosSetDlTransFunc);
s32 iosSetDlTransFunc(void* arg0)
{
    D_40045AE0 = arg0;
    return 1;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosSetVSchedulerFunc);
s32 iosSetVSchedulerFunc(void* arg0)
{
    D_40045AE4 = arg0;
    return 1;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosWaitGPFinish);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", func_40028600);
s32 func_40028600(void)
{
    u64 temp_s0;

    temp_s0 = isceGsPutIMR(0xFF00U);
    D_40045AC8 += 1;
    if (D_40045ADC != NULL)
        D_40045ADC();
    
    *GS_CSR |= 2;
    isceGsPutIMR(temp_s0);
    ExitHandler();
    return 0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iStart);
void iStart(void)
{
    if (D_40045AD4 != NULL)
        D_40045AD4();
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iFinish);
void iFinish(void)
{
    if (D_40045AD8 != NULL)
        D_40045AD8();
    D_40045AC0 = 1;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", func_400286E8);
/*void func_400286E8(void)
{
    while(1)
    {
        iosDebPcVsyncCounterCheck();
        waitVscheduler();
        iosPushVu0Registers();
        gpMainCallCounter++;
        
        if (D_40045AE4 != NULL)
            D_40045AE4();
        
        D_40045AC4 = iosCheckDrawFinish();
        if (D_40045AC4 != 0)
        {
            if (D_40045AC0 != 0)
            {
                D_40045AC0 = 0;
                D_40045AEC = D_40045AC0;
            }
            if (D_40045AE8 != 0)
                D_40045AE8();
            
            D_40045AD0 = 0;
        }
        else
        {
            if (++D_40045AEC >= 121)
                iosSyncPath(0, 0);
        }
        iosPopVu0Registers();
    }
}*/

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", gsRegBegin);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", gsRegEnd);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", gsRegEndWithPtr);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", gsReglistBegin);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", gsReglistEnd);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", gsBox);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", gsBox2);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", gsBoxLine);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", spriteNoTexFast);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", spriteTexFast);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", gsReg);
void gsReg(s64 arg0, s64 arg1)
{
    if (D_40045AF4->unk1C == 0)
    {
        *(D_40045AF4->unkC++) = arg1;
        *(D_40045AF4->unkC++) = arg0;
    }
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", getSpriteDrawFirst);
s32* getSpriteDrawFirst()
{
    return &D_40045AF8;
}

void gsReglist(void)
{}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", iosTransTex);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/graph/gp", getTWH);



