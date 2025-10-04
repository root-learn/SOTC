#include "common.h"
#include "ios/cd.h"
#include "gcc/string.h"
#include "ios/kernel.h"



// .data
static void (*D_400454B0)() = NULL;
static void (*D_400454B4)() = NULL;

// .bss
static sceCdRMode D_40073BC0;
static sceCdRMode D_40073BC8;
static sceCdRMode* D_40073BCC;
static sceCdRMode* D_40073BD0;
static s32 D_40073BD4;
static s32 D_40073BD8;
static s32 D_40073BDC;
static char D_40073C00[0x8000];
static s64 D_4007BC00;
static s64 D_4007BC08;
static s64 D_4007BC10;
static char D_4007BC18[0x18];

// .rodata
//extern const char D_40046460;
//extern const char D_40046468;




//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", func_400121D0);
void func_400121D0(s32 lbn)
{
    sceCdlFILE fp;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s0;
    s32 var_s0_2;

    D_4007BC00 = (u64)lbn << 11;
    if (D_40073BD8 == 0)
    {
    UNKNOWN_LABEL:
        while (sceCdRead(lbn, 0x10, &D_40073C00, D_40073BCC) != 1)
        {
            var_s0 = 0;
            while (sceCdDiskReady(1) != 2)
            {
                temp_v0 = sceCdStatus();
                if (temp_v0 == 0)
                    var_s0++;
                if ((temp_v0 == 1) || (var_s0 >= 5))
                {
                    if (D_400454B0 != NULL)
                    {
                        D_400454B0();
                    }
                    do
                    {
                        while (sceCdDiskReady(0), sceCdSearchFile(&fp, D_4007BC18) == 0)
                        {}
                    } while (D_40073BD4 != fp.lsn);
                    if (D_400454B4 != NULL)
                    {
                        D_400454B4();
                    }
                }
                DelayThread(0x823);
            }
        }
        sceCdSync(0);
        if (sceCdGetError() != 0)
        {
            var_s0_2 = 0;
            while (sceCdDiskReady(1) != 2)
            {
                temp_v0_2 = sceCdStatus();
                if (temp_v0_2 == 0)
                    var_s0_2++;
                if ((temp_v0_2 == 1) || (var_s0_2 >= 5))
                {
                    if (D_400454B0 != NULL)
                    {
                        D_400454B0();
                    }
                    do
                    {
                        while (sceCdDiskReady(0), sceCdSearchFile(&fp, D_4007BC18) == 0)
                        {}
                    } while (D_40073BD4 != fp.lsn);
                    if (D_400454B4 != NULL)
                    {
                        D_400454B4();
                    }
                }
                DelayThread(0x823);
            }
            goto UNKNOWN_LABEL;
        }
    }
    else
    {
        LoaderSysFSeek64(D_40073BDC, (u64)lbn << 11, 0);
        LoaderSysFRead(D_40073BDC, &D_40073C00, 0x8000);
    }
    D_4007BC08 = (u64)(lbn + 0x10) << 11;
}


//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", iosIopRead);
void iosIopRead(u32 arg0, s32 arg1, void* arg2)
{
    sceCdlFILE var_s2;
    s32 sp30[0x800];
    s32 sp2030;
    s32 temp_s0;
    s32 temp_s4;
    s32 temp_v0;
    s32 var_s1;
    s32 var_s1_2;
    
    
    if (D_40073BD8 == 0)
    {
        temp_s4 = D_40073BD4 + arg0;

    READ_LOOP:;
        if (sceCdReadIOPm(temp_s4, arg1, arg2, D_40073BCC) == 0)
        {
            for (var_s1=0; sceCdDiskReady(1) != 2;)
            {
                temp_v0 = sceCdStatus();
                if (temp_v0 == 0)
                    var_s1++;
                
                if ((temp_v0 == 1) || (var_s1 >= 5))
                {
                    if (D_400454B0 != NULL)
                        D_400454B0();
                    
                    do
                    {
                        do
                        {
                            sceCdDiskReady(0);
                        } while (sceCdSearchFile(&var_s2, D_4007BC18) == 0);
                    } while (D_40073BD4 != var_s2.lsn);
                    
                    if (D_400454B4 != NULL)
                        D_400454B4();
                }
                DelayThread(0x823);
            }
            goto READ_LOOP;
        }
        
        sceCdSync(0);
        
        if (sceCdGetError() != 0)
            goto READ_LOOP;
        return;
    }
    
    else
    {
        LoaderSysFSeek64(D_40073BDC, (s64)arg0 << 11, 0);
        
        for (var_s1_2=arg1<<11; var_s1_2 > 0;)
        {
            temp_s0 = (var_s1_2 < 0x2001) ? var_s1_2 : 0x2000;
            
            LoaderSysFRead(D_40073BDC, sp30, temp_s0);
            iosEE2IopRequest((s32)sp30, (s32)arg2, temp_s0, (s32)&sp2030);
            
            while (iosEE2IopEndChk(sp2030) == 0){}
            
            var_s1_2 -= temp_s0;
            arg2 += temp_s0;
        }
    }
}

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", iosCdStStart);
void iosCdStStart(s32 arg0)
{
    D_40073BCC = D_40073BD0;
    D_4007BC10 = (s64)(arg0 + D_40073BD4) << 11;
}

