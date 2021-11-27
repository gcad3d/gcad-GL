/* testprog Gtk3 + Opengl (gtk_gl_glarea)
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================


Driver OpenGL-V3.3


=====================================================
List_functions_start:

GL_add_pt
GL_add_cv
GL_add_sur

GL_dyn_sym3

GL_render__         redraw OpenGL-window

GL_init
GL_InitInfo         disp GL-version ..
GL_viewport         Set Viewport to window dimensions
GL_view_ortho       set view-matrix GR_matd_scl (ortho from WCS)
GL_view_update      set view-matrix GR_matd_view
GL_scale_def        set default-scale
GL_buffer_f_add_d   add floats into active buffer from doubles

GL_prog_make        link and check program
GL_shad_create      create VERTEX_SHADER and FRAGMENT_SHADER
GL_shad_make        compile, check shader

GL_test_bufferSD    test (read and print) BufferSubData
GL_test_VS1         test vertex-shader

List_functions_end:
=====================================================
*/
#ifdef globTag
 void GL(){}
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include <GL/gl.h>

#include "../ut/geo.h"                     // Point Plane Mat_4x4D ..
#include "../ut/deb.h"                     // DEB_dump_pt
#include "../ut/matrix.h"                  // M44..
#include "../ut/ut.h"                      // TX_Error
#include "../gr/gr.h"                      // GR_..
#include "../dl/dl.h"                      // DL_..
#include "../dl/dl_sym_dyn.h"                  // DL_SYD_add____
#include "../gui/gui.h"                     // GUI_CB_win_is_up
#include "../app/app.h"               // Typ_*
#include "gl_shCV.h"
#include "gl_shadSUn.h"
#include "gl_shSY2.h"
#include "glbt.h"

#define extern  // does eliminate "extern"
#include "../gl/gl.h"                      // GR_..
#undef extern




//----------------------------------------------------------------
// EXTERNALS:


//----------------------------------------------------------------
// PROTOTYPES:
void loadObj_tria (GLuint *vao_out, GLuint *buffer_out);




//================================================================
//================================================================

//================================================================
void GLAPIENTRY MessageCallback (GLenum source, GLenum type, GLuint id,
                GLenum severity, GLsizei length, const GLchar* message,
                const void* userParam ) {
//================================================================
 
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}
 


//================================================================
//================================================================



//=====================================================================
  int GL_InitInfo () {
//=====================================================================
// glDrawElements needs GL_VERSION > 2.0

  int     i1, i2;

  printf("GL_InitInfo\n");


  printf("GL_RENDERER:    %s\n",glGetString(GL_RENDERER));
  printf("GL_VERSION:     %s\n",glGetString(GL_VERSION));
  // printf("GL_EXTENSIONS:  %s\n",glGetString(GL_EXTENSIONS));
  glGetIntegerv (GL_MAJOR_VERSION, &i1);
  glGetIntegerv (GL_MINOR_VERSION, &i2);
    printf(" GL_MAJOR_VERSION = %d, GL_MINOR_VERSION = %d\n",i1,i2);
  // glGetIntegerv (GL_DEPTH_BITS, &i1);
    // printf(" GL_DEPTH_BITS = %d\n",i1);
  

  // printf("GLU_VERSION:    %s\n",gluGetString(GLU_VERSION));
  // printf("GLU_EXTENSIONS: %s\n",gluGetString(GLU_EXTENSIONS));

  return 0;
}




//================================================================
  int GL_init () {
//================================================================

  int   irc;

  // disp grafic-card-info
  const GLubyte* renderer = glGetString(GL_RENDERER);
  printf("Renderer: %s\n", renderer);

  // disp GL-version
  const GLubyte* vGL = glGetString(GL_VERSION);
  printf("OpenGL version supported %s\n", vGL);

  // disp GSLS-version
  const GLubyte* vGSLS = glGetString(GL_SHADING_LANGUAGE_VERSION);
  printf("GSLS version %s\n", vGSLS);

  printf("====================================== \n\n");

  glEnable (GL_DEBUG_OUTPUT);
  glDebugMessageCallback (MessageCallback, 0);

  //----------------------------------------------------------------
  // init matrix transl and scale
  M44D_init (GR_matd_mdl[0]);
  M44D_init (GR_matd_mdl[1]);
  M44D_init (GR_matd_mdl[2]);

  // create prog_SU = program surfaces 
  irc = GL_shadSUn_init ();
  if(irc < 0) exit(0);

  // create points/curves-shader shCV
  irc = GL_shCV_init ();
  if(irc < 0) exit(0);

  // create 2D-symbols-shader shSY2
  irc = GL_shSY2_init ();
  if(irc < 0) exit(0);


  //----------------------------------------------------------------
  glDisable(GL_BLEND);
  // glDisable(GL_DEPTH_CLAMP);
  // glDepthRange(0.00001f, 10000.f);
  glDepthFunc (GL_LEQUAL);     // overwrite if new Z-val is less or equal (normal)

  // create 2D-system-buttons
  GLBT_but_ini ();

  // init topView
  GR_view__ (FUNC_ViewTop);


  //----------------------------------------------------------------
  // do APP
  GUI_CB_win_is_up ();
  // loadObj_tria (&iBuf1, NULL);

  return 0;

}

 
//================================================================
  int GL_viewport () {
//================================================================

  // Set Viewport to window dimensions
  glViewport(0, 0, GR_sizWinX_px, GR_sizWinY_px);

//   GL_scale ();

  return 0;

}


