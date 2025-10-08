#include "common.h"
#include "ios/kernel.h"

// .rodata
//extern char D_40046430;


//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/assert", _iosAssert);
void _iosAssert(char* arg0, s32 arg1, char* arg2)
{
    iosJumpRecoverPoint("assertion failed...\n\tat file:\"%s\" line:%d\n\t%s", arg0, arg1, arg2);
//    iosJumpRecoverPoint(&D_40046430, arg0, arg1, arg2);
}
