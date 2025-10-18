#include "common.h"
#include "ios/isys/obj.h"





INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysDeleteObjManager);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", func_4002AF90);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", func_4002B218);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysAddGroupWithLinkParam);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysAddGroupRelativeWithLinkParam);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", _isysDeleteGroup);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysDeleteGroupWithLinkParam);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysDeleteGroupAllObj);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysDeleteAllGroup);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGroupForFirst);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGroupForFirstForce);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGroupGetObjNumForce);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", _isysCreateObj);
isysObj* _isysCreateObj(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    s32 part;
    isysObj* obj;
    register void* ra asm("ra");
    void* tmp;

    tmp = ra;
    part = iosCreatePartitionWithPc(arg0, arg2 + arg3 + sizeof(isysObj) + 8, tmp);
    if (part != 0)
    {
        obj = iosMallocAlign(part, arg2, arg5);
        if (obj != NULL)
        {
            obj->partition = part;
            obj->sourceFunc = tmp;
            *(u8*)&obj->flags = arg1;
            *(u8*)(((s32)&obj->flags)+1) = 0;
            obj->flags = *(u16*)&obj->flags;
            obj->unk14 = arg4;
            obj->unkC = obj->unkE = 0;
            obj->immediateDeleteFunc = NULL;
        }
        return obj;
    }
    return NULL;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysSetObjImmediateDeleteFunc);
void isysSetObjImmediateDeleteFunc(isysObj* obj, void* func)
{
    obj->immediateDeleteFunc = func;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysRequestDeleteObj);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysInitGroup);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysAddGroupWithLinkParamNoWait);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysObjDeleteLock);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysObjDeleteUnLock);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGroupForNext);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGroupForNextForce);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGroupForExit);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysInitObjSystem);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGetLinkNum);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGetLinkMax);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGetObjIdentifierByObj);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysGetGroupSrhNext);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysObjWaitSema);

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/obj", isysObjSignalSema);
