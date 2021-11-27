/* ../ut/ut.h
// utily- functions
// TX_..    text-handling ..
// MEM_*
*/



#include <alloca.h>                    // alloca



//================================================================
// int DSIGTOL (double, double);
// double ACOS (double);
// int ICHG01 (int);

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


// DSIGTOL                              sign of double with tolerance (-1|0|1)
#define DSIGTOL(dd,tol) ((dd>tol)?(1):((dd<-(tol))?(-1):(0)))
// replacing code:
//   if (fabs(dd) < tol) rc =  0;
//   else if (dd < 0.)   rc =  1;
//   else                rc = -1;


#define ACOS(dCos) ((dCos>=1.)?(0.):((dCos<=-1.)?(RAD_180):acos(dCos)))


// EOF

