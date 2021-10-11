/* ../ut/ut2d.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

geometric computations 2D

=====================================================
List_functions_start:

UT2D_angr_vc              get angle (radians) from vector (0-2Pi)
UT2D_vc_setLength         change 2D-Vectorlength

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


#include "../ut/ut.h"                      // TX_Error
#include "../ut/geo.h"                     // Point ..



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


// EOF
