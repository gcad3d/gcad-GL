/* ./dl.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Displaylist; manage all attributes for the grafic objects


=====================================================
List_functions_start:

DL_init__
DL_add__           create new DL-record
DL_hide__          hide | reDisplay obj
DL_dli_typ_dbi     get dispListIndex from typ, dataBaseIndex
GL_col_dl          get color for obj <dli> from dispList
DL_dump

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

#include "./ut_types.h"                // UINT_32
#include "./ut.h"                      // ICHG01
#include "./geo.h"                     // Point ..
#include "./gr.h"                      // Att_ln ColRGB

#define extern  // does eliminate "extern" in includefile
#include "./dl.h"       
#undef extern

#include "tst_prim1.h"                 // Typ_*


//================================================================
  int DL_init__ () {
//================================================================

  int   iSiz;

  // get space for DisplayList
  DL_siz = 1000;
  iSiz = DL_siz * sizeof(DLrec);

  printf("DL_init__ -------- %d\n",iSiz);

  DL__ = (DLrec*) malloc (DL_siz * sizeof(DLrec));
  DL_nr = 0;

}


//================================================================
  int DL_add__ (int typ, int dbi, int *iatt, int iOff, int vtxNr) {
//================================================================
// DL_add__      create new DL-record
  

  printf("DL_add__ DL_nr=%d dbi=%d iOff=%d\n",DL_nr,dbi,iOff);


  DL__[DL_nr].typ   = typ;       // store objTyp of obj
  DL__[DL_nr].dbi   = dbi;       // store dataBaseIndex of obj
  DL__[DL_nr].bOff  = iOff;      // store bufferOffset of obj
  DL__[DL_nr].iatt  = *iatt;     // store ColRGB | Att_ln | imagesize
  DL__[DL_nr].vtxNr = vtxNr;     // store ColRGB | Att_ln | imagesize

  ++DL_nr;

  if(DL_nr >= DL_siz) {
    TX_Error("******* ERROR BUFFER OVERFLOW DisplayList -");
    TX_Error("******* TODO: realloc DL__.");
    return -1;
  }


  return 0;

}


//================================================================
  int DL_hide__ (int dli, int mode) {
//================================================================
// DL_hide__          hide | reDisplay obj
// Input:
//   dli         dispListIndex
//   mode        0 = display; 1 = hide;  -1 = swap


  if(!mode) {
    // reDisplay
    DL__[dli].disp = 0;

  } else if (mode > 0) {
    // hide
    DL__[dli].disp = 1;

  } else {
    // swap
    DL__[dli].disp = ICHG01(DL__[dli].disp);

  }

  return 0;

} 


//================================================================
  int DL_dli_typ_dbi (int typ, int dbi) {
//================================================================
// DL_dli_typ_dbi     get dispListIndex from typ, dataBaseIndex

  int   i1, dli;

  printf("DL_dli_typ_dbi typ=%d dbi=%d\n",typ,dbi);


  for(i1=DL_nr-1; i1>=0; --i1) {
    if(DL__[i1].typ != typ) continue;
    if(DL__[i1].dbi != dbi) continue;
    dli = i1;
    goto L_exit;
  }

  dli = -1;


  L_exit:
    printf(" ex-DL_dli_typ_dbi dli=%d typ=%d dbi=%d\n",dli,typ,dbi);
  return dli;

}


//================================================================
  int DL_col_dl (float *fCol, int dli) {
//================================================================
// GL_col_dl         get color for obj <dli> from dispList

  ColRGB  *col =  (ColRGB*)(&DL__[dli].iatt);

  fCol[0] = (float)col->cr / 256.f;
  fCol[1] = (float)col->cg / 256.f;
  fCol[2] = (float)col->cb / 256.f;

    printf(" ex-GL_col_dl %f %f %f %f\n",fCol[0],fCol[1],fCol[2],fCol[3]);

  return 0;

}


//================================================================
  int DL_dump () {
//================================================================
 
  int    i1;
  char   s1[80];
  Att_ln *att;
  ColRGB *col;



  printf("DL_dump :::::::::::::::::::::::: \n");

  for(i1=0; i1<DL_nr; ++i1) {
    if(DL__[i1].typ < Typ_GL_Sur) {
      // point,curve - iatt = Att_ln    see Att_dump
      att = (Att_ln*)&DL__[i1].iatt;
      sprintf(s1, "ATT %02x %02x %02x dash=%d thick=%d",
          att->cr, att->cg, att->cb, att->dash, att->thick);
    } else {
      // surface - iatt = ColRGB        see Col_dump
      col = (ColRGB*)&DL__[i1].iatt;
      sprintf(s1, "RGB %02x %02x %02x tra=%d sym=%d tex=%d col=%d",
            col->cr, col->cg, col->cb,
            col->vtra, col->vsym, col->vtex, col->color);
    }



     printf("%3d typ=%2d dbi=%2d iOff=%3d disp=%d hili=%d dim=%d %s\n",i1,
            DL__[i1].typ, DL__[i1].dbi, DL__[i1].bOff,
            DL__[i1].disp, DL__[i1].hili, DL__[i1].dim, s1);
  }

  return 0;

}


// EOF
