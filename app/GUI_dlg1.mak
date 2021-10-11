# Link & run:
# ./GUI_dlg1.sh
#
# DIR_DEV     home of sourcefiles, scripts, linkjobs, packages
# DIR_BIN     home of objects, executables, libraries
#


## get VGUI  - eg gtk3
#VGUI := $(shell cat ../gcad_gui_version)
VGUI := gtk3

gcad_dir_bin := ../bin/



# ../gui_gtk3/GUI_dlg1.c
# ../gui_gtk2/GUI_dlg1.c
# ../gui_gtk2_MS/GUI_dlg1.c
SRC1 =\
 ../gui_$(VGUI)/GUI_dlg1.c ../ut/deb_prt.c

# get SRCOS
SRCOS = ../ut/os_uix.c


EXENAM  = GUI_dlg1_$(VGUI)


OBJ1 = $(patsubst %.c,%.o, $(notdir $(SRC1)))
OBJOS = $(patsubst %.c,%.o, $(notdir $(SRCOS)))


# get SRCGUI GUICP GUILIB
#include ../gui/gui_$(VGUI).mak

# gtk3:
GUICP = `pkg-config --cflags gtk+-3.0` -Dgtk3
GUILIB = `pkg-config --libs gtk+-3.0`


# get debug-settings DEB CPDEB LKDEB
include deb.mak

#VPATH = $(OUTDIR):../ut/:../gui/:../gui_$(VGUI)/:.
VPATH = $(OUTDIR):../ut/:../gui_$(VGUI)/:.

CPFLG = $(CPDEB) -D$(VGUI) -Wno-implicit

# fmod dlclose
LKDEF = -lm -ldl


LKFLG = $(LKDEF) $(LKDEB)



default: $(OBJ1) $(OBJOS)
	@echo "........... build $(EXENAM) ..........."
	cd $(gcad_dir_bin) && \
	$(CC) -o a.out $(OBJ1) $(OBJOS) $(GUILIB) $(LKFLG)
	mv -f $(gcad_dir_bin)a.out $(EXENAM)
	@echo


.c.o:
	$(CC) -c $(GUICP) $(CPFLG) $<
	mv -f $(@F) $(gcad_dir_bin).
	ctags -f ../tags/$(*F).tag -IMemTab --tag-relative=yes $<

# eof

