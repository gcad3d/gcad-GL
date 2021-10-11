/* ./dl.h
DisplayList permanent objects
needs:
 ../ut/ut_types.h       UINT_32
static in dl.c
*/


//================================================================
 
// displayListRecord
// typ    type of obj; eg Typ_PT
// dbi    dataBaseIndex of obj
// bOff   GL-bufferOffset of obj
// iatt   for typ=LN/AC/Curve: LineTypNr.
//        for typ=Surf/Model: ColRGB
//        for typ=TAG/IMG: sx/sy
//        in Parent-state DL-index of child
// disp   0 = display;  1 = hide obj
// hili   0 = normal;   1 = hilite obj
// dim    0 = normal;   1 = dim obj

typedef struct {UINT_32 dbi, objID, glOff, iatt; UINT_16 vtxNr, typ;
                unsigned disp:1,  hili:1,  dim:1, UU:29;}                   DLrec;


//================================================================
extern int DL_siz;                 // size of dispList
extern int DL_nr;                  // nr of existig objects
extern DLrec *DL__;


//================================================================
int DL_init__ ();
int DL_add__ (int typ, int dbi, int objID, int *iatt, int iOff, int vtxNr);
int DL_hide__ (int dli, int mode);
int DL_dli_typ_dbi (int typ, int dbi);
int DL_col_dl (float *fCol, int dli);
int DL_dump ();


// EOF
