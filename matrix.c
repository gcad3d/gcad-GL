/* ./matrix.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Matrix-functions -

M44D     matrix 4x4 double-row-order
M44FC    matrix 4x4 float-column-order


=====================================================
List_functions_start:

M44D_transl__       copy translation into matrix 4x4-double-row-order
M44D_scale__        scaling around origin 4x4-double-row-order
M44D_rot_X          rotate around X 4x4-double-row-order
M44D_rot_Y          rotate around Y 4x4-double-row-order
M44D_rot_Z          rotate around Z 4x4-double-row-order
M44D__mult__        multiply 2 mat 4x4-double-row-order
M44D__mult_n        multiply n Mat_4x4D double-row-order

M44FC__m44dr        float-column-order mat-4x4 from double-row-order mat-4x4

M44FC_transl__      copy translation into matrix 4x4-float-column-order
M44FC__mult__       mat-c = mat-a * mat-b; 4x4-float-column-order
M44FC_vec4_vec3     copy vec3 + float -> vec4
M44FC_vc4_mult_m44  vec4 = multiply vec4 * matrix 4x4-float-column-order

M44FC_dump_mx4x4
M44FC_dump_vc4

List_functions_end:
=====================================================

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./geo.h"           // Point Mat_4x4D
#include "./matrix.h"



//================================================================
  void M44FC__m44dr (Mat_4x4F fMat, Mat_4x4D dMat) {
//================================================================
// M44FC__m44dr           float-column-order mat-4x4 from double-row-order mat-4x4

  int   i1, i2;

  fMat[0][0] = dMat[0][0];
  fMat[0][1] = dMat[1][0];
  fMat[0][2] = dMat[2][0];
  fMat[0][3] = dMat[3][0];

  fMat[1][0] = dMat[0][1];
  fMat[1][1] = dMat[1][1];
  fMat[1][2] = dMat[2][1];
  fMat[1][3] = dMat[3][1];

  fMat[2][0] = dMat[0][2];
  fMat[2][1] = dMat[1][2];
  fMat[2][2] = dMat[2][2];
  fMat[2][3] = dMat[3][2];

  fMat[3][0] = dMat[0][3];
  fMat[3][1] = dMat[1][3];
  fMat[3][2] = dMat[2][3];
  fMat[3][3] = dMat[3][3];

}


//================================================================
  void M44D_transl__ (Mat_4x4D dMat, Point *ptOri) {
//================================================================
// M44D_transl__       copy translation into matrix 4x4-double-row-order
//
//  - - - x
//  - - - y
//  - - - z
//  - - - -

  M44D_init (dMat);

  dMat[0][3] = ptOri->x;
  dMat[1][3] = ptOri->y;
  dMat[2][3] = ptOri->z;

}


//================================================================
void M44FC_transl__ (float *ma, float px, float py, float pz) {
//================================================================
// M44FC_transl__       copy translation into matrix 4x4-float-column-order
//
//  - - - -
//  - - - -
//  - - - -
//  x y z -

  M44F_init (ma);

  ma[12] = px;
  ma[13] = py;
  ma[14] = pz;

}



//=======================================================================
 void M44D_scale__ (Mat_4x4D dMat, double dSx, double dSy, double dSz) {
//=======================================================================
//   M44D_scale__                     scaling around origin 4x4-double-row-order
//
//  x - - -
//  - y - -
//  - - z -
//  - - - -

  M44D_init (dMat);

  dMat[0][0] = dSx;
  dMat[1][1] = dSy;
  dMat[2][2] = dSz;

}



//=======================================================================
 void M44D_scale_ortho (Mat_4x4D dMat) {
//=======================================================================
//   M44D_scale__                     scaling around origin 4x4-double-row-order
//
//  x - - -              x = 2/(r-l)                  l=0, r=800
//  - y - -              y = 2/(t-b)                  t=0, b=800
//  - - z -              z = -2/(f-n)                 n=0, f=1000
//  a b c -              a = -(r+l)/(r-l)
//                       b = -(t+b)/(t-b)
//                       c = -(f+n)/(f-n)            

  M44D_init (dMat);

  dMat[0][0] = 2.;     // x = 2/(r-l)
  dMat[1][1] = 2.;     // y = 2/(t-b)
  dMat[2][2] = -2./ 10.;    // z = -2/(f-n)

  dMat[0][3] = -1.;     // a = -(r+l)/(r-l)   -1
  dMat[1][3] = -1.;     // b = -(t+b)/(t-b)   -1
  dMat[2][3] = -1.;   // c = -(f+n)/(f-n)   -1

/*
  dMat[0][0] = 2. / 800.;     // x = 2/(r-l)
  dMat[1][1] = 2. / 800.;     // y = 2/(t-b)
  dMat[2][2] = -2./ 1000.;    // z = -2/(f-n)

  dMat[0][3] = -800./800.;     // a = -(r+l)/(r-l)   -1
  dMat[1][3] = -800./800.;     // b = -(t+b)/(t-b)   -1
  dMat[2][3] = -1000./1000.;   // c = -(f+n)/(f-n)   -1
*/
}



