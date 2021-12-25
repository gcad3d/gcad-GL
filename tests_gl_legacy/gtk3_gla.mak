# makes gtk3_gla
# make -f gtk3_gla.mak && ./gtk3_gla


BIN = gtk3_gla

SRCS   = gtk3_gla.c

OBJS   = $(patsubst %.c,%.o,$(SRCS))


CFLAGS  += -std=c99 -DGL_GLEXT_PROTOTYPES

#CC = gcc -std=c99

CFLAGS  += $(shell pkg-config --cflags gtk+-3.0 gl)

LIBS  += $(shell pkg-config --libs gtk+-3.0 gl)

LIBS  += -lm -lGLU


$(BIN): $(OBJS)
	@echo ================= link $@
	@echo $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)


%.o: %.c
	@echo ================= compile $@
	$(CC) $(CFLAGS) -o $@ -c $^


# eof

