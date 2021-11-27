/* ./tst_prim.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================


testprog for (Gtk3 + Opengl (gtk_gl_glarea))

cad-GL  
- is a hi-level library connecting CAD-applications with OpenGL
- written in C, under Gpl3 license.
See README.md


//----------------------------------------------------------------
Build:
Prerequisites:
gcc with tools (ctags)

On debian-OS:
sudo apt install libgtk-3-dev mesa-common-dev

On redhat-OS:
sudo yum install gtk3-devel mesa-libGL-devel

make && ./a.out


For using see Help-file using.txt and console-output



=====================================================
List_functions_start:

main

AP_help           display helpfile
AP_do             do testfunctions
AP_defaults_read  read default-values
AP_CB_key         user pressed key
AP_CB_win_is_up   callback from GUI_init - all windows are up.

AP_test_cv1       test vector-at-position; TODO-buggy

List_functions_end:
=====================================================

structure:
main
  AP_defaults_read    // load startupParameters
  DB_init
  GR_init0
  GUI_init            // returns with AP_CB_win_is_up

AP_CB_win_is_up
  .......             // load unused startupParameters (eg load model)

GUI_CB_mouse          // callback mouse-button pressed/released
GUI_CB_key            // callback key pressed/released



//----------------------------------------------------------------
// TODO  block gtk-inputs until app is completely up



//----------------------------------------------------------------
*/

#ifdef globTag
 void AP(){}
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>


#define extern  // does eliminate "extern" in includefile
#include "../ut/geo.h"                     // Point Plane Mat_4x4D ..
#undef extern

#include "../gui/gui.h"
#include "../gr/gr.h"
#include "../ut/ut.h"                      // TX_Print
#include "../db/db.h"                      // DB_..
#include "../dl/dl.h"                      // DL_nr
#include "../dl/dl_sym_perm.h"             // DL_SYP_dump
#include "../dl/dl_sym_dyn.h"              // DL_SYD_dump
#include "../gl/glbt.h"                    // GLBT_..
#include "../gr/col.h"
#include "../ut/os.h"                      // OS_system

#define extern  // does eliminate "extern"
#include "../app/app.h"               // APP
#undef extern


//----------------------------------------------------------------

double ptDat1[] = {   // startpos point
  -0.80,  0.50,  -0.5
};


double lnDat1[] = {         // data curve GL_LINE_STRIP
  -0.50,  0.50,  0.,
  -0.65,  0.50,  0.,
  -0.80,  0.65,  0.,
  -0.80,  0.90,  0.
};


double surDat1[] = {       // data triangle
  0.,     0.,    0.,
  0.5,    0.,    0.,
  0.,     0.25,  0.
};

float col1[] = { 0.1f, 0.1f, 0.6f };            // R G B

float lnWidth = 1.f;     // width of curves
float ptWidth = 3.f;     // width of points

int iBt = 0;      // nr of defined buttons on bottom of screen





//================================================================
  int AP_help () {
//================================================================
// AP_help   display help-file with editor
 
  printf(" AP_help\n");

  OS_system ("/usr/bin/xdg-open using.txt");


  return 0;

}


