/*   ./geo.h
static include in application/main
Functions ../ut/ut3d.c ../ut/ut2d.c
*/


#include "../ut/ut_types.h"               // INT_8 - UINT_64
#include "../ut/types.h"                  // Typ_PT ..
#include "../ut/ut.h"                     // UTI UTP BIT I* D*
#include "../ut/mem.h"                    // MEM_*
#include "../ut/tol.h"                    // UT_TOL_..
#include "../ut/TX_.h"                    // TX_Print ..


//================================================================

// 2D-point, Typ_PT2
typedef struct {double x, y;}                                       Point2;
// size = 16

// 3D-point, Typ_PT
typedef struct {double x, y, z;}                                    Point;
// size = 24

typedef struct {float x, y, z;}                                     Pointf;

// 2D-vector, Typ_VC2
typedef struct {double dx, dy;}                                     Vector2;
// size = 16

// 3D-vector, Typ_VC
typedef struct {double dx, dy, dz;}                                 Vector;
// size = 24

typedef struct {float dx, dy, dz;}                                  Vectorf;

// 3D-position+vector, Typ_PVC
typedef struct {Point pos; Vector vc;}                              VecPos;
// size = 24


// 2D-line, Typ_LN2
typedef struct {Point2 p1, p2; char typ;}                           Line2;
//  typ      0  both sides limited                               -
//           1  1 side limited  (p1 is startpoint, p2 unlimited) UNL1
//           2  1 side limited  (p2 is startpoint, p1 unlimited) UNL2
//           3  both sides unlimited                             UNL


// 3D-plane, Typ_PLN
// (pln.vz.dx * p.x) + (pln.vz.dy * p.y) + (pln.vz.dz * p.z) + pln.p = 0
// p      normal-distance of plane from 0,0,0
typedef struct {Point po; Vector vx, vy, vz; double p;}             Plane;
// size = 112


typedef double Mat_4x4D[4][4];            // Typ_M4x4D
typedef float  Mat_4x4F[4][4];            // Typ_M4x4F


// text or image or label; Typ_ATXT, Typ_Tag ..
// p1   Textblock-/Imageposition (left middle)
// p2   Startpoint leaderline
// scl  Scale (for Image)
// typ  0=Text, 1=Image, 2=Label-Block, 3=Label-Kreis  4=PointCoord
//      5=Symbol SYM_STAR_S (Stern klein) 6=Symbol SYM_TRI_S (Dreieck klein)
//      7=Symbol SYM_CIR_S (Kreis klein)  8=SYM_TRI_B (Viereck)
//      9=Vector (normalized)            10=Vector (true length)
// col  Farbe fuer Label; -1=kein Label
// ltyp Linetyp Leaderline; -1=no Leaderline.
// txt  Text/ImageFilename
typedef struct {Point p1, p2; char *txt;
                float scl; char typ, col, ltyp;}                    AText;


// grafic text; Typ_GTXT
// dir          direction in degree
// size         textsize-in-mm; 0=2D-text
typedef struct {Point pt; float size, dir; char *txt;}              GText;


// dimension; Typ_Dimen
// p1    dimensionpoint 1
// p2    dimensionpoint 2
// p3    textpoint
// dtyp: 0=linear 1=diameter 2=radius 3=angle 21=leader
// hd:   heads: 0=none, 1=<, 2=>, 3=/, 4=X; Default is 12.
// ld    lines: 0=none, 1=line; Default is 11.
// a1    angle (linear:textline; angle: line1; leader:text)
// a2    angle (angle: line2)
typedef struct {Point2 p1, p2, p3; float a1, a2;
                char dtyp, hd, ld, uu; char *txt;}                  Dimen;



// 2D-symbol; Typ_SymRef2 Typ_SymB Typ_SymV
// ang        angle in rad
typedef struct {Point2 pt; float ang, scl; int typ;}                SymRef2;
// GL_DrawSymV2



// 3D-symbol; Typ_SymRef
typedef struct {Point pt; Vector dx, dy; float scl; int typ;}       SymRef;
// GL_DrawSymV3


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
int UT2D_sidPerp_ptvc (Point2 *pt,  Point2 *pl, Vector2 *vl);

double UT2D_len_vc (Vector2 *);
double UT2D_angr_vc (Vector2 *vc);
void   UT2D_vc_2pt (Vector2 *, Point2 *, Point2 *);
void UT2D_vc_setLength (Vector2 *vco, Vector2 *vci, double new_len);

int    UT2D_comp2pt (Point2*, Point2*, double);
double UT2D_len_2pt (Point2 *,Point2 *);
void   UT2D_pt_traptvc (Point2 *, Point2 *, Vector2 *);
Point2 UT2D_pt_pt3 (Point *);
int UT2D_pt_projptptvc (Point2 *pp, Point2 *pt, Point2 *pl, Vector2 *vl);
void   UT2D_pt_traptvclen (Point2 *,Point2 *,Vector2 *,double);

//----------------------------------------------------------------
void UT3D_angrZX_vc (double *angrZ, double *angrX, Vector *vc);

int UT3D_comp2vc_d (Vector *v1, Vector *v2, double tol);
double UT3D_len_vc (Vector *);
int UT3D_2angr_vc__ (double *az, double *ay, Vector *vc1);

void UT3D_pt_3db (Point *pt, double px, double py, double pz);
void UT3D_pt_traptvc (Point*, Point*, Vector*);
int UT3D_ptDi_intptvcpln (Point *ip, double *dist,
                          Plane *pl, Point *pt, Vector *vln);
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


// UT2D_vc_angr            2DVector =  angle (radians)
#define UT2D_vc_angr(vc,angr){\
  (vc)->dx = cos(angr);\
  (vc)->dy = sin(angr);}


// UT2D_comp2pt              compare 2 points
// RC = 0: points not identical; distance > tolerance
// RC = 1: points are identical; distance < tolerance
#define UT2D_comp2pt(p1,p2,tol)\
  ((fabs((p2)->x - (p1)->x) < tol) &&\
   (fabs((p2)->y - (p1)->y) < tol))


// UT2D_vc_2pt               2D-Vector = 2D-Point -> 2D-Point
#define UT2D_vc_2pt(vc,p1,p2){\
 (vc)->dx = (p2)->x - (p1)->x;\
 (vc)->dy = (p2)->y - (p1)->y;;}



// UT2D_pt_pt3               2D-Point = 3D-Point (cut off z-coord)
#define UT2D_pt_pt3(pt3)\
  (*(Point2*)pt3)

// UT2D_pt_traptvc               2D Point = Point + Vector
#define UT2D_pt_traptvc(po,pi,vc){\
 (po)->x = (pi)->x + (vc)->dx;\
 (po)->y = (pi)->y + (vc)->dy;}


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

/// UT3D_vc_2pt               Vector = Point -> Point
#define UT3D_vc_2pt(vc,p1,p2){\
 (vc)->dx = (p2)->x - (p1)->x;\
 (vc)->dy = (p2)->y - (p1)->y;\
 (vc)->dz = (p2)->z - (p1)->z;}


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
