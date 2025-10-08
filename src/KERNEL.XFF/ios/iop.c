#include "common.h"

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosAllocIopHeap);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosFreeIopHeap);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosLoadIopModule);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosGetIopTotalFreeMemSize);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosGetIopTotalMemSize);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosWaitIopSema);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", iosSignalIopSema);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/iop", InitIopDev);
