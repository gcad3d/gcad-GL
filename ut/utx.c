/* ./utx.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

text handling

=====================================================
List_functions_start:

UTX_CleanCR             Delete Blanks, CR's and LF's at end of string
UTX_del_foll0           Delete following 0's and following "."

UTX_cnr_chr             returns nr of char c1 in string txt

UTX_add_fl_u            add double unformatted (del foll. 0's and ".")
UTX_add_fl_f            add double with <nkAnz> digits after dec.point

List_functions_end:
=====================================================

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>


#include "../ut/ut_types.h"                // FLT_32_MAX
#include "../ut/utx.h" 


//================================================================
  int UTX_cnr_chr (char *txt, char c1) {
//================================================================
// UTX_cnr_chr          returns nr of char c1 in string txt


  int   iNr;
  char  *p1;

  iNr = 0;

  p1 = txt;

  while (*p1) {
    if(*p1 == c1) ++iNr;
    ++p1;
  }

  return iNr;

}


//==============================================================
  int UTX_del_foll0 (char strBuf[]) {
//==============================================================
// UTX_del_foll0       Delete following 0's and following ".".


  char *tpos;

  // printf("UTX_del_foll0 |%s|\n",strBuf);


  // search for '.'; if none: nothing to do ..
  tpos = strchr (strBuf, '.');
  if(!tpos) return 0;

  // delete following 0's
  tpos = &strBuf[strlen (strBuf)];
  --tpos;
  while (*tpos == '0') --tpos;

  if(*tpos == '.') goto L_done;               // delete following .

  // if(isdigit(*tpos)) {++tpos; goto L_done;}   // keep this dig; eg "0.5"
  // // now we have the char before '0'; must keep this 0 !
  // ++tpos;
  ++tpos;

  L_done:
  *tpos = '\0';

    // printf("ex UTX_del_foll0 |%s|\n",strBuf);

  return 0;
}


//==============================================================
  int UTX_add_fl_u (char strBuf[], double zahl) {
//==============================================================
// UTX_add_fl_u            add double unformatted (del foll. 0's and ".")
// 
//   Delete following 0's and following ".".
// see UTX_add_fl_f UTX_db10__


  double u1;

  // Die Zahl -0.0 auf 0.0 korrigieren
  u1 = fabs(zahl);
  if(u1 < FLT_32_MIN1) { strcat (strBuf, "0"); return 0; }
  if(u1 > FLT_32_MAX)  {
      if(zahl < 0.) { strcat (strBuf, "-9999999999999.999"); return -1; }
    else            { strcat (strBuf, "9999999999999.999"); return 1; }
                                  //   123456789012345678
  }


  sprintf (&strBuf[strlen(strBuf)],"%lf", zahl);

  // delete following 0's
  UTX_del_foll0 (strBuf);


  // printf("UTX_add_fl_u |%s|\n",strBuf);

  return 0;
}


//==============================================================
  int UTX_add_fl_f (char strBuf[], double zahl, int nkAnz) {
//==============================================================
// UTX_add_fl_f            add double with <nkAnz> digits after dec.point
//   to string. The nr of digits before dec.point is floating.
//
// IGES verwendet %.10f !
// 
// see UTX_del_foll0


  char   auxBuf[40], fmtBuf[16];
  double u1;


  // correct -0.0 -> 0.0
  u1 = fabs(zahl);
  if(u1 < FLT_32_MIN1) { strcat (strBuf, "0"); return 0; }
  if(u1 > FLT_32_MAX)  {
      if(zahl < 0.) { strcat (strBuf, "-9999999999999.999"); return -1; }
    else            { strcat (strBuf, "9999999999999.999"); return 1; }
                                  //   123456789012345678
  }

  // create Formatstring
  sprintf (auxBuf, "%.*f", nkAnz, zahl);

  strcat (strBuf, auxBuf);

    // printf("UTX_add_fl_f |%s|\n",strBuf);

  return 0;
}



//===========================================================
  char* UTX_CleanCR (char* string) {
//===========================================================
/// \code
/// UTX_CleanCR              Delete Blanks, CR's and LF's at end of string
/// returns positon of stringterminator \0
/// 
/// see also UTX_del_follBl UTX_CleanSC
/// \endcode


  int  ilen;
  char *tpos;

  ilen = strlen (string);

  tpos = &string[ilen];

  if(ilen < 1) goto L_exit;

  --tpos;


  while ((*tpos  == ' ')  ||
         (*tpos  == '\t') ||          /* tab */
         (*tpos  == '\n') ||          /* newline */
         (*tpos  == '\r'))   {        /* CR */

    *tpos    = '\0';
    if(tpos <= string) goto L_exit;
    --tpos;
  }

  ++tpos;


  L_exit:
  // printf("ex UTX_CleanCR |%s|\n", string);

  return tpos;
}


// EOF
