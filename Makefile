# Makefile for tetris
# Copyright (c) 2016-2018 - Alessandro Righi - All rights reserved
# You are free to use and modify this makefile under the terms of the MIT licence

CFLAGS=-O3 -Wall -Wextra -pedantic -std=c11
LDFLAGS=-lcurses
BINNAME=tetris
OBJS=tetris.o game.o screen.o pieces.o score.o 
PREFIX=/usr/local
VERSION=1.4.0

.PHONY: all clean install uninstall

all: $(BINNAME)

tetris.o: tetris.c game.h screen.h score.h 
	$(CC) -c -o $@ $< $(CFLAGS)

screen.o: screen.c screen.h game.h score.h 
	$(CC) -c -o $@ $< $(CFLAGS)

game.o: game.c game.h screen.h pieces.h score.h 
	$(CC) -c -o $@ $< $(CFLAGS)

score.o: score.c score.h 
	$(CC) -c -o $@ $< $(CFLAGS)

pieces.o: pieces.c pieces.h 
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINNAME): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o 
	rm -f $(BINNAME)

install: $(BINNAME) $(MAN_PAGE)
	install -s $(BINNAME) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(BINNAME)
