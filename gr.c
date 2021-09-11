/* ./gr.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

grafic-functions independent from drivers (Gtk, OpenGL)

=====================================================
List_functions_start:

GR_perm_pt             add permanent point
GR_perm_cv             add permanent curve
GR_perm_sur            add permanent surface
GR_perm_txtA           add alpha-text

GR_view__              (re)set view top | front | side | iso
GR_sele__              analyze selection
GR_redraw

GR_init0
GR_resize              resize window
GR_Modsiz              set new modelsize
GR_view_cen_set        set new rotation-centerpoint

GR_Xmcs_Xpx            get X-coord in modelCoordinates from pixels
GR_Ymcs_Ypx            get Y-coord in modelCoordinates from pixels

GR_dump

List_functions_end:
=====================================================

*/

#ifdef globTag
 void GR(){}
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include "./geo.h"                     // Point Plane Mat_4x4D ..
#include "./matrix.h"
#include "./gui.h"                     // GUI_redraw
#include "./gl_font1.h"                // GR_vf1SizXmm
#include "./glbt.h"                    // GLBT_sele__
#include "./gui.h"                     // GUI_CB_sele

#define extern  // does eliminate "extern"
#include "./gr.h"                    // FUNC_ViewTop ..
#undef extern

#include "./gl.h"


//----------------------------------------------------------------
// active view - FUNC_ViewTop | FUNC_ViewFront | FUNC_ViewSide | FUNC_ViewIso
int   GR_actView;


Vector GR_view_vz;
Vector GR_view_vx;
// Plane GR_view_pln;
// GR_view_pln.vx goes parallel to horizontal windowBorder
// GR_view_pln.vy goes parallel to vertical windowBorder
// GR_view_pln.vz = eyeVector (goes from the screenCenter to the eye)


int  isu;   // ??


//================================================================
//================================================================
 


//================================================================
  double GR_Xmcs_Xpx (float X_px) {
//================================================================
// GR_Xmcs_Xpx             get X-coord in modelCoordinates from pixels
// returns x-coord between +1 and -1 (MCS, see INF_CoordinateSystems)


  double Xwin2 = (double)GR_sizWinX_px / 2.;

  return (((double)X_px - Xwin2) / Xwin2);

}


//================================================================
  double GR_Ymcs_Ypx (float Y_px) {
//================================================================
// GR_Ymcs_Ypx             get Y-coord in modelCoordinates from pixels
// returns y-coord between +1 and -1 (MCS, see INF_CoordinateSystems)


  double Ywin2 = (double)GR_sizWinY_px / 2.;

  return -(((double)Y_px - Ywin2) / Ywin2);

}


//================================================================
//================================================================
 

//================================================================
  int GR_view__ (int typ) {
//================================================================
// GR_view__                  (re)set view top | front | side | iso

  printf("GR_view__ %d\n",typ);


  if(typ == FUNC_ViewTop) {
    // reset scale
    GL_scale_def ();
    // set top-view
    GR_angZ = UT_RADIANS(0.);
    GR_angX = UT_RADIANS(0.);
    // reset screenCenter
    GR_cenView = PT_NUL;
    GL_view_update ();


  } else if(typ == FUNC_ViewIso) {
    // reset scale
    GL_scale_def ();
    // set iso-view
    GR_angZ = UT_RADIANS(45.);
    GR_angX = UT_RADIANS(-45.);
    // reset screenCenter
    GR_cenView = PT_NUL;
    GL_view_update ();


  } else {
    printf("***** ERROR GR_view__ typ=%d\n",typ);
    return -1;
  }

  GR_redraw ();

  return 0;

}


