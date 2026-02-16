extern void iosDlChainTailCurrent(int index);

void iosPushDCWithClear(int param_1) 
{
    int *indexHelper;

    indexHelper = &gBasedIndex + gLastIndex;
    gLastIndex = gLastIndex + 1;
    *indexHelper = param_1;
    iosDlChainTailCurrent(*indexHelper);
    if ((*(int *)(param_1 + 0x170) & 1) != 0) {
      iosDlChainTailCurrent(param_1 + 0xd0);
    }
    gData = param_1;
    return;
}
