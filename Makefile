# Makefile for tetris
# Copyright (c) 2016-2018 - Alessandro Righi - All rights reserved
# You are free to use and modify this makefile under the terms of the MIT licence

SOURCEDIR:=src
INCLUDEDIR:=include
BUILDDIR:=build
CFLAGS:=-O3 -Wall -Wextra -pedantic -std=c11
LDFLAGS:=-lcurses
BINNAME:=tetris
SOURCES:=$(wildcard $(SOURCEDIR)/*.c)
HEADERS:=$(wildcard $(INCLUDEDIR)/*.h)
OBJECTS:=$(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))
PREFIX:=/usr/local

.PHONY: all clean install uninstall

all: $(BUILDDIR) $(BINNAME)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -I$(INCLUDEDIR) -o $@ $< $(CFLAGS)

$(BINNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^ 

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
	rm -f $(BINNAME)

install: $(BINNAME) $(MAN_PAGE)
	install -s $(BINNAME) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(BINNAME)
