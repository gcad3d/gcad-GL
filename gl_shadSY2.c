/* ./gl_shadSY2.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Shader for symbols (2D-text, 3D-text, vectors, axes, dimensions, balloons ..)

=====================================================
List_functions_start:

GL_shSY2_add              add a data-record (symbol or fontChar) into buffer
GL_symbols_load           load symbols into buffer

GL_shadSY2_init
GL_shadSY2_render


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
#include "./geo.h"                     // Point Plane Mat_4x4D ..
#include "./matrix.h"
#include "./gr.h"                      // Att_ln GR_..
#include "./gl.h"                      // sh*
#include "./dl.h"                      // DL_..
#include "./col.h"                     // Att_4f_att
#include "./tst_prim1.h"               // Typ_*
#include "./gl_font1.h"                // GL_vfont1 SIZ_FONT_DAT
#include "./ut.h"                      // TX_Error

#define extern  // does eliminate "extern"
#include "./gl_shadSY2.h"              // 
#undef extern



//----------------------------------------------------------------
// vertexShader
char *shadSY2_src_vtx =
"#version 330 core\n\
uniform mat4 mvp;\
uniform vec3 off;\
in vec3 aVert;\
void main() {\
  gl_Position = mvp * vec4(aVert, 1.0);\
  gl_Position.x += off.x;\
}";


//----------------------------------------------------------------
/*
// geometryShader
char *shadSY2_src_geom =
"#version 330 core\n\
layout (points) in;\
layout (line_strip, max_vertices = 2) out;\
void main() {\
    gl_Position = gl_in[0].gl_Position;\
    EmitVertex();\
    gl_Position = gl_in[0].gl_Position;\
    gl_Position.x += 0.5f;\
    EmitVertex();\
  EndPrimitive();\
}";
*/

//----------------------------------------------------------------
// fragmentShader
char *shadSY2_src_frag =
"#version 330 core\n\
uniform vec3 att;\
out vec4 color;\
void main() {\
  color = vec4(att, 1.0f);\
}";


//================================================================
  int GL_shadSY2_init () {
//================================================================
// GL_shadSY2_init            create 2D-symbols-shader shadSY2

  int    irc, idPrg = 0, iSym;
  GLuint vertex, geometry, fragment, vao, vbo, bSiz, ulMvp, ulAtt, ulOff, iOff;


  printf("GL_shadSY2_init -----------\n");

  // create shader for surfaces
  irc = GL_shad_create (&vertex, shadSY2_src_vtx, GL_VERTEX_SHADER);
  if(irc < 0) exit(0);

//   irc = GL_shad_create (&geometry, shadSY2_src_geom, GL_GEOMETRY_SHADER);
//   if(irc < 0) exit(0);

  irc = GL_shad_create (&fragment, shadSY2_src_frag, GL_FRAGMENT_SHADER);
  if(irc < 0) exit(0);

  // combine shaders to Program
  // get empty program and program-ID
  idPrg = glCreateProgram ();
  glAttachShader (idPrg, vertex);
//   glAttachShader (idPrg, geometry);
  glAttachShader (idPrg, fragment);

  // link & check program
  irc = GL_prog_make (idPrg);
  if(irc < 0) return -1;


  //----------------------------------------------------------------
  // get unifom-record-index
  ulMvp = glGetUniformLocation (idPrg, "mvp");
  if(ulMvp == -1) printf(" ************ ERROR glGetUniformLocation mvp\n");

  // get unifom-record-index
  ulAtt = glGetUniformLocation (idPrg, "att");
  if(ulAtt == -1) printf(" ************ ERROR glGetUniformLocation att\n");

  // get unifom-record-index
  ulOff = glGetUniformLocation (idPrg, "off");
  if(ulOff == -1) printf(" ************ ERROR glGetUniformLocation off\n");


    printf(" shadSY2_init idPrg=%d ulMvp=%d ulAtt=%d ulOff=%d\n",
           idPrg,ulMvp,ulAtt,ulOff);


  //----------------------------------------------------------------
  // Generate 1 empty vertex array object; return its ID(s) in vao
  glGenVertexArrays (1, &vao);

  // Generate 1 empty buffer; return its ID(s) in buffer
  glGenBuffers (1, &vbo);

  // Set as current vertex array:
  glBindVertexArray (vbo);

  // bind named-bufferObj
  glBindBuffer (GL_ARRAY_BUFFER, vbo);

  // get space for SIZB_SY2 bytes
  bSiz = SIZB_SY2;
  // (type, size_in_bytes, data, usage)
  glBufferData (GL_ARRAY_BUFFER, bSiz, NULL, GL_STATIC_DRAW);

  shSY2.idVao = vao;
  shSY2.idVbo = vbo;
  shSY2.idPrg = idPrg;
  shSY2.ulMvp = ulMvp;
  shSY2.ulAtt = ulAtt;
  shSY2.ulOff = ulOff;
  shSY2.bSiz  = bSiz;
    printf(" shadSY2_init idPrg=%d vao=%d vbo=%d\n",
           shSY2.idPrg, shSY2.idVao, shSY2.idVbo);


  //----------------------------------------------------------------
  // load font1 into the buffer
  iSym = 0;
  iOff = 0;
    printf(" GL_vf1_CW=%f GL_vf1_CH=%f\n",GL_vf1_CW,GL_vf1_CH);

  irc = GL_font1_chr_load (&iSym, &iOff);
  if(irc < 0) return -1;

  // load symbols into buffer
  irc = GL_symbols_load (&iSym, &iOff);
  if(irc < 0) return -1;


  shSY2.iOff = iOff;


  //----------------------------------------------------------------
  glDetachShader (idPrg, vertex);
  glDetachShader (idPrg, fragment);

  irc = 0;

  //----------------------------------------------------------------


  L_out:
    glDeleteShader (vertex);
    glDeleteShader (fragment);

  L_exit:
    printf(" ex-GL_shadSY2_init irc=%d %d\n",irc, idPrg);
  return irc;

}