//================================================================
  int GL_scale_def () {
//================================================================
// GL_scale_def              set default-scale

  double  winsiz;
  printf("GL_scale_def %d %f\n",GR_sizWinX_px,GR_Modsiz);

  // scale
  if(GR_sizWinX_px > GR_sizWinY_px) winsiz = (double)GR_sizWinY_px;
  else                              winsiz = (double)GR_sizWinX_px;
   
  GR_Scale = winsiz / GR_Modsiz;

    printf(" ex-GL_scale_def %f\n",GR_Scale);

  return 0;

}


//================================================================
  void GL_view_ortho () {
//================================================================
// GL_view_ortho             set view-matrix GR_matd_scl (ortho from WCS)

  double scl, sx, sy, sz;


  printf("GL_view_update angZ=%f angX=%f Scale=%f\n",GR_angZ,GR_angX,GR_Scale);
  printf("   GR_cenRot = %f %f %f\n",GR_cenRot.x,GR_cenRot.y,GR_cenRot.z);


  //----------------------------------------------------------------
  // set view-matrix for scale (deviceCoords from userCoords)
  sx = 2. / (double)GR_sizWinX_px;
  sy = 2. / (double)GR_sizWinY_px;
  sz = 2. / -60000.;                    // ???????
  M44D_scale__ (GR_matd_scl, sx, sy, sz);

}


//================================================================
  void GL_view_update () {
//================================================================
// GL_view_update                    set view-matrix GR_matd_view

  double scl, sx, sy, sz;


  printf("GL_view_update angZ=%f angX=%f Scale=%f\n",GR_angZ,GR_angX,GR_Scale);
  printf("   GR_cenRot = %f %f %f\n",GR_cenRot.x,GR_cenRot.y,GR_cenRot.z);


  //----------------------------------------------------------------
//   // set view-matrix for scale (deviceCoords from userCoords)
//   sx = 2. / (double)GR_sizWinX_px;
//   sy = 2. / (double)GR_sizWinY_px;
//   sz = 2. / -60000.;                    // ???????
//   M44D_scale__ (GR_matd_scl, sx, sy, sz);



  //----------------------------------------------------------------
  // set model-matrix mdm

  // translate to screen-center 
  M44D_transl__ (GR_matd_mdl[1], &GR_cenView);

  // scale 
  scl = GR_Scale;
  M44D_scale__ (GR_matd_mdl[2], scl, scl, scl);

  // set rotation
  // rotate around model-Z-axis (in WCS always 0,0,1)
  M44D_rot_Z (GR_matd_mdl[3], GR_angZ);

  // rotate around GR_view_vx (vector parallel to horizontal windowboundary)
  M44D_rot_X (GR_matd_mdl[4], GR_angX);


  // modelmatrix = rotation * translation
  // M44D_mult_n (GR_matd_mdl[0], 4, &GR_matd_mdl[1]);

  // multiply view-matrix * model-matrix    - deviceCoords from modelCoords
  // M44D_mult__ (GR_matd_view, GR_matd_scl, GR_matd_mdl[0]);

M44D_mult__ (GR_matd_mdl[5], GR_matd_mdl[4], GR_matd_mdl[3]);  // 5=rot+tilt
M44D_mult__ (GR_matd_mdl[6], GR_matd_mdl[5], GR_matd_mdl[2]);  // 6=rot+tilt+scl
M44D_mult__ (GR_matd_mdl[7], GR_matd_mdl[6], GR_matd_mdl[1]);  // 7=rot+tilt+scl+tra
M44D_mult__ (GR_matd_view, GR_matd_scl, GR_matd_mdl[7]);

  // get float mat-4x4 from double mat-4x4
  M44FC__m44dr (GR_matf_dev, GR_matd_view);


  //----------------------------------------------------------------
    M44FC_dump_mx4x4 ((float*)GR_matf_dev, "ex-GL_view_update-GR_matf_dev");

}


