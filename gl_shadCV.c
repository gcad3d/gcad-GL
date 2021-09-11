/* ./gl_shadCV.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Shader for points and curves

=====================================================
List_functions_start:

GL_shadCV_init
GL_shadCV_render

List_functions_end:
=====================================================

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include <GL/gl.h>

#include "./ut_types.h"                // UINT_32
#include "./geo.h"             // Point Plane Mat_4x4D ..
#include "./gr.h"                      // Att_ln GR_..
#include "./gl.h"                      // shCV
#include "./dl.h"                      // DL_..
#include "./col.h"                     // Att_4f_att
#include "./tst_prim1.h"               // Typ_*





//----------------------------------------------------------------
// vertexShader
char *shadCV_src_vtx =
"#version 330 core\n\
in vec3 aVert;\
uniform mat4 mfv;\
uniform vec4 aColor;\
out vec4 fColor;\
void main() {\
  gl_Position = mfv * vec4(aVert, 1.0);\
  fColor = aColor;\
}";


//----------------------------------------------------------------
// fragmentShader
char *shadCV_src_frag =
"#version 330 core\n\
in vec4 fColor;\
out vec4 pColor;\
void main() {\
  pColor = fColor;\
}";


//================================================================
  int GL_shadCV_init () {
//================================================================
// GL_shadCV_init            create points/curves shader shadCV

  int    irc, program = 0;
  GLuint vertex, fragment, vao, vbo, bSiz,  ulMvp, ulAtt;


  printf("GL_shadCV_init -----------\n");

  // create shader for surfaces
  irc = GL_shad_create (&vertex, shadCV_src_vtx, GL_VERTEX_SHADER);
  if(irc < 0) exit(0);
  irc = GL_shad_create (&fragment, shadCV_src_frag, GL_FRAGMENT_SHADER);
  if(irc < 0) exit(0);

  // combine shaders to Program
  // get empty program and program-ID
  program = glCreateProgram ();
  glAttachShader (program, vertex);
  glAttachShader (program, fragment);

  // link & check program
  irc = GL_prog_make (program);
  if(irc < 0) return -1;



  //----------------------------------------------------------------
  // ortho:
  // get unifom-record-index
  ulMvp = glGetUniformLocation (program, "mfv");
  if(ulMvp == -1) printf(" ************ ERROR glGetUniformLocation mvp\n");

  // get unifom-record-index
  ulAtt = glGetUniformLocation (program, "aColor");
  if(ulAtt == -1) printf(" ************ ERROR glGetUniformLocation aColor\n");
    printf(" program=%d ulMvp=%d ulAtt = %d\n",program,ulMvp,ulAtt);



  //----------------------------------------------------------------
  // Generate 1 empty vertex array object; return its ID(s) in vao
  glGenVertexArrays (1, &vao);

  // Generate 1 empty buffer; return its ID(s) in buffer
  glGenBuffers (1, &vbo);

  // Set as current vertex array:
  glBindVertexArray (vbo);

  // bind named-bufferObj
  glBindBuffer (GL_ARRAY_BUFFER, vbo);

  // get space for 1000 vertices
  bSiz = sizeof(float) * 1000;
  // (type, size_in_bytes, data, usage)
  glBufferData (GL_ARRAY_BUFFER, bSiz, NULL, GL_STATIC_DRAW);

  shCV.idVao = vao;
  shCV.idVbo = vbo;
  shCV.idPrg = program;
  shCV.ulMvp = ulMvp;
  shCV.ulAtt = ulAtt;
  shCV.bSiz = bSiz;
  shCV.iOff = 0;
    printf(" shadCV_init idPrg=%d vao=%d vbo=%d\n",
           shCV.idPrg, shCV.idVao, shCV.idVbo);


  glDetachShader (program, vertex);
  glDetachShader (program, fragment);

  irc = 0;

  //----------------------------------------------------------------


  L_out:
    glDeleteShader (vertex);
    glDeleteShader (fragment);

  L_exit:
    printf(" ex-GL_shadCV_init irc=%d %d\n",irc, program);
  return irc;

}


//================================================================
  void GL_shadCV_render (void) {
//================================================================
// Input:
//   fixed for all surfaces: prog_SU ulMvp, ulAtt,
//   variable:  bufNr

  int     i1, vtxTyp, vtxNr;
  float   fCol[4];
  Att_ln  actAtt, iatt;


  printf("GL_shadCV_render idPrg=%d DL_nr=%d\n",shCV.idPrg,DL_nr);

  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_POLYGON_SMOOTH);
  glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //----------------------------------------------------------------
  // activate shaderProg shCV
  glUseProgram (shCV.idPrg);

  // set model-view-matrix
  glUniformMatrix4fv (shCV.ulMvp, 1, GL_FALSE, (float*)mfv);

  // bind the VAO (the triangle)
  glBindVertexArray (shCV.idVao);
  glBindBuffer (GL_ARRAY_BUFFER, shCV.idVbo);

  glEnableVertexAttribArray(0);
  // (index-vertex-attrib, nr-components, format, do-normalize, stride, offset)
// POINTS: 3 floats
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // glDisable (GL_BLEND);
  // glEnable (GL_LINE_SMOOTH);

  //----------------------------------------------------------------
  // loop tru all objs in this buffer
  actAtt = Att_ln_NUL;

  for(i1=0; i1<DL_nr; ++i1) {
    // skip all but points/curves (Typ_PT, Typ_CV)
    if(DL__[i1].typ > Typ_CV) continue;

      printf(" rend_CV----- %d iOff=%d\n",i1,DL__[i1].bOff);

    // skip hidden obj    TODO: respect subModelID
    if(DL__[i1].disp) continue;

    // update width, color
    if(memcmp(&actAtt, &DL__[i1].iatt, sizeof(UINT_32))) {
      // activate modified width or color
      iatt = *(Att_ln*)&DL__[i1].iatt;
      glPointSize ((float)iatt.thick);
      glLineWidth ((float)iatt.thick / 2.f);
      Att_4f_att (fCol, &iatt);
      glUniform4fv (shCV.ulAtt, 1, fCol);
    }

    // display obj; startIndex = vertxIndex (12 bytes per vertex)
    //  (vertexType, startIndex, nr-vertices-to-render)
    if(DL__[i1].typ < Typ_CV) {
      glDrawArrays (GL_POINTS, DL__[i1].bOff / 12, 1);

    } else {
      // display obj; startIndex = vertxIndex (12 bytes per vertex)
      //  (vertexType, startIndex, nr-vertices-to-render)
      // vtxNr = (DL__[i1+1].bOff - DL__[i1].bOff) / 12;
      vtxNr = DL__[i1].vtxNr;
      glDrawArrays (GL_LINE_STRIP, DL__[i1].bOff / 12, vtxNr);
    }


  }

  // glDisable (GL_LINE_SMOOTH);

  //----------------------------------------------------------------
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glBindVertexArray (0);  // clear active buffer

  // We finished using the buffers and program
  // close - disactivate
  glUseProgram (0);     // disactivate active program


  //----------------------------------------------------------------
// disp indexed array:
//   // (lineMode, nr, typDatInd, indArr)
//   glDrawElements (GL_LINE_STRIP, n, GL_UNSIGNED_SHORT, indices);


  //----------------------------------------------------------------
  glDisable(GL_BLEND);


}


// EOF
