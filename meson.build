project('tetris', 'c')
sources = [
	'src/tetris.c', 
	'src/screen.c',
	'src/game.c',
	'src/pieces.c', 
	'src/score.c',
]
ncursess = dependency('curses')
include_dir = include_directories('include')
executable('tetris', sources, dependencies: ncursess, include_directories: include_dir, install: true) 
