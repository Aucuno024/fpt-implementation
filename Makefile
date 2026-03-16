.PHONY: all clean fclean make_dir
.SUFFIXES:
CC=gcc
SRCDIR=src
OBJDIR=obj
EXEC=
EXECDIR=bin
SRCS=$(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
CFLAGS=-Wall -g
CPPFLAGS=-Iinclude
LIBS+=

all: fclean make_dir $(EXECDIR)/$(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
        $(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(EXECDIR)/$(EXEC):  $(OBJS)
        $(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

make_dir:
        -mkdir $(OBJDIR)
        -mkdir $(EXECDIR)

clean:
        -rm -rf $(OBJDIR)

fclean:
        make clean
        -rm -rf $(EXECDIR)
