#include "common.h"
#include "ios/iop.h"
#include "sdk/ee/sifdev.h"
#include "loaderSys2.h"
#include "loaderSysFileIO.h"
#include "sdk/ee/sifrpc.h"


extern s32 iosLoaderFOpen(char*, s32, s32);
extern s32 iosLoaderGetFileSize(s32);
extern s32 rootPtn;
extern void* iosMallocAlign(s32, s32, s32);
extern void iosFree(void*);
extern s32 iosLoaderFRead(s32, s32, s32);
extern s32 iosLoaderFClose(s32);



// .bss
static s32 D_400F6D90;







//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosAllocIopHeap);
void* iosAllocIopHeap(s32 arg0)
{
    void* temp_s0;

    iosWaitIopSema();
    temp_s0 = sceSifAllocIopHeap(arg0);
    iosSignalIopSema();
    if (temp_s0 != NULL)
    {
        iosWaitIopSema();
        LoaderSysEntryExternalIopMemoryList((s32)temp_s0);
        iosSignalIopSema();
    }
    return temp_s0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosFreeIopHeap);
s32 iosFreeIopHeap(void* arg0)
{
    s32 result;

    iosWaitIopSema();
    sceSifFreeIopHeap(arg0);
    result = LoaderSysDeleteExternalIopMemoryList((s32)arg0);
    if (result >= 0)
        result = 0;
    else
        result = -1;
    iosSignalIopSema();
    return result;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosLoadIopModule);
s32 iosLoadIopModule(char* arg0, char* arg1, s32 arg2, char* arg3)
{
    s32 temp_s1;
    s32 temp_v0;
    s32 temp_v0_2;
    void* temp_v0_3;
    s32 var_v0;

    temp_v0 = iosLoaderFOpen(arg0, 1, 0);
    var_v0 = -1;
    if (temp_v0 >= 0)
    {
        temp_v0_2 = iosLoaderGetFileSize(temp_v0);
        temp_v0_3 = iosMallocAlign((s32) &rootPtn, temp_v0_2, 0x40);
        iosLoaderFRead(temp_v0, (s32)temp_v0_3, temp_v0_2);
        temp_s1 = LoaderSysLoadIopModuleFromEEBuffer(arg1, (s32)temp_v0_3, temp_v0_2, arg2, arg3);
        iosFree(temp_v0_3);
        iosLoaderFClose(temp_v0);
        var_v0 = temp_s1;
    }
    return  var_v0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosGetIopTotalFreeMemSize);
u32 iosGetIopTotalFreeMemSize(void)
{
    u32 temp_s0;

    iosWaitIopSema();
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    temp_s0 = sceSifQueryTotalFreeMemSize();
    iosSignalIopSema();
    return temp_s0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosGetIopTotalMemSize);
s32 iosGetIopTotalMemSize(void)
{
    return (LoaderSysCheckCDBootMode() == 0) ? 0x800000 : 0x200000;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosWaitIopSema);
s32 iosWaitIopSema(void)
{
    return WaitSema(D_400F6D90);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosSignalIopSema);
s32 iosSignalIopSema(void)
{
    return SignalSema(D_400F6D90);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", InitIopDev);
void InitIopDev(void)
{
    D_400F6D90 = iosCreateSema(1, 1, 0);
}


