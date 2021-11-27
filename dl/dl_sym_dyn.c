/* ./dl_sym_dyn.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Displaylist dynamic objects; manage all attributes


=====================================================
List_functions_start:

DL_SYD_init__
DL_SYD_add__          add new dynamic-DL-record
DL_SYD_dump

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

#include "../ut/geo.h"                     // Point ..
#include "../ut/matrix.h"                  // M44..
#include "../gr/gr.h"                      // Att_ln ColRGB
#include "../gl/gl_shSY2.h"                // 
#include "../ut/deb.h"                     // DEB_dump_..

#define extern  // does eliminate "extern" in includefile
#include "../dl/dl_sym_dyn.h"
#undef extern

#include "../app/app.h"                 // Typ_*



//================================================================
  int DL_SYD_init__ () {
//================================================================

  int   iSiz;

  // get space for DisplayList
  DL_SYD_siz = 1000;
  iSiz = DL_SYD_siz * sizeof(DL_SYD_rec);

  printf("DL_SYD_init__ -------- %d\n",iSiz);

  DL_SYD_tab = (DL_SYD_rec*) malloc (iSiz);
  DL_SYD_nr = 0;

}


//=======================================================================
  int DL_SYD_add__ (int typ, void *data, float size, void *att) {
//=======================================================================
// DL_SYD_add__      add new dynamic-DL-record

  int     irc = 0;

  printf("DL_SYD_add__ DL_nr=%d typ=%d size=%f\n",DL_SYD_nr,typ,size);

  switch (typ) {

    case SYM_PT:
      DL_SYD_tab[DL_SYD_nr].oTyp = iSym_pt;
      DL_SYD_tab[DL_SYD_nr].lTyp = GL_POINTS;
      break;

    case SYM_TRIA:
      DL_SYD_tab[DL_SYD_nr].oTyp = iSym_tria;
      DL_SYD_tab[DL_SYD_nr].lTyp = GL_LINE_STRIP;
      break;

    case SYM_PLN:
      DL_SYD_tab[DL_SYD_nr].oTyp = iSym_pln;
      DL_SYD_tab[DL_SYD_nr].lTyp = GL_LINE_LOOP;
      break;

    case SYM_STAR:
      DL_SYD_tab[DL_SYD_nr].oTyp = iSym_star;
      DL_SYD_tab[DL_SYD_nr].lTyp = GL_LINES;
      break;

    case SYM_ARH2:
      DL_SYD_tab[DL_SYD_nr].oTyp = iSym_arh2;
      DL_SYD_tab[DL_SYD_nr].lTyp = GL_LINE_STRIP;
      break;

    case SYM_ARH3:
      DL_SYD_tab[DL_SYD_nr].oTyp = iSym_arh3;
      DL_SYD_tab[DL_SYD_nr].lTyp = GL_LINE_STRIP;
      break;

    case Typ_PVC:
      DEB_dump_pvc (data, "DL_SYD_add__");
      irc = DL_SYD_add_pvc (data, size, att);
      goto L_done;

/* UNUSED: display curve
      UT3D_pt_traptvc (&pta[1], &pta[0], &((VecPos*)data)->vc);
      iOff = GL_shSY2_add_cv (pta, 2);  // add the points into SY-buffer
      DL_SYD_tab[DL_SYD_nr].oTyp = -Typ_CV;         // negative - no symbol
      DL_SYD_tab[DL_SYD_nr].lTyp = GL_LINE_STRIP;
      DL_SYD_tab[DL_SYD_nr].iOff  = iOff;           // data-pos in buffer
      DL_SYD_tab[DL_SYD_nr].iatt  = *(int*)att;     // store ColRGB | Att_ln | imagesize
      DL_SYD_tab[DL_SYD_nr].vtxNr = 2;              // nr vertexes
      ++DL_SYD_nr;
      if(DL_SYD_nr >= DL_SYD_siz) goto L_err1;
*/

    default:
      TX_Error ("DL_SYD_add__ typ = %d",Typ_PVC);
      return -1;
  }


  DL_SYD_tab[DL_SYD_nr].iatt  = *(int*)att;     // store ColRGB | Att_ln | imagesize
  DL_SYD_tab[DL_SYD_nr].pos   = *(Point*)data;      // store startpos
  DL_SYD_tab[DL_SYD_nr].size  = size;      // store size

  ++DL_SYD_nr;
  if(DL_SYD_nr >= DL_SYD_siz) irc = -1;


  L_done:
    if(!irc) return 0;

  L_err1:
    TX_Error("******* ERROR BUFFER OVERFLOW dynamic-DisplayList -");
    TX_Error("******* TODO: realloc DLdyn.");
    return -1;

}


