# Makefile for tetris
# Copyright (c) 2016 - Alessandro Righi - All rights reserved
# You are free to use and modify this makefile under the terms of the MIT licence

CC=cc
CFLAGS=-O3 -Wall -Wextra -pedantic
LDFLAGS=-lcurses
BINNAME=tetris
OBJ=tetris.o game.o screen.o pieces.o high_score.o 
HEADERS=screen.h game.h pieces.h high_score.h
PREFIX=/usr/local/
BIN_DIR=$(PREFIX)/bin
MAN_DIR=$(PREFIX)/share/man/man6
MAN_PAGE=tetris.6
RM=rm -rf
INSTALL=install
VERSION=1.3.0
ECHO=/bin/echo

.PHONY: all run clean rebuild help install uninstall

%.o: %.c $(HEADERS)
	@$(ECHO) -n "Compiling $<	"
	@$(CC) -c -o $@ $< $(CFLAGS)
	@$(ECHO) "[ ok ]"

$(BINNAME): $(OBJ)
	@$(ECHO) -n "Linking $(BINNAME)		"
	@$(CC) -o $@ $^ $(LDFLAGS)
	@$(ECHO) "[ ok ]"

all: $(BINNAME)

run: $(BINNAME)
	@$(ECHO) "Running $(BINNAME)"
	@./$(BINNAME)

clean:
	@$(ECHO) -n "Cleaning sources 	"
	@$(RM) *.o $(BINNAME).dSYM
	@$(RM) $(BINNAME)
	@$(ECHO) "[ ok ]"

install: $(BINNAME) $(MAN_PAGE)
	@$(ECHO) -n "Installing $(BINNAME) binary 	"
	@$(INSTALL) -s $(BINNAME) $(BIN_DIR)
	@$(ECHO) "[ ok ]"
	@$(ECHO) -n "Installing $(BINNAME) manpage 	"
	@$(INSTALL) -d $(MAN_DIR)
	@$(INSTALL) $(MAN_PAGE) $(MAN_DIR)
	@$(ECHO) "[ ok ]"

uninstall:
	@$(ECHO) -n "Uninstalling $(BINNAME) binary 	"
	@$(RM) $(BIN_DIR)/$(BINNAME)
	@$(ECHO) "[ ok ]"
	@$(ECHO) -n "Uninstalling $(BINNAME) manpage 	"
	@$(RM) $(MAN_DIR)/$(MAN_PAGE)
	@$(ECHO) "[ ok ]"

rebuild: clean $(BINNAME)

help:
	@$(ECHO) "This is $(BINNAME), version $(VERSION)"
	@$(ECHO) "To compile the game type 'make'"
	@$(ECHO) "To install the game, type 'make install'"
	@$(ECHO) "To run the game whitout installing type './$(BINNAME)' or type 'make run'"
	@$(ECHO) "To clean the sources eliminating all the binary file type 'make clean'"
	@$(ECHO) "To completly rebuild the program type 'make rebuild'"
	@$(ECHO) "To get addictional help about the program type 'man $(BINNAME)' after installing the game"
