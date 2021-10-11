/* ./gtk3.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Driver GUI - user-interface Gtk-V3

-----------------------------------------------------
TODO:
- press shift: display hand-cursor;
- press Strg:  display rotate-cursor;


-----------------------------------------------------
Modifications:
2012-09-01  extracted from first version from 2001-06-15. RF.

-----------------------------------------------------

=====================================================
List_functions_start:

GUI_redraw           start redraw window
GUI_Color_select     get new color from user
GUI_get_fontSiz      get fontsize of Gtk-font
GUI_ck_version       check GTK_MAJOR_VERSION,GTK_MINOR_VERSION
GUI_init             create complete window; return to app with MAIN_CB_win_is_up
GUI_cursor__         change cursor;
GUI_block_gr         block/unblock grafic-input from mouse for grafic-window

Gtk_Color_select     select-color-Dialog. Modal (wait).

Gtk_CB_key
Gtk_CB_motion        callback mouse-move in grafic-window
Gtk_CB_button        callback mouse-button pressed / released
Gtk_CB_scroll        callback scroll-wheel activated
Gtk_CB_resize        callback window resizing
Gtk_CB_realize       callback window established
Gtk_CB_render        callback render window
Gtk_CB_quit          callback exit-window activated


List_functions_end:
=====================================================

../gui_gtk3/gtk.h

*/

#ifdef globTag
 void Gtk3(){}
#endif

# define VER_GTK_MAJ 3
# define VER_GTK_MIN 0


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

// #include <GL/gl.h>
#include <gtk/gtk.h>


#include "../ut/ut_types.h"                // UINT_32
#include "../ut/geo.h"                     // Point Plane Mat_4x4D ..
#include "../gr/gr.h"                      // GR_sizWinX_px ..
#include "../gl/gl.h"
#include "../ut/matrix.h"
#include "../gui/gui.h"                     // GUI_CB_key
#include "../ut/os.h"                      // OS_checkFilExist
#include "../ut/ut.h"                      // TX_Write
#include "../app/app.h"               // Typ_* GUI_CB_key



//----------------------------------------------------------------
static GtkWidget *gl_area;

static int KeyStatShift = 0;                // 1=pressed, 0=not
static int KeyStatCtrl  = 0;
static int KeyStatAlt   = 0;

static int statM1=0, statM2=0, statM3=0;    // state mousebuttons
static int statMK=0;                        // state modifier-keys






//================================================================
//================================================================


 
//================================================================
  int Gtk_Color_select (int *ir, int *ig, int *ib) {
//================================================================
// Gtk_Color_select          select-color-Dialog. Modal (wait).
// Returns 3 colors as 16-bit-integers.
// Returncode   0=OK; -1=cancel.

  int         iRes;     // response
  GtkWidget   *wdlg;
  GtkWidget   *wSel;
  // GdkColor    rgb;      // 3 x short
  GdkRGBA      rgb;

  // printf("Gtk_Color_select\n");


  // wdlg = gtk_color_selection_dialog_new ("select color ..");
  wdlg = gtk_color_chooser_dialog_new ("select color ..", NULL);



  // stay on top (modal)
  // gtk_window_set_transient_for (GTK_WINDOW(wdlg), GTK_WINDOW(UI_MainWin));


  // start waiting; does not return until user clicks button.
  iRes = gtk_dialog_run (GTK_DIALOG (wdlg));    // wait (modal) !
    printf(" iRes=%d\n",iRes);
  // -6=_CANCEL, -5=_OK -4=

  if((iRes == GTK_RESPONSE_CANCEL)  ||
     (iRes == GTK_RESPONSE_DELETE_EVENT)) {
    gtk_widget_destroy (wdlg);
    return -1;
  }

  // wSel = gtk_color_selection_dialog_get_color_selection (
             // GTK_COLOR_SELECTION_DIALOG(wdlg));
  // gtk_color_selection_get_current_color (GTK_COLOR_SELECTION (wSel), &rgb);

  gtk_color_chooser_get_rgba (GTK_COLOR_CHOOSER (wdlg), &rgb);
    // printf(" sel.Col = %lf %lf %lf\n",rgb.red,rgb.green,rgb.blue);
  *ir = (int) (rgb.red * 65535.);
  *ig = (int) (rgb.green * 65535.);
  *ib = (int) (rgb.blue * 65535.);
  gtk_widget_destroy (wdlg);
    // printf(" sel.Col = %d %d %d\n",*ir,*ig,*ib);


  return 0;

}


