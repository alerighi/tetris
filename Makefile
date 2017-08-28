# Makefile for tetris
# Copyright (c) 2016 - Alessandro Righi - All rights reserved
# You are free to use and modify this makefile under the terms of the MIT licence

CC=cc
CFLAGS=-O3 -W -Wall -Wextra
LDFLAGS=-lcurses -lm
BINNAME=tetris
OBJ=tetris.o game.o screen.o tetris_pieces.o
HEADERS=screen.h game.h
PREFIX=/usr/local/
BIN_DIR=$(PREFIX)/bin
MAN_DIR=$(PREFIX)/share/man/man6
MAN_PAGE=tetris.6
RM=rm -rf
INSTALL=install
VERSION=1.1.0
ECHO=/bin/echo

.PHONY: all run clean rebuild help install uninstall

%.o: %.c $(HEADERS)
	@$(ECHO) -n "Compiling $< 	"
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

install_binary: $(BINNAME)
	@$(ECHO) -n "Installing $(BINNAME) binary 	"
	@$(INSTALL) -s $(BINNAME) $(BIN_DIR)
	@$(ECHO) "[ ok ]"


install_man: $(MAN_PAGE)
	@$(ECHO) -n "Installing $(BINNAME) manpage 	"
	@$(INSTALL) -d $(MAN_DIR)
	@$(INSTALL) $(MAN_PAGE) $(MAN_DIR)
	@$(ECHO) "[ ok ]"

uninstall_binary:
	@$(ECHO) -n "Uninstalling $(BINNAME) binary 	"
	@$(RM) $(BIN_DIR)/$(BINNAME)
	@$(ECHO) "[ ok ]"

uninstall_man:
	@$(ECHO) -n "Uninstalling $(BINNAME) manpage 	"
	@$(RM) $(MAN_DIR)/$(MAN_PAGE)
	@$(ECHO) "[ ok ]"

install: install_binary install_man

uninstall: uninstall_binary uninstall_man

rebuild: clean $(BINNAME)

help:
	@$(ECHO) "This is $(BINNAME), version $(VERSION)"
	@$(ECHO) "To compile the game type 'make'"
	@$(ECHO) "To install the game, type 'make install'"
	@$(ECHO) "To run the game whitout installing type './$(BINNAME)' or type 'make run'"
	@$(ECHO) "To clean the sources eliminating all the binary file type 'make clean'"
	@$(ECHO) "To completly rebuild the program type 'make rebuild'"
	@$(ECHO) "To get addictional help about the program type 'man $(BINNAME)' after installing the game"
