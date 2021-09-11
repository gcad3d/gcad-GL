/* col.h
*/

int Att__3i (Att_ln *att1, int cr, int cg, int cb);
int Att__width (Att_ln *att1, float cvw);
void Att_4f_att (float *fCol, Att_ln *att1);

int Col__3f (ColRGB *col1, float fCol[]);
int Col__3i (ColRGB *col1, int cr, int cg, int cb);

int Att_dump (Att_ln *att, char *sInf);
int Col_dump (ColRGB *col, char *sInf);

// EOF
