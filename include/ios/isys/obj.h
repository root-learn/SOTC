#ifndef _ISYS_OBJ
#define _ISYS_OBJ

#include "common.h"


typedef struct isysObj {
    s32 partition;
    void* sourceFunc;
    s32 flags;
    s16 unkC;
    s16 unkE;
    struct isysObj* unk10;
    s32 unk14;
    void* immediateDeleteFunc;
} isysObj;











#endif /* _ISYS_OBJ */
