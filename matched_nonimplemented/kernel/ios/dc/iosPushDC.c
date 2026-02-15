// int gLastIndex; //last indeks
// int* gBasedIndex; //array static or dynamic 
// int gData; // curretn data node

extern void iosDlChainTailCurrent(int data);
void iosPushDC(int currentData) {
      int *indexHelper = &gBasedIndex + gLastIndex;
      ++gLastIndex;
      *indexHelper = currentData;
      iosDlChainTailCurrent(currentData);
      gData = currentData;
      return;
}

// void iosPushDC(param_1) {
//       int *puVar1;
//       puVar1 = &DAT_00065fc8 + DAT_00043868;
//       int DAT_00043868 = DAT_00043868 + 1;
//       *puVar1 = param_1;
//       iosDlChainTailCurrent();
//       int DAT_00043860 = param_1;
//       return;
// }