//================================================================
  int DL_SYD_add_pvc (void *data, float size, void *att) {
//================================================================
// DL_SYD_add_pvc               add 3D-vector from VecPos
// INPUT:
//   data         VecPos*
//   size         0.f = true size (user-units);
//                else absolute-size (in mm, - alway same size);

  int       iOff;
  double    az, ay, lv;
  Point     pta[2];
  Vector    vc1;

  printf("DL_SYD_add_pvc size=%f\n",size);

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
      printf(" DL_SYD_add_pvc-az=%f %f ay=%f %f\n",
             az,UT_DEGREES(az),ay,UT_DEGREES(ay));
    az = UTP_angr_set_0_2pi (az);
    ay = UTP_angr_set_0_2pi (ay);


  // add the vector as line into buffer
  iOff = GL_shSY2_add_cv (pta, 2);  // add the points into SY-buffer


  DL_SYD_tab[DL_SYD_nr].oTyp  = -Typ_PVC;         // negative - no symbol
  DL_SYD_tab[DL_SYD_nr].lTyp  = GL_LINE_STRIP;
  DL_SYD_tab[DL_SYD_nr].iOff  = iOff;             // data-pos in buffer
  DL_SYD_tab[DL_SYD_nr].vtxNr = 2;              // nr vertexes
  DL_SYD_tab[DL_SYD_nr].pos   = ((VecPos*)data)->pos;// startPos
  DL_SYD_tab[DL_SYD_nr].size  = size;
  DL_SYD_tab[DL_SYD_nr].az    = az;                  // store angle around Z
  DL_SYD_tab[DL_SYD_nr].ay    = ay;                  // store angle aroung new Y
  DL_SYD_tab[DL_SYD_nr].iatt  = *(int*)att;      // store ColRGB | Att_ln | imagesize
  DL_SYD_tab[DL_SYD_nr].vtxNr = 2;               // nr vertexes

  ++DL_SYD_nr;
  if(DL_SYD_nr >= DL_SYD_siz) return -1;


  // add the arrowhead
  DL_SYD_tab[DL_SYD_nr].oTyp  = -Typ_PVC;         // negative - no symbol
  DL_SYD_tab[DL_SYD_nr].lTyp  = GL_LINE_STRIP;
  DL_SYD_tab[DL_SYD_nr].iOff  = iOff;             // data-pos in buffer
  DL_SYD_tab[DL_SYD_nr].vtxNr = 2;              // nr vertexes
  DL_SYD_tab[DL_SYD_nr].pos   = pta[1];         // endPos true-length|length-1
  DL_SYD_tab[DL_SYD_nr].size  = size;
  DL_SYD_tab[DL_SYD_nr].az    = az;                  // store angle around Z
  DL_SYD_tab[DL_SYD_nr].ay    = ay;                  // store angle aroung new Y
  DL_SYD_tab[DL_SYD_nr].iatt  = *(int*)att;      // store ColRGB | Att_ln | imagesize
  DL_SYD_tab[DL_SYD_nr].vtxNr = 2;               // nr vertexes

  ++DL_SYD_nr;
  if(DL_SYD_nr >= DL_SYD_siz) return -1;





  //----------------------------------------------------------------
// TODO: // add symbol iSym_arh3 at pta[1] oriented with ((VecPos*)data)->vc);
//       DL_SYD_add_cv (pta, 2);

  return 0;

}


//================================================================
  int DL_SYD_dump () {
//================================================================

  int    i1;
  char   s1[80];
  Att_ln *att;
  ColRGB *col;



  printf("DL_SYD_dump :::::::::::::::::::::::: \n");

  for(i1=0; i1<DL_SYD_nr; ++i1) {
    // point,curve - iatt = Att_ln    see Att_dump
    att = (Att_ln*)&DL_SYD_tab[i1].iatt;
    sprintf(s1, "ATT %02x %02x %02x dash=%d thick=%d",
            att->cr, att->cg, att->cb, att->dash, att->thick);

    printf("%3d oTyp=%2d lTyp=%2d size=%f vtxNr=%d %s\n",i1,
           DL_SYD_tab[i1].oTyp, DL_SYD_tab[i1].lTyp, DL_SYD_tab[i1].size, DL_SYD_tab[i1].vtxNr, s1);
  }

  return 0;

}


// EOF
