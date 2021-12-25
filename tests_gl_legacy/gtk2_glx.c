// gtk2_glx.c
//
// OpenGL-testprog with Gtk2 + GLX and Xlib - see also
// https://www.khronos.org/opengl/wiki/Programming_OpenGL_in_Linux:_GLX_and_Xlib
//
// build & start:
// make -f gtk2_glx.mak && ./a.out


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <gtk/gtk.h>
#include <gdk/gdkx.h>        // gdk_x11_get_default_xdisplay GDK_WINDOW_XID
// #include <gdk/gdkkeysyms.h>



static  Display     *GLB_display;
static  GLXContext  GLB_x_context;
static  int         GLB_x_id = 0;
static  GtkWidget   *glarea;


//================================================================
  int GL_config_test () {
//================================================================

  int  i1, i2, i3, i4;

  printf("GL_config_test: \n");


  glGetIntegerv (GL_DEPTH_BITS, &i1);
  printf(" GL_DEPTH_BITS = %d\n",i1);

  glGetIntegerv (GL_STENCIL_BITS, &i1);
  printf(" GL_STENCIL_BITS = %d\n",i1);


  glGetIntegerv (GL_ACCUM_RED_BITS,   &i1);
  glGetIntegerv (GL_ACCUM_GREEN_BITS, &i2);
  glGetIntegerv (GL_ACCUM_BLUE_BITS,  &i3);
  glGetIntegerv (GL_ACCUM_ALPHA_BITS, &i4);
  printf(" GL_ACCUM_RED/GREEN/BLUE/ALPHA_BITS = %d %d %d %d\n",i1,i2,i3,i4);

  glGetIntegerv (GL_RED_BITS,   &i1);
  glGetIntegerv (GL_GREEN_BITS, &i2);
  glGetIntegerv (GL_BLUE_BITS,  &i3);
  glGetIntegerv (GL_ALPHA_BITS, &i4);
  printf(" GL_RED/GREEN/BLUE/ALPHA_BITS = %d %d %d %d\n",i1,i2,i3,i4);

  printf(" GLU_VERSION:    %s\n",gluGetString(GLU_VERSION));
  printf(" GLU_EXTENSIONS: %s\n",gluGetString(GLU_EXTENSIONS));

    return 0;

}


//================================================================
  int GLX_config_test () {
//================================================================

  int     i1;
  char  const  *p1, *p2;


  printf("GLX_config_test: \n");


  // GLB_display = gdk_x11_get_default_xdisplay ();

  p1 = glXGetClientString (GLB_display, GLX_VENDOR);
  if(!p1) return -1;

  p2 = glXGetClientString(GLB_display, GLX_VERSION);
    printf("OpenGL Vendor %s  Version %s\n",p1,p2);

//   printf("GLX visual %p selected\n", (void*)vi->visualid);
// 
//   glXGetConfig (dpy, vi, GLX_RED_SIZE, &i1);
//   printf(" GLX_RED_SIZE=%d\n",i1);
// 
//   glXGetConfig (dpy, vi, GLX_DEPTH_SIZE, &i1);
//   printf(" GLX_DEPTH_SIZE=%d\n",i1);
// 
//   p1 = glXGetClientString (dpy, GLX_VENDOR);
//   printf(" GLX_VENDOR=%s\n",p1);
// 
//   p1 = glXGetClientString(dpy, GLX_VERSION);
//   printf(" GLX_VERSION=%s\n",p1);

  return 0;

}


//================================================================
void DrawAQuad() {
//================================================================

 GL_config_test ();

 glClearColor(1.0, 1.0, 1.0, 1.0);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-1., 1., -1., 1., 1., 20.);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

 glBegin(GL_QUADS);
  glColor3f(1., 0., 0.); glVertex3f(-.75, -.75, 0.);
  glColor3f(0., 1., 0.); glVertex3f( .75, -.75, 0.);
  glColor3f(0., 0., 1.); glVertex3f( .75,  .75, 0.);
  glColor3f(1., 1., 0.); glVertex3f(-.75,  .75, 0.);
 glEnd();

} 
 

