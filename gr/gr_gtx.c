/* ../gr/gr.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

grafic-text-functions independent from drivers (Gtk, OpenGL)

=====================================================
List_functions_start:

GR_gxt_prep1           prepare dimText
GR_gxt_subst1          substitute [% by text
GR_gtx_ckBlockWidth    get width of textblocks in Usercoords
GR_gxt_strLen          get stringlength of textnote
GTX_chw_               get width character + intercharspace

List_functions_end:
=====================================================

*/

#ifdef globTag
 void GR_gxt(){}
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>                    // va_list va_arg
#include <math.h>

#include "../ut/ut.h"                      // TX_Error
#include "../ut/utx.h"                     // UTX_add_fl_f
#include "../db/db.h"                      // DB_..


#define extern  // does eliminate "extern"
#include "../gr/gr_gtx.h"                    // GR_tx_nkNr
#undef extern

#include "../app/aped.h"                   // APED_dbo_oid
#include "../ut/TX_.h"                    // TX_Print ..



//================================================================
  double GTX_chw_ (double size) {
//================================================================
// GTX_chw_         get width character + intercharspace
// #define GTX_chw_ (scale) (GR_tx_scale * (scale))

  // printf("GTX_chw_ %f\n",size);
  // printf("  GR_tx_chw=%f scale=%f fontSiz=%f\n",
    // GR_tx_chw,GR_tx_scale,GR_fontSiz);
  // printf("ex GTX_chw_ %f\n",GR_tx_chw * GR_tx_scale * GR_fontSiz * size);

  printf(" ***************** TODO: GTX_chw_\n");

  return 5.;

  // return GR_tx_chw * GR_tx_scale * GR_fontSiz * size;

}


//================================================================
  double GR_gtx_ckBlockWidth (char *txt, double size) {
//================================================================
// GR_gtx_ckBlockWidth         get width of textblocks in Usercoords

// sollte nach dem subst [% erfolgen !!
// TODO: should use GR_gtx_BlockWidth__


  int    chrNr;
  double dx;


  // i1 = strlen(txt) - UTX_cnr_chr (txt, '[');
  chrNr = GR_gxt_strLen (txt);

  // dx = chrNr * GR_tx_chw * GR_tx_scale * size;
  dx = chrNr * GTX_chw_ (size);

    // printf("ex GR_gtx_ckBlockWidth bw=%lf chrNr=%d\n",dx,chrNr);

  return dx;

}


//================================================================
  int GR_gxt_prep1 (char *txo, double d1) {
//================================================================
// GR_gxt_prep1          prepare dimText


  txo[0] = '\0';
  UTX_add_fl_f (txo, d1, GR_tx_nkNr);
  UTX_del_foll0 (txo);

  return 0;

}


//================================================================
  int GR_gxt_subst1 (char *txo, char *fmt, char *tx1) {
//================================================================
// GR_gxt_subst1         substitute [% by text
//
// [%   wird durch Masztext ersetzt. Wenn im Text kein [% enthalten ist,
//      wird der Masztext ganz an den Textbeginn gestellt.
// [-   keinen Masztext einfuegen.
// [V#] durch numer. Variable ersetzen
//
// fmt: "Hor. [% mm"
// tx1: "55"
// txo: "Hor. 55 mm"
// Retcod: BlockColor; 0=keine, 1-9 ist Farbe und wird als Block dargestellt.

  int    irc, i1, i2, i3, ilen;
  long   dbi;
  double d1;
  char   c1, *p1;


  // printf("GR_gxt_subst1 |%s|%s|\n",fmt,tx1);

  txo[0] = '\0';
  irc = 0;


  // BlockColor muss ganz am Beginn stehen:
  // see GR_gxt_nkNr
  if(fmt == NULL) goto L_add_tx1;
  if(fmt[0] == '[') {
    if((fmt[1] >= '0')&&(fmt[1] <= '9')) {
      irc = fmt[1] - '0';
      fmt += 2;
    }
  }


  // test if to start with tx1
  // wenn fmt kein "[%" und kein "[-" enthaelt: tx1 ganz an den Beginn.
  // L_start:
  if(strstr(fmt, "[-") != NULL) goto L_add_fmt;
  if(strstr(fmt, "[%") != NULL) goto L_add_fmt;



  // start with txt1
  L_add_tx1:
  if(tx1 == NULL) goto L_add_fmt;
  strcat(txo, tx1);


  // add fmt;  exclude "[-"; substitute "[%" by tx1
  L_add_fmt:
  if(fmt == NULL) goto L_exit;

  i1   = 0;
  ilen = strlen(fmt);


  //----------------------------------------------------------------
  NextChar:
  c1 = fmt[i1];
  // printf(" next char i1=%d c1=%d/%c\n",i1,c1,c1);

  if(c1 == '[') {                 // SONDERZEICHEN
    ++i1;
    if(i1 >= ilen) goto L_parErr;
    c1 = fmt[i1];

    if(c1 == '-') {                   // skip
      ++i1;
      goto NextChar;

    } else if(c1 == '%') {            // substitute
      ++i1;
      strcat(txo, tx1);
      goto NextChar;

    } else if(c1 == 'V') {            // expand parameter
      irc = APED_dbo_oid (&i2, &dbi, &fmt[i1]);
      if(irc < 0) goto L_parErr;
      p1 = strchr (&fmt[i1], ']');
      if(p1 == NULL) goto L_parErr;
      d1 = DB_GetVar (dbi);
      // printf(" V[%d]=%f\n",dbi,d1);
      UTX_add_fl_u (txo, d1);
      i1 += p1 - &fmt[i1];
      ++i1; // skip "]"
      goto NextChar;

    } else {                          // copy
      strcat(txo, "[");
    }
  }
  strncat(txo, &c1, 1);
  ++i1;
  if(i1 < ilen)  goto NextChar;


  L_exit:
  // printf("ex GR_gxt_subst1 |%s| %d\n",txo,irc);
  return irc;


  L_parErr:
  TX_Error("GR_gxt_subst1 E001");
  return -1;

}


//=================================================================
  int GR_gxt_dim_upd (double txsiz, int nkNr) {
//=================================================================
// GR_gxt_dim_upd           Graf.FontParameter setzen
// INPUT:
//   txsiz      size of text for dimensions
//   nkNr       nr of chars following decimal-char; -1=do-not-change
// was GR_InitGFPar


  printf("GR_gxt_dim_upd txsiz=%f nkNr=%d\n",txsiz,nkNr);
  // printf("GR_gxt_dim_upd txsiz=%f GR_tx_scale=%f\n",txsiz,GR_tx_scale);
  // printf(" AP_txdimsiz=%f\n",AP_txdimsiz);
/*
  // Characterwidth + charSpace
  GR_tx_chw   = GL_vf1_CW + GL_vf1_CS;

  // Characterheigth + lineSpace
  GR_tx_chh   = GL_vf1_CH + GL_vf1_LS;

  // Size Arrowhead in X-Dir
  GR_tx_ardx  = GR_tx_scale * GR_fontSiz * txsiz * 20.;

  // Size Arrowhead in Y-Dir
  GR_tx_ardy  = GR_tx_ardx * 0.25;

  // overlength of leaderlines of dimensions
  GR_tx_ldExt = GR_tx_ardx;

  // nr of chars following decimal-char; -1=do-not-change
  if(nkNr >= 0)
    GR_tx_nkNr = nkNr;
*/

  return 0;

}


// EOF
