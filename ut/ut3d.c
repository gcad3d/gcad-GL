/* ../ut/ut3d.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

geometric computations 3D

=====================================================
List_functions_start:


UT3D_len_vc               Vectorlength                    INLINE
UT3D_angrZX_vc            get rotation-angles of Z-axis and X-axis for vector
UT3D_2angr_vc__           2 opening angles of vec (angle to Z-axis, tilt-angle)

-------------- points --------------------------------------
UT3D_pt_3db               Point < 3 doubles (x, y, z)               (INLINE)
UT3D_pt_traptvc           Point = Point + Vector                    (INLINE)
UT3D_ptDi_intptvcpln      intersect line-plane (pt-vc  pt-vc)

-------------- vectors -------------------------------------
UT3D_comp2vc              compare 2 vectors for parallel and antiparallel
UT3D_vc_perp2vc           vector = perpendic. to 2 vectors (crossprod)
UT3D_vc_multvc            vo = vi * fakt     Vec-multiplication     (INLINE)
UT3D_vc_invert            Vector invert (change direction)                   INLINE
UT3D_vc_setLength         change vectorlength
UT3D_vc_3db               Vector = 3 doubles (x, y, z)              (INLINE)
UT3D_vc_rotvc_angZ        rotate a vector around Z-axis
UT3D_vc_rotvc_angX        rotate a vector around the X-axis
UT3D_vc_rotvc_angZ_angX   rotate a vector around Z-axis then around X-axis


List_functions_end:
=====================================================

*/

#ifdef globTag
 void UT3D(){}
#endif



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

// #define extern  // does eliminate "extern" in includefile
// #include "./tol.h"                     // UT_TOL_min1
// #undef extern
 
#include "../ut/geo.h"                     // Point Plane Mat_4x4D ..
#include "../ut/ut.h"                      // ACOS




//======================================================================
  void UT3D_angrZX_vc (double *angrZ, double *angrX, Vector *vc) {
//======================================================================
// UT3D_angrZX_vc            get rotation-angles of Z-axis and X-axis for vector
// vc must be in absolute axis-system;
// returns 0 - RAD_360 (PI * 2);  X-axis is 0;
// Example: (1,1,0.1) returns angrZ = 0.78-rad = 45-deg;  angrX = 0.99-rad = 5.7-deg

  Vector2    vc2;

  // aZ = angle X-axis - projected onto X-Y-plane
  vc2.dx = vc->dx;
  vc2.dy = vc->dy;
  *angrZ = UT2D_angr_vc (&vc2);

  // aX = angle X-axis - projected onto Y-Z-plane
  vc2.dx = vc->dy;
  vc2.dy = vc->dz;
  *angrX = UT2D_angr_vc (&vc2);

}


//=======================================================================
  int UT3D_2angr_vc__ (double *az, double *ay, Vector *vc1) {
//=======================================================================
// UT3D_2angr_vc__           2 opening angles of vec (angle to Z-axis, tilt-angle)
// Output:
//   ay   tilt-angle = angle between vc1 and the absolute-XY-plane
//   az   angle between vc1 and the absolute XZ-plane
//
//  az: get vcx = project vc1 onto absolute-XY-plane;
//      az = angle between vcx - absolute-X-axis)
//
// restore vector from angles:
//   vcx = rotate X-axis az radians around Z-axis;
//   vc1 = rotate vcx ay radians around the (new) Y-axis
//         (the new Y-axis yet is in the absolute-XY-plane)
//
// see UT3D_angrR_vc UT3D_angrT_vc


  Vector2 vcxy, vcxz;

  
  // 2D-Vector aus Draufsicht auf X-Y
  vcxy.dx = vc1->dx;
  vcxy.dy = vc1->dy;
  
  // 2D-Vector normieren
  UT2D_vc_setLength (&vcxy, &vcxy, 1.);
  // printf("vcxy=%f,%f\n",vcxy.dx,vcxy.dy);

  *az = ACOS(vcxy.dx);          // um Z-Achse verdrehen
  

  // Seitenansicht X-Z aus Richtung -Y   
  vcxz.dx = sqrt((vc1->dx*vc1->dx)+(vc1->dy*vc1->dy));
  vcxz.dy = vc1->dz;

    
  // normieren
  UT2D_vc_setLength (&vcxz, &vcxz, 1.);
    // printf("vcxz=%f,%f\n",vcxz.dx,vcxz.dy);

  *ay = ACOS(vcxz.dx);          // um (die neue) Y-Achse verdrehen


  if(vc1->dy < 0.) *az *= -1.;
  if(vc1->dz < 0.) *ay *= -1.;


    // printf("ex-UT3D_2angr_vc__ az=%f ay=%f vc=%f,%f,%f\n",
            // UT_DEGREES(*az),UT_DEGREES(*ay),
            // vc1->dx,vc1->dy,vc1->dz);


  return 0;

}


