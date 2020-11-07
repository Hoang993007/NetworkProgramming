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

SERVEROBJ	= $(OBJDIR)/server.o

.PHONY: 
.PHONY: all

all: $(SERVEROBJ) 

$(SERVER): $(OBJS) $(BINDIR) $(SERVEROBJ)
	$(CC) $(CFLAGS) $(OBJS) $(SERVEROBJ) -o $(SERVER)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVEROBJ): server.c $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~
