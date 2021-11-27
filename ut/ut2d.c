/* ../ut/ut2d.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

geometric computations 2D

=====================================================
List_functions_start:

UT2D_sidPerp_ptvc        compare if pt is right/on/left of a normal to pl+vc
UT2D_angr_vc             get angle (radians) from vector (0-2Pi)
UT2D_vc_setLength        change 2D-Vectorlength

UT2D_pt_traptvclen       move pi along vc length lenv
UT2D_pt_projptptvc       point = project point to line (pt+vc)

List_functions_end:
=====================================================

*/

#ifdef globTag
 void UT2D(){}
#endif



#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#include "../ut/geo.h"                     // Point ..



//========================================================================
  void UT2D_pt_traptvclen (Point2 *po,Point2 *pi,Vector2 *vc,double lenv) {
//========================================================================
// UT2D_pt_traptvclen    move pi along vc length lenv

  double    l1, lvc;


  lvc = UT2D_len_vc (vc);
  if(lvc < UT_TOL_min2) {
    *po = *pi;
    return;
  }

  l1 = lenv / lvc;

  po->x = pi->x + vc->dx * l1;
  po->y = pi->y + vc->dy * l1;

  /* printf("UT2D_pt_traptvclen %f %f\n",po->x,po->y); */
  /* printf("          pt in %f %f\n",pi->x,pi->y); */
  /* printf("          vc %f %f  - len %f\n",vc->dx,vc->dy,lenv); */

}


//======================================================================
  int UT2D_sidPerp_ptvc (Point2 *pt,  Point2 *pl, Vector2 *vl) {
//======================================================================
// UT2D_sidPerp_ptvc        compare if pt is right/on/left of a normal to pl+vc
//
//              
//   pt      |           pt
//   -1      |           1
//           |  
//           |--------->vl
// ----------X----------------
//           pl
//           |
//
// retcode:
//   0   pt is on a normal to line pl-vl
//   1   pt is before pl (to the right side of a normal to line pl-vl)
//  -1   pt is behind pl (to the left side of a normal to line pl-vl)


  int      rc;
  double   d1;
  Vector2  vp;

  /* ------------------------------------------------------------------- */
  // UT2D_vc_invert (&vn, vl);

  UT2D_vc_2pt (&vp, pl, pt);

  d1 = vp.dx * vl->dx + vp.dy * vl->dy;

  rc = DSIGTOL (d1, UT_TOL_min2);         // 2017-10-04 was UT_TOL_min0

  // if (UTP_comp_0 (d1)) rc =  0;
  // else if (d1 < 0.)    rc = -1;
  // else                 rc =  1;
  // printf("UT2D_sidPerp_ptvc %d %f\n",rc,d1);

  return rc;

}


//====================================================================
void UT2D_vc_setLength (Vector2 *vco, Vector2 *vci, double new_len) {
//====================================================================
// UT2D_vc_setLength                change 2D-Vectorlength
// vco and vci may be the same address
//
// if length of vci is known:
//   UT2D_vc_multvc (&vco, &vci, newLengthOfVco / actLengthOfVci);


  double len;

  len = UT2D_len_vc (vci);

  if (len != 0.0)
    {
      len = new_len / len;
      vco->dx = vci->dx*len;
      vco->dy = vci->dy*len;
      /* printf("UT2D_vc_setLength %f %f %f\n",vco->dx,vco->dy,new_len); */
    }
  else
    {
      /* printf("**** UT2D_vc_setLength error\n"); */
      vco->dx = 1.0;
      vco->dy = 0.0;
    }

}


//======================================================================
  double UT2D_angr_vc (Vector2 *vc) {
//======================================================================
// UT2D_angr_vc              angle (radians) = vector (0-2Pi)
// returns 0 - RAD_360 (PI * 2)
// Example: (1,1) returns RAD_45 (0.785)



  double angr;

  /* parallel to Y - */
  /* if (vc->dx == 0.0) { */
  if (UTP_comp_0 (vc->dx)) {
    if (vc->dy > 0.0 ) {
      angr = RAD_90;
    }
    else if (vc->dy < 0.0 ) {
      angr = RAD_180 + RAD_90;
    }
    else {
      angr = 0.;
    }
    goto Fertig;
  }


  /* parallel to X - */
  /* if (vc->dy == 0.0) { */
  if (UTP_comp_0 (vc->dy)) {
    if (vc->dx > 0.0 ) {
      angr = 0.;
    }
    else if (vc->dx < 0.0 ) {
      angr = RAD_180;
    }
    else {
      angr = 0.;
    }
    goto Fertig;
  }


  if ( fabs(vc->dx) < fabs(vc->dy)) {
    /* printf(" > 45 \n"); */
    angr = atan (vc->dy / vc->dx);
    if ( vc->dx < 0.0 ) angr = angr + RAD_180;
  } else {
    /* printf(" < 45\n"); */
    angr = RAD_90 - (atan (vc->dx / vc->dy));
    if ( vc->dy < 0.0 ) angr = angr + RAD_180;
  }

  if ( angr < 0.0 ) angr = angr + RAD_360;    /* + 360 */



  Fertig:
  /* printf("UT2D_angr_vc %f %f\n",angr, angr/RAD_1); */

  return angr;
}


//====================================================================
  double UT2D_len_2pt (Point2 *p1,Point2 *p2) {
//====================================================================
// UT2D_len_2pt              distance pt - pt

  double dx, dy;

  dx = p2->x - p1->x;
  dy = p2->y - p1->y;

  // printf("UT2D_len_2pt dx=%f dy=%f\n",dx,dy);


  return (sqrt(dx*dx + dy*dy));

}


//================================================================
  int UT2D_pt_projptptvc (Point2 *pp, Point2 *pt, Point2 *pl, Vector2 *vl) {
//================================================================
// UT2D_pt_projptptvc        point = project point to line (pt+vc)
// Project a point onto a line given by point and vector.
// pl - vl gives a line, pt is projected onto this line.
//
// IN:
//   Point2 *pt   ... point to project on line pl-vl
//   Point2 *pl   ... point on line
//   Vector2 *vl  ... vector of line
// OUT:
//   Point2 *pp   ... normal projection of pt onto line
// Returncodes:
//   0 = OK
//   1 = point is on line


   double  sprod,lenl;
   Vector2 vp;


  /* change linstartpoint > ext.point into vector vp */
  UT2D_vc_2pt (&vp, pl, pt);

  /* length of Line */
  lenl = UT2D_len_vc (vl);

  /* wenn lenl==0 dann pp=pt */
  if(lenl < UT_TOL_min2) {
    *pp = *pt;
    return 1;
  }

  /* Project vp on vl. */
  sprod = (vl->dx*vp.dx + vl->dy*vp.dy) / lenl;

  /* printf(" %f %f\n",lenl,sprod); */

  /* compute prjPt on vector */
  sprod /= lenl;
  vp.dx = vl->dx * sprod;
  vp.dy = vl->dy * sprod;

  UT2D_pt_traptvc (pp, pl, &vp);


  /* printf("UT2D_pt_projptptvc %f %f\n",pp->x,pp->y); */
  /* printf("      pt %f %f\n",pt->x,pt->y); */
  /* printf("      pl %f %f\n",pl->x,pl->y); */
  /* printf("      vl %f %f\n",vl->dx,vl->dy); */

  return UT2D_comp2pt(pt, pp, UT_TOL_pt);
  // return UT2D_comp2pt(pt, pp, UT_TOL_min1);

}


// EOF
