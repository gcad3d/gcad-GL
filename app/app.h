/* ./tst_prim1.h
testapplication 1
*/




//----------------------------------------------------------------
// static in file ../app/app.c
#ifdef extern

double    AP_txdimsiz = 2.5;     // Dimensions-Text-Defaultsize
UINT_32   AP_OBJ_ID = 0;         // objID - codeLineNr

ColRGB actCol = _ColRGB_NUL;     // active defaultColor  (surfaces)
Att_ln cvAtt  = _Att_ln_NUL;     // active defaultLineTyp
Att_ln ptAtt  = _Att_ln_NUL;     // active defaultPointTyp


#else
extern double    AP_txdimsiz;
extern UINT_32   AP_OBJ_ID;           

extern ColRGB actCol;     // active defaultColor  (surfaces)
extern Att_ln cvAtt;      // active defaultLineTyp
extern Att_ln ptAtt;      // active defaultPointTyp

#endif


//----------------------------------------------------------------
int AP_do (int typ);
int AP_do_dump ();
int AP_help ();
int AP_defaults_read ();
int AP_CB_key (int iKey);
int AP_CB_win_is_up ();

int AP_obj_add_perm (int typ, void *data, int ptNr, void *iatt, int dbi);
int AP_obj_add_dyn (int typ, void *data, int ptNr, void *iatt, float size);

int main (int argc, char** argv);

int AP_test_cv1 (VecPos *pvc1);
int AP_test_cv2 (VecPos *pvc1);
int AP_test_Dimen (Point *pt1);
int AP_test_GTxt (Point *pt1);

// EOF
