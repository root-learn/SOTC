#include "common.h"
#include "ios/isys/letter.h"
#include "ios/memory.h"



// all the *Obj functions work with module objects. so letter_unkstrc is actually an obj struct


extern s32 rootPtn;



// .data
//s32* g_pLetterMem = NULL;


// .bss
static LetterPost* D_400F5B90;
static LetterType* D_400F5B98[20];




// prototypes
void func_4002D1E0(Letter*, s32, s32, LetterType*);
void func_4002D638(LetterPost* arg0);




//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", isysInitISYSLetterObj);
void isysInitISYSLetterObj(struct letter_unkstrc* arg0)
{
    arg0->unk1C = (LetterPost*)create_letter_post();
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", isysFlushISYSLetterObj);
void isysFlushISYSLetterObj(struct letter_unkstrc* arg0)
{
    check_free_letter_post(&arg0->unk1C);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", isysGetLetterObj);
struct letter_unkstrc* isysGetLetterObj(struct letter_unkstrc* arg0)
{
    switch (arg0->unk8)
    {
        case 6:
        case 9:
        case 11:
        case 13:
        case 22:
            return arg0;
        default:
            return NULL;
    }
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", isysLetterObjDiscardLetterAll);
void isysLetterObjDiscardLetterAll(struct letter_unkstrc* arg0)
{
    LetterPost* letterPost;
    void* var_v0;

    switch (arg0->unk8)
    {
        case 6:
        case 9:
        case 11:
        case 13:
        case 22:
            var_v0 = arg0;
            break;
        default:
            var_v0 = NULL;
    }
    if (var_v0 != NULL)
    {
        switch (arg0->unk8)
        {
            case 6:
            case 9:
            case 11:
            case 13:
            case 22:
                break;
            default:
                arg0 = NULL;
                break;
        }
        letterPost = arg0->unk1C;
        open_letter_post(letterPost);
        discard_all_letter_in_post(letterPost);
        close_letter_post(letterPost);
    }
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", create_letter);
Letter* create_letter(s32* arg0, s32* arg1, s32 arg2, s32 arg3, s32 arg4)
{
    void (*temp_v0_3)(s32);
    void (*temp_v0_5)(s32);
    LetterType* letterType;
    Letter* letter;

    letterType = (LetterType*)get_letter_type(arg2);
    if (letterType != 0)
    {
        letter = iosMallocAlign((s32)g_pLetterMem, letterType->receiverSize + letterType->dataSize + sizeof(Letter), 0x10);
        if (letter != NULL)
        {
            letter->unkC = arg2;
            letter->refCount = 1;
            letter->unk4 = 0;
            letter->unk8 = 0;
            letter->type = letterType;
            func_4002D1E0((void*)letter, arg3, arg4, letterType);
            letter->receiverData = (s32)(letter + 1);
            temp_v0_3 = (void*)letter->type->receiverInit;
            if (temp_v0_3 != NULL)
                temp_v0_3(letter->receiverData);

            letter->contentsData = letter->receiverData + letter->type->receiverSize;
            temp_v0_5 = (void*)(letter->type->dataInit);
            if (temp_v0_5 != NULL)
                temp_v0_5(letter->contentsData);
            letter->is_initialized = 0;
            *arg0 = letter->receiverData;
            *arg1 = letter->contentsData;
            return letter;
        }
    }
    return NULL;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", init_letter);
void init_letter(Letter* arg0, s32* arg1, s32* arg2)
{
    *arg1 = 0;
    *arg2 = 0;
    arg0->is_initialized = 1;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", is_hope_letter_type);
s32 is_hope_letter_type(Letter* arg0, s32 arg1)
{
    return arg0->unkC == arg1;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", get_letter_contents_data);
void* get_letter_contents_data(Letter* arg0)
{
    return (void*)arg0->contentsData;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", func_4002D1E0);
void func_4002D1E0(Letter* arg0, s32 arg1, s32 arg2, LetterType* arg3)
{
    arg0->receiverObjGroup = (s32)((arg3->unk0 != 0) ? arg2 : arg1);
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", increment_letter_refcount);
void increment_letter_refcount(Letter* arg0)
{
    arg0->refCount += 1;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", decrement_letter_refcount);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", is_letter_reach_time);
s32 is_letter_reach_time(Letter* arg0, f32 arg1)
{
    if (arg0->unk4 <= arg0->unk8)
        return 1;
    
    arg0->unk8 += arg1;
    return 0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", is_init_letter_called);
s32 is_init_letter_called(Letter* arg0)
{
    return arg0->is_initialized;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", get_reference_letter_type);
LetterType* get_reference_letter_type(Letter* arg0)
{
    return arg0->type;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", get_letter_receiver_obj_group);
s32 get_letter_receiver_obj_group(Letter* arg0)
{
    return arg0->receiverObjGroup;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", get_letter_receiver_data);
s32 get_letter_receiver_data(Letter* arg0)
{
    return arg0->receiverData;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", doit_letter_manager);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", init_letter_manager);
void init_letter_manager(void)
{
    init_letter_type_store();
    g_pLetterMem = &rootPtn;
    D_400F5B90 = create_letter_post();
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", send_letter);
void send_letter(LetterLink* arg0)
{
    push_back_letter_in_post(D_400F5B90, arg0->letter);
    decrement_letter_refcount(arg0);
    arg0->letter = NULL;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", func_4002D480);
s32 func_4002D480(struct letter_unkstrc* arg0, Letter* arg1, LetterType* arg2)
{
    typedef s32 (*temp)(void*, s32);
    
    if (arg2->unkC == NULL || ((temp)arg2->unkC)(arg0, get_letter_receiver_data(arg1)) == 1)
    {
        push_back_letter_in_post(arg0->unk1C, arg1);
        return 1;
    }
    
    return 0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", create_letter_post);
LetterPost* create_letter_post(void)
{
    LetterPost* temp_v0;

    temp_v0 = iosMallocAlign((s32)g_pLetterMem, sizeof(LetterPost), 0x10);
    func_4002D638(temp_v0);
    return temp_v0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", check_free_letter_post);
void check_free_letter_post(LetterPost** arg0)
{
    if (*arg0 != 0)
    {
        discard_all_letter_in_post(*arg0);
        iosFree(*arg0);
        *arg0 = 0;
    }
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", discard_all_letter_in_post);
void discard_all_letter_in_post(LetterPost* arg0)
{
    LetterLink* temp_a0;
    LetterLink* temp_s0;
    LetterLink* var_s1;

    var_s1 = arg0->letters_tail;
    if (var_s1 != NULL)
    {
        do
        {
            temp_s0 = var_s1->next;
            decrement_letter_refcount(var_s1);
            temp_a0 = var_s1;
            var_s1 = temp_s0;
            iosFree(temp_a0);
        } while (var_s1 != NULL);
    }
    
    arg0->letterCount = 0;
    arg0->letters_tail = NULL;
    arg0->letters_head = NULL;
    arg0->read = NULL;
    arg0->reading = NULL;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", get_total_letter_in_post);
s32 get_total_letter_in_post(LetterPost* arg0)
{
    return arg0->letterCount;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", open_letter_post);
void open_letter_post(LetterPost* arg0)
{
    arg0->read = NULL;
    arg0->reading = arg0->letters_tail;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", get_next_letter_in_post);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", close_letter_post);
void close_letter_post(LetterPost* arg0)
{
    arg0->read = 0;
    arg0->reading = 0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", func_4002D638);
void func_4002D638(LetterPost* arg0)
{
    arg0->letterCount = 0;
    arg0->letters_tail = NULL;
    arg0->letters_head = NULL;
    arg0->read = NULL;
    arg0->reading = NULL;
}

INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", push_back_letter_in_post);

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", discard_selecting_letter_in_post);
void discard_selecting_letter_in_post(LetterPost* arg0)
{
    LetterLink* temp_s0;
    LetterLink* temp_s0_2;
    LetterLink* temp_v0;
    LetterLink* temp_v0_2;

    temp_s0 = arg0->read;
    if (temp_s0 != NULL)
    {
        if (arg0->letterCount == 1)
        {
            decrement_letter_refcount(temp_s0);
            iosFree(temp_s0);
            arg0->letterCount = 0;
            arg0->letters_tail = NULL;
            arg0->letters_head = NULL;
            arg0->read = NULL;
            arg0->reading = 0;
        }
        else
        {
            if (temp_s0 == arg0->letters_tail)
            {
                temp_v0_2 = temp_s0->next;
                arg0->letters_tail = temp_v0_2;
                temp_v0_2->prev = NULL;
                
            }
            else
            {
                if (temp_s0 == arg0->letters_head)
                {
                    temp_v0 = temp_s0->prev;
                    arg0->letters_head = temp_v0;
                    temp_v0->next = NULL;
                    
                }
                else
                {
                    temp_s0->prev->next = temp_s0->next;
                    temp_s0->next->prev = temp_s0->prev;
                }
            }
            temp_s0_2 = arg0->read;
            decrement_letter_refcount(temp_s0_2);
            iosFree(temp_s0_2);
            arg0->read = NULL;
            arg0->letterCount--;
        }
    }
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", constructor_letter_type);
void constructor_letter_type(LetterType* arg0)
{
    arg0->unk0 = 2;
    arg0->receiverSize = 0;
    arg0->receiverInit = 0;
    arg0->unkC = 0;
    arg0->dataSize = 0;
    arg0->dataInit = 0;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", entry_new_letter_type);
s32 entry_new_letter_type(LetterType* arg0)
{
    s32 i;

    for (i=0; i<20; i++)
    {
        if (D_400F5B98[i] == NULL)
        {
            D_400F5B98[i] = arg0;
            return i;
        }
    }
    return -1;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", init_letter_type_store);
void init_letter_type_store(void)
{
    s32 i;

    for (i=0; i<20; i++)
        D_400F5B98[i] = NULL;
}

//INCLUDE_ASM("asm/KERNEL.XFF/nonmatchings/ios/isys/letter", get_letter_type);
LetterType* get_letter_type(u32 arg0)
{
    if (arg0 < 20)
        return D_400F5B98[arg0];
    
    return 0;
}
