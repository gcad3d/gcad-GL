/* ./dl_sym_dyn.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Displaylist dynamic objects; manage all attributes


=====================================================
List_functions_start:

DL_SYP_init__
DL_SYP_add__          add new dynamic-DL-record
DL_SYP_dump

List_functions_end:
=====================================================
*/


#ifdef globTag
 void DL_SYP(){}
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
#include "../gl/gl.h"                      // GL_add_cv
// #include "../gl/gl_shSY2.h"                // 
#include "../ut/deb.h"                     // DEB_dump_..

#define extern  // does eliminate "extern" in includefile
#include "../dl/dl_sym_perm.h"
#undef extern

#include "../app/app.h"                 // Typ_*



//================================================================
  int DL_SYP_init__ () {
//================================================================

  int   iSiz;

  // get space for DisplayList
  DL_SYP_siz = 1000;
  iSiz = DL_SYP_siz * sizeof(DL_SYP_rec);

  printf("DL_SYP_init__ -------- %d\n",iSiz);

  DL_SYP_tab = (DL_SYP_rec*) malloc (iSiz);
  DL_SYP_nr = 0;

}


//=======================================================================
  int DL_SYP_add__ (int typ, void *data, float size, void *att) {
//=======================================================================
// DL_SYP_add__      add new dynamic-DL-record

  int     irc = 0;

  printf("DL_SYP_add__ DL_nr=%d typ=%d size=%f\n",DL_SYP_nr,typ,size);

  switch (typ) {

    case Typ_LN2:
      DEB_dump_obj__ (Typ_LN2, data, "DL_SYP_add__ ");
      irc = DL_SYP_add_ln2 (data, size, att);
      goto L_done;

/*
    case SYM_PT:
      DL_SYP_tab[DL_SYP_nr].oTyp = iSym_pt;
      DL_SYP_tab[DL_SYP_nr].lTyp = GL_POINTS;
      break;

    case SYM_TRIA:
      DL_SYP_tab[DL_SYP_nr].oTyp = iSym_tria;
      DL_SYP_tab[DL_SYP_nr].lTyp = GL_LINE_STRIP;
      break;

    case SYM_PLN:
      DL_SYP_tab[DL_SYP_nr].oTyp = iSym_pln;
      DL_SYP_tab[DL_SYP_nr].lTyp = GL_LINE_LOOP;
      break;

    case SYM_STAR:
      DL_SYP_tab[DL_SYP_nr].oTyp = iSym_star;
      DL_SYP_tab[DL_SYP_nr].lTyp = GL_LINES;
      break;

    case SYM_ARH2:
      DL_SYP_tab[DL_SYP_nr].oTyp = iSym_arh2;
      DL_SYP_tab[DL_SYP_nr].lTyp = GL_LINE_STRIP;
      break;

    case SYM_ARH3:
      DL_SYP_tab[DL_SYP_nr].oTyp = iSym_arh3;
      DL_SYP_tab[DL_SYP_nr].lTyp = GL_LINE_STRIP;
      break;

    case Typ_PVC:
      DEB_dump_pvc (data, "DL_SYP_add__");
      irc = DL_SYP_add_pvc (data, size, att);
      goto L_done;

// UNUSED: display curve
//    UT3D_pt_traptvc (&pta[1], &pta[0], &((VecPos*)data)->vc);
//    iOff = GL_shSY2_add_cv (pta, 2);  // add the points into SY-buffer
//    DL_SYP_tab[DL_SYP_nr].oTyp = -Typ_CV;         // negative - no symbol
//    DL_SYP_tab[DL_SYP_nr].lTyp = GL_LINE_STRIP;
//    DL_SYP_tab[DL_SYP_nr].iOff  = iOff;           // data-pos in buffer
//    DL_SYP_tab[DL_SYP_nr].iatt  = *(int*)att;     // store ColRGB | Att_ln | imagesize
//    DL_SYP_tab[DL_SYP_nr].vtxNr = 2;              // nr vertexes
//    ++DL_SYP_nr;
//    if(DL_SYP_nr >= DL_SYP_siz) goto L_err1;
*/

    default:
      TX_Error ("DL_SYP_add__ typ = %d",Typ_PVC);
      return -1;
  }


  DL_SYP_tab[DL_SYP_nr].iatt  = *(int*)att;     // store ColRGB | Att_ln | imagesize
  // DL_SYP_tab[DL_SYP_nr].pos   = *(Point*)data;      // store startpos
  // DL_SYP_tab[DL_SYP_nr].size  = size;      // store size

  ++DL_SYP_nr;
  if(DL_SYP_nr >= DL_SYP_siz) irc = -1;


  L_done:
    if(!irc) return 0;

  L_err1:
    TX_Error("******* ERROR BUFFER OVERFLOW dynamic-DisplayList -");
    TX_Error("******* TODO: realloc DLdyn.");
    return -1;

}



