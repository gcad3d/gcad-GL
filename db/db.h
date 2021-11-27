/*  ./db.h
*/


//----------------------------------------------------------------
int DB_init ();
int DB_get_free (int typ);
int DB_bSav ();

int DB_add__ (int typ, double *data, int ptNr, int dbi);
double DB_GetVar (long Ind);

// EOF