//================================================================
  int UT3D_comp2vc_d (Vector *v1, Vector *v2, double tol) {
//================================================================
// UT3D_comp2vc              compare 2 vectors for parallel and antiparallel
// tolerances: see func. UT3D_vc_ck_parpl
// RC=1:   vectors are parallel or antiparallel
// RC=0:   not (anti)parallel

  int      ibp;
  double   d1, dd, dx, dy, dz;


  // printf("==================== UT3D_comp2vc_d %f\n",tol);
  // DEB_dump_obj__ (Typ_VC, v1, " V1:");
  // DEB_dump_obj__ (Typ_VC, v2, " V2:");


  // find component with max. value (x|y|z)

  dx = fabs(v1->dx);
  dy = fabs(v1->dy);
  dz = fabs(v1->dz);
    // printf(" dx=%f dy=%f dz=%f\n",dx,dy,dz);


  if(dx > dy) {
    if(dz > dx) goto L_z_max;
    else goto L_x_max;

  } else {
    if(dz > dy) goto L_z_max;
    else goto L_y_max;
  }


  // multiply component ibp with length
  // get dd = deviation

  L_x_max:
    if(dx < UT_TOL_min2) goto L_exit_no;
    d1 = fabs(v2->dx / dx);
    dd = fabs(tol * d1 * dx);
    ibp = 0;
    goto L_1;

  L_y_max:
    if(dy < UT_TOL_min2) goto L_exit_no;
    d1 = fabs(v2->dy / dy);
    dd = fabs(tol * d1 * dy);
    ibp = 1;
    goto L_1;

  L_z_max:
    if(dz < UT_TOL_min2) goto L_exit_no;
    d1 = fabs(v2->dz / dz);
    dd = fabs(tol * d1 * dz);
    ibp = 2;


  L_1:
    // printf(" ibp=%d d1=%f dd=%f\n",ibp,d1,dd);
  if(d1 < UT_TOL_min1) goto L_exit_no;


  if(ibp != 0) {
      // printf(" dx=%f\n",(dx * d1) - fabs(v2->dx));
    if(fabs((dx * d1) - fabs(v2->dx)) > dd) goto L_exit_no;
  }

  if(ibp != 1) {
      // printf(" dy=%f\n",(dy * d1) - fabs(v2->dy));
    if(fabs((dy * d1) - fabs(v2->dy)) > dd) goto L_exit_no;
  }

  if(ibp != 2) {
      // printf(" dz=%f\n",(dz * d1) - fabs(v2->dz));
    if(fabs((dz * d1) - fabs(v2->dz)) > dd) goto L_exit_no;
  }


    // printf("ex UT3D_comp2vc_d 1\n");
  return 1;

  L_exit_no:  // vectors totally different
      // printf("ex UT3D_comp2vc_d 0\n");
    return 0;

}


//====================================================================
  void UT3D_vc_setLength (Vector *vco, Vector *vci, double new_len) {
//====================================================================
// UT3D_vc_setLength               change vectorlength
// 
// vco und vci duerfen ident sein
// 
// use UT3D_vc_multvc with vectors of length=1


  double len;

  /* printf("UT2D_vc_setLength %f %f %f %f\n", */
                   /* vci->dx,vci->dy,vci->dz,new_len); */

  len = UT3D_len_vc (vci);

  if (len != 0.0)
    {
      len = new_len / len;
      vco->dx = vci->dx*len;
      vco->dy = vci->dy*len;
      vco->dz = vci->dz*len;
      /* printf("UT3D_vc_setLength %f %f %f\n",vco->dx,vco->dy,vco->dz); */
    }
  else
    {
      // printf("**** UT3D_vc_setLength error\n");
      *vco = UT3D_VECTOR_X;
    }

}


