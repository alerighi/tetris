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
	@rm -f *.o
	@rm -f $(BINNAME)

install_binary: $(BINNAME)
	@echo "Installing tetris binary"
	@install -s $(BINNAME) $(BIN_DIR)

install_man: $(MAN_PAGE)
	@echo "Installing tetris manpage"
	@install -d $(MAN_DIR)
	@install $(MAN_PAGE) $(MAN_DIR)

uninstall_binary:
		@echo "Uninstalling tetris binary"
		@rm -f $(BIN_DIR)/$(BINNAME)

uninstall_man:
		@echo "Uninstalling tetris manpage"
		@rm -f $(MAN_DIR)/$(MAN_PAGE)


install: install_binary install_man

uninstall: uninstall_binary uninstall_man


rebuild: clean $(BINNAME)

help:
	@echo "This is tetris version 1.0"
	@echo "To compile the game type 'make'"
	@echo "To install the game, type 'make install'"
	@echo "To run the game whitout installing type './$(BINNAME)' or type 'make run'"
	@echo "To clean the sources eliminating all the binary file type 'make clean'"
	@echo "To completly rebuild the program type 'make rebuild'"
	@echo "To get addictional help about the program type 'man tetris' after installing the game"