//================================================================
  int GL_symbols_load (int *iSym, int *iOff) {
//================================================================
// load symbols into buffer;

  int   irc, bNr;


  float sq1Dat[] = {   // GL_TRIANGLE_FAN
    0.f,  0.f,  0.f,
    0.f, -1.f,  0.f,
    1.f, -1.f,  0.f,
    1.f,  0.f,  0.f
  };


  printf("GL_symbols_load %d %d\n",*iSym,*iOff);

  // add iSym_tag = square 1 x 1 = sq1Dat into buffer
  iSym_tag = *iSym;
  bNr = sizeof(sq1Dat);
  irc = GL_shSY2_add (iSym, bNr, iOff, sq1Dat, 4);
  if(irc < 0) goto L_exit;



  irc = 0;

  L_exit:
  return irc;

}


//=======================================================================
  int GL_shSY2_add (int *iSym, int bNr, int *iOff, void *cvDat, int oNr) {
//=======================================================================
// add a data-record (symbol of fontChar) into buffer
// Input:
//   oNr     nr of objects (eg lines)

  int iOff2;


  // printf("GL_shSY2_add *iSym=%d bNr=%d iOff=%d oNr=%d\n",*iSym,bNr,*iOff,oNr);

 
  // check for buffer overflow
  iOff2 = *iOff + bNr;
  if(iOff2 >= shSY2.bSiz) {
    TX_Error("******* ERROR BUFFER OVERFLOW SHADER shSY2 -");
    TX_Error("******* TODO: get new bigger buffer, copy and delete buffer.");
    return -1;
  }


  // add cvDat to buffer
  glBufferSubData (GL_ARRAY_BUFFER, *iOff, bNr, cvDat);


  // store bufferOffset (in bytes), lineNr per char
  symTab[*iSym].iOff = *iOff;
  symTab[*iSym].oNr  = oNr;
    // printf(" load_font1-chr '%c' %d iOff=%d lNr=%d\n",i1+32,i1,iOff1,oNr);


  *iSym += 1;
  *iOff = iOff2;

  return 0;

}


