# Tetris

Tetris game written in C with ncurses libraries. 

Check out also the [new Rust rewrite](https://github.com/alerighi/tetris-rust)!

### Installation 

Compile with `make`. Note: on Ubuntu, Debian and derivates you need to install the package `libncurses-dev`. You can the run the game with `./tetris`, or install it in `/usr/local/bin` with `sudo make install`. 

#### Instal with Meson
Alternatively, you could try to build it with `meson`, a new modern build system. To compile, run the following commands:
```bash
meson build 
cd build/
ninja
sudo ninja install # if you want to install it to /usr/local/bin
```