/* UU
//================================================================
  int GL_dyn_sym2 (int typ, Point *pos, Att_ln att, float size) {
//================================================================
// GL_dyn_sym2            add dynamic 2D-symbol
// INPUT:
//   typ        2D-symbol, eg SYM_TRIA
//   pos        position, world-coords
//   att        
//   size       in mm

  int      oSiz, iOff, iDyn;


  printf("GL_dyn_sym2 typ=%d\n",typ);
  DEB_dump_pt (pos, " _dyn_sym2-pos");


//   // size of 1 point (3 floats)
//   oSiz = sizeof(float) * 3;
// 
//   // use shaSY2
//   // Set as current vertex array:
//   glBindVertexArray (shSY2.idVao);
// 
//   // bind named-bufferObj
//   glBindBuffer (GL_ARRAY_BUFFER, shSY2.idVbo);
// 
//   //----------------------------------------------------------------
//   // check for buffer overflow
//   iOff = shSY2.iOff;
//   shSY2.iOff += oSiz;
//   if(shSY2.iOff >= shSY2.bSiz) {
//     TX_Error("******* ERROR BUFFER OVERFLOW SHADER shSY2 -");
//     TX_Error("******* TODO: get new bigger buffer, copy and delete buffer.");
//     return -1;
//   }
// 
//   //----------------------------------------------------------------
//   // Upload vertex data
//   // load data (target, offset_in_bytes, size, data)
//   GL_buffer_f_add_d (iOff, oSiz, (double*)pos);
// 
//   // store dataBaseIndex, bufferOffset (in bytes), objTyp in dynamic-dispList
//   DL_SYD_add__ (typ, pos, size, (void*)&att, iOff, 1);

  //----------------------------------------------------------------
  // disactivate active buffer and VAO
  // glBindBuffer (GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

    printf(" ex-GL_dyn_sym2 iDyn=%d\n",iDyn);

  return iDyn;

}

//================================================================
  int GL_dyn_sym3 (int typ, void *data, void *att, float size) {
//================================================================
// GL_dyn_sym3            add dynamic 3D-symbol
// INPUT:
//   typ        3D-symbol, eg SYM_TRIA
//   pos        position, world-coords
//   att        
//   size       in mm

  int      oSiz, iOff, iDyn;
  float    ptf1[4], ptf2[4];


  printf("GL_dyn_sym3 typ=%d\n",typ);
  // DEB_dump_pt (pos, " _dyn_sym3-pos");

  //----------------------------------------------------------------
  // Typ_PVC   VecPos     point+vector
  if(typ != Typ_PVC) goto L_errTyp;


  

  //----------------------------------------------------------------
 
  return 0;

  L_errTyp: 
    TX_Error("GL_dyn_sym3 L_errTyp %d ",typ);
    return -1;

}
*/


//================================================================
  int GL_add_pt (double ptPos[], Att_ln iatt, int dbi) {
//================================================================
// GL_add_sur                add point to displayList and grafic-system
// add to shader shCV


  int    oSiz, iOff;


  printf("GL_add_pt dbi=%d\n",dbi);

  // size of 1 point (3 floats)
  oSiz = sizeof(float) * 3;

  // Set as current vertex array:
  glBindVertexArray (shCV.idVao);

  // bind named-bufferObj
  glBindBuffer (GL_ARRAY_BUFFER, shCV.idVbo);

  //----------------------------------------------------------------
  // check for buffer overflow
  iOff = shCV.iOff;
  shCV.iOff += oSiz;
  if(shCV.iOff >= shCV.bSiz) {
    TX_Error("******* ERROR BUFFER OVERFLOW SHADER shCV -");
    TX_Error("******* TODO: get new bigger buffer, copy and delete buffer.");
    return -1;
  }

  // Upload vertex data
  // load data (target, offset_in_bytes, size, data)
  GL_buffer_f_add_d (iOff, oSiz, ptPos);

  //----------------------------------------------------------------
  // disactivate active buffer and VAO
  // glBindBuffer (GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

    printf(" ex-GL_add_sur dbi=%d\n",dbi);

  return iOff;

}


