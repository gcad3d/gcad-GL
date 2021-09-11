/* ./tst_prim1.h
testapplicationa 1
*/

#define Typ_PT             3  ///< P   Point
#define Typ_CV            20  ///< S   all curves

#define Typ_SUR           50  ///< A all surfaces (support-surface, bounded)

#define Typ_GL_Sur        70  ///< tesselated surface, bMsh
#define Typ_GL_iSur       71  ///< tesselated indexed-surface
#define Typ_GL_tSur       72  ///< tesselated textured-surface
#define Typ_GL_Trias      77  ///< GL_TRIANGLES
#define Typ_GL_Tria_fan   78  ///< GL_TRIANGLE_FAN
#define Typ_GL_Tria_strip 79  ///< GL_TRIANGLE_STRIP

#define Typ_SOL           80  ///< B  body (CON, TOR, ..)


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



//----------------------------------------------------------------
#define FUNC_ViewTop       4
#define FUNC_ViewFront     5
#define FUNC_ViewSide      6
#define FUNC_ViewIso       7

//----------------------------------------------------------------


int do_func (int typ);
int do_help ();
int do_defaults_read ();

int main (int argc, char** argv);


// EOF

