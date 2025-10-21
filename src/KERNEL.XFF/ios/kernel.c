#include "common.h"
#include "ios/kernel.h"
#include "loaderSys.h"
#include "loaderSysFileIO.h"


extern void IosCdvdManagerSimulation();
extern void IosLoadIrxSimulation();
extern void bootSchedulerThread();
extern void initSound();
extern void iosDeleteThread(s32);
extern void iosKeybdInit();
extern void iosMcInit();
extern void iosMouseInit();
extern void iosInitIntrSystem();
extern void iosInitMallocSystem(void*, void*, s32);
extern void initDldSys();
extern void InitMath();
extern void InitNet();
extern void InitGp();
extern void iosInit();
extern void isysInitObjSystem();
extern void InitDld();
extern void InitIopDev();
extern void InitSubThread();
extern s32 iosCreateThreadS(void*, s32, s32*, s32, s32, s32);
extern void iosStartThread(s32, s32);


extern f32 atof(char*);
void func_40018D30();
//extern char D_40046D88;
//extern char D_40046DD8;
//extern char D_40046E00;
//extern char D_40046E28;
//extern char D_40046E58;
//extern char D_40046EB8;
extern s32 D_40081588;
extern void* __loader_version_id__;
extern s32 rootPtn;



// .bss
static char D_40081188[0x400];




INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/kernel", ioskernel);
// 91% matched
/*s32 ioskernel(s32 arg0, void* arg1) {
    memory_info meminfo;
    void (*LoaderSysPrintf)(char*,...);
    void (*LoaderSysJumpRecoverPoint)(char*,...);
    void* tmp;

    LoaderSysPrintf = *(void**)0x100000;
    LoaderSysJumpRecoverPoint = *(void**)0x100004;
    if (arg0 != 0) {
        LoaderSysPrintf("krnl:\tThis is a CD boot environment. \x1b[36mNeed loader version check:\x1b[m\n");
        LoaderSysPrintf("krnl:\t\t   boot loader version: %x\n", arg1);
        LoaderSysPrintf("krnl:\t\tcurrent loader version: %x\n", &__loader_version_id__);
        if (arg1 == &__loader_version_id__)
            LoaderSysPrintf("krnl:\t\t\x1b[36mloader version check passed.\x1b[m\n");
        else
            LoaderSysJumpRecoverPoint("krnl:\t\x1b[31mloader version does not match. (boot:%x != current:%x)\x1b[m\nkrnl:\tsystem down.\n", arg1, &__loader_version_id__);
    }
    atof("1.f");
    LoaderSysGetMemoryInfo(&meminfo);
    iosInitIntrSystem();
    iosInitMallocSystem(meminfo.unk18, meminfo.stack_base2, 0xDAE000);
    initDldSys();
    InitMath();
    InitNet();
    InitGp();
    iosInit();
    isysInitObjSystem();
    InitDld();
    InitIopDev();
    InitSubThread();
    D_40081588 = iosCreateThreadS(func_40018D30, 0, &rootPtn, 0x10000, 0x2d, 0);
    iosStartThread(D_40081588, 0);
    return 0;
}*/

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/kernel", iosJumpRecoverPoint);
void iosJumpRecoverPoint(const char* msg, ...)
{
    va_list args;
    
    va_start(args, msg);
    vsprintf(D_40081188, msg, args);
    va_end(args);
    
    LoaderSysJumpRecoverPoint(D_40081188);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/kernel", iosResetKernel);
void iosResetKernel(void)
{
    LoaderSysResetSystem();
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/kernel", func_40018D30);
void func_40018D30(void) {
    IosLoadIrxSimulation();
    initSound();
    iosMcInit();
    bootSchedulerThread();
    iosMouseInit();
    iosKeybdInit();
    IosCdvdManagerSimulation();
    iosDeleteThread(0);
}
