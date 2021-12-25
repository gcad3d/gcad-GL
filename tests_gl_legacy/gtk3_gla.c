// gtk3_gla.c
//
// OpenGL-testprog with Gtk3 + gtkglarea  for OpenGL versions prior 3;
// DOES NOT WORK - ONLY FOR OpenGL - versions >= 3.3
// 
// gdk_gl_context_set_required_version:
// context versions less than 3.2 are not supported.
//
// build & start:
// make -f gtk3_gla.mak && ./gtk3_gla


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <GL/gl.h>
#include <GL/glu.h>
#include <gtk/gtk.h>



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


/*
//================================================================
static GdkGLContext* context (GtkGLArea *area, void* data) {
//================================================================

  GdkGLContext    *contx;
  GError *error = NULL;
  GdkWindow       *glw;

  printf("------------ context -\n");


  contx = gtk_gl_area_get_context (area);           // BUG. ??

  // contx = gdk_gl_context_get_current ();

  // gdk_gl_context_set_debug_enabled (contx, TRUE);

  // glw = gtk_widget_get_window(area);

  // glw = gdk_gl_context_get_window (contx);
  // contx = gdk_window_create_gl_context (glw, &error);

  // gdk_gl_context_set_required_version (contx, 1, 2);

  gdk_gl_context_make_current (contx);

  return contx;
}
*/

//================================================================
static void realize (GtkGLArea *area) {
//================================================================

  GdkGLContext    *contx;

  printf("------------ realize -\n");

  // gtk_gl_area_make_current(area);

  if (gtk_gl_area_get_error (area) != NULL) {
    printf("***** Failed to initialize\n");
    return;
  }

  contx = gtk_gl_area_get_context (area);
  printf("legacy: %d\n",gdk_gl_context_is_legacy (contx));  // 0=false, 1=TRUE

  // gtk_gl_area_set_required_version (GTK_GL_AREA(area), 1, 2);
  // gtk_gl_area_set_required_version (GTK_GL_AREA(G_OBJECT(glarea)), 1, 2);

  gtk_gl_area_set_has_depth_buffer (area, TRUE);
  gtk_gl_area_set_has_stencil_buffer (area, FALSE);

    GL_config_test ();


  return;
}


//================================================================
static gboolean render (GtkGLArea *area, void *context) {
//================================================================

  printf("------------ render\n");



  // gtk_gl_area_make_current(area);

  DrawAQuad ();

  // glFlush();

  return TRUE;
}


//================================================================
int main(int argc, char *argv[]) {
//================================================================


  int         i1, i2;
  GtkWidget   *window;
  GdkGLContext    *contx;


  printf("gtk3_gla main \n");


  gtk_init (&argc, &argv);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  glarea = gtk_gl_area_new ();

  if(!glarea) {
    printf ("***** Can't create GtkGlArea widget\n");
    return FALSE;
  }


  gtk_gl_area_set_required_version (GTK_GL_AREA(glarea), 1, 2);
  gtk_gl_area_get_required_version (GTK_GL_AREA(glarea), &i1, &i2);
    printf(" OpenGL-version is %d.%d\n",i1,i2);

  gtk_gl_area_set_has_alpha (GTK_GL_AREA(glarea), TRUE);
  gtk_gl_area_set_has_depth_buffer (GTK_GL_AREA(glarea), TRUE);
  gtk_gl_area_set_has_stencil_buffer (GTK_GL_AREA(glarea), FALSE);
  gtk_gl_area_set_auto_render (GTK_GL_AREA(glarea), FALSE);


//   contx = gtk_gl_area_get_context (glarea);
// 
//   gdk_gl_context_set_required_version (contx, 1, 2);



  gtk_widget_set_events (glarea,
            GDK_EXPOSURE_MASK
            // |GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK|
            // GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK |
            // GDK_SCROLL_MASK|
            // GDK_POINTER_MOTION_MASK
            );

  // gtk_widget_add_events (glarea,
       // GDK_BUTTON1_MOTION_MASK    |
       // GDK_BUTTON2_MOTION_MASK    |
       // GDK_VISIBILITY_NOTIFY_MASK);


  // g_signal_connect(glarea, "create-context",  G_CALLBACK(context),        NULL);
  g_signal_connect(glarea, "realize",         G_CALLBACK(realize),        NULL);
  g_signal_connect(glarea, "configure_event", G_CALLBACK(config),         NULL);
  g_signal_connect(glarea, "draw",            G_CALLBACK(render),         NULL);

  g_signal_connect(window,"delete-event", G_CALLBACK(gtk_main_quit), NULL);

  // button_press_event
  // motion_notify_event
  // key_press_event
  // unrealize

  gtk_container_add (GTK_CONTAINER(window), glarea);

  gtk_widget_set_size_request (glarea, 400, 400);
  gtk_widget_set_app_paintable (glarea, TRUE);
  gtk_widget_set_redraw_on_allocate (glarea, FALSE);
  gtk_widget_set_can_focus (glarea, TRUE);           // for keys necessary ..

  gtk_widget_show_all (window);

  gtk_main();

}


// EOF
