/*   ./geo.h
*/


// 3D-point, Typ_PT
typedef struct {double x, y, z;}                                    Point;
// size = 24

// 3D-vector, Typ_VC
typedef struct {double dx, dy, dz;}                                 Vector;
// size = 24

// 3D-plane, Typ_PLN
// (pln.vz.dx * p.x) + (pln.vz.dy * p.y) + (pln.vz.dz * p.z) + pln.p = 0
// p      normal-distance of plane from 0,0,0
typedef struct {Point po; Vector vx, vy, vz; double p;}             Plane;
// size = 112


typedef double Mat_4x4D[4][4];            // Typ_M4x4D
typedef float  Mat_4x4F[4][4];            // Typ_M4x4F


//----------------------------------------------------------------
// aux. structs:

// int = char[3] + char
typedef struct {unsigned b123:24, b4:8;}                            stru_c3c1;



//================================================================
 
#define RAD_1     0.0174532925199432954         //   1 degree


#define _UT3D_PT_NUL { 0., 0., 0. }

#define _UT3D_VC_X { 1., 0., 0. }
#define _UT3D_VC_Z { 0., 0., 1. }

// Mat_4x4D-double  // VX   VY   VZ   PO
#define _MAT_4x4D  { {1.0, 0.0, 0.0, 0.0},\
                     {0.0, 1.0, 0.0, 0.0},\
                     {0.0, 0.0, 1.0, 0.0},\
                     {0.0, 0.0, 0.0, 1.0} }


// Mat_4x4D-float   // VX   VY   VZ   PO
#define _MAT_4x4F  { {1.f, 0.f, 0.f, 0.f},\
                     {0.f, 1.f, 0.f, 0.f},\
                     {0.f, 0.f, 1.f, 0.f},\
                     {0.f, 0.f, 0.f, 1.f} }




//----------------------------------------------------------------
// static in application/main
#ifdef extern
const Point   PT_NUL       = _UT3D_PT_NUL;
const Vector  VC_X         = _UT3D_VC_X;
const Vector  VC_Z         = _UT3D_VC_Z;
const Mat_4x4D MAT_4x4D_NUL  = _MAT_4x4D;
const Mat_4x4F MAT_4x4F_NUL  = _MAT_4x4F;


#else
extern Point   PT_NUL;
extern Vector  VC_X;
extern Vector  VC_Z;
extern Vector  VC_NUL;
extern Mat_4x4D MAT_4x4D_NUL;
extern Mat_4x4F MAT_4x4F_NUL;

#endif



//================================================================
 
/// DLIM2                     returns x = between lo and hi
double DLIM2 (double, double, double);
#define DLIM2(x,lo,hi) (((x)>(hi))?(hi):(((x)<(lo))?(lo):(x)))


/// UT_RADIANS                radians from degrees
#define UT_RADIANS(angDeg) ((angDeg)*(RAD_1))
/// UT_DEGREES                degrees from radians
#define UT_DEGREES(angRad) ((angRad)/(RAD_1))


// EOF
