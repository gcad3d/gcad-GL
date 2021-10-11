/*   ./geo.h
static include in application/main
Functions ../ut/ut3d.c
*/

#include "./tol.h" 



//================================================================

/// 2D-vector, Typ_VC2
typedef struct {double dx, dy;}                                     Vector2;
// size = 16

// 3D-point, Typ_PT
typedef struct {double x, y, z;}                                    Point;
// size = 24

// 3D-vector, Typ_VC
typedef struct {double dx, dy, dz;}                                 Vector;
// size = 24

// 3D-position+vector, Typ_PVC
typedef struct {Point pos; Vector vc;}                              VecPos;
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
#define RAD_90    1.5707963267948965579    ///<  90
#define RAD_180   3.14159265358979323846        // 180 degree
#define RAD_360   6.2831853071795862319    ///< 360 degree


#define SR_3      1.7320508075688772       ///< SQRT(3)
#define SR_2      1.4142135623730951       ///< SQRT(2)


#define _UT3D_PT_NUL { 0., 0., 0. }

#define _UT3D_VC_X  { 1., 0., 0. }
#define _UT3D_VC_Y  { 0., 1., 0. }
#define _UT3D_VC_Z  { 0., 0., 1. }
#define _UT3D_VC_IX { -1., 0., 0. }
#define _UT3D_VC_IY { 0., -1., 0. }
#define _UT3D_VC_IZ { 0., 0., -1. }


// Plane        
#define _PLANE_NUL { {0.,0.,0.}, {1.,0.,0.}, {0.,1.,0.}, {0.,0.,1.}, 0.}
//                      po          vx          vy          vz        p


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
const Point     UT3D_PT_NUL           = _UT3D_PT_NUL;
const Vector    UT3D_VECTOR_X         = _UT3D_VC_X;
const Vector    UT3D_VECTOR_Y         = _UT3D_VC_Y;
const Vector    UT3D_VECTOR_Z         = _UT3D_VC_Z;
const Vector    UT3D_VECTOR_IX        = _UT3D_VC_IX;
const Vector    UT3D_VECTOR_IY        = _UT3D_VC_IY;
const Vector    UT3D_VECTOR_IZ        = _UT3D_VC_IZ;

const Plane     PLANE_NUL      = _PLANE_NUL;

const Mat_4x4D  MAT_4x4D_NUL  = _MAT_4x4D;
const Mat_4x4F  MAT_4x4F_NUL  = _MAT_4x4F;


#else
extern Point    UT3D_PT_NUL;
extern Vector   UT3D_VECTOR_NUL;
extern Vector   UT3D_VECTOR_X;
extern Vector   UT3D_VECTOR_Y;
extern Vector   UT3D_VECTOR_Z;
extern Vector   UT3D_VECTOR_IY;
extern Plane    PLANE_NUL;
extern Mat_4x4D MAT_4x4D_NUL;
extern Mat_4x4F MAT_4x4F_NUL;

#endif


//================================================================
double UT2D_len_vc (Vector2 *);
double UT2D_angr_vc (Vector2 *vc);
void UT2D_vc_setLength (Vector2 *vco, Vector2 *vci, double new_len);

//----------------------------------------------------------------
double UT3D_len_vc (Vector *);
int UT3D_2angr_vc__ (double *az, double *ay, Vector *vc1);
void UT3D_angrZX_vc (double *angrZ, double *angrX, Vector *vc);

void UT3D_pt_3db (Point *pt, double px, double py, double pz);
void UT3D_pt_traptvc (Point*, Point*, Vector*);
int UT3D_ptDi_intptvcpln (Point *ip, double *dist,
                          Plane *pl, Point *pt, Vector *vln);
int UT3D_comp2vc_d (Vector *v1, Vector *v2, double tol);

void   UT3D_vc_invert (Vector*, Vector*);
void UT3D_vc_setLength (Vector *vco, Vector *vci, double new_len);
void UT3D_vc_3db (Vector *vc, double x, double y, double z);
void UT3D_vc_perp2vc (Vector *vp, Vector *v1, Vector *v2);
Vector UT3D_vc_rotvc_angZ (Vector *vci, double angZ);
Vector UT3D_vc_rotvc_angX (Vector *vci, double angX);
Vector UT3D_vc_rotvc_angZ_angX (Vector *vci, double angZ, double angX);
void UT3D_vc_multvc (Vector*, Vector*, double);



//================================================================
 
// DLIM2                     returns x = between lo and hi
double DLIM2 (double, double, double);
#define DLIM2(x,lo,hi) (((x)>(hi))?(hi):(((x)<(lo))?(lo):(x)))


// UT_RADIANS                radians from degrees
double UT_RADIANS (double);
#define UT_RADIANS(angDeg) ((angDeg)*(RAD_1))

// UT_DEGREES                degrees from radians
double UT_DEGREES (double);
#define UT_DEGREES(angRad) ((angRad)/(RAD_1))


//----------------------------------------------------------------
// UT2D_len_vc          length of 2D-vector
#define UT2D_len_vc(vc) (sqrt((vc)->dx * (vc)->dx + (vc)->dy * (vc)->dy))


//----------------------------------------------------------------
/// UT3D_len_vc          length of 3D-vector
#define UT3D_len_vc(vc)\
 (sqrt((vc)->dx*(vc)->dx + (vc)->dy*(vc)->dy + (vc)->dz*(vc)->dz))


// UT3D_pt_traptvc               Point = Point + Vector
#define UT3D_pt_traptvc(po,pi,vc) {\
 (po)->x = (pi)->x + (vc)->dx;\
 (po)->y = (pi)->y + (vc)->dy;\
 (po)->z = (pi)->z + (vc)->dz;}


// UT3D_pt_3db               Vector < 3 doubles (x, y, z)
#define UT3D_pt_3db(pt,px,py,pz) {\
 (pt)->x = (px);\
 (pt)->y = (py);\
 (pt)->z = (pz);}


/// UT3D_vc_3db               Vector < 3 doubles (x, y, z)
#define UT3D_vc_3db(vc,x,y,z) {\
 (vc)->dx = (x);\
 (vc)->dy = (y);\
 (vc)->dz = (z);}

/// UT3D_vc_invert            invert Vector
#define UT3D_vc_invert(vio,vii){\
  (vio)->dx = -(vii)->dx;\
  (vio)->dy = -(vii)->dy;\
  (vio)->dz = -(vii)->dz;}

/// UT3D_vc_multvc                vo = vi * d                 MULT
#define UT3D_vc_multvc(vo,vi,d) {\
 (vo)->dx = (vi)->dx * (d);\
 (vo)->dy = (vi)->dy * (d);\
 (vo)->dz = (vi)->dz * (d);}


//================================================================




// EOF