//================================================================
static gboolean Gtk_CB_render (GtkGLArea *area, GdkGLContext *context) {
//================================================================
// Gtk_CB_render         redraw OpenGL-window

  long   gli = 1;


  printf("=================== Gtk_CB_render \n");

  if (gtk_gl_area_get_error (area) != NULL) return FALSE;

  // block keystrokes & grafic_selections; activate wait-cursor;
  GUI_block__ (1, 1, 1);

  GL_render__ ();

  // unblock keystrokes & grafic_selections; activate default-cursor;
  GUI_block__ (0, 0, 0);


  return TRUE;

}

//=======================================================================
static gboolean Gtk_CB_motion (GtkWidget *widget, GdkEventMotion *event) {
//=======================================================================
// Gtk_CB_motion                      callback mouse-move in OpenGL-window
 

  // GtkAllocation allocation;
  int     ikey;
  double  dx, dy;

  // gtk_widget_get_allocation(widget, &allocation);



  printf(" Gtk_CB_motion %f %f keyShift=%d\n",event->x,event->y,KeyStatShift);



  //----------------------------------------------------------------
  // update state modifier-keys
    // get state of shift,ctrl,alt-keys
  ikey = gdk_keymap_get_modifier_state (
           gdk_keymap_get_for_display (
             gdk_display_get_default()));


  // shift=1, ctrl=4
  if(ikey & 1)     KeyStatShift = 1;     else KeyStatShift = 0;
  if(ikey & 4)     KeyStatCtrl  = 1;     else KeyStatCtrl  = 0;
  if(ikey & 8)     KeyStatAlt   = 1;     else KeyStatAlt   = 0;


  // get relative movement of mouse from last stored position
  dx = event->x - GR_mousePosX;
  dy = GR_mousePosY - event->y;
    // printf(" CB_motion dx=%f dy=%f\n",dx,dy);


  //----------------------------------------------------------------
  if(KeyStatShift == 0) goto L_ck_rot;      // 1=pressed, 0=not

  // move model (pan)
  GR_do_pan (dx, dy);

  goto L_upd;


  //----------------------------------------------------------------
  L_ck_rot:
  if(KeyStatCtrl == 0) goto L_exit;

    // rotate model
    GR_do_rot (dx, dy);

    goto L_upd;
  

  //----------------------------------------------------------------
  // update view-matrix
  L_upd:
    // redraw
    gtk_gl_area_queue_render (GTK_GL_AREA (gl_area));


  //----------------------------------------------------------------
  L_exit:

    GR_mousePosX = (float)event->x;
    GR_mousePosY = (float)event->y;




    return FALSE;
}


//================================================================
static gboolean Gtk_CB_button (GtkWidget *widget, GdkEventButton *event,
                              void *data) {
//================================================================

//   GtkAllocation allocation;

  int    ityp, ikey, iEv, iMb;


  printf("Gtk_CB_button %d |%s|\n",event->button,(char*)data);

  ityp = ((GdkEventAny*)event)->type;
  iMb = ((GdkEventButton*)event)->button;
    // printf("  _gl_button %d\n",typ);

  if(ityp == GDK_BUTTON_PRESS) {
    iEv = TYP_EventPress;

  } else if(ityp == GDK_BUTTON_RELEASE) {
    iEv = TYP_EventRelease;

  } else if(ityp == GDK_SCROLL) {
      // printf(" scroll(%d) %d\n",GDK_SCROLL,((GdkEventScroll*)ev)->direction);
    iEv = TYP_EventPress;
    if(((GdkEventScroll*)event)->direction == GDK_SCROLL_UP)
      iMb = GUI_MouseScUp;
    else if(((GdkEventScroll*)event)->direction == GDK_SCROLL_DOWN)
      iMb = GUI_MouseScDown;
    else goto L_exit;

  } else return FALSE;  // TRUE=1   FALSE=0

    // printf("Gtk_CB_button typ=%d but=%d\n",typ,ikey);


//   // block mousemoves, keystrokes & grafic_selections
//   Gtk_gl_block  (&winMain, 1);
//   // wait-cursor
//   GUI_CursorWait (0);

  // call user - report mouse-button pressed
  GUI_CB_mouse (iEv, iMb);

//   // reset cursor from wait
//   GUI_CursorWait (1);            
//   // unblock keystrokes & grafic_selections
//   GUI_gl_block (&winMain, 0);   
    // printf("ex Gtk_CB_button %d %d %d\n",x, y, state);



  L_exit:
    return FALSE;  // TRUE=1=stop further handling of event ..

}


