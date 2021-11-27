/* ./gl_shadSUn.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Shader for not-indexed surfaces

=====================================================
List_functions_start:

GL_shadSUn_init
GL_shadSUn_render

List_functions_end:
=====================================================

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include <GL/gl.h>

#include "../ut/geo.h"             // Point Plane Mat_4x4D ..
#include "../gr/gr.h"                      // Att_ln GR_..
#include "../gl/gl.h"                      // shCV
#include "../dl/dl.h"                      // DL_..
#include "../gr/col.h"                     // Att_4f_att
#include "../app/app.h"               // Typ_*




//----------------------------------------------------------------
// ortho
// vertexShader surfaces
char *shadSUn_src_vtx =
"#version 330 core\n\
in vec3 vertex;\
uniform mat4 GR_matf_dev;\
uniform vec4 aColor;\
out vec4 fColor;\
void main() {\
  gl_Position = GR_matf_dev * vec4(vertex, 1.0);\
  fColor = aColor;\
}";


//----------------------------------------------------------------
// fragmentShader surfaces
char *shadSUn_src_frag =
"#version 330 core\n\
in vec4 fColor;\
out vec4 pColor;\
void main() {\
  pColor = fColor;\
}";




//================================================================
  int GL_shadSUn_init () {
//================================================================
// GL_shadSUn_init            create shader shSUn
// Initialize the shader and link them into a program 

  GLuint vertex, fragment, buffer, ulMvp, ulMvm, ulAtt, vao, vbo, bSiz;
  int irc, status, program = 0;


  printf("GL_shadSUn_init -----------\n");

  // create shader for surfaces
  irc = GL_shad_create (&vertex, shadSUn_src_vtx, GL_VERTEX_SHADER);
  if(irc < 0) exit(0);
  irc = GL_shad_create (&fragment, shadSUn_src_frag, GL_FRAGMENT_SHADER);
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
  // get unifom-record-index of view-matrix
  ulMvp = glGetUniformLocation (program, "GR_matf_dev");
  if(ulMvp == -1) printf(" ************ ERROR glGetUniformLocation GR_matf_dev\n");


  //----------------------------------------------------------------
  // get unifom-record-index
  ulAtt = glGetUniformLocation (program, "aColor");
  if(ulAtt == -1) printf(" ************ ERROR glGetUniformLocation aColor\n");
    printf(" program=%d ulMvp=%d ulAtt = %d\n",program,ulMvp,ulAtt);



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

  shSUn.idVao = vao;
  shSUn.idVbo = vbo;
  shSUn.idPrg = program;
  shSUn.ulMvp = ulMvp;
  shSUn.ulMvm = ulMvm;
  shSUn.ulAtt = ulAtt;
  shSUn.bSiz = bSiz;
  shSUn.iOff = 0;
    printf(" shadSUn_init idPrg=%d vao=%d vbo=%d\n",
           shSUn.idPrg, shSUn.idVao, shSUn.idVbo);


  glDetachShader (program, vertex);
  glDetachShader (program, fragment);

  irc = 0;

  //----------------------------------------------------------------
  L_out:
    glDeleteShader (vertex);
    glDeleteShader (fragment);

  L_exit:
      printf(" ex-GL_shadSUn_init irc=%d %d\n",irc, program);
    return irc;

}


//================================================================
  void GL_shadSUn_render (void) {
//================================================================
// Input:
//   fixed for all surfaces: prog_SU ulMvp, ulAtt,
//   variable:  bufNr

  int   i1, vtxNr;
  static float fCol[4] = {0.6f, 0.2f, 0.2f, 1.0f};


  printf("GL_shadSUn_render idPrg=%d DL_nr=%d\n",shSUn.idPrg,DL_nr);


  // activate shaderProg shSUn
  glUseProgram (shSUn.idPrg);


//----------------------------------------------------------------
  // ortho:
  // set view-matrix
  glUniformMatrix4fv (shSUn.ulMvp, 1, GL_FALSE, (float*)GR_matf_dev);


//----------------------------------------------------------------
  // bind the VAO (the triangle)
  glBindVertexArray (shSUn.idVao);
  glBindBuffer (GL_ARRAY_BUFFER, shSUn.idVbo);

  glEnableVertexAttribArray(0);
  // (index-vertex-attrib, nr-components, format, do-normalize, stride, offset)
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);


  //----------------------------------------------------------------
  // loop tru all objs in this buffer

  for(i1=0; i1<DL_nr; ++i1) {
    // skip all but surfs
    if((DL__[i1].typ >= Typ_SOL) ||
       (DL__[i1].typ < Typ_SUR))      continue;

      printf(" rend_SUn----- %d iOff=%d\n",i1,DL__[i1].glOff);

    // skip hidden obj    TODO: respect subModelID
    if(DL__[i1].disp) continue;

    // get color from DL
    DL_col_dl (fCol, i1);
    glUniform4fv (shSUn.ulAtt, 1, fCol);

    // display obj; startIndex = vertxIndex (12 bytes per vertex)
    //  (vertexType, startIndex, nr-vertices-to-render)
    vtxNr = DL__[i1].vtxNr;

      // TESTBLOCK
      GL_test_VS1 (i1);
      // END TESTBLOCK


    glDrawArrays (GL_TRIANGLES, DL__[i1].glOff / 12, vtxNr);

  }

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

}


// EOF
