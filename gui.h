/* ./gui.h
*/

#define GUI_KeyEsc          0xff1b           // 65307
#define GUI_KeyShift_L      0xffe1           // 65505
#define GUI_KeyControl_L    0xffe3           // 65507  Control-key left


#define TYP_EventPress      402  ///< event press (key)
#define TYP_EventRelease    403  ///< event release (key)

#define GUI_MouseL          1  ///< left mousebutton
#define GUI_MouseM          2  ///< mid mousebutton
#define GUI_MouseR          3  ///< right mousebutton
#define GUI_Mouse2L         4  ///< doubleclick
#define GUI_MouseScUp       5  ///< mouse scroll up
#define GUI_MouseScDown     6  ///< mouse scroll down


//----------------------------------------------------------------
int GUI_redraw ();
int GUI_Color_select (int *ir, int *ig, int *ib);

int GUI_CB_win_is_up ();
int GUI_CB_key (int iEv, int iKey);
int GUI_CB_mouse (int iEv, int iMb);
int GUI_CB_sele ();

int GUI_init (int argc, char** argv);
int GUI_ck_version (int vMaj, int vMin);
int GUI_get_fontSiz (int *isx, int *isy, void *win1);

// EOF
