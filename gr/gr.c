/* ../gr/gr.c
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

GR_dyn_sym             add dynamic symbol

GR_view__              (re)set view top | front | side | iso
GR_sele__              analyze selection
GR_get_eyeVecs         compute vectors GR_eyeZ GR_eyeX GR_eyeY
GR_redraw

GR_init0
GR_resize              resize window
GR_Modsiz              set new modelsize
GR_do_pan              update rotation-centerpoint from mousemove (pan)
GR_do_rot              update rotation-angles, viewplane ..
GR_view_cen_set        set new rotation-centerpoint

GR_get_curPos__        get mousePosition on constructionPlane in UCS
GR_Xndc_Xpx            get X-coord in modelCoordinates from pixels
GR_Yndc_Xpx            get Y-coord in modelCoordinates from pixels

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

#include "../ut/geo.h"                     // Point Plane Mat_4x4D ..
#include "../ut/matrix.h"
#include "../ut/utx.h"                     // UTX_add_fl_f
#include "../ut/deb.h"                     // DEB_dump_pt
#include "../gl/gl_font1.h"                // GR_vf1SizXmm
#include "../gl/gl_shCV.h"                 // chrTab
#include "../gl/gl_shSY2.h"                // GL_ISYM_CHR
#include "../gl/glbt.h"                    // GLBT_sele__
#include "../db/db.h"                      // DB_..
#include "../dl/dl.h"                      // DL_init__
#include "../dl/dl_sym_dyn.h"              // DL_SYD_init__
#include "../dl/dl_sym_perm.h"             // DL_SYP_init__
#include "../gui/gui.h"                     // GUI_CB_sele

#define extern  // does eliminate "extern"
#include "../gr/gr.h"                    // FUNC_ViewTop ..
#undef extern

#include "../gr/gr_gtx.h"                    // GR_gxt_subst1
#include "../gr/col.h"                     // Att__width
#include "../gl/gl.h"
#include "../app/app.h"               // AP_*
#include "../ut/os.h"                      // OS_system


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
  double GR_Xndc_Xpx (float X_px) {
//================================================================
// GR_Xndc_Xpx             get X-coord in deviceCoordinates from pixels
// returns x-coord between +1 and -1 (NDC, see INF_CoordinateSystems)


  double Xwin2 = (double)GR_sizWinX_px / 2.;

  return (((double)X_px - Xwin2) / Xwin2);

}


//================================================================
  double GR_Yndc_Ypx (float Y_px) {
//================================================================
// GR_Yndc_Ypx             get Y-coord in deviceCoordinates from pixels
// returns y-coord between +1 and -1 (NDC, see INF_CoordinateSystems)


  double Ywin2 = (double)GR_sizWinY_px / 2.;

  return -(((double)Y_px - Ywin2) / Ywin2);

}


//================================================================
//================================================================
 

//================================================================
  int GR_view__ (int typ) {
//================================================================
// GR_view__                  (re)set view top | front | side | iso

  int   irc,
        i_get_eyeVecs = 0;     // 1 = compute GR_eyeZ GR_eyeX GR_eyeY; 0 = not.

  printf("GR_view__ %d\n",typ);


  if(typ == 0) {
    // get type of view from user
    typ = GR_view_get ();
    if(typ <= 0) return irc;
  }


  //----------------------------------------------------------------
  if(typ == FUNC_ViewTop) {
    // set top-view
    GR_angZ = UT_RADIANS(0.);
    GR_angX = UT_RADIANS(0.);
    GR_eyeX = UT3D_VECTOR_X;
    GR_eyeY = UT3D_VECTOR_Y;
    GR_eyeZ = UT3D_VECTOR_Z;
    goto L_OK;


  } else if(typ == FUNC_ViewFront) {
    // set front-view
    GR_angZ = UT_RADIANS(-90.);
    GR_angX = UT_RADIANS(-90.);
    GR_eyeX = UT3D_VECTOR_Y;
    GR_eyeY = UT3D_VECTOR_Z;
    GR_eyeZ = UT3D_VECTOR_X;
    goto L_OK;


  } else if(typ == FUNC_ViewSide) {
    // set side-view
    GR_angZ = UT_RADIANS(0.);
    GR_angX = UT_RADIANS(-90.);
    GR_eyeX = UT3D_VECTOR_X;
    GR_eyeY = UT3D_VECTOR_Z;
    GR_eyeZ = UT3D_VECTOR_IY;
    goto L_OK;


  } else if(typ == FUNC_ViewIso) {
    // set iso-view
    GR_angZ = UT_RADIANS(-45.);
    GR_angX = UT_RADIANS(-60.);
    i_get_eyeVecs = 1;    // compute vectors GR_eyeZ GR_eyeX GR_eyeY
    goto L_OK;


  } else if(typ == FUNC_ViewSet) {
    // store active view
    view1.az = GR_angZ;
    view1.ax = GR_angX;
    view1.scl = GR_Scale;
    view1.pcv = GR_cenView;
    view1.pcr = GR_cenRot;
    goto L_done;


  } else if(typ == FUNC_ViewGet) {
    // reset stored view
    GR_angZ = view1.az;
    GR_angX = view1.ax;
    GR_Scale = view1.scl;
    GR_cenView = view1.pcv;
    GR_cenRot = view1.pcr;
    i_get_eyeVecs = 1;    // compute vectors GR_eyeZ GR_eyeX GR_eyeY
    goto L_done;


  } else {
    printf("***** ERROR GR_view__ typ=%d\n",typ);
    return -1;
  }


  L_OK:
    GR_actView = typ;
    GL_scale_def ();       // reset scale
    GR_cenView = UT3D_PT_NUL;   // reset screenCenter


  L_done:
    GL_view_ortho ();
    GL_view_update ();
    // compute vectors GR_eyeZ GR_eyeX GR_eyeY
    if(i_get_eyeVecs) GR_get_eyeVecs ();
    GR_redraw ();


  L_exit:
    return 0;

}


//================================================================
  int GR_do_pan (double dx, double dy) {
//================================================================
// GR_do_pan              update viewpointCenter from mousemove (pan)


  Point    pt1, pt2, ptx;
  float    dz;


  printf("GR_do_pan %f %f view=%d\n",dx,dy,GR_actView);


  //----------------------------------------------------------------
  if(GR_actView == FUNC_ViewFront) {

    GR_cenView.y += dx / GR_Scale;
    GR_cenView.z -= dy / GR_Scale;
    goto L_upd1;


  //----------------------------------------------------------------
  } else if(GR_actView == FUNC_ViewSide) {

    GR_cenView.x += dx / GR_Scale;
    GR_cenView.z -= dy / GR_Scale;            // up-down; OK
    goto L_upd1;


  //----------------------------------------------------------------
  } else if(GR_actView == FUNC_ViewTop) {
  
    GR_cenView.x += dx / GR_Scale;
    GR_cenView.y += dy / GR_Scale;
    goto L_upd1;


  //----------------------------------------------------------------
  } else {       // FUNC_ViewIso

    // get pt1 = old mouse-position in UCS
    GR_get_curPos__ (&ptx, &pt1, GR_mousePosX, GR_mousePosY);

    // get pt2 = active mouse-position in UCS
    GR_get_curPos__ (&ptx, &pt2, GR_mousePosX + dx, GR_mousePosY - dy);

    // get difference
    dx = pt2.x - pt1.x;
    dy = pt2.y - pt1.y;
    dz = pt2.z - pt1.z;
      
    // update view-center
    GR_cenView.x += dx;
    GR_cenView.y += dy;
    GR_cenView.z += dz;

  }


  //----------------------------------------------------------------
  L_upd1:

    // update
    GL_view_update  ();

    return 0;

}


//================================================================
  int GR_do_rot (double dx, double dy) {
//================================================================
// GR_do_rot              update rotation-angles, viewplane ..


  printf("GR_do_rot %f %f\n",dx,dy);

  // rotate around Z
  // angZ = angle to rotate around Z
  GR_angZ += dx / 100.;
  GR_angZ = UTP_angr_set_0_2pi (GR_angZ); // set angle between 0 to 2Pi

  // rotate around Y
  // angX = angle to tilt
  GR_angX -= dy / 100.;
  GR_angX = UTP_angr_set_0_2pi (GR_angX); // set angle between 0 to 2Pi

  // update
  GL_view_update ();

  // compute vectors GR_eyeZ GR_eyeX GR_eyeY
  GR_get_eyeVecs ();


  // check if View is top, front, side or isoview. Set GR_actView.
  // top
  if(UT3D_comp2vc_d(&GR_eyeZ, (Vector*)&UT3D_VECTOR_Z, UT_TOL_min1) == 1) {
    GR_actView = FUNC_ViewTop;
    // AP_view_upd (GR_actView);

  // front
  } else if(UT3D_comp2vc_d(&GR_eyeY, (Vector*)&UT3D_VECTOR_Y, UT_TOL_min1) == 1) {
    GR_actView = FUNC_ViewFront;
    // AP_view_upd (GR_actView);

  // side
  } else if(UT3D_comp2vc_d(&GR_eyeX, (Vector*)&UT3D_VECTOR_X, UT_TOL_min1) == 1) {
    GR_actView = FUNC_ViewSide;
    // AP_view_upd (GR_actView);

  } else {
    GR_actView = FUNC_ViewIso;
    // AP_view_upd (GR_actView);
  }

  return 0;

}


//================================================================
  int GR_get_curPos__ (Point *pt_ind, Point *pt_cps,
                       float mouseX, float mouseY) {
//================================================================
// GR_get_curPos__         get mousePosition on constructionPlane in UCS
// INPUT:
//   mouseX       mouseCoords in pixels
//   mouseY       
// OUTPUT:
//   pt_ind       indicated point in space; UCS
//   pt_cps       mouseposition on constructionPlane; UCS
//   retCode      0 = eyeVec parallel to constructionPlane; 1 = intersection OK

  int        irc;
  double     d1;
  Point      pt1;
  Mat_4x4D   midv;


  printf("GR_get_curPos__ \n");
  printf("  GR_mousePosX. = %f %f\n",GR_mousePosX,GR_mousePosY);


  //----------------------------------------------------------------
  // get pt1 = mousePos in space - device-coords from pixels (screen-coords)
  //   NDC from SCS - see INF_CoordinateSystems
  pt1.x = GR_Xndc_Xpx (mouseX);
  pt1.y = GR_Yndc_Ypx (mouseY);
  pt1.z = 0.;
    DEB_dump_pt (&pt1, " get_curPos__-pt1");


  //----------------------------------------------------------------
  // get pt_ind = mousePos in space - world-coords from device-coords pt1
  //   WCS from NDC - see INF_CoordinateSystems
  M44D_inv__ (midv, GR_matd_view); // get midv = inverse GR_matd_view
  M44D_pt_mult_m44 (pt_ind, midv, &pt1);

    // TESTBLOCK
//     DEB_dump_pt (pt_ind, " get_curPos__-pt_ind");
//     // disp symbol at pt_ind = mousepos in space in UCS
//     Att_ln att = {0, 3, 9, 0, 0, 0};
//     Att__width (&att, 2.f);
//     GR_dyn_sym (SYM_PLN, pt_ind, &att, 3.f);
    // END TESTBLOCK


  //----------------------------------------------------------------
  // get pt_cps = intersect eyeVec GR_eyeZ at mousePos with GR_constrPln
  irc = UT3D_ptDi_intptvcpln (pt_cps, &d1, &GR_constrPln, pt_ind, &GR_eyeZ);
  if(!irc) {
    // GL_eyeZ - GR_constrPln parallel; ??
    *pt_cps = *pt_ind;
  }

    // TESTBLOCK
//     DEB_dump_pt (pt_cps, " get_curPos__-pt_cps");
//     // disp point at mousePos on constrPlan
//     GR_dyn_sym (SYM_STAR, pt_cps, &att, 3.f);
    // END TESTBLOCK



  return irc;

}


//================================================================
  int GR_get_eyeVecs () {
//================================================================
// GR_get_eyeVecs            compute vectors GR_eyeZ GR_eyeX GR_eyeY


  Mat_4x4D   midv;

  // get midv = inverse GR_matd_view
  M44D_inv__ (midv, GR_matd_mdl[7]);

  // get the vectors out of the matrix
  GR_eyeX.dx = midv[0][0];
  GR_eyeX.dy = midv[1][0];
  GR_eyeX.dz = midv[2][0];

  GR_eyeY.dx = midv[0][1];
  GR_eyeY.dy = midv[1][1];
  GR_eyeY.dz = midv[2][1];

  GR_eyeZ.dx = midv[0][2];
  GR_eyeZ.dy = midv[1][2];
  GR_eyeZ.dz = midv[2][2];

//     DEB_dump_vc (&GR_eyeZ, " _eyeVecs-GR_eyeZ");

  return 0;

}


//================================================================
  int GR_view_get () {
//================================================================
 
  int   irc;
  char  s1[80];

  // get type of view from to set from user
  irc = GUI_dlg_list1 (s1, sizeof(s1),
                       "views.txt",
                       "select view",
                       "x30,y10");
    printf(" view_get %d |%s|\n",irc,s1);



  if(irc <= 0) return irc;

  irc = -1;
  if(!strcmp(s1, "top-View"))                 irc = FUNC_ViewTop;
  if(!strcmp(s1, "front-View"))               irc = FUNC_ViewFront;
  if(!strcmp(s1, "side-View"))                irc = FUNC_ViewSide;
  if(!strcmp(s1, "iso-View"))                 irc = FUNC_ViewIso;
  if(!strcmp(s1, "store-active-view"))        irc = FUNC_ViewSet;
  if(!strcmp(s1, "restore-last-stored-view")) irc = FUNC_ViewGet;

    printf(" ex-GR_view_get %d |%s|\n",irc,s1);

  return irc;

}


//================================================================
  int GR_sele__ () {
//================================================================
// GR_sele__               analyze selection
 
  int    irc;
  Point  pt_ind, pt_cps;


  // get and store select-position
  GR_selPosX = GR_mousePosX;
  GR_selPosY = GR_mousePosY;


  printf("GR_sele__ SSSSSSSSSSSSSSS\n");
  printf(" _sele__-pos %f %f\n",GR_selPosX,GR_selPosY);


  //================================================================
  // test if 2D-system-button selected

  // get GR_mousePosY in modelCoordinates
  double mousePosYmcs = GR_Yndc_Ypx (GR_selPosY);
    // printf(" sele__-mousePosYmcs=%f ButtYmcs=%f\n",mousePosYmcs,GR_ButtY_ndc);
  if(mousePosYmcs <= GR_ButtY_ndc) {
    irc = GLBT_sele__ (GR_selPosX);
    if(irc >= 0) {
      // 2D-system-button selected
      irc = GUI_CB_sele (irc);
      if(irc) { GR_render_skip = 1; goto L_return; }
      goto L_exit;
    }
  }



  //================================================================
  // get mousePosition in space and on constructionPlane in UCS
  irc = GR_get_curPos__ (&pt_ind, &pt_cps, GR_selPosX, GR_selPosY);

    // TESTBLOCK
    // disp symbol at pt_ind = mousepos in space in UCS
    Att_ln att = {0, 3, 9, 0, 0, 0};
    Att__width (&att, 2.f);
    GR_dyn_sym (SYM_PLN, &pt_ind, &att, 3.f);
    // disp point at mousePos on constrPlan
    GR_dyn_sym (SYM_STAR, &pt_cps, &att, 3.f);
    // END TESTBLOCK
 





  //================================================================
// TODO: obj-selection ...
//    GR_test__ ();



  //================================================================
  L_exit:
    GR_redraw ();   // only for testDisplayObjs ?
    printf(" ex-GR_sele__\n");


  //================================================================
  L_return:
  return 0;

}


//================================================================
  int GR_init0 () {
//================================================================
 

  printf("GR_init0 \n");

  DL_init__ ();     // init DisplayList permanent objects
  DL_SYP_init__ ();  // init Displaylist permanent-symbol objects
  DL_SYD_init__ ();  // init Displaylist dynamic-symbol objects


  GR_Modsiz = 1.;

  GR_actView = FUNC_ViewTop;

  GR_cenRot = UT3D_PT_NUL;
  GR_cenView = UT3D_PT_NUL;

  GR_view_vx = UT3D_VECTOR_X;
  GR_view_vz = UT3D_VECTOR_Z;

  // initial size of grafic-window
  GR_sizWinX_px = 800;
  GR_sizWinY_px = 800;

  GR_vf1SizXmm =  2.6f;  // fontsize-X in mm
  GR_vf1SizYmm =  3.0f;  // fontsize-Y in mm

  // GR_scaleWinY = (double )GR_sizWinX_px / (double )GR_sizWinY_px;

  GR_angZ = 0.;
  GR_angX = 0;

  GR_constrPln = PLANE_NUL;

  GR_render_skip = 0;

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

  // size of 1 millimeter in device-coords
  GR_sizX_mm_ndc = 2. / sizWinXmm;
  GR_sizY_mm_ndc = 2. / sizWinYmm;
    printf(" resize_mm_mcs=%f GR_sizY_mm_ndc=%f\n",GR_sizX_mm_ndc,GR_sizY_mm_ndc);


  // // siz.px = size pixel in modelspace (modelspace = -1. to +1)
  // GR_sizX_px_mcs = GR_sizX_mm_px / (double)GR_sizWinX_px;
  // GR_sizY_px_mcs = GR_sizY_mm_px / (double)GR_sizWinY_px;
    // printf(" GR_resize-px_mcs %f %f\n",GR_sizX_px_mcs,GR_sizY_px_mcs);

//----------------------------------------------------------------
// // ortho:
//   M44D_scale_ortho (GR_matd_mdl[0]);
//     M44D_dump (GR_matd_mdl[0], "_resize-GR_matd_mdl[0]");


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
  // set view-matrix GR_matf_dev and model-matrix mfm
  GL_view_ortho ();
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
  int GR_dyn_sym (int typ, void *data, void *att, float size) {
//================================================================
// GR_dyn_sym2            add dynamic 2D-symbol
// INPUT:
//   typ        2D-symbol, eg SYM_TRIA
//   pos        position, world-coords
//   att        
//   size       in mm

 
  int iOff;

  printf("GR_dyn_sym2 typ=%d\n",typ);
    
  // iOff = GL_dyn_sym2 (typ, pos, att, size);

  // store dataBaseIndex, bufferOffset (in bytes), objTyp in dynamic-dispList
  DL_SYD_add__ (typ, data, size, att);

  return 0;

}   


// //================================================================
//   int GR_dyn_sym3 (int typ, void *data, void *att, float size) {
// //================================================================
// // GR_dyn_sym2            add dynamic 2D-symbol
// // INPUT:
// //   typ        3D-symbol, eg Typ_PVC
// //   data       data-struct for typ
// //   att        
// //   size       in mm
// 
// 
//   int iOff;
// 
//   printf("GR_dyn_sym3 typ=%d\n",typ);
// 
//   iOff = GL_dyn_sym3 (typ, data, att, size);
// 
//   // store dataBaseIndex, bufferOffset (in bytes), objTyp in dynamic-dispList
//   DL_SYD_add__ (typ, data, size, att, 0, 0);
// 
//   return 0;
// 
// }


//================================================================
  int GR_perm_pt (double *ptPos, Att_ln iatt, int objID, int dbi) {
//================================================================
// GR_perm_pt             add permanent point (Typ_PT)
// Input:
//   ptPos      x,y,z coords
//   iatt       color, width
 
  int iOff;

  printf("GR_perm_pt dbi=%d\n",dbi);

  iOff = GL_add_pt (ptPos, iatt, dbi);

  // store dataBaseIndex, bufferOffset (in bytes), objTyp in dispList
  DL_add__ (Typ_PT, dbi, objID, (int*)&iatt, iOff, 1);

  return 0;

}


//===========================================================================
  int GR_perm_cv (double *cvDat, int ptNr, Att_ln iatt, int objID, int dbi) {
//===========================================================================
// GR_perm_cv             add permanent curve (Typ_CV)
// Input:
//   cvDat     <ptNr> points from 3 doubles with x,y,z coords
//   ptNr      nr of points (GL_TRIANGLES: 3 points = one triangle)

  int iOff;

  printf("GR_perm_cv dbi=%d\n",dbi);

  iOff = GL_add_cv (cvDat, ptNr, iatt, dbi);

  // store dataBaseIndex, bufferOffset (in bytes), objTyp in dispList
  DL_add__ (Typ_CV, dbi, objID, (int*)&iatt, iOff, ptNr);

  return 0;

}


//===============================================================================
  int GR_perm_sur (double *surDat, int ptNr, int typ, ColRGB color,
                   int objID, int dbi) {
//===============================================================================
// GR_perm_sur                     add permanent surface to grafic-system
// typ = Typ_GL_Sur
// Input:
//   surDat    <ptNr> points from 3 doubles with x,y,z coords
//   ptNr      nr of points (GL_TRIANGLES: 3 points = one triangle)
//   typ       GL_TRIANGLES|GL_TRIANGLE_FAN|GL_TRIANGLE_STRIP
//   ptPos     x,y,z coords
//   color     color ..


  int iOff;

  printf("GR_perm_sur typ=%d ptNr=%d dbi=%d\n",typ,ptNr,dbi);

  // store surface in grafic-controller; get offset in grafic-controller as ID
  iOff = GL_add_sur (surDat, ptNr, typ, color, dbi);

  // store dataBaseIndex, bufferOffset (in bytes), objTyp in dispList
  DL_add__ (typ, dbi, objID, (int*)&color, iOff, ptNr);

  return 0;

}


//=================================================================================
 int GR_perm_Note (Dimen *dim1, int oNr, int typ, void *iatt, int objID, int dbi) {
//=================================================================================

  int     i1, i2, bMod;
  double  a1, d1, dVal;
  Line2   ll1, ll2, lnd;
  SymRef2 hd1, hd2;
  Vector2 vct;
  char    outText[256];


  printf("GR_perm_Note typ=%d objID=%d dbi=%d\n",typ,objID,dbi);

  // analyze dim1 - get lines
/*
  if(dim1->dtyp == 3)      GL_set_Dima   (att, dim1);
  else if(dim1->dtyp > 20) GL_set_Ldr    (att, dim1);    // 21
  else if(dim1->dtyp > 1)  GL_set_Dimrad (att, dim1);    // 2
  else if(dim1->dtyp > 0)  GL_set_Dimdia (att, dim1);    // 1
  else                     GL_set_Dimen  (att, dim1);    // 0
*/


  //----------------------------------------------------------------
  // (dtyp = 0) linear-dimension
  bMod = GR_dim_lin__ (&ll1, &ll2, &lnd, &hd1, &hd2, &vct, &dVal, outText,
                       dim1);

    DEB_dump_obj__ (Typ_LN2, &ll1, " perm_Note-ll1");
    DEB_dump_obj__ (Typ_LN2, &ll2, " perm_Note-ll2");
    DEB_dump_obj__ (Typ_LN2, &lnd, " perm_Note-lnd");

  DL_SYP_add__ (Typ_LN2, &ll1, 0.f, NULL);
  DL_SYP_add__ (Typ_LN2, &ll2, 0.f, NULL);
  DL_SYP_add__ (Typ_LN2, &lnd, 0.f, NULL);


  return 0;

}


