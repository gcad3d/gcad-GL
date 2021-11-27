/* ./gr_view.h
neutral grafic-definitions (not OpenGL, Gtk-specific)
needs geo.h
full include (without extern) in ./gr.c
*/



//----------------------------------------------------------------
#define FUNC_ViewTop       4
#define FUNC_ViewFront     5
#define FUNC_ViewSide      6
#define FUNC_ViewIso       7
#define FUNC_ViewBox       8
#define FUNC_ViewSet       9
#define FUNC_ViewGet      10


//----------------------------------------------------------------

#define SCALE_MAX 1000000.0
#define SCALE_MIN 0.0000001

// defaultColor
#define _ColRGB_NUL {32, 32, 32,\
                     0, 0, 0, 1, 0}
// defaultLinetyp
#define _Att_ln_NUL {0, 2,\
                     0, 0, 0, 0}





//----------------------------------------------------------------
// grafic attribute; Typ_G_Att for curves
// dash       0 = full-line (VollLinie);    1 = dash-dot (Strich-Punkt),
//            2 = dashed (kurz strichliert) 3 = dashed-long (lang strichliert),
// thick      line-thickness in pixels; 1-6
// cr cg cb   color red, green, blue;  0-9  (digit; not hex-char)
// used for line,circle,curve,text
typedef struct {unsigned dash:8, thick:8,
                         cr:4, cg:4, cb:4, uu:4;}                   Att_ln;
// size = 4


// grafic attribute; Typ_Color for surfaces
// vtra   view transparent; 0=not, 1=25% transparent, 2=50%, 3=75%
// vtex   has texture; 0=not, 1=yes; (cr+cg+cb) = textureRefNr
// vsym   view normal (0,shaded) or symbolic (1,wireframeboundary)
// color  0=color not active (use AP_defcol); 1=color from cr/cg/cb
// cr,cg,cb  red, green, blue; 0-255
// Grafic-Attribute for surface, solid (not line, curve - see Att_ln).
typedef struct {unsigned cr:8, cg:8, cb:8,
                vtra:2,vsym:1,vtex:1,color:1,UU:3;}                 ColRGB;
// size = 4


typedef struct {double az, ax, scl; Point pcv, pcr;}                View;

//----------------------------------------------------------------
// static in file gr.c
#ifdef extern
const ColRGB  ColRGB_NUL   = _ColRGB_NUL;
const Att_ln  Att_ln_NUL   = _Att_ln_NUL;

static View   view1;

#else
extern ColRGB  ColRGB_NUL;
extern Att_ln  Att_ln_NUL;

#endif


//----------------------------------------------------------------
extern double GR_Modsiz;          // modelsize   TODO

extern int    GR_sizScrX_px;      // 3840   total screensize in pixels 
extern int    GR_sizScrY_px;      // 2160   total screensize in pixels
extern int    GR_sizScrX_mm;      // total screensize-X in mm 
extern int    GR_sizScrY_mm;      // total screensize-Y in mm 
extern int    GR_sizWinX_px;      // size grafic-window pixels 
extern int    GR_sizWinY_px;      // size grafic-window pixels
extern double GR_sizX_mm_ndc;     // size of 1 millimeter in device-coords (NDC)
extern double GR_sizY_mm_ndc;     // size of 1 millimeter in device-coords
extern double GR_sizX_mm_px;      // size of 1 millimeter in pixels (SCS)
extern double GR_sizY_mm_px;      // size of 1 millimeter in pixels (SCS)
// extern double GR_sizX_px_mcs;     // size of 1 pixel in modelspace-coords
// extern double GR_sizY_px_mcs;     // size of 1 pixel in modelspace-coords

extern float  GR_mousePosX,       // active position of cursor in pixels
              GR_mousePosY;
extern float  GR_selPosX,         // select-position of cursor in pixels
              GR_selPosY;

extern double GR_ButtY_ndc;       // Y-value of top of 2D-system-buttons in MSC

extern float  GR_vf1SizXmm;       // fontsize-X in mm
extern float  GR_vf1SizYmm;       // fontsize-Y in mm
extern float  GR_vf1SizScl;       // scale for fontsize

extern double GR_Scale;

extern double GR_angZ;            // angle rotation around model-Z-axis
extern double GR_angX;            // angle tilted Z-axis into direction GR_angZ

extern int   GR_actView;          // active view - FUNC_ViewTop | FUNC_ViewIso | ..
extern Point GR_cenRot;           // centerpoint of rotation
extern Point GR_cenView;          // point in the center of the screen, in WCS

extern Plane  GR_constrPln;       // the active construction-plane

extern ColRGB defCol;             // default-color
extern Att_ln defAtt;             // default-linetyp

extern int    GR_render_skip;     // 0=do-render; 1=skip-next-render;

Vector GR_eyeX;  // vector parallel to horizontal windowBorder (WCS)
Vector GR_eyeY;  // vector parallel to vertical windowBorder (WCS)
Vector GR_eyeZ;  // vector from the screenCenter to the eye (WCS)





//----------------------------------------------------------------
// PROTOTYPES:
double GR_Xndc_Xpx (float X_px);
double GR_Yndc_Ypx (float Y_px);

int GR_sele__ ();
int GR_init0 ();
int GR_modsiz_set (double mSiz);
int GR_view_cen_set (double px, double py, double pz);
int GR_do_pan (double dx, double dy);
int GR_do_rot (double dx, double dy);
int GR_get_curPos__ (Point *pt_ind, Point *pt_cps, float mouseX, float mouseY);
int GR_get_eyeVecs ();
int GR_view__ (int typ);
int GR_view_get ();

int GR_dyn_sym (int typ, void *data, void *att, float size);

int GR_perm_pt (double *ptPos, Att_ln iatt, int objId, int dbi);
int GR_perm_cv (double *cvDat, int ptNr, Att_ln iatt, int objId, int dbi);
int GR_perm_sur (double *surDat, int ptNr, int typ, ColRGB color,
                 int objId, int dbi);
int GR_perm_Note (Dimen *dim1, int oNr, int typ, void *iatt, int objID, int dbi);
int GR_perm_GTxt (GText *tx1, int oNr, int typ, void *iatt, int objID, int dbi);
int GR_perm_txtA (double fPos[], char *txt, Att_ln iatt, int dbi);

int GR_dim_lin__ (Line2 *ll1, Line2 *ll2, Line2 *lnd,
                    SymRef2 *hd1, SymRef2 *hd2, Vector2 *vct,
                    double *dval, char *outText,
                    Dimen *dim1);


int GR_gxt_prep1 (char *txo, double d1);

int GR_resize ();
int GR_redraw ();
int GR_dump ();
int GR_test__ ();

// EOF
