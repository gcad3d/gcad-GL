/* ../ut/types.h
application-types
*/

#define Typ_VAR            1  // V   numer. variable
#define Typ_VC             2  // D   Vector
#define Typ_PT             3  // P   Point
#define Typ_LN             4  // L   Line
#define Typ_PVC            6  // P   Point + Vector    VecPos

#define Typ_VC2           10  // D   Vector - 2D
#define Typ_PT2           11  // P   Point  - 2D
#define Typ_LN2           12  // L   Line2
#define Typ_PVC2          16  // P   Point + Vector - 2D

#define Typ_CV            20  // S   all curves

#define Typ_SUR           50  // A all surfaces (support-surface, bounded)

#define Typ_GL_Sur        70  // tesselated surface, bMsh
#define Typ_GL_iSur       71  // tesselated indexed-surface
#define Typ_GL_tSur       72  // tesselated textured-surface
#define Typ_GL_Trias      77  // GL_TRIANGLES
#define Typ_GL_Tria_fan   78  // GL_TRIANGLE_FAN
#define Typ_GL_Tria_strip 79  // GL_TRIANGLE_STRIP

#define Typ_SOL           80  // B  body (CON, TOR, ..)


/// notes, images
#define Typ_Note          90  // GroupCode f Typ_ATXT|Typ_GTXT|Typ_Dimen..
#define Typ_ATXT          91  // N  AText (2D-text, tag, bitmap, symbol ..)
#define Typ_GTXT          92  // N  GText (grafic text)
#define Typ_Dimen         93  // N  Dimen (dimension)
#define Typ_Dim3          94  // N  Dim3  (3D-dimension)
#define Typ_Tag           95  // N Bitmap (Blocknote, Image ..)


//----------------------------------------------------------------
#define SYM_PT           501   // point
#define SYM_TRIA         502   // triangle                       
#define SYM_STAR         503   // star                           
#define SYM_CIR          504   // circle                         
#define SYM_SQU          505   // square 1 x 1 starts at low-left
#define SYM_PLN          506   // square 1 x 1 in centerpos

#define SYM_ARH2         507   // arrowhead - 2D
#define SYM_ARH3         508   // arrowhead - 3D


//----------------------------------------------------------------
#define Typ_Fnc0        1040  //
#define Typ_Fnc1        1041  //
#define Typ_Fnc2        1042  //
#define Typ_Fnc3        1043  //
#define Typ_Fnc4        1044  //
#define Typ_Fnc5        1045  //
#define Typ_Fnc6        1046  //
#define Typ_Fnc7        1047  //
#define Typ_Fnc8        1048  //
#define Typ_Fnc9        1049  //



// EOF