//=======================================================================
 void M44FC_scale__ (float *ma, float sx, float sy, float sz) {
//=======================================================================
//   M44D_scale__                     scaling around origin column-order
//
//  x - - -
//  - y - -
//  - - z -
//  - - - -

  M44F_init (ma);

  ma[0] = sx;
  ma[5] = sy;
  ma[10] = sz;

}


//=======================================================================
 void M44D_rot_X (Mat_4x4D dMat, double angr) {
//=======================================================================
// M44D_rot_X             rotate around X 4x4-double-row-order
//
//    1    0    0    0
//    0   cos -sin   0
//    0   sin  cos   0
//    0    0    0    1

  M44D_init (dMat);

  dMat[1][1] = cos(angr);
  dMat[1][2] = -sin(angr);

  dMat[2][1] = -dMat[1][2];
  dMat[2][2] = dMat[1][1];

}


//=======================================================================
 void M44D_rot_Y (Mat_4x4D dMat, double angr) {
//=======================================================================
// M44D_rot_Y             rotate around Y 4x4-double-row-order
//
//   cos   0  sin  0
//    0    1   0   0
//  -sin   0  cos  0
//    0    0   0   1

  M44D_init (dMat);
  
  dMat[0][0] = cos(angr);
  dMat[0][2] = sin(angr);

  dMat[2][0] = -dMat[0][2];
  dMat[2][2] = dMat[0][0];

} 


//=======================================================================
 void M44D_rot_Z (Mat_4x4D dMat, double angr) {
//=======================================================================
// M44D_rot_Z             rotate around Z 4x4-double-row-order
//
//   cos   -sin  0   0
//   sin    cos  0   0
//    0      0   1   0
//    0      0   0   1

  M44D_init (dMat);

  dMat[0][0] = cos(angr);
  dMat[0][1] = -sin(angr);

  dMat[1][0] = -dMat[0][1];
  dMat[1][1] = dMat[0][0];

}


//=====================================================================/
void M44D__mult__ (Mat_4x4D dMat, Mat_4x4D dMat1, Mat_4x4D dMat2) {
//=====================================================================/
// M44D__mult__          multiply 2 mat 4x4-double-row-order
// dMat = dMat1 * dMat2;


  int     i, j, k;
  Mat_4x4D dM;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      dM[i][j] = 0.0;
      for (k = 0; k < 4; k++) {
        dM[i][j] += dMat1[i][k] * dMat2[k][j];
      }
    }
  }

  memcpy (dMat, dM, sizeof(Mat_4x4D));
}


//=====================================================================/
void M44D__mult_n (Mat_4x4D dMat, int nMat, Mat_4x4D *dMatTr) {
//=====================================================================/
// M44D__mult_n        multiply n Mat_4x4D
//  Die Ergebnismatrix darf den Speicherplatz mit einer der Ausgangsmatrizen teilen.
//------------------------------------------------------------------------
//
//  INPUT:
//  ------
//    long nMat             ... Anzahl der 3D-Transformationen
//    double dMatTr[][4][4] ... Array/Abfolge der Matrixen der 3D-Transformationen
//
//  OUTPUT:
//  -------
//    double dMat[4][4]    ... Matrix der verknüpften 3D-Transformationen.
//                             Idendität / Einheitsmatrix falls nMat < 1
//
//=======================================================================/

  int     i;
  Mat_4x4D dM;

  if (nMat < 1) {
    M44D_init (dMat);
    return;
  }

  memcpy (dM, dMatTr[0], sizeof(Mat_4x4D));

  for (i = 1; i < nMat; i++) M44D__mult__ (dM, dMatTr[i], dM);

  memcpy (dMat, dM, sizeof(Mat_4x4D));

}


