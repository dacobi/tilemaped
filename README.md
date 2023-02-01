# Tilemaped
Simple Tilemap Editor is a tilemap editor for the Commander X16 retro computer.

(It's not currently complete, but is being worked on)

It uses C++17 and SDL2, SDL2_image and SDL2_ttf.

The only OS specific code is #define DIRDEL "/" so it should compile on any system with a working C++17 compiler and the required SDL2 libs.

A copy of Nerdfonts TTF fonts are included in the repo according to [LICENSE.nerdfonts](https://github.com/ryanoasis/nerd-fonts/blob/master/LICENSE)

To compile in \*nix simply run make or:
g++ tilemaped.cpp -I/usr/include/SDL2/ -lSDL2 -lSDL2_image -lSDL2_ttf -o tilemaped

