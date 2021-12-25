# makes gtk3_glx
# make -f gtk3_glx.mak && ./gtk3_glx


BIN = gtk3_glx

SRCS   = gtk3_glx.c

OBJS   = $(patsubst %.c,%.o,$(SRCS))


#CFLAGS  += -std=c99 -DGL_GLEXT_PROTOTYPES

CFLAGS  += $(shell pkg-config --cflags gtk+-3.0)

LIBS  += $(shell pkg-config --libs gtk+-3.0)

LIBS  += -lm -lX11 -lGL -lGLU


$(BIN): $(OBJS)
	@echo ================= link $@
	@echo $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)


%.o: %.c
	@echo ================= compile $@
	$(CC) $(CFLAGS) -o $@ -c $^


# eof

