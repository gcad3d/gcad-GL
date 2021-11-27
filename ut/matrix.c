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

M44D_init          Initialize a 4x4 - matrix  doubles
M44D_transl__      copy translation into matrix 4x4-double-row-order
M44D_transl_3      copy translation into matrix 4x4-double-row-order
M44D_scale__       scaling around origin 4x4-double-row-order
M44D_rot_X         rotate around X 4x4-double-row-order
M44D_rot_Y         rotate around Y 4x4-double-row-order
M44D_rot_Z         rotate around Z 4x4-double-row-order
M44D_mult__        multiply 2 mat 4x4-double-row-order
M44D_mult_n        multiply n Mat_4x4D double-row-order
M44D_pt_mult_m44   point = multiply point * matrix 4x4-double-row-order
M44D_inv__         invert 4x4-double-matrix row-order
M44D_dump__        dump 4x4 matrix in row-major order

M44FC__m44dr        float-column-order mat-4x4 from double-row-order mat-4x4

M44F_init           Initialize a 4x4 - matrix  floats
M44FC_transl__      copy translation into matrix 4x4-float-column-order
M44FC_scale__       scaling around origin 4x4-float-column-order
M44FC_m44_mult_m44  mat-c = mat-a * mat-b; 4x4-float-column-order
M44FC_vec4_vec3     copy vec3 + float -> vec4
M44FC_vc4_mult_m44  vec4 = multiply vec4 * matrix 4x4-float-column-order
M44FC_dump_mx4x4

List_functions_end:
=====================================================



//----------------------------------------------------------------
M44D 4x4-double-row-order:                        // C-compiler-default-order

 0=[0][0]=vx.x   1=[0][1]=vy.x   2=[0][2]=vz.x   3=[0][3]=or.x
 4=[1][0]=vx.y   5=[1][1]=vy.y   6=[1][2]=vz.y   7=[1][3]=or.y
 8=[2][0]=vx.z   9=[2][1]=vy.z  10=[2][2]=vz.z  11=[2][3]=or.z
12=[3][0]=0     13=[3][1]=0     14=[3][2]=0     15=[3][3]=1



//----------------------------------------------------------------
M44FC 4x4-float-column-order                  // used by opengl

 0=[0][0]=vx.x   1=[0][1]=vx.y   2=[0][2]=vx.z   3=[0][3]=0
 4=[1][0]=vy.x   5=[1][1]=vy.y   6=[1][2]=vy.z   7=[1][3]=0
 8=[2][0]=vz.x   9=[2][1]=vz.y  10=[2][2]=vz.z  11=[2][3]=0
12=[3][0]=or.x  13=[3][1]=or.y  14=[3][2]=or.z  15=[3][3]=1






//----------------------------------------------------------------
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../ut/geo.h"           // Point Mat_4x4D

#define extern  // does eliminate "extern"
#include "../ut/matrix.h"
#undef extern



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
  void M44D_transl_3 (Mat_4x4D dMat, double dx, double dy, double dz) {
//================================================================
// M44D_transl_3       copy translation into matrix 4x4-double-row-order
//
//  - - - x
//  - - - y
//  - - - z
//  - - - -

  M44D_init (dMat);

  dMat[0][3] = dx;
  dMat[1][3] = dy;
  dMat[2][3] = dz;

}


