// glx_only.c
//
// OpenGL-testprog with GLX and Xlib - see also
// https://www.khronos.org/opengl/wiki/Programming_OpenGL_in_Linux:_GLX_and_Xlib
//
// build & start:
// gcc -o glx_only glx_only.c -lX11 -lGL -lGLU && ./glx_only


#include<stdio.h>
#include<stdlib.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

Display                 *dpy;
Window                  root;
GLint                   att[] = {GLX_RGBA,
                                 GLX_DEPTH_SIZE, 24,
                                 GLX_DOUBLEBUFFER,
                                 None };
XVisualInfo             *vi;
Colormap                cmap;
XSetWindowAttributes    swa;
Window                  win;
GLXContext              glc;
XWindowAttributes       gwa;
XEvent                  xev;

//================================================================
  int GL_config_test () {
//================================================================
/// see also GL_GetColorBits
/// Retcod: 0=no, hardware does not accept textures; 1=Yes, Ok.

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

    return 0;

}


//================================================================
  int GLX_config_test () {
//================================================================

  int     i1;
  char  const  *p1;

  printf("GLX visual %p selected\n", (void*)vi->visualid);

  glXGetConfig (dpy, vi, GLX_RED_SIZE, &i1);
  printf(" GLX_RED_SIZE=%d\n",i1);

  glXGetConfig (dpy, vi, GLX_DEPTH_SIZE, &i1);
  printf(" GLX_DEPTH_SIZE=%d\n",i1);

  p1 = glXGetClientString (dpy, GLX_VENDOR);
  printf(" GLX_VENDOR=%s\n",p1);

  p1 = glXGetClientString(dpy, GLX_VERSION);
  printf(" GLX_VERSION=%s\n",p1);

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
int main(int argc, char *argv[]) {
//================================================================

  dpy = XOpenDisplay(NULL);
 
  if(dpy == NULL) {
    printf("\n\tcannot connect to X server\n\n");
    exit(0);
  }
        
  root = DefaultRootWindow(dpy);

  vi = glXChooseVisual(dpy, 0, att);

  if(vi == NULL) {
    printf("\n\tno appropriate visual found\n\n");
    exit(0);
  }

  GLX_config_test ();


  cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

  swa.colormap = cmap;
  swa.event_mask = ExposureMask | KeyPressMask;
 
  win = XCreateWindow(dpy, root,
                      0, 0, 600, 600,
                      0, vi->depth, InputOutput, vi->visual,
                      CWColormap | CWEventMask, &swa);

  XMapWindow(dpy, win);
  XStoreName(dpy, win, "tst_gl1");
  
  glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
  glXMakeCurrent(dpy, win, glc);
 
  glEnable(GL_DEPTH_TEST); 
 
  while(1) {
    XNextEvent(dpy, &xev);
        
    if(xev.type == Expose) {
      XGetWindowAttributes(dpy, win, &gwa);
      glViewport(0, 0, gwa.width, gwa.height);
      DrawAQuad(); 
      glXSwapBuffers(dpy, win);

    } else if(xev.type == KeyPress) {
      glXMakeCurrent(dpy, None, NULL);
      glXDestroyContext(dpy, glc);
      XDestroyWindow(dpy, win);
      XCloseDisplay(dpy);
      exit(0);
    }
  }
}


// EOF
