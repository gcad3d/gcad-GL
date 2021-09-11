/* ./col.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

struct Att_ln - handle attributes (color and wiidth) of points, curves


=====================================================
List_functions_start:

Att__3i             get Att_ln (color for curves) from 3 ints
Att_4f_att          get color (4 floats) from Att_ln
Att__width        set width (for points, curves) in Att_ln
Att_dump

Col__3f             get ColRGB from 3 floats
Col__3i             get ColRGB from 3 ints
Col_dump


List_functions_end:
=====================================================

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include "./geo.h"                     // Point Plane Mat_4x4D ..
#include "./gr.h"                      // GR_..
#include "./col.h"                     // ColRGB


//================================================================
  int Att__3i (Att_ln *att1, int cr, int cg, int cb) {
//================================================================
// Att__3i             get Att_ln (color for curves) from 3 ints

  att1->cr = cr / 16;
  att1->cg = cg / 16;
  att1->cb = cb / 16;

    Att_dump (att1, "ex-Att__3i");

  return 0;

}


//================================================================
  void Att_4f_att (float *fCol, Att_ln *att1) {
//================================================================
// Att_4f_att          get color (4 floats) from Att_ln


  fCol[0] = (float)att1->cr / 16.f;
  fCol[1] = (float)att1->cg / 16.f;
  fCol[2] = (float)att1->cb / 16.f;
  fCol[3] = 1.f;

    printf(" ex-Att_4f_att %f %f %f\n",fCol[0],fCol[1],fCol[2]);

}


//================================================================
  int Att__width (Att_ln *att1, float cvw) {
//================================================================
// Att__width        set width (for points, curves) in Att_ln

  printf("Att__width %f\n",cvw);

  att1->thick = cvw;

    Att_dump (att1, "ex-Att__3i");

  return 0;

}


//================================================================
  int Col__3f (ColRGB *col1, float fCol[]) {
//================================================================
// Col__3f             get ColRGB from 3 floats

  printf("Col__3f d1=%f d2=%f d3=%f\n",fCol[0],fCol[1],fCol[2]);

  *col1 = ColRGB_NUL;

  col1->cr = fCol[0] * 255;
  col1->cg = fCol[1] * 255;
  col1->cb = fCol[2] * 255;

  // 0 als Col ist Default - geht ned !!
  if(col1->cr < 1) col1->cr = 1;
  if(col1->cg < 1) col1->cg = 1;
  if(col1->cb < 1) col1->cb = 1;

//   col1->color = 1;
//   col1->vtra  = 0;
//   col1->vsym  = 0;
//   col1->vtex  = 0;

    Col_dump (col1, "ex-Col__3f");

  return 0;

}


//================================================================
  int Col__3i (ColRGB *col1, int cr, int cg, int cb) {
//================================================================
// Col__3i             get ColRGB from 3 ints
// Set vtra,vsym,vtex = 0; color = 1.
// Input:
//   cr         red part of colour; 0-255
//   cg         green part of colour; 0-255
//   cb         blue part of colour; 0-255
// Output:
//   col1

  static int  i0 = 0;


  // printf("Col__3i %d %d %d\n",cr,cg,cb);

  *col1 = ColRGB_NUL;

  col1->cr =  cr;
  col1->cg =  cg;
  col1->cb =  cb;

  return 0;

}


//================================================================
  int Att_dump (Att_ln *att, char *sInf) {
//================================================================

  char   cAtt[80];

  printf("================= Att_dump  %s\n",sInf);

  printf("ATT %02x %02x %02x, dash=%d, thick=%d\n",
          att->cr, att->cg, att->cb, att->dash, att->thick);

  return 0;

}


//================================================================
  int Col_dump (ColRGB *col, char *sInf) {
//================================================================

  char   cAtt[80];

  printf("================= Col_dump  %s\n",sInf);

  if(col->vtex == 0) {
    sprintf(cAtt, "RGB %02x %02x %02x, transp=%d, sym=%d tex=%d color=%d",
            col->cr, col->cg, col->cb,
            col->vtra, col->vsym, col->vtex, col->color);
  } else {
    sprintf(cAtt, "texture %d",((stru_c3c1*)col)->b123);
  }

  printf("  %s\n",cAtt);


  return 0;

}


// EOF
