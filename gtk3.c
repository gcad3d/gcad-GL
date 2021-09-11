/* ./gtk3.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Driver GUI - user-interface Gtk-V3

=====================================================
List_functions_start:

GUI_redraw           start redraw window
GUI_Color_select     get new color from user
GUI_get_fontSiz      get fontsize of Gtk-font
GUI_ck_version       check GTK_MAJOR_VERSION,GTK_MINOR_VERSION
GUI_init             create complete window; return to app with MAIN_CB_win_is_up

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
*/

#ifdef globTag
 void GTK(){}
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


#include "./geo.h"                   // Point Plane Mat_4x4D ..
#include "./gr.h"                    // GR_sizWinX_px ..
#include "./gl.h"
#include "./matrix.h"
#include "./gui.h"                   // GUI_CB_key
#include "./os.h"                    // OS_checkFilExist
#include "./ut.h"                    // TX_Write
#include "./tst_prim1.h"               // Typ_* GUI_CB_key



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
/// \code
/// GUI_Color_select          select-color-Dialog. Modal (wait).
/// Returns 3 colors as 16-bit-integers.
/// Returncode   0=OK; -1=cancel.
/// \endcode

  int         iRes;     // response
  GtkWidget   *wdlg;
  GtkWidget   *wSel;
  // GdkColor    rgb;      // 3 x short
  GdkRGBA      rgb;

  // printf("GUI_ColSel\n");


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

  GL_render__ ();

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

  // printf(" Gtk_CB_motion %f %f keyShift=%d\n",event->x,event->y,KeyStatShift);


  //----------------------------------------------------------------
  // update state modifier-keys
    // get state of shift,ctrl,alt-keys
  ikey = gdk_keymap_get_modifier_state
           (gdk_keymap_get_for_display(gdk_display_get_default()));


  // shift=1, ctrl=4
  if(ikey & 1)     KeyStatShift = 1;     else KeyStatShift = 0;
  if(ikey & 4)     KeyStatCtrl  = 1;     else KeyStatCtrl  = 0;
  if(ikey & 8)     KeyStatAlt   = 1;     else KeyStatAlt   = 0;


  //----------------------------------------------------------------
  if(KeyStatShift == 0) goto L_ck_rot;      // 1=pressed, 0=not
  // move model

  // get mousepos in WCS on constrPlane along eyeLine
  dx = event->x - GR_mousePosX;
  dy = event->y - GR_mousePosY;
    printf(" CB_motion dx=%f dy=%f\n",dx,dy);

// TODO:
  // must be usercoords ..
  dx /= (float)GR_sizWinX_px / 2.f;
  dy /= (float)GR_sizWinY_px / 2.f;

  GR_cenView.x += dx;
  GR_cenView.y -= dy;
    printf(" CB_motion-transl %f,%f\n",GR_cenView.x,GR_cenView.y);

  // translate to center of screen
  GL_view_update  ();
  goto L_upd;


  //----------------------------------------------------------------
  L_ck_rot:
  if(KeyStatCtrl == 0) goto L_exit;
  // rotate model

  // get relative movement of mouse from last stored position
  dx = event->x - GR_mousePosX;
  dy = GR_mousePosY - event->y;
    printf(" CB_motion-rot dx=%f dy=%f\n",dx,dy);

  // rotate around Z
  // angZ = angle to rotate around Z
  GR_angZ += dx / 100.;

  // rotate around Y
  // angX = angle to tilt
  GR_angX -= dy / 100.;

  GL_view_update ();

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

    // printf("GUI_gl_button typ=%d but=%d\n",typ,ikey);


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
//   GL_scale ();
  GL_view_update ();

  // redraw
  gtk_gl_area_queue_render (GTK_GL_AREA (gl_area));


  L_exit:
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
  int GUI_redraw () {
//================================================================
 
  gtk_gl_area_queue_render (GTK_GL_AREA (gl_area));

  return 0;

}


//================================================================
  int GUI_Color_select (int *ir, int *ig, int *ib) {
//================================================================
// GUI_Color_select             get new defaultColor from user

  int   i1, i2, i3;

  Gtk_Color_select (ir, ig, ib);

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
                    G_CALLBACK(Gtk_CB_button), "1");
  g_signal_connect (gl_area, "button-release-event",
                    G_CALLBACK(Gtk_CB_button), "0");
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
  int GUI_ck_version (int vMaj, int vMin) {
//================================================================
/// INTERNAL check GTK_MAJOR_VERSION,GTK_MINOR_VERSION


  // printf("GUI_ck_version %d %d\n",vMaj,vMin);


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
  int GUI_get_fontSiz (int *isx, int *isy, void *win1) {
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

    printf("GUI_get_fontSiz %d %d\n",*isx,*isy);

  return 0;

}



// EOF