//================================================================
  int AP_do (int typ) {
//================================================================
// retCode    0=redraw; 1=do-not-redraw

  int   i1, i2, i3;

  printf("AP_do %d\n",typ);


  switch (typ) {

    case Typ_Fnc0:
      // Help
      AP_help ();
      break;

    case Typ_Fnc1:
      // change view
      GR_view__ (0);
      break;

    case Typ_Fnc2:
      // change-Color
      // get new color from user
      GUI_Color_select (&i1, &i2, &i3);
      // ColRGB from 3 ints
      Col__3i (&actCol, i1, i2, i3);
      // set color also for curves
      Att__3i (&cvAtt, i1, i2, i3);
      Att__3i (&ptAtt, i1, i2, i3);
      break;

    case Typ_Fnc3:
      // add Point
      ptDat1[0]   += 0.2;     // add X
      ptDat1[1]   += 0.1;     // add Y
      ptDat1[2]   += 0.05;    // add Z
      // GR_perm_pt (ptDat1, ptAtt, DB_get_free(Typ_PT));
      AP_obj_add_perm (Typ_PT, ptDat1, 1, &ptAtt, DB_get_free(Typ_PT));
      GR_redraw ();
      break;

    case Typ_Fnc4:
      // add Curve
      for(i1=0; i1<12; i1 += 3) { 
        lnDat1[i1]   += 0.25;     // add X
        lnDat1[i1+2] += 0.1;      // add Z
      }
      // GR_perm_cv (lnDat1, 4, cvAtt, DB_get_free(Typ_CV));
      AP_obj_add_perm (Typ_CV, lnDat1, 4, &cvAtt, DB_get_free(Typ_CV));
      GR_redraw ();
      break;

    case Typ_Fnc5:
      // add Surface
      for(i1=0; i1<9; i1 += 3) {
        surDat1[i1]   += 0.25;     // add X
        surDat1[i1+2] += 0.1;      // add Z
      }
      AP_obj_add_perm (Typ_GL_Trias, surDat1, 3, &actCol, DB_get_free(Typ_SUR));
      GR_redraw ();
      break;

    case Typ_Fnc6:
      // Dump:
      AP_do_dump ();
      return 1; // do not redraw



    default:
      printf("***** AP_do error %d\n",typ);

      break;
  }


  return 0;

}


//================================================================
  int AP_do_dump () {
//================================================================
  int   irc;
  char  s1[80];

  // get type of view from to set from user
  irc = GUI_dlg_list1 (s1, sizeof(s1),
                       "dumps.txt",
                       "select func",
                       "x30,y10");
    printf(" view_get %d |%s|\n",irc,s1);



  if(irc <= 0) return irc;

  irc = -1;
  if     (!strcmp(s1, "DL_SYP_dump"))   DL_SYP_dump ();
  else if(!strcmp(s1, "DL_SYD_dump"))   DL_SYD_dump ();
  else if(!strcmp(s1, "DL_dump"))       DL_dump ();
  else if(!strcmp(s1, "GR_dump"))       GR_dump ();
  else if(!strcmp(s1, "GLBT_dump"))     GLBT_dump ();
  else TX_Error ("***** AP_do_dump ");


  return 0;

}


//==============================================================================
  int AP_obj_add_perm (int typ, void *data, int oNr, void *iatt, int dbi) {
//==============================================================================
// AP_obj_add_perm           add obj permanent into DB, DL and display
// INPUT:
//   typ       type (of data) eg Typ_PT Typ_CV Typ_GL_Trias
//   data      data=double[3 * nr-points]
//   oNr       nr of points in data;
//   iatt      graf.attribute; Att_ln for point,curve; ColRGB for surface
//
// Typ_PT point    oNr=nr-points data=double[]    // oNr points
// Typ_CV polygon; oNr=ptNr      data=double[]    // oNr points for polygon
// Typ_GL_Trias;   oNr=ptNr      data=double[]    // 3 pts per triangle


  int objId;

  // store obj in DB and get the (sequential) obj-ID
  objId = DB_add__ (typ, data, oNr, dbi);

  switch (typ) {

    case Typ_PT:
// TODO oNr > 1
      GR_perm_pt (data, *(Att_ln*)iatt, objId, dbi);
      break;

    case Typ_CV:
      GR_perm_cv (data, oNr, *(Att_ln*)iatt, objId, dbi);
      break;

    case Typ_GL_Trias:
      GR_perm_sur (data, oNr, typ, *(ColRGB*)iatt, objId, dbi);
      break;

    case Typ_GTXT:
      GR_perm_GTxt (data, oNr, typ, iatt, objId, dbi);
      break;

//     case Typ_ATXT:
//       GR_perm_ATxt (data, oNr, typ, *(Att_ln*)iatt, objId, dbi);
//       break;

    case Typ_Dimen:
      GR_perm_Note (data, oNr, typ, iatt, objId, dbi);
      break;


    default:
      TX_Error ("******* ERROR AP_obj_add_perm typ %d",typ);
      return -1;
  }
 

  return 0;

}


