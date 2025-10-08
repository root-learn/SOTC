#include "common.h"
#include "ios/system.h"
#include "ios/thread.h"
#include "sdk/ee/eekernel.h"
#include "sdk/ee/eeregs.h"
#include "sdk/ee/sif.h"
#include "ios/kernel.h"

extern void iosCreateIdleThread();
extern void iosCreateIopMgr();
extern void iosCreatePrintMgr();
extern void iosInitMessageSystem();
extern void iosInitThreadSystem();
extern s32 CreateSema(struct SemaParam *);
extern void LoaderSysEntryExternalSemaList(s32 arg1);
extern void iosSignalIopSema();
extern void iosWaitIopSema();
extern void iosAddIntcHandler(s32 arg0, void* arg1);


// prototypes
void func_40013058();


// .data
static s32 D_40045544 = 0;
static f32 D_40045548[] = {
    1.5e+8,
    9375000.0,
    585937.0
};
static s32 D_40045554 = 0;
s32 iosSystemSema = 0;

// .rodata
//extern char D_40046518;
//extern char D_40046530;
//extern char D_40046548;
//extern char D_40046578;

// .bss
static volatile s64 D_4007CC60;
static s32 D_4007CC68;
static s32 D_4007CC6C;
static void (*D_4007CC70[])();







//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosInit);
void iosInit()
{
    iosInitThreadSystem();
    iosInitMessageSystem();
    iosCreateIdleThread();
    iosCreatePrintMgr();
    iosCreateIopMgr();
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosAddIntcHandler);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosCreateSema);
s32 iosCreateSema(s32 arg0, s32 arg1)
{
    struct SemaParam sparam;
    s32 temp_v0;

    sparam.initCount = arg0;
    sparam.maxCount = arg1;

    temp_v0 = CreateSema(&sparam);
    LoaderSysEntryExternalSemaList(temp_v0);
    return temp_v0;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosDeleteSema);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosInitIntrSystem);
void iosInitIntrSystem()
{
    s32 sema;
    struct SemaParam semaParam;

    *T0_MODE = 0;
    *T1_MODE = 0;
    iosAddIntcHandler(9, func_40013058);
    *T0_COUNT = 0;
    *T1_COUNT = 0;
    *T0_MODE = 0x280;
    *T1_MODE = 0x80;
    
    semaParam.initCount = 1;
    semaParam.maxCount = 1;
    sema = CreateSema(&semaParam);
    LoaderSysEntryExternalSemaList(sema);
    iosSystemSema = sema;
    D_4007CC68 = -1;
    D_4007CC6C = 0;
}


//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosGetTCount);
s64 iosGetTCount()
{
    return D_4007CC60;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosGetTCountExtension);
s64 iosGetTCountExtension()
{
    return D_4007CC60 | *T0_COUNT;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosGetSecondFromTCount);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosResetCpuRapCounter);
void iosResetCpuRapCounter(u32 arg0)
{
    D_40045544 = arg0;
    *T1_MODE = arg0 | 0x80;
    *T1_COUNT = 0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosGetCpuRapCountPar1Int);
f32 iosGetCpuRapCountPar1Int()
{
    f32 var_f0;
    
    var_f0 = (f32)*T1_COUNT;
    return (var_f0 * 60.0f) / D_40045548[D_40045544];
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosDI);
s32 iosDI()
{
    if (DIntr() != 0)
    {
        if (D_40045554 != 0)
//            iosJumpRecoverPoint(&D_40046518, D_40045554);
            iosJumpRecoverPoint("iosDI: DICnt != 0   %d\n", D_40045554);
        
    }
    else if (D_40045554 <= 0)
//        iosJumpRecoverPoint(&D_40046530, D_40045554);
        iosJumpRecoverPoint("iosDI: DICnt <= 0   %d\n", D_40045554);
    
    D_40045554++;
    return D_40045554;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosEI);
s32 iosEI()
{
    D_40045554--;
    if (D_40045554 == 0)
    {
        if (EIntr() != 0)
//            iosJumpRecoverPoint(&D_40046548);
            iosJumpRecoverPoint("iosEI: yobareru maekara warikomi enable\n");
        
    }
    else if (D_40045554 < 0)
//        iosJumpRecoverPoint(&D_40046578);
        iosJumpRecoverPoint("iosEI: DICnt < 0\n");
    
    return D_40045554;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosLoaderResetEntry);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", func_40013058);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", func_40013090);
void func_40013090()
{
    s32 i;

    for (i=0; i<2; i++)
    {
        if (D_4007CC70[i] != NULL)
            D_4007CC70[i]();
    }
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosEE2IopRequest);
void iosEE2IopRequest(s32 data, s32 addr, s32 size)
{
    s32 temp;
    sceSifDmaData sifDmaData;

    sifDmaData = (sceSifDmaData)
    {
        .data = data,
        .addr = addr,
        .size = size,
        .mode = 0
    };
    
    FlushCache(0);
    iosWaitIopSema();
    temp = sceSifSetDma(&sifDmaData, 1);
    while (sceSifDmaStat(temp) >= 0){}
    iosSignalIopSema();
    FlushCache(0);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/system", iosEE2IopEndChk);
s32 iosEE2IopEndChk()
{
    return 1;
}

void iosCreateIopMgr(void)
{}



