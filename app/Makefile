# makes a.out


gcad_dir_bin := ../bin/
VGUI := gtk3

EXENAM = a.out

CLEAN = $(shell /bin/rm -rf t1.o)

SRC1 = $(shell find . -type f -name "*.c")
SRC1 += $(shell find ../ut -type f -name "*.c")
SRC1 += $(shell find ../db -type f -name "*.c")
SRC1 += $(shell find ../dl -type f -name "*.c")
SRC1 += $(shell find ../gr -type f -name "*.c")
SRC1 += $(shell find ../gui -type f -name "*.c")
SRC1 += $(shell find ../gui_gtk3 -type f -name "*.c")
SRC1 += $(shell find ../gl -type f -name "*.c")

OBJ0 = $(patsubst %.c,%.o, $(notdir $(SRC1)))

# remove GUI_dlg1.o
OBJ1   = $(patsubst GUI_dlg1.o,,$(OBJ0))

VPATH = $(gcad_dir_bin):../ut/:../db/:../dl/:../gr/:../gui/:../gui_$(VGUI)/:../gl/:.


CFLAGS  += -std=c99 -DGL_GLEXT_PROTOTYPES
CFLAGS  += -DGL_GLEXT_PROTOTYPES
CFLAGS  += $(shell pkg-config --cflags gtk+-3.0 gl)
CPDEB = -DDEB -ggdb -pg -fstack-check -O0

LIBS  += $(shell pkg-config --libs   gtk+-3.0 gl)
LIBS  += -lm



$(EXENAM): $(OBJ1)
	@echo ================= link $@
	@echo "OBJ1=" $(OBJ1) "==================="
	cd $(gcad_dir_bin) && \
	$(CC) $(LDFLAGS) -o a.out $(OBJ1) $(LIBS)
	mv -f $(gcad_dir_bin)a.out $(EXENAM)
	make -f inf.mak makeTagfiles


srclst:
	@echo ================= srclst
	echo $(SRC1) > srcFiles


%.o: %.c
	@echo ================= compile $^ $@
	$(CC) $(CFLAGS) $(CPDEB) -o $@ -c $^
	mv -f $(@F) $(gcad_dir_bin).
	ctags -f ../tags/$(*F).tag -IMemTab --c-kinds=dfstvx --tag-relative=yes --excmd=pattern $<


# eof
