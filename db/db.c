/*  db.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

[dummy-] functions for dataBase

=====================================================
List_functions_start:

DB_init           init DB__, DB_tab
DB_get_free       returns next free index
DB_bSav           store binary-data


DB_add__          add geom. obj into DB
DB_GetVar         get value variable;          DUMMY

List_functions_end:
=====================================================

DEMO-VERSION

- store only text

TODO: reset with DB_nr-in


*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../ut/geo.h"                     // Point Plane Mat_4x4D ..
#include "../db/db.h"                      // DB_..
#include "../gr/gr.h"                      // ColRGB
#include "../app/app.h"               // AP_OBJ_ID


//----------------------------------------------------------------
#define DB__iniSiz 2056               // initial size DB__ in bytes
#define DB_tab_Siz 100                // initial size DB_tab nr-of-records


//----------------------------------------------------------------
// dbOff       offset of data in bytes in DB__ 
typedef struct {UINT_32 objId, dbOff;}                     DB_rec;


//----------------------------------------------------------------
static int DB_ID = 0;
static int ivc = 0;
static int ipt = 0;
static int icv = 0;
static int isu = 0;
static int ino = 0;  // dbi Note dummy

static char   *DB__ = NULL;
static int     DB__nr = 0;         // nr stored bytes in DB__
static int     DB__tot = 0;        // nr allocated bytes in DB__

static DB_rec *DB_tab = NULL;
static int     DB_tab_nr = 0;      // nr active records in DB_tab
static int     DB_tab_tot = 0;     // nr allocated records in DB_tab



//================================================================
  int DB_init () {
//================================================================
// DB_init           init DB__, DB_tab



  //----------------------------------------------------------------
  DB__ = (char*)realloc(DB__, DB__iniSiz);
  if(DB__ == NULL) {
    printf ("******** out of memory - DB_init DB__ *********\n");
    TX_Error ("******** out of memory - DB_init DB__ *********");
    return -1;
  }
  DB__tot = DB__iniSiz;


  //----------------------------------------------------------------
  DB_tab = (DB_rec*)realloc(DB__, DB_tab_Siz * sizeof(DB_rec));
  if(DB_tab == NULL) {
    printf ("******** out of memory - DB_init DB_tab *********\n");
    TX_Error ("******** out of memory - DB_init DB_tab *********");
    return -1;
  }
  DB_tab_tot = DB_tab_Siz;


  return 0;

}


//================================================================
  int DB_get_free (int typ) {
//================================================================
// DB_get_free       returns next free index


  if(typ == Typ_VC) {
    ++ivc;
    return ivc;

  } else if(typ == Typ_PT) {
    ++ipt;
    return ipt;

  } else if(typ == Typ_CV) {
    ++icv;
    return icv;

  } else if(typ == Typ_SUR) {
    ++isu;
    return isu;

  } else if(typ == Typ_Note) {
    ++ino;
    return ino;

  }

  return -1;
}


//================================================================
  int DB_bSav (void *data, int size) {
//================================================================
// DB_bSav           store binary-data
// INPUT:
//   data, size      size = nor of bytes
// OUTPUT:
//   retCode         the CAD_ID

  int  dbOff, dbNxt;

  printf("DB_bSav \n");

  // check space in DB_tab
  if(DB_tab_nr >= DB_tab_tot) {
    TX_Error ("******** out of memory - DB_bSav DB__ DB_tab *********");
    return -1;
  } 

  // make size mod 8
  size = UTI_round_8up (size);

  // check space in DB__
  dbOff = DB__nr;
  dbNxt = dbOff + size;
  if(dbNxt >= DB__tot) {
    TX_Error ("******** out of memory - DB_bSav DB__ DB__ *********");
    return -1;
  }

  // copy data -> DB__
  memcpy (&DB__[DB__nr], data, size);

  // add DB_tab - record
  DB_tab[DB_tab_nr].dbOff = DB__nr;

  DB__nr = dbNxt;
  ++DB_tab_nr;

  return dbOff;

}


//================================================================
  int DB_add__ (int typ, double *data, int ptNr, int dbi) {
//================================================================
// DB_add__                    add geom. obj into DB          DUMMY
 
  int objID = AP_OBJ_ID;

  ++AP_OBJ_ID;

  return objID;

}
 



//==============================================================
  double DB_GetVar (long Ind) {
//==============================================================
// DB_GetVar    get value variable; DUMMY
  static double d1 = 12.34;

  return d1;

}


// EOF

