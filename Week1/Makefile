default: run

# set suffix list, to prevent confusion between different make programs
# line 17 clears an implied suffix list, and line 18 sets a new one
.SUFFIXES:
.SUFFIXES: .c .h .o

# specify the shell, in case the SHELL variable is not set or is not
# inherited from the environment
SHELL = /bin/bash

# project set up and directories
CC 	= gcc
INCLDIR = inc
OBJDIR 	= obj
SRCDIR 	= src
BINDIR 	= bin

# final executable name
_BIN	= main.exe
BIN	= $(addprefix $(BINDIR)/,$(_BIN))

CFLAGS = -I$(INCLDIR)

# files; here all object files will be stored in $(OBJDIR), with the
# same base names as corresponding c files from SRCDIR
DEPS 	= $(wildcard $(INCLDIR)/*.h)

SRCS	= $(wildcard $(SRCDIR)/*.c)

_OBJS	= $(patsubst $(SRCDIR)/%.c,%.o,$(SRCS))
OBJS	= $(addprefix $(OBJDIR)/,$(_OBJS))

MAINOBJ	= $(OBJDIR)/main.o

.PHONY: 
.PHONY: all

all: $(BIN)

$(BIN): $(OBJS) $(BINDIR) $(MAINOBJ)
	$(CC) $(CFLAGS) $(OBJS) $(MAINOBJ) -o $(BIN)
#	clear

#if write $< then it will take the first file name
#It will not take all $(OBJS) but only src/accountSystem.o
#Because $(OBJS) = src/accountSystem.o menu.o

#Complie .o file
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAINOBJ): main.c $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

#Create file if not exits
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

run: $(BIN)
#	clear
	./$(BIN) $(BINDIR)/nguoidung.txt

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~
