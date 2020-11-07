.SUFFIXES:
.SUFFIXES: .c .h .o

SHELL = /bin/bash

CC 	= gcc

CLIENTOBJ	= ./Client/obj/client.o
SERVEROBJ	= ./Server/obj/server.o

.PHONY: 
.PHONY: all

all: client server

client: $(CLIENTOBJ)
	$(CC) $(CFLAGS) $< -o $@

$(CLIENTOBJ): Client
	make -C ./Client -f Makefile

server: $(SERVEROBJ)
	$(CC) $(CFLAGS) $< -o $@

$(SERVEROBJ): Server
	make -C ./Server -f Makefile
