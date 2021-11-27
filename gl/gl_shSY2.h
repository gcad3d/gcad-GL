/* gl_shSY2.h
needs ../ut/ut_types.h
full include only in ../gl/gl_shSY2.c
*/

#define SIZB_SY2  100000          // space for symbol-buffer in bytes

// iOff   bufferOffset in bytes
// lTyp   GL_LINE_STRIP|GL_LINE_LOOP|GL_POINTS
// oNr    nr of objects (eg lines for a character)
typedef struct {int iOff; UINT_16 lTyp, oNr;} symRec;

# define SIZ_SYMTAB 120     // nr of symbols - size symTab

// keep startPos and size of font1-characters and symbols
// 0-93 fontCharacters (char - 32)   from 32 (blank) - 125 ('}')
// 94   Diameter  is char 126
// 95   Degree    is char 127
// 96   plusMinus is char 128
extern symRec symTab[SIZ_SYMTAB];


//----------------------------------------------------------------
// symbol-indexes in buffer
extern int iSym_pt;      // symbol point
extern int iSym_vcx;     // symbol VC_X length 1
extern int iSym_vcy;     // symbol VC_Y length 1
extern int iSym_vcz;     // symbol VC_Z length 1
extern int iSym_squ;     // symbol square 1 x 1 starts at low-left
extern int iSym_pln;     // symbol square 1 x 1 in centerpos
extern int iSym_tria;    // symbol triangle
extern int iSym_star;    // symbol star
extern int iSym_arh2;    // symbol 2D-arrowhead
extern int iSym_arh3;    // symbol 3D-arrowhead


//----------------------------------------------------------------
int GL_shSY2_init ();
void GL_shSY2_render (void);
int GL_font1_chr_load ( int *iSym, int *iOff);
int GL_symbols_load (int *iSym, int *iOff);
int GL_shSY2_add_sym (int *iSym, int bNr, int *iOff, void *cvDat, int lTyp, int oNr);
int GL_shSY2_add_cv (Point *pta, int ptNr);
int GL_shSY2_axis ();
int GL_txt_2D (float *pos0, char *txt, float offC);

int GL_ISYM_CHR (char chr);




//================================================================
#define GL_ISYM_CHR(char) ((char) - 32)



// EOF