//================================================================
static gboolean Gtk_CB_scroll (GtkWidget* widget, GdkEventScroll *event) {
//================================================================

  printf("Gtk_CB_scroll %d\n",event->direction);

  switch (event->direction) {

  case GDK_SCROLL_UP:
      printf(" scroll-up\n");
    GR_Scale *= 1.2;
    break;

  case GDK_SCROLL_DOWN:
      printf(" scroll-down\n");
    GR_Scale /= 1.2;
    break;

  default:
    break;
  }

  // scale
  GL_view_update ();

  // redraw
  gtk_gl_area_queue_render (GTK_GL_AREA (gl_area));


  L_exit:
      printf(" ex-Gtk_CB_scroll GR_Scale=%f\n",GR_Scale);
    return FALSE;
}



//================================================================
static void Gtk_CB_resize (GtkGLArea *area, gint width, gint height) {
//================================================================

  double sx, sy, sz, sxy;

  printf("Gtk_CB_resize %d %d\n",width,height);

  if(width > 0) {
    GR_sizWinX_px = width;
    if(GR_sizWinX_px <= 0) GR_sizWinX_px = 1;
    GR_sizWinY_px = height;
    if(GR_sizWinY_px <= 0) GR_sizWinY_px = 1;

    GR_resize ();

  }

  GL_viewport ();

}


//================================================================
static gboolean Gtk_CB_realize (GtkGLArea *area, GdkGLContext *context) {
//================================================================
// init OpenGL-window, OpenGL

  printf("\n\n");
  printf("=================== Gtk_CB_realize \n");
  printf(" - exit mit key Esc\n");
  printf(" - ----------------\n");



  //----------------------------------------------------------------
  gtk_gl_area_make_current(area);

  if (gtk_gl_area_get_error (GTK_GL_AREA(area)) != NULL) {
    printf("Failed to initialiize buffers\n");
    return FALSE;
  }

  // gtk_gl_area_set_has_alpha (area, TRUE);
  gtk_gl_area_set_has_depth_buffer (area, TRUE);
  // gtk_gl_area_set_auto_render (area, FALSE);


  //----------------------------------------------------------------
  GR_resize ();    // init display-driver
  GL_init ();      // init shaders, return to app

  return TRUE;
}


//================================================================
static void Gtk_CB_quit (GtkGLArea *area, GdkEvent *ev, void *data) {
//================================================================

  printf("Gtk_CB_quit \n");

    // glDeleteTextures(1, &iTex1);

    // glDeleteVertexArrays(1, &gVAO);
    // glDeleteBuffers(1, &gVBO);
    // glDeleteBuffers(1, &gIBO);

    // glDetachShader(prg_prim1, vertex_shader);
    // glDetachShader(prg_prim1, fragment_shader);
    // glDeleteShader(vertex_shader);
    // glDeleteShader(fragment_shader);
    // glDeleteProgram(prg_prim1);

  gtk_main_quit();

}