//================================================================
  int GL_add_cv (double cvDat[], int ptNr, Att_ln iatt, int dbi) {
//================================================================
// GL_add_sur                add polygon to displayList and grafic-system
// add to shader shCV
// dbi UU

  int    oSiz, iOff;

  printf("GL_add_cv dbi=%d\n",dbi);

  // size of vtxNr points (3 floats)
  oSiz = ptNr * sizeof(float) * 3;

  // Set as current vertex array:
  glBindVertexArray (shCV.idVao);

  // bind named-bufferObj
  glBindBuffer (GL_ARRAY_BUFFER, shCV.idVbo);
  
  //----------------------------------------------------------------
  // check for buffer overflow
  iOff = shCV.iOff;
  shCV.iOff += oSiz;
  if(shCV.iOff >= shCV.bSiz) {
    TX_Error("******* ERROR BUFFER OVERFLOW SHADER shCV -");
    TX_Error("******* TODO: get new bigger buffer, copy and delete buffer.");
    return -1;
  }

  // Upload vertex data
  // load data (target, offset_in_bytes, size, data)
  GL_buffer_f_add_d (iOff, oSiz, cvDat);

  //----------------------------------------------------------------
  // disactivate active buffer and VAO 
  // glBindBuffer (GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

    printf(" ex-GL_add_sur dbi=%d\n",dbi);

  return iOff;

}


//===========================================================================
  int GL_add_sur (double *surDat, int ptNr, int typ, ColRGB color, int dbi) {
//===========================================================================
// GL_add_sur                add surface to displayList and grafic-system
// add to shader shSUn

  int      i1, oSiz, iOff;


  printf("GL_add_sur typ=%d ptNr=%d dbi=%d\n",typ,ptNr,dbi);

  // size of <ptNr> points (3 * 3 floats)
  oSiz = ptNr * sizeof(float) * 3;

  // Set as current vertex array:
  glBindVertexArray (shSUn.idVao);

  // bind named-bufferObj
  glBindBuffer (GL_ARRAY_BUFFER, shSUn.idVbo);

  //----------------------------------------------------------------
  // check for buffer overflow
  iOff = shSUn.iOff;
  shSUn.iOff += oSiz;
  if(shSUn.iOff >= shSUn.bSiz) {
    TX_Error("******* ERROR BUFFER OVERFLOW SHADER shSUn -");
    TX_Error("******* TODO: get new bigger buffer, copy and delete buffer.");
    return -1;
  }

  // Upload vertex data
  // load data (target, offset_in_bytes, size_in_bytes, data)
  // glBufferSubData (GL_ARRAY_BUFFER, iOff, oSiz, vertDat);
  GL_buffer_f_add_d (iOff, oSiz, surDat);
  
  //----------------------------------------------------------------
  // disactivate active buffer and VAO
  // glBindBuffer (GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);


    printf(" ex-GL_add_sur dbi=%d\n",dbi);

  return iOff;

}


//================================================================
  int GL_buffer_f_add_d (int iOff, int oSiz, double *dData) {
//================================================================
// GL_buffer_f_add_d              add floats into active buffer from doubles
// Input:
//   iOff     startPos in buffer
//   oSiz     size in buffer in bytes for floats; size of dData: oSiz * 2)

  int    i1, ie;
  float  *fData;


  fData = (float*) MEM_alloc_tmp (oSiz);

  ie = oSiz / 4;
  // floats from doubles
  for(i1=0; i1<ie; ++i1) fData[i1] = dData[i1];

  // Upload vertex data
  // load data (target, offset_in_bytes, size_in_bytes, data)
  glBufferSubData (GL_ARRAY_BUFFER, iOff, oSiz, fData);

    // TESTBLOCK
    for(i1=0; i1<ie; i1 += 3)
      printf(" f_add_d %3d %9.2f %9.2f %9.2f\n",i1,
             fData[i1], fData[i1+1], fData[i1+2]);

    // END TESTBLOCK


  return 0;

}


//=======================================================================
  int GL_shad_create (int *shadID, char* shadSrc, int shadTyp) {
//=======================================================================
// GL_shad_create          create VERTEX_SHADER and FRAGMENT_SHADER

 
  int    irc, shader;


  printf("GL_shad_create %d\n",shadTyp);


  //----------------------------------------------------------------
  shader = glCreateShader (shadTyp);

  // get source from variable shadSU_src_frag
  glShaderSource (shader, 1, (const GLchar* const*)&shadSrc, NULL);

  irc = GL_shad_make (shader, shadTyp);
  if(irc < 0) return -2;

  *shadID = shader;

  return 0;

}


