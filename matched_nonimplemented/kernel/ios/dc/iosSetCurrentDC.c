extern void iosDlChainTailCurrent(int);

int iosSetCurrentDC()

{
  iosDlChainTailCurrent(gData);
  return;
}