//================================================================
  void M44DC_transl__ (Mat_4x4D dMat, Point *ptOri) {
//================================================================
// M44D_transl__       copy translation into matrix 4x4-double-column-order
//
//  - - - -
//  - - - -
//  - - - -
//  x y z -
  
  M44D_init (dMat);

  dMat[3][0] = ptOri->x;
  dMat[3][1] = ptOri->y;
  dMat[3][2] = ptOri->z;

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

//   ma[3]  = px;
//   ma[7]  = py;
//   ma[11] = pz;

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
//  x - - a              x = 2/(r-l)                  l=0, r=800
//  - y - b              y = 2/(t-b)                  t=0, b=800
//  - - z c              z = -2/(f-n)                 n=0, f=1000
//  - - - -              a = -(r+l)/(r-l)
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
void M44D_mult__ (Mat_4x4D dMat, Mat_4x4D dMat1, Mat_4x4D dMat2) {
//=====================================================================/
// M44D_mult__          multiply 2 mat 4x4-double-row-order
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
void M44D_mult_n (Mat_4x4D dMat, int nMat, Mat_4x4D *dMatTr) {
//=====================================================================/
// M44D_mult_n        multiply n Mat_4x4D
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

  for (i = 1; i < nMat; i++) M44D_mult__ (dM, dMatTr[i], dM);

  memcpy (dMat, dM, sizeof(Mat_4x4D));

}


//================================================================
  int M44D_inv__ (void *m44dOut, void *m44dIn) {
//================================================================
// M44D_inv__          invert 4x4-double-matrix row-order
// Idea: David Moore (Mesa)
// INPUT:
//   m44dIn   Mat_4x4D / double[16]
// OUTPUT:
//   m44dOut  Mat_4x4D / double[16]
// Test: matFwd * matBwd must result in matIdentity


    double *invOut = m44dOut;
    double *m = m44dIn;
    double inv[16], det;
    int    i;

    inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
             + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
    inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
             - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
    inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
             + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
    inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
             - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
    inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
             - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
    inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
             + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
    inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
             - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
    inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
             + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
    inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
             + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
    inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
             - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
    inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
             + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
    inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
             - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
    inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
             - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
    inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
             + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
    inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
             - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
    inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
             + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

    det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
    if (det == 0)
        return -1;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return 0;
}


//====================================================================
  void M44D_pt_mult_m44 (void *ptOut, double mat[4][4], void *ptIn) {
//====================================================================
//  M44D_pt_mult_m44        point = multiply point * matrix 4x4-double-row-order
//  pto, pti = 3D-points (x,y,z); pto[3] = 1.0 assumed;
//
//  INPUT:
//    pti       Point|double[3] 3D-point x,y,z
//    mat       Mat_4x4D|double[16] matrix 4x4-double-row-order
//
//  OUTPUT:
//    pto        Point|double[3] 3D-point x,y,z
//

  double *pto = ptOut;
  double *pti = ptIn;
  double pt1[3];

  for (int i = 0; i < 3; i++) {
    pt1[i] = 0.0;
    for (int j = 0; j < 3; j++) {
      pt1[i] += mat[i][j] * pti[j];
    }
    pt1[i] += mat[i][3];
  }

  memcpy(pto, &pt1, sizeof(double) * 3);
}


/*
//================================================================
void M44FC_m44_mult_m44 (float *product, float *a, float *b) {
//================================================================

#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define P(row,col)  product[(col<<2)+row]


   int i;
   for (i = 0; i < 4; i++) {
      const float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
      P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
      P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
      P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
      P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
   }
}
*/


//================================================================
void M44FC_m44_mult_m44 (float *c, float *a, float *b) {
//================================================================
// M44FC_m44_mult_m44        mat-c = mat-a * mat-b; 4x4-float-column-order
// - eine  row-order-multiplication ??

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
// BUGGY !!!

   const float v0 = v[0], v1 = v[1], v2 = v[2], v3 = v[3];

   u[0] = v0 * m[0]  + v1 * m[1]  + v2 * m[2]  + v3 * m[3];
   u[1] = v0 * m[4]  + v1 * m[5]  + v2 * m[6]  + v3 * m[7];
   u[2] = v0 * m[8]  + v1 * m[9]  + v2 * m[10] + v3 * m[11];
   u[3] = v0 * m[12] + v1 * m[13] + v2 * m[14] + v3 * m[15];

}



//================================================================
void M44D_dump__ (Mat_4x4D ma, char *inf) {
//================================================================
// M44D_dump__         dump 4x4 matrix in row-major order
  
  char cps[256];
  
  printf("M44D_dump__ %s\n",inf);
  
  
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


// // __gluMultMatrixVecd
// //================================================================
//   void M44D_pt_mult_m44 (double out[4], double in[4], double m[16]) {
// //================================================================
// // M44D_pt_mult_m44        vec4 = multiply vec4 * matrix 4x4-double-row-order
// // u = v * m
// 
//   int i;
// 
//   for(i=0; i<4; i++) {
//     out[i] = in[0] * m[0*4+i] +
//              in[1] * m[1*4+i] +
//              in[2] * m[2*4+i] +
//              in[3] * m[3*4+i];
//   }
// }


//====================================================================
  void UT3D_pt_tra_pt_m4 (Point *p2, Mat_4x4D ma, Point *p1) {
//====================================================================
/// \code
/// UT3D_pt_tra_pt_m4           apply transformation to point (from 4x4-matrix)
///  p1 in p2 out.
/// \endcode


  double x, y, z, w;

  x = p1->x;
  y = p1->y;
  z = p1->z;

  w = (ma[0][3]*x + ma[1][3]*y + ma[2][3]*z + ma[3][3]);

  if(w != 1.0) {
    w = 1.0 / w;
    p2->x = w * (x*ma[0][0] + y*ma[1][0] + z*ma[2][0] + ma[3][0]);
    p2->y = w * (x*ma[0][1] + y*ma[1][1] + z*ma[2][1] + ma[3][1]);
    p2->z = w * (x*ma[0][2] + y*ma[1][2] + z*ma[2][2] + ma[3][2]);

  } else {
    p2->x = x*ma[0][0] + y*ma[1][0] + z*ma[2][0] + ma[3][0];
    p2->y = x*ma[0][1] + y*ma[1][1] + z*ma[2][1] + ma[3][1];
    p2->z = x*ma[0][2] + y*ma[1][2] + z*ma[2][2] + ma[3][2];
  }

  /* printf ("UT3D_pt_tra_pt_m4 %f %f %f \n",p2->x,p2->y,p2->z); */
}





// EOF
