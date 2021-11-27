// ../ut/mem.h


void* MEM_alloc_tmp (int);
void MEM_swap_int (int *i1, int *i2);


//----------------------------------------------------------------
// allocate temp-space 
// DO NOT free; space is freed on exit of active function.
#define MEM_alloc_tmp(siz)\
 alloca(siz);
 // if(siz >= SPC_MAX_STK) printf("*** ERR MEM_alloc_tmp |%s| %d\n",__func__,siz);\
 // fflush(stdout)



// swap 2 ints
#define MEM_swap_int(i1,i2){\
  int _i3 = *(i1); *(i1) = *(i2); *(i2) = _i3;}

// eof
