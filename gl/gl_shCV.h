/* gl_shCV.h
full include only in ../gl/gl_shCV.c
*/

#define SIZB_CV  100000          // space for buffer in bytes

# define SIZ_CHRTAB 100     // nr of symbols - size symTab

// iOff   bufferOffset in bytes
// lTyp   GL_LINE_STRIP|GL_LINE_LOOP|GL_POINTS
// oNr    nr of objects (eg lines for a character)
typedef struct {int iOff; UINT_16 lTyp, oNr;} chrRec;

// keep startPos and size of font1-characters and symbols
// 0-93 fontCharacters (char - 32)   from 32 (blank) - 125 ('}')
// 94   Diameter  is char 126
// 95   Degree    is char 127
// 96   plusMinus is char 128
extern chrRec chrTab[SIZ_CHRTAB];

//----------------------------------------------------------------
int GL_shCV_init ();
void GL_shCV_render (void);
void GL_SYP_render (void);
int GL_font1_load (int *iSym, int *iOff);
int GL_shCV_add_chr (int *iSym, int bNr, int *iOff, void *cvDat,
                        int lTyp, int oNr);
 

// EOF