//==============================================================================
  int AP_obj_add_dyn (int typ, void *data, int ptNr, void *iatt, float size) {
//==============================================================================
// AP_obj_add_dyn           add dynamic obj into DB, DL and display


//   // store obj in DB and get the (sequential) obj-ID
//   objId = DB_add__ (typ, data, ptNr, dbi);


    // SYM_TRIA, SYM_STAR, SYM_PLN, Typ_PVC
    // TODO ptNr > 1
      GR_dyn_sym (typ, data, iatt, size);


  return 0;

}


//================================================================
  int AP_CB_key (int iKey) {
//================================================================
// AP_CB_key       user pressed key
// Input:
//   iKey     keycode pressed
// Output:
//   retcode  0 = do defaultOperations of keyboard-interface
//            1 = do NOT defaultOperations of keyboard-interface

  int  i1, i2, i3, dli, typ;

  float color[] = {
    0.1f, 1.0f, 0.1f, 1.0f               // R G B alpha
  };


  printf("AP_CB_key %d %c\n",iKey,iKey);


  //----------------------------------------------------------------
  // grafic:
  // 1,2,3 - change rotationCenter. TODO: set from select vertex)
  if(iKey == '1') { GR_view_cen_set (-0.5, 0., 0.); goto L_draw;}
  if(iKey == '2') { GR_view_cen_set (0., -0.25, 0.); goto L_draw;}
  if(iKey == '0') { GR_view_cen_set (0., 0., 0.); goto L_draw;}


  //----------------------------------------------------------------
  // surfaces:
  // 't' - create one more triangle, 
  if(iKey == 't') {
    AP_do (Typ_Fnc5);
    goto L_exit;
  }


  //----------------------------------------------------------------
  // add point
  if(iKey == 'p') {
    AP_do (Typ_Fnc3);
    goto L_exit;
  }

  // add curve
  if(iKey == 'l') {
    AP_do (Typ_Fnc4);
    goto L_exit;
  }


  //----------------------------------------------------------------
//   // symbols:
//   // alpha-text
//   if(iKey == 'a') {
//     GR_perm_txtA (fPos, "Abc", cvAtt, dbi);
//   }


  //----------------------------------------------------------------
  // attributes:
  // dump DL ..
  if(iKey == 'd') {
    GR_dump ();
    DL_dump ();
    GLBT_dump ();
    goto L_exit;
  }

  // get new defaultColor from user (as 3 ints)
  if(iKey == 'c') {
    AP_do (Typ_Fnc2);
    goto L_exit;
  }


  // point/curveWidth
  if(iKey == 'w') {
    Att__width (&cvAtt, (float)cvAtt.thick + 2.f);
    Att__width (&ptAtt, (float)ptAtt.thick + 2.f);
    goto L_exit;
  }


  // hide/restore obj last-1
  if(iKey == 'h') {
     // here dispListIndex is ident with dbi ..
     // hide or reDisplay obj
     dli = DL_nr - 3;
     if(dli >= 0) {
       DL_hide__ (dli, -1);
     }
     goto L_draw;
  }


  //----------------------------------------------------------------
  L_draw:
    GR_redraw ();



  L_exit:
    printf(" ex-AP_CB_key\n");

  return 1;

}


