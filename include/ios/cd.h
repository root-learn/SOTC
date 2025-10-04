#ifndef IOSCD_H
#define IOSCD_H

#include "common.h"
#include "sdk/ee/eekernel.h"
#include "loaderSysFileIO.h"
#include "sdk/common/libcdvd.h"

void func_400121D0(s32 lbn);
void iosIopRead(u32 arg0, s32 arg1, void* arg2);
void iosCdStStart(s32 arg0);
void iosCdStStop(void);
void iosCdStRead(void* buf, s32 len);
void iosCdStSkip(u32 amount);
void iosCdStSectorAlignSkip(void);
void iosCdSetErrorCallback(void *arg0, void *arg1);
void iosCdSetMode(s32 mode);
void iosCdStInit(char* filename);
void iosCdInit(void);




#endif /* IOSCD_H */