//=================================================================================
 int GR_perm_GTxt (GText *tx1, int oNr, int typ, void *iatt, int objID, int dbi) {
//=================================================================================
// GR_perm_GTxt            create grafic-(3D-)text

  int   i1, ic;
  char  *sp;

  printf("GR_perm_GTxt |%s| siz=%f dir=%f\n",tx1->txt, tx1->size, tx1->dir);


  // loop tru chars
  sp = tx1->txt;
  i1 = 0;
  while(sp[i1]) {
      printf(" GTxt %d = %c\n",i1,sp[i1]);
    ic = GL_ISYM_CHR(sp[i1]); // get index for next char
    // glDrawArrays (chrTab[ic].lTyp, chrTab[ic].iOff / 12, chrTab[ic].oNr);   
    // DL_SYP_add__ (Typ_LN2, &ll1, 0.f, NULL);


    ++i1;
  }


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


//=======================================================================
  int GR_dim_lin__ (Line2 *ll1, Line2 *ll2, Line2 *lnd,
                    SymRef2 *hd1, SymRef2 *hd2, Vector2 *vct,
                    double *dval, char *outText,
                    Dimen *dim1) {
//=======================================================================
// create objects for linear dimension.
// components of dimension:
// Output:
//   2 leaderlines (ll1, ll2);
//   1 dimensionLine (lnd);
//   2 symbols at the ends of the dimensionLine (hd1, hd2);
//   direction of symbols and text (vct);
//   value of the dimemsion (distanceValue; dval).
//   the decoded text (outText)
//   RetCod:  BlockColor; 0=keine, 1-9 ist Farbe und wird als Block dargestellt.

// see GL_DrawDimen SVG_w_dim__

  int        i12, i13, i23, bMod;
  double     txAng, ldAng, d12, dtx;
  char       dimText[256];
  Point2     *pt1, *pt2, *pt3;
  Vector2    vcl;


  // printf("GR_dim_lin__ \n");

  pt1 = (Point2*)&dim1->p1;
  pt2 = (Point2*)&dim1->p2;
  pt3 = (Point2*)&dim1->p3;


  txAng = dim1->a1;               // Winkel der Textzeile
  ldAng = txAng - 90.;            // Winkel der Leaderlines
    // printf(" txAng=%f ldAng=%f\n",txAng,ldAng);

  // Richtung Leaderline:     vcl
  UT2D_vc_angr (&vcl, UT_RADIANS(ldAng));

  // Richtung Text u. Arrows: vct
  UT2D_vc_angr (vct, UT_RADIANS(txAng));

  // Leaderlines.Den pt3 auf pt1/pt2-vcl projizieren.
  UT2D_pt_projptptvc (&hd1->pt, pt3, pt1, &vcl);
  UT2D_pt_projptptvc (&hd2->pt, pt3, pt2, &vcl);


  // compute distanceValue
  *dval = UT2D_len_2pt (&hd1->pt, &hd2->pt);
    // printf(" dval = %f\n",*dval);


  // create/decode text; value=dVal
  GR_gxt_prep1 (dimText, *dval);
    // printf(" tx1: |%s|\n",dimText);

  // substitute [% by dimText
  bMod = GR_gxt_subst1 (outText, dim1->txt, dimText);
    // printf(" tx2: |%s|\n",outText);



  // Leaderlines um Texthoehe verlaengern.
  // wenn pt3a oben ist: Abstand umdrehen !
  ll1->p1 = UT2D_pt_pt3 (pt1);
  if(UT2D_sidPerp_ptvc (&hd1->pt, pt1, &vcl)> 0) {
    d12 = GR_tx_ldExt;
  } else {
    d12 = -GR_tx_ldExt;
  }
  UT2D_pt_traptvclen (&ll1->p2, &hd1->pt, &vcl, d12);



  // ebenso fuer ll2:
  ll2->p1 = UT2D_pt_pt3 (pt2);
  if(UT2D_sidPerp_ptvc (&hd2->pt, pt2, &vcl)> 0) {
    d12 = GR_tx_ldExt;
  } else {
    d12 = -GR_tx_ldExt;
  }
  UT2D_pt_traptvclen (&ll2->p2, &hd2->pt, &vcl, d12);



  // get Headtypes
  hd2->typ = dim1->hd % 10;
  // *sTyp1 = (dim1->hd - *sTyp2) / 10;
  hd1->typ = dim1->hd  / 10;
    // printf("Headtypes = %d,%d\n", hd1->typ, hd2->typ);



  // test if text is outside of the leaderlines;
  // if yes: if Headtypes not given: fix Headtypes
  //    the dimensionLine starts at p3; extend with half blocksize
  i13 = UT2D_sidPerp_ptvc (&hd1->pt, pt3, vct); // s1 rechts od links von pt3
  i23 = UT2D_sidPerp_ptvc (&hd2->pt, pt3, vct); // s2 rechts od links von pt3
    // printf(" i13 %d i23 %d\n",i13,i23);



  if((i13 + i23) == 0) {  // different: text is inside !
    lnd->p1 = hd1->pt;
    lnd->p2 = hd2->pt;
    // test if ll2 is right of ll1; if not: change symboltypes !
    // d12 = UT2D_len_ptlnX(&lnd->p1, &lnd->p2, vct);
    if(i23 < 0) {
      MEM_swap_int (&hd1->typ, &hd2->typ);
      // printf(" SWAP SYMBOLTYPES !\n");
    }
    goto L_9;
  }


  // Text ist aussen: p3 um halbe Blockbreite nach aussen schieben
  lnd->p1 = hd1->pt;
  lnd->p2 = hd2->pt;
  i12 = UT2D_sidPerp_ptvc (&hd1->pt, &hd2->pt, vct); // s1 rechts od links von s2
    // printf(" dimText outside %d %d\n",i13,i23);
  d12 = GR_gtx_ckBlockWidth (outText, AP_txdimsiz) / 2.;
    // printf(" d12=%f\n",d12);


  if(i12 < 0) {        //   1 2
    MEM_swap_int (&hd1->typ, &hd2->typ);
    if(i13 > 0) {      // 3 1 2
      UT2D_pt_traptvclen (&lnd->p1, pt3, vct, -d12);
    } else {           //   1 2 3
      UT2D_pt_traptvclen (&lnd->p2, pt3, vct, d12);
    }

  } else {             //   2 1
    if(i13 > 0) {      // 3 2 1
      UT2D_pt_traptvclen (&lnd->p2, pt3, vct, -d12);
    } else {           //   2 1 3
      UT2D_pt_traptvclen (&lnd->p1, pt3, vct, d12);
    }


  }


  L_9:
  return bMod;

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


//================================================================
  int GR_test__ () {
//================================================================

  int        irc;
  Point      pt1, pt2, pt3;
  double     d1; // x_ndc, y_ndc;
  Mat_4x4D   midv;
  Att_ln     att = {0, 3, 9, 0, 0, 0};

static int  iix = 0;

  printf("GR_test__ ::::::::::::::::::::::::\n");
  printf("  GR_angZ=%f angX=%f Scale=%f\n",GR_angZ,GR_angX,GR_Scale);
  printf("  GR_cenRot  = %f %f %f\n",GR_cenRot.x,GR_cenRot.y,GR_cenRot.z);
  printf("  GR_cenView = %f %f %f\n",GR_cenView.x,GR_cenView.y,GR_cenView.z);
  printf("  GR_mousePosX. = %f %f\n",GR_mousePosX,GR_mousePosY);








  //----------------------------------------------------------------
  GR_redraw ();

  return 0;

}



// EOF
