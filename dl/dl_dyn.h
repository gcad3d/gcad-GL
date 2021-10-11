/* ./dl_dyn.h
Displaylist dynamic objects
needs:
 ../ut/ut_types.h       UINT_32
static in ./dl_dyn.h.c
*/



//================================================================

// displayListRecord
// typ    type of obj; eg Typ_PT
// bOff   GL-bufferOffset of obj
// iatt   for typ=LN/AC/Curve: LineTypNr.
//        for typ=Surf/Model: ColRGB
//        for typ=TAG/IMG: sx/sy
//        in Parent-state DL-index of child

typedef struct {Point pos; UINT_32 iatt, iOff;
                float size, az, ay; UINT_16 vtxNr;
                short oTyp, lTyp;}                DLdynrec;


//================================================================
extern int DLdyn_siz;                 // size of dispList
extern int DLdyn_nr;                  // nr of existig objects
extern DLdynrec *DLdyn;


//================================================================
int DLdyn_init__ ();
int DLdyn_add__ (int typ, void *pos, float size, void *att);
int DLdyn_add_pvc (void *data, float size, void *att);
int DLdyn_dump ();

// EOF