//================================================================
void M44FC_m44_mult_m44 (float *c, float *a, float *b) {
//================================================================
// M44FC_m44_mult_m44        mat-c = mat-a * mat-b; 4x4-float-column-order

  float a11,a12,a13,a14,a21,a22,a23,a24,a31,a32,a33,a34,a41,a42,a43,a44;
  float b11,b12,b13,b14,b21,b22,b23,b24,b31,b32,b33,b34,b41,b42,b43,b44;


  a11 = a[0];
  a12 = a[4];
  a13 = a[8];
  a14 = a[12];
  a21 = a[1];
  a22 = a[5];
  a23 = a[9];
  a24 = a[13];
  a31 = a[2];
  a32 = a[6];
  a33 = a[10];
  a34 = a[14];
  a41 = a[3];
  a42 = a[7];
  a43 = a[11];
  a44 = a[15];

  b11 = b[0];
  b12 = b[4];
  b13 = b[8];
  b14 = b[12];
  b21 = b[1];
  b22 = b[5];
  b23 = b[9];
  b24 = b[13];
  b31 = b[2];
  b32 = b[6];
  b33 = b[10];
  b34 = b[14];
  b41 = b[3];
  b42 = b[7];
  b43 = b[11];
  b44 = b[15];

  c[0] = a11*b11 + a12*b21 + a13*b31 + a14*b41;
  c[4] = a11*b12 + a12*b22 + a13*b32 + a14*b42;
  c[8] = a11*b13 + a12*b23 + a13*b33 + a14*b43;
  c[12] = a11*b14 + a12*b24 + a13*b34 + a14*b44;

  c[1] = a21*b11 + a22*b21 + a23*b31 + a24*b41;
  c[5] = a21*b12 + a22*b22 + a23*b32 + a24*b42;
  c[9] = a21*b13 + a22*b23 + a23*b33 + a24*b43;
  c[13] = a21*b14 + a22*b24 + a23*b34 + a24*b44;

  c[2] = a31*b11 + a32*b21 + a33*b31 + a34*b41;
  c[6] = a31*b12 + a32*b22 + a33*b32 + a34*b42;
  c[10] = a31*b13 + a32*b23 + a33*b33 + a34*b43;
  c[14] = a31*b14 + a32*b24 + a33*b34 + a34*b44;

  c[3] = a41*b11 + a42*b21 + a43*b31 + a44*b41;
  c[7] = a41*b12 + a42*b22 + a43*b32 + a44*b42;
  c[11] = a41*b13 + a42*b23 + a43*b33 + a44*b43;
  c[15] = a41*b14 + a42*b24 + a43*b34 + a44*b44;

//   for (i = 0; i < 4; i++) {
//     for (j = 0; j < 4; j++) {
//       c[i][j] = 0.0;
//       for (k = 0; k < 4; k++) {
//         c[i][j] += a[i][k] * b[k][j];
//       }
//     }
// q  }

}


//================================================================
  void M44FC_vc4_mult_m44 (float u[4], float v[4], float m[16]) {
//================================================================
// M44FC_vc4_mult_m44        vec4 = multiply vec4 * matrix 4x4-float-column-order
// u = v * m

   const float v0 = v[0], v1 = v[1], v2 = v[2], v3 = v[3];

   u[0] = v0 * m[0]  + v1 * m[1]  + v2 * m[2]  + v3 * m[3];
   u[1] = v0 * m[4]  + v1 * m[5]  + v2 * m[6]  + v3 * m[7];
   u[2] = v0 * m[8]  + v1 * m[9]  + v2 * m[10] + v3 * m[11];
   u[3] = v0 * m[12] + v1 * m[13] + v2 * m[14] + v3 * m[15];

}




//================================================================
void M44D_dump (Mat_4x4D ma, char *inf) {
//================================================================
// M44FC_dump            dumpx 4x4 matrix in column-major order
  
  char cps[256];
  
  printf("M44D_dump %s\n",inf);
  
  
    // Mat_4x4D[4][4]; RecLen = 4
    sprintf(cps,"  %9.3f,%9.3f,%9.3f,%9.3f",ma[0][0],ma[1][0],ma[2][0],ma[3][0]);
      printf(" %s\n",cps);
  
    sprintf(cps,"  %9.3f,%9.3f,%9.3f,%9.3f",ma[0][1],ma[1][1],ma[2][1],ma[3][1]);
      printf(" %s\n",cps);

    sprintf(cps,"  %9.3f,%9.3f,%9.3f,%9.3f",ma[0][2],ma[1][2],ma[2][2],ma[3][2]);
      printf(" %s\n",cps); 
  
    sprintf(cps,"  %9.3f,%9.3f,%9.3f,%9.3f",ma[0][3],ma[1][3],ma[2][3],ma[3][3]);
      printf(" %s\n",cps);
  
} 
  