//================================================================
  int AP_CB_win_is_up () {
//================================================================
// AP_CB_win_is_up       callback from GUI_init - all windows are up.
// TODO: load unused startupParameters (eg load model)

  int    i1;
  Point  pt1, pt2;
  VecPos pvc1;

  printf("AP_CB_win_is_up \n");
  printf(" \n");



  //----------------------------------------------------------------
  OS_system ("cat using.txt"); // display help-text on console


  // init modelsize (viewspace -2.5 - +2.5)
  GR_modsiz_set (5.0);



  //----------------------------------------------------------------
  // PERM-CURVES

  // ColRGB from 3 floats
  Col__3f (&actCol, col1);

  // set default-linetyp
  Att__width (&cvAtt, lnWidth);
  Att__width (&ptAtt, ptWidth);

  // create point
  AP_do (Typ_Fnc3);

  // create 2. point
  ptDat1[0] += 0.25;
  AP_do (Typ_Fnc3);

  // create polygon
  AP_do (Typ_Fnc4);


  //----------------------------------------------------------------
  // PERM-SURFACES

  // create a triangle; add it into DispList and GL
  AP_obj_add_perm  (Typ_GL_Trias, surDat1, 3, &actCol, DB_get_free(Typ_SUR));

  // create one more triangle, 
  AP_do (Typ_Fnc5);


  //----------------------------------------------------------------
  // PERM-SYMBOLS

  // create dimension
  UT3D_pt_3db (&pt1, -1., -1., 0.);
  AP_test_Dimen (&pt1);
  
  // create 3D-text
  UT3D_pt_3db (&pt1, -1., -1.5, 0.);
  AP_test_GTxt (&pt1);



  
  //----------------------------------------------------------------
  // DYM-SYMBOLS

  // point
  UT3D_pt_3db (&pt1, -1., -0.5, 0.);
  AP_obj_add_dyn (SYM_PT, &pt1, 1, &cvAtt,  6.f);

  // create a dynamic small triangle; size 3 mm
  pt1.x += 0.2;
  Att__width (&cvAtt, 2.f);
  AP_obj_add_dyn (SYM_TRIA, &pt1, 1, &cvAtt, 3.f);

  // create a small star
  pt1.x += 0.2;
  AP_obj_add_dyn (SYM_STAR, &pt1, 1, &cvAtt, 3.f);

  // create a small rectangle
  pt1.x += 0.2;
  Att__3i (&cvAtt, 0, 200, 200);  // color
  AP_obj_add_dyn (SYM_PLN, &pt1, 1, &cvAtt, 3.f);

  // create pvc1 = dynamic 3D-vector at position pt1 true-length;
  pt1.x += 0.75;
  pvc1.pos = pt1;
  AP_test_cv1 (&pvc1);

  // create pvc1 = dynamic 3D-vector at position pt1 normalized-length;
  pt1.x += 0.75;
  pvc1.pos = pt1;
  AP_test_cv2 (&pvc1);
  




  //----------------------------------------------------------------
  // create 2D-system-buttons
  GLBT_but_add  (&iBt, Typ_Fnc0, "Help");
  GLBT_but_add  (&iBt, Typ_Fnc1, "View");
  GLBT_but_add  (&iBt, Typ_Fnc2, "change-Color");
  GLBT_but_add  (&iBt, Typ_Fnc3, "Point");
  GLBT_but_add  (&iBt, Typ_Fnc4, "Curve");
  GLBT_but_add  (&iBt, Typ_Fnc5, "Surface");
  GLBT_but_add  (&iBt, Typ_Fnc6, "Dump");


  return 0;

}


//================================================================
  int AP_defaults_read () {
//================================================================
// AP_defaults_read  read default-values


  char    *p1, txbuf[1024];
  FILE    *fp1 = NULL;


  sprintf(txbuf,"defaults.txt");  //,OS_get_cfg_dir());
    printf("AP_defaults_read |%s|\n",txbuf);


  if((fp1=fopen(txbuf,"r")) == NULL) {
    printf("****** OPEN ERROR FILE %s **********\n",txbuf);
    TX_Print("****** OPEN ERROR FILE %s **********",txbuf);
    return -1; 
  }
  
  
  // Line 1: factor size for textcharacters - if OS_get_scrRes() - if
  // xdpyinfo | awk '/dimensions/' does not provide correct values
  fgets (txbuf, 120, fp1);
  sscanf(txbuf, "%f",&GR_vf1SizScl);
    printf(" GR_vf1SizScl=|%f|\n",GR_vf1SizScl);




  return 0;

}


//================================================================
  int main (int argc, char** argv) {
//================================================================
//

  printf("\n\n========================= gcad-GL V0.0.1 \n");


  AP_defaults_read ();

  DB_init ();    // init DB
  GR_init0 ();   // init grafic

  // open (GTK-)Window, if window is up GUI_CB_win_is_up () comes back
  GUI_init (argc, argv);

  return 0;

}

