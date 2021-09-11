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

do_help           display helpfile
do_func           do testfunctions
do_defaults_read  read default-values
GUI_CB_sele       callback from obj-selection
GUI_CB_mouse      callback mouse-button pressed - released
GUI_CB_key        callback keyboard-key pressed - released
GUI_CB_win_is_up  callback from GUI_init - all windows are up.

main

List_functions_end:
=====================================================


//----------------------------------------------------------------
*/

#ifdef globTag
 void APP(){}
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>


#include "./ut_types.h"                // UINT_32

#define extern  // does eliminate "extern" in includefile
#include "./geo.h"                     // Point Plane Mat_4x4D ..
#undef extern

#include "./gui.h"
#include "./gr.h"
#include "./ut.h"                      // TX_Print
#include "./dl.h"                      // DL_..
#include "./glbt.h"                    // GLBT_..
#include "./tst_prim1.h"               // Typ_sur_n
#include "./col.h"
#include "./os.h"                      // OS_system


int dbi = 1L;     // index database


ColRGB actCol;    // active defaultColor  (surfaces)
Att_ln cvAtt;     // active defaultLineTyp
Att_ln ptAtt;     // active defaultPointTyp


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

float col1[] = { 0.4f, 0.4f, 0.4f };            // R G B

float lnWidth = 2.f;     // width of curves
float ptWidth = 4.f;     // width of points

int iBt = 0;      // nr of defined buttons on bottom of screen



//================================================================
  int do_help () {
//================================================================
 
  printf(" do_help\n");

  OS_system ("/usr/bin/xdg-open using.txt");


  return 0;

}


//================================================================
  int do_func (int typ) {
//================================================================

  int   i1, i2, i3;

  printf("do_func %d\n",typ);


  switch (typ) {

    case Typ_Fnc0:
      // Help
      do_help ();
      break;

    case Typ_Fnc1:
      // isoView
      GR_view__ (FUNC_ViewIso);
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
      GR_perm_pt (ptDat1, ptAtt, dbi);
      ++dbi;
      GR_redraw ();
      break;

    case Typ_Fnc4:
      // add Curve
      for(i1=0; i1<12; i1 += 3) { 
        lnDat1[i1]   += 0.25;     // add X
        lnDat1[i1+2] += 0.1;      // add Z
      }
      GR_perm_cv (lnDat1, 4, cvAtt, dbi);
      ++dbi;
      GR_redraw ();
      break;

    case Typ_Fnc5:
      // add Surface
      for(i1=0; i1<9; i1 += 3) {
        surDat1[i1]   += 0.25;     // add X
        surDat1[i1+2] += 0.1;      // add Z
      }
      GR_perm_sur (surDat1, 3, Typ_GL_Trias, actCol, dbi);
      ++dbi;
      GR_redraw ();
      break;

    default:
      printf("***** do_func error %d\n",typ);

      break;
  }


  return 0;

}


