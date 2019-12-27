# tetris - a small ncursess tetris game
# Copyright (C) 2016-2019 Alessandro Righi
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

SOURCES=src/screen.c src/game.c src/pieces.c src/score.c
HEADERS=src/screen.h src/game.h src/pieces.h src/score.h
CFLAGS=-O2 -Wall -Wextra -pedantic -Wno-vla -std=c99 -lncurses -lm
BINNAME=tetris

$(BINNAME): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@
