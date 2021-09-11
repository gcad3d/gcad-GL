/* ./ut.h
// utily- functions
// TX_..    text-handling ..
// MEM_*
*/



#include <alloca.h>                    // alloca



//================================================================
 
void* MEM_alloc_tmp (int);
// allocate temp-space 
// DO NOT free; space is freed on exit of active function.
#define MEM_alloc_tmp(siz)\
 alloca(siz);\
 // if(siz >= SPC_MAX_STK) printf("*** ERR MEM_alloc_tmp |%s| %d\n",__func__,siz);\
 // fflush(stdout)




//================================================================

/// change 0 -> 1, 1 -> 0;                   i1 = ICHG01 (i1);
#define ICHG01(i)  (((i)>0)?(0):(1))



//================================================================
void TX_Print (char* txt, ...);
void TX_Error (char* txt, ...);
void TX_Write (char* txt);


// EOF

