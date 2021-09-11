/*  ./ut_TX.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================


write messages, errors
console-version, not GUI-Version

-----------------------------------------------------
TODO:
  ..

-----------------------------------------------------
Modifications:
  ..

-----------------------------------------------------

=====================================================
List_functions_start:

TX_Print
TX_Error
TX_Write

List_functions_end:
=====================================================

\endcode *//*----------------------------------------


Compile gcc:
cc -c ut_TX.c

*/


#ifdef _MSC_VER
#define _CRT_SECURE_NO_DEPRECATE
#endif


#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>


#define ON                 0
#define OFF                1


#include "./ut.h" 


/*  alle Ausgaben EIN */
int   TX_mode = ON;







//==========================================================
  void TX_Print (char* txt, ...) {
//==========================================================
// formatted write into messagewindow.
// Example:
//   TX_Print (".. distance is %lf", d1);


  va_list va;
  char    TX_buf1[1024];


  if(TX_mode == OFF) {
    return;
  }

  va_start(va,txt);

  vsprintf(TX_buf1,txt,va);

  va_end(va);

  if(strlen(TX_buf1) > 100) {
    TX_buf1[100] ='\0';
  }

  TX_Write (TX_buf1);

}


//==========================================================
  void TX_Error (char* txt, ...) {
//==========================================================
// formatted errortext into messagewindow.
// Example:
//   TX_Print (".. distance must not exceed %lf", d1);


  char   TX_buf1[1024];
  va_list va;

  va_start(va,txt);

  strcpy(TX_buf1, "*** Fehler: ");
                // o123456789012
  vsprintf(&TX_buf1[12],txt,va);
  va_end(va);
  TX_Write (TX_buf1);

}


//==========================================================
  void TX_Write (char* txt) {
//==========================================================
//  direkt rausschreiben.


  printf("%s\n",txt);

  if(TX_mode == OFF) {
    return;
  }

}


// EOF
