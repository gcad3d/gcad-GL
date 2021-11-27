/* ./dl_sym_perm.h
Displaylist dynamic objects
needs:
 ../ut/ut_types.h       UINT_32
static in ./dl_dyn.h.c
*/



//================================================================

// displayListRecord
// typ    type of obj; eg Typ_PT
// iOff   GL-bufferOffset of obj
// iatt   for typ=LN/AC/Curve: Att_ln (dash, thick, color)
//        for typ=Surf/Model: ColRGB
//        for typ=TAG/IMG: sx/sy

typedef struct {Pointf pos; UINT_32 iatt, iOff;
                float ax, ay, az; UINT_16 vtxNr;
                short oTyp, lTyp;}                DL_SYP_rec;


//================================================================
extern int DL_SYP_siz;                 // size of dispList
extern int DL_SYP_nr;                  // nr of existig objects
extern DL_SYP_rec *DL_SYP_tab;


//================================================================
int DL_SYP_init__ ();
int DL_SYP_add__ (int typ, void *pos, float size, void *att);
int DL_SYP_add_ln2 (Line2 *data, float size, void *att);
int DL_SYP_add_pvc (void *data, float size, void *att);
int DL_SYP_dump ();

// EOF
