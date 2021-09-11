# makes a.out


BIN = a.out


#OBJS = $(ALL:.c=.o)
OBJ1   = $(patsubst %.c,%.o,$(wildcard *.c))
#OBJ2   = $(patsubst %.c,%.o,$(wildcard ../tst_ut/*.c))
#OBJS  += $(patsubst %.glsl,%.o,$(wildcard *.glsl))


CFLAGS  += -std=c99 -DGL_GLEXT_PROTOTYPES
CFLAGS  += $(shell pkg-config --cflags gtk+-3.0 gl)
CPDEB = -DDEB -ggdb -pg -fstack-check -O0

LIBS  += $(shell pkg-config --libs   gtk+-3.0 gl)
LIBS  += -lm


$(BIN): $(OBJ1) $(OBJ2)
	@echo ================= link $@
	@echo "OBJ1=" $(OBJ1) "==================="
	@echo "OBJ2=" $(OBJ2) "==================="
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)


%.o: %.c
	@echo ================= compile $@
	$(CC) $(CFLAGS) $(CPDEB) -o $@ -c $^
	ctags -f tags/$(*F).tag -IMemTab --c-kinds=dfstvx --tag-relative=yes --excmd=pattern $<


# eof
