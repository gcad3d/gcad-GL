# makes gtk2_glx
# make -f gtk2_glx.mak && ./gtk2_glx


BIN = gtk2_glx

SRCS   = gtk2_glx.c

OBJS   = $(patsubst %.c,%.o,$(SRCS))


CFLAGS  += $(shell pkg-config --cflags gtk+-2.0)

LIBS  += $(shell pkg-config --libs gtk+-2.0)

LIBS  += -lm -lX11 -lGL -lGLU


$(BIN): $(OBJS)
	@echo ================= link $@
	@echo $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)


%.o: %.c
	@echo ================= compile $@
	$(CC) $(CFLAGS) -o $@ -c $^


# eof
