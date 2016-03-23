# Makefile for tetris
# Copyright (c) 2016 - Alessandro Righi - All rights reserved
# You are free to use and modify this makefile under the terms of the BSD licence

CC=cc
CFLAGS=-O3 -W
LDFLAGS=-lcurses -lm
BINNAME=tetris
OBJ=tetris.o game.o screen.o
HEADERS=screen.h game.h
PREFIX=/usr/local/
BIN_DIR=$(PREFIX)/bin
MAN_DIR=$(PREFIX)/share/man/man6
MAN_PAGE=tetris.6
RM=rm -f
INSTALL=install

.PHONY: all run clean rebuild help install uninstall

%.o: %.c $(HEADERS)
	@echo "Compiling $<"
	@$(CC) -c -o $@ $< $(CFLAGS)

$(BINNAME): $(OBJ)
	@echo "Linking $(BINNAME)"
	@$(CC) -o $@ $^ $(LDFLAGS)

all: $(BINNAME)

run: $(BINNAME)
	@echo "Running $(BINNAME)"
	@./$(BINNAME)

clean:
	@echo "Cleaning sources"
	@$(RM) *.o
	@$(RM) $(BINNAME)

install_binary: $(BINNAME)
	@echo "Installing $(BINNAME) binary"
	@$(INSTALL) -s $(BINNAME) $(BIN_DIR)

install_man: $(MAN_PAGE)
	@echo "Installing $(BINNAME) manpage"
	@$(INSTALL) -d $(MAN_DIR)
	@$(INSTALL) $(MAN_PAGE) $(MAN_DIR)

uninstall_binary:
		@echo "Uninstalling $(BINNAME) binary"
		@$(RM) $(BIN_DIR)/$(BINNAME)

uninstall_man:
		@echo "Uninstalling $(BINNAME) manpage"
		@$(RM) $(MAN_DIR)/$(MAN_PAGE)

install: install_binary install_man

uninstall: uninstall_binary uninstall_man

rebuild: clean $(BINNAME)

help:
	@echo "This is $(BINNAME), version 1.0"
	@echo "To compile the game type 'make'"
	@echo "To install the game, type 'make install'"
	@echo "To run the game whitout installing type './$(BINNAME)' or type 'make run'"
	@echo "To clean the sources eliminating all the binary file type 'make clean'"
	@echo "To completly rebuild the program type 'make rebuild'"
	@echo "To get addictional help about the program type 'man $(BINNAME)' after installing the game"
