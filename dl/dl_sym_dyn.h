/* ./dl_sym_dyn.h
Displaylist dynamic objects
needs:
 ../ut/ut_types.h       UINT_32
static in ./dl_dyn.h.c
*/



//================================================================

// displayListRecord
// typ    type of obj; eg Typ_PT
// bOff   GL-bufferOffset of obj
// iatt   for typ=LN/AC/Curve: Att_ln
//        for typ=Surf/Model: ColRGB
//        for typ=TAG/IMG: sx/sy

typedef struct {Point pos; UINT_32 iatt, iOff;
                float size, az, ay; UINT_16 vtxNr;
                short oTyp, lTyp;}                DL_SYD_rec;


//================================================================
extern int DL_SYD_siz;                 // size of dispList
extern int DL_SYD_nr;                  // nr of existig objects
extern DL_SYD_rec *DL_SYD_tab;


//================================================================
int DL_SYD_init__ ();
int DL_SYD_add__ (int typ, void *pos, float size, void *att);
int DL_SYD_add_pvc (void *data, float size, void *att);
int DL_SYD_dump ();

// EOF
