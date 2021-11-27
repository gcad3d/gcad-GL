/* ../ut/dump.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

- dump and debug - functions

=====================================================
List_functions_start:

List_functions_end:
=====================================================

*/

#ifdef globTag
 void DEB(){}
#endif




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>                    // va_list

#include "../ut/geo.h"                     // Point Plane Mat_4x4D ..

#define DEB_dump_obj_out(cps)\
  printf("%s\n",(cps));


//===============================================================
  int DEB_dump_obj__ (int typ, void *data, char *txt, ...) {
//===============================================================

  char     cps[512];
  Line2    *ln2;


  //----------------------------------------------------------------
  if(typ == Typ_LN2) {
    ln2 = data;
    sprintf(cps,"2D-Line %s typ = %d",txt,ln2->typ);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_CI);
    DEB_dump_obj_out (cps);
    sprintf(cps,"(Line2).p1=%9.3f,%9.3f",ln2->p1.x,ln2->p1.y);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    DEB_dump_obj_out (cps);
    sprintf(cps,"(Line2).p2=%9.3f,%9.3f",ln2->p2.x,ln2->p2.y);
    // UT3D_dump_add (sTab, cbuf, ipar, ICO_PT);
    DEB_dump_obj_out (cps);

  //----------------------------------------------------------------
  } else {
    TX_Error ("DEB_dump_obj__ unsupp. typ %d",typ);
    return -1;
  }


  //----------------------------------------------------------------

  return 0;

}


//================================================================
  int DEB_dump_pt (Point *pt, char *txt, ...) {
//================================================================
// see DEB_dump_obj__

  va_list    va;
  char       cbuf[256];


  va_start(va,txt);
  vsprintf(cbuf,txt,va);
  va_end(va);

  printf("%s %9.3f,%9.3f,%9.3f\n",cbuf,pt->x,pt->y,pt->z);

  return 0;

}


//================================================================
  int DEB_dump_vc (Vector *vc, char *txt, ...) {
//================================================================
// see DEB_dump_obj__

  va_list    va;
  char       cbuf[256];


  va_start(va,txt);
  vsprintf(cbuf,txt,va);
  va_end(va);

  printf("%s %9.3f,%9.3f,%9.3f\n",cbuf,vc->dx,vc->dy,vc->dz);

  return 0;

}


//================================================================
  int DEB_dump_pvc (VecPos *pvc, char *txt, ...) {
//================================================================
// see DEB_dump_obj__

  va_list    va;
  char       cbuf[256];


  va_start(va,txt);
  vsprintf(cbuf,txt,va);
  va_end(va);

  Vector *vc = &pvc->vc;
  printf("%s VecPos %9.3f,%9.3f,%9.3f\n",cbuf,vc->dx,vc->dy,vc->dz);
  Point *pt = &pvc->pos;
  printf("   at %9.3f,%9.3f,%9.3f\n",pt->x,pt->y,pt->z);

  return 0;

}



//================================================================
// EOF
