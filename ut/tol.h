/* tol.h

tolerances

static include in application/main with ./geo.h
*/




//================================================================
#ifdef extern

//----------------------------------------------------------------
// constant Tolerances:
double UT_TOL_min1 = 0.00000001;
double UT_TOL_min2 = 1E-12;

//----------------------------------------------------------------
// Tolerances depending on Modelsize
double UT_TOL_pt = 0.001;         // identical Points-tolerence


//================================================================
#else
//----------------------------------------------------------------
// constant Tolerances:
extern double UT_TOL_min1;
extern double UT_TOL_min2;


//----------------------------------------------------------------
// Tolerances depending on Modelsize
extern double UT_TOL_pt;         // identical Points-tolerence


#endif
//================================================================

// EOF
