#include "common.h"
//#include "sdk/ee/sifrpc.h"
#include "sdk/ee/eekernel.h"


extern int sceSifCallRpc(void *, unsigned int, unsigned int, void *, int, void *, int, void*);

// .data
static s32 D_40045EB4 = 0;
static void* D_40045EB8 = 0;
static s32 D_40045EBC = 0;
static s32 D_40045EC0 = 0;

// .bss
static s32 D_400F6DC0;
//extern sceSifClientData D_400F6DC8;
static char D_400F6DC8[56];
static char D_400F6E00[4224];
static s32 D_400F7EA0;





INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/dev", func_400303A8);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/dev", iosKeybdInit);
// The order of restoring save registers is different, functionally works though
//s32 iosKeybdInit(void)
//{
//    D_40045EB4 = 0;
//    sceSifInitRpc(0);
//    return D_40045EB4;
//}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/dev", iosKeybdSetCallBackFunc);
s32 iosKeybdSetCallBackFunc(void* arg0)
{
    if (!D_40045EB4)
    {
        return 0;
    }
    {
        D_40045EB8 = arg0;
        return 1;
    }
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/dev", func_40030620);
void func_40030620(void) {
    iSignalSema(D_400F6DC0);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/dev", iosKeybdGetInfo);
void iosKeybdGetInfo(void* arg0)
{
    s32 sp0[2];
    WaitSema(D_400F6DC0);
    sp0[0] = 0;
    sceSifCallRpc(D_400F6DC8, 1, 1, D_400F6E00, 32, arg0, 32, &func_40030620);
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/dev", func_400306A8);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/dev", iosMouseInit);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/dev", iosMouseSetCallBackFunc);
s32 iosMouseSetCallBackFunc(s32 arg0)
{
    if (!D_40045EBC)
    {
        return 0;
    }
    else {
        D_40045EC0 = arg0;
        return 1;
    }
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/dev", func_40030970);
void func_40030970(void) {
    iSignalSema(D_400F7EA0);
}