//================================================================
  int DL_SYP_add_ln2 (Line2 *data, float size, void *att) {
//================================================================
// DL_SYP_add_ln2               add 2D-line from *Line2
// INPUT:
//   data         VecPos*
//   size         0.f = true size (user-units);
//                else absolute-size (in mm, - alway same size);

  int       iOff;
  double    az, ay, lv;
  double    pta[6];
  Vector    vc1;
  Att_ln    iatt;


  printf("DL_SYP_add_ln2 size=%f\n",size);



  // DL_SYP_tab[DL_SYP_nr].lTyp  = GL_LINE_STRIP;

  // copy ln2.p1 -> pos
  pta[0] = ((Line2*)data)->p1.x;
  pta[1] = ((Line2*)data)->p1.y;
  pta[2] = 0.f;

  // copy ln2.p2 -> ax,ay,az
  pta[3] = ((Line2*)data)->p2.x;
  pta[4] = ((Line2*)data)->p2.y;
  pta[5] = 0.f;

  // DL_SYP_tab[DL_SYP_nr].vtxNr = 2;              // nr vertexes


  if(att) {
    iatt  = *(Att_ln*)att;
  } else {
    iatt  = cvAtt;  // default
    // iatt  = defAtt;  // default
  }


  // // store dataBaseIndex, bufferOffset (in bytes), objTyp in dispList
  // DL_add__ (Typ_CV, dbi, objID, (int*)&iatt, iOff, ptNr);


  iOff = GL_add_cv (pta, 2, iatt, -1);

  DL_SYP_tab[DL_SYP_nr].iOff  = iOff;
  DL_SYP_tab[DL_SYP_nr].lTyp  = GL_LINES;
  DL_SYP_tab[DL_SYP_nr].vtxNr = 2;



  ++DL_SYP_nr;
  if(DL_SYP_nr >= DL_SYP_siz) return -1;

  return 0;

}


//================================================================
  int DL_SYP_add_pvc (void *data, float size, void *att) {
//================================================================
// DL_SYP_add_pvc               add 3D-vector from VecPos
// INPUT:
//   data         VecPos*
//   size         0.f = true size (user-units);
//                else absolute-size (in mm, - alway same size);

  int       iOff;
  double    az, ay, lv;
  Point     pta[2];
  Vector    vc1;

  printf("DL_SYP_add_pvc size=%f\n",size);

/*  TODO
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
      printf(" DL_SYP_add_pvc-az=%f %f ay=%f %f\n",
             az,UT_DEGREES(az),ay,UT_DEGREES(ay));
    az = UTP_angr_set_0_2pi (az);
    ay = UTP_angr_set_0_2pi (ay);


  // add the vector as line into buffer
  iOff = GL_shSY2_add_cv (pta, 2);  // add the points into SY-buffer

  DL_SYP_tab[DL_SYP_nr].oTyp  = -Typ_PVC;         // negative - no symbol
  DL_SYP_tab[DL_SYP_nr].lTyp  = GL_LINE_STRIP;
  DL_SYP_tab[DL_SYP_nr].iOff  = iOff;             // data-pos in buffer
  DL_SYP_tab[DL_SYP_nr].pos   = ((VecPos*)data)->pos;// startPos
  DL_SYP_tab[DL_SYP_nr].size  = size;
  DL_SYP_tab[DL_SYP_nr].az    = az;                  // store angle around Z
  DL_SYP_tab[DL_SYP_nr].ay    = ay;                  // store angle aroung new Y
  DL_SYP_tab[DL_SYP_nr].iatt  = *(int*)att;      // store ColRGB | Att_ln | imagesize
  DL_SYP_tab[DL_SYP_nr].vtxNr = 2;               // nr vertexes

  ++DL_SYP_nr;
  if(DL_SYP_nr >= DL_SYP_siz) return -1;


  // add the arrowhead
  DL_SYP_tab[DL_SYP_nr].oTyp  = -Typ_PVC;         // negative - no symbol
  DL_SYP_tab[DL_SYP_nr].lTyp  = GL_LINE_STRIP;
  DL_SYP_tab[DL_SYP_nr].iOff  = iOff;             // data-pos in buffer
  DL_SYP_tab[DL_SYP_nr].pos   = pta[1];         // endPos true-length|length-1
  DL_SYP_tab[DL_SYP_nr].size  = size;
  DL_SYP_tab[DL_SYP_nr].az    = az;                  // store angle around Z
  DL_SYP_tab[DL_SYP_nr].ay    = ay;                  // store angle aroung new Y
  DL_SYP_tab[DL_SYP_nr].iatt  = *(int*)att;      // store ColRGB | Att_ln | imagesize
  DL_SYP_tab[DL_SYP_nr].vtxNr = 2;               // nr vertexes

  ++DL_SYP_nr;
  if(DL_SYP_nr >= DL_SYP_siz) return -1;
*/




  //----------------------------------------------------------------
// TODO: // add symbol iSym_arh3 at pta[1] oriented with ((VecPos*)data)->vc);
//       DL_SYP_add_cv (pta, 2);

  return 0;

}


//================================================================
  int DL_SYP_dump () {
//================================================================

  int    i1;
  char   s1[80];
  Att_ln *att;
  ColRGB *col;



  printf("DL_SYP_dump :::::::::::::::::::::::: \n");

  for(i1=0; i1<DL_SYP_nr; ++i1) {

    // printf("%3d %8.2f %8.2f %8.2f - %8.2f %8.2f %8.2f \n",i1,
          // DL_SYP_tab[i1].pos.x, DL_SYP_tab[i1].pos.y, DL_SYP_tab[i1].pos.z,
          // DL_SYP_tab[i1].ax,    DL_SYP_tab[i1].ay,    DL_SYP_tab[i1].az);

    // point,curve - iatt = Att_ln    see Att_dump
    att = (Att_ln*)&DL_SYP_tab[i1].iatt;
    printf("%3d  ATT %02x %02x %02x dash=%d thick=%d lTyp=%2d vtxNr=%2d iOff=%4d\n",
            i1,
            att->cr, att->cg, att->cb,
            att->dash, att->thick,
            DL_SYP_tab[i1].lTyp, DL_SYP_tab[i1].vtxNr,
            DL_SYP_tab[i1].iOff);

  }

  return 0;

}


// EOF
