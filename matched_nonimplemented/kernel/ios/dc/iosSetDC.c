extern void iosDlChainTailCurrent(int currentData);
int iosSetDC(int param_1)

{
  iosDlChainTailCurrent(param_1);
  if ((*(int *)(param_1 + 0x170) & 1) != 0) {
    iosDlChainTailCurrent(param_1 + 0xd0);
    return;
  }
  return;
}
