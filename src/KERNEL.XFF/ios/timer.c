#include "common.h"
#include "ios/message.h"
#include <time.h>
#include "ios/timer.h"
#include "ios/thread.h"




static s32 D_400454B8[] = {
    0x000F4240, 0
};


// .bss
static t_iosMessageQueue D_4007BC30;
static char D_4007BC50[0x1000];
static iosThread* D_4007CC50;
static s64 D_4007CC58;




// prototypes
void func_40012A78();



//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/timer", iosWaitIdleThread);
void iosWaitIdleThread(void)
{
    iosSendMsg(&D_4007BC30, 0, 1);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/timer", iosCreateIdleThread);
void iosCreateIdleThread(void)
{
    iosThread* tmp;
    
    iosCreateMsgQueue(&D_4007BC30, 0, 0);
    tmp = iosCreateThread(func_40012A78, D_4007BC50, 0x1000, 0x7D, 0);
    D_4007CC50 = tmp;
    iosStartThread(tmp, 0);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/timer", iosGetClock);
void iosGetClock(s64* arg0)
{
    *arg0 = D_4007CC58;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/timer", iosGetClockSecond);
s64 iosGetClockSecond(sceCdCLOCK* arg0)
{
    struct tm sp;

    if (arg0->stat != 0)
        return 0;
    
    sp.tm_sec = arg0->second;
    sp.tm_min = arg0->minute;
    sp.tm_hour = arg0->hour;
    sp.tm_mday = arg0->day;
    sp.tm_mon = arg0->month -1;
    sp.tm_year = arg0->year - 1900;
    sp.tm_wday = 0;
    sp.tm_yday = 0;
    sp.tm_isdst = 0;
    return mktime(&sp);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/timer", func_40012A78);
void func_40012A78(s32 arg0)
{
    s32 temp_v1;
    sceCdCLOCK sp;

    while (1)
    {
        iosRecvMsg(&D_4007BC30, NULL, 0);
        if (--D_400454B8[0] >= 0)
            continue;
        
        if (sceCdReadClock(&sp) == 1 && sp.stat == 0)
            D_4007CC58 = *(s64*)&sp;
        
        D_400454B8[0] = 0xF4240;
    }
}
