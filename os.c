/* ./os.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Operating-system-functions
  


=====================================================
List_functions_start:

OS_system                Perform OS-Command; wait for completion (system)
OS_checkFilExist         check if File or Directory exists
OS_sys1                  get systemCommand (popen); skip if starting with "##"

OS_get_scrRes            get total screensize in pixels and millimetres
OS_ck_libcVersion        check C-library version

List_functions_end:
=====================================================

*/

#ifdef globTag
void OS(){}
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>              // for ...

#include <unistd.h>          ///f. access, R_OK ..
#include <errno.h>
#include <wordexp.h>         // OS_filnam_eval
#include <signal.h>


#include "./ut.h"            // TX_Error
#include "./os.h"            // OS_..



//======================== Perform OS - Command =========
  int OS_system (char *buf) {
//======================== Perform OS - Command =========
// OS_system                  Perform OS-Command; wait for completion (system)
//   do not wait: use OS_exec

// Spezialversion fuer AIX + CATIA.


  void *catch;
  int ret;

  catch = signal(SIGCLD, SIG_DFL);
  ret = system(buf);

  signal(SIGCLD, catch);
  if (ret) { perror(buf); }

  return(ret);

}


//=============================================================
  int OS_checkFilExist (char* filnam, int mode) {
//=============================================================
// OS_checkFilExist         check if File or Directory exists
//   filnam may not have shell-variables; see OS_filnam_eval
// mode = 0: display message sofort;
// mode = 1: just fix returncode, no message
// mode = 2: make Errormessage (TX_Error) if File does not exist
// 
// rc = 0 = Fehler; datei existiert NICHT.
// rc = 1 = OK, Datei existiert.


  char    buf[256];


  // printf("OS_checkFilExist |%s| %d\n", filnam, mode);

  // remove following "/"
  strcpy (buf, filnam);

  // if last char of string == '/': remove it
  if(buf[strlen(buf)-1] == '/') buf[strlen(buf)-1] = '\0';


  if ((access (buf, R_OK)) != 0) {
    if (mode == 0) {
      // printf ("*** Error OS_checkFilExist: %s does not exist\n",filnam);
      // MSG_get_1 (buf, 256, "NOEX_fil", "%s", filnam);
      TX_Error ("*** Error OS_checkFilExist: %s\n", buf);
      /* UI_Dialog (NULL, buf); */

    } else if (mode == 2) {
      TX_Error(" - File %s does not exist ..",filnam);
      // MSG_err_1 ("NOEX_fil", "%s", filnam);
    }

    return 0;
  }

  // printf("ex OS_checkFilExist YES |%s| %d\n",filnam,mode);
  return 1;

}


//================================================================
  int OS_ck_libcVersion (int vMaj, int vMin) {
//================================================================
// test if new software with Lib-version <versionLinked> can be used.
// <versionLinked> must be equal or less then the active Lib-version.
// View the active GNU-Lib-version in a shell with "/lib/libc.so.6"
// Mandriva 2009 reports "glibc 2.8"
// Input:
//   versionLinked  of the software at the time of binding
// RetCod:  0       OK; active-libc-version is >= <versionLinked>
//         -1       Error; active-libc-version is < <versionLinked>

  // static float versionLinked = 2.8;

  char   cBuf[256], *p1;
  size_t i1;
  int    iv1, iv2;


  // printf("OS_ck_libcVersion %d %d\n", vMaj, vMin);


  //  GNU C library only: get GNU C library version
  i1 = confstr (_CS_GNU_LIBC_VERSION, NULL, 0);
  if(i1 > 256) i1 = 256;
  confstr (_CS_GNU_LIBC_VERSION, cBuf, i1);
    printf("GNU_LIBC_VERSION = |%s|\n", cBuf);
  p1 = strchr (cBuf, ' ');
  if(!p1) return -1;
  iv1 = atoi (p1);
  p1 = strchr (cBuf, '.');
  if(!p1) return -1;
  ++p1;
  iv2 = atoi (p1);
    // printf(" _libcVersion %d %d\n", iv1, iv2);


  if(iv1 < vMaj) return -1;

  if(iv1 == vMaj) {
    if(iv2 < vMin) {
      return -1;
    }
  }

  return 0;

}


//=============================================================================
  void OS_get_scrRes (int *xSiz_px, int *ySiz_px, int *xSiz_mm, int *ySiz_mm) {
//=============================================================================
// OS_get_scrRes                    get total screensize in pixels and millimetres

  int  irc;
  char s1[256], *p1;


  // irc = OS_sys1 (s1, 80, "xdpyinfo | awk '/dimensions/ {print $2}'");
  irc = OS_sys1 (s1, sizeof(s1), "xdpyinfo | awk '/dimensions/'");

  if(irc < 0) goto L_err;

  p1 = strstr(s1, "dimensions");
  if(!p1) goto L_err;
  p1 += 10;
  p1 = strchr(p1, ' ');
  if(!p1) goto L_err;
    printf(" s1-3=|%s|\n",p1);
  sscanf(p1, "%dx%d", xSiz_px, ySiz_px);

  p1 = strchr(p1, '(');
  if(!p1) goto L_err;
  ++p1;
  sscanf(p1, "%dx%d", xSiz_mm, ySiz_mm);
  goto L_exit;


  L_err:
    TX_Write("***** ERROR OS_get_scrRes - reset to 1920 X 1080");
    *xSiz_px = 1920;
    *ySiz_px = 1080;
    *xSiz_mm = 508;
    *ySiz_mm = 285;
   
  L_exit:
      printf("OS_get_scrRes %d %d %d %d\n",*xSiz_px,*ySiz_px,*xSiz_mm,*ySiz_mm);
    return;
}


//================================================================
  int OS_sys1 (char *sOut, int sSiz, char *cmd) {
//================================================================
/// \code
/// OS_sys1                  get systemCommand (popen); skip if starting with "##"
/// RetCod:
///     >0      OK, nr of chars returned in sOut
///    -1       cannot open pipe
///    -2       sOut too small
///    -3       execution-error
/// \endcode

  int   irc=0, ii=0, i1;
  FILE  *fPip1;
  char  s1[256], *p1;


  // printf("OS_sys1 |%s| %d\n",cmd,sSiz);

  sOut[0] = '\0';
  fflush(stdout);
  errno = 0;
  // dup2(STDOUT_FILENO, STDERR_FILENO);

  fPip1 = popen (cmd, "r");
  if (fPip1 == NULL) {irc = -1; goto L_exit;}
    // fprintf(stderr, "_sys1-1 errno = %d\n", errno);

  while (1 == 1) {
    if (fgets (s1, 255, fPip1) == NULL) break;
    if((s1[0] == '#')&&(s1[1] == '#')) continue;
    i1 = strlen(s1);
    ii += i1 + 1;
      // printf(" s1=|%s| i1=%d ii=%d\n",s1,i1,ii);
    if(ii >= sSiz) {irc = -2; pclose (fPip1); goto L_exit;}
    strcat(sOut, s1);
  }

  pclose (fPip1);
  // UTX_CleanCR (sOut);  // remove LF from last word
  irc = strlen(sOut);
    // printf(" _sys1-out %d |%s|\n",irc,sOut);


  // test errno for execution-error
  if(errno) {
      // fprintf(stderr, "_sys1-2 errno = %d\n", errno);
    strcpy(sOut, "Error: ");
               // 0123456
    strerror_r (errno, &sOut[7], sSiz - 7);
    TX_Print (sOut);
    return -3;
  }

  L_exit:
    // printd("ex-OS_sys1 %d |%s|\n",irc,sOut);
  return irc;

}


// EOF
