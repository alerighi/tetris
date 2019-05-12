project('tetris', 'c', 
	version: '1.2', 
	license: 'GPL3', 
	default_options: [
		'c_std=c11',
		'warning_level=3',
		'optimization=3',
		'strip=true',
	]
)
sources = [
	'src/tetris.c', 
	'src/screen.c',
	'src/game.c',
	'src/pieces.c', 
	'src/score.c',
]
cc = meson.get_compiler('c')
ncursess = cc.find_library('ncurses')
math = cc.find_library('m')
include_dir = include_directories('include')
executable('tetris', sources, dependencies: [ncursess, math], include_directories: include_dir, install: true) 