//========================================================================
  void UT3D_vc_perp2vc (Vector *vp, Vector *v1, Vector *v2) {
//========================================================================
// UT3D_vc_perp2vc           vector = perpendic. to 2 vectors (crossprod)
//   NO OUTPUT if v1-v2 is parallel or antiparallel (can BE 0,0,0)
//
// Output:
//   vp       vector normal to v1 and v2;
//            length = 1 only if v1 and v2 are normalized AND have angle = 90 deg
// 
// 
// Get VZ from VX, VY:    UT3D_vc_perp2vc (&vz, &vx, &vy);
// Get VY from VZ, VX:    UT3D_vc_perp2vc (&vy, &vz, &vx);
// Get VX from VY, VZ:    UT3D_vc_perp2vc (&vx, &vy, &vz);
//
// Get -VZ from VX, VY:    UT3D_vc_perp2vc (&vz, &vy, &vx);
// Get -VY from VX, VZ:    UT3D_vc_perp2vc (&vy, &vx, &vz);
// Get -VX from VY, VZ:    UT3D_vc_perp2vc (&vx, &vz, &vy);
//
// Rotation 90 deg CCW:
//   get vp = v2 rotated 90 deg CCW around axis v1
//
// Rotation 90 deg CW:
//   get vp = v1 rotated 90 deg CW around axis v2
//
//
//    vp (z)
//     |    /v2 (y)
//     |   / 
//     |  /                  
//     | / 
//     x----------v1 (x)      angle v1 v2 < PI
//      
//           
//        /v1 (x)
//       / 
//      / 
//     x----------v2 (y)       angle v1 v2 > PI
//     |
//     |
//     |
//    vp (z)
//     


  Vector vn;

    // DEB_dump_obj__ (Typ_VC, v1, " _perp2vc v1");
    // DEB_dump_obj__ (Typ_VC, v2, " _perp2vc v2");

    vn.dx = v1->dy * v2->dz - v1->dz * v2->dy;
    vn.dy = v1->dz * v2->dx - v1->dx * v2->dz;
    vn.dz = v1->dx * v2->dy - v1->dy * v2->dx;
    *vp = vn;

    // DEB_dump_obj__ (Typ_VC, &vn, "ex UT3D_vc_perp2vc vn");
    // DEB_dump_obj__ (Typ_VC, vp, "ex UT3D_vc_perp2vc vp");

}


