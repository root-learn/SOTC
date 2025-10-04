#include "common.h"
#include "ios/kernel.h"
#include "loaderSys.h"


static char D_40081188[0x400];




INCLUDE_ASM("asm/kernel/nonmatchings/ios/kernel", ioskernel);

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/kernel", iosJumpRecoverPoint);
void iosJumpRecoverPoint(const char* msg, ...)
{
    va_list args;
    
    va_start(args, msg);
    vsprintf(D_40081188, msg, args);
    va_end(args);
    
    LoaderSysJumpRecoverPoint(D_40081188);
}

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/kernel", iosResetKernel);
void iosResetKernel(void)
{
    LoaderSysResetSystem();
}

INCLUDE_ASM("asm/kernel/nonmatchings/ios/kernel", func_40018D30);
