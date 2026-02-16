//still 98%
extern void iosDlChainTailCurrent(int index);
void iosPopDC() 
{
    int indexHelper;
    
    gLastIndex = gLastIndex + -1;
    indexHelper = *(int *)(&indexToPop + gLastIndex * 4);
    iosDlChainTailCurrent(indexHelper);
    gData = indexHelper;
    return;
}