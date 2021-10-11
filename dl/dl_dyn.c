/* ./dl_dyn.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Displaylist dynamic objects; manage all attributes


=====================================================
List_functions_start:

DLdyn_init__
DLdyn_add__          add new dynamic-DL-record
DLdyn_dump

List_functions_end:
=====================================================
*/


#ifdef globTag
 void DL(){}
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include <GL/gl.h>                     // GL_LINES ..

#include "../ut/ut_types.h"                // UINT_32
#include "../ut/types.h"                   // Typ_PT ..
#include "../ut/ut.h"                      // ICHG01
#include "../ut/geo.h"                     // Point ..
#include "../ut/matrix.h"                  // M44..
#include "../gr/gr.h"                      // Att_ln ColRGB
#include "../gl/gl_shSY2.h"                // 
#include "../app/deb.h"                     // DEB_dump_..

#define extern  // does eliminate "extern" in includefile
#include "../dl/dl_dyn.h"
#undef extern

#include "../app/app.h"                 // Typ_*



//================================================================
  int DLdyn_init__ () {
//================================================================

  int   iSiz;

  // get space for DisplayList
  DLdyn_siz = 1000;
  iSiz = DLdyn_siz * sizeof(DLdynrec);

  printf("DL_init__ -------- %d\n",iSiz);

  DLdyn = (DLdynrec*) malloc (iSiz);
  DLdyn_nr = 0;

}


//=======================================================================
  int DLdyn_add__ (int typ, void *data, float size, void *att) {
//=======================================================================
// DLdyn_add__      add new dynamic-DL-record

  int     irc = 0;

  printf("DLdyn_add__ DL_nr=%d typ=%d size=%f\n",DLdyn_nr,typ,size);

  switch (typ) {

    case SYM_TRIA:
      DLdyn[DLdyn_nr].oTyp = iSym_tria;
      DLdyn[DLdyn_nr].lTyp = GL_LINE_STRIP;
      break;

    case SYM_PLN:
      DLdyn[DLdyn_nr].oTyp = iSym_pln;
      DLdyn[DLdyn_nr].lTyp = GL_LINE_LOOP;
      break;

    case SYM_STAR:
      DLdyn[DLdyn_nr].oTyp = iSym_star;
      DLdyn[DLdyn_nr].lTyp = GL_LINES;
      break;

    case SYM_ARH2:
      DLdyn[DLdyn_nr].oTyp = iSym_arh2;
      DLdyn[DLdyn_nr].lTyp = GL_LINE_STRIP;
      break;

    case SYM_ARH3:
      DLdyn[DLdyn_nr].oTyp = iSym_arh3;
      DLdyn[DLdyn_nr].lTyp = GL_LINE_STRIP;
      break;

    case Typ_PVC:
      DEB_dump_pvc (data, "DLdyn_add__");
      irc = DLdyn_add_pvc (data, size, att);
      goto L_done;

/* UNUSED: display curve
      UT3D_pt_traptvc (&pta[1], &pta[0], &((VecPos*)data)->vc);
      iOff = GL_shSY2_add_cv (pta, 2);  // add the points into SY-buffer
      DLdyn[DLdyn_nr].oTyp = -Typ_CV;         // negative - no symbol
      DLdyn[DLdyn_nr].lTyp = GL_LINE_STRIP;
      DLdyn[DLdyn_nr].iOff  = iOff;           // data-pos in buffer
      DLdyn[DLdyn_nr].iatt  = *(int*)att;     // store ColRGB | Att_ln | imagesize
      DLdyn[DLdyn_nr].vtxNr = 2;              // nr vertexes
      ++DLdyn_nr;
      if(DLdyn_nr >= DLdyn_siz) goto L_err1;
*/

    default:
      TX_Error ("DLdyn_add__ typ = %d",Typ_PVC);
      return -1;
  }


  // DLdyn[DLdyn_nr].bOff  = iOff;      // store bufferOffset of obj
  DLdyn[DLdyn_nr].iatt  = *(int*)att;     // store ColRGB | Att_ln | imagesize
  DLdyn[DLdyn_nr].pos   = *(Point*)data;      // store startpos
  DLdyn[DLdyn_nr].size  = size;      // store size
  // DLdyn[DLdyn_nr].vtxNr = vtxNr;     // nr vertexes

  ++DLdyn_nr;
  if(DLdyn_nr >= DLdyn_siz) irc = -1;


  L_done:
    if(!irc) return 0;

  L_err1:
    TX_Error("******* ERROR BUFFER OVERFLOW dynamic-DisplayList -");
    TX_Error("******* TODO: realloc DLdyn.");
    return -1;

}


