/*  ./glbt.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

2D-system-buttons

=====================================================
List_functions_start:

GLBT_but_ini
GLBT_sele__        test if 2D-system-button selected
GLBT_but_add       add GL-2D-button
GLBT_render
GLBT_dump

List_functions_end:
=====================================================

*/



#ifdef globTag
 void GLBT(){}
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

#include <GL/gl.h>


#include "./geo.h"                     // Point Plane Mat_4x4D ..
#include "./ut.h"                      // TX_Print
#include "./matrix.h"
#include "./gr.h"                      // Att_ln GR_..
#include "./gl.h"                      // shSY2
#include "./gl_shadSY2.h"              // symTab
#include "./glbt.h"


#define I2D_TABSITZ  32


static int   I2D_iNr = 0;                        // nr of active 2D-buttons
static int   I2D_typTab[I2D_TABSITZ];            // type of obj (VC, PLN ..)
static char  I2D_txtTab[I2D_TABSITZ][32];        // button-text
static char  I2D_stat[I2D_TABSITZ];              // '0'=active, 'X'=disabled
static float I2D_posTab[I2D_TABSITZ];      // X-value startPos of tag in modelCoords
static float I2D_endTab[I2D_TABSITZ];      // X-value endPos of tag in modelCoords
 


//================================================================
  int GLBT_sele__ (float posXpx) {
//================================================================
// GLBT_sele__      test if 2D-system-button selected
// test only x-coords;  Y-coord already tested
// Input:
//   mouseXpx   mousepos in screenCoords (pixel, SCS)
// Output:
//   retCode    >= 0 - selected button;   -1 = no button selected
 

  int    irc, ib;


  // printf("GLBT_sele__ %f\n",posXpx);

  // get modelcoords of screenCorrds
  float posXmcs = (float) GR_Xmcs_Xpx (posXpx);
    // printf(" GLBT_sele__-posXmcs=%f\n",posXmcs);


  // loop tru buttons
  for(ib=0; ib<I2D_iNr; ++ib) {
    if(posXmcs < I2D_posTab[ib]) continue;
    if(posXmcs < I2D_endTab[ib]) {
      // user activated this button
      irc = I2D_typTab[ib];
      goto L_exit;
    }
  }

  irc = -1;


  L_exit:
    printf(" ex-GLBT_sele__ %d\n",irc);

  return irc;

}


//================================================================
  int GLBT_but_ini () {
//================================================================

  I2D_iNr = 0;

  // M44F_init (mSY2f);  // init

  return 0;

}


//================================================================
  int GLBT_but_add  (int *ib, int iTyp, char *bTxt) {
//================================================================
// GLBT_but_add       add GL-2D-button
// Input:
//   ib      button-index to use, clearing all following buttons
//           first index is 0
//   iTyp    1 = button green
// Output:
//   ib      ib += 1;


  printf("GLBT_but_add ib=%d typ=%d |%s|\n",*ib,iTyp,bTxt);


  I2D_iNr = *ib;

  I2D_typTab[I2D_iNr] = iTyp;

  strcpy (I2D_txtTab[I2D_iNr], bTxt);

  I2D_stat[I2D_iNr] = '0';          // 0=active X=disabled

  I2D_iNr += 1;
  *ib = I2D_iNr;


  return 0;

}


