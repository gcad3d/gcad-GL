/* ./gl_shSY2.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Shader for symbols (2D-text, 3D-text, vectors, axes, dimensions, balloons ..)

=====================================================
List_functions_start:

GL_shSY2_add_sym          add a data-record (symbol or fontChar) into buffer
GL_shSY2_add_cv           add a curve

GL_shSY2_init
GL_shSY2_render
GL_symbols_load           load symbols into buffer
GL_font1_chr_load         load vectorfont into buffer
GL_shSY2_add_cv           load polygon
GL_shSY2_axis              display axis-systems

GL_txt_2D                display string 2D (always same size, normal to user)

List_functions_end:
=====================================================

See INF_symbols

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include <GL/gl.h>

#include "../ut/geo.h"                     // Point Plane Mat_4x4D ..
#include "../ut/deb.h"                     // DEB_dump_..
#include "../ut/matrix.h"
#include "../gr/gr.h"                      // Att_ln GR_..
#include "../gl/gl.h"                      // sh*
#include "../dl/dl.h"                      // DL_..
#include "../dl/dl_sym_dyn.h"                  // DLdyn..
#include "../gr/col.h"                     // Att_4f_att
#include "../app/app.h"                    // Typ_*
#include "../gl/gl_font1.h"                // GL_vfont1 SIZ_FONT_DAT

#define extern  // does eliminate "extern"
#include "../gl/gl_shSY2.h"              // 
#undef extern

// startpos dynamic objects, (after symbols); reset to iOff_base at new-model
static int iOff_base;



//----------------------------------------------------------------
// vertexShader
char *shSY2_src_vtx =
"#version 330 core\n\
uniform mat4 mat3D;\
uniform vec3 off;\
in vec3 aVert;\
void main() {\
  gl_Position = mat3D * vec4(aVert, 1.0);\
  gl_Position.x += off.x;\
  gl_Position.y += off.y;\
}";


//----------------------------------------------------------------
/*
// geometryShader
char *shSY2_src_geom =
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
char *shSY2_src_frag =
"#version 330 core\n\
uniform vec3 att;\
out vec4 color;\
void main() {\
  color = vec4(att, 1.0f);\
}";


//================================================================
  int GL_shSY2_init () {
//================================================================
// GL_shSY2_init            create 2D-symbols-shader shSY2

  int    irc, idPrg = 0, iSym;
  GLuint vertex, geometry, fragment, vao, vbo, bSiz, ulMvp, ulAtt, ulOff, iOff;


  printf("GL_shSY2_init -----------\n");

  // create shader for surfaces
  irc = GL_shad_create (&vertex, shSY2_src_vtx, GL_VERTEX_SHADER);
  if(irc < 0) exit(0);

//   irc = GL_shad_create (&geometry, shSY2_src_geom, GL_GEOMETRY_SHADER);
//   if(irc < 0) exit(0);

  irc = GL_shad_create (&fragment, shSY2_src_frag, GL_FRAGMENT_SHADER);
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
  ulMvp = glGetUniformLocation (idPrg, "mat3D");
  if(ulMvp == -1) printf(" ************ ERROR glGetUniformLocation mvp\n");

  // get unifom-record-index
  ulOff = glGetUniformLocation (idPrg, "off");
  if(ulOff == -1) printf(" ************ ERROR glGetUniformLocation off\n");

  // get unifom-record-index
  ulAtt = glGetUniformLocation (idPrg, "att");
  if(ulAtt == -1) printf(" ************ ERROR glGetUniformLocation att\n");


    printf(" shSY2_init idPrg=%d ulMvp=%d ulAtt=%d ulOff=%d\n",
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
    printf(" shSY2_init idPrg=%d vao=%d vbo=%d\n",
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
    printf(" ex-GL_shSY2_init irc=%d %d\n",irc, idPrg);
  return irc;

}


//================================================================
  int GL_symbols_load (int *iSym, int *iOff) {
//================================================================
// load symbols into buffer;

  int   irc, bNr;
  float fa[24], d1, d2, d3, d4;


  printf("GL_symbols_load %d %d\n",*iSym,*iOff);


  //----------------------------------------------------------------
  // add iSym_pt - point
  iSym_pt = *iSym;
  fa[ 0]=0.f;  fa[ 1]=0.f;  fa[ 2]=0.f;
  bNr = 3 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_POINTS, 1);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  // add iSym_vcx = line length 1 =  VC_X;
  iSym_vcx = *iSym;
  fa[ 0]=0.f;  fa[ 1]=0.f;  fa[ 2]=0.f;
  fa[ 3]=1.f;  fa[ 4]=0.f;  fa[ 5]=0.f;

  bNr = 6 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_LINES, 2);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  // add iSym_vcy = line length 1 =  VC_Y;
  iSym_vcy = *iSym;
  fa[ 3]=0.f;  fa[ 4]=1.f;  fa[ 5]=0.f;

  bNr = 6 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_LINES, 2);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  // add iSym_vcx = line length 1 =  VC_Z;
  iSym_vcz = *iSym;
  fa[ 3]=0.f;  fa[ 4]=0.f;  fa[ 5]=1.f;

  bNr = 6 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_LINES, 2);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  // add iSym_squ = square 1 x 1 starting at low left corner
  iSym_squ = *iSym;
  d1 = 1.;
    printf(" iSym_pln %d\n",iSym_pln);

  fa[ 0]=0.f;  fa[ 1]=0.f;  fa[ 2]=0.f;
  fa[ 3]=d1;   fa[ 4]=0.f;  fa[ 5]=0.f;
  fa[ 6]=d1;   fa[ 7]=d1;   fa[ 8]=0.f;
  fa[ 9]=0.f;  fa[10]=d1;   fa[11]=0.f;

  bNr = 12 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_LINE_LOOP, 4);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  // add iSym_pln = square 1 x 1 in centerpos
  iSym_pln = *iSym;
  d1 = 1.;
  d2 = d1 / 2.;
    printf(" iSym_pln %d\n",iSym_pln);

  fa[ 0]=-d2;  fa[ 1]=-d2;  fa[ 2]=0.f;
  fa[ 3]=d2;   fa[ 4]=-d2;  fa[ 5]=0.f;
  fa[ 6]=d2;   fa[ 7]=d2;   fa[ 8]=0.f;
  fa[ 9]=-d2;  fa[10]=d2;   fa[11]=0.f;

  bNr = 12 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_LINE_LOOP, 4);
  if(irc < 0) goto L_exit;



  //----------------------------------------------------------------
  // iSym_tria = triangle
  iSym_tria = *iSym;
  d1 = 1.;
  d2 = d1 * SR_3 / 6.;
  d3 = d1 / 2.;
  d4 = 2. * d2;
    printf(" iSym_tria %d = %f %f %f\n",iSym_tria,d2,d3,d4);
//
//       /|
//      / | d4
//     /  +
//    /   | d2
//   +----+
//     d3

  fa[ 0]=-d3;  fa[ 1]=-d2;  fa[ 2]=0.f;
  fa[ 3]=d3;   fa[ 4]=-d2;  fa[ 5]=0.f;
  fa[ 6]=0.f;  fa[ 7]=d4;   fa[ 8]=0.f;
  fa[ 9]=-d3;  fa[10]=-d2;  fa[11]=0.f;

  bNr = 12 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_LINE_STRIP, 4);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  // iSym_star
  iSym_star = *iSym;
  d1 = 1.;
  d2 = d1 * SR_3 / 4.;
  d3 = d1 / 2.;
  d4 = d1 / 4.;
    printf(" iSym_star %d = %f %f %f\n",iSym_star,d2,d3,d4);
//
//      . d2  x         .
//            |
//      x     |     x   d3
//      .  x  |  x
//            x         .
//      .  x  |  x
//      x     |     x
//   p5       |      p0
//            x         
//           p3

  fa[ 0]=d2;   fa[ 1]=-d4;  fa[ 2]=0.f;    // p0
  fa[ 3]=-d3;  fa[ 4]=d4;   fa[ 5]=0.f;
  fa[ 6]=0.f;  fa[ 7]=-d3;  fa[ 8]=0.f;    // p3
  fa[ 9]=0.f;  fa[10]=d3;   fa[11]=0.f;
  fa[12]=-d2;  fa[13]=-d4;  fa[14]=0.f;    // p5
  fa[15]=d2;   fa[16]=d4;   fa[17]=0.f;

  bNr = 18 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_LINES, 6);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  // 2D-arrowhead
  iSym_arh2 = *iSym;
  d1 = 1.;
  d2 = d1 / 3.;
    printf(" iSym_arh2 %d = %f\n",iSym_arh2,d2);

  fa[ 0]=d1;   fa[ 1]=-d2;  fa[ 2]=0.f;  // p0
  fa[ 3]=0.f;  fa[ 4]=0.f;  fa[ 5]=0.f;  // p3
  fa[ 6]=d1;   fa[ 7]=d2;   fa[ 8]=0.f;  // p1

  bNr = 9 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_LINE_STRIP, 3);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  // 3D-arrowhead
  iSym_arh3 = *iSym;
  d1 = 1.;
  d2 = d1 / 4.;
  d3 = d2 / 2.;
  d4 = d3 * SR_3;
    printf(" iSym_arh3 %d = %f %f %f\n",iSym_arh3,d2,d3,d4);

  fa[ 0]=0.f;  fa[ 1]=0.f;  fa[ 2]=0.f;  // 1 p0
  fa[ 3]=d1;   fa[ 4]=d4;   fa[ 5]=-d3;  // 2 p3
  fa[ 6]=d1;   fa[ 7]=0.f;  fa[ 8]=0.f;  // 3 p1
  fa[ 9]=d1;   fa[10]=0.f;  fa[11]=d2;   // 4 p2
  fa[12]=0.f;  fa[13]=0.f;  fa[14]=0.f;  // 5 p0
  fa[15]=d1;   fa[16]=-d4;  fa[17]=-d3;  // 6 p4
  fa[18]=d1;   fa[19]=0.f;  fa[20]=0.f;  // 7 p1

  bNr = 21 * sizeof(float);
  irc = GL_shSY2_add_sym (iSym, bNr, iOff, fa, GL_LINE_STRIP, 7);
  if(irc < 0) goto L_exit;


  //----------------------------------------------------------------
  iOff_base = *iOff;   // first pos for dynamic objects ...
  irc = 0;

  L_exit:
  return irc;

}


//=======================================================================
  int GL_shSY2_add_sym (int *iSym, int bNr, int *iOff, void *cvDat,
                        int lTyp, int oNr) {
//=======================================================================
// add a data-record (symbol of fontChar) into buffer
// Input:
//   oNr     nr of vertexes

  int iOff2;


  // printf("GL_shSY2_add_sym *iSym=%d bNr=%d iOff=%d oNr=%d\n",*iSym,bNr,*iOff,oNr);

 
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
  symTab[*iSym].lTyp = lTyp;
  symTab[*iSym].oNr  = oNr;
    // printf(" load_font1-chr '%c' %d iOff=%d lNr=%d\n",i1+32,i1,iOff1,oNr);


  *iSym += 1;
  *iOff = iOff2;

  if(*iSym >= SIZ_SYMTAB) {
    TX_Error ("******* ERROR BUFFER OVERFLOW SHADER shSY2 - symTab");
    return -1;
  }


  return 0;

}


//================================================================
  void GL_shSY2_render (void) {
//================================================================
// Input:
//   fixed for all surfaces: prog_SU ulMvp, ulAtt,
//   variable:  bufNr

  int      i1, vtxTyp, iSym;
  float    fCol[4], scale;
  double   d1, d2, d3;
  Att_ln   actAtt, iatt;
  Point    pt0, pt1, pt2, pt3;
  Vector   vc1;
  float pos1[3], pos2[3];
  Mat_4x4D mx3D[10], mdm;
  Mat_4x4F mx2F, mx3F;


  printf("GL_shSY2_render idPrg=%d DL_SYD_nr=%d\n",shSY2.idPrg,DL_SYD_nr);
  if(!DL_SYD_nr) DL_SYD_dump ();

  if(!DL_SYD_nr) return;

  //----------------------------------------------------------------
  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_POLYGON_SMOOTH);
  glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //----------------------------------------------------------------
  // activate shaderProg shSY2
  glUseProgram (shSY2.idPrg);

  // bind the VAO, VBO
  glBindVertexArray (shSY2.idVao);
  glBindBuffer (GL_ARRAY_BUFFER, shSY2.idVbo);

  glEnableVertexAttribArray(0);
  // (index-vertex-attrib, nr-components, format, do-normalize, stride, offset)
// POINTS: 3 floats
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  // glDisable (GL_BLEND);
  // glEnable (GL_LINE_SMOOTH);

  //================================================================
  // loop tru all objs in this buffer
  actAtt = Att_ln_NUL;

  for(i1=0; i1<DL_SYD_nr; ++i1) {
    iSym = DL_SYD_tab[i1].oTyp;
      printf(" rend_SY2----- %d iSym=%d\n",i1,iSym);


    // export width, color
    if(memcmp(&actAtt, &DL_SYD_tab[i1].iatt, sizeof(UINT_32))) {
      // activate modified width or color
      iatt = *(Att_ln*)&DL_SYD_tab[i1].iatt;
        // Att_dump (&iatt, " SY2_render-att");
      // glPointSize ((float)iatt.thick * 2.f);
      glLineWidth ((float)iatt.thick);
      Att_4f_att (fCol, &iatt);
      glUniform3fv (shSY2.ulAtt, 1, fCol);
    }


    scale = DL_SYD_tab[i1].size;
        printf(" rend_SY2-siz=%f aZ=%f aX=%f\n",scale,GR_angZ,GR_angX);


    //----------------------------------------------------------------
    if(iSym > 0) goto L_rend_sym;

    //================================================================
    // render VecPos (Vector with position)

      // set and export matrix;
      if(scale != 0.f) {
        // vector normalized-length
        // display line iSym_vcx scaled, translated, tilted, rotated

        // export offsets 
        glUniform3f (shSY2.ulOff, 0.f, 0.f, 0.f);

        // scale to constant length
        d1 = scale / GR_Scale * 5.5;     // ??
        M44D_scale__ ((void*)mx3D[1], d1, d1, d1);     // scale 

        // tilt
        d1 = DL_SYD_tab[i1].ay;
          printf(" shSY2_render-VecPos-az=%f ay=%f\n",d2,d1);
        M44D_rot_Y (mx3D[3], d1);
        M44D_mult__ (mx3D[5], mx3D[1], mx3D[3]);       // scal+tilt

        // rot
        d2 = DL_SYD_tab[i1].az + RAD_180;
        M44D_rot_Z (mx3D[4], d2);
        M44D_mult__ (mx3D[6], mx3D[4], mx3D[5]);       // +rot

        // translate -> startposition of vector
        // input = startPos of vec in WCS
        pt0 = DL_SYD_tab[i1].pos;
        M44D_transl_3 (mx3D[2], pt0.x, pt0.y, pt0.z);
        M44D_mult__ (mx3D[7], mx3D[2], mx3D[6]);       // +transl

// get pt2 = endPt vector
pt1.x = 1.; pt1.y = 0.; pt1.z = 0.;
M44D_pt_mult_m44 (&pt2, mx3D[7], &pt1);

        // add ortho-view
        M44D_mult__ (mx3D[8], GR_matd_view, mx3D[7]);


        // export matrix
        M44FC__m44dr (mx3F, mx3D[8]);
        glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)mx3F);

        // disp
        glDrawArrays (symTab[iSym_vcx].lTyp,
                      symTab[iSym_vcx].iOff / 12, symTab[iSym_vcx].oNr);


      } else {
        // vector true-length
        pos1[0] = 0.f; pos1[1] = 0.f; pos1[2] = 0.f;
        glUniform3fv (shSY2.ulOff, 1, pos1); // export offsets 

        // export matrix
        glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)GR_matf_dev);

        // disp vector in sgSY2-buffer
        glDrawArrays (DL_SYD_tab[i1].lTyp, DL_SYD_tab[i1].iOff / 12, DL_SYD_tab[i1].vtxNr);
      }


      //----------------------------------------------------------------
      // disp arrowhead
      ++i1;

      if(scale != 0.f) {
        // normalized-length

        // endPt vec = startPt arrowHead
        pos2[0] = 0.f; pos2[1] = 0.f; pos2[2] = 0.f;
        glUniform3fv (shSY2.ulOff, 1, pos2); // export offsets 

        // scale to constant length
        d1 = 25. / GR_Scale;
        M44D_scale__ ((void*)mx3D[1], d1, d1, d1);     // scale 

        // tilt
        M44D_rot_Y (mx3D[2], -DL_SYD_tab[i1].ay);
        M44D_mult__ (mx3D[3], mx3D[1], mx3D[2]);  // scal+tilt

        // rotate around Z-axis
        M44D_rot_Z (mx3D[4], DL_SYD_tab[i1].az);
        M44D_mult__ (mx3D[5], mx3D[4], mx3D[3]);

        // translate -> endPt
        M44D_transl__ (mx3D[6], &pt2);
        M44D_mult__ (mx3D[7], mx3D[6], mx3D[5]);

        // add ortho-view
        M44D_mult__ (mx3D[8], GR_matd_view, mx3D[7]);

        // export matrix
        M44FC__m44dr (mx3F, mx3D[8]);                // float from double
        glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)mx3F);


      } else {
        // true-length
        pos2[0] = 0.f; pos2[1] = 0.f; pos2[2] = 0.f;
        glUniform3fv (shSY2.ulOff, 1, pos2); // export offsets 

        d1 =  25. / GR_Scale;                 // normalize arrowhead
        M44D_scale__ ((void*)mx3D[1], d1, d1, d1);
        // M44D_mult__ (mx3D[6], mx3D[4], mx3D[5]);

        // tilt
        M44D_rot_Y (mx3D[2], -DL_SYD_tab[i1].ay);  
        M44D_mult__ (mx3D[3], mx3D[1], mx3D[2]);  // scal+tilt

        // rotate around Z-axis
        M44D_rot_Z (mx3D[4], DL_SYD_tab[i1].az);
        M44D_mult__ (mx3D[5], mx3D[4], mx3D[3]);

        // translate -> endPt
        M44D_transl__ (mx3D[6], &DL_SYD_tab[i1].pos);
        M44D_mult__ (mx3D[7], mx3D[6], mx3D[5]);

        // add ortho-view
        M44D_mult__ (mx3D[8], GR_matd_view, mx3D[7]);

        // export matrix
        M44FC__m44dr (mx3F, mx3D[8]);                // float from double
        glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)mx3F);

      }

      // disp symbol 3d-arrowhead
      glDrawArrays (symTab[iSym_arh3].lTyp,
                    symTab[iSym_arh3].iOff / 12, symTab[iSym_arh3].oNr);

      continue;


//     //================================================================
//     // render curve
// 
//     // export matrix (scale) 
//     glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)GR_matf_dev);
// 
//     // export offset
//     pos1[0] = 0.f;
//     pos1[1] = 0.f;
//     glUniform3fv (shSY2.ulOff, 1, pos1);
// 
//     // display
//     glDrawArrays (DL_SYD_tab[i1].lTyp, DL_SYD_tab[i1].iOff / 12, DL_SYD_tab[i1].vtxNr);
//     continue;



    //================================================================
    L_rend_sym:     
      // render symbol (SYM_TRIA, SYM_PLN, ..)

      // get pt1 = startposition in device-coords from world-coords
      M44D_pt_mult_m44 (&pt1, GR_matd_view, &DL_SYD_tab[i1].pos);
      // set as offset for 2D-matrix
      pos1[0] = pt1.x;
      pos1[1] = pt1.y;
      pos1[2] = pt1.z;
        DEB_dump_pt (&pt1, "  rend_SY2-pos");
      glUniform3fv (shSY2.ulOff, 1, pos1); // export offsets 

      // export matrix (scale)
      d1 = GR_sizX_mm_ndc * DL_SYD_tab[i1].size;
      d2 = GR_sizY_mm_ndc * DL_SYD_tab[i1].size;
      M44FC_scale__ ((void*)mx2F, d1, d2, -d1);
      glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)mx2F);

          printf(" rend_SY2 %d sym=%d iOff=%d vtxNr=%d\n",i1,
                  iSym,symTab[iSym].iOff, symTab[iSym].oNr);


      // points: set size
      if(iSym == iSym_pt) {
        glPointSize (DL_SYD_tab[i1].size);
      }

      // display obj; startIndex = vertxIndex (12 bytes per vertex)
      //  (vertexType, startIndex, nr-vertices-to-render)
      // glDrawArrays (DL_SYD_tab[i1].lTyp,
      glDrawArrays (symTab[iSym].lTyp,
                    symTab[iSym].iOff / 12, symTab[iSym].oNr);

  }


  //================================================================
  // close - disactivate
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glBindVertexArray (0);  // clear active buffer
  glUseProgram (0);     // disactivate active program


  //----------------------------------------------------------------
  glDisable(GL_BLEND);


  //----------------------------------------------------------------
// disp indexed array:
//   // (lineMode, nr, typDatInd, indArr)
//   glDrawElements (GL_LINE_STRIP, n, GL_UNSIGNED_SHORT, indices);

}


//================================================================
  int GL_shSY2_add_cv (Point *pta, int ptNr) {
//================================================================
// GL_shSY2_add_cv               load polygon

  int    oSiz, iOff;


  // size of vtxNr points (3 floats)
  oSiz = ptNr * sizeof(float) * 3;

  // Set as current vertex array:
  glBindVertexArray (shSY2.idVao);

  // bind named-bufferObj
  glBindBuffer (GL_ARRAY_BUFFER, shSY2.idVbo);

  //----------------------------------------------------------------
  // check for buffer overflow
  iOff = shSY2.iOff;
  shSY2.iOff += oSiz;
  if(shSY2.iOff >= shSY2.bSiz) {
    TX_Error("******* ERROR BUFFER OVERFLOW SHADER GL_shSY2_add_cv");
    TX_Error("******* TODO: get new bigger buffer, copy and delete buffer.");
    return -1;
  }

  // Upload vertex data
  // load data (target, offset_in_bytes, size, data)
  GL_buffer_f_add_d (iOff, oSiz, (double*)pta);

  //----------------------------------------------------------------
  // disactivate active buffer and VAO 
  // glBindBuffer (GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

    printf(" ex-GL_shSY2_add_cv ptNr=%d\n",ptNr);

  return iOff;

}



//================================================================
 int GL_shSY2_axis () {
//================================================================
// GL_shSY2_axis              display axis-systems

  float    pos1[3] = {0.f, 0.f, 0.f}; //, scale, sclX, sclY;
  double   scale, sclX, sclY;
  Mat_4x4D mxdv, mxds, mxdr;
  Mat_4x4F mx2D, mxfv;


  printf("GL_shSY2_axis \n");
  printf(" GR_sizX_mm_ndc=%f GR_sizY_mm_ndc=%f\n",GR_sizX_mm_ndc,GR_sizY_mm_ndc);


  // activate shaderProg shSY2
  glUseProgram (shSY2.idPrg);

  // bind the VAO, VBO
  glBindVertexArray (shSY2.idVao);
  glBindBuffer (GL_ARRAY_BUFFER, shSY2.idVbo);

  glEnableVertexAttribArray(0);
  // (index-vertex-attrib, nr-components, format, do-normalize, stride, offset)
// POINTS: 3 floats
  glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);


  //----------------------------------------------------------------
  glDisable (GL_DEPTH_TEST);   // 2D only

  glEnable (GL_LINE_SMOOTH);
  glEnable (GL_POLYGON_SMOOTH);
  glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glLineWidth ((float)1.8f);   // maximum is 10

  //----------------------------------------------------------------
  // disp axes
  // set model-view-matrix
  scale = 10.;    // size in mm
  sclX = GR_sizX_mm_ndc * scale;
  sclY = GR_sizY_mm_ndc * scale;

  // get scaling-matrix
  M44D_scale__ (mxds, sclX, sclY, 1.f);

  // get complete matrix (scale + rot + tilt)
  M44D_mult__ (mxdv, mxds, GR_matd_mdl[5]);

  // double-row -> float-column
  M44FC__m44dr (mxfv, mxdv);

  // set position = screenCenter in screenCoords
  glUniform3fv (shSY2.ulOff, 1, pos1);

  // provide color
  glUniform3fv (shSY2.ulAtt, 1, COL_RED);

  // export matrix
  glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)mxfv);

  // display - 3 lines = 6 vertexes
  glDrawArrays (symTab[iSym_vcx].lTyp, symTab[iSym_vcx].iOff / 12, 2);
  glDrawArrays (symTab[iSym_vcx].lTyp, symTab[iSym_vcy].iOff / 12, 2);
  glDrawArrays (symTab[iSym_vcx].lTyp, symTab[iSym_vcz].iOff / 12, 2);


  //----------------------------------------------------------------
  // disp 'x' and 'z'
  // get 2D-matrix
  // get size of 1 char in device-coords (-1, to +1)
  sclX = GR_sizX_mm_ndc * GR_vf1SizXmm * GR_vf1SizScl;
  sclY = GR_sizY_mm_ndc * GR_vf1SizYmm * GR_vf1SizScl;

  // hight and width of a char is 1; set to requested size GR_vf1Siz in millimetres
  M44FC_scale__ ((float*)mx2D, sclX, sclY, 1.f);
    // M44FC_dump ((float*)mx2D, "GLBT_render-mSY2f-mx2D");

  // export a 2D- scale-only matrix;
  glUniformMatrix4fv (shSY2.ulMvp, 1, GL_FALSE, (float*)mx2D);

  //............................
  // get pt2 = endpoint of vc-X in device-coords
  Point pt1 = {1., 0., 0.};
  Point pt2;
  M44D_pt_mult_m44 (&pt2, mxdv, &pt1);

  // display 'x' at pt2
  pos1[0] = pt2.x;
  pos1[1] = pt2.y;
  glUniform3fv (shSY2.ulOff, 1, pos1);

  int ic = GL_ISYM_CHR('x');
  glDrawArrays (symTab[ic].lTyp, symTab[ic].iOff / 12, symTab[ic].oNr);


  //............................
  // get pt2 = endpoint of vc-Z in device-coords
  pt1.x = 0.;
  pt1.z = 1.;
  M44D_pt_mult_m44 (&pt2, mxdv, &pt1);

  // display 'z' at pt2
  pos1[0] = pt2.x; 
  pos1[1] = pt2.y; 
  glUniform3fv (shSY2.ulOff, 1, pos1);

  // ic = 'z' - 32;
  ic = GL_ISYM_CHR('z');
  glDrawArrays (symTab[ic].lTyp, symTab[ic].iOff / 12, symTab[ic].oNr);


  //----------------------------------------------------------------
  glEnable (GL_DEPTH_TEST);   // restore 3D
  glDisable(GL_BLEND);


  //----------------------------------------------------------------
  // close - disactivate
  glBindBuffer (GL_ARRAY_BUFFER, 0);
  glBindVertexArray (0);  // clear active buffer
  glUseProgram (0);     // disactivate active program




  return 0;

}


//================================================================
  int GL_font1_chr_load (int *iSym, int *iOff) {
//================================================================
// GL_font1_chr_load     load vectorfont into buffer
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
    irc = GL_shSY2_add_sym (iSym, bNr, &iOff1, cvDat, GL_LINES, lNr * 2);
    if(irc < 0) goto L_exit;

    // if(*iSym < ie) 
    goto L_nxt;


  L_done:
    *iOff = iOff1;
    irc = 0;

  L_exit:
    printf(" ex-GL_font1_chr_load irc=%d iSym=%d iOff=%d\n",irc,*iSym,shSY2.iOff);
  return 0;

}


//================================================================
  int GL_txt_2D (float *pos0, char *txt, float offC) {
//================================================================
// display string 2D (always same size, normal to user)
// INPUT:
//   pos0     startpos text, in screenCoords
//   txt      text, null-terminated
//   offC     width character; 

  int   ic;
  static float pos1[3] = {0.f, 0.f, 0.f};


  // printf("GL_txt_2D |%s| pos=%f offC=%f\n",txt,*pos0,offC);

  pos1[0] = *pos0;

  // loop tru string - get ic = index of char in symTab
  int sl = strlen(txt);

  // float offP = 0.f;  // offset position

  for(int is=0; is<sl; ++is) {
    ic = GL_ISYM_CHR(txt[is]); // get index for next char
      // printf(" txt_2D-ic=%d '%c' iOff=%d lNr=%d\n",ic,ic+32,
             // symTab[ic].iOff,symTab[ic].oNr);

    // provide the positionOffest in uniform
    glUniform3fv (shSY2.ulOff, 1, pos1);

    // do shSY2
    glDrawArrays (symTab[ic].lTyp, symTab[ic].iOff / 12, symTab[ic].oNr);

    pos1[0] += offC;
  }

  *pos0 = pos1[0];

  return 0;

}


// EOF