//================================================================
  int Gtk_CB_key (void *parent, void *event, void *data) {
//================================================================
// Gtk_CB_key              callback keyboard - key pressed in OGL-window


  int     irc, iEv, iKey;


  iEv  = ((GdkEventAny*)event)->type;
  iKey = ((GdkEventKey*)event)->keyval;

  printf("Gtk_CB_key ev=%d iKey=%d\n",iEv,iKey);

  if(iEv == GDK_KEY_PRESS) {
    iEv = TYP_EventPress;
    if(iKey == GUI_KeyShift_L)        {KeyStatShift = 1; goto L_exit;}
    else if(iKey == GUI_KeyControl_L) {KeyStatCtrl = 1;  goto L_exit;}
    

  } else if(iEv == GDK_KEY_RELEASE) {
    iEv = TYP_EventRelease;
    if(iKey == GUI_KeyShift_L)        KeyStatShift = 0;
    else if(iKey == GUI_KeyControl_L) KeyStatCtrl = 0;
    goto L_exit;

  } else return TRUE;  // TRUE=1   FALSE=0


  //----------------------------------------------------------------
  // handle GDK_KEY_PRESS ..
//   statMK = ((GdkEventKey*)event)->state;
//     printf("Gtk_CB_key key=%d ev=%d state=%d\n",iKey,iEv,statMK);

  irc = GUI_CB_key (iEv, iKey);  // report to app
  // irc=0: redraw / do-defaultOperations; else ignore
  if(irc) goto L_exit;           


  L_activate:
    // redraw
    gtk_gl_area_queue_render (GTK_GL_AREA (gl_area));

    return FALSE;
  

  L_exit:
    return TRUE;  // TRUE=1: do no defaultOperations

}


//================================================================
  int Gtk_redraw () {
//================================================================
 
  gtk_gl_area_queue_render (GTK_GL_AREA (gl_area));

  return 0;

}


//================================================================
  int GUI_init (int argc, char** argv) {
//================================================================
// create complete window; return to app with app_CB_init

  int       xRes, yRes, xfSiz, yfSiz;
  char      s1[256];
  GtkWidget *window, *box1;


  printf("GUI_init \n");

    // Linux-Version: /proc/version or "uname -a"
  system("uname -a");
  if(OS_checkFilExist("/proc/version", 1)) {
    system("cat /proc/version");
  }

  // libc-Version:
  // Linux32: 2011-07-13 2/9;
  if(OS_ck_libcVersion (VER_LIBC_MAJ, VER_LIBC_MIN) < 0) {
    sprintf(s1,"OS_config ERROR libcVersion; libc %d.%d or newer necessary.",
            VER_LIBC_MAJ, VER_LIBC_MIN);
    printf("%s\n",s1);
    // GUI_MsgBox (s1);
    TX_Write(s1);
    exit (0);
  }

  // gtk-Version
  // Linux32: 2011-07-13  2/16.
  if(GUI_ck_version (VER_GTK_MAJ, VER_GTK_MIN) < 0) {
    sprintf(s1,"OS_config ERROR gtkVersion; GTK %d.%d or newer necessary.",
            VER_GTK_MAJ, VER_GTK_MIN);
    printf("%s\n",s1);
    // GUI_MsgBox (s1);
    TX_Write(s1);
    exit (0);
  }


  //----------------------------------------------------------------
  // need this for recognizing '.' as diecimal-delimiter for strtof ..
  gtk_disable_setlocale ();  // sonst Beistrich statt Decimalpunkt !! (LC_ALL)

  gtk_init(&argc, &argv);

  // create primary window
  window = gtk_window_new( GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "GL Area");
  // gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  // create box in window
  box1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, FALSE);
  // g_object_set (box1, "margin", 12, NULL);
  // gtk_box_set_spacing (GTK_BOX (box1), 6);
  gtk_container_add (GTK_CONTAINER (window), box1);


  //----------------------------------------------------------------
  // get total screensize
  // OS_get_scrRes (&xRes, &yRes);
  OS_get_scrRes (&GR_sizScrX_px, &GR_sizScrY_px, &GR_sizScrX_mm, &GR_sizScrY_mm);

  // get size of font
  GUI_get_fontSiz (&xfSiz, &yfSiz, window);


  //----------------------------------------------------------------
  // add grafic-window into box1

  gl_area = gtk_gl_area_new ();