//================================================================
  void GL_shadSY2_render (void) {
//================================================================
// Input:
//   fixed for all surfaces: prog_SU ulMvp, ulAtt,
//   variable:  bufNr

  int     i1, vtxTyp, vtxNr;
  float   fCol[4];
  Att_ln  actAtt, iatt;


  printf("GL_shadSY2_render idPrg=%d DL_nr=%d\n",shSY2.idPrg,DL_nr);


  // activate shaderProg shSY2
  glUseProgram (shSY2.idPrg);

  // set model-view-matrix
  M44F_init (mvpf);
    // M44FC_dump ((float*)mvpf, "shadSY2_render-mvpf");
  glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)mvpf);

  // bind the VAO, VBO
  glBindVertexArray (shSY2.idVao);
  glBindBuffer (GL_ARRAY_BUFFER, shSY2.idVbo);

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

      printf(" rend_SY2----- %d iOff=%d\n",i1,DL__[i1].bOff);

    // skip hidden obj    TODO: respect subModelID
    if(DL__[i1].disp) continue;

    // update width, color
    if(memcmp(&actAtt, &DL__[i1].iatt, sizeof(UINT_32))) {
      // activate modified width or color
      iatt = *(Att_ln*)&DL__[i1].iatt;
      glPointSize ((float)iatt.thick);
      glLineWidth ((float)iatt.thick / 2.f);
      Att_4f_att (fCol, &iatt);
      glUniform3fv (shSY2.ulAtt, 1, fCol);
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
  // close - disactivate
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glBindVertexArray (0);  // clear active buffer
  glUseProgram (0);     // disactivate active program


  //----------------------------------------------------------------
// disp indexed array:
//   // (lineMode, nr, typDatInd, indArr)
//   glDrawElements (GL_LINE_STRIP, n, GL_UNSIGNED_SHORT, indices);

}


//================================================================
  int GL_font1_chr_load (int *iSym, int *iOff) {
//================================================================
// first char is blank (32), last char is plusMinus (128)
// - font is 3D GL_LINES in 3D (z-coord = 0);
// - each char has 0 or 1 or more lines;
// - bufferindex of a charcter is <char> - 32; for 'A': iBuf = 'A' - 32;
// - bufferOffset for this char (in bytes): bOff = symTab[i1].iOff;
// - nr-of-lines for this char: lNr =  symTab[i1].lNr          
// 
// load fontcharacters 0 (blank) - 93 ('}')
// - bufferindex Diameter  is char 126
// - bufferindex Degree    is char 127
// - bufferindex plusMinus is char 128
// see ./gl_font1.h

 
  int     irc, i1, ie, lNr, fNr, iOff1, iOff2, bNr;
  float   cvDat[SIZ_FONT_DAT];


  printf("GL_load_font1 %d %d\n",*iSym,*iOff);

  iOff1 = *iOff;

  // ie = *iSym + 97;


  L_nxt:
    // load <lNr> lines for char i1
    irc = font1_load_char (cvDat, &lNr, *iSym);
    if(irc > 0) goto L_done;  // normal end
    if(irc < 0) return -1;

    fNr = lNr * 6; // floats (1 line = 6 floats = 24 bytes)
    bNr = fNr * 4; // bytes

    if(lNr < 1) {
      // no line
      // ..
      *iSym += 1;
      goto L_nxt;
    }

    // add cvDat into buffer
    irc = GL_shSY2_add (iSym, bNr, &iOff1, cvDat, lNr);
    if(irc < 0) goto L_exit;

    // if(*iSym < ie) 
    goto L_nxt;


  L_done:
    irc = 0;

  L_exit:
    // printf(" ex-GL_font1_chr_load irc=%d iSym=%d iOff=%d\n",irc,*iSym,shSY2.iOff);
  return 0;

}


//================================================================
  int GL_txt_2D (float *pos0, char *txt, float offC) {
//================================================================
// display string 2D (always same size, normal to user)

  int   ic;
  static float pos1[3] = {0.f, 0.f, 0.f};


  // printf("GL_txt_2D |%s| pos=%f offC=%f\n",txt,*pos0,offC);

  pos1[0] = *pos0;

  // loop tru string - get ic = index of char in symTab
  int sl = strlen(txt);

  // float offP = 0.f;  // offset position

  for(int is=0; is<sl; ++is) {
    ic = txt[is] - 32; // get index for next char
      // printf(" txt_2D-ic=%d '%c' iOff=%d lNr=%d\n",ic,ic+32,
             // symTab[ic].iOff,symTab[ic].oNr);

    // provide the positionOffest in uniform
    glUniform3fv (shSY2.ulOff, 1, pos1);

    // do shadSY2
    glDrawArrays (GL_LINES, symTab[ic].iOff / 12, symTab[ic].oNr * 2);

    pos1[0] += offC;
  }

  *pos0 = pos1[0];

  return 0;

}


// EOF
