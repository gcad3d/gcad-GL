/* gtk.h
*/

int Gtk_init (int argc, char** argv);
int Gtk_redraw ();
int Gtk_Color_select (int *ir, int *ig, int *ib);
int Gtk_get_fontSiz (int *isx, int *isy, void *win1);
int Gtk_ck_version (int vMaj, int vMin);
int Gtk_cursor__ (int iCur);
int Gtk_block_gr (int mode);

// EOF
