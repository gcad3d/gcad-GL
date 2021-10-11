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

extern Mat_4x4D GR_matd_scl;    // scale user-coords to device-coords
extern Mat_4x4D GR_matd_mdl[4]; // model-matrix (pan, rotate); see INF_matrix
extern Mat_4x4D GR_matd_view;   // view-matrix; GL_matd_scl * GR_matd_mdl
extern Mat_4x4F GR_matf_dev;    // final matrix GR_matd_view as floats, column-order

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

int GL_dyn_sym3 (int typ, void *data, void *att, float size);

int GL_add_pt (double ptPos[], Att_ln iatt, int dbi);
int GL_add_cv (double cvDat[], int vtxNr, Att_ln iatt, int dbi);
int GL_add_sur (double *surDat, int ptNr, int typ, ColRGB color, int dbi);

int GL_test_bufferSD (int iOff, int fNr);
int GL_test_VS1 (int dli);

// EOF