//================================================================
  int GR_sele__ () {
//================================================================
// GR_sele__               analyze selection
 
  int    irc;

  // get and store select-position
  GR_selPosX = GR_mousePosX;
  GR_selPosY = GR_mousePosY;


  printf("GR_sele__ SSSSSSSSSSSSSSS\n");
  printf(" _sele__-pos %f %f\n",GR_selPosX,GR_selPosY);


  //----------------------------------------------------------------
  // test if 2D-system-button selected

  // get GR_mousePosY in modelCoordinates
  double mousePosYmcs = GR_Ymcs_Ypx (GR_selPosY);
    // printf(" sele__-mousePosYmcs=%f ButtYmcs=%f\n",mousePosYmcs,GR_ButtYmcs);
  if(mousePosYmcs <= GR_ButtYmcs) {
    irc = GLBT_sele__ (GR_selPosX);
    if(irc >= 0) {
      // 2D-system-button selected
      GUI_CB_sele (irc);
      goto L_exit;
    }
  }



  //----------------------------------------------------------------
// TODO: obj-selection ...


  L_exit:
    printf(" ex-GR_sele__\n");
  return 0;

}


//================================================================
  int GR_init0 () {
//================================================================
 

  printf("GR_init0 \n");

  GR_Modsiz = 1.;

  GR_actView = FUNC_ViewTop;

  GR_cenRot = PT_NUL;
  GR_cenView = PT_NUL;

  GR_view_vx = VC_X;
  GR_view_vz = VC_Z;

  // initial size of grafic-window
  GR_sizWinX_px = 800;
  GR_sizWinY_px = 800;

  GR_vf1SizXmm =  2.6f;  // fontsize-X in mm
  GR_vf1SizYmm =  3.0f;  // fontsize-Y in mm

  // GR_scaleWinY = (double )GR_sizWinX_px / (double )GR_sizWinY_px;

  GR_angZ = 0.;
  GR_angX = 0;

  return 0;

}


//================================================================
  int GR_modsiz_set (double mSiz) {
//================================================================
// GR_Modsiz            set new modelsize
// set viewspace -(mSiz/2) - +(mSiz/2)

  GR_Modsiz = mSiz;

  GL_scale_def ();      // apply new modelsize

// TODO:
//   // set modelTolerances
//   GR_Tol ();

  return 0;

}


//================================================================
  int GR_resize () {
//================================================================
// GR_resize                 resize window

  double sizWinXmm, sizWinYmm;


  // float sizXpx, sizYpx;

  printf("GR_resize \n");

  // GR_sizX_mm_px = size pixel in mm
  GR_sizX_mm_px = (double)GR_sizScrX_px / (double)GR_sizScrX_mm;
  GR_sizY_mm_px = (double)GR_sizScrY_px / (double)GR_sizScrY_mm;
    printf(" resize-mm_px %f %f\n",GR_sizX_mm_px,GR_sizY_mm_px);


  // size of window in mm
  sizWinXmm = (double)GR_sizScrX_mm * ((double)GR_sizWinX_px/(double)GR_sizScrX_px);
  sizWinYmm = (double)GR_sizScrY_mm * ((double)GR_sizWinY_px/(double)GR_sizScrY_px);
    printf(" resize-sizWin-mm %f %f\n",sizWinXmm,sizWinYmm);

  // size of 1 millimeter in modelspace-coords
  GR_sizX_mm_mcs = 2. / sizWinXmm;
  GR_sizY_mm_mcs = 2. / sizWinYmm;
    printf(" resize_mm_mcs=%f GR_sizY_mm_mcs=%f\n",GR_sizX_mm_mcs,GR_sizY_mm_mcs);


  // // siz.px = size pixel in modelspace (modelspace = -1. to +1)
  // GR_sizX_px_mcs = GR_sizX_mm_px / (double)GR_sizWinX_px;
  // GR_sizY_px_mcs = GR_sizY_mm_px / (double)GR_sizWinY_px;
    // printf(" GR_resize-px_mcs %f %f\n",GR_sizX_px_mcs,GR_sizY_px_mcs);

//----------------------------------------------------------------
// // ortho:
//   M44D_scale_ortho (mva[0]);
//     M44D_dump (mva[0], "_resize-mva[0]");


//----------------------------------------------------------------
// perspectiv:
  // Create mxpv = view-matrix;
//   float aspect_ratio = (float)GR_sizWinX_px / (float)GR_sizWinY_px;
//     printf(" aspect_ratio=%f\n",aspect_ratio);
// 
//   // 0.7 = angle_of_view, 0.5 = z_near, 6 = z_far
//   mat_frustum (mxpf, 0.7, aspect_ratio, 0.0000001, 3);
// 
//   // Create frustum translation matrix:
//   mat_translate (mxpt, 0.f, 0.f, (float)GR_Scale);
// 
//   // Combine into view-matrix:   view * perspective
//   mat_multiply (mxpv, mxpf, mxpt);


//----------------------]------------------------------------------
  // set view-matrix mfv and model-matrix mfm
  GL_view_update ();


//----------------------------------------------------------------


    printf(" ex-GR_resizef\n");

  return 0;

}


