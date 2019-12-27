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

SOURCEDIR:=src
BUILDDIR:=build
CFLAGS:=-O2 -Wall -Wextra -pedantic -Wno-vla -std=c99
LDFLAGS:=-lncurses -lm
BINNAME:=tetris
SOURCES:=$(wildcard $(SOURCEDIR)/*.c)
HEADERS:=$(wildcard $(SOURCEDIR)/*.h)
OBJECTS:=$(patsubst $(SOURCEDIR)/%.c, $(BUILDDIR)/%.o, $(SOURCES))
PREFIX:=/usr/local

.PHONY: binary clean install uninstall

binary: $(BUILDDIR) $(BINNAME)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINNAME): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
	rm -f $(BINNAME)

install: binary
	install -s $(BINNAME) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/$(BINNAME)