//================================================================
  int do_defaults_read () {
//================================================================
// do_defaults_read  read default-values


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
  int GUI_CB_sele (int typ) {
//================================================================
// GUI_CB_sele               callback from obj-selection


  printf("GUI_CB_sele %d\n",typ);

  do_func (typ);

  return 0;

}


//================================================================
  int GUI_CB_mouse (int iEv, int iMb) {
//================================================================
// GUI_CB_mouse            callback mouse-button pressed
//   iEv   TYP_EventPress|TYP_EventRelease
//   iMb   GUI_MouseL|      left mousebutton
//         GUI_MouseM|      mid mousebutton
//         GUI_MouseR|      right mousebutton
//         GUI_MouseScUp|   mouse scroll up
//         GUI_MouseScDown  mouse scroll down

  printf("GUI_CB_mouse %d %d\n",iEv,iMb);

  if(iEv == TYP_EventPress) {
    if(iMb == GUI_MouseL) {
      // left mouseButt pressed - analyze and process selection
      GR_sele__ ();
    }
  }

  return 0;

}


//================================================================
  int GUI_CB_key (int iEv, int iKey) {
//================================================================
// Input:
//   iEv      TYP_EventPress|TYP_EventRelease
// Output:
//   retcode  0 = do defaultOperations of keyboard-interface
//            1 = do NOT defaultOperations of keyboard-interface

  int  i1, i2, i3, dli, typ;

  float color[] = {
    0.1f, 1.0f, 0.1f, 1.0f               // R G B alpha
  };


  printf("GUI_CB_key %d %c\n",iEv,iKey);

  if(iEv != TYP_EventPress) goto L_exit;


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
    do_func (Typ_Fnc5);
    goto L_exit;
  }


  //----------------------------------------------------------------
  // add point
  if(iKey == 'p') {
    do_func (Typ_Fnc3);
    goto L_exit;
  }

  // add curve
  if(iKey == 'l') {
    do_func (Typ_Fnc4);
    goto L_exit;
  }


  //----------------------------------------------------------------
//   // symbols:
//   // alpha-text
//   if(iKey == 'a') {
//     GR_perm_txtA (fPos, "Abc", cvAtt, dbi);
//     ++dbi;
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
    do_func (Typ_Fnc2);
    goto L_exit;
  }


  // point/curveWidth
  if(iKey == 'w') { 
    Att__width (&cvAtt, cvAtt.thick + 2.f);
    Att__width (&ptAtt, ptAtt.thick + 2.f);
    goto L_exit;
  }


  // hide/restore obj last-1
  if(iKey == 'h') {
     // here dispListIndex is ident with dbi ..
     // hide or reDisplay obj
     dli = dbi - 3;
     if(dli >= 0) {
       DL_hide__ (dli, -1);
     }
     goto L_draw;
  }


  // Esc = exit
  if(iKey == GUI_KeyEsc) exit (0);
  goto L_exit;



  //----------------------------------------------------------------
  L_draw:
    GR_redraw ();



  L_exit:
    printf(" ex-GUI_CB_key\n");

  return 1;

}

 
//================================================================
  int GUI_CB_win_is_up () {
//================================================================
// MAIN_CB_win_is_up       callback from GUI_init - all windows are up.

  int    i1;


  printf("GUI_CB_win_is_up \n");
  printf(" \n");

  OS_system ("cat using.txt"); // display help-text on console


  //----------------------------------------------------------------
  // init modelsize (viewspace -2.5 - +2.5)
  GR_modsiz_set (5.0);


  // ColRGB from 3 floats
  Col__3f (&actCol, col1);
//   // set defCol
//   GR_col_def (actCol);

  // set default-linetyp
  Att__width (&cvAtt, lnWidth);
  Att__width (&ptAtt, ptWidth);


  //----------------------------------------------------------------
  // create point
  do_func (Typ_Fnc3);


  // create 2. point
  ptDat1[0] += 0.25;
  do_func (Typ_Fnc3);


  // create polygon
  do_func (Typ_Fnc4);


  // create a triangle; add it into DispList and GL
  GR_perm_sur (surDat1, 3, Typ_GL_Trias, actCol, dbi);
  ++dbi;


  // create one more triangle, 
  do_func (Typ_Fnc5);


  // create 2D-system-buttons
  GLBT_but_add  (&iBt, Typ_Fnc0, "Help");
  GLBT_but_add  (&iBt, Typ_Fnc1, "isoView");
  GLBT_but_add  (&iBt, Typ_Fnc2, "change-Color");
  GLBT_but_add  (&iBt, Typ_Fnc3, "Point");
  GLBT_but_add  (&iBt, Typ_Fnc4, "Curve");
  GLBT_but_add  (&iBt, Typ_Fnc5, "Surface");


  //----------------------------------------------------------------
  // redraw all active objects
  GR_redraw ();

 
  return 0;

}


//================================================================
  int main (int argc, char** argv) {
//================================================================
//

  printf("\n\n========================= gcad-GL V0.0.1 \n");


  do_defaults_read ();

  DL_init__ ();
  GR_init0 ();

  // open (GTK-)Window, if window is up continue with app_CB_init
  GUI_init (argc, argv);

  return 0;

}


// EOF