//================================================================
  int GL_shad_make (int shader, int type) {
//================================================================
// GL_shad_make                compile, check shader
 
  int   status;

  glCompileShader (shader);

  //----------------------------------------------------------------
  // check compile-status
  glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
  if(status == GL_FALSE) {
    int log_len;
    char *buffer;
    glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &log_len);
    buffer = MEM_alloc_tmp (log_len + 1);
    glGetShaderInfoLog (shader, log_len, NULL, buffer);
    printf ("***** Compile failure in %s shader:\n%s",
            type == GL_VERTEX_SHADER ? "vertex" : "fragment", buffer);
    glDeleteShader (shader);
    return -1;
  }


  return 0;
}


//================================================================
  int GL_prog_make (int program) {
//================================================================
// GL_prog_make         link and check program

  int status;


  glLinkProgram (program);


  // check program - link-status
  glGetProgramiv (program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    int log_len;
    char *buffer;
    glGetProgramiv (program, GL_INFO_LOG_LENGTH, &log_len);
    buffer = MEM_alloc_tmp (log_len + 1);
    glGetProgramInfoLog (program, log_len, NULL, buffer);
    printf ("***** Linking failure:\n%s", buffer);
    glDeleteProgram (program);
    return -1;
  }

  return 0;

}


//================================================================
 int GL_render__ () {
//================================================================
// GL_render__         redraw OpenGL-window


  printf("=================== GL_render__ \n");

  // Clear the viewport 
  glClearColor (0.8, 0.8, 0.8, 1.0);    // set background-color
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // disp objects
  GL_shadSUn_render ();    // disp shader surfaces with normals
//   // GL_rend_SUi ();    // shader surfaces with normals and indices
//   // GL_rend_SUt ();    // surfaces with normals texture-uv and indices (quads)
  GL_shCV_render ();     // disp point/curves (polygons)
  GL_shSY2_render ();    // disp dynamic-objects
  GLBT_render ();          // disp screen-buttons

  GL_shSY2_axis ();        // display axis-systems 

  return 0;
}


//================================================================
  int GL_test_bufferSD (int iOff, int fNr) {
//================================================================
// GL_test_bufferSD        test (read and print) BufferSubData
//   glGetBufferSubData - get a copy of data from the video-card
// INPUT:
//   iOff    offset from start-of-buffer in bytes
//   fNr     nr of floats to print


  printf(" GL_test_bufferSD %d %d ---------------\n",iOff,fNr);

  float data[3];
  int   iSiz = fNr * sizeof(float);

  // (GL_ARRAY_BUFFER, offset_in_bytes, size_in_bytes, ptr_data_out);
  // copy -> data
  glGetBufferSubData (GL_ARRAY_BUFFER, iOff, iSiz, data);

  int ii;

  for(ii = 0; ii < fNr; ii += 3) {
    printf(" BSD[%2d] %9.3f %9.3f %9.3f\n",ii,data[ii],data[ii+1],data[ii+2]);
  }

  return 0;

}


//================================================================
  int GL_test_VS1 (int dli) {
//================================================================
// GL_test_VS1                       test vertex-shader 
// buffer must be already activated

  int   i1, i2, i3, vtxNr, glOff;
  float data[9], vc4[4], vco[4];

return 0;
  printf("GL_test_VS1 %d\n",dli);

  vtxNr = DL__[dli].vtxNr;
  glOff  = DL__[dli].glOff;
    printf(" test_VS1-vtxNr=%d glOff=%d\n",vtxNr,glOff);


  // get coordinates out of active buffer - triangle; 3 points = 36 byte
  glGetBufferSubData (GL_ARRAY_BUFFER, 0, 36, data);


//   for(i1=0; i1<9; i1 += 3) {
//     printf("%2d %9.3f %9.3f %9.3f\n",i1,data[i1],data[i1+1],data[i1+2]);
//   }

  // translate
//   for(i1=0; i1<9; i1 += 3) {
//     // make 4f-vector
//     M44FC_vec4_vec3 (vc4, &data[i1], 1.f);
//       M44FC_dump_vc4 (vc4, "vc4");
// 
// 
//     // multiply with GR_matf_dev = NDC
//     M44FC_vc4_mult_m44 (vco, vc4, (float*)GR_matf_dev);
//       M44FC_dump_vc4 (vco, "vco");
//     // ..
//     // printf("%2d %9.3f %9.3f %9.3f\n",i1,data[i1],data[i1+1],data[i1+2]);
//   }


  return 0;

}


// EOF
