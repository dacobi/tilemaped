# Tilemaped
Simple Tilemap Editor is a tilemap editor for the Commander X16 retro computer.

(It's not currently complete, but is being worked on)

It uses C++17 and SDL2, SDL2_image and SDL2_ttf.

Apart from directory separation, there is no OS specific code.
Currently directory separation is defined as    
```
#define DIRDEL std::filesystem::path::preferred_separator    
```
I've only tested this in Linux with g++ but would assume that it works in Windows and MacOS as well.    
It should compile on any system with a working C++17 compiler and the required SDL2 libs.

A copy of Nerdfonts TTF fonts are included in the repo according to [LICENSE.nerdfonts](https://github.com/ryanoasis/nerd-fonts/blob/master/LICENSE)

To compile in \*nix simply run make or:
g++ -std=c++17 tilemaped.cpp -I/usr/include/SDL2/ -lSDL2 -lSDL2_image -lSDL2_ttf -o tilemaped

I've added a Makefile.win64 that will work in MSYS2 when the required SDL2 libs are installed.
I'm also working on a CMakeLists.txt that's currently working in MSYS2 ("cmake CMakeLists.txt && cmake --build . ")

