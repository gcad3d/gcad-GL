/* ../app/aped.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================


=====================================================
List_functions_start:

APED_dbo_oid        give typ, index from text "P12"      DUMMY

List_functions_end:
=====================================================

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include "../ut/types.h"                   // application-types
#include "../app/aped.h"                   //


//================================================================
  int APED_dbo_oid (int *defTyp, long *defInd, char* txtIn) {
//================================================================
// APED_dbo_oid                get type and dbi from object-ID;
// eg returns defTyp=Typ_PT and defInd=20 for "P20"
// Returns also this constantObjects: DX DY DZ DIX DIY DIZ RX RY RZ
// see also AP_cut_defHdr
// 
// Input:
//   txtIn
//
// Output:
//   defTyp: see wincut_base.h
//           values return 0 !
//   defInd
// 
// Retcodes:
//   0 = OK
//  -1 = only defTyp (eg P(..) or D(..))
//  -2 = Error
//
// TODO: add into DBO_dbo_src__


  int   rc, i1, ilen;

  *defTyp = Typ_VAR;
  *defInd = 1;

  return 0;

}


// EOF
