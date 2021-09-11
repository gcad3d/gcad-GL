// gl.h
// needs ./geo.h



// idPrg  index program
// idVao  index VAO
// idVbo  index VBO
// iOff   bufferOffset in bytes
// bSiz   total size of buffer in bytes
// ulMvp  unifom-location transformation matrix 4x4f
// ulAtt  unifom-location color 3f
// ulOff  unifom-location offset 3f
typedef struct {int idPrg, idVao, idVbo, iOff, ulMvp, ulMvm, ulAtt, ulOff, bSiz;} Shader;


//----------------------------------------------------------------
extern Shader shSUn;    // shader surfaces with normals
// extern Shader shSUi;   // shader surfaces with normals and indices
// extern Shader shSUt;   // shader surfaces with normals texture-uv and indices
extern Shader shCV;     // shader points, curves
extern Shader shSY2;    // shader 2D-symbols

// extern Mat_4x4D mva[6];   // 0=transl,1=scale,2=rot-Z,3=rot-vwpY,4=transl,5=ortho
extern Mat_4x4D mva[4];   // 0=transl,1=scale,2=rot-Z,3=rot-vwpY,4=transl,5=ortho
// extern Mat_4x4D mvpd;     // model-view-projection matrix double
extern Mat_4x4F mvpf;     // model-view-projection matrix float
// extern Mat_4x4F mSY2f;    // screen-coords-matrix


extern Mat_4x4D mdv;         // view-matrix   double row
extern Mat_4x4D mdm;         // model-matrix  double row
extern Mat_4x4F mfv;         // view-matrix * model-matrix   float column

//----------------------------------------------------------------
// PROTOTYPES:
int GL_init ();
int GL_shad_create (int *shadID, char* shadSrc, int shadTyp);
int GL_shad_make (int shader, int type);
int GL_prog_make (int program);
int GL_viewport ();
int GL_scale_def ();
int GL_view_update ();
int GL_render__ ();
int GL_buffer_f_add_d (int iOff, int oSiz, double *dData);

int GL_add_pt (double ptPos[], Att_ln iatt, int dbi);
int GL_add_cv (double cvDat[], int vtxNr, Att_ln iatt, int dbi);
int GL_add_sur (double *surDat, int ptNr, int typ, ColRGB color, int dbi);

int GL_test_VS1 (int dli);

// EOF