//================================================================
  int GLBT_render () {
//================================================================


  int      i1, ib;
  float    offStr, offChr, sizT, offT, sclX, sclY, ptStart[3];
  Mat_4x4F m2D, mxt, mxs;

  // return if no 2D-obj is active
  if(I2D_iNr <= 0) return 0;

  printf("GLBT_render %d ------------------ \n",I2D_iNr);
  printf(" GR_sizX_mm_mcs=%lf GR_sizY_mm_mcs=%lf\n",GR_sizX_mm_mcs,GR_sizY_mm_mcs);

  // printf("  GR_scaleWinY=%f GR_SclFontX=%f GR_SclFontY=%f\n",
          // GR_scaleWinY, GR_SclFontX, GR_SclFontY);


  //----------------------------------------------------------------
  glDisable (GL_DEPTH_TEST);   // 2D only

  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_POLYGON_SMOOTH);
  glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



  glLineWidth ((float)1.8f);   // maximum is 10 !
  glPointSize ((float)1.8f);


  //----------------------------------------------------------------
  // activate shaderProg shSY2
  glUseProgram (shSY2.idPrg);

  // bind VAO, VBO
  glBindVertexArray (shSY2.idVao);
  glBindBuffer (GL_ARRAY_BUFFER, shSY2.idVbo);

  glEnableVertexAttribArray(0);
  // (index-vertex-attrib, nr-components, format, do-normalize, stride, offset)
  // points, lines, triangles: 3 floats
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);


  //----------------------------------------------------------------
  // draw tags for buttons

  // provide the positionOffest in uniform
  glUniform3f (shSY2.ulOff, 0.f, 0.f, 0.f);

  // color yellow
  glUniform3f (shSY2.ulAtt, 0.8f, 0.9f, 0.0f);

  // set low-left (-1,-1 in MCS)
  ptStart[0] = -1.f; 
  ptStart[1] = -1.f;

  // get size of 1 char in model-coords (-1, to +1)
  sclX = GR_sizX_mm_mcs * GR_vf1SizXmm * GR_vf1SizScl;
  sclY = GR_sizY_mm_mcs * GR_vf1SizYmm * 1.5f * GR_vf1SizScl;
    printf(" _render-tag-sclX=%f sclY=%f\n",sclX,sclY);

  GR_ButtYmcs = -1. + sclY;

  // X-size of half char
  float chr2X = sclX / 2.f;


  for(ib=0; ib<I2D_iNr; ++ib) {
//     if(I2D_stat[ib] != '0') continue;

    // set model-view-matrix; goto startpoint MCS bottom-left
    M44FC_transl__ ((float*)mxt, ptStart[0], ptStart[1], 0.f);
      // M44FC_dump ((float*)mxt, "GLBT_render-mSY2f-1");

    // width of a char is 1; set to requested size GR_vf1Siz in millimetres
    sizT = sclX * strlen(I2D_txtTab[ib]); // * GR_vf1SizScl;
    M44FC_scale__ ((float*)mxs, sizT+chr2X, -sclY, 1.f);
      // M44FC_dump ((float*)mxs, "GLBT_render-mSY2f-mxs");

    // multiply ..
    M44FC_m44_mult_m44 ((float*)m2D, (float*)mxt, (float*)mxs);
      // M44FC_dump ((float*)m2D, "GLBT_render-mSY2f-2");

    // load tranformation (startpos, x-size, y-size)
    glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)m2D);

    // dispa tag
    glDrawArrays (GL_TRIANGLE_FAN, symTab[iSym_tag].iOff / 12, 4);
    // GL_tag_2D (0.05f, 0.01f, GR_OffFontX);

    // store startPos of tag in I2D_posTab (modelCoords)
    I2D_posTab[ib] = ptStart[0];

    // store endpos of tag in I2D_endTab (modelCoords)
    I2D_endTab[ib] = ptStart[0] + sizT + chr2X;

    // move startpoint
    ptStart[0] += sizT + sclX;
  }


  //----------------------------------------------------------------
  // draw text for buttons

  // set low-left (-1,-1 in MCS) + 1 millimeter up right
  ptStart[0] = -1.f + (1.f * GR_sizX_mm_mcs);  // 0.95f;
  ptStart[1] = -1.f + (1.f * GR_sizY_mm_mcs);  // -0.95f;

  // set model-view-matrix; goto startpoint MCS bottom-left
  M44FC_transl__ ((float*)mxt, ptStart[0], ptStart[1], 0.f);
    // M44FC_dump ((float*)mxt, "GLBT_render-mSY2f-1");

  // get size of 1 char in model-coords (-1, to +1)
  sclX = GR_sizX_mm_mcs * GR_vf1SizXmm * GR_vf1SizScl;
  sclY = GR_sizY_mm_mcs * GR_vf1SizYmm * GR_vf1SizScl;

  // hight and width of a char is 1; set to requested size GR_vf1Siz in millimetres
  M44FC_scale__ ((float*)mxs, sclX, sclY, 1.f);
    // M44FC_dump ((float*)mxs, "GLBT_render-mSY2f-mxs");

  // multiply ..
  M44FC_m44_mult_m44 ((float*)m2D, (float*)mxt, (float*)mxs);
    // M44FC_dump ((float*)m2D, "GLBT_render-mSY2f-2");

  glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)m2D);

  // color black
  glUniform3f (shSY2.ulAtt, 0.0f, 0.0f, 0.0f);

  // char-offset = 1 charwidth
  offChr = sclX; // * GR_vf1SizScl;
  offStr = 0.f;

  // loop tru buttons
  for(ib=0; ib<I2D_iNr; ++ib) {
//     if(I2D_stat[ib] != '0') continue;
// 
//     if(I2D_typTab[ib] != 1) {
//       TX_Print("***** GLBT_render TODO typ=%d",I2D_typTab[ib]);
//       continue;
//     }

    // disp string 
    GL_txt_2D (&offStr, I2D_txtTab[ib], offChr);
    // offStr += offXtot;
    offStr += offChr;
  }


  //----------------------------------------------------------------
  // close - disactivate
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glBindVertexArray (0);  // clear active buffer
  glUseProgram (0);     // disactivate active program




  //----------------------------------------------------------------
  // reset 3D
  glEnable (GL_DEPTH_TEST);   // restore 3D
  glDisable(GL_BLEND);


    printf(" ex-GLBT_render\n");

  return 0;

}
 

//================================================================
  int GLBT_dump () {
//================================================================
 
  int    i1;

  printf("GLBT_dump ::::::::::::::::::::::::\n");

  for(i1=0; i1<I2D_iNr; ++i1) {
    printf("  %2d typ=%2d stat=%c pos=%6.3f end=%6.3f |%s|\n",i1,
           I2D_typTab[i1], I2D_stat[i1],
           I2D_posTab[i1], I2D_endTab[i1], I2D_txtTab[i1]);
  }

  return 0;

}


// EOF
