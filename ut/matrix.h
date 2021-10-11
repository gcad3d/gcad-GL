/* ./matrix.h"
needs ./geo.h
*/

//----------------------------------------------------------------
// #ifdef extern
// //----------------------------------------------------------------
// #else
// #endif


//----------------------------------------------------------------
void M44D_init (Mat_4x4D ma);
void M44D_scale__ (Mat_4x4D dMat, double dSx, double dSy, double dSz);
void M44D_transl__ (Mat_4x4D dMat, Point *ptOri);
void M44D_rot_X (Mat_4x4D dMat, double angr);
void M44D_rot_Z (Mat_4x4D dMat, double angr);
void M44D_rot_Y (Mat_4x4D dMat, double angr);

void M44D_pt_mult_m44 (void *ptOut, double mat[4][4], void *ptIn);
void M44D_mult__ (Mat_4x4D dMat, Mat_4x4D dMat1, Mat_4x4D dMat2);
void M44D_mult_n (Mat_4x4D dMat, int nMat, Mat_4x4D *dMatTr);
void M44D_scale_ortho (Mat_4x4D dMat);
int M44D_inv__ (void *m44dOut, void *m44dIn);
void M44D_dump__ (Mat_4x4D ma, char *inf);

void M44F_init (Mat_4x4F ma);
void M44FC__m44dr (Mat_4x4F fMat, Mat_4x4D dMat);
void M44FC_transl__ (float *ma, float px, float py, float pz);
void M44FC_scale__ (float *ma, float sx, float sy, float sz);
void M44FC_m44_mult_m44 (float *c, float *a, float *b);
void M44FC_vc4_mult_m44 (float *u, float *v, float *m);

void M44FC_dump_mx4x4 (float *ma, char *inf);

//----------------------------------------------------------------
void mat_frustum (float *matrix, float angle_of_view, float aspect_ratio, float z_near, float z_far);
void mat_translate (float *matrix, float dx, float dy, float dz);
void mat_rotate (float *matrix, float x, float y, float z, float angle);
void mat_multiply (float *matrix, float *a, float *b);

void mat_frustum (float *matrix, float angle_of_view, float aspect_ratio,
                  float z_near, float z_far);
void mat_translate (float *matrix, float dx, float dy, float dz);
void mat_multiply (float *matrix, float *a, float *b);



//----------------------------------------------------------------
// M44D_init    Initialize a 4x4 - matrix  doubles
#define M44D_init(mx4x4) memcpy (mx4x4, MAT_4x4D_NUL, sizeof(Mat_4x4D));


// M44F_init    Initialize a 4x4 - matrix  floats
#define M44F_init(mx4x4) memcpy (mx4x4, MAT_4x4F_NUL, sizeof(Mat_4x4F));


void M44D_vec4_vec3 (double *vc4, double *vc3, double vc_4);
// M44D_vec4_vec3        copy vec3 + double -> vec4
#define M44D_vec4_vec3(vc4,vc3,vc_4)\
  memcpy (vc4, vc3, sizeof(double) * 3); vc4[3] = vc_4;


void M44FC_vec4_vec3 (float *vc4, float *vc3, float vc_4);
// M44FC_vec4_vec3        copy vec3 + float -> vec4
#define M44FC_vec4_vec3(vc4,vc3,vc_4)\
  memcpy (vc4, vc3, sizeof(float) * 3); vc4[3] = vc_4;

// EOF