//================================================================
void M44FC_dump_mx4x4 (float *ma, char *inf) {
//================================================================
// M44FC_dump_mx4x4            dump 4x4 matrix in column-major order
 
  char cps[256];

  printf("M44FC_dump_mx4x4 %s\n",inf);


    // Mat_4x4D[4][4]; RecLen = 4
    sprintf(cps," M44VX=%9.3f,%9.3f,%9.3f,%9.3f",ma[0],ma[4],ma[8],ma[12]);
      printf(" %s\n",cps);

    sprintf(cps,"    VY=%9.3f,%9.3f,%9.3f,%9.3f",ma[1],ma[5],ma[9],ma[13]);
      printf(" %s\n",cps);

    sprintf(cps,"    VZ=%9.3f,%9.3f,%9.3f,%9.3f",ma[2],ma[6],ma[10],ma[14]);
      printf(" %s\n",cps);

    sprintf(cps,"    PO=%9.3f,%9.3f,%9.3f,%9.3f",ma[3],ma[7],ma[11],ma[15]);
      printf(" %s\n",cps);

}


//================================================================
void M44FC_dump_vc4 (float *vc4, char *inf) {
//================================================================
// M44FC_dump_vc4            dump vec4

  char cps[256];

  // printf("M44FC_dump_vc4 %s\n",inf);


    sprintf(cps," vec4=%9.3f,%9.3f,%9.3f,%9.3f",vc4[0],vc4[1],vc4[2],vc4[3]);
      printf(" %s     %s\n",cps,inf);

}



//================================================================
//================================================================


void
mat_frustum (float *matrix, float angle_of_view, float aspect_ratio,
             float z_near, float z_far)
{
  matrix[0] = 1.0f / tanf(angle_of_view);
  matrix[1] = 0.0f;
  matrix[2] = 0.0f;
  matrix[3] = 0.0f;
  matrix[4] = 0.0f;
  matrix[5] = aspect_ratio / tanf(angle_of_view);
  matrix[6] = 0.0f;
  matrix[7] = 0.0f;
  matrix[8] = 0.0f;
  matrix[9] = 0.0f;
  matrix[10] = (z_far + z_near) / (z_far - z_near);
  matrix[11] = 1.0f;
  matrix[12] = 0.0f;
  matrix[13] = 0.0f;
  matrix[14] = -2.0f * z_far * z_near / (z_far - z_near);
  matrix[15] = 0.0f;
}



void
mat_translate (float *matrix, float dx, float dy, float dz)
{
  matrix[0] = 1;
  matrix[1] = 0;
  matrix[2] = 0;
  matrix[3] = 0;
  matrix[4] = 0;
  matrix[5] = 1;
  matrix[6] = 0;
  matrix[7] = 0;
  matrix[8] = 0;
  matrix[9] = 0;
  matrix[10] = 1;
  matrix[11] = 0;
  matrix[12] = dx;
  matrix[13] = dy;
  matrix[14] = dz;
  matrix[15] = 1;
}

static void
normalize (float *x, float *y, float *z)
{
  float d = sqrtf((*x) * (*x) + (*y) * (*y) + (*z) * (*z));
  *x /= d;
  *y /= d;
  *z /= d;
}


void
mat_rotate (float *matrix, float x, float y, float z, float angle)
{
  normalize(&x, &y, &z);

  float s = sinf(angle);
  float c = cosf(angle);
  float m = 1 - c;

  matrix[0] = m * x * x + c;
  matrix[1] = m * x * y - z * s;
  matrix[2] = m * z * x + y * s;
  matrix[3] = 0;
  matrix[4] = m * x * y + z * s;
  matrix[5] = m * y * y + c;
  matrix[6] = m * y * z - x * s;
  matrix[7] = 0;
  matrix[8] = m * z * x - y * s;
  matrix[9] = m * y * z + x * s;
  matrix[10] = m * z * z + c;
  matrix[11] = 0;
  matrix[12] = 0;
  matrix[13] = 0;
  matrix[14] = 0;
  matrix[15] = 1;
}

void
mat_multiply (float *matrix, float *a, float *b)
{
  float result[16];
  for (int c = 0; c < 4; c++) {
    for (int r = 0; r < 4; r++) {
      int index = c * 4 + r;
      float total = 0;
      for (int i = 0; i < 4; i++) {
        int p = i * 4 + r;
        int q = c * 4 + i;
        total += a[p] * b[q];
      }
      result[index] = total;
    }
  }
  for (int i = 0; i < 16; i++)
    matrix[i] = result[i];
}

// EOF
