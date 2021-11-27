/* ./gl_shCV.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Shader for points and curves

=====================================================
List_functions_start:

GL_shCV_init
GL_shCV_render

GL_font1_load
GL_shCV_add_chr

List_functions_end:
=====================================================

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include <GL/gl.h>

#include "../ut/geo.h"                     // Point Plane Mat_4x4D ..
#include "../gr/gr.h"                      // Att_ln GR_..
#include "../gl/gl.h"                      // shCV
#include "../dl/dl.h"                      // DL_..
#include "../dl/dl_sym_perm.h"                  // 
#include "../gr/col.h"                     // Att_4f_att
#include "../gl/gl_font1.h"                // GL_vfont1 SIZ_FONT_DAT

#define extern  // does eliminate "extern"
#include "../gl/gl_shCV.h"              // 
#undef extern

#include "../app/app.h"               // Typ_*



//----------------------------------------------------------------
// vertexShader
char *shCV_src_vtx =
"#version 330 core\n\
in vec3 aVert;\
uniform mat4 mat3D;\
uniform vec4 aColor;\
out vec4 fColor;\
void main() {\
  gl_Position = mat3D * vec4(aVert, 1.0);\
  fColor = aColor;\
}";


//----------------------------------------------------------------
// fragmentShader
char *shCV_src_frag =
"#version 330 core\n\
in vec4 fColor;\
out vec4 pColor;\
void main() {\
  pColor = fColor;\
}";


//================================================================
  int GL_shCV_init () {
//================================================================
// GL_shCV_init            create points/curves shader shCV

  int    irc, program = 0, iSym;
  GLuint vertex, fragment, vao, vbo, bSiz,  ulMvp, ulAtt, iOff;


  printf("GL_shCV_init -----------\n");

  // create shader for surfaces
  irc = GL_shad_create (&vertex, shCV_src_vtx, GL_VERTEX_SHADER);
  if(irc < 0) exit(0);
  irc = GL_shad_create (&fragment, shCV_src_frag, GL_FRAGMENT_SHADER);
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
  ulMvp = glGetUniformLocation (program, "mat3D");
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

  // get space
  bSiz = SIZB_CV;

  // (type, size_in_bytes, data, usage)
  glBufferData (GL_ARRAY_BUFFER, bSiz, NULL, GL_STATIC_DRAW);

  shCV.idVao = vao;
  shCV.idVbo = vbo;
  shCV.idPrg = program;
  shCV.ulMvp = ulMvp;
  shCV.ulAtt = ulAtt;
  shCV.bSiz = bSiz;
  shCV.iOff = 0;
    printf(" shCV_init idPrg=%d vao=%d vbo=%d\n",
           shCV.idPrg, shCV.idVao, shCV.idVbo);


  glDetachShader (program, vertex);
  glDetachShader (program, fragment);

  irc = 0;

  //----------------------------------------------------------------
  // load font1 into the buffer
  iSym = 0;
  iOff = 0;
    // printf(" GL_vf1_CW=%f GL_vf1_CH=%f\n",GL_vf1_CW,GL_vf1_CH);
  // load characters into active buffer
  irc = GL_font1_load (&iSym, &iOff);
  if(irc < 0) return -1;

  shCV.iOff = iOff;


  //----------------------------------------------------------------

  L_out:
    glDeleteShader (vertex);
    glDeleteShader (fragment);

  L_exit:
    printf(" ex-GL_shCV_init irc=%d %d\n",irc, program);
  return irc;

}


//================================================================
  void GL_shCV_render (void) {
//================================================================
// Input:
//   fixed for all surfaces: prog_SU ulMvp, ulAtt,
//   variable:  bufNr

  int     i1, vtxTyp, vtxNr;
  float   fCol[4];
  Att_ln  actAtt, iatt;


  printf("GL_shCV_render idPrg=%d DL_nr=%d\n",shCV.idPrg,DL_nr);

  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_POLYGON_SMOOTH);
  glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //----------------------------------------------------------------
  // activate shaderProg shCV
  glUseProgram (shCV.idPrg);

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

  // set model-view-matrix
  glUniformMatrix4fv (shCV.ulMvp, 1, GL_FALSE, (float*)GR_matf_dev);

  actAtt = Att_ln_NUL;

  for(i1=0; i1<DL_nr; ++i1) {
    // skip all but points/curves (Typ_PT, Typ_CV)
    if(DL__[i1].typ > Typ_CV) continue;

      printf(" rend_CV----- %d iOff=%d\n",i1,DL__[i1].glOff);

    // skip hidden obj    TODO: respect subModelID
    if(DL__[i1].disp) continue;

    // update width, color
    if(memcmp(&actAtt, &DL__[i1].iatt, sizeof(UINT_32))) {
      // activate modified width or color
      iatt = *(Att_ln*)&DL__[i1].iatt;
        Att_dump (&iatt, "rend_CV-iatt");
      glPointSize ((float)iatt.thick + 2.f);
      glLineWidth ((float)iatt.thick);
      Att_4f_att (fCol, &iatt);
      glUniform4fv (shCV.ulAtt, 1, fCol);
    }

    // display obj; startIndex = vertxIndex (12 bytes per vertex)
    //  (vertexType, startIndex, nr-vertices-to-render)
    if(DL__[i1].typ < Typ_CV) {
      glDrawArrays (GL_POINTS, DL__[i1].glOff / 12, 1);

    } else {
      // display obj; startIndex = vertxIndex (12 bytes per vertex)
      //  (vertexType, startIndex, nr-vertices-to-render)
      // vtxNr = (DL__[i1+1].glOff - DL__[i1].glOff) / 12;
      vtxNr = DL__[i1].vtxNr;
      glDrawArrays (GL_LINE_STRIP, DL__[i1].glOff / 12, vtxNr);
    }


  }

  // glDisable (GL_LINE_SMOOTH);

  //================================================================
  GL_SYP_render ();  // render perm-symbols


  //================================================================
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


//================================================================
  void GL_SYP_render () {
//================================================================
// GL_SYP_render         render perm-symbols

  int    i1, iSym;

  printf("GL_SYP_render () :::::::::::::::::::::::::\n");
  DL_SYP_dump ();

  for(i1=0; i1<DL_SYP_nr; ++i1) {
    iSym = DL_SYP_tab[i1].oTyp;
      printf(" SYP_render----- %d iSym=%d\n",i1,iSym);

      // display obj; startIndex = vertxIndex (12 bytes per vertex)
      //  (vertexType, startIndex, nr-vertices-to-render)
      // vtxNr = (DL__[i1+1].glOff - DL__[i1].glOff) / 12;
      glDrawArrays (DL_SYP_tab[i1].lTyp,
                    DL_SYP_tab[i1].iOff / 12,
                    DL_SYP_tab[i1].vtxNr);



  }

  return;

}


//================================================================
  int GL_font1_load (int *iSym, int *iOff) {
//================================================================
// GL_font1_chr_load     load vectorfont into buffer
// first char is blank (32), last char is plusMinus (128)
// - font is 3D GL_LINES in 3D (z-coord = 0);
// - each char has 0 or 1 or more lines;
// - bufferindex of a charcter is <char> - 32; for 'A': iBuf = 'A' - 32;
// - bufferOffset for this char (in bytes): bOff = chrTab[i1].iOff;
// - nr-of-lines for this char: lNr =  chrTab[i1].lNr          
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
    irc = GL_shCV_add_chr (iSym, bNr, &iOff1, cvDat, GL_LINES, lNr * 2);
    if(irc < 0) goto L_exit;

    // if(*iSym < ie) 
    goto L_nxt;


  L_done:
    *iOff = iOff1;
    irc = 0;

  L_exit:
    printf(" ex-GL_font1_load irc=%d iSym=%d iOff=%d\n",irc,*iSym,shSY2.iOff);
  return 0;

}


//=======================================================================
  int GL_shCV_add_chr (int *iSym, int bNr, int *iOff, void *cvDat,
                        int lTyp, int oNr) {
//=======================================================================
// add a data-record (symbol of fontChar) into buffer
// Input:
//   oNr     nr of vertexes

  int iOff2;


  // printf("GL_shCV_add_chr *iSym=%d bNr=%d iOff=%d oNr=%d\n",*iSym,bNr,*iOff,oNr);


  // check for buffer overflow
  iOff2 = *iOff + bNr;
  if(iOff2 >= shCV.bSiz) {
    TX_Error("******* ERROR BUFFER OVERFLOW SHADER shCV -");
    TX_Error("******* TODO: get new bigger buffer, copy and delete buffer.");
    return -1;
  }


  // add cvDat to buffer
  glBufferSubData (GL_ARRAY_BUFFER, *iOff, bNr, cvDat);


  // store bufferOffset (in bytes), lineNr per char
  chrTab[*iSym].iOff = *iOff;
  chrTab[*iSym].lTyp = lTyp;
  chrTab[*iSym].oNr  = oNr;
    // printf(" load_font1-chr '%c' %d iOff=%d lNr=%d\n",i1+32,i1,iOff1,oNr);


  *iSym += 1;
  *iOff = iOff2;

  if(*iSym >= SIZ_CHRTAB) {
    TX_Error ("******* ERROR BUFFER OVERFLOW SHADER shSY2 - chrTab");
    return -1;
  }


  return 0;

}


// EOF
