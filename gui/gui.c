/* ./gui.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================


GUI    user interface independent from drivers (Gtk, OpenGL)




=====================================================
List_functions_start:

GUI_init
GUI_redraw
GUI_Color_select
GUI_get_fontSiz
GUI_ck_version

GUI_CB_sele       callback from obj-selection
GUI_CB_mouse      callback mouse-button pressed - released
GUI_CB_key        callback keyboard-key pressed - released
GUI_CB_win_is_up  callback from GUI_init - all windows are up.
GUI_dlg_list1     GUI_dlg_list1        get list-selection from user

GUI_redraw     
GUI_block__       // block/unblock keys, mouse, set/reset cursor ..

List_functions_end:
=====================================================


//----------------------------------------------------------------
*/

#ifdef globTag
 void GUI(){}
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>


#include "../ut/geo.h"                     // Point
#include "../gui/gui.h"
#include "../gr/gr.h"
#include "../ut/utx.h"                     // UTX_CleanCR
#include "../dl/dl.h"                      // DL_..
#include "../gl/glbt.h"                    // GLBT_..
#include "../gr/col.h"
#include "../ut/os.h"                      // OS_system

#include "../app/app.h"               // APP


static char *exe_GUI_dlg1 = {"./GUI_dlg1_gtk3"};



//================================================================
  int GUI_dlg_info1 (char *info) {
//================================================================
// GUI_dlg_info1       display text in new dialog-window
// TODO: separate at '\n' to separate string ... or make variadic ..
//       - needs one string per line ( "exe_GUI_dlg1 info "line1" line2"")

  int     irc;
  char    cmd[40], s2[512];

  // get cmd
  strcpy (cmd, exe_GUI_dlg1);
  // if(!cmd) return -1;

  sprintf(s2,"%s info \" %s \"", cmd, info);
    printf(" GUI_dlg_info1 |%s|\n",s2);


  OS_system (s2);

  printf(" ex-GUI_dlg_info1\n");

  return 0;

}


//===============================================================================
  int GUI_dlg_list1 (char *sOut, int sSiz, char *fn, char *info, char *options) {
//===============================================================================
// GUI_dlg_list1        get list-selection from user
// Input:
//   sSiz     size of sOut in chars
//   fn       file with lines to select
//   info     infotext
//   options  "x<width>,y<lnNr>"
// Output:
//   sOut     text of selected line of file fn
//   retCode  >0 = nr of chars;
//            0  = cancel;
//            <0 = error

  int     irc;
  char    cmd[40], s2[512];



  // get cmd
  strcpy (cmd, exe_GUI_dlg1);
  // if(!cmd) return -1;

  sprintf(s2,"%s list1 %s \'%s\' \'%s\'",
             cmd,      fn, info, options);
    // printf(" dlg_list1 |%s|\n",s2);


  irc = OS_sys1 (sOut, sSiz, s2);
  // irc >0 = nr of chars; 0=cancel; <0=error
  if(irc > 0) {
    UTX_CleanCR (sOut);
    irc = strlen(sOut);
  }

  printf(" ex-GUI_dlg_list1 %d |%s|\n",irc,sOut);

  return irc;

}


//================================================================
  int GUI_CB_sele (int typ) {
//================================================================
// GUI_CB_sele               callback from obj-selection
// retCode    0=redraw; 1=do-not-redraw


  printf("GUI_CB_sele %d\n",typ);

  return AP_do (typ);

}


//================================================================
  int GUI_CB_mouse (int iEv, int iMb) {
//================================================================
// GUI_CB_mouse            callback mouse-button pressed/released
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
// GUI_CB_key            callback key pressed/released
// Input:
//   iEv      TYP_EventPress|TYP_EventRelease
// Output:
//   retcode  0 = do defaultOperations of keyboard-interface
//            1 = do NOT defaultOperations of keyboard-interface

  int  irc;


  printf("GUI_CB_key %d %c\n",iEv,iKey);

  if(iEv != TYP_EventPress) goto L_exit;


  // Esc = exit
  if(iKey == GUI_KeyEsc) exit (0);

  // report to app
  irc = AP_CB_key (iKey);


  //----------------------------------------------------------------
  L_exit:
    printf(" ex-GUI_CB_key %d\n",irc);

  return irc;

}

 
//================================================================
  int GUI_CB_win_is_up () {
//================================================================
// MAIN_CB_win_is_up       callback from AP_gui_init - all windows are up.

  int    i1;


  printf("GUI_CB_win_is_up \n");


  // report to app
  AP_CB_win_is_up ();

  // redraw all active objects
  GR_redraw ();

 
  return 0;

}


//================================================================
  int GUI_block__ (int iFunctions, int iInput, int iCursor) {
//================================================================
// GUI_block__       // block/unblock keys, mouse, set/reset cursor ..
// Input:
//   iFunctions    0=reset;  1=blockFunctions;   -1=do not modify
//   iInput        0=reset;  1=blockInput;       -1=do not modify
//   iCursor       0=reset;  1-5 cursorNr;       -1=do not modify
//


static int UI_block_cur = 0;
static int UI_block_inp = 0;


  // printf("UI_block__ mode=%d actMode=%d\n",mode,actMode);



  //----------------------------------------------------------------
  L_cur:
  if((iCursor < 0) || (UI_block_cur == iCursor)) goto L_func;

  GUI_cursor__ (iCursor); // change cursor; 1=wait, ..
  UI_block_cur = iCursor;


  //----------------------------------------------------------------
  L_func:
//   // 0 = reset to normal
//   if((iFunctions < 0) ||
//      (UI_block_func == iFunctions)) goto L_inp;
// ... - app - specific


  //----------------------------------------------------------------
  L_inp:
  if((iInput < 0) || (UI_block_inp == iInput)) goto L_exit;

  GUI_block_gr (iInput); // block/unblock grafic-input from mouse for grafic-window
  UI_block_inp = iInput;


  //----------------------------------------------------------------
  L_exit:
  return 0;

}


// EOF
