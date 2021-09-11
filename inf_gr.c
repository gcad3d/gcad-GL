/* ./inf_gr.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Dokumentation for gcad-GL

Contains infotext only, no code.
Use it with "vi -t <code>"
  vi -t INF

make does reCreate all tag-files
  (does ctags -f ../tags/<fnam>.tag ../<fnam>.c)
//   (does ctags -f ../tags/<fnam>.tag ../inf/<fnam>.c)

HOWTO ADD new tags:
- tag must start with INF_
- tag may NOT have  a '-' - character

HOWTO ADD new tagfile:
- add <newTagfileName>.c
// - add in ../inf/<newTagfileName>.c



*/
#ifdef OFFLINE
void INF (){/*

INF_modules
INF_workflows
INF_CoordinateSystems



=========================================================================== */}
void INF_modules (){        /*

APP
  GUI      GR      DL
  Gtk      GL


APP
  GUI:     user-interface-functions independent from GUI-driver (eg Gtk)
    Gtk:     Gtk3 or Gtk2

APP
  GR:      grafic-functions independent from drivers (Gtk, OpenGL)
    GL:      OpenGL3 or OpenGL1

APP
  DL:      Displaylist

//----------------------------------------------------------------


=========================================================================== */}
void INF_workflows (){        /*

Startup:
GR_init0
GUI_init
  Gtk_CB_realize            OpenGL-window comes up
    GL_init                   init all shaders
      GL_shadSUn_init           init shader shSUn
      GUI_CB_win_is_up          report to app that grafic window is up


Create_objects:
GR_perm_sur              add permanent surface to grafic-system
  GL_add_sur               add surface to displayList and grafic-system
    DL_add__                 add new DL-record


Redraw:
GR_redraw
  GL_render__
    GL_shadSUn_render


Selection:
Gtk_CB_button            mouse-button pressed
  GUI_CB_mouse
    GR_sele__                analyze grafic-selection
      GUI_CB_sele



=========================================================================== */}
void INF_CoordinateSystems (){        /*


MCS - OpenGL-Inputcoordinates:
             -1,1 --------- 1,1
                 |         |
                 |   0,0   |
                 |         |
            -1,-1 --------- 1,-1

SCS - screenCoords
   0,GR_sizWinY_px --------- GR_sizWinX_px,GR_sizWinY_px
                 |         |
                 |         |
                 |         |
              0,0 --------- GR_sizWinX_px,0




MCS              modelCoords    float; OpenGL-Inputcoordinates
SCS              screenCoords   int, pixels, 2D. LowLeft=0,0; UpperRight=maxX,maxY
WCS              worldCoords    double, absolut usercoords. DB keeps worldCoords.
UCS              userCoords     double; relativ to the active constrPln

eyeVecWCS =
cenSCS = SCS-maxX/2, SCS-maxY/2
cenWCS = intersectionPoint eyeVecWCS - planeWCS


plnWCS           always center=0,0,0; vcx=0,0,1; vcy=0,1,0
plnUCS           the active constrPln; initally plnWCS
plnEye           center = screenCenter (cenSCS = SCS-maxX/2, SCS-maxY/2)
                 vcx = vcxEye, vcy = vcyEye, vcz = vczEye

vcxEye           vector parallel to horizontal windowBorder (plnEye.vx)
vcyEye           vector parallel to vertical windowBorder   (plnEye.vy)
vczEye           vector from the screenCenter to the eye;   (plnEye.vz)


Variables:
GR_sizWinX_px     size horizontal of grafic-window in SCS
GR_sizWinY_px     size vertical of grafic-window in SCS

GR_cenRot        model is rotated around this point (WCS)
GR_cenView       screenCenter in WCS

GR_view_vx
GR_view_vz

GR_angZ          angle used to rotate model around SCS-vector 1,0
GR_angX          angle used to rotate model around plnUCS.vz


Functions:
GR_ptSCS_ptWCS   ???

Matrix:
?                MCS from WCS
?                MCS from SCS  TODO


=========================================================================== */}

#endif

// EOF
