# make tag-files from files ../inf/*.f

DIR_TAGS := ../tags/

SRC1 = $(shell find ../inf -type f -name "*.c")
TAG1 = $(patsubst %.c,%.tag, $(notdir $(SRC1)))

VPATH = $(DIR_TAGS):../inf/:.

makeTagfiles: $(TAG1)
	@echo inf.mak done $(TAG1)

%.tag: %.c
	@echo ================= make tag $<
	ctags -f ../tags/$(*F).tag --c-kinds=dfstvx --tag-relative=yes --excmd=pattern $<

# eof
