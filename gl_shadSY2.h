/* gl_shadSY2.h
*/

#define SIZB_SY2  100000          // space for symbol-buffer in bytes

// iOff   bufferOffset in bytes
// oNr    nr of objects (eg lines for a character)
typedef struct {int iOff, oNr;} font1Char;

// keep startPos and size of font1-characters and symbols
// 0-93 fontCharacters (char - 32)   from 32 (blank) - 125 ('}')
// 94   Diameter  is char 126
// 95   Degree    is char 127
// 96   plusMinus is char 128
// 98   square 1 x 1
extern font1Char symTab[100];

// symbol-indexes in buffer
extern int iSym_tag;

//----------------------------------------------------------------
int GL_shadSY2_init ();
void GL_shadSY2_render (void);
int GL_font1_chr_load ( int *iSym, int *iOff);
int GL_symbols_load (int *iSym, int *iOff);
int GL_shSY2_add (int *iSym, int bNr, int *iOff, void *cvDat, int oNr);
int GL_txt_2D (float *pos0, char *txt, float offC);

// EOF
