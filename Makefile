CC=gcc
CFLAGS=-O2 -Wall
LDFLAGS=-lcurses -lm
BINNAME=tetris
OBJ=tetris.o game.o screen.o

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINNAME): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm $(OBJ) $(BINNAME)
