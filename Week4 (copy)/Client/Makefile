.SUFFIXES:
.SUFFIXES: .c .h .o

SHELL = /bin/bash

CC 	= gcc
INCLDIR = inc
OBJDIR 	= obj
SRCDIR 	= src

CFLAGS = -I$(INCLDIR)

DEPS 	= $(wildcard $(INCLDIR)/*.h)

SRCS	= $(wildcard $(SRCDIR)/*.c)

_OBJS	= $(patsubst $(SRCDIR)/%.c,%.o,$(SRCS))
OBJS	= $(addprefix $(OBJDIR)/,$(_OBJS))

CLIENTOBJ	= $(OBJDIR)/client.o

.PHONY: 
.PHONY: all

all: $(CLIENTOBJ)

$(CLIENTOBJ): client.c $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Create file if not exits
$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~