//================================================================
  int AP_test_cv1 (VecPos *pvc1) {
//================================================================
// AP_test_cv1       test vector-at-position true-length;
  
  float scale = 0.f;

  Att__width (&cvAtt, 1.f);
  Att__3i (&cvAtt, 0, 0, 0);   // color black


  // scale 0 = true length, else length in mm
  UT3D_vc_3db (&pvc1->vc, 0.25, 0.0, 0.15);
  AP_obj_add_dyn (Typ_PVC, pvc1, 1, &cvAtt,  scale);
  
  UT3D_vc_3db (&pvc1->vc, 0.25, 0.25, 0.15);
  AP_obj_add_dyn (Typ_PVC, pvc1, 1, &cvAtt,  scale);
  
  UT3D_vc_3db (&pvc1->vc, -0.25, 0.25, 0.15);
  AP_obj_add_dyn (Typ_PVC, pvc1, 1, &cvAtt,  scale);
  
  UT3D_vc_3db (&pvc1->vc, -0.25, -0.25, 0.15);
  AP_obj_add_dyn (Typ_PVC, pvc1, 1, &cvAtt,  scale);

  return 0;

}


//================================================================
  int AP_test_cv2 (VecPos *pvc1) {
//================================================================
// AP_test_cv1       test vector-at-position normalized-length;

  float scale = 10.f;

  Att__width (&cvAtt, 1.f);
  Att__3i (&cvAtt, 0, 200, 0);   // color green


  // AP_obj_add_perm (Typ_PT, &pvc1->pos, 1, &ptAtt, DB_get_free(Typ_PT));
  // AP_obj_add_dyn (SYM_PT, &pvc1->pos, 1, &ptAtt, 10.f);


  // scale 0 = true length, else length in mm
  UT3D_vc_3db (&pvc1->vc, 0.5, 0.0, 0.25);
  AP_obj_add_dyn (Typ_PVC, pvc1, 1, &cvAtt,  scale);

  UT3D_vc_3db (&pvc1->vc, 0.25, 0.25, 0.15);
  AP_obj_add_dyn (Typ_PVC, pvc1, 1, &cvAtt,  scale);

  UT3D_vc_3db (&pvc1->vc, -0.25, 0.25, 0.15);
  AP_obj_add_dyn (Typ_PVC, pvc1, 1, &cvAtt,  scale);

  UT3D_vc_3db (&pvc1->vc, -0.25, -0.25, 0.15);
  AP_obj_add_dyn (Typ_PVC, pvc1, 1, &cvAtt,  scale);
 
  return 0;

}

//================================================================
  int AP_test_GTxt (Point *pt1) {
//================================================================
// AP_test_GTxt       test perm-3D-Text

  static char  *sTxt = "Test 123.45";

  GText tx1;


  tx1.pt = *pt1;
  tx1.size = 2.5f;
  tx1.dir  = 0.f;
  tx1.txt  = sTxt;

  AP_obj_add_perm  (Typ_GTXT, &tx1, 1, NULL, DB_get_free(Typ_Note));

  return 0;

}



//================================================================
  int AP_test_Dimen (Point *pt1) {
//================================================================
// AP_test_Dimen       test dimensions


  Dimen  dim1;


  dim1.p1.x = pt1->x;
  dim1.p1.y = pt1->y;

  dim1.p2.x = pt1->x + 0.2;
  dim1.p2.y = pt1->x - 0.1;

  dim1.p3.x = pt1->x + 0.1;   // textPos
  dim1.p3.y = pt1->x - 0.3;

  dim1.dtyp = 0;   // linear
  dim1.hd   = 12;  //  <-->
  dim1.a1   = 0.;  // linear:textline;
  dim1.a2   = 0.;  // UU

  AP_obj_add_perm  (Typ_Dimen, &dim1, 1, NULL, DB_get_free(Typ_Note));

  return 0;

}


// EOF