void iosCdStStop(void)
{
}

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", iosCdStRead);
void iosCdStRead(void* buf, s32 len)
{
    s32 count;
    s64 tmp0;

    while (len > 0)
    {
        if (D_4007BC10 >= D_4007BC00 && D_4007BC08 > D_4007BC10)
        {
            count = D_4007BC08 - D_4007BC10;
            count = (len <= count) ? len : count;
            
            memcpy(buf, D_40073C00 + (D_4007BC10 - D_4007BC00), count);
            len -= count;
            buf += count;
            D_4007BC10 += count;
        }
            
        else
        {
            tmp0 = D_4007BC10 < 0 ? D_4007BC10 + 2047 : D_4007BC10;
            func_400121D0((s32)(tmp0 >> 11));
        }
    }
}

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", iosCdStSkip);
void iosCdStSkip(u32 amount)
{
    D_4007BC10 += amount;
}

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", iosCdStSectorAlignSkip);
void iosCdStSectorAlignSkip(void)
{
    s64 sector;
    u32 offset;
    s64 unknown;

    sector = D_4007BC10;
    
    unknown = (sector < 0) ? sector + 2047 : sector;
    
    offset = sector - ((unknown >> 11) << 11);
    
    if (offset != 0)
        D_4007BC10 += 2048 - offset;
}

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", iosCdSetErrorCallback);
void iosCdSetErrorCallback(void *arg0, void *arg1)
{
    D_400454B0 = arg0;
    D_400454B4 = arg1;
}

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", iosCdSetMode);
void iosCdSetMode(s32 mode)
{
    D_40073BD0 = (mode == 1) ? &D_40073BC0 : &D_40073BC8;
}

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", iosCdStInit);
void iosCdStInit(char* filename)
{
    sceCdlFILE file;
    
//    D_40073BD8 = strncmp(&D_40046460, filename, 6) == 0;
    D_40073BD8 = strncmp("host0:", filename, 6) == 0;
    
    D_4007BC10 = D_4007BC00 = D_4007BC08 = -1;
    
    if (!D_40073BD8)
    {
        if (sceCdSearchFile(&file, filename) == 0)
            asm("break");
        
        D_40073BD4 = file.lsn;
        strcpy(D_4007BC18, filename);
        return;
    }
    
    D_40073BDC = LoaderSysFOpen(filename, 1, 0x1FF);
    
    if (D_40073BDC < 0)
//        iosJumpRecoverPoint(&D_40046468, filename); //"iosCdStInit:file %s not open\n", filename);
        iosJumpRecoverPoint("iosCdStInit:file %s not open\n", filename);
}

//INCLUDE_ASM("asm/kernel/nonmatchings/ios/cd", iosCdInit);
void iosCdInit(void)
{
    sceCdInit(0);
    sceCdMmode(2);
    D_40073BC0.trycount = 0;
    D_40073BC0.spindlctrl = 1;
    D_40073BC0.datapattern = 0;
    D_40073BC8.trycount = 0;
    D_40073BC8.spindlctrl = 0;
    D_40073BC8.datapattern = 0;
    D_40073BCC = D_40073BD0 = &D_40073BC0;
}

