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

Run "cmake CMakeLists.txt" to configure and then "make" to build in \*nix.
In Windows in MSYS2 run "cmake --build . " to build.
