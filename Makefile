CC=gcc
CFLAGS=-O2 -Wall
LDFLAGS=-lcurses -lm
BINNAME=tetris
OBJ=tetris.o game.o screen.o
HEADERS=screen.h tetris.h game.h

%.o: %.c $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINNAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean: 
	rm -f *.o
	rm -f $(BINNAME)
