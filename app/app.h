/* ./tst_prim1.h
testapplication 1
full include (without extern) in ./tst_prim1.c)
*/




//----------------------------------------------------------------
extern UINT_32 AP_OBJ_ID;           


//----------------------------------------------------------------
int AP_do (int typ);
int AP_help ();
int AP_defaults_read ();
int AP_CB_key (int iKey);
int AP_CB_win_is_up ();

int AP_obj_add_perm (int typ, double *data, int ptNr, void *iatt, int dbi);
int AP_obj_add_dyn (int typ, void *data, int ptNr, void *iatt, float size);

int main (int argc, char** argv);

int AP_test_cv1 (VecPos *pvc1);
int AP_test_cv2 (VecPos *pvc1);

// EOF
