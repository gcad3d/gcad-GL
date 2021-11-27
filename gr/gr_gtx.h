// ../gr/gr_gtx.h
// needs ../ut/utx.h



//----------------------------------------------------------------
// static in file ../gr/gr_gtx.c
#ifdef extern
int    GR_tx_nkNr = 2;         // Anzahl Nachkommastellen beim Text
double GR_tx_ldExt;            // Ueberlaenge der Leaderlines

#else
extern double GR_tx_ldExt;            // Ueberlaenge der Leaderlines


#endif

//----------------------------------------------------------------
int GR_gxt_subst1 (char *txo, char *fmt, char *tx1);
double GR_gtx_ckBlockWidth (char *txt, double size);
int    GR_gxt_strLen (char*);
double GTX_chw_ (double size);
int GR_gxt_dim_upd (double txsiz, int nkNr);
int GR_gxt_prep1 (char *txo, double d1);

//----------------------------------------------------------------
// GR_gxt_strLen         get stringlength of textnote
// ignore all controlcharacters '['
#define GR_gxt_strLen(txt)\
 strlen(txt) - UTX_cnr_chr (txt, '[')


// EOF
