#include "common.h"

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", func_4002F2A8);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepTickProc);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepReq);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepPlayerSrh);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepPlayerSrh2);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepStopReq);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepSetVol);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepSetPan);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepSetDefaultParam);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepPauseAll);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepPause);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepCont);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepStopTypeAll);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepInitTickParam);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndSepInit);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndVabCreate);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndVabDelete);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndVabHDOpen);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndVabHDClose);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndBDOpen);

void sndBDClose(void) {
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", sndVabInit);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", initSound);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", tickProcSound);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", addAdpcmTickProcFunc);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", setPcmTickProcFunc);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", soundGetTickProcCnt);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", soundSetRevMode);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", soundGetRevMode);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", getSlotInfo);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/snd", setSoundMode);
