/* ../ut/ut.h
// utily- functions
// TX_..    text-handling ..
// MEM_*
*/



#include <alloca.h>                    // alloca



//================================================================
void TX_Print (char* txt, ...);
void TX_Error (char* txt, ...);
void TX_Write (char* txt);


double ACOS (double);


//================================================================
 
void* MEM_alloc_tmp (int);
// allocate temp-space 
// DO NOT free; space is freed on exit of active function.
#define MEM_alloc_tmp(siz)\
 alloca(siz);\
 // if(siz >= SPC_MAX_STK) printf("*** ERR MEM_alloc_tmp |%s| %d\n",__func__,siz);\
 // fflush(stdout)




//================================================================

// change 0 -> 1, 1 -> 0;                   i1 = ICHG01 (i1);
#define ICHG01(i)  (((i)>0)?(0):(1))


// UTI_round_8up              round integer up to 8
//   eg 2 -> 4; 4 -> 4;  5 -> 8;
#define UTI_round_8up(ii) (ii + 7) & ~0x7


// UTP_comp_0             compare double (double == 0.0 + - UT_TOL_min1)
// Retcode 0:   db <> 0.0
//         1:   db = 0.0
// UTP_comp_0 (1.);   // returns 0  (false, not equal to 0.0)
// UTP_comp_0 (0.);   // returns 1  (true, is equal 0)
// see also UTP_db_comp_0
#define UTP_comp_0(db) (fabs(db) < UT_TOL_min1)



// UT2D_angr_set_2angr_sr        set angle between 0 to 2Pi
#define UTP_angr_set_0_2pi(angr)\
 (((angr)<0.)?((angr)+RAD_360):(((angr)>RAD_360)?((angr)-RAD_360):(angr)))



#define ACOS(dCos) ((dCos>=1.)?(0.):((dCos<=-1.)?(RAD_180):acos(dCos)))


// EOF