//================================================================
  int GR_view_cen_set (double px, double py, double pz) {
//================================================================
// GR_view_cen_set        set new rotation-centerpoint

  printf("GR_view_cen_set %f %f %f\n",px, py, pz);

  GR_cenRot.x = px;
  GR_cenRot.y = py;
  GR_cenRot.z = pz;

  GL_view_update ();

  return 0;

}


//================================================================
  int GR_perm_pt (double *ptPos, Att_ln iatt, int dbi) {
//================================================================
// GR_perm_pt             add permanent point (Typ_PT)
// Input:
//   ptPos      x,y,z coords
//   iatt       color, width
 
  printf("GR_perm_pt dbi=%d\n",dbi);

  GL_add_pt (ptPos, iatt, dbi);

  return 0;

}


//================================================================
  int GR_perm_cv (double *cvDat, int ptNr, Att_ln iatt, int dbi) {
//================================================================
// GR_perm_cv             add permanent curve (Typ_CV)
// Input:
//   cvDat     <ptNr> points from 3 doubles with x,y,z coords
//   ptNr      nr of points (GL_TRIANGLES: 3 points = one triangle)

  printf("GR_perm_cv dbi=%d\n",dbi);

  GL_add_cv (cvDat, ptNr, iatt, dbi);

  return 0;

}


//===============================================================================
  int GR_perm_sur (double *surDat, int ptNr, int typ, ColRGB color, int dbi) {
//===============================================================================
// GR_perm_sur                     add permanent surface to grafic-system
// typ = Typ_GL_Sur
// Input:
//   surDat    <ptNr> points from 3 doubles with x,y,z coords
//   ptNr      nr of points (GL_TRIANGLES: 3 points = one triangle)
//   typ       GL_TRIANGLES|GL_TRIANGLE_FAN|GL_TRIANGLE_STRIP
//   ptPos     x,y,z coords
//   color     color ..


  printf("GR_perm_sur typ=%d ptNr=%d dbi=%d\n",typ,ptNr,dbi);

  GL_add_sur (surDat, ptNr, typ, color, dbi);

  return 0;

}


//================================================================
  int GR_perm_txtA (double fPos[], char *txt, Att_ln iatt, int dbi) {
//================================================================
// GR_perm_txtA                   alpha-text

  printf("GR_perm_txtA dbi=%d\n",dbi);
printf("************ TODO\n");


  
  return 0;

}


//================================================================
  int GR_redraw () {
//================================================================
 
  printf("GR_redraw -------------------------\n");

  GUI_redraw ();
  GL_render__ ();

  return 0;

}


//================================================================
  int GR_dump () {
//================================================================
 
  printf("GR_dump ::::::::::::::::::::::::\n");

  printf(" GR_cenView %f %f %f\n",GR_cenView.x,GR_cenView.y,GR_cenView.z);


  return 0;

}


// EOF