//================================================================
  int UT3D_ptDi_intptvcpln (Point *ip, double *dist,
                            Plane *pl, Point *pt, Vector *vln) {
//================================================================
/// \code
/// UT3D_ptDi_intptvcpln      intersect line-plane (pt-vc  pt-vc)
/// provide also parameter of intersection-point on line.
/// Von pl wird nur p und vz benutzt.
/// 
/// Input:
///   pt,vln     line to intersect with plane pl
/// Output:
///   ip         intersectionpoint; 
///   dist       der Faktor fuer den Abstand vom pt zum ip in Richtung vln.
///              vln * dist ab pt ergibt ip.
/// Returncodes:
///   0 = vc parallel to plane
///   1 = OK;
///
/// see also UT3D_pt_intplnln UT3D_pt_intlnpl1 UT3D_pt_intptvcpln
/// see also UPRJ_app_pt UT3D_pt_intptvcplx/y/z
/// \endcode

  double   k1, k2;
  Vector   v1;





  // printf("UT3D_pt_intptvcpl_ %f %f %f\n",pt->x,pt->y,pt->z);
  // printf("             vc = %f %f %f\n",vln->dx,vln->dy,vln->dz);
  // printf("          pl-po = %f %f %f\n",pl->po.x,pl->po.y,pl->po.z);
  // printf("          pl-vz = %f %f %f\n",pl->vz.dx,pl->vz.dy,pl->vz.dz);
  // printf("          pl-p  = %f\n",pl->p);



  k1 = pl->vz.dx * vln->dx + pl->vz.dy * vln->dy + pl->vz.dz * vln->dz;
  // printf(" k1=%f\n",k1);

  if (fabs(k1) < 1E-12 ) {
  // if ( fabs(k1) == 0.) {

    // printf("UT3D_pt_intptvcpl: Vector parallel to plane\n");
    // printf(" vc   %f %f %f\n",vln->dx,vln->dy,vln->dz);
    // printf(" pl-Z %f %f %f\n",pl->vz.dx,pl->vz.dy,pl->vz.dz);

    return 0;
  }


/* MUSS bei Plane gesetzt sein !!
  pl->p = pl->vz.dx * pl->po.x +
          pl->vz.dy * pl->po.y +
          pl->vz.dz * pl->po.z;
  printf(" pl->p=%f\n",pl->p);
*/


  k2 = pl->p - pl->vz.dx * pt->x -
               pl->vz.dy * pt->y -
               pl->vz.dz * pt->z;

  *dist = k2/k1;

  // printf("  intptvcpl k1=%f k2=%f dist=%f\n",k1,k2,*dist);

  v1.dx = vln->dx * *dist;
  v1.dy = vln->dy * *dist;
  v1.dz = vln->dz * *dist;

  // printf("  intptvcpl %f %f %f\n",v1.dx,v1.dy,v1.dz);

  UT3D_pt_traptvc (ip, pt, &v1);

  // printf("ex UT3D_pt_intptvcpl_ %f %f %f\n",ip->x,ip->y,ip->z);

  return 1;

}


//================================================================
  Vector UT3D_vc_rotvc_angZ (Vector *vci, double angZ) {
//================================================================
// UT3D_vc_rotvc_angZ                 rotate a vector around Z-axis
// ar = radians
// 
// vco and vci can be the same adress

  Vector  vs;
  double  sin_a, cos_a;

  sin_a = sin(angZ);
  cos_a = cos(angZ);

  vs.dx = cos_a * vci->dx - sin_a * vci->dy;
  vs.dy = sin_a * vci->dx + cos_a * vci->dy;
  vs.dz = vci->dz;

    printf(" ex-UT3D_vc_rotvc_angZ %f %f %f a=%f\n",
           vs.dx,vs.dy,vs.dz,UT_DEGREES(angZ));

  return vs;

}


//================================================================
  Vector UT3D_vc_rotvc_angX (Vector *vci, double angX) {
//================================================================
// UT3D_vc_rotvc_angX                rotate a vector around the X-axis

  Vector  vs;
  double  sin_a, cos_a;
  
  sin_a = sin(angX);
  cos_a = cos(angX);

  vs.dx = vci->dx;
  vs.dy = cos_a * vci->dy - sin_a * vci->dz;
  vs.dz = sin_a * vci->dy + cos_a * vci->dz;
  
    printf(" ex-UT3D_vc_rotvc_angX %f %f %f a=%f\n",
           vs.dx,vs.dy,vs.dz,UT_DEGREES(angX));

  return vs;

}


//========================================================================
  Vector UT3D_vc_rotvc_angZ_angX (Vector *vci, double angZ, double angX) {
//========================================================================
// UT3D_vc_rotvc_angZ_angX     rotate a vector around Z-axis then around X-axis

  Vector  vc1, vc2;
  double  sin_a, cos_a;

  sin_a = sin(angZ);
  cos_a = cos(angZ);

  vc1.dx = cos_a * vci->dx - sin_a * vci->dy;
  vc1.dy = sin_a * vci->dx + cos_a * vci->dy;
  vc1.dz = vci->dz;


  sin_a = sin(angX);
  cos_a = cos(angX);

  vc2.dx = vc1.dx;
  vc2.dy = cos_a * vc1.dy - sin_a * vc1.dz;
  vc2.dz = sin_a * vc1.dy + cos_a * vc1.dz;

    printf(" ex-UT3D_vc_rotvc_angX %f %f %f az=%f ax=%f\n",
           vc2.dx,vc2.dy,vc2.dz,UT_DEGREES(angZ),UT_DEGREES(angX));

  return vc2;

}


// EOF
