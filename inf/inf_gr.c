/* ./inf_gr.c
//================================================================
Copyright 2021 Thomas Backmeister, Franz Reiter, Karl Sauer - support@gcad3d.org

See the file "LICENSE_GPLv3.txt" for information on usage and redistribution
of this file, and for a DISCLAIMER OF ALL WARRANTIES.
//================================================================

Dokumentation for gcad-GL

Contains infotext only, no code.
Editors can jump into following functions - eg INF_workflows
with vim:
vi -t <code>
  vi -t INF


Example vim:
add into ~/.vimrc:
let tagfiles = system("ls -m tags/*.tag |sed \"s/ //g\"")
let &tags = substitute(tagfiles, "\n", "", "g")

Jump direct into function - eg INF_workflows:
vi -t INF_workflows

jump from inside editor into function:
set cursor into word INF_workflows and key Ctrl ]
return with Ctrl O


make does reCreate all tag-files
  (does ctags -f tags/<fnam>.tag ../<fnam>.c)
//   (does ctags -f tags/<fnam>.tag ../inf/<fnam>.c)

HOWTO ADD new tags:
- tag must start with INF_
- tag may NOT have  a '-' - character

HOWTO ADD new tagfile:
- add <newTagfileName>.c
// - add in <newTagfileName>.c


Format include // at 40

*/
#ifdef OFFLINE
void INF (){/*

INF_modules
INF_workflows
INF_CoordinateSystems
INF_matrix
INF_symbols
INF_shaders

INF_debug


=========================================================================== */}
void INF_modules (){        /*

AP 
  GUI      GR      DL
  Gtk      GL


AP       application, main
  GUI:     user-interface-functions independent from GUI-driver (eg Gtk)
    Gtk:     Gtk3 or Gtk2

  GR:      grafic-functions independent from drivers (Gtk, OpenGL)
    GL:      OpenGL3 or OpenGL1

  DL:      Displaylist

//----------------------------------------------------------------

../gui_gtk3/gtk3.c      Gtk3_..




=========================================================================== */}
void INF_workflows (){        /*

Startup:
GR_init0
GUI_init
  Gtk_CB_realize            OpenGL-window comes up
    GL_init                   init all shaders
      GL_shadSUn_init           init shader shSUn
      GUI_CB_win_is_up          report to app that grafic window is up


//----------------------------------------------------------------
Create_objects:
AP_obj_add_perm
  DB_add__
  GR_perm_sur              add permanent surface to grafic-system
    GL_add_sur               add surface to displayList and grafic-system
      DL_add__                 add new DL-record

AP_obj_add_dyn
  GR_dyn_sym2
    DL_SYD_add__

AP_obj_add_temp            TODO


//----------------------------------------------------------------
Display:

Gtk_CB_realize      // grafic-win is up
  GL_init
    GUI_CB_win_is_up
      AP_CB_win_is_up
        GR_modsiz_set

Gtk_CB_resize       // resize window
  GR_resize           // 
    GL_view_ortho
  GL_viewport         // glViewport
    glViewport

Gtk_CB_motion      // move objects with mouse
  GR_do_pan          // get center of window as rotation-center
    GR_view_cen_set

Gtk_CB_motion      // rotate objects with mouse
  GR_do_rot          // get new rotation-angle (Z-angle, tilt-angle)

Gtk_CB_scroll      // zoom objects

GL_view_update     // update view after GR_do_pan, GR_do_rot, GR_resize



//----------------------------------------------------------------
Redraw:
GR_redraw
  GL_render__
    GL_shadSUn_render        // disp shader surfaces with normals
    GL_shCV_render ();     // disp point/curves (polygons)
    GL_shSY2_render ();      // disp dynamic-objects
    GL_shSY2_axis ();        // display axis-systems


//----------------------------------------------------------------
Selection:
Gtk_CB_button            mouse-button pressed
  GUI_CB_mouse
    GR_sele__                analyze grafic-selection
      GUI_CB_sele



=========================================================================== */}
void INF_CoordinateSystems (){        /*


NDC - OpenGL-normalized-device-coordinates
             -1,1 --------- 1,1
                 |         |
                 |   0,0   |
                 |         |
            -1,-1 --------- 1,-1

SCS - screenCoords (updated - Y-input from grafic-card is 0 on top)
   0,GR_sizWinY_px --------- GR_sizWinX_px,GR_sizWinY_px
                 |         |
                 |         |
                 |         |
              0,0 --------- GR_sizWinX_px,0




NDC              OpenGL-normalized-device-coordinates; -1 to +1
SCS              screenCoords   int, pixels, 2D. LowLeft=0,0; UpperRight=maxX,maxY
WCS              worldCoords    double, absolut usercoords. DB keeps worldCoords.
UCS              userCoords     double; relativ to the active constrPln

eyeVecWCS =
cenSCS = SCS-maxX/2, SCS-maxY/2
cenWCS = intersectionPoint eyeVecWCS - planeWCS


plnWCS           always center=0,0,0; vcx=0,0,1; vcy=0,1,0
GR_constrPln     the active construction-plane; initally plnWCS
// plnEye           center = screenCenter (cenSCS = SCS-maxX/2, SCS-maxY/2)
//                  vcx = vcxEye, vcy = vcyEye, vcz = vczEye

GR_eyeX          vector parallel to horizontal windowBorder (WCS)
GR_eyeY          vector parallel to vertical windowBorder   (WCS)
GR_eyeZ          vector from the screenCenter to the eye;   (WCS)

GR_sizWinX_px     size horizontal of grafic-window in SCS
GR_sizWinY_px     size vertical of grafic-window in SCS

GR_cenRot        model is rotated around this point (WCS)
GR_cenView       screenCenter in WCS

GR_view_vx
GR_view_vz

GR_angZ          angle used to rotate model around GR_eyeZ
GR_angX          angle used to rotate model around GR_eyeX


Functions:
GR_ptSCS_ptWCS   ???

Matrix:
?                WCS from SCS  TODO



=========================================================================== */}
void INF_matrix (){        /*
===========================================================================


C-code reads left-to-right first and then top-to-bottom = row-major order
OpenGL matrix is a one-dimensional array listed in column-major order.

M44..       4x4-matrix
M..D_       doubles - row-order
M..FC_      floats - column-order


//----------------------------------------------------------------
VARIABLES:

GR_matd_scl        // modelViewMatrix ortho - scale down to NDC

GR_matd_mdl[0]    // UU
GR_matd_mdl[1]    // translate to screen-center
GR_matd_mdl[2]    // rotate around model-Z-axis (in WCS always 0,0,1)
GR_matd_mdl[3]    // rotate around GR_view_vx (vector parallel to horizontal windowboundary)


mdm = GR_matd_mdl[1]*GR_matd_mdl[2]*GR_matd_mdl[3] // view-coords from world-coords
GR_matd_view = GR_matd_scl * mdm      // matrix device-coords from world-coords
GR_matf_dev = GR_matd_view            // float-column-order from double-row-order
out GR_matf_dev * point

mvpf      // float-column-order identity


//----------------------------------------------------------------
Files:
../ut/matrix.c

//----------------------------------------------------------------
Functions:
GL_view_update     sets GR_matd_mdl



=========================================================================== */}
void INF_symbols (){        /*
===========================================================================

0-96 = characters (eg index for 'z' is ('z' - 32))
iSym_ln     97
iSym_pln
iSym_tria
iSym_arh2
iSym_arh3



//----------------------------------------------------------------
Functions:
GL_font1_chr_load     load vectorfont into buffer
GL_symbols_load       load symbols into buffer


=========================================================================== */}
void INF_shaders (){        /*
===========================================================================

VS:
- VS (vertex-shader)

GS:
- GS (geometry-shader) follows VS
- handles points triangles or lines
- input is ALL vertices of obj (point line or triangle)
    for(int i=0; i<3; i++) { gl_Position = gl_in[i].gl_Position; ..
- can eg compute normal-vector of triangle (has all 3 vertexes)
- output: .. EmitVertex(); } EndPrimitive();
- in GS: no gl-functions (eg glLineWidth())
- can output modified object, additional objects or skip object;

FS:
- FS (fragment-shader) follows GS
- single call for every pixel



=========================================================================== */}
void INF_debug (){        /*
===========================================================================

gdb a.out

or:

edit file ../app/gdbinit
and use with:
gdb -x ./gdbinit a.out




=========================================================================== */}
#endif

// EOF