//   glarea = gtk_gl_area_new_vargs(
//          NULL,                      // no sharing
//          GDK_GL_DOUBLEBUFFER,
//          GDK_GL_RGBA,
//          GDK_GL_DEPTH_SIZE,1,
//          GDK_GL_NONE);                        // must be last arg

  if(!gl_area) {
    printf ("***** Can't create GtkGlArea widget\n");
    return FALSE;
  }


  gtk_widget_set_size_request(gl_area, GR_sizWinX_px, GR_sizWinY_px);
  gtk_widget_set_can_focus (gl_area, TRUE);  // else no keyb-callback
  gtk_box_pack_start (GTK_BOX(box1), gl_area, 1, 1, 0);


  gtk_widget_set_events(gl_area,
            GDK_EXPOSURE_MASK|
            GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK|
            GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
            GDK_SCROLL_MASK|
            GDK_POINTER_MOTION_MASK);


  g_signal_connect (window,  "delete-event",        
                    G_CALLBACK(Gtk_CB_quit),NULL);

  g_signal_connect (gl_area, "realize",             
                    G_CALLBACK(Gtk_CB_realize),NULL);

  g_signal_connect (gl_area, "render",              
                    G_CALLBACK(Gtk_CB_render), NULL);

  g_signal_connect (gl_area, "resize",              
                    G_CALLBACK(Gtk_CB_resize), NULL);

  g_signal_connect (gl_area, "motion-notify-event", 
                    G_CALLBACK(Gtk_CB_motion), NULL);

  g_signal_connect (gl_area, "button-press-event",  
                    G_CALLBACK(Gtk_CB_button), NULL);
  g_signal_connect (gl_area, "button-release-event",
                    G_CALLBACK(Gtk_CB_button), NULL);

  g_signal_connect (gl_area, "scroll-event",        
                    G_CALLBACK(Gtk_CB_scroll), NULL);

  g_signal_connect (gl_area, "key-press-event",     
                    G_CALLBACK(Gtk_CB_key),    NULL);
  g_signal_connect (gl_area, "key-release-event",   
                    G_CALLBACK(Gtk_CB_key),    NULL);

  gtk_widget_show_all (window);


  // start main loop, continue with Gtk_CB_realize
  gtk_main ();

  return 0;
}


//================================================================
  int Gtk_ck_version (int vMaj, int vMin) {
//================================================================
/// INTERNAL check GTK_MAJOR_VERSION,GTK_MINOR_VERSION


  // printf("Gtk_ck_version %d %d\n",vMaj,vMin);


  printf("GTK_MAJOR_VERSION=%d GTK_MINOR_VERSION=%d\n",
          GTK_MAJOR_VERSION,GTK_MINOR_VERSION);


  if(GTK_MAJOR_VERSION < vMaj) return -1;

  if(GTK_MAJOR_VERSION == vMaj) {
    if(GTK_MINOR_VERSION < vMin) {
      return -1;
    }
  }

  return 0;

}


//================================================================
  int Gtk_get_fontSiz (int *isx, int *isy, void *win1) {
//================================================================
// Output:
//   isx     nr of pixels hor.
//   isy     nr of pixels vert.


  GtkStyleContext      *style;
  PangoFontMetrics     *fontGeo;
  PangoContext         *context;
  PangoFontDescription *fontDesc;
  // PangoLayout *pl;
  int   UI_fontsizX, UI_fontsizY;



  style = gtk_widget_get_style_context (win1);
  //fontDesc = gtk_style_context_get_font (style, GTK_STATE_FLAG_NORMAL);
  gtk_style_context_get (style, GTK_STATE_FLAG_NORMAL,
                         GTK_STYLE_PROPERTY_FONT, &fontDesc, NULL);

  context = gtk_widget_get_pango_context (win1);

  fontGeo = pango_context_get_metrics (context, fontDesc, NULL);

  // pango_layout_get_pixel_size (pl, isx, isy);

  // UI_fontsizX = pango_font_metrics_get_approximate_char_width (fontGeo);
  UI_fontsizX = pango_font_metrics_get_approximate_digit_width (fontGeo);

  UI_fontsizY = pango_font_metrics_get_ascent (fontGeo);
  UI_fontsizY += pango_font_metrics_get_descent (fontGeo);


  // change device-units -> absolute units (pixels)
  *isx = UI_fontsizX / PANGO_SCALE;
  *isx += 1;          // + space between 2 chars

  *isy = UI_fontsizY / PANGO_SCALE;
  *isy += *isy / 3;   // + space between 2 lines

    printf("ex-Gtk_get_fontSiz %d %d\n",*isx,*isy);

  return 0;

}


