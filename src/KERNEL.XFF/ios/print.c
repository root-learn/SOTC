#include "common.h"

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/print", iosSPrintf);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/print", iosUsbSerialPrintf);

void iosCreatePrintMgr(void) {
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/print", iosGetPrintfMode);
