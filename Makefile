CC=cc
CFLAGS=-O3 -W
LDFLAGS=-lcurses -lm
BINNAME=tetris
OBJ=tetris.o game.o screen.o
HEADERS=screen.h game.h
.PHONY : all run clean rebuild

%.o: %.c $(HEADERS)
	@echo "Compiling: $<"
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

rebuild: clean $(BINNAME)

help:
	@echo "Tetris version 1.0"
	@echo "To compile the program type \'make\'"
	@echo "To run the program type \'./tetris\' or type \'make run\'"
	@echo "To clean the sources eliminating all the binary file type \'make clean\'"
	@echo "To completly rebuild the program type \'make rebuild\'"