//================================================================
  int DLdyn_add_pvc (void *data, float size, void *att) {
//================================================================
// DLdyn_add_pvc               add 3D-vector from VecPos
// INPUT:
//   data         VecPos*
//   size         0.f = true size (user-units);
//                else absolute-size (in mm, - alway same size);

  int       iOff;
  double    az, ay, lv;
  Point     pta[2];
  Vector    vc1;

  printf("DLdyn_add_pvc size=%f\n",size);

  // UT3D_pt_traptvc (&pta[1], &pta[0], &((VecPos*)data)->vc);

  if(size != 0.f) {
    // normalized-length
    UT3D_vc_setLength (&vc1, &((VecPos*)data)->vc, 1.);

  } else {
    // true-length
    vc1 = ((VecPos*)data)->vc;
  }


  // 0=startpt, 1=endPt (normalized: length 1)
    pta[0] = ((VecPos*)data)->pos;
    UT3D_pt_traptvc (&pta[1], &pta[0], &vc1);

    UT3D_vc_invert (&vc1, &((VecPos*)data)->vc);
    // get az = angle rotation around Z and ay = tilt-angle

    UT3D_2angr_vc__ (&az, &ay, &vc1);
    // UT3D_angrZX_vc (&az, &ay, &((VecPos*)data)->vc);
      printf(" DLdyn_add_pvc-az=%f %f ay=%f %f\n",
             az,UT_DEGREES(az),ay,UT_DEGREES(ay));
    // az -= RAD_90;
    az = UTP_angr_set_0_2pi (az);
    // reverse angle
    // if(?)
    // ay += RAD_180;
    // ay = RAD_360 - ay;
    ay = UTP_angr_set_0_2pi (ay);


  // add the vector as line into buffer
  iOff = GL_shSY2_add_cv (pta, 2);  // add the points into SY-buffer


  DLdyn[DLdyn_nr].oTyp  = -Typ_PVC;         // negative - no symbol
  DLdyn[DLdyn_nr].lTyp  = GL_LINE_STRIP;
  DLdyn[DLdyn_nr].iOff  = iOff;             // data-pos in buffer
  DLdyn[DLdyn_nr].vtxNr = 2;              // nr vertexes
  DLdyn[DLdyn_nr].pos   = ((VecPos*)data)->pos;// startPos
  DLdyn[DLdyn_nr].size  = size;
  // DLdyn[DLdyn_nr].az    = az;                  // store angle around Z
  // DLdyn[DLdyn_nr].ay    = ay;                  // store angle aroung new Y
  DLdyn[DLdyn_nr].iatt  = *(int*)att;      // store ColRGB | Att_ln | imagesize
  DLdyn[DLdyn_nr].vtxNr = 2;               // nr vertexes

  ++DLdyn_nr;
  if(DLdyn_nr >= DLdyn_siz) return -1;


  // add the arrowhead
  DLdyn[DLdyn_nr].oTyp  = -Typ_PVC;         // negative - no symbol
  DLdyn[DLdyn_nr].lTyp  = GL_LINE_STRIP;
  DLdyn[DLdyn_nr].iOff  = iOff;             // data-pos in buffer
  DLdyn[DLdyn_nr].vtxNr = 2;              // nr vertexes
  DLdyn[DLdyn_nr].pos   = pta[1];         // endPos true-length|length-1
  DLdyn[DLdyn_nr].size  = size;
  DLdyn[DLdyn_nr].az    = az;                  // store angle around Z
  DLdyn[DLdyn_nr].ay    = ay;                  // store angle aroung new Y
  DLdyn[DLdyn_nr].iatt  = *(int*)att;      // store ColRGB | Att_ln | imagesize
  DLdyn[DLdyn_nr].vtxNr = 2;               // nr vertexes

  ++DLdyn_nr;
  if(DLdyn_nr >= DLdyn_siz) return -1;





  //----------------------------------------------------------------
// TODO: // add symbol iSym_arh3 at pta[1] oriented with ((VecPos*)data)->vc);
//       DLdyn_add_cv (pta, 2);

  return 0;

}


//================================================================
  int DLdyn_dump () {
//================================================================

  int    i1;
  char   s1[80];
  Att_ln *att;
  ColRGB *col;



  printf("DLdyn_dump :::::::::::::::::::::::: \n");

  for(i1=0; i1<DLdyn_nr; ++i1) {
    // point,curve - iatt = Att_ln    see Att_dump
    att = (Att_ln*)&DLdyn[i1].iatt;
    sprintf(s1, "ATT %02x %02x %02x dash=%d thick=%d",
            att->cr, att->cg, att->cb, att->dash, att->thick);

    printf("%3d oTyp=%2d lTyp=%2d size=%f vtxNr=%d %s\n",i1,
           DLdyn[i1].oTyp, DLdyn[i1].lTyp, DLdyn[i1].size, DLdyn[i1].vtxNr, s1);
  }

  return 0;

}


// EOF
