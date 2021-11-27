/* ./gl_font1.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

load vector-font-1

=====================================================
List_functions_start:

font1_load_char           get <lNr> 2D-lines for character <ichar>

main                      testprog only for font1_load_char

List_functions_end:
=====================================================

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>


#include "../ut/geo.h"                    // Point
#include "../gr/gr.h"                     // GR_SclFontX GR_SclFontY
#include "../gl/gl_font1.h"               // GL_vfont1




//==================================================================================
  int font1_load_char (float cvDat[], int *lNr, int ichar) {
//==================================================================================
// font1_load_char           get <lNr> 2D-lines for character <ichar>
// the nr of characters is 96 (32-128) - the first char is the blank (32)
// cvDat - maxSize SIZ_FONT_DAT = (SIZ_FONT_DAT / 6)  3D-lines
// z-coord is always 0.f
// Input:
//   ichar   index character to load (0 = first = blank)
// Output:
//   cvDat   lines in groups of 4 floats
//   lNr     nr of lines
//  retCode  1 = end of data
//           0 = OK
//           <0 = error


  int     irc, ilen, icv;
  char    *p0, *p1, *p2, iact, ilast;


  // printf("font1_load_char %d\n",ichar);


  *lNr = 0;
  icv = 0;   // next free index in cvDat
  irc = 0;

  // get p1 = string for char ichar (fontChar i1+32)
  p0 = GL_vfont1[ichar];
  if(p0 == NULL)  {irc = -1; goto L_err;}
  p1 = p0;
  if(*p1 == 'x') {irc = 1; goto L_exit;}
    ilen = strlen(p1);


  L_Next:
    while (*p1==' ') { ++p1; }  // skip blanks
    if((p1 - p0) >= ilen) goto L_exit;

    iact = *p1;
    ++p1;

    if(iact == 'm') {
      cvDat[icv] = strtof (p1, &p2); p1 = p2; ++p1;   // x-val
      cvDat[icv] /= GL_vf1_CW;
      ++icv;
      cvDat[icv] = strtof (p1, &p2); p1 = p2; ++p1;  // y-val
      cvDat[icv] /= GL_vf1_CH;
      ++icv;
      cvDat[icv] = 0.f;
      ++icv;
      if(icv >= SIZ_FONT_DAT) {irc = -3; goto L_err;}
      ilast = iact;
      goto L_Next;
    }

    if(iact != 'l') {irc = -2; goto L_err;}

    if(ilast == 'l') {
      if(icv < 3) {irc = -4; goto L_err;}
      // copy last endPt = new startPt
      memcpy(&cvDat[icv], &cvDat[icv-3], sizeof(float) * 3); // copy last endPt
      icv += 3;
    }

    // get and copy endPoint
    cvDat[icv] = strtof (p1, &p2); p1 = p2; ++p1;  // x-val
    cvDat[icv] /= GL_vf1_CW;
    ++icv;
    cvDat[icv] = strtof (p1, &p2); p1 = p2; ++p1;  // y-val
    cvDat[icv] /= GL_vf1_CH;
    ++icv;
    cvDat[icv] = 0.f;
    ++icv;
    if(icv >= SIZ_FONT_DAT) {irc = -3; goto L_err;}
    ilast = iact;
    goto L_Next;


  L_exit:
    *lNr = icv / 6;

      // TESTBLOCK
      // printf(" ex-font1_load_char %d %d '%c'lNr=%d\n",irc,ichar,ichar+32,*lNr);
      // for(int i1=0; i1<icv; i1+=3)
        // printf(" %d %9.2f %9.2f %9.2f\n",i1,cvDat[i1],cvDat[i1+1],cvDat[i1+2]);
      // END TESTBLOCK

    return irc;


  L_err:
    TX_Error("***** ERROR font1_load_char E-%d ichar=%d",irc,ichar);
    return irc;

}


//################################################################
#ifdef OFFLINE
//================================================================
  int main () {
//================================================================
// test only
// load font into float-structs.
// the nr of characters is 96 (32-128) - the first char is the blank (32)
// the nr of lines per character is lNr
// cc gl_font1.c ut_TX.c -ggdb -pg -DOFFLINE&&./a.out

  int     i1, lNr;
  float   cvDat[SIZ_FONT_DAT];



  printf("main-gl_font1.c\n");

  for(i1=0; i1<97; ++i1) {
    font1_load_char (cvDat, &lNr, i1);
  }

  return 0;

}


#endif
//################################################################

// EOF
