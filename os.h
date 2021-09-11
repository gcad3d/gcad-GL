/* ./os.h
Operating-System-functions
*/

# define VER_LIBC_MAJ 2
# define VER_LIBC_MIN 11

int OS_system (char *buf);
int OS_sys1 (char *sOut, int sSiz, char *cmd);
int OS_checkFilExist (char* filnam, int mode);
int OS_ck_libcVersion (int vMaj, int vMin);
void OS_get_scrRes (int *xSiz_px, int *ySiz_px, int *xSiz_mm, int *ySiz_mm);

// EOF