//================================================================
  int GL_init () {
//================================================================

  int         xscreen, i1;
  GdkWindow   *window;
  GdkScreen   *screen;
  GdkVisual   *visual;
  Window      root;
  XVisualInfo *xvisual;
  Colormap    xcolormap;
  int         attributes[] = {
                GLX_RGBA,
                GLX_DEPTH_SIZE, 24,
                GLX_DOUBLEBUFFER,
                None };


  printf("GL_init \n");


  glarea = gtk_drawing_area_new ();
  gtk_widget_set_double_buffered (glarea, FALSE);


  GLB_display = gdk_x11_get_default_xdisplay ();
  xscreen = DefaultScreen (GLB_display);
  // screen = gdk_screen_get_default ();
    // printf(" screenNr = %d\n",gdk_screen_get_number(screen));

  xvisual = glXChooseVisual (GLB_display, xscreen, attributes);
    // printf(" xvisualid=%d\n",xvisual->visualid);

//   visual = gdk_x11_screen_lookup_visual (screen, xvisual->visualid);
// 
//   root = RootWindow (GLB_display, xscreen);
//   xcolormap = XCreateColormap (GLB_display, root, xvisual->visual, AllocNone);
// 
//   //colormap = gdk_x11_colormap_foreign_new (visual, xcolormap);
//   //gtk_widget_set_colormap (glarea, colormap);

  GLB_x_context = glXCreateContext (GLB_display, xvisual, NULL, TRUE);
  free (xvisual);

  glXWaitX();
  glXWaitGL();

  return 0;

}

//================================================================
  int config (void *parent, void *event, void *data) {
//================================================================
// INTERNAL   draw / expose & config-callback


  GtkAllocation allocation;

  printf("------------ config\n");

  gtk_widget_get_allocation (parent, &allocation);
    printf("  new size %d %d\n", allocation.width, allocation.height);

  gtk_widget_queue_draw (glarea);  // redraw

  return TRUE;
  // return FALSE;  // FALSE=continue event handling ..
}



//================================================================
  int realize (void *parent, void *event, void *data) {
//================================================================

  gint    i1, i2;
  GError *error = NULL;
  GdkWindow     *window;

  printf("------------ realize\n");
  // printf(" eventType=%d\n",((GdkEventAny*)event)->type);

  GLB_x_id = gdk_x11_drawable_get_xid (gtk_widget_get_window (glarea));
    printf(" GLB_x_id=%d\n",GLB_x_id);

  glXMakeCurrent (GLB_display, GLB_x_id, GLB_x_context);

    GL_config_test ();


  return TRUE;
}


//================================================================
static gboolean render (GtkWidget *area, void *context) {
//================================================================

  printf("------------ render\n");



  // glEnable(GL_DEPTH_TEST);
  // glEnable(GL_BLEND);

  glXMakeCurrent (GLB_display, GLB_x_id, GLB_x_context);

  DrawAQuad ();

  // glXSwapBuffers (dpy, win);

  // gdk_gl_wait_gl();
  glFlush();

  glXSwapBuffers (GLB_display, GLB_x_id);

  return TRUE;
}


//================================================================
int main(int argc, char *argv[]) {
//================================================================


  int         i1, i2;
  GtkWidget   *window;
  GdkWindow   *glWindow;


  printf("gtk2_glx main \n");


  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  GL_init (); 

  gtk_widget_set_double_buffered (glarea, FALSE);
  gtk_widget_set_size_request (glarea, 400, 400);
  gtk_widget_set_app_paintable (glarea, TRUE);
  gtk_widget_set_redraw_on_allocate (glarea, FALSE);
  gtk_widget_set_can_focus (glarea, TRUE);           // for keys necessary ..


  gtk_widget_set_events (GTK_WIDGET (glarea),
                         GDK_EXPOSURE_MASK);

//   gtk_widget_add_events (glarea,
//        GDK_BUTTON1_MOTION_MASK    |
//        GDK_BUTTON2_MOTION_MASK    |
//        GDK_BUTTON_PRESS_MASK      |
//        GDK_VISIBILITY_NOTIFY_MASK);


  g_signal_connect(glarea, "realize",         G_CALLBACK(realize),        NULL);
  g_signal_connect(glarea, "configure_event", G_CALLBACK(config),         NULL);
  g_signal_connect(glarea, "expose_event",    G_CALLBACK(render),         NULL);

  g_signal_connect(window,"delete-event", G_CALLBACK(gtk_main_quit), NULL);


//   g_signal_connect (G_OBJECT (drawing_area), "motion_notify_event",
//         G_CALLBACK (motion_notify_event), NULL);
//   g_signal_connect (G_OBJECT (drawing_area), "button_press_event",
//         G_CALLBACK (button_press_event), NULL);
//   g_signal_connect (G_OBJECT (drawing_area), "unrealize",
//         G_CALLBACK (unrealize), NULL);
// //   g_signal_connect_swapped (G_OBJECT (window), "key_press_event",
// //           G_CALLBACK (key_press_event), drawing_area);
// 
//   // g_signal_connect(glarea,  "render",       G_CALLBACK(render),        NULL);


  gtk_container_add (GTK_CONTAINER(window), glarea);

  gtk_widget_show_all (window);

  gtk_main();

}


// EOF