//================================================================
  int Gtk_block_gr (int mode) {
//================================================================
// Gtk_block_gr          block/unblock grafic-input from mouse for grafic-window
// Blocks scroll-events, key-events, button-events and move-events.
// block (1) or unblock (0) input

  int         i1;
//   Obj_GLwin   *go;
  GtkWidget   *wi;



  // printf("Gtk_block_gr %d\n",mode);


//   go = GUI_obj_pos (mo);
//   if(!go) return 0;
// 
  // if(!UI_MainWin) return 0;  // only for MS-Win !

  wi = gl_area;

  if(mode != 0) goto L_1;


  //----------------------------------------------------------------
  // unblock scroll
  g_signal_handlers_unblock_by_func (wi, G_CALLBACK(Gtk_CB_scroll), NULL);

  // unblock mouse-buttons
  g_signal_handlers_unblock_by_func (wi, G_CALLBACK(Gtk_CB_button), NULL);

  // unblock mouse-movements
  g_signal_handlers_unblock_by_func (wi, G_CALLBACK(Gtk_CB_motion), NULL);

  return 0;


  //----------------------------------------------------------------
  // 1 = block keystrokes & grafic_selections
  L_1:
  // block mouse-movements
  g_signal_handlers_block_by_func (wi, G_CALLBACK(Gtk_CB_motion), NULL);

  // block mouse-buttons
  g_signal_handlers_block_by_func (wi, G_CALLBACK(Gtk_CB_button), NULL);

  // block scroll
  g_signal_handlers_block_by_func (wi, G_CALLBACK(Gtk_CB_scroll), NULL);


  //----------------------------------------------------------------

  return 0;

}


//================================================================
  int Gtk_cursor__ (int iCur) {
//================================================================
// GUI_cursor__              change cursor;
//   mo         window-obj; NULL = mainWindow
//   iCur       0=normal, 1=wait, 2=rotate, 3=move, 4=zoom 5=blocked

  static int curTab[]={GDK_LEFT_PTR,       // 0 68   normal
                      GDK_WATCH,           // 1 150  wait
                      GDK_EXCHANGE,        // 2 50   rotate
                      GDK_HAND1,           // 3 58   move
                      GDK_SIZING,          // 4 120  zoom
                      GDK_X_CURSOR};       // 5 0    GDK_MAN,GDK_PIRATE};
// GDK_DIAMOND_CROSS      36
// GDK_SB_V_DOUBLE_ARROW 116 
// GDK_MAN                80   no selections


  int         iNew;
  void        *win;
  GdkCursor   *cNxt;


  // printf("GUI_cursor__ %d\n",iCur);

  win = gl_area;


  if(iCur < 0) return -1;
  if(iCur > 5) return -1;

  iNew = curTab[iCur];

  if(gtk_widget_is_drawable (win) == FALSE) return 0;

  cNxt = gdk_cursor_new_for_display (gdk_display_get_default(), iNew);

  // gdk_window_set_cursor (gtk_widget_get_window(win), gdk_cursor_new (iNew));
  gdk_window_set_cursor (gtk_widget_get_window(win), cNxt);

  // GUI_update__ ();    // wait for cursor-update
  // gdk_flush();
  // gdk_display_sync (gdk_display_get_default ());

  return 0;

}




// EOF
