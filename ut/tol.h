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


//================================================================
#else
//----------------------------------------------------------------
// constant Tolerances:
extern double UT_TOL_min1;
extern double UT_TOL_min2;


//----------------------------------------------------------------
// Tolerances depending on Modelsize


#endif
//================================================================

// EOF
