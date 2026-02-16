extern void _isysDeleteGroup(int , int* , int);
extern void isysRequestDeleteObj(int);

int iosReleaseDC(int param_1) 
{
    
  if (param_1 != 0) {
    _isysDeleteGroup(param_1,&D_40092688,0);
    isysRequestDeleteObj(param_1);
    return;
  }
  return;
